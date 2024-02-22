/*============================================================================*/
/* MathMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Utilities
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// MIN & MAX
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _MIN_I8(iArg1, iArg2)    ( (iArg1) < (iArg2) ? (iArg1) : (iArg2) )
#define _MAX_I8(iArg1, iArg2)    ( (iArg1) > (iArg2) ? (iArg1) : (iArg2) )

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Overflow detection bcd
// Assumption is that the scales of the arguments are compatible
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _OF_BCDADD                                                         \
(                                                                          \
  (bcdArg1.m_iMantissa > 0 &&                                              \
  bcdArg2.m_iMantissa > C_BCD_MAX - bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  (bcdArg1.m_iMantissa < 0 &&                                              \
  bcdArg2.m_iMantissa < C_BCD_MIN - bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_UNDERFLOW :                                                \
  (bcdArg1.m_iMantissa == 0 && bcdArg2.m_iMantissa > C_BCD_MAX) ?          \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  (bcdArg1.m_iMantissa == 0 && bcdArg2.m_iMantissa < C_BCD_MIN) ?          \
  EC_ARITHMETIC_UNDERFLOW :                                                \
  EC_NONE                                                                  \
)

#define _OF_BCDSUB                                                         \
(                                                                          \
  (bcdArg1.m_iMantissa < 0 &&                                              \
  bcdArg2.m_iMantissa > C_BCD_MAX + bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  (bcdArg1.m_iMantissa > 0 &&                                              \
  bcdArg2.m_iMantissa < C_BCD_MIN + bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_UNDERFLOW :                                                \
  (bcdArg1.m_iMantissa == 0 && bcdArg2.m_iMantissa > C_BCD_MAX) ?          \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  (bcdArg1.m_iMantissa == 0 && bcdArg2.m_iMantissa < C_BCD_MIN) ?          \
  EC_ARITHMETIC_UNDERFLOW :                                                \
  EC_NONE                                                                  \
)

// TODO: Craig
#define _OF_BCDMUL                                                         \
(                                                                          \
  EC_NONE                                                                  \
)

// TODO: Craig
#define _OF_BCDDIV                                                         \
(                                                                          \
  (bcdArg2.m_iMantissa == 0) ?                                             \
  EC_DIVIDE_BY_ZERO :                                                      \
  (bcdArg1.m_iMantissa == C_BCD_MIN &&                                     \
  bcdArg2.m_iMantissa == -1) ?                                             \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  EC_NONE                                                                  \
)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Overflow detection i8
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _OF_I8ADD                                                          \
(                                                                          \
  (iArg1 > 0 && iArg2 > C_I8_MAX - iArg1) ? EC_ARITHMETIC_OVERFLOW :       \
  (iArg1 < 0 && iArg2 < C_I8_MIN - iArg1) ? EC_ARITHMETIC_UNDERFLOW :      \
  EC_NONE                                                                  \
)

#define _OF_I8SUB                                                          \
(                                                                          \
  (iArg1 < 0 && iArg2 > C_I8_MAX + iArg1) ? EC_ARITHMETIC_OVERFLOW :       \
  (iArg1 > 0 && iArg2 < C_I8_MIN + iArg1) ? EC_ARITHMETIC_UNDERFLOW :      \
  EC_NONE                                                                  \
)

//If one number is -1 and another is INT_MIN, multiplying them we get abs(INT_MIN) which is 1 higher than INT_MAX

#define _OF_I8MUL                                                          \
(                                                                          \
  (iArg1 == 0 || iArg2 == 0) ? EC_NONE :                                   \
  (iArg1 == -1 && iArg2 == C_I8_MIN) ? EC_ARITHMETIC_OVERFLOW :            \
  (iArg1 == C_I8_MIN && iArg2 == -1) ? EC_ARITHMETIC_OVERFLOW :            \
  (iArg1 > 0 && iArg2 > C_I8_MAX / iArg1) ? EC_ARITHMETIC_OVERFLOW :       \
  (iArg1 > 0 && iArg2 < C_I8_MIN / iArg1) ? EC_ARITHMETIC_UNDERFLOW :      \
  (iArg1 < 0 && iArg2 < C_I8_MAX / iArg1) ? EC_ARITHMETIC_OVERFLOW :       \
  (iArg1 < 0 && iArg2 > C_I8_MIN / iArg1) ? EC_ARITHMETIC_UNDERFLOW :      \
  EC_NONE                                                                  \
)

//For division (except for the INT_MIN and -1 special case), there isn't any possibility of going over INT_MIN or INT_MAX
#define _OF_I8DIV                                                          \
(                                                                          \
  (iArg1 == C_I8_MIN && iArg2 == -1) ? EC_ARITHMETIC_OVERFLOW :            \
  EC_NONE                                                                  \
)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _ADD_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg2;
  union Bcd_T bcdArg1;

  bcdArg2.m_iAsI8 = *(--(*ppaW));
  bcdArg1.m_iAsI8 = *(--(*ppaW));

  // Check for nulls
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 iPrecMax = C_MAX_BCD_PRECISION;

  // Extract the precision and scale of each operand
  i8 iPrecArg1  = iPrecision_signed(bcdArg1.m_iMantissa);
  i8 iPrecArg2  = iPrecision_signed(bcdArg2.m_iMantissa);
  i8 iScaleArg1 = (i8) bcdArg1.m_uScale;
  i8 iScaleArg2 = (i8) bcdArg2.m_uScale;

  // Num integral digits in each operand
  i8 iIntegralPrec1 = iPrecArg1 - iScaleArg1;
  i8 iIntegralPrec2 = iPrecArg2 - iScaleArg2;

  // Calc theoretical precision & scale of the operation
  i8 iPrecOp  =   _MAX_I8(iScaleArg1, iScaleArg2)
                + _MAX_I8(iIntegralPrec1, iIntegralPrec2)
                + 1;
  i8 iScaleOp = _MAX_I8(iScaleArg1, iScaleArg2);

//  printf("\n");
//  printf("bcdArg1 mantissa = %lld\n", bcdArg1.m_iMantissa);
//  printf("bcdArg2 mantissa = %lld\n", bcdArg2.m_iMantissa);
//  printf("iPrecArg1 = %lld\n", iPrecArg1);
//  printf("iPrecArg2 = %lld\n", iPrecArg2);
//  printf("iScaleArg1 = %lld\n", iScaleArg1);
//  printf("iScaleArg2 = %lld\n", iScaleArg2);
//  printf("iIntegralPrec1 = %lld\n", iIntegralPrec1);
//  printf("iIntegralPrec2 = %lld\n", iIntegralPrec2);
//  printf("iPrecOp = %lld\n", iPrecOp);
//  printf("iScaleOp = %lld\n", iScaleOp);

  // How many digits in the result?
  // We add +1 because an overflow to the next digit could occur
  i8 iIntegralPrecResult = _MAX_I8(iIntegralPrec1, iIntegralPrec2) + 1;

//  printf("iIntegralPrecResult = %lld\n", iIntegralPrecResult);

  // This is an early definitive check to see if the result won't fit.
  // We've already added 1 to the integral precision for a potential
  // overflow to the next digit, and here we are checking if this value is
  // also greater than 1 more of the maximum supported precision. If so
  // then the value will never fit, regardless of whether the potential
  // overflow occurs or not.
  if (iIntegralPrecResult > iPrecMax + 1) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // Now we need to work out the *actual* number of integral digits
  // in the result. We will use this to do a definite check for
  // precision overflow, as well as determine the biggest
  // scale we can support for this result before any scale rounding.

  // We need to determine if the operation will cause an overflow
  // to another digit of precision. e.g. The result of 9.8 + 0.2
  // will overflow from 2 to 3 digits of precision whereas the
  // result of 9.8 + 0.1 will remain with 2 digits of precision.

  // To check for this potential integral overflow we first
  // rescale all args to the smaller of the two scales. Then
  // we subtract arg1 from the smallest value with
  // iIntegralResult digits and compare this to arg2.
  // All values need to be in the same scale.

  i8 iSmallerScale = _MIN_I8(iScaleArg1, iScaleArg2);

  union Bcd_T bcdUpperBound;
  union Bcd_T bcdTemp1;
  union Bcd_T bcdTemp2;
  i1 iRoundedUp = 0;

  bcdUpperBound.m_iSpare = 0;
  bcdUpperBound.m_uScale = 0;
  bcdUpperBound.m_iMantissa = aDynamicT->m_iaPow10[iIntegralPrecResult - 1];
  *(*ppaW)++ = bcdUpperBound.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iSmallerScale, &iRoundedUp);
  bcdUpperBound.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg1.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iSmallerScale, &iRoundedUp);
  bcdTemp1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iSmallerScale, &iRoundedUp);
  bcdTemp2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

//  printf("bcdUpperBound = %lld\n", bcdUpperBound.m_iMantissa);
//  printf("bcdTemp1 = %lld\n", bcdTemp1.m_iMantissa);
//  printf("bcdTemp2 = %lld\n", bcdTemp2.m_iMantissa);

  if (bcdUpperBound.m_iMantissa > abs(bcdTemp1.m_iMantissa + bcdTemp2.m_iMantissa)) {
    // No overflow will occur.
    // The actual integral digits is one less
    --iIntegralPrecResult;
  }

//  printf("iIntegralPrecResult = %lld\n", iIntegralPrecResult);


  // We now know exactly how many integral digits we need in the result,
  // so we can now do a hard check for precision overflow
  if (iIntegralPrecResult > iPrecMax) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // And we can now calculate the scale of the result,
  // and any rounding/truncation that may be required.
  // The scale is the smaller of the theoretical operation
  // scale, and the max precision minus the num digits in the
  // integral part (accounting for possible overflow).
  //
  // (basically we will reduce the scale to fit all of the digits
  // in the integral result if necessary).

  i8 iScaleResult = _MIN_I8(iScaleOp, iPrecMax - iIntegralPrecResult);
  if (iScaleResult > C_MAX_BCD_SCALE) {
    iScaleResult = C_MAX_BCD_SCALE;
  }

//  printf("iScaleResult = %lld\n", iScaleResult);

  // When we calculate the result we will convert and round
  // the arguments to have a scale of iScaleResult before
  // they are added. When they are rounded to this scale it
  // is possible that both values are rounded up, which will
  // give a final result that is off by 1. So we need to adjust
  // for that.

  i1 iRoundedUp1 = 0;
  i1 iRoundedUp2 = 0;

  // Now we can scale and round the arguments to the desired scale
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iScaleResult, &iRoundedUp1);
  bcdArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iScaleResult, &iRoundedUp2);
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

//  printf("bcdArg1 = %lld\n", bcdArg1.m_iMantissa);
//  printf("bcdArg2 = %lld\n", bcdArg2.m_iMantissa);
//  printf("\n\n");

  // We've already checked that this won't overflow
  bcdArg1.m_iMantissa =   bcdArg1.m_iMantissa
                        + bcdArg2.m_iMantissa
                        - (iRoundedUp1 & iRoundedUp2);
  *(*ppaW)++ = bcdArg1.m_iAsI8;
}

static inline void _DIV_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  union Flt_T fltArg1;

  bcdArg2.m_iAsI8 = *(--(*ppaW));
  bcdArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode!= EC_NONE ||
    bcdArg1.m_iAsI8 == C_DATUM_NULL ||
    bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (bcdArg1.m_uScale != bcdArg2.m_uScale) {
      *(*ppaW)++ = bcdArg1.m_iAsI8;
      *(*ppaW)++ = bcdArg2.m_iAsI8;
      vRescaleBcdMaxOfTwo(ppaW, aDynamicT);
      bcdArg2.m_iAsI8 = *(--(*ppaW));
      bcdArg1.m_iAsI8 = *(--(*ppaW));
    }

    if(aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      aDynamicT->m_iErrorCode = _OF_BCDDIV;
      if(aDynamicT->m_iErrorCode != EC_NONE) {
        *(*ppaW)++ = C_DATUM_NULL;
      }
      else {
        fltArg1.m_dFlt = ((d8)bcdArg1.m_iMantissa) / ((d8)bcdArg2.m_iMantissa);
        *(*ppaW)++ = bcdArg1.m_uScale + 1;
        *(*ppaW)++ = fltArg1.m_iAsI8;

        vFltToBcdToScale(ppaW, aDynamicT);
        vRoundBcd(ppaW);

        if(aDynamicT->m_iErrorCode != EC_NONE) {
          --(*ppaW);
          *(*ppaW)++ = C_DATUM_NULL;
        }
      }
    }
  }
}

static inline void _MUL_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  i8 iScale;

  bcdArg2.m_iAsI8 = *(--(*ppaW));
  bcdArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    aDynamicT->m_iErrorCode = _OF_BCDMUL;

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      i8 iScaleNew = bcdArg1.m_uScale > bcdArg2.m_uScale ? bcdArg1.m_uScale : bcdArg2.m_uScale;
      i8 iResult = bcdArg1.m_iMantissa * bcdArg2.m_iMantissa;
      iScale = bcdArg1.m_uScale + bcdArg2.m_uScale;
      if (iScale != iScaleNew) {
        bcdArg1.m_iMantissa = iResult / aDynamicT->m_iaPow10[iScale - iScaleNew - 1];
        bcdArg1.m_uScale = iScaleNew + 1;
        *(*ppaW)++ = bcdArg1.m_iAsI8;
        vRoundBcd(ppaW);
      }
      else {
        bcdArg1.m_iMantissa = iResult;
        bcdArg1.m_uScale = iScaleNew;
       *(*ppaW)++ = bcdArg1.m_iAsI8;
       }
    }
  }
}

static inline void _NEG_BCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    bcdArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    bcdArg1.m_iMantissa = - bcdArg1.m_iMantissa;
    *(*ppaW)++ = bcdArg1.m_iAsI8;
  }
}

static inline void _SUB_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg2;
  union Bcd_T bcdArg1;

  bcdArg2.m_iAsI8 = *(--(*ppaW));
  bcdArg1.m_iAsI8 = *(--(*ppaW));

  // Check for nulls
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 iPrecMax = 5;
//  const i8 iPrecMax = C_MAX_BCD_PRECISION;

  // Extract the precision and scale of each operand
  i8 iPrecArg1  = iPrecision_signed(bcdArg1.m_iMantissa);
  i8 iPrecArg2  = iPrecision_signed(bcdArg2.m_iMantissa);
  i8 iScaleArg1 = (i8) bcdArg1.m_uScale;
  i8 iScaleArg2 = (i8) bcdArg2.m_uScale;

  // Num integral digits in each operand
  i8 iIntegralPrec1 = iPrecArg1 - iScaleArg1;
  i8 iIntegralPrec2 = iPrecArg2 - iScaleArg2;

  // Calc theoretical precision & scale of the operation
  i8 iPrecOp  =   _MAX_I8(iScaleArg1, iScaleArg2)
                + _MAX_I8(iIntegralPrec1, iIntegralPrec2)
                + 1;
  i8 iScaleOp = _MAX_I8(iScaleArg1, iScaleArg2);

//  printf("\n");
//  printf("bcdArg1 mantissa = %lld\n", bcdArg1.m_iMantissa);
//  printf("bcdArg2 mantissa = %lld\n", bcdArg2.m_iMantissa);
//  printf("iPrecArg1 = %lld\n", iPrecArg1);
//  printf("iPrecArg2 = %lld\n", iPrecArg2);
//  printf("iScaleArg1 = %lld\n", iScaleArg1);
//  printf("iScaleArg2 = %lld\n", iScaleArg2);
//  printf("iIntegralPrec1 = %lld\n", iIntegralPrec1);
//  printf("iIntegralPrec2 = %lld\n", iIntegralPrec2);
//  printf("iPrecOp = %lld\n", iPrecOp);
//  printf("iScaleOp = %lld\n", iScaleOp);

  // How many digits in the result?
  // We add +1 because an overflow to the next digit could occur
  i8 iIntegralPrecResult = _MAX_I8(iIntegralPrec1, iIntegralPrec2) + 1;

//  printf("iIntegralPrecResult = %lld\n", iIntegralPrecResult);

  // This is an early definitive check to see if the result won't fit.
  // We've already added 1 to the integral precision for a potential
  // overflow to the next digit, and here we are checking if this value is
  // also greater than 1 more of the maximum supported precision. If so
  // then the value will never fit, regardless of whether the potential
  // overflow occurs or not.
  if (iIntegralPrecResult > iPrecMax + 1) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // Now we need to work out the *actual* number of integral digits
  // in the result. We will use this to do a definite check for
  // precision overflow, as well as determine the biggest
  // scale we can support for this result before any scale rounding.

  // We need to determine if the operation will cause an overflow
  // to another digit of precision. e.g. The result of 9.8 + 0.2
  // will overflow from 2 to 3 digits of precision whereas the
  // result of 9.8 + 0.1 will remain with 2 digits of precision.

  // To check for this potential integral overflow we first
  // rescale all args to the smaller of the two scales. Then
  // we subtract arg1 from the smallest value with
  // iIntegralResult digits and compare this to arg2.
  // All values need to be in the same scale.

  i8 iSmallerScale = _MIN_I8(iScaleArg1, iScaleArg2);

  union Bcd_T bcdUpperBound;
  union Bcd_T bcdTemp1;
  union Bcd_T bcdTemp2;
  i1 iRoundedUp = 0;

  bcdUpperBound.m_iSpare = 0;
  bcdUpperBound.m_uScale = 0;
  bcdUpperBound.m_iMantissa = aDynamicT->m_iaPow10[iIntegralPrecResult - 1];
  *(*ppaW)++ = bcdUpperBound.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iSmallerScale, &iRoundedUp);
  bcdUpperBound.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg1.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iSmallerScale, &iRoundedUp);
  bcdTemp1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iSmallerScale, &iRoundedUp);
  bcdTemp2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

//  printf("bcdUpperBound = %lld\n", bcdUpperBound.m_iMantissa);
//  printf("bcdTemp1 = %lld\n", bcdTemp1.m_iMantissa);
//  printf("bcdTemp2 = %lld\n", bcdTemp2.m_iMantissa);

  // if (upperbound > abs(bcd1 - bcd2))
  if (bcdUpperBound.m_iMantissa > abs(bcdTemp1.m_iMantissa - bcdTemp2.m_iMantissa)) {
    // No overflow will occur.
    // The actual integral digits is one less
    --iIntegralPrecResult;
  }

//  printf("iIntegralPrecResult = %lld\n", iIntegralPrecResult);


  // We now know exactly how many integral digits we need in the result,
  // so we can now do a hard check for precision overflow
  if (iIntegralPrecResult > iPrecMax) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // And we can now calculate the scale of the result,
  // and any rounding/truncation that may be required.
  // The scale is the smaller of the theoretical operation
  // scale, and the max precision minus the num digits in the
  // integral part (accounting for possible overflow).
  //
  // (basically we will reduce the scale to fit all of the digits
  // in the integral result if necessary).

  i8 iScaleResult = _MIN_I8(iScaleOp, iPrecMax - iIntegralPrecResult);
  if (iScaleResult > C_MAX_BCD_SCALE) {
    iScaleResult = C_MAX_BCD_SCALE;
  }

//  printf("iScaleResult = %lld\n", iScaleResult);

  // When we calculate the result we will convert and round
  // the arguments to have a scale of iScaleResult before
  // they are subtracted. When they are rounded to this scale it
  // is possible that both values are rounded up, which will
  // give a final result that is off by 1. So we need to adjust
  // for that.

  i1 iRoundedUp1 = 0;
  i1 iRoundedUp2 = 0;

  // Now we can scale and round the arguments to the desired scale
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iScaleResult, &iRoundedUp1);
  bcdArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = bcdArg2.m_iAsI8;
  vRescaleBcdToScale(ppaW, aDynamicT, iScaleResult, &iRoundedUp2);
  bcdArg2.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

//  printf("bcdArg1 = %lld\n", bcdArg1.m_iMantissa);
//  printf("bcdArg2 = %lld\n", bcdArg2.m_iMantissa);
//  printf("\n\n");

  // We've already checked that this won't overflow
  bcdArg1.m_iMantissa =   bcdArg1.m_iMantissa
                        - bcdArg2.m_iMantissa
                        + (iRoundedUp1 & iRoundedUp2);
  *(*ppaW)++ = bcdArg1.m_iAsI8;
}

static inline void _SUM_BCDBCD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  bcdArg2.m_iAsI8 = *(--(*ppaW));
  bcdArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;

  } else if (bcdArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = bcdArg2.m_iAsI8;

  } else if (bcdArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = bcdArg1.m_iAsI8;

  } else {
    if (bcdArg1.m_uScale != bcdArg2.m_uScale) {
      *(*ppaW)++ = bcdArg1.m_iAsI8;
      *(*ppaW)++ = bcdArg2.m_iAsI8;
      vRescaleBcdMaxOfTwo(ppaW, aDynamicT);
      bcdArg2.m_iAsI8 = *(--(*ppaW));
      bcdArg1.m_iAsI8 = *(--(*ppaW));
   }

    if(aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    } else {
       aDynamicT->m_iErrorCode = _OF_BCDADD;
      if(aDynamicT->m_iErrorCode != EC_NONE) {
        *(*ppaW)++ = C_DATUM_NULL;
      } else {
        bcdArg1.m_iMantissa = bcdArg1.m_iMantissa + bcdArg2.m_iMantissa;
        *(*ppaW)++ = bcdArg1.m_iAsI8;
      }
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DATE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Convert to a valid date
#define _CONVERT_TO_VALID_DATE \
{                              \
  if (iMonth <= 0) {                                                       \
    iMonth += 12;                                                          \
    iYearDelta--;                                                          \
  }                                                                        \
  if (iMonth > 12) {                                                       \
    iYearDelta = iYearDelta+iMonth/12;                                     \
    iMonth = iMonth%12;                                                    \
  }                                                                        \
  i8 iYear = iYearDelta + C_YEAR_BASE;                                     \
  i8 bIsLeapYear = ( (iYear % 4 == 0) && (iYear % 100 != 0) ) || (iYear % 400 == 0); \
  if (bIsLeapYear) {                                                       \
    if (iDay > iDaysPerMonthLeap[iMonth]) {                                \
      iDay = iDaysPerMonthLeap[iMonth];                                    \
    }                                                                      \
  }                                                                        \
  else {                                                                   \
    if (iDay > iDaysPerMonth[iMonth]) {                                    \
      iDay = iDaysPerMonth[iMonth];                                        \
    }                                                                      \
  }                                                                        \
}

static inline void _ADD_DTEIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Dte_T xDte1;
  i8 iArg2;
  union Tsp_T xTsp2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xDte1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

    i8 iSecs = iArg2 / _C_1M;
    struct tm xDateTime = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime.tm_year = xDte1.m_uYearDelta;
    xDateTime.tm_mon  = xDte1.m_uMonth - 1;
    xDateTime.tm_mday = xDte1.m_uDay + (iSecs / C_SECS_PER_DAY);
    xDateTime.tm_hour = 0;
    xDateTime.tm_min  = 0;
    xDateTime.tm_sec  = iSecs % C_SECS_PER_DAY;

    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
		time_t iSecondsSincePosixEpoch = timegm(&xDateTime);

    i8 iMicroSeconds = iArg2%_C_1M;

		if (iMicroSeconds < 0) {
      xDateTime.tm_sec--;
      iMicroSeconds = _C_1M + iMicroSeconds;
		}

    if (xDateTime.tm_year > (i8) C_YEAR_RANGE ||
        xDateTime.tm_year < (i8) -C_YEAR_RANGE ||
        iSecondsSincePosixEpoch == -1) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }

    xTsp2.m_iAsI8 = 0;

    xTsp2.m_uYearDelta = xDateTime.tm_year;
    xTsp2.m_uMonth     = xDateTime.tm_mon + 1;
    xTsp2.m_uDay       = xDateTime.tm_mday;
    xTsp2.m_uHour      = xDateTime.tm_hour;
    xTsp2.m_uMinute    = xDateTime.tm_min;
    xTsp2.m_uSecond    = xDateTime.tm_sec;
    xTsp2.m_uMicro     = iMicroSeconds;

    *(*ppaW)++ = xTsp2.m_iAsI8;
  }
}

static inline void _ADD_DTEIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Dte_T xDte1;
  union Dte_T xDte2;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xDte1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;

    i8 iYear_arg2 = iArg2/12;
    i8 iMonth_arg2 = iArg2%12;

    if (ivlArg2.m_xIvl.m_bIsNeg) {
      iYear_arg2 = -1 * iYear_arg2;
      iMonth_arg2 = (-1) * iMonth_arg2;
    }

    i8 iYearDelta;
    i8 iMonth;
    i8 iDay;
    iYearDelta = xDte1.m_uYearDelta + iYear_arg2;
    iMonth = xDte1.m_uMonth + iMonth_arg2;
    iDay = xDte1.m_uDay;

    if (iYearDelta > (i8) C_YEAR_RANGE) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }
    else {
      _CONVERT_TO_VALID_DATE
      xDte2.m_uYearDelta = iYearDelta;
      xDte2.m_uMonth = iMonth;
      xDte2.m_uDay = iDay;
      xDte2.m_uSpare = 0;
      *(*ppaW)++ = xDte2.m_iAsI8;
    }
 }
}

static inline void _ADD_IVDDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte2;
  union Ivl_T ivlArg1;
  i8 iArg1;
  union Tsp_T xTsp2;

  xDte2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte2.m_iAsI8 == C_DATUM_NULL ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;

    i8 iSecs = iArg1 / _C_1M;
    struct tm xDateTime = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime.tm_year = xDte2.m_uYearDelta;
    xDateTime.tm_mon  = xDte2.m_uMonth - 1;
    xDateTime.tm_mday = xDte2.m_uDay + (iSecs / C_SECS_PER_DAY);
    xDateTime.tm_hour = 0;
    xDateTime.tm_min  = 0;
    xDateTime.tm_sec  = iSecs % C_SECS_PER_DAY;

    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch = timegm(&xDateTime);

    i8 iMicroSeconds = iArg1%_C_1M;

		if (iMicroSeconds < 0) {
      xDateTime.tm_sec--;
      iMicroSeconds = _C_1M + iMicroSeconds;
    }

    if (xDateTime.tm_year > (i8) C_YEAR_RANGE ||
        xDateTime.tm_year < (i8) -C_YEAR_RANGE ||
        iSecondsSincePosixEpoch == -1) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }

    xTsp2.m_iAsI8 = 0;
    xTsp2.m_uYearDelta = xDateTime.tm_year;
    xTsp2.m_uMonth     = xDateTime.tm_mon + 1;
    xTsp2.m_uDay       = xDateTime.tm_mday;
    xTsp2.m_uHour      = xDateTime.tm_hour;
    xTsp2.m_uMinute    = xDateTime.tm_min;
    xTsp2.m_uSecond    = xDateTime.tm_sec;
    xTsp2.m_uMicro     = iMicroSeconds;

    *(*ppaW)++ = xTsp2.m_iAsI8;
  }
}

static inline void _ADD_IVYDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;
  union Ivl_T ivlArg1;
  i8 iArg1;

  xDte2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte2.m_iAsI8 == C_DATUM_NULL ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;

    i8 iYear_arg1 = iArg1/12;
    i8 iMonth_arg1 = iArg1%12;

    if (ivlArg1.m_xIvl.m_bIsNeg) {
      iYear_arg1 = (-1) * iYear_arg1;
      iMonth_arg1 = (-1) * iMonth_arg1;
    }

    i8 iYearDelta;
    i8 iMonth;
    i8 iDay;
    iYearDelta = xDte2.m_uYearDelta + iYear_arg1;
    iMonth = xDte2.m_uMonth + iMonth_arg1;
    iDay = xDte2.m_uDay;

    if (iYearDelta > (i8) C_YEAR_RANGE) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }
    else {
      _CONVERT_TO_VALID_DATE
      xDte1.m_uYearDelta = iYearDelta;
      xDte1.m_uMonth = iMonth;
      xDte1.m_uDay = iDay;
      xDte1.m_uSpare = 0;
      *(*ppaW)++ = xDte1.m_iAsI8;
    }
 }
}

static inline void _SUB_DTEDTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Dte_T xDte2;
  union Ivl_T wIvlResult;

  xDte2.m_iAsI8 = *(--(*ppaW));
  xDte1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      xDte2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    struct tm xDateTime1 = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime1.tm_year = xDte1.m_uYearDelta;
    xDateTime1.tm_mon  = xDte1.m_uMonth - 1;
    xDateTime1.tm_mday = xDte1.m_uDay;
    xDateTime1.tm_hour = 0;
    xDateTime1.tm_min  = 0;
    xDateTime1.tm_sec  = 0;

    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch1 = timegm(&xDateTime1);

    struct tm xDateTime2 = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime2.tm_year = xDte2.m_uYearDelta;
    xDateTime2.tm_mon  = xDte2.m_uMonth - 1;
    xDateTime2.tm_mday = xDte2.m_uDay;
    xDateTime2.tm_hour = 0;
    xDateTime2.tm_min  = 0;
    xDateTime2.tm_sec  = 0;

    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch2 = timegm(&xDateTime2);

    i8 iSeconds = iSecondsSincePosixEpoch1 - iSecondsSincePosixEpoch2;

    wIvlResult.m_iAsI8 = 0;
    wIvlResult.m_xIvl.m_eType = IVL_DAY;

    if (iSeconds < 0) {
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  -1 * iSeconds * _C_1M;
      wIvlResult.m_xIvl.m_bIsNeg = 1;
    } else {
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  iSeconds*_C_1M;
      wIvlResult.m_xIvl.m_bIsNeg = 0;
    }

    *(*ppaW)++ = wIvlResult.m_iAsI8;
  }
}

static inline void _SUB_DTEIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Dte_T xDte1;
  i8 iArg2;
  union Tsp_T xTsp2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xDte1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;

    if ( ! ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

    i8 iSecs = iArg2 / _C_1M;
    struct tm xDateTime = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime.tm_year = xDte1.m_uYearDelta;
    xDateTime.tm_mon  = xDte1.m_uMonth - 1;
    xDateTime.tm_mday = xDte1.m_uDay + (iSecs / C_SECS_PER_DAY);
    xDateTime.tm_hour = 0;
    xDateTime.tm_min  = 0;
    xDateTime.tm_sec  = iSecs % C_SECS_PER_DAY;

    i8 iMicroSeconds = iArg2%_C_1M;

    if (iMicroSeconds < 0) {
      xDateTime.tm_sec--;
      iMicroSeconds = _C_1M + iMicroSeconds;
    }

    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch = timegm(&xDateTime);

    if (xDateTime.tm_year > (i8) C_YEAR_RANGE ||
        xDateTime.tm_year < (i8) -C_YEAR_RANGE ||
        iSecondsSincePosixEpoch == -1) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }

    xTsp2.m_iAsI8 = 0;
    xTsp2.m_uYearDelta = xDateTime.tm_year;
    xTsp2.m_uMonth     = xDateTime.tm_mon + 1;
    xTsp2.m_uDay       = xDateTime.tm_mday;
    xTsp2.m_uHour      = xDateTime.tm_hour;
    xTsp2.m_uMinute    = xDateTime.tm_min;
    xTsp2.m_uSecond    = xDateTime.tm_sec;
    xTsp2.m_uMicro     = iMicroSeconds;

    *(*ppaW)++ = xTsp2.m_iAsI8;
  }
}

static inline void _SUB_DTEIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Dte_T xDte1;
  union Dte_T xDte2;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xDte1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xDte1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;

  } else {
    iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;

    i8 iYear_arg2 = iArg2/12;
    i8 iMonth_arg2 = iArg2%12;

    if (ivlArg2.m_xIvl.m_bIsNeg) {
      iYear_arg2 = (-1) * iYear_arg2;
      iMonth_arg2 = (-1) * iMonth_arg2;
    }

    i8 iYearDelta;
    i8 iMonth;
    i8 iDay;
    iYearDelta = xDte1.m_uYearDelta - iYear_arg2;
    iMonth = xDte1.m_uMonth - iMonth_arg2;
    iDay = xDte1.m_uDay;

    if (iYearDelta > (i8) C_YEAR_RANGE) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;

    } else {
      _CONVERT_TO_VALID_DATE
      xDte2.m_uYearDelta = iYearDelta;
      xDte2.m_uMonth = iMonth;
      xDte2.m_uDay = iDay;
      xDte1.m_uSpare = 0;
      *(*ppaW)++ = xDte2.m_iAsI8;
    }
  }
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// FLOAT
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


static inline void _ADD_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg2.m_iAsI8 = *(--(*ppaW));
  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    aDynamicT->m_iErrorCode = EC_NONE;
    if(aDynamicT->m_iErrorCode != EC_NONE)
    {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      if(fltArg1.m_dFlt >= C_FLT_MAX ||
        fltArg2.m_dFlt >= C_FLT_MAX) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
        *(*ppaW)++ = C_DATUM_NULL;
      }
      else if(fltArg1.m_dFlt <  (d8)-C_FLT_MAX ||
        fltArg2.m_dFlt < (d8)-C_FLT_MAX) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
          *(*ppaW)++ = C_DATUM_NULL;
      }
      else
      {
        fltArg1.m_dFlt += fltArg2.m_dFlt;
        if(fltArg1.m_dFlt > C_FLT_MAX) {
          aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
          *(*ppaW)++ = C_DATUM_NULL;
        }
        else
          *(*ppaW)++ = fltArg1.m_iAsI8;
      }
    }
  }
}

static inline void _SUM_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg2.m_iAsI8 = *(--(*ppaW));
  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = fltArg2.m_iAsI8;
  }
  else if (fltArg2.m_iAsI8 == C_DATUM_NULL) {
     *(*ppaW)++ = fltArg1.m_iAsI8;
  }
  else {
    if (fltArg1.m_dFlt >= C_FLT_MAX ||
        fltArg2.m_dFlt >= C_FLT_MAX) {
      aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else if (fltArg1.m_dFlt <  (d8)C_FLT_MIN ||
      fltArg2.m_dFlt < (d8)C_FLT_MIN) {
      aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      fltArg1.m_dFlt += fltArg2.m_dFlt;
      if(fltArg1.m_dFlt > C_FLT_MAX) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
        *(*ppaW)++ = C_DATUM_NULL;
      }
      else
        *(*ppaW)++ = fltArg1.m_iAsI8;
    }
  }
}

static inline void _DIV_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg2.m_iAsI8 = *(--(*ppaW));
  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (fltArg2.m_iAsI8 == 0) {
      *(*ppaW)++ = C_DATUM_NULL;
      aDynamicT->m_iErrorCode = EC_DIVIDE_BY_ZERO;
    }
    else {
      if(fltArg1.m_dFlt > C_FLT_MAX ||
        fltArg2.m_dFlt > C_FLT_MAX) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
        *(*ppaW)++ = C_DATUM_NULL;
      }
      else if(fltArg1.m_dFlt <  C_FLT_MIN ||
        fltArg2.m_dFlt < C_FLT_MIN) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
        *(*ppaW)++ = C_DATUM_NULL;
      }
      else {
        fltArg1.m_dFlt /= fltArg2.m_dFlt;
        *(*ppaW)++ = fltArg1.m_iAsI8;
      }
    }
  }
}

static inline void _NEG_FLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    fltArg1.m_dFlt = (double)(-1)*fltArg1.m_dFlt;
    *(*ppaW)++ = fltArg1.m_iAsI8;
  }
}

static inline void _SUB_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg2.m_iAsI8 = *(--(*ppaW));
  fltArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if(fltArg1.m_dFlt > C_FLT_MAX ||
      fltArg2.m_dFlt > C_FLT_MAX) {
      aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else if(fltArg1.m_dFlt <  C_FLT_MIN ||
    fltArg2.m_dFlt < C_FLT_MIN) {
      aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      fltArg1.m_dFlt -= fltArg2.m_dFlt;
      *(*ppaW)++ = fltArg1.m_iAsI8;
    }
  }
}


static inline void _MUL_FLTFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;

  fltArg2.m_iAsI8 = *(--(*ppaW));
  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL ||
    fltArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if(fltArg1.m_dFlt > C_FLT_MAX ||
      fltArg2.m_dFlt > C_FLT_MAX) {
      aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else if(fltArg1.m_dFlt <  (d8)-C_FLT_MAX ||
    fltArg2.m_dFlt < (d8)-C_FLT_MAX) {
      aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      fltArg1.m_dFlt *= fltArg2.m_dFlt;
      if(fltArg1.m_dFlt > C_FLT_MAX) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
        *(*ppaW)++ = C_DATUM_NULL;
      }
      else
      if(fltArg1.m_dFlt < C_FLT_MIN) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
        *(*ppaW)++ = C_DATUM_NULL;
      }
      else
        *(*ppaW)++ = fltArg1.m_iAsI8 ;
    }
  }
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// inc arg1
static inline void _INC_INT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ =  ++iArg1;
  }
}

// inc arg1 if arg2 is not null
static inline void _INC_IFNOTNULL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode == EC_NONE &&
      iArg2 != C_DATUM_NULL) {
    *(*ppaW)++ = ++iArg1;
  }
  else {
    *(*ppaW)++ =  iArg1;
  }
}

static inline void _ADD_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    aDynamicT->m_iErrorCode = _OF_I8ADD;
    if(aDynamicT->m_iErrorCode != EC_NONE)
    {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else
    {
      *(*ppaW)++ = iArg1 + iArg2;
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INTERVALS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _OUT_OF_RANGE_IVY                                                 \
(                                                                         \
  (iMonths > 0 &&  (u8) iMonths > _C_IVL_MONTH_MAX ) ? EC_INTERVAL_OUT_OF_RANGE :      \
  (iMonths < 0 &&  (u8) (-1*iMonths) > _C_IVL_MONTH_MAX ) ? EC_INTERVAL_OUT_OF_RANGE : \
  EC_NONE                                                                 \
)

#define _OUT_OF_RANGE_IVD \
(                                                                         \
  (iMicros > 0 &&  (u8) iMicros > _C_IVL_USEC_MAX ) ? EC_INTERVAL_OUT_OF_RANGE :      \
  (iMicros < 0 &&  (u8) (-1*iMicros) > _C_IVL_USEC_MAX ) ? EC_INTERVAL_OUT_OF_RANGE : \
  EC_NONE                                                                 \
  )


static inline void _ADD_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;
  union Ivl_T wIvlResult;
  i8 iArg1;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

    i8 iMicros = iArg1 + iArg2;

    aDynamicT->m_iErrorCode = _OUT_OF_RANGE_IVD;

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {

      wIvlResult.m_iAsI8 = 0;

      if (iMicros < 0) {
        wIvlResult.m_xIvl.m_bIsNeg = 1;
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = -1*iMicros;
      }
      else {
        wIvlResult.m_xIvl.m_bIsNeg = 0;
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;
      }
      wIvlResult.m_xIvl.m_eType =
      eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType];
      *(*ppaW)++ = wIvlResult.m_iAsI8;
    }
 }
}

static inline void _ADD_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;
  union Ivl_T wIvlResult;
  i8 iArg1;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

    i8 iMonths = iArg1 + iArg2;
    aDynamicT->m_iErrorCode = _OUT_OF_RANGE_IVY;

    if(aDynamicT->m_iErrorCode != EC_NONE)  {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {

      wIvlResult.m_iAsI8 = 0;

      if (iMonths < 0) {
        wIvlResult.m_xIvl.m_bIsNeg = 1;
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = -1*iMonths;
      }
      else {
        wIvlResult.m_xIvl.m_bIsNeg = 0;
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iMonths;
      }
      wIvlResult.m_xIvl.m_eType =
      eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType];
      *(*ppaW)++ = wIvlResult.m_iAsI8;
    }
 }
}

static inline void _DIV_IVDINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;
  union Ivl_T ivlArg1;
  union Ivl_T wIvlResult;

  iArg2 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (iArg2 == 0) {
      *(*ppaW)++ = C_DATUM_NULL;
      aDynamicT->m_iErrorCode = EC_DIVIDE_BY_ZERO;
    }
    else {

     wIvlResult.m_iAsI8 = 0;
     wIvlResult.m_xIvl.m_eType = ivlArg1.m_xIvl.m_eType;

     iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;

     if ( (iArg2 < 0 && ! ivlArg1.m_xIvl.m_bIsNeg) ||
          (iArg2 > 0 && ivlArg1.m_xIvl.m_bIsNeg) )
        wIvlResult.m_xIvl.m_bIsNeg = 1;

     if (iArg2 < 0)
       iArg2 = (-1) * iArg2;

     d8 dMicros = (d8)iArg1 / (d8)iArg2;
     i8 iMicros = 0;
     i8 iRound = (dMicros * 10);
     iRound %= 10;
     if ( iRound >= 5)
       iMicros = dMicros + 1;
     else
       iMicros = dMicros;

      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;
      *(*ppaW)++ = wIvlResult.m_iAsI8;
    }
  }
}

static inline void _DIV_IVYINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;
  union Ivl_T ivlArg1;
  union Ivl_T wIvlResult;

  iArg2 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (iArg2 == 0) {
      *(*ppaW)++ = C_DATUM_NULL;
      aDynamicT->m_iErrorCode = EC_DIVIDE_BY_ZERO;
    }
    else {

     wIvlResult.m_iAsI8 = 0;
     wIvlResult.m_xIvl.m_eType = ivlArg1.m_xIvl.m_eType;

     iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;

     if ( (iArg2 < 0 && ! ivlArg1.m_xIvl.m_bIsNeg) ||
          (iArg2 > 0 && ivlArg1.m_xIvl.m_bIsNeg) )
        wIvlResult.m_xIvl.m_bIsNeg = 1;

     if (iArg2 < 0) {
       iArg2 = (-1) * iArg2;
     }

     i8 iMonths = iArg1 / iArg2;

     wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iMonths;
     *(*ppaW)++ = wIvlResult.m_iAsI8;
   }
 }
}


static inline void _MUL_INTIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;
  union Ivl_T ivlArg2;
  union Ivl_T wIvlResult;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL ||
      iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    wIvlResult.m_iAsI8 = 0;
    wIvlResult.m_xIvl.m_eType = ivlArg2.m_xIvl.m_eType;

    if (iArg1 < 0 || ivlArg2.m_xIvl.m_bIsNeg)
       wIvlResult.m_xIvl.m_bIsNeg = 1;

    if (iArg1 < 0)
      iArg1 = (-1) * iArg1;

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;

    i8 iMicros = iArg1 * iArg2;

    aDynamicT->m_iErrorCode = _OUT_OF_RANGE_IVD;

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;
      *(*ppaW)++ = wIvlResult.m_iAsI8;
    }
  }
}

static inline void _MUL_INTIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;
  union Ivl_T ivlArg2;
  union Ivl_T wIvlResult;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL ||
      iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    wIvlResult.m_iAsI8 = 0;
    wIvlResult.m_xIvl.m_eType = ivlArg2.m_xIvl.m_eType;
    iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;

    if (iArg1 < 0 || ivlArg2.m_xIvl.m_bIsNeg)
       wIvlResult.m_xIvl.m_bIsNeg = 1;

    if (iArg1 < 0)
      iArg1 = (-1) * iArg1;

    i8 iMonths = iArg1 * iArg2;

    aDynamicT->m_iErrorCode = _OUT_OF_RANGE_IVY;

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iMonths;
      *(*ppaW)++ = wIvlResult.m_iAsI8;
    }
  }
}

static inline void _MUL_IVDINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;
  union Ivl_T ivlArg1;
  union Ivl_T wIvlResult;

  iArg2 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    wIvlResult.m_iAsI8 = 0;
    wIvlResult.m_xIvl.m_eType = ivlArg1.m_xIvl.m_eType;

    iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (iArg2 < 0 || ivlArg1.m_xIvl.m_bIsNeg)
       wIvlResult.m_xIvl.m_bIsNeg = 1;

    if (iArg2 < 0)
      iArg2 = (-1) * iArg2;

    i8 iMicros = iArg1 * iArg2;

    aDynamicT->m_iErrorCode = _OUT_OF_RANGE_IVD;

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;
      *(*ppaW)++ = wIvlResult.m_iAsI8;
    }
  }
}

static inline void _MUL_IVYINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;
  union Ivl_T ivlArg1;
  union Ivl_T wIvlResult;

  iArg2 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    wIvlResult.m_iAsI8 = 0;
    wIvlResult.m_xIvl.m_eType = ivlArg1.m_xIvl.m_eType;

    iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;

    if (iArg2 < 0 || ivlArg1.m_xIvl.m_bIsNeg) {
       wIvlResult.m_xIvl.m_bIsNeg = 1;
       }

    if (iArg2 < 0) {
      iArg2 = (-1) * iArg2;
    }

    i8 iMonths = iArg1 * iArg2;

    aDynamicT->m_iErrorCode = _OUT_OF_RANGE_IVY;

    if(aDynamicT->m_iErrorCode != EC_NONE)  {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = (u8)iMonths;
      *(*ppaW)++ = wIvlResult.m_iAsI8;
    }
  }
}

static inline void _NEG_IVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;

  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (ivlArg1.m_xIvl.m_bIsNeg) {
      ivlArg1.m_xIvl.m_bIsNeg = 0;
    } else {
      ivlArg1.m_xIvl.m_bIsNeg = 1;
    }

    *(*ppaW)++ = ivlArg1.m_iAsI8;
 }
}

static inline void _NEG_IVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;

  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (ivlArg1.m_xIvl.m_bIsNeg) {
      ivlArg1.m_xIvl.m_bIsNeg = 0;
    } else {
      ivlArg1.m_xIvl.m_bIsNeg = 1;
    }

    *(*ppaW)++ = ivlArg1.m_iAsI8;
 }
}

static inline void _SUB_IVDIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;
  i8 iArg1;
  i8 iArg2;
  union Ivl_T wIvlResult;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;
    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

    i8 iMicros = iArg1 - iArg2;

    aDynamicT->m_iErrorCode = _OUT_OF_RANGE_IVD;

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      wIvlResult.m_iAsI8 = 0;

      if (iMicros < 0) {
        wIvlResult.m_xIvl.m_bIsNeg = 1;
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = -1*iMicros;
      }
      else {
        wIvlResult.m_xIvl.m_bIsNeg = 0;
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;
     }
     wIvlResult.m_xIvl.m_eType =
      eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType];
     *(*ppaW)++ = wIvlResult.m_iAsI8;
   }
  }
}


static inline void _SUB_IVYIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Ivl_T ivlArg2;
  i8 iArg1;
  i8 iArg2;
  union Ivl_T wIvlResult;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;
    iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

    i8 iMonths = iArg1 - iArg2;

    aDynamicT->m_iErrorCode = _OUT_OF_RANGE_IVY;

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      wIvlResult.m_iAsI8 = 0;

      if (iMonths < 0) {
        wIvlResult.m_xIvl.m_bIsNeg = 1;
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = -1*iMonths;
      }
      else {
        wIvlResult.m_xIvl.m_bIsNeg = 0;
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iMonths;
     }
     wIvlResult.m_xIvl.m_eType =
      eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType];
     *(*ppaW)++ = wIvlResult.m_iAsI8;
   }
  }
}

// ignore NULLS
//
static inline void _SUM_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else if (iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = iArg2;
  }
  else if (iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = iArg1;
  }
  else {
    aDynamicT->m_iErrorCode = _OF_I8ADD;
    if(aDynamicT->m_iErrorCode != EC_NONE)  {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      *(*ppaW)++ = iArg1 + iArg2;
    }
  }
}

static inline void _DIV_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (iArg2 == 0) {
       *(*ppaW)++ = C_DATUM_NULL;
      aDynamicT->m_iErrorCode = EC_DIVIDE_BY_ZERO;
    }
    else {
      aDynamicT->m_iErrorCode = _OF_I8DIV;
      if(aDynamicT->m_iErrorCode != EC_NONE) {
        *(*ppaW)++ = C_DATUM_NULL;
      }
      else {
        *(*ppaW)++ = iArg1 / iArg2;
      }
    }
  }
}

//TODO: OVERFLOW/UNDERFLOW
static inline void _MOD_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    if (iArg2 == 0) {
       *(*ppaW)++ = C_DATUM_NULL;
      aDynamicT->m_iErrorCode = EC_DIVIDE_BY_ZERO;
    }
    else {
      *(*ppaW)++ = iArg1 % iArg2;
    }
  }
}

static inline void _MOD_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _MOD_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _MUL_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    aDynamicT->m_iErrorCode = _OF_I8MUL;
    if(aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else {
      *(*ppaW)++ = iArg1 * iArg2;
    }
  }
}

static inline void _NEG_INT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    iArg1 = 0;
    aDynamicT->m_iErrorCode = _OF_I8SUB;
    if(aDynamicT->m_iErrorCode != EC_NONE)
    {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else
    {
      *(*ppaW)++ = -iArg2;
    }
  }
}


static inline void _SUB_INTINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      iArg2 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    aDynamicT->m_iErrorCode = _OF_I8SUB;
    if(aDynamicT->m_iErrorCode != EC_NONE)
    {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else
    {
      *(*ppaW)++ = iArg1 - iArg2;
    }
  }
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TIME
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _MICROS_TO_TIME                                                    \
  {                                                                        \
                                                                           \
    xTme2.m_uHour   = (iMicros / (_C_SECS_PER_HR  * _C_1M)) % _C_HOURS_PER_DAY; \
    xTme2.m_uMinute = (iMicros % (_C_SECS_PER_HR  * _C_1M)) / (_C_SECS_PER_MIN * _C_1M);   \
    xTme2.m_uSecond = (iMicros % (_C_SECS_PER_MIN * _C_1M)) / _C_1M;       \
    xTme2.m_uMicro  = iMicros % _C_1M;                                     \
    xTme2.m_uSpare = 0;                                                    \
                                                                           \
    if (xTme2.m_uHour > 24)                                                \
      xTme2.m_uHour   = xTme2.m_uHour % 24;                                \
                                                                           \
    *paW++ = xTme2.m_iAsI8;                                                \


static inline void vMicrosToTime(i8 iMicros, i8** ppaW)
{
  union Tme_T xTme2;

  xTme2.m_uHour   = (iMicros / (_C_SECS_PER_HR  * _C_1M)) % _C_HOURS_PER_DAY;
  xTme2.m_uMinute = (iMicros % (_C_SECS_PER_HR  * _C_1M)) / (_C_SECS_PER_MIN * _C_1M);
  xTme2.m_uSecond = (iMicros % (_C_SECS_PER_MIN * _C_1M)) / _C_1M;
  xTme2.m_uMicro  = iMicros % _C_1M;
  xTme2.m_uSpare = 0;

  if (xTme2.m_uHour > 24)
    xTme2.m_uHour   = xTme2.m_uHour % 24;

  *(*ppaW)++ = xTme2.m_iAsI8;
}

static inline void _ADD_TMEIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Tme_T xTme1;
  i8 iArg1;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xTme1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg1 = ((xTme1.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +
            (xTme1.m_uMinute*_C_SECS_PER_MIN*_C_1M) +
            (xTme1.m_uSecond*_C_1M) +
            xTme1.m_uMicro;

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

     iArg2 %= _C_SECS_PER_DAY  * _C_1M;
     i8 iMicros = iArg1 + iArg2;

    vMicrosToTime(iMicros, ppaW);
  }
}

static inline void _ADD_IVDTME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme2;
  union Ivl_T ivlArg1;
  i8 iArg1;
  i8 iArg2;

  xTme2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme2.m_iAsI8 == C_DATUM_NULL ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg2 = ((xTme2.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +
            (xTme2.m_uMinute*_C_SECS_PER_MIN*_C_1M) +
            (xTme2.m_uSecond*_C_1M) +
            xTme2.m_uMicro;

    iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;
    else

    iArg1 %= _C_SECS_PER_DAY  * _C_1M;
    i8 iMicros = iArg1 + iArg2;

    vMicrosToTime(iMicros, ppaW);
  }
}

static inline void _SUB_TMEIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Tme_T xTme1;
  i8 iArg1;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xTme1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg1 = ((xTme1.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +
            (xTme1.m_uMinute*_C_SECS_PER_MIN*_C_1M) +
            (xTme1.m_uSecond*_C_1M) +
            xTme1.m_uMicro;

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg2.m_xIvl.m_bIsNeg) {
      iArg2 = (-1) * iArg2;
    } else {
      iArg2 %= _C_SECS_PER_DAY * _C_1M;
    }

    if (iArg2 > iArg1) {
      iArg1 += _C_SECS_PER_DAY * _C_1M;
    }

    i8 iMicros = iArg1 - iArg2;
    vMicrosToTime(iMicros, ppaW);
  }
}

static inline void _SUB_TMETME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tme_T xTme2;
  i8 iArg1;
  i8 iArg2;
  union Ivl_T ivlArg1;

  xTme2.m_iAsI8 = *(--(*ppaW));
  xTme1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTme1.m_iAsI8 == C_DATUM_NULL ||
      xTme2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg1 = ((xTme1.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +
            (xTme1.m_uMinute*_C_SECS_PER_MIN*_C_1M) +
            (xTme1.m_uSecond*_C_1M) +
            xTme1.m_uMicro;

    iArg2 = ((xTme2.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +
            (xTme2.m_uMinute*_C_SECS_PER_MIN*_C_1M) +
            (xTme2.m_uSecond*_C_1M) +
            xTme2.m_uMicro;

    if (iArg1 < iArg2) {
      ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros = iArg2 - iArg1;
      ivlArg1.m_xIvl.m_bIsNeg = 1;
    }
    else {
      ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 - iArg2;
      ivlArg1.m_xIvl.m_bIsNeg = 0;
    }
    ivlArg1.m_xIvl.m_eType =  IVL_HS;

    *(*ppaW)++ = ivlArg1.m_iAsI8;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TIMESTAMP
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ADJUST_SECS_MICROS_                                           \
{                                                                      \
  if (iMicroSecs < 0) {                                                \
     iSecs = iSecs - 1;                                                \
     iMicroSecs = _C_1M + iMicroSecs;                                  \
  }                                                                    \
  if (iMicroSecs > _C_1M) {                                            \
      iMicroSecs -= _C_1M;                                             \
      iSecs++;                                                         \
  }                                                                    \
}

static inline void _ADD_TSPIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xTsp1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg2.m_xIvl.m_bIsNeg)
      iArg2 = (-1) * iArg2;

    struct tm xDateTime = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime.tm_year = xTsp1.m_uYearDelta;
    xDateTime.tm_mon  = xTsp1.m_uMonth - 1;
    xDateTime.tm_mday = xTsp1.m_uDay;
    xDateTime.tm_hour = xTsp1.m_uHour;
    xDateTime.tm_min  = xTsp1.m_uMinute;
    xDateTime.tm_isdst  = 0;

    i8 iSecs = iArg2/_C_1M;
    i8 iMicroSecs = iArg2%_C_1M + xTsp1.m_uMicro;

    _ADJUST_SECS_MICROS_

    xDateTime.tm_mday  = xTsp1.m_uDay + iSecs / C_SECS_PER_DAY;
    xDateTime.tm_sec  = xTsp1.m_uSecond + (iSecs % C_SECS_PER_DAY);
    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch = timegm(&xDateTime);

    if (xDateTime.tm_year > (i8) C_YEAR_RANGE ||
        xDateTime.tm_year < (i8) -C_YEAR_RANGE ||
        iSecondsSincePosixEpoch == -1) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }

    xTsp2.m_iAsI8 = 0;

    xTsp2.m_uYearDelta = xDateTime.tm_year;
    xTsp2.m_uMonth     = xDateTime.tm_mon + 1;
    xTsp2.m_uDay       = xDateTime.tm_mday;
    xTsp2.m_uHour      = xDateTime.tm_hour;
    xTsp2.m_uMinute    = xDateTime.tm_min;
    xTsp2.m_uSecond    = xDateTime.tm_sec;
    xTsp2.m_uMicro     = iMicroSecs;

    *(*ppaW)++ = xTsp2.m_iAsI8;
  }
}

static inline void _ADD_IVDTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  union Ivl_T ivlArg1;
  i8 iArg1;

  xTsp2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp2.m_iAsI8 == C_DATUM_NULL ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;

    if (ivlArg1.m_xIvl.m_bIsNeg)
      iArg1 = (-1) * iArg1;

    struct tm xDateTime = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime.tm_year = xTsp2.m_uYearDelta;
    xDateTime.tm_mon  = xTsp2.m_uMonth - 1;
    xDateTime.tm_mday = xTsp2.m_uDay;
    xDateTime.tm_hour = xTsp2.m_uHour;
    xDateTime.tm_min  = xTsp2.m_uMinute;
    xDateTime.tm_isdst  = 0;

    i8 iSecs = iArg1/_C_1M;
    i8 iMicroSecs = iArg1%_C_1M + xTsp2.m_uMicro;

    _ADJUST_SECS_MICROS_

    xDateTime.tm_mday = xTsp2.m_uDay + iSecs / C_SECS_PER_DAY;
    xDateTime.tm_sec  = xTsp2.m_uSecond + (iSecs % C_SECS_PER_DAY);
    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch = timegm(&xDateTime);

    if (xDateTime.tm_year > (i8) C_YEAR_RANGE ||
        xDateTime.tm_year < (i8) -C_YEAR_RANGE ||
        iSecondsSincePosixEpoch == -1) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }

    xTsp1.m_iAsI8 = 0;

    xTsp1.m_uYearDelta = xDateTime.tm_year;
    xTsp1.m_uMonth     = xDateTime.tm_mon + 1;
    xTsp1.m_uDay       = xDateTime.tm_mday;
    xTsp1.m_uHour      = xDateTime.tm_hour;
    xTsp1.m_uMinute    = xDateTime.tm_min;
    xTsp1.m_uSecond    = xDateTime.tm_sec;
    xTsp1.m_uMicro     = iMicroSecs;

    *(*ppaW)++ = xTsp1.m_iAsI8;
  }
}

static inline void _ADD_TSPIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xTsp1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;

  } else {

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;

    i8 iYear_arg2 = iArg2/12;
    i8 iMonth_arg2 = iArg2%12;

    if (ivlArg2.m_xIvl.m_bIsNeg) {
      iYear_arg2 = -1 * iYear_arg2;
      iMonth_arg2 = (-1) * iMonth_arg2;
    }

    i8 iYearDelta;
    i8 iMonth;
    i8 iDay;
    iYearDelta = xTsp1.m_uYearDelta + iYear_arg2;
    iMonth     = xTsp1.m_uMonth + iMonth_arg2;
    iDay       = xTsp1.m_uDay;

    if (iYearDelta > (i8) C_YEAR_RANGE) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;

    } else {
      _CONVERT_TO_VALID_DATE

      xTsp2.m_iAsI8 = 0;

      xTsp2.m_uYearDelta = iYearDelta;
      xTsp2.m_uMonth     = iMonth;
      xTsp2.m_uDay       = iDay;
      xTsp2.m_uHour      = xTsp1.m_uHour;
      xTsp2.m_uMinute    = xTsp1.m_uMinute;
      xTsp2.m_uSecond    = xTsp1.m_uSecond;
      xTsp2.m_uMicro     = xTsp1.m_uMicro;

      *(*ppaW)++ = xTsp2.m_iAsI8;
    }
  }
}

static inline void _ADD_IVYTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg1;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  i8 iArg1;

  xTsp2.m_iAsI8 = *(--(*ppaW));
  ivlArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp2.m_iAsI8 == C_DATUM_NULL ||
      ivlArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;

  } else {
    iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;

    i8 iYear_arg1 = iArg1/12;
    i8 iMonth_arg1 = iArg1%12;

    if (ivlArg1.m_xIvl.m_bIsNeg) {
      iYear_arg1 = -1 * iYear_arg1;
      iMonth_arg1 = (-1) * iMonth_arg1;
    }

    i8 iYearDelta;
    i8 iMonth;
    i8 iDay;
    iYearDelta = xTsp2.m_uYearDelta + iYear_arg1;
    iMonth     = xTsp2.m_uMonth + iMonth_arg1;
    iDay       = xTsp2.m_uDay;

    if (iYearDelta > (i8) C_YEAR_RANGE) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;

    } else {
      _CONVERT_TO_VALID_DATE

      xTsp1.m_iAsI8 = 0;

      xTsp1.m_uYearDelta = iYearDelta;
      xTsp1.m_uMonth     = iMonth;
      xTsp1.m_uDay       = iDay;
      xTsp1.m_uHour      = xTsp2.m_uHour;
      xTsp1.m_uMinute    = xTsp2.m_uMinute;
      xTsp1.m_uSecond    = xTsp2.m_uSecond;
      xTsp1.m_uMicro     = xTsp2.m_uMicro;

      *(*ppaW)++ = xTsp1.m_iAsI8;
    }
  }
}

static inline void _SUB_TSPIVD(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xTsp1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;
    if ( ! ivlArg2.m_xIvl.m_bIsNeg )
      iArg2 = (-1) * iArg2;

    struct tm xDateTime = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime.tm_year = xTsp1.m_uYearDelta;
    xDateTime.tm_mon  = xTsp1.m_uMonth - 1;
    xDateTime.tm_mday = xTsp1.m_uDay;
    xDateTime.tm_hour = xTsp1.m_uHour;
    xDateTime.tm_min  = xTsp1.m_uMinute;
    xDateTime.tm_isdst  = 0;

    i8 iSecs = iArg2/_C_1M;
    i8 iMicroSecs = iArg2%_C_1M + (i8) xTsp1.m_uMicro;
    _ADJUST_SECS_MICROS_

    xDateTime.tm_mday = xTsp1.m_uDay + iSecs / C_SECS_PER_DAY;
    xDateTime.tm_sec  = xTsp1.m_uSecond + (iSecs % C_SECS_PER_DAY);
    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch = timegm(&xDateTime);

    if (xDateTime.tm_year > (i8) C_YEAR_RANGE ||
        xDateTime.tm_year < (i8) -C_YEAR_RANGE ||
        iSecondsSincePosixEpoch == -1) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }

    xTsp2.m_iAsI8 = 0;

    xTsp2.m_uYearDelta = xDateTime.tm_year;
    xTsp2.m_uMonth     = xDateTime.tm_mon + 1;
    xTsp2.m_uDay       = xDateTime.tm_mday;
    xTsp2.m_uHour      = xDateTime.tm_hour;
    xTsp2.m_uMinute    = xDateTime.tm_min;
    xTsp2.m_uSecond    = xDateTime.tm_sec;
    xTsp2.m_uMicro     = iMicroSecs;
    xTsp2.m_uLocal     = _C_TSP_LOCAL;

    *(*ppaW)++ = xTsp2.m_iAsI8;
  }
}

static inline void _SUB_TSPIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T ivlArg2;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;
  i8 iArg2;

  ivlArg2.m_iAsI8 = *(--(*ppaW));
  xTsp1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp1.m_iAsI8 == C_DATUM_NULL ||
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {

    iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;

    i8 iYear_arg2 = iArg2/12;
    i8 iMonth_arg2 = iArg2%12;

    if (ivlArg2.m_xIvl.m_bIsNeg) {
      iYear_arg2 = (-1) * iYear_arg2;
      iMonth_arg2 = (-1) * iMonth_arg2;
    }

    i8 iYearDelta;
    i8 iMonth;
    i8 iDay;
    iYearDelta = xTsp1.m_uYearDelta - iYear_arg2;
    iMonth     = xTsp1.m_uMonth - iMonth_arg2;
    iDay       = xTsp1.m_uDay;

    if (iYearDelta > (i8) C_YEAR_RANGE) {
      aDynamicT->m_iErrorCode = EC_DATE_OUT_OF_RANGE;
    }
    else {
      _CONVERT_TO_VALID_DATE

      xTsp2.m_iAsI8 = 0;

      xTsp2.m_uYearDelta = iYearDelta;
      xTsp2.m_uMonth     = iMonth;
      xTsp2.m_uDay       = iDay;
      xTsp2.m_uHour      = xTsp1.m_uHour;
      xTsp2.m_uMinute    = xTsp1.m_uMinute;
      xTsp2.m_uSecond    = xTsp1.m_uSecond;
      xTsp2.m_uMicro     = xTsp1.m_uMicro;

      *(*ppaW)++ = xTsp2.m_iAsI8;
    }
  }
}


static inline void _SUB_TSPTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Ivl_T wIvlResult;
  union Tsp_T xTsp1;
  union Tsp_T xTsp2;

  xTsp2.m_iAsI8 = *(--(*ppaW));
  xTsp1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      xTsp2.m_iAsI8 == C_DATUM_NULL ||
      xTsp1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;

  } else {
    struct tm xDateTime1 = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    struct tm xDateTime2 = {0,0,0,0,0,0,0,0,0,0,NULL} ;

    xDateTime1.tm_year = xTsp1.m_uYearDelta;
    xDateTime1.tm_mon  = xTsp1.m_uMonth - 1 ;
    xDateTime1.tm_mday = xTsp1.m_uDay;
    xDateTime1.tm_hour = xTsp1.m_uHour;
    xDateTime1.tm_min  = xTsp1.m_uMinute;
    xDateTime1.tm_sec  = xTsp1.m_uSecond;
    xDateTime1.tm_isdst = 0;

    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch1 = timegm(&xDateTime1);

    xDateTime2.tm_year = xTsp2.m_uYearDelta;
    xDateTime2.tm_mon  = xTsp2.m_uMonth - 1;
    xDateTime2.tm_mday = xTsp2.m_uDay;
    xDateTime2.tm_hour = xTsp2.m_uHour;
    xDateTime2.tm_min  = xTsp2.m_uMinute;
    xDateTime2.tm_sec  = xTsp2.m_uSecond;
    xDateTime2.tm_isdst = 0;

    // This next call seems to have a side effect of normalising
    // all member values in the struct tm.
    time_t iSecondsSincePosixEpoch2 = timegm(&xDateTime2);

    i8 iSeconds = iSecondsSincePosixEpoch1 - iSecondsSincePosixEpoch2;

    wIvlResult.m_xIvl.m_eType = IVL_DS;

    if (iSeconds < 0) {
      wIvlResult.m_xIvl.m_bIsNeg = 1;
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  (-1)*iSeconds*_C_1M +
      (xTsp2.m_uMicro - xTsp1.m_uMicro);
    } else {
			wIvlResult.m_xIvl.m_bIsNeg = 0;
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  iSeconds*_C_1M +
      (xTsp1.m_uMicro - xTsp2.m_uMicro);
    }
    *(*ppaW)++ = wIvlResult.m_iAsI8;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// UNKNOWN and ANY
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _ADD_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _DIV_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _MUL_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _SUB_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _SUM_ANYUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  *(*ppaW)++ = iArg1;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  ANY and UNKNOWN
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _ADD_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _DIV_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _MUL_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _SUB_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}


static inline void _SUM_UNKANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1;
  i8 iArg2;

  iArg2 = *(--(*ppaW));
  iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  *(*ppaW)++ = iArg2;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
