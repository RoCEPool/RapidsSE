/*============================================================================*/
/* CastMacros.h -- Cast Macros                                                */
/*============================================================================*/
#pragma once
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CAST
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _ANY_TOUNK(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  --(*ppaW);
  *(*ppaW)++ = C_DATUM_NULL;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  TO BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _BCD_TOBCD(i8** ppaW, struct Dynamic_T* aDynamicT, i8 iPrecision, i8 iScale)
{
  union Bcd_T bcdArg1;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      bcdArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // Rescaling may also produce an overflow, as might any precision limitations
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  vRescaleBcdToPrecisionScale(ppaW, aDynamicT, iPrecision, iScale, NULL);

  // The final value would have been pushed onto the stack during the rescaling
  // unless an error was encountered
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
}


static inline void _INT_TOBCD(i8** ppaW, struct Dynamic_T* aDynamicT, i8 iPrecision, i8 iScale)
{
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // Check if the integer is too big for a decimal
  if(iArg1 > C_BCD_MAX) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;

  } else if(iArg1 < (i8) C_BCD_MIN) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // Convert it to a BCD with scale 0, then rescale it.
  bcdArg1.m_iMantissa = iArg1;
  bcdArg1.m_uScale = 0;
  aDynamicT->m_iErrorCode = EC_NONE;

  // Rescaling may also produce an overflow, as might any precision limitations
  *(*ppaW)++ = bcdArg1.m_iAsI8;
  vRescaleBcdToPrecisionScale(ppaW, aDynamicT, iPrecision, iScale, NULL);

  // The final value would have been pushed onto the stack during the rescaling
  // unless an error was encountered
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
}

// Declaration, used in _VCH_TOBCD below.
static inline void _VCH_TOINT(i8** ppaW, struct Dynamic_T* aDynamicT);

static inline void _VCH_TOBCD(i8** ppaW, struct Dynamic_T* aDynamicT, i8 iResultPrecision, i8 iResultScale)
{
  i8 iArg1 = *(--(*ppaW));
  char* psArg = (char*) iArg1;

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      *psArg == 0) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Bcd_T bcdResult;

  // Where is the decimal point, if any?
  i8 iPointPos = -1;
  char* pS = psArg;
  for (i8 i=0; pS[i]; i++) {
    if (pS[i] == '.') {
      iPointPos = i;
      break;
    }
  }

  if (iPointPos == -1) {
    // We've got a decimal with no scale. Looks like an integer,
    // so let's reuse that functionality.
    *(*ppaW)++ = iArg1;
    _VCH_TOINT(ppaW, aDynamicT);

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      return;
    }

    i8 iMantissa = *(--(*ppaW));

    // Check that the integer value doesn't exceed BCD limits
    if (iMantissa > C_BCD_MAX || iMantissa < C_BCD_MIN) {
      aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
      *(*ppaW)++ = C_DATUM_NULL;
      return;
    }

    bcdResult.m_iMantissa = iMantissa;
    bcdResult.m_uScale = 0;
    *(*ppaW)++ = bcdResult.m_iAsI8;

    // Rescale to the desired precision/scale
    vRescaleBcdToPrecisionScale(ppaW, aDynamicT, iResultPrecision, iResultScale, NULL);

    // result is on the stack unless there is an error
    if (aDynamicT->m_iErrorCode != EC_NONE) {
      *(*ppaW)++ = C_DATUM_NULL;
    }

    return;
  }

  // There is a decimal place that we have to handle

  i8 iStrPrecision = 0;
  i8 iStrScale = 0;
  bool bNegative = false;
  u8 uUnsignedResult = 0;

  pS = psArg;

  // skip any leading whitespace
  if (*pS && isspace(*pS)) {
    ++pS;
  }

  // test for negative or positive sign
  if (*pS && '-' == *pS) {
    bNegative = true;
    ++pS;

  } else if (*pS && '+' == *pS) {
    ++pS;
  }

  if (!*pS || !(isdigit(*pS) || '.' == *pS)) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // ignore any leading 0's
  while (isdigit(*pS) && *pS == '0') {
    ++pS;
  }

  // collect leading digits (till '.')
  while (*pS && isdigit(*pS)) {
    uUnsignedResult = uUnsignedResult * 10 + (*pS - '0');
    ++pS;
    ++iStrPrecision;
  }

  if (!*pS || !(isdigit(*pS) || '.' == *pS)) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // Have we exceeded maximum precision digits?
  if (iStrPrecision > iResultPrecision) {
    aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if ('.' == *pS) {

    pS++; // skip '.'

    while (*pS && isdigit(*pS)) {
      uUnsignedResult = uUnsignedResult * 10 + (*pS - '0');
      ++pS;
      ++iStrScale;
      ++iStrPrecision;

      // Have we exceeded maximum precision digits?
      if (iStrPrecision > iResultPrecision) {
        aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
        *(*ppaW)++ = C_DATUM_NULL;
        return;
      }
    }

  } else {
    // its not a '.', must be an error
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }


  // have we exceeded the max ?
  if (uUnsignedResult > (u8)C_BCD_MAX) {
    aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  bcdResult.m_iMantissa = (i8) uUnsignedResult * (bNegative ? -1 : 1);
  bcdResult.m_uScale = iStrScale;

  // Now rescale the value to the desired precision and scale
  *(*ppaW)++ = bcdResult.m_iAsI8;
  vRescaleBcdToPrecisionScale(ppaW, aDynamicT, iResultPrecision, iResultScale, NULL);

  // result is on the stack unless there is an error
  if (aDynamicT->m_iErrorCode != EC_NONE) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  TO FLOAT
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _BCD_TOFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Bcd_T bcdArg1;
  
  bcdArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    bcdArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  fltArg1.m_dFlt = (d8)(bcdArg1.m_iMantissa);
  fltArg1.m_dFlt = fltArg1.m_dFlt / aDynamicT->m_iaPow10[bcdArg1.m_uScale];
  *(*ppaW)++ = fltArg1.m_iAsI8 ;
}

// INT to Float
static inline void _INT_TOFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  union Flt_T fltArg2;
  
  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
  fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if(fltArg1.m_dFlt > (d8)C_FLT_MAX) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;

  } else if(fltArg1.m_dFlt < (d8) C_FLT_MIN) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;

  } else {
    fltArg2.m_dFlt = fltArg1.m_iAsI8;
    *(*ppaW)++ = fltArg2.m_iAsI8;
  }
}

// Float to int is just drop the decimal digits?
static inline void _FLT_TOINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  
  i8 iArg1 = *(--(*ppaW));
  fltArg1.m_iAsI8 = iArg1;
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if(fltArg1.m_dFlt > (i8)C_I8_MAX) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;

  } else if(fltArg1.m_dFlt < (i8) C_I8_MIN) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;

  } else {
    *(*ppaW)++ = (i8)( fltArg1.m_dFlt >= 0.0 ?
      (fltArg1.m_dFlt + 0.5) : (fltArg1.m_dFlt - 0.5) );
  }
}

static inline void _FLT_TOBCD(i8** ppaW, struct Dynamic_T* aDynamicT, i8 iPrecision, i8 iScale)
{
  union Flt_T fltArg1;
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;
  
  fltArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if (fltArg1.m_dFlt > (d8)C_BCD_MAX) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }
  else if (fltArg1.m_dFlt < (d8) C_BCD_MIN) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 cnt = 0;
  d8 dt = fltArg1.m_dFlt - (i8)(fltArg1.m_dFlt);
  if (fltArg1.m_dFlt < 0 ) dt = dt * -1;
  while (dt >= 0.0000001 && cnt < 14) {
    dt = dt * 10.0;
    dt = dt - (int)(dt+0.0000001);
    cnt++;
  }

  bcdArg1.m_iMantissa = fltArg1.m_iAsI8;
  bcdArg2.m_iMantissa = aDynamicT->m_iaPow10[cnt];

  i8 iMantissa = (i8)(fltArg1.m_dFlt * (d8)bcdArg2.m_iMantissa);
  while (iMantissa > C_BCD_MAX) {
    iMantissa = iMantissa / 10;
    cnt--;
  }

  bcdArg1.m_iMantissa = iMantissa;
  bcdArg1.m_uScale = cnt;

  *(*ppaW)++ = bcdArg1.m_iAsI8;
}

static inline void vFltToBcdToScale(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Flt_T fltArg1;
  i8 iScale;
  union Bcd_T bcdArg1;
  union Bcd_T bcdArg2;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  iScale = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if (fltArg1.m_dFlt > (d8)C_BCD_MAX) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;

  } else if (fltArg1.m_dFlt < (d8) C_BCD_MIN) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 cnt = 0;
  d8 dt = fltArg1.m_dFlt - (i8)(fltArg1.m_dFlt);
  if (fltArg1.m_dFlt < 0 ) dt = dt * -1;
  while (dt >= 0.0000001 && cnt < iScale) {
    dt = dt * 10.0;
    dt = dt - (int)(dt+0.0000001);
    cnt++;
  }

  bcdArg1.m_iMantissa = fltArg1.m_iAsI8;
  bcdArg2.m_iMantissa = aDynamicT->m_iaPow10[iScale];

  i8 iMantissa = (i8)(fltArg1.m_dFlt * (d8)bcdArg2.m_iMantissa);
  while (iMantissa > C_BCD_MAX) {
    iMantissa = iMantissa / 10;
    cnt--;
  }

  bcdArg1.m_iMantissa = iMantissa;
  bcdArg1.m_uScale = iScale;

  *(*ppaW)++ = bcdArg1.m_iAsI8;
}


static inline void _FLT_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszStrArea, i8* piAreaRemaining)
{
  union Flt_T fltArg1;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  int iSize = snprintf((*ppszStrArea), *piAreaRemaining, "%.9e", fltArg1.m_dFlt);
  if (iSize < *piAreaRemaining) {
    *(*ppaW)++ = (i8) (*ppszStrArea);
    (*ppszStrArea) += (iSize + 1);
    (*piAreaRemaining) -= (iSize + 1);
  } else {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
  }
}

/*
 * Does not take care of min/max flt to varchar
 *
// how many decimal places , 10?
//WARNING do we need to length of string?

static inline void _FLT_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszStrArea)
{
  union Flt_T fltArg1;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    char *pS = (*ppszStrArea);
    char *pS1 = (*ppszStrArea);
    i8 iVal;
    i8 iDecs;
    if (fltArg1.m_iAsI8 < 0) {
      fltArg1.m_dFlt -= 0.00000000005;
      iDecs = (i8)(fltArg1.m_dFlt * -10000000000) % 10000000000;
      iVal = (i8)(-1 * fltArg1.m_dFlt);
    }
    else {
      fltArg1.m_dFlt += 0.00000000005;
      iDecs = (i8)(fltArg1.m_dFlt * 10000000000) % 10000000000;
      iVal = (i8)fltArg1.m_dFlt;
    }
    for (i8 i=0; i< 10; i++) {
      *pS++ = (iDecs % 10) + '0';
      iDecs /= 10;
    }
    *pS++ = '.';
    if (iVal == 0)
      *pS++ = '0' ;
    else {
      while (iVal > 0) {
      *pS++ = (iVal % 10) + '0';
        iVal /= 10;
      }
    }
    if (fltArg1.m_dFlt < 0.0) *pS++ = '-';
    *pS = '\0';
    while (--pS > pS1) {
      char c = *pS;
      *pS = *pS1;
      *pS1++ = c;
    }
    *(*ppaW)++ = (i8)(*ppszStrArea);
    (*ppszStrArea) += strlen((*ppszStrArea)) + 1;
  }
}
*/

// varchar TO FLT
static inline void _VCH_TOFLT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  char *pS = (char*)iArg1;

  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Flt_T fltArg1;
  char* pEndPtr;

  fltArg1.m_dFlt = strtod(pS, &pEndPtr);

  if (fltArg1.m_dFlt == 0 && pEndPtr == pS) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;

  } else if ( (fltArg1.m_dFlt == HUGE_VAL || fltArg1.m_dFlt == -HUGE_VAL) && errno == ERANGE) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;

  } else if (fltArg1.m_dFlt == 0 && errno) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_UNDERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;

  } else if (fltArg1.m_dFlt > C_FLT_MAX || fltArg1.m_dFlt < C_FLT_MIN) {
    aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *(*ppaW)++ = fltArg1.m_iAsI8;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _BCD_TOINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Bcd_T bcdArg1;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    bcdArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 iVal = bcdArg1.m_iMantissa / aDynamicT->m_iaPow10[bcdArg1.m_uScale];
  *(*ppaW)++ = iVal;
}


static inline bool bIsHex(const char c)
{
  if (c >= 'a' && c <= 'f') return true;
  if (c >= 'A' && c <= 'F') return true;
  if (c >= '0' && c <= '9') return true;

  return false;
}

static inline i8 iHexToInw(const char c)
{
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= '0' && c <= '9') return c - '0';

  return -1;
}

static inline void _VCH_TOINT(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  char* psArg = (char*) iArg1;

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      *psArg == 0) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 iIntVal = 0;
  bool bNegative = false;

  if ('-' == *psArg) {
    bNegative = true;
    psArg++;
  }

  if (!*psArg || (!isdigit(*psArg) && *psArg != 'x' && *psArg != 'X')) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // skip leading 0's
  while ('0' == *psArg)
    psArg++;

  if ('x' == *psArg || 'X' == *psArg) {
    const i8 iLimit = C_I8_MAX / 16;
    const i8 iModulus = bNegative ? -(C_I8_MIN % 16) : C_I8_MAX % 16;

    psArg++;
    while (bIsHex(*psArg)) {
      i8 j = iHexToInw(*psArg++);

      if (iIntVal > iLimit || (iIntVal == iLimit && j > iModulus) ) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
        *(*ppaW)++ = C_DATUM_NULL;
        return;
      }

      iIntVal = iIntVal * 16 + j;
    }

  } else {
    const i8 iLimit = C_I8_MAX / 10;
    const i8 iModulus = bNegative ? -(C_I8_MIN % 10) : C_I8_MAX % 10;

    while (isdigit(*psArg)) {
      i8 j = (*psArg++ - '0');

      if (iIntVal > iLimit || (iIntVal == iLimit && j > iModulus) ) {
        aDynamicT->m_iErrorCode = EC_ARITHMETIC_OVERFLOW;
        *(*ppaW)++ = C_DATUM_NULL;
        return;
      }

      iIntVal = iIntVal * 10 + j;
    }
  }

  if (bNegative)
    iIntVal *= -1;

  *(*ppaW)++ = iIntVal;
  aDynamicT->m_iErrorCode = EC_NONE;
}



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO DATE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _TSP_TODTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Dte_T xDte1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xTsp1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  xDte1.m_iAsI8 = 0;

  xDte1.m_uDay = xTsp1.m_uDay;
  xDte1.m_uMonth = xTsp1.m_uMonth;
  xDte1.m_uYearDelta = xTsp1.m_uYearDelta;

  *(*ppaW)++ = xDte1.m_iAsI8;
}

static inline void _VCH_TODTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO IVL
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// INT to DAY
static inline void _INT_TODAY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Ivl_T wIvlResult;

  wIvlResult.m_iAsI8 = 0;
  wIvlResult.m_xIvl.m_eType = IVL_DAY;

  if (iArg1 < 0) {
    wIvlResult.m_xIvl.m_bIsNeg = 1;
  }
  wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_SECS_PER_DAY * _C_1M;

  *(*ppaW)++ = wIvlResult.m_iAsI8;
}

// INT to HOUR
static inline void _INT_TOHOUR(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Ivl_T wIvlResult;

  wIvlResult.m_iAsI8 = 0;
  wIvlResult.m_xIvl.m_eType = IVL_HOUR;

  if (iArg1 < 0) {
    wIvlResult.m_xIvl.m_bIsNeg = 1;
  }
  wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_SECS_PER_HR * _C_1M;

  *(*ppaW)++ = wIvlResult.m_iAsI8;
}

// INT to MINUTE
static inline void _INT_TOMINUTE(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Ivl_T wIvlResult;

  wIvlResult.m_iAsI8 = 0;
  wIvlResult.m_xIvl.m_eType = IVL_MINUTE;

  if (iArg1 < 0) {
    wIvlResult.m_xIvl.m_bIsNeg = 1;
  }
  wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_SECS_PER_MIN * _C_1M;

  *(*ppaW)++ = wIvlResult.m_iAsI8;
}

// INT to MONTH
static inline void _INT_TOMONTH(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Ivl_T wIvlResult;

  wIvlResult.m_iAsI8 = 0;
  wIvlResult.m_xIvl.m_eType = IVL_MONTH;

  if (iArg1 < 0) {
    wIvlResult.m_xIvl.m_bIsNeg = 1;
  }
  wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iArg1;

  *(*ppaW)++ = wIvlResult.m_iAsI8;
}

// INT to SECOND
static inline void _INT_TOSECOND(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Ivl_T wIvlResult;

  wIvlResult.m_iAsI8 = 0;
  wIvlResult.m_xIvl.m_eType = IVL_SECOND;

  if (iArg1 < 0) {
    wIvlResult.m_xIvl.m_bIsNeg = 1;
  }
  wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_1M;

  *(*ppaW)++ = wIvlResult.m_iAsI8;
}

// INT to YEAR
static inline void _INT_TOYEAR(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Ivl_T wIvlResult;

  wIvlResult.m_iAsI8 = 0;
  wIvlResult.m_xIvl.m_eType = IVL_DAY;

  if (iArg1 < 0) {
    wIvlResult.m_xIvl.m_bIsNeg = 1;
  }
  wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 * _C_MONTHS_PER_YEAR;

  *(*ppaW)++ = wIvlResult.m_iAsI8;
}

static inline void _VCH_TOIVL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO IVY
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _VCH_TOIVY(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO TME
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _TSP_TOTME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tsp_T xTsp1;
  union Tme_T xTme1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xTsp1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  xTme1.m_iAsI8 = 0;

  xTme1.m_uMicro = xTsp1.m_uMicro;
  xTme1.m_uSecond = xTsp1.m_uSecond;
  xTme1.m_uMinute = xTsp1.m_uMinute;
  xTme1.m_uHour = xTsp1.m_uHour;

  *(*ppaW)++ = xTme1.m_iAsI8;
}

static inline void _VCH_TOTME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO TSP
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _DTE_TOTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Dte_T xDte1;
  union Tsp_T xTsp1;

  xDte1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xDte1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  xTsp1.m_iAsI8 = 0;

  xTsp1.m_uDay = xDte1.m_uDay;
  xTsp1.m_uMonth = xDte1.m_uMonth;
  xTsp1.m_uYearDelta = xDte1.m_uYearDelta;
  xTsp1.m_uHour      = 0;
  xTsp1.m_uMinute    = 0;
  xTsp1.m_uSecond    = 0;
  xTsp1.m_uMicro     = 0;
  xTsp1.m_uLocal     = _C_TSP_LOCAL;
  *(*ppaW)++ = xTsp1.m_iAsI8;
}

static inline void _TME_TOTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  union Tme_T xTme1;
  union Tsp_T xTsp1;

  xTme1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xTme1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  time_t iSecondsSincePosixEpoch = time(0);
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);

  xTsp1.m_iAsI8 = 0;

  xTsp1.m_uYearDelta = aLocalDateTime->tm_year;
  xTsp1.m_uMonth = aLocalDateTime->tm_mon + 1;
  xTsp1.m_uDay = aLocalDateTime->tm_mday;

  xTsp1.m_uMicro = xTme1.m_uMicro;
  xTsp1.m_uSecond = xTme1.m_uSecond;
  xTsp1.m_uMinute = xTme1.m_uMinute;
  xTsp1.m_uHour = xTme1.m_uHour;
  xTsp1.m_uLocal = _C_TSP_LOCAL;

  *(*ppaW)++ = xTsp1.m_iAsI8;
}

static inline void _VCH_TOTSP(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO VCH
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _BOL_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 nSize = iArg1 ? 5 : 6;

  if (*piAreaRemaining < nSize) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if (iArg1)
    strncpy((*ppszDstArea), "true", nSize);
  else
    strncpy((*ppszDstArea), "false", nSize);

  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}



static inline void _BCD_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  union Bcd_T bcdArg1;
  i8 iInitialArea = *piAreaRemaining;

  bcdArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    bcdArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 bNegative = 0;

  if (bcdArg1.m_iMantissa < 0) {
    bcdArg1.m_iMantissa = -bcdArg1.m_iMantissa;
    bNegative = 1;
  }

  char *p = (*ppszDstArea), *q = (*ppszDstArea);

  while (bcdArg1.m_uScale && --(*piAreaRemaining)) {
    *p++ = (bcdArg1.m_iMantissa % 10) + '0';
    bcdArg1.m_iMantissa /= 10ll;
    bcdArg1.m_uScale--;
  }

  *p++ = '.';
  --(*piAreaRemaining);
  i1 iFirst = 1; // Always print the first digit after the decimal point,
                 // especially if it is a zero.

  while ((bcdArg1.m_iMantissa || iFirst) && --(*piAreaRemaining)) {
    *p++ = (bcdArg1.m_iMantissa % 10) + '0';
    bcdArg1.m_iMantissa /= 10ll;
    iFirst = 0;
  }

  if (bNegative && --(*piAreaRemaining))
    *p++ = '-';

  if (*piAreaRemaining <= 0) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *p = '\0';

  while (--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }
  *(*ppaW)++ = (i8)(*ppszDstArea);
  i8 iLen = (i8) strnlen(*ppszDstArea, iInitialArea) + 1;
  (*ppszDstArea) += iLen;
}

static inline void _TME_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  union Tme_T xTme1;

  xTme1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xTme1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 nSize = 9 + (xTme1.m_uMicro != 0 ? 7 : 0);
  if (*piAreaRemaining < nSize) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  char *p = (*ppszDstArea);

  u8 tmp = xTme1.m_uHour;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xTme1.m_uHour;
  *p++ = (tmp % 10) + '0';

  *p++ = ':';

  tmp = xTme1.m_uMinute;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xTme1.m_uMinute;
  *p++ = (tmp % 10) + '0';

  *p++ = ':';

  tmp = xTme1.m_uSecond;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xTme1.m_uSecond;
  *p++ = (tmp % 10) + '0';

  if(xTme1.m_uMicro != 0) {
    *p++ = '.';

    u8 uFactor = 100000;
    u8 uDigit;
    tmp = xTme1.m_uMicro;

    while (uFactor) {
      uDigit = tmp / uFactor;
      *p++ = uDigit + '0';

      tmp -= (uDigit * uFactor);
      uFactor /= 10;
    }
  }
  *(*ppaW)++ = (i8)(*ppszDstArea);
  i8 iLen = (i8) strnlen(*ppszDstArea,  *piAreaRemaining) + 1;
  (*ppszDstArea) += iLen;
  (*piAreaRemaining) -= iLen;
}

static inline void _DTE_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  union Dte_T xDte1;

  xDte1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xDte1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 nSize = 11;
  if (*piAreaRemaining < nSize) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  u8 tmpYear = C_YEAR_BASE + xDte1.m_uYearDelta;
  char *p = (*ppszDstArea);

  u8 tmp = tmpYear;
  tmp /= 1000llu;
  *p++ = tmp + '0';

  tmp = tmpYear;
  tmp /= 100llu;
  *p++ = (tmp % 10) + '0';

  tmp = tmpYear;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = tmpYear;
  *p++ = (tmp % 10) + '0';

  *p++ = '-';

  tmp = xDte1.m_uMonth;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xDte1.m_uMonth;
  *p++ = (tmp % 10) + '0';

  *p++ = '-';

  tmp = xDte1.m_uDay;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xDte1.m_uDay;
  *p++ = (tmp % 10) + '0';

  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}


static inline void _INT_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 nSize = 2; // null terminator + 1st digit
  i8 bNegative = 0;

  if (iArg1 < 0) {
    iArg1 = -iArg1;
    bNegative = 1;
  }

  if (*piAreaRemaining < nSize) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  char *p = (*ppszDstArea), *q = (*ppszDstArea);
  do {
    *p++ = (iArg1 % 10) + '0';
    iArg1 /= 10ll;
    ++nSize;
  } while (iArg1 && *piAreaRemaining >= nSize);

  if (bNegative) {
    *p++ = '-';
    ++nSize;
  }

  *p = '\0';
  while (--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }
  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}

static inline void _IVL_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _IVY_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _TSP_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  union Tsp_T xTsp1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xTsp1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 nSize = 20 + (xTsp1.m_uMicro != 0 ? 7 : 0);
  if (*piAreaRemaining < nSize) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  u8 tmpYear = C_YEAR_BASE + xTsp1.m_uYearDelta;
  char *p = (*ppszDstArea);

  u8 tmp = tmpYear;
  tmp /= 1000llu;
  *p++ = tmp + '0';

  tmp = tmpYear;
  tmp /= 100llu;
  *p++ = (tmp % 10) + '0';

  tmp = tmpYear;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = tmpYear;
  *p++ = (tmp % 10) + '0';

  *p++ = '-';

  tmp = xTsp1.m_uMonth;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xTsp1.m_uMonth;
  *p++ = (tmp % 10) + '0';

  *p++ = '-';

  tmp = xTsp1.m_uDay;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xTsp1.m_uDay;
  *p++ = (tmp % 10) + '0';

  *p++ = ' ';

  tmp = xTsp1.m_uHour;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xTsp1.m_uHour;
  *p++ = (tmp % 10) + '0';

  *p++ = ':';

  tmp = xTsp1.m_uMinute;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xTsp1.m_uMinute;
  *p++ = (tmp % 10) + '0';

  *p++ = ':';

  tmp = xTsp1.m_uSecond;
  tmp /= 10llu;
  *p++ = (tmp % 10) + '0';

  tmp = xTsp1.m_uSecond;
  *p++ = (tmp % 10) + '0';

  if(xTsp1.m_uMicro != 0) {
    *p++ = '.';

    u8 uFactor = 100000;
    u8 uDigit;
    tmp = xTsp1.m_uMicro;

    while (uFactor) {
      uDigit = tmp / uFactor;
      *p++ = uDigit + '0';

      tmp -= (uDigit * uFactor);
      uFactor /= 10;
    }
  }

  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}

static inline void _VCH_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining)
{
  i8 iArg1 = *(--(*ppaW));
  char *pS = (char*)iArg1;

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // TODO: handle precision
  *(*ppaW)++ = iArg1;

//  *(*ppaW)++ = (i8)(*ppszDstArea);
//  (*ppszDstArea) += nSize;
//  (*piAreaRemaining) -= nSize;
}

// nothing to do
// unk remains unk
static inline void _UNK_TOANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
