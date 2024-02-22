/*============================================================================*/
/* RelationalMacros.h -- Math Macros                                          */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void vRoundBcdCheckRoundUp(i8** ppaW, i1* piWasRoundedUp)
{
  union Bcd_T bcdRound;

  bcdRound.m_iAsI8 = *(--(*ppaW));

  i8 iNegAdj = 1;
  if (bcdRound.m_iMantissa < 0) {
    iNegAdj = -1;
    bcdRound.m_iMantissa *= -1;
  }

  if (bcdRound.m_iMantissa % 10 >= 5) {
    bcdRound.m_iMantissa = (bcdRound.m_iMantissa / 10) + 1;
    if (piWasRoundedUp) {
      *piWasRoundedUp = 1;
    }

  } else {
    bcdRound.m_iMantissa = bcdRound.m_iMantissa / 10;
    if (piWasRoundedUp) {
      *piWasRoundedUp = 0;
    }
  }

  bcdRound.m_iMantissa *= iNegAdj;

  bcdRound.m_uScale--;
  *(*ppaW)++ = bcdRound.m_iAsI8;
}

static inline void vRoundBcd(i8** ppaW)
{
  vRoundBcdCheckRoundUp(ppaW, NULL); // can't use nullptr when compiled as a C program
}

// Rescale a BCD on the stack to a given scale. Checks for overflows
static inline void vRescaleBcdToScale(i8** ppaW, struct Dynamic_T* aDynamicT, i8 iScale, i1* piWasRoundedUp)
{
  union Bcd_T bcdReScale1;

  bcdReScale1.m_iAsI8 = *(--(*ppaW));
  i8 iScaleDiff = iScale - (i8) bcdReScale1.m_uScale;

  if (iScaleDiff > 0) {
    // Need to check for overflow
    if ( (bcdReScale1.m_iMantissa < 0 &&
         C_BCD_MIN / aDynamicT->m_iaPow10[iScaleDiff] > bcdReScale1.m_iMantissa)
         ||
         (C_BCD_MAX / aDynamicT->m_iaPow10[iScaleDiff] < bcdReScale1.m_iMantissa)
       ) {
      aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
      return;
    }

    bcdReScale1.m_uScale = (u8) (iScale);
    bcdReScale1.m_iMantissa =
      bcdReScale1.m_iMantissa * aDynamicT->m_iaPow10[iScaleDiff];

  } else if (iScaleDiff < 0) {
    // No over/underflow check required for division.
    // Adjust BCD, then round the last digit as needed.

    if (iScaleDiff < -1) {
      bcdReScale1.m_uScale = (u8) (iScale) + 1;
      bcdReScale1.m_iMantissa =
        bcdReScale1.m_iMantissa / aDynamicT->m_iaPow10[(-iScaleDiff)-1];
    }

    /* Now round the number */
    *(*ppaW)++ = bcdReScale1.m_iAsI8;
    vRoundBcdCheckRoundUp(ppaW, piWasRoundedUp);
    bcdReScale1.m_iAsI8 = *(--(*ppaW));
  }

  *(*ppaW)++ = bcdReScale1.m_iAsI8;
}

// Rescale a decimal to a given precision and scale.
// Note: The scale may be reduced to fit the integral part of the value, if necessary.
static inline void vRescaleBcdToPrecisionScale(i8** ppaW, struct Dynamic_T* aDynamicT,
                                               i8 iPrecision, i8 iScale, i1* piWasRoundedUp)
{
  // Value should be on expression stack.
  // First rescale the value and check for any overflows
  vRescaleBcdToScale(ppaW, aDynamicT, iScale, piWasRoundedUp);

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    return;
  }

  // Now check that the value doesn't exceed the stated precision
  union Bcd_T bcdVal;
  bcdVal.m_iAsI8 = *(--(*ppaW));

  i8 iBcdPrec = iPrecision_signed(bcdVal.m_iMantissa);

  if (iBcdPrec <= iPrecision) {
    // all good
    *(*ppaW)++ = bcdVal.m_iAsI8;
    return;
  }

  // We may have a problem. Can we reduce the scale to fit the integral part?
  i8 iNewScale = iBcdPrec - iPrecision;
  if (iNewScale <= iScale) {
    *(*ppaW)++ = bcdVal.m_iAsI8;
    vRescaleBcdToPrecisionScale(ppaW, aDynamicT, iPrecision, iNewScale, piWasRoundedUp);
    return;
  }

  // Houston, we have a problem
  aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
  return;
}

// Accept two decimals and rescale the one with the smaller scale to the same
// scale as the other. Put both decimals back on stack. May give an out of bounds error.
// TODO: This should be removed as it will likely overflow one of the values
static inline void vRescaleBcdMaxOfTwo(i8** ppaW, struct Dynamic_T* aDynamicT) {
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  i8 iScaleDiff;

  bcdArg2.m_iAsI8 = *(--(*ppaW));
  bcdArg1.m_iAsI8 = *(--(*ppaW));

  if (bcdArg1.m_uScale == bcdArg2.m_uScale) {
    // Nothing to do
    *(*ppaW)++ = bcdArg1.m_iAsI8;
    *(*ppaW)++ = bcdArg2.m_iAsI8;
    return;

  } else if (bcdArg1.m_uScale > bcdArg2.m_uScale) {
    // Rescale bcdArg2 to a larger scale
    *(*ppaW)++ = bcdArg2.m_iAsI8;
    vRescaleBcdToScale(ppaW, aDynamicT, (i8) bcdArg1.m_uScale, NULL);
    bcdArg2.m_iAsI8 = *(--(*ppaW));

  } else {
    *(*ppaW)++ = bcdArg1.m_iAsI8;
    vRescaleBcdToScale(ppaW, aDynamicT, (i8) bcdArg2.m_uScale, NULL);
    bcdArg1.m_iAsI8 = *(--(*ppaW));
  }

  *(*ppaW)++ = bcdArg1.m_iAsI8;
  *(*ppaW)++ = bcdArg2.m_iAsI8;
}


// Note on comparing decimals:
// ---------------------------
// It is not possible to just rescale both decimals to a common scale
// and then compare their mantissas, as either decimal operand may overflow
// during the rescaling. We need to compare them without running the risk
// of them overflowing.
//
// The solution here is to do a two part comparison, where we know that each
// part won't overflow.
//
// First we extract out the integral (whole integer) portion of each number
// and compare them as integers. This works because SE's BCD implementation
// doesn't accept negative scale and the BCD mantissa is smaller than a
// 64 bit integer.
//
// Secondly, if both integral portions are equal then we extract out the
// fractional part and raise each to the maximum scale before they are also
// compared as integers.
//
// We then return -1, 0 or 1 depending on whether arg1 (the last argument
// pushed onto the stack) is smaller, both arguments are equal, or arg2
// (the first argument pushed onto the stack) is smaller.
//
// Note: BCD arguments cannot be NULL.
static inline i8 iCompareBcds(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  union Bcd_T bcdTmp1;
  union Bcd_T bcdTmp2;

  bcdArg2.m_iAsI8 = *(--(*ppaW));
  bcdArg1.m_iAsI8 = *(--(*ppaW));

  i8 iIntegral1;
  i8 iIntegral2;
  i8 iFractional1;
  i8 iFractional2;
  i1 bRoundedUp = false;

  *(*ppaW)++ = bcdArg1.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, 0, &bRoundedUp);
  bcdTmp1.m_iAsI8 = *(--(*ppaW));
  iIntegral1 = bcdTmp1.m_iMantissa - (bRoundedUp ? 1 : 0);

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, 0, &bRoundedUp);
  bcdTmp2.m_iAsI8 = *(--(*ppaW));
  iIntegral2 = bcdTmp2.m_iMantissa - (bRoundedUp ? 1 : 0);

  if (iIntegral1 != iIntegral2) {
    return (iIntegral1 < iIntegral2 ? -1 : 1);
  }

  // Integral parts are equal, so extract the fractional parts and compare.
  bcdTmp1.m_iAsI8 = bcdArg1.m_iAsI8;
  bcdTmp2.m_iAsI8 = bcdArg2.m_iAsI8;

  // Convert the integral parts into the same scale as their corresponding
  // decimals so we can then subtract them from their mantissas to leave
  // the fractional components.
  iIntegral1 *= aDynamicT->m_iaPow10[bcdTmp1.m_uScale];
  iIntegral2 *= aDynamicT->m_iaPow10[bcdTmp2.m_uScale];

  iFractional1 = bcdTmp1.m_iMantissa - iIntegral1;
  iFractional2 = bcdTmp2.m_iMantissa - iIntegral2;

  // Put these fractional values back into the BCDs as mantissas. Then
  // we can rescale them to the maximum scale before we compare them.
  bcdTmp1.m_iMantissa = iFractional1;
  bcdTmp2.m_iMantissa = iFractional2;

  *(*ppaW)++ = bcdTmp1.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, C_MAX_BCD_SCALE, NULL);
  bcdTmp1.m_iAsI8 = *(--(*ppaW));

  *(*ppaW)++ = bcdTmp2.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, C_MAX_BCD_SCALE, NULL);
  bcdTmp2.m_iAsI8 = *(--(*ppaW));

  if (bcdTmp1.m_iMantissa == bcdTmp2.m_iMantissa) {
    return 0;
  }

  return (bcdTmp1.m_iMantissa < bcdTmp2.m_iMantissa ? -1 : 1);
}


static inline void _EQ_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  
  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  i8 iCmp = iCompareBcds(ppaW, aDynamicT);
  *(*ppaW)++ = (iCmp == 0);
}


static inline void _GE_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  i8 iCmp = iCompareBcds(ppaW, aDynamicT);
  *(*ppaW)++ = (iCmp >= 0);
}

static inline void _GT_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  i8 iCmp = iCompareBcds(ppaW, aDynamicT);
  *(*ppaW)++ = (iCmp > 0);
}

static inline void _GTA_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }
  else if (bcdArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
    return;
  }
  else if (bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  i8 iCmp = iCompareBcds(ppaW, aDynamicT);
  *(*ppaW)++ = (iCmp > 0);
}

static inline void _LE_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  i8 iCmp = iCompareBcds(ppaW, aDynamicT);
  *(*ppaW)++ = (iCmp <= 0);
}

static inline void _LT_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  i8 iCmp = iCompareBcds(ppaW, aDynamicT);
  *(*ppaW)++ = (iCmp < 0);
}

static inline void _LTA_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }
  else if (bcdArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
    return;
  }
  else if (bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  i8 iCmp = iCompareBcds(ppaW, aDynamicT);
  *(*ppaW)++ = (iCmp < 0);
}

static inline void _NE_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  i8 iCmp = iCompareBcds(ppaW, aDynamicT);
  *(*ppaW)++ = (iCmp != 0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BOOLEAN
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 == iArg1;
  }
}

static inline void _GE_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 >= iArg1;
  }
}

static inline void _GT_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 > iArg1 ? 1 : 0;
  }
}

static inline void _GTA_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = iArg2 > iArg1 ? 1 : 0;
  }
}

static inline void _LE_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 <= iArg1;
  }
}

static inline void _LT_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 < iArg1 ? 1 : 0;
  }
}

static inline void _LTA_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = iArg2 < iArg1 ? 1 : 0;
  }
}

static inline void _NE_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 != iArg1 ? 1 : 0;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// FLOAT
#define AS_FLT(x) \
(    *(d8*)&x   )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _EQ_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;
  
  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = fltArg2.m_dFlt == fltArg1.m_dFlt ? 1 : 0;
  }
}

static inline void _GE_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
  *(*ppaW)++ = fltArg2.m_dFlt >= fltArg1.m_dFlt ? 1 : 0;
  }
}

static inline void _GT_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = fltArg2.m_dFlt > fltArg1.m_dFlt ? 1 : 0;
  }
}

static inline void _GTA_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = fltArg2.m_iAsI8 > fltArg1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LE_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = fltArg2.m_dFlt <= fltArg1.m_dFlt ? 1 : 0;
  }
}

static inline void _LTA_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = fltArg2.m_dFlt < fltArg1.m_dFlt ? 1 : 0;
  }
}

// printf("INT:  %lld ? %lld \n", fltArg2.m_iAsI8, fltArg1.m_iAsI8) ;
// printf("FLOAT1:  %g ? %g \n", fltArg2.m_dFlt, fltArg1.m_dFlt) ;
static inline void _LT_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = fltArg2.m_dFlt < fltArg1.m_dFlt ? 1 : 0;
  }
}

static inline void _NE_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  fltArg2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = fltArg2.m_dFlt != fltArg1.m_dFlt ? 1: 0;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 == iArg1;
  }
}

static inline void _GE_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 >= iArg1;
  }
}

static inline void _GT_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 > iArg1 ? 1 : 0;
  }
}

static inline void _GTA_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = iArg2 > iArg1 ? 1 : 0;
  }
}

static inline void _LE_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 <= iArg1;
  }
}

static inline void _LT_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 < iArg1 ? 1 : 0;
  }
}

static inline void _LTA_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = iArg2 < iArg1 ? 1 : 0;
  }
}

static inline void _NE_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = iArg2 != iArg1 ? 1 : 0;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DATE-TIME INTERVAL
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 == iIvlArg1;
  }
}

static inline void _GE_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
    ivlArg1.m_iAsI8 == C_DATUM_NULL ||
    ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 >= iIvlArg1;
  }
}

static inline void _GT_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 > iIvlArg1;
  }
}

static inline void _GTA_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 > iIvlArg1;
  }
}

static inline void _LE_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
     i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
     i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
     if (ivlArg1.m_xIvl.m_bIsNeg)
       iIvlArg1 = (-1) * iIvlArg1;
     if (ivlArg2.m_xIvl.m_bIsNeg)
       iIvlArg2 = (-1) * iIvlArg2;
     *(*ppaW)++ = iIvlArg2 <= iIvlArg1;
  }
}

static inline void _LT_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 < iIvlArg1;
  }
}

static inline void _LTA_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 < iIvlArg1;
  }
}

static inline void _NE_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 != iIvlArg1;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// YEAR-MONTH INTERVAL
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 == iIvlArg1;
  }
}

static inline void _GE_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 >= iIvlArg1;
  }
}

static inline void _GT_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 > iIvlArg1;
  }
}

static inline void _GTA_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 > iIvlArg1;
  }
}

static inline void _LE_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 <= iIvlArg1;
  }
}

static inline void _LT_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 < iIvlArg1;
  }
}

static inline void _LTA_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
  *(*ppaW)++ = iIvlArg2 < iIvlArg1;
  }
}

static inline void _NE_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;

  ivlArg1.m_iAsI8 = *(--(*ppaW));
  ivlArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;
    if (ivlArg1.m_xIvl.m_bIsNeg)
      iIvlArg1 = (-1) * iIvlArg1;
    if (ivlArg2.m_xIvl.m_bIsNeg)
      iIvlArg2 = (-1) * iIvlArg2;
    *(*ppaW)++ = iIvlArg2 != iIvlArg1;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TIME
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTme2.m_iAsI8 == xTme1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _NE_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTme2.m_iAsI8 != xTme1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GE_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTme2.m_iAsI8 >= xTme1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GT_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTme2.m_iAsI8 > xTme1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GTA_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (xTme1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = xTme2.m_iAsI8 > xTme1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LE_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTme2.m_iAsI8 <= xTme1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LT_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTme2.m_iAsI8 < xTme1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LTA_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;

  xTme1.m_iAsI8 = *(--(*ppaW));
  xTme2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (xTme1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = xTme2.m_iAsI8 < xTme1.m_iAsI8 ? 1 : 0;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TIMESTAMP
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      xTsp2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTsp2.m_iAsI8 == xTsp1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _NE_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      xTsp2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTsp2.m_iAsI8 != xTsp1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GE_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      xTsp2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTsp2.m_iAsI8 >= xTsp1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GT_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      xTsp2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTsp2.m_iAsI8 > xTsp1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GTA_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (xTsp1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (xTsp2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = xTsp2.m_iAsI8 > xTsp1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LE_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      xTsp2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTsp2.m_iAsI8 <= xTsp1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LT_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      xTsp2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xTsp2.m_iAsI8 < xTsp1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LTA_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  xTsp2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (xTsp1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (xTsp2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = xTsp2.m_iAsI8 < xTsp1.m_iAsI8 ? 1 : 0;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DATE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xDte2.m_iAsI8 == xDte1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _NE_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xDte2.m_iAsI8 != xDte1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GE_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
  *(*ppaW)++ = xDte2.m_iAsI8 >= xDte1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GT_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if(xDte2.m_iAsI8 == xDte1.m_iAsI8) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = xDte2.m_iAsI8 > xDte1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _GTA_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (xDte1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = xDte2.m_iAsI8 > xDte1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LE_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ =C_DATUM_NULL;
  }
  else if(xDte2.m_iAsI8 == xDte1.m_iAsI8) {
    *(*ppaW)++ = 1;
  }
  else {
    *(*ppaW)++ = xDte2.m_iAsI8 <= xDte1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LT_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ = xDte2.m_iAsI8 < xDte1.m_iAsI8 ? 1 : 0;
  }
}

static inline void _LTA_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;

  xDte1.m_iAsI8 = *(--(*ppaW));
  xDte2.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (xDte1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    *(*ppaW)++ = xDte2.m_iAsI8 < xDte1.m_iAsI8 ? 1 : 0;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// UNKNOWN
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _GT_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _GTA_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _GE_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _LT_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _LTA_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _LE_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _NE_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _EQ_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _GT_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _GTA_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _GE_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _LT_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _LTA_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _LE_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _NE_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// VARCHAR
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _EQ_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (strcmp((char*)iArg2, (char*)iArg1) == 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _GE_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (strcmp((char*)iArg2, (char*)iArg1) >= 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _GT_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (strcmp((char*)iArg2, (char*)iArg1) > 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _GTA_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
     if (strcmp((char*)iArg2, (char*)iArg1) > 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _GTA_VCHVCH_PL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (*(i8*)iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (*(i8*)iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
     if (strcmp((char*)iArg2, (char*)iArg1) > 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _LE_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (strcmp((char*)iArg2, (char*)iArg1) <= 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _LT_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (strcmp((char*)iArg2, (char*)iArg1) < 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _LTA_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    if (strcmp((char*)iArg2, (char*)iArg1) < 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _LTA_VCHVCH_PL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (*(i8*)iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = 1;
  }
  else if (*(i8*)iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = 0;
  }
  else {
    if (strcmp((char*)iArg2, (char*)iArg1) < 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

static inline void _NE_VCHVCH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (strcmp((char*)iArg2, (char*)iArg1) != 0)
      *(*ppaW)++ = 1;
    else
      *(*ppaW)++ = 0;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/