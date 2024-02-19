/*============================================================================*/
/* CastMacros.h -- Cast Macros                                                */
/*============================================================================*/
#pragma once
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

// Some function declarations
static inline void _VCH_TOTSP(i8** ppaW, struct Dynamic_T* aDynamicT);
static inline void _VCH_TOINT(i8** ppaW, struct Dynamic_T* aDynamicT);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Helper functions
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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

static inline i8 iDigitToI8(const char cDigit)
{
  i8 i = cDigit - '0';

  return i;
}

// *************************
// A note on string copying:
// *************************
//
// As we all know, strcpy() is unsafe because it blindly assumes that the
// source string will fit in the destination buffer. The early solution to this
// is strncpy() which needs to be told the size of the destination buffer.
// strncpy() will then not exceed this buffer, but it has two side effects:
//  (1) It won't null terminate the string if the the source string is larger
//      than the buffer. This is up to the caller.
//  (2) If the source string is smaller than the buffer then IT WILL FILL THE
//      REMAINING BUFFER WITH NULLS. This is a performance issue.
//
// BSD-based systems like MacOS have alternatives like strlcpy() which will
// always null terminate the string just the once, but this function is not
// available on Linux.
//
// The C11 standard adds strncpy_s() which only adds 1 null character if the
// size of the source is smaller than the buffer. But it still won't null
// terminate the string if the source string is larger than the buffer size.
// Supporting the C11 standard also requires at least GCC 4.7, which is not
// standard on CentOS 7.
//
// Another alternative is to use snprintf(), which always inserts a null
// character, but this seems like the sledgehammer approach.
//
// Other solutions include strncat() as well as using strlen() and memcpy().
// Each have their own pros and cons.
//
// That leaves us with writing our own safe strcpy routine that won't exceed
// the destination buffer size, while also ensuring the copied string is
// terminated and terminated just the once. It also returns the number of
// bytes copied (excluding the null character) so that the caller can determine
// if a string truncation occurred.
//
// psDst - Destination buffer
// iDstSize - the size (in bytes) of the destination buffer. This must also include
//            room for a null character too.
// psSrc - the source buffer to read from.
// pbWasTruncated - an optional boolean that is set to true if the string was
//                  truncated and couldn't fit in the buffer. Otherwise just
//                  pass NULL here (or nullptr in C++).
static inline i8 iSafeStrcpy(char* psDst, i8 iDstSize, char* psSrc, b1* pbWasTruncated)
{
  i8 i = 0;

  if (pbWasTruncated) {
    *pbWasTruncated = false;
  }

  if (iDstSize > 0) {
    for (i = 0; i < iDstSize - 1; i++) {
      if (*psSrc == '\0') {
        if (pbWasTruncated) {
          *pbWasTruncated = true;
        }
        break;
      }

      *(psDst++) = *(psSrc++);
    }

    *psDst = '\0';

  } else {
    if (pbWasTruncated) {
      *pbWasTruncated = true;
    }
  }

  return i;
}

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
  i8 i;

  for (i=0; pS[i]; i++) {
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
    uUnsignedResult = uUnsignedResult * 10 + iDigitToI8(*pS);
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
      uUnsignedResult = uUnsignedResult * 10 + iDigitToI8(*pS);
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
  union Bcd_T bcdVal;

  fltArg1.m_iAsI8 = *(--(*ppaW));

  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // Is the float too big to be represented as an integer?
  if (fltArg1.m_dFlt > (d8) C_I8_MAX || fltArg1.m_dFlt < (d8) C_I8_MIN) {
    aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // Get the integral part and work out if it fits in the desired precision.
  // Any left over digits can then be used towards scale.
  i8 iIntegralVal = (i8) fltArg1.m_dFlt;
  i8 iIntegralDigits = iPrecision_signed(iIntegralVal);
  i8 iMantissa = iIntegralVal;
  b1 bIsNeg = false;

  if (iIntegralDigits > iPrecision) {
    aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if (fltArg1.m_dFlt < 0) {
    fltArg1.m_dFlt *= -1;
    bIsNeg = true;
  }

  i8 iScaleDigits = (iPrecision - iIntegralDigits) < iScale ? iPrecision - iIntegralDigits : iScale;

  // Multiply by number of scale digits + 1 for rounding of the last value
  i8 iFracVal = (fltArg1.m_dFlt - (d8) iIntegralVal) * aDynamicT->m_iaPow10[iScaleDigits + 1];

  b1 bRoundUp = iFracVal % 10 >= 5;


  i8 i;
  i8 iDigitVal;
  for (i=iScaleDigits; i>0; i--) {
    iDigitVal = iFracVal / aDynamicT->m_iaPow10[i];
    iMantissa *= 10;
    iMantissa += iDigitVal;
    iFracVal -= iDigitVal * aDynamicT->m_iaPow10[i];
  }

  if (bRoundUp) {
    ++iMantissa;
  }

  if (bIsNeg) {
    iMantissa *= -1;
  }

  bcdVal.m_iMantissa = iMantissa;
  bcdVal.m_uScale = iScaleDigits;
  bcdVal.m_iSpare = 0;

  *(*ppaW)++ = bcdVal.m_iAsI8;
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


static inline void _FLT_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszStrArea, i8* piAreaRemaining, i8 iLength)
{
  union Flt_T fltArg1;

  fltArg1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    fltArg1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 iMaxSize = *piAreaRemaining;

  if (iLength > 0) {
    iMaxSize = *piAreaRemaining < iLength + 1 ? *piAreaRemaining : iLength + 1;
  }

  int iSize = snprintf((*ppszStrArea), iMaxSize, "%.9e", fltArg1.m_dFlt);
  if (iSize >= iMaxSize) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
  } else {
    *(*ppaW)++ = (i8) (*ppszStrArea);
    (*ppszStrArea) += (iSize + 1);
    (*piAreaRemaining) -= (iSize + 1);
  }
}

/*
 * Does not take care of min/max flt to varchar
 *
// how many decimal places , 10?
//WARNING do we need to length of string?

static inline void _FLT_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszStrArea, i8 iLength)
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
      i8 j = iDigitToI8(*psArg++);

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
  i8 iArg1 = *(--(*ppaW));
  char* psArg = (char*) iArg1;

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      *psArg == 0) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Dte_T dteResult;

  i8 nLen = strlen(psArg);
  if (nLen < 10) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if (!isdigit(psArg[0]) ||
      !isdigit(psArg[1]) ||
      !isdigit(psArg[2]) ||
      !isdigit(psArg[3]) ||
      psArg[4] != '-'       ||
      !isdigit(psArg[5]) ||
      !isdigit(psArg[6]) ||
      psArg[7] != '-'       ||
      !isdigit(psArg[8]) ||
      !isdigit(psArg[9]) ) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 iYear  = 0;
  i8 iMonth = 0;
  i8 iDay   = 0;

  iYear *= 10; iYear += iDigitToI8(psArg[0]);
  iYear *= 10; iYear += iDigitToI8(psArg[1]);
  iYear *= 10; iYear += iDigitToI8(psArg[2]);
  iYear *= 10; iYear += iDigitToI8(psArg[3]);

  iMonth *= 10; iMonth += iDigitToI8(psArg[5]);
  iMonth *= 10; iMonth += iDigitToI8(psArg[6]);

  iDay *= 10; iDay += iDigitToI8(psArg[8]);
  iDay *= 10; iDay += iDigitToI8(psArg[9]);

  if (iYear < (i8) C_YEAR_BASE ||
      iYear > (i8) C_YEAR_MAX  ||
      iMonth < 1  ||
      iMonth > 12 ||
      iDay < 1    ||
      iDay > 31) {
    aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  b1 bIsLeapYear = ( (iYear % 4 == 0) && (iYear % 100 != 0) ) || (iYear % 400 == 0);

  // Check days in month
  if ( (!bIsLeapYear && iDay > iDaysPerMonth[iMonth]) ||
       (bIsLeapYear &&  iDay > iDaysPerMonthLeap[iMonth]) ) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // We've got a valid date
  dteResult.m_uYearDelta = iYear - C_YEAR_BASE;
  dteResult.m_uMonth = iMonth;
  dteResult.m_uDay = iDay;
  dteResult.m_uSpare = 0;

  *(*ppaW)++ = dteResult.m_iAsI8;
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

  xTme1.m_uMicro = 0; //xTsp1.m_uMicro;
  xTme1.m_uSecond = xTsp1.m_uSecond;
  xTme1.m_uMinute = xTsp1.m_uMinute;
  xTme1.m_uHour = xTsp1.m_uHour;

  *(*ppaW)++ = xTme1.m_iAsI8;
}


static inline void _VCH_TOTME(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  char* psArg = (char*) iArg1;

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      *psArg == 0) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Tme_T tmeResult;
  i8 iHour = 0;
  i8 iMin  = 0;
  i8 iSec  = 0;

  // The string may contain a time or even a timestamp

  i8 nLen = strlen(psArg);

  if (nLen < 8) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // First try to extract it as a time
  if (isdigit(psArg[0]) ||
      isdigit(psArg[1]) ||
      psArg[2] == ':' ||
      isdigit(psArg[3]) ||
      isdigit(psArg[4]) ||
      psArg[5] == ':' ||
      isdigit(psArg[6]) ||
      isdigit(psArg[7]) ) {

    iHour *= 10; iHour += iDigitToI8(psArg[0]);
    iHour *= 10; iHour += iDigitToI8(psArg[1]);

    iMin *= 10; iMin += iDigitToI8(psArg[3]);
    iMin *= 10; iMin += iDigitToI8(psArg[4]);

    iSec *= 10; iSec += iDigitToI8(psArg[6]);
    iSec *= 10; iSec += iDigitToI8(psArg[7]);

    if (iHour > 23 || iHour < 0) {
      aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
      *(*ppaW)++ = C_DATUM_NULL;
      return;
    }

    if (iMin > 59 || iMin < 0) {
      aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
      *(*ppaW)++ = C_DATUM_NULL;
      return;
    }

    if (iSec > 59 || iSec < 0) {
      aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
      *(*ppaW)++ = C_DATUM_NULL;
      return;
    }

    tmeResult.m_uHour = iHour;
    tmeResult.m_uMinute = iMin;
    tmeResult.m_uSecond = iSec;
    tmeResult.m_uMicro = 0;
    tmeResult.m_uSpare = 0;
    // fall out

  } else {
    // Parse it as a timestamp first. This may generate an error or a valid timestamp
    aDynamicT->m_iErrorCode = EC_NONE;
    _VCH_TOTSP(ppaW, aDynamicT);

    if (aDynamicT->m_iErrorCode != EC_NONE) {
      // We've already got an error and a null value on the stack
      return;
    }

    // We've got a valid timestamp. Extract the time portion.
    union Tsp_T tspVal;
    tspVal.m_iAsI8 = *(--(*ppaW));

    tmeResult.m_uHour = tspVal.m_uHour;
    tmeResult.m_uMinute = tspVal.m_uMinute;
    tmeResult.m_uSecond = tspVal.m_uSecond;
  }

  *(*ppaW)++ = tmeResult.m_iAsI8;
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
  i8 iArg1 = *(--(*ppaW));
  char* psArg = (char*) iArg1;

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL ||
      *psArg == 0) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  union Tsp_T tspResult;
  i8 iYear  = 0;
  i8 iMonth = 0;
  i8 iDay   = 0;
  i8 iHour = 0;
  i8 iMin  = 0;
  i8 iSec  = 0;
  i8 iMicro = 0;

  i8 iLen = strlen(psArg);

  if (iLen < 19              ||
      !isdigit(psArg[0])  ||
      !isdigit(psArg[1])  ||
      !isdigit(psArg[2])  ||
      !isdigit(psArg[3])  ||
      psArg[4] != '-'        ||
      !isdigit(psArg[5])  ||
      !isdigit(psArg[6])  ||
      psArg[7] != '-'        ||
      !isdigit(psArg[8])  ||
      !isdigit(psArg[9])  ||
      psArg[10] != ' '       ||
      !isdigit(psArg[11]) ||
      !isdigit(psArg[12]) ||
      psArg[13] != ':'       ||
      !isdigit(psArg[14]) ||
      !isdigit(psArg[15]) ||
      psArg[16] != ':'       ||
      !isdigit(psArg[17]) ||
      !isdigit(psArg[18]) ) {

    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  iYear *= 10; iYear += iDigitToI8(psArg[0]);
  iYear *= 10; iYear += iDigitToI8(psArg[1]);
  iYear *= 10; iYear += iDigitToI8(psArg[2]);
  iYear *= 10; iYear += iDigitToI8(psArg[3]);

  iMonth *= 10; iMonth += iDigitToI8(psArg[5]);
  iMonth *= 10; iMonth += iDigitToI8(psArg[6]);

  iDay *= 10; iDay += iDigitToI8(psArg[8]);
  iDay *= 10; iDay += iDigitToI8(psArg[9]);

  iHour *= 10; iHour += iDigitToI8(psArg[11]);
  iHour *= 10; iHour += iDigitToI8(psArg[12]);

  iMin *= 10; iMin += iDigitToI8(psArg[14]);
  iMin *= 10; iMin += iDigitToI8(psArg[15]);

  iSec *= 10; iSec += iDigitToI8(psArg[17]);
  iSec *= 10; iSec += iDigitToI8(psArg[18]);

  // Check for microseconds
  if (iLen > 19 && psArg[19] == '.') {

    i8 iIdx;
    for (iIdx = 20; iIdx < 26, psArg[iIdx]; iIdx++) {

      if (!isdigit(psArg[iIdx])) {
        aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
        *(*ppaW)++ = C_DATUM_NULL;
        return;
      }

      iMicro *= 10;
      iMicro += iDigitToI8(psArg[iIdx]);
    }

    // May not have specified all fractional digits
    while (iIdx < 26) {
      iMicro *= 10;
    }
  }

  // Now validate the results

  if (iYear  < (i8) C_YEAR_BASE ||
      iYear  > (i8) C_YEAR_MAX  ||
      iMonth < 1  ||
      iMonth > 12 ||
      iDay   < 1  ||
      iDay   > 31) {
    aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  b1 bIsLeapYear = ( (iYear % 4 == 0) && (iYear % 100 != 0) ) || (iYear % 400 == 0);

  // Check days in month
  if ( (!bIsLeapYear && iDay > iDaysPerMonth[iMonth]) ||
       (bIsLeapYear &&  iDay > iDaysPerMonthLeap[iMonth]) ) {
    aDynamicT->m_iErrorCode = EC_INVALID_VALUE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if ( iHour > 23     ||  iHour < 0 ||
        iMin > 59     ||   iMin < 0   ||
        iSec > 59     ||   iSec < 0   ||
      iMicro > 999999 || iMicro < 0) {
    aDynamicT->m_iErrorCode = EC_VALUE_OUT_OF_RANGE;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // We've got a valid timestamp
  tspResult.m_uYearDelta = iYear - C_YEAR_BASE;
  tspResult.m_uMonth     = iMonth;
  tspResult.m_uDay       = iDay;
  tspResult.m_uHour      = iHour;
  tspResult.m_uMinute    = iMin;
  tspResult.m_uSecond    = iSec;
  tspResult.m_uMicro     = iMicro;
  tspResult.m_uSpare     = 0;

  *(*ppaW)++ = tspResult.m_iAsI8;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO VCH
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static inline void _BOL_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining, i8 iLength)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 nSize = iArg1 ? 5 : 6; // including null char

  if (*piAreaRemaining < nSize || (iLength > 0 && iLength + 1 < nSize)) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if (iArg1)
    strcpy((*ppszDstArea), "true");
  else
    strcpy((*ppszDstArea), "false");

  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}



static inline void _BCD_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining, i8 iLength)
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
  i8 nSize = 0;
  i8 iSizeLeft = *piAreaRemaining;
  if (iLength > 0 && iLength + 1 < *piAreaRemaining) {
    iSizeLeft = iLength + 1;
  }

  while (bcdArg1.m_uScale && iSizeLeft > 0) {
    *p++ = (bcdArg1.m_iMantissa % 10) + '0';
    bcdArg1.m_iMantissa /= 10ll;
    bcdArg1.m_uScale--;
    --iSizeLeft;
    ++nSize;
  }

  if (iSizeLeft > 0) {
    *p++ = '.';
    --iSizeLeft;
    ++nSize;
  }

  i1 iFirst = 1; // Always print the first digit after the decimal point,
                 // especially if it is a zero.

  while ((bcdArg1.m_iMantissa || iFirst) && iSizeLeft > 0) {
    *p++ = (bcdArg1.m_iMantissa % 10) + '0';
    bcdArg1.m_iMantissa /= 10ll;
    iFirst = 0;
    --iSizeLeft;
    ++nSize;
  }

  if (bNegative && iSizeLeft > 0) {
    *p++ = '-';
    --iSizeLeft;
    ++nSize;
  }

  if (iSizeLeft == 0) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *p = '\0';
  --iSizeLeft;
  ++nSize;

  while (--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }

  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}

static inline void _TME_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining, i8 iLength)
{
  union Tme_T xTme1;

  xTme1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xTme1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 nSize = 9; //+ (xTme1.m_uMicro != 0 ? 7 : 0);
  if (*piAreaRemaining < nSize || (iLength > 0 && iLength + 1 < nSize)) {
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

//  if(xTme1.m_uMicro != 0) {
//    *p++ = '.';
//
//    u8 uFactor = 100000;
//    u8 uDigit;
//    tmp = xTme1.m_uMicro;
//
//    while (uFactor) {
//      uDigit = tmp / uFactor;
//      *p++ = uDigit + '0';
//
//      tmp -= (uDigit * uFactor);
//      uFactor /= 10;
//    }
//  }

  *p = '\0';

  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}

static inline void _DTE_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining, i8 iLength)
{
  union Dte_T xDte1;

  xDte1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xDte1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 nSize = 11;
  if (*piAreaRemaining < nSize || (iLength > 0 && iLength + 1 < nSize)) {
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

  *p = '\0';

  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}


static inline void _INT_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining, i8 iLength)
{
  i8 iArg1 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  i8 nSize = 0;
  i8 bNegative = 0;

  if (iArg1 < 0) {
    iArg1 = -iArg1;
    bNegative = 1;
  }

  i8 iSizeRemaining = *piAreaRemaining;
  if (iLength > 0 && iLength + 1 < *piAreaRemaining) {
    iSizeRemaining = iLength + 1;
  }

  if (iSizeRemaining < 1) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  char *p = (*ppszDstArea), *q = (*ppszDstArea);
  do {
    *p++ = (iArg1 % 10) + '0';
    iArg1 /= 10ll;
    ++nSize;
    --iSizeRemaining;
  } while (iArg1 && iSizeRemaining >= 1);

  if (bNegative && iSizeRemaining >= 1) {
    *p++ = '-';
    ++nSize;
    --iSizeRemaining;
  }

  if (iSizeRemaining == 0) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  *p = '\0';
  ++nSize;
  --iSizeRemaining;

  while (--p > q) {
    char c = *p;
    *p = *q;
    *q++ = c;
  }
  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}

static inline void _IVL_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining, i8 iLength)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _IVY_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining, i8 iLength)
{
  aDynamicT->m_iErrorCode = EC_NOT_IMPLEMENTED;
  *(*ppaW)++ = C_DATUM_NULL;
}

static inline void _TSP_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea, i8* piAreaRemaining, i8 iLength)
{
  union Tsp_T xTsp1;

  xTsp1.m_iAsI8 = *(--(*ppaW));
  if (aDynamicT->m_iErrorCode != EC_NONE ||
    xTsp1.m_iAsI8 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  const i8 nSize = 20 + (xTsp1.m_uMicro != 0 ? 7 : 0);
  if (*piAreaRemaining < nSize || (iLength > 0 && iLength + 1 < nSize)) {
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

  *p++ = 0;

  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += nSize;
  (*piAreaRemaining) -= nSize;
}

static inline void _VCH_TOVCH(i8** ppaW, struct Dynamic_T* aDynamicT, char** ppszDstArea,
                              i8* piAreaRemaining, i8 iLength)
{
  i8 iArg1 = *(--(*ppaW));
  char *pS = (char*)iArg1;

  if (aDynamicT->m_iErrorCode != EC_NONE ||
      iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  if (iLength <= 0) {
    *(*ppaW)++ = iArg1;
    return;
  }

  // Only copy iLength number of characters, or whatever will fit in the buffer,
  // whichever is smallest.
  i8 iMaxBytes = (iLength + 1) < *piAreaRemaining ? (iLength + 1) : *piAreaRemaining;
  b1 bWasTruncated = false;
  i8 iBytesCopied = 1 + iSafeStrcpy(*ppszDstArea, iMaxBytes, pS, &bWasTruncated);

  if (bWasTruncated) {
    aDynamicT->m_iErrorCode = EC_STRING_TRUNCATION;
    *(*ppaW)++ = C_DATUM_NULL;
    return;
  }

  // All good
  *(*ppaW)++ = (i8)(*ppszDstArea);
  (*ppszDstArea) += iBytesCopied;
  (*piAreaRemaining) -= iBytesCopied;
}

// nothing to do
// unk remains unk
static inline void _UNK_TOANY(i8** ppaW, struct Dynamic_T* aDynamicT)
{

}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
