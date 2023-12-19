/*============================================================================*/
/* MathMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Utilities
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Overflow detection bcd
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _OF_BCDADD                                                         \
(                                                                          \
  (bcdArg1.m_iMantissa > 0 &&                                              \
  bcdArg2.m_iMantissa > C_BCD_MAX - bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  (bcdArg1.m_iMantissa < 0 &&                                              \
  bcdArg2.m_iMantissa < C_BCD_MIN - bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_UNDERFLOW :                                                \
  EC_NONE                                                                  \
)

// TODO: OVERFLOW CHECK NOT WORKING WITH arg1 = 0 and arg2 is C_MIN
#define _OF_BCDSUB                                                         \
(                                                                          \
  (bcdArg1.m_iMantissa < 0 &&                                              \
  bcdArg2.m_iMantissa > C_BCD_MAX + bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  (bcdArg1.m_iMantissa > 0 &&                                              \
  bcdArg2.m_iMantissa < C_BCD_MIN + bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_UNDERFLOW :                                                \
  EC_NONE                                                                  \
)

#define _OF_BCDMUL                                                         \
(                                                                          \
  EC_NONE                                                                  \
)

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

#define _ADD_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
        if (bcdArg1.m_uScale != bcdArg2.m_uScale) {                        \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          _RESCALE_BCD_MAX_OF_TWO(eErrorCode)                              \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
                                                                           \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
        eErrorCode = _OF_BCDADD;                                           \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          bcdArg1.m_iMantissa = bcdArg1.m_iMantissa + bcdArg2.m_iMantissa; \
          *paW++ = bcdArg1.m_iAsI8;                                        \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

#define _DIV_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (bcdArg1.m_uScale != bcdArg2.m_uScale) {                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD_MAX_OF_TWO(eErrorCode)                                \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
                                                                           \
      if(eErrorCode != EC_NONE) {                                        \
        *paW++ = C_DATUM_NULL;                                           \
      }                                                                  \
      else {                                                             \
        eErrorCode = _OF_BCDDIV;                                         \
        if(eErrorCode != EC_NONE) {                                      \
          *paW++ = C_DATUM_NULL;                                         \
        }                                                                \
        else {                                                           \
          fltArg1.m_dFlt = ((d8)bcdArg1.m_iMantissa) / ((d8)bcdArg2.m_iMantissa); \
          *paW++ = bcdArg1.m_uScale + 1;                                 \
          _PSH_FLT(fltArg1.m_dFlt)                                       \
          _FLT_TOBCD_TO_SCALE(eErrorCode)                                \
          _ROUND_BCD(eErrorCode)                                         \
          if(eErrorCode != EC_NONE) {                                    \
            --paW;                                                       \
            *paW++ = C_DATUM_NULL;                                       \
          }                                                              \
        }                                                                \
     }                                                                   \
   }                                                                     \
 }

#define _MUL_BCDBCD(eErrorCode)                                          \
{                                                                        \
  bcdArg2.m_iAsI8 = *(--paW);                                            \
  bcdArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
      bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
      bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    eErrorCode = _OF_BCDMUL;                                             \
    if (eErrorCode != EC_NONE) {                                         \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else {                                                               \
      i8 iScaleNew = bcdArg1.m_uScale > bcdArg2.m_uScale ? bcdArg1.m_uScale : bcdArg2.m_uScale;  \
      i8 iResult = bcdArg1.m_iMantissa * bcdArg2.m_iMantissa;            \
      iScale = bcdArg1.m_uScale + bcdArg2.m_uScale;                      \
      if (iScale != iScaleNew) {                                         \
        bcdArg1.m_iMantissa = iResult / aDynamicT->m_iaPow10[iScale - iScaleNew - 1]; \
        bcdArg1.m_uScale = iScaleNew + 1;                                \
        *paW++ = bcdArg1.m_iAsI8;                                        \
        _ROUND_BCD(eErrorCode)                                           \
      }                                                                  \
      else {                                                             \
        bcdArg1.m_iMantissa = iResult;                                   \
        bcdArg1.m_uScale = iScaleNew;                                    \
       *paW++ = bcdArg1.m_iAsI8;                                         \
       }                                                                 \
    }                                                                    \
  }                                                                      \
}

#define _NEG_BCD(eErrorCode)                                               \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
       bcdArg1.m_iAsI8 == C_DATUM_NULL) {                                  \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      bcdArg1.m_iMantissa = - bcdArg1.m_iMantissa;                         \
      *paW++ = bcdArg1.m_iAsI8;                                            \
    }                                                                      \
  }

#define _SUB_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
        if (bcdArg1.m_uScale != bcdArg2.m_uScale) {                        \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          *paW++ = bcdArg2.m_iAsI8;                                       \
          _RESCALE_BCD_MAX_OF_TWO(eErrorCode)                              \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
                                                                           \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
        eErrorCode = _OF_BCDSUB;                                           \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          bcdArg1.m_iMantissa = bcdArg1.m_iMantissa - bcdArg2.m_iMantissa; \
          *paW++ = bcdArg1.m_iAsI8;                                        \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

#define _SUM_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (bcdArg1.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = bcdArg2.m_iAsI8;                                            \
    }                                                                      \
    else if (bcdArg2.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = bcdArg1.m_iAsI8;                                            \
    }                                                                      \
    else {                                                                 \
       if (bcdArg1.m_uScale != bcdArg2.m_uScale) {                         \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          _RESCALE_BCD_MAX_OF_TWO(eErrorCode)                              \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
        if(eErrorCode != EC_NONE) {                                        \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        eErrorCode = _OF_BCDADD;                                           \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          bcdArg1.m_iMantissa = bcdArg1.m_iMantissa + bcdArg2.m_iMantissa; \
          *paW++ = bcdArg1.m_iAsI8;                                        \
        }                                                                  \
      }                                                                    \
    }                                                                      \
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


#define _ADD_DTEIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xDte1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                           \
                                                                           \
    if (ivlArg2.m_xIvl.m_bIsNeg)                                           \
      iArg2 = (-1) * iArg2;                                                \
                                                                           \
    struct tm xDateTime = {0} ;                                            \
                                                                           \
    xDateTime.tm_year = xDte1.m_uYearDelta;                                \
    xDateTime.tm_mon  = xDte1.m_uMonth - 1;                                \
    xDateTime.tm_mday = xDte1.m_uDay;                                      \
    xDateTime.tm_hour = 0;                                                 \
    xDateTime.tm_min  = 0;                                                 \
    xDateTime.tm_sec  = iArg2/_C_1M;                                       \
                                                                           \
    time_t iSecondsSincePosixEpoch = timegm(&xDateTime);                   \
                                                                           \
    i8 iMicroSeconds = iArg2%_C_1M;                                        \
                                                                           \
    if (iMicroSeconds && iArg2 < 0) {                                      \
      xDateTime.tm_sec--;                                                  \
      iMicroSeconds = _C_1M - iMicroSeconds;                               \
    }                                                                      \
    xTsp2.m_iAsI8 = 0;                                                     \
                                                                           \
    xTsp2.m_uYearDelta = xDateTime.tm_year;                                \
    xTsp2.m_uMonth     = xDateTime.tm_mon + 1;                             \
    xTsp2.m_uDay       = xDateTime.tm_mday;                                \
    xTsp2.m_uHour      = xDateTime.tm_hour;                                \
    xTsp2.m_uMinute    = xDateTime.tm_min;                                 \
    xTsp2.m_uSecond    = xDateTime.tm_sec;                                 \
    xTsp2.m_uMicro     = iMicroSeconds;                                    \
                                                                           \
    *paW++ = xTsp2.m_iAsI8;                                                \
  }                                                                        \
}

#define _ADD_DTEIVY(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xDte1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      i8 iYear_arg2 = iArg2/12;                                            \
      i8 iMonth_arg2 = iArg2%12;                                           \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg) {                                       \
        iYear_arg2 = -1 * iYear_arg2;                                      \
        iMonth_arg2 = (-1) * iMonth_arg2;                                  \
      }                                                                    \
                                                                           \
      i8 iYearDelta;                                                       \
      i8 iMonth;                                                           \
      i8 iDay;                                                             \
      iYearDelta = xDte1.m_uYearDelta + iYear_arg2;                        \
      iMonth = xDte1.m_uMonth + iMonth_arg2;                               \
      iDay = xDte1.m_uDay;                                                 \
                                                                           \
      if (iYearDelta > C_YEAR_RANGE) {                                     \
        eErrorCode = EC_DATE_OUT_OF_RANGE;                                 \
      }                                                                    \
      else {                                                               \
        _CONVERT_TO_VALID_DATE                                             \
        xDte2.m_uYearDelta = iYearDelta;                                   \
        xDte2.m_uMonth = iMonth;                                           \
        xDte2.m_uDay = iDay;                                               \
        xDte2.m_uSpare = 0;                                                \
        *paW++ = xDte2.m_iAsI8;                                            \
      }                                                                    \
   }                                                                       \
 }

#define _ADD_IVDDTE(eErrorCode)                                            \
  {                                                                        \
    xDte2.m_iAsI8 = *(--paW);                                              \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xDte2.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
    iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                           \
                                                                           \
    if (ivlArg1.m_xIvl.m_bIsNeg)                                           \
      iArg1 = (-1) * iArg1;                                                \
                                                                           \
    struct tm xDateTime = {0} ;                                            \
                                                                           \
    xDateTime.tm_year = xDte2.m_uYearDelta;                                \
    xDateTime.tm_mon  = xDte2.m_uMonth - 1;                                \
    xDateTime.tm_mday = xDte2.m_uDay;                                      \
    xDateTime.tm_hour = 0;                                                 \
    xDateTime.tm_min  = 0;                                                 \
    xDateTime.tm_sec  = iArg1/_C_1M;                                       \
                                                                           \
      if (iMicroSeconds && iArg2 < 0) {                                    \
      xDateTime.tm_sec--;                                                  \
      iMicroSeconds = _C_1M - iMicroSeconds;                               \
    }                                                                      \
                                                                           \
    time_t iSecondsSincePosixEpoch = timegm(&xDateTime);                   \
                                                                           \
    xTsp2.m_iAsI8 = 0;                                                     \
    xTsp2.m_uYearDelta = xDateTime.tm_year;                                \
    xTsp2.m_uMonth     = xDateTime.tm_mon + 1;                             \
    xTsp2.m_uDay       = xDateTime.tm_mday;                                \
    xTsp2.m_uHour      = xDateTime.tm_hour;                                \
    xTsp2.m_uMinute    = xDateTime.tm_min;                                 \
    xTsp2.m_uSecond    = xDateTime.tm_sec;                                 \
    xTsp2.m_uMicro     = 0;                                                \
                                                                           \
    *paW++ = xTsp2.m_iAsI8;                                                \
    xTsp2.m_uMicro     = iMicroSeconds;                                    \
  }                                                                        \
}

#define _ADD_IVYDTE(eErrorCode)                                            \
  {                                                                        \
    xDte2.m_iAsI8 = *(--paW);                                              \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xDte2.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      i8 iYear_arg1 = iArg1/12;                                            \
      i8 iMonth_arg1 = iArg1%12;                                           \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg) {                                       \
        iYear_arg1 = (-1) * iYear_arg1;                                    \
        iMonth_arg1 = (-1) * iMonth_arg1;                                  \
      }                                                                    \
                                                                           \
      i8 iYearDelta;                                                       \
      i8 iMonth;                                                           \
      i8 iDay;                                                             \
      iYearDelta = xDte2.m_uYearDelta + iYear_arg1;                        \
      iMonth = xDte2.m_uMonth + iMonth_arg1;                               \
      iDay = xDte2.m_uDay;                                                 \
                                                                           \
      if (iYearDelta > C_YEAR_RANGE) {                                     \
        eErrorCode = EC_DATE_OUT_OF_RANGE;                                 \
      }                                                                    \
      else {                                                               \
        _CONVERT_TO_VALID_DATE                                             \
        xDte1.m_uYearDelta = iYearDelta;                                   \
        xDte1.m_uMonth = iMonth;                                           \
        xDte1.m_uDay = iDay;                                               \
        xDte1.m_uSpare = 0;                                                \
        *paW++ = xDte1.m_iAsI8;                                            \
      }                                                                    \
   }                                                                       \
 }

#define _SUB_DTEDTE(eErrorCode)                                            \
  {                                                                        \
    xDte2.m_iAsI8 = *(--paW);                                              \
    xDte1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      struct tm xDateTime1 = {0} ;                                         \
                                                                           \
      xDateTime1.tm_year = xDte1.m_uYearDelta;                             \
      xDateTime1.tm_mon  = xDte1.m_uMonth - 1;                             \
      xDateTime1.tm_mday = xDte1.m_uDay;                                   \
      xDateTime1.tm_hour = 0;                                              \
      xDateTime1.tm_min  = 0;                                              \
      xDateTime1.tm_sec  = 0;                                              \
                                                                           \
      time_t iSecondsSincePosixEpoch1 = timegm(&xDateTime1);               \
                                                                           \
      struct tm xDateTime2 = {0} ;                                         \
                                                                           \
      xDateTime2.tm_year = xDte2.m_uYearDelta;                             \
      xDateTime2.tm_mon  = xDte2.m_uMonth - 1;                             \
      xDateTime2.tm_mday = xDte2.m_uDay;                                   \
      xDateTime2.tm_hour = 0;                                              \
      xDateTime2.tm_min  = 0;                                              \
      xDateTime2.tm_sec  = 0;                                              \
                                                                           \
      time_t iSecondsSincePosixEpoch2 = timegm(&xDateTime2);               \
                                                                           \
      i8 iSeconds = iSecondsSincePosixEpoch1 - iSecondsSincePosixEpoch2;   \
                                                                           \
      wIvlResult.m_iAsI8 = 0;                                              \
      wIvlResult.m_xIvl.m_eType = IVL_DAY;                                 \
                                                                           \
      if (iSeconds < 0) {                                                  \
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  -1 * iSeconds * _C_1M;   \
        wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
      }                                                                    \
      else {                                                               \
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  iSeconds*_C_1M;          \
        wIvlResult.m_xIvl.m_bIsNeg = 0;                                    \
      }                                                                    \
      *paW++ = wIvlResult.m_iAsI8;                                         \
  }                                                                        \
}

#define _SUB_DTEIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xDte1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
    iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                           \
                                                                           \
    if (ivlArg2.m_xIvl.m_bIsNeg)                                           \
      iArg2 = (-1) * iArg2;                                                \
                                                                           \
    struct tm xDateTime = {0} ;                                            \
                                                                           \
    xDateTime.tm_year = xDte1.m_uYearDelta;                                \
    xDateTime.tm_mon  = xDte1.m_uMonth - 1;                                \
    xDateTime.tm_mday = xDte1.m_uDay;                                      \
    xDateTime.tm_hour = 0;                                                 \
    xDateTime.tm_min  = 0;                                                 \
    xDateTime.tm_sec  -= iArg2/_C_1M;                                      \
                                                                           \
    i8 iMicroSeconds = iArg2%_C_1M;                                        \
                                                                           \
    if (iMicroSeconds) {                                                   \
      xDateTime.tm_sec--;                                                  \
      iMicroSeconds = _C_1M - iMicroSeconds;                               \
    }                                                                      \
                                                                           \
    time_t iSecondsSincePosixEpoch = timegm(&xDateTime);                   \
    xTsp2.m_iAsI8 = 0;                                                     \
    xTsp2.m_uYearDelta = xDateTime.tm_year;                                \
    xTsp2.m_uMonth     = xDateTime.tm_mon + 1;                             \
    xTsp2.m_uDay       = xDateTime.tm_mday;                                \
    xTsp2.m_uHour      = xDateTime.tm_hour;                                \
    xTsp2.m_uMinute    = xDateTime.tm_min;                                 \
    xTsp2.m_uSecond    = xDateTime.tm_sec;                                 \
    xTsp2.m_uMicro     = iMicroSeconds;                                    \
                                                                           \
    *paW++ = xTsp2.m_iAsI8;                                                \
  }                                                                        \
}

#define _SUB_DTEIVY(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xDte1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      i8 iYear_arg2 = iArg2/12;                                            \
      i8 iMonth_arg2 = iArg2%12;                                           \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg) {                                       \
        iYear_arg2 = (-1) * iYear_arg2;                                    \
        iMonth_arg2 = (-1) * iMonth_arg2;                                  \
      }                                                                    \
                                                                           \
      i8 iYearDelta;                                                       \
      i8 iMonth;                                                           \
      i8 iDay;                                                             \
      iYearDelta = xDte1.m_uYearDelta - iYear_arg2;                        \
      iMonth = xDte1.m_uMonth - iMonth_arg2;                               \
      iDay = xDte1.m_uDay;                                                 \
                                                                           \
      if (iYearDelta > C_YEAR_RANGE) {                                     \
        eErrorCode = EC_DATE_OUT_OF_RANGE;                                 \
      }                                                                    \
      else {                                                               \
        _CONVERT_TO_VALID_DATE                                             \
        xDte2.m_uYearDelta = iYearDelta;                                   \
        xDte2.m_uMonth = iMonth;                                           \
        xDte2.m_uDay = iDay;                                               \
        xDte1.m_uSpare = 0;                                                \
        *paW++ = xDte2.m_iAsI8;                                            \
      }                                                                    \
   }                                                                       \
 }


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// FLOAT
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



#define _ADD_FLTFLT(eErrorCode) \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    eErrorCode = EC_NONE;                                                \
    if(eErrorCode != EC_NONE)                                            \
    {                                                                    \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else {                                                               \
      if(fltArg1.m_dFlt >= C_FLT_MAX ||                                  \
        fltArg2.m_dFlt >= C_FLT_MAX) {                                   \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
        *paW++ = C_DATUM_NULL;                                           \
      }                                                                  \
      else if(fltArg1.m_dFlt <  (d8)-C_FLT_MAX ||                        \
        fltArg2.m_dFlt < (d8)-C_FLT_MAX) {                               \
          eErrorCode = EC_ARITHMETIC_UNDERFLOW;                          \
          *paW++ = C_DATUM_NULL;                                         \
      }                                                                  \
      else                                                               \
      {                                                                  \
        fltArg1.m_dFlt += fltArg2.m_dFlt;                                \
        if(fltArg1.m_dFlt > C_FLT_MAX) {                                 \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
          *paW++ = C_DATUM_NULL;                                         \
        }                                                                \
        else                                                             \
          *paW++ = fltArg1.m_iAsI8;                                      \
      }                                                                  \
    }                                                                    \
  }                                                                      \
}


#define _SUM_FLTFLT(eErrorCode)                                          \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE) {                                           \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else if (fltArg1.m_iAsI8 == C_DATUM_NULL) {                            \
    *paW++ = fltArg2.m_iAsI8;                                            \
  }                                                                      \
  else if (fltArg2.m_iAsI8 == C_DATUM_NULL) {                            \
     *paW++ = fltArg1.m_iAsI8;                                           \
  }                                                                      \
  else {                                                                 \
    if (fltArg1.m_dFlt >= C_FLT_MAX ||                                   \
        fltArg2.m_dFlt >= C_FLT_MAX) {                                   \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else if (fltArg1.m_dFlt <  (d8)C_FLT_MIN ||                          \
      fltArg2.m_dFlt < (d8)C_FLT_MIN) {                                  \
      eErrorCode = EC_ARITHMETIC_UNDERFLOW;                              \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else {                                                               \
      fltArg1.m_dFlt += fltArg2.m_dFlt;                                  \
      if(fltArg1.m_dFlt > C_FLT_MAX) {                                   \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
        *paW++ = C_DATUM_NULL;                                           \
      }                                                                  \
      else                                                               \
        *paW++ = fltArg1.m_iAsI8;                                        \
    }                                                                    \
  }                                                                      \
}


#define _DIV_FLTFLT(eErrorCode)                                          \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    if (fltArg2.m_iAsI8 == 0) {                                          \
      *paW++ = C_DATUM_NULL;                                             \
      eErrorCode = EC_DIVIDE_BY_ZERO;                                    \
    }                                                                    \
    else {                                                               \
      if(fltArg1.m_dFlt > C_FLT_MAX ||                                   \
        fltArg2.m_dFlt > C_FLT_MAX) {                                    \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
        *paW++ = C_DATUM_NULL;                                           \
      }                                                                  \
      else if(fltArg1.m_dFlt <  C_FLT_MIN ||                             \
        fltArg2.m_dFlt < C_FLT_MIN) {                                    \
          eErrorCode = EC_ARITHMETIC_UNDERFLOW;                          \
          *paW++ = C_DATUM_NULL;                                         \
      }                                                                  \
      else {                                                             \
        fltArg1.m_dFlt /= fltArg2.m_dFlt;                                \
        *paW++ = fltArg1.m_iAsI8;                                        \
      }                                                                  \
    }                                                                    \
  }                                                                      \
}

#define _NEG_FLT(eErrorCode)                                             \
{                                                                        \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    fltArg1.m_dFlt = (double)(-1)*fltArg1.m_dFlt;                        \
    *paW++ = fltArg1.m_iAsI8;                                            \
  }                                                                      \
}

#define _SUB_FLTFLT(eErrorCode)                                          \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    if(fltArg1.m_dFlt > C_FLT_MAX ||                                     \
      fltArg2.m_dFlt > C_FLT_MAX) {                                      \
      eErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else if(fltArg1.m_dFlt <  C_FLT_MIN ||                               \
    fltArg2.m_dFlt < C_FLT_MIN) {                                        \
      eErrorCode = EC_ARITHMETIC_UNDERFLOW;                              \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else {                                                               \
      fltArg1.m_dFlt -= fltArg2.m_dFlt;                                  \
      *paW++ = fltArg1.m_iAsI8;                                          \
    }                                                                    \
  }                                                                      \
}



#define _MUL_FLTFLT(eErrorCode)                                          \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    if(fltArg1.m_dFlt > C_FLT_MAX ||                                   \
      fltArg2.m_dFlt > C_FLT_MAX) {                                    \
      eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
      *paW++ = C_DATUM_NULL;                                           \
    }                                                                  \
    else if(fltArg1.m_dFlt <  (d8)-C_FLT_MAX ||                        \
    fltArg2.m_dFlt < (d8)-C_FLT_MAX) {                                 \
      eErrorCode = EC_ARITHMETIC_UNDERFLOW;                            \
      *paW++ = C_DATUM_NULL;                                           \
    }                                                                  \
    else {                                                             \
      fltArg1.m_dFlt *= fltArg2.m_dFlt;                                \
      if(fltArg1.m_dFlt > C_FLT_MAX) {                                 \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                           \
        *paW++ = C_DATUM_NULL;                                         \
      }                                                                \
      else                                                             \
      if(fltArg1.m_dFlt < C_FLT_MIN) {                                 \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                           \
        *paW++ = C_DATUM_NULL;                                         \
      }                                                                \
      else                                                             \
        *paW++ = fltArg1.m_iAsI8 ;                                     \
    }                                                                  \
  }                                                                    \
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// inc arg1
#define _INC_INT(eErrorCode)                                               \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ =  ++iArg1;                                                   \
    }                                                                      \
  }

// inc arg1 if arg2 is not null
#define _INC_IFNOTNULL(eErrorCode)                                         \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode == EC_NONE &&                                           \
        iArg2 != C_DATUM_NULL) {                                           \
      *paW++ = ++iArg1;                                                    \
    }                                                                      \
    else {                                                                 \
      *paW++ =  iArg1;                                                     \
    }                                                                      \
  }

#define _ADD_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      eErrorCode = _OF_I8ADD;                                              \
      if(eErrorCode != EC_NONE)                                            \
      {                                                                    \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else                                                                 \
      {                                                                    \
        *paW++ = iArg1 + iArg2;                                            \
      }                                                                    \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INTERVALS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _OUT_OF_RANGE_IVY                                                 \
(                                                                         \
  (iMonths > 0 &&  iMonths > _C_IVL_MONTH_MAX ) ? EC_INTERVAL_OUT_OF_RANGE :      \
  (iMonths < 0 &&  (-1*iMonths) > _C_IVL_MONTH_MAX ) ? EC_INTERVAL_OUT_OF_RANGE : \
  EC_NONE                                                                 \
)

#define _OUT_OF_RANGE_IVD \
(                                                                         \
  (iMicros > 0 &&  iMicros > _C_IVL_USEC_MAX ) ? EC_INTERVAL_OUT_OF_RANGE :      \
  (iMicros < 0 &&  (-1*iMicros) > _C_IVL_USEC_MAX ) ? EC_INTERVAL_OUT_OF_RANGE : \
  EC_NONE                                                                 \
  )



#define _ADD_IVDIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * iArg1;                                              \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * iArg2;                                              \
                                                                           \
      i8 iMicros = iArg1 + iArg2;                                          \
                                                                           \
      eErrorCode = _OUT_OF_RANGE_IVD;                                      \
                                                                           \
      if (eErrorCode != EC_NONE) {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
                                                                           \
        wIvlResult.m_iAsI8 = 0;                                            \
                                                                           \
        if (iMicros < 0) {                                                 \
          wIvlResult.m_xIvl.m_bIsNeg = 1;                                  \
          wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = -1*iMicros;             \
        }                                                                  \
        else {                                                             \
          wIvlResult.m_xIvl.m_bIsNeg = 0;                                  \
          wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;                \
        }                                                                  \
        wIvlResult.m_xIvl.m_eType =                                        \
        eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType];\
        *paW++ = wIvlResult.m_iAsI8;                                       \
      }                                                                    \
   }                                                                       \
 }


#define _ADD_IVYIVY(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * iArg1;                                              \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * iArg2;                                              \
                                                                           \
      i8 iMonths = iArg1 + iArg2;                                          \
      eErrorCode = _OUT_OF_RANGE_IVY;                                      \
                                                                           \
      if(eErrorCode != EC_NONE)  {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
                                                                           \
        wIvlResult.m_iAsI8 = 0;                                            \
                                                                           \
        if (iMonths < 0) {                                                 \
          wIvlResult.m_xIvl.m_bIsNeg = 1;                                  \
          wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = -1*iMonths;             \
        }                                                                  \
        else {                                                             \
          wIvlResult.m_xIvl.m_bIsNeg = 0;                                  \
          wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iMonths;                \
        }                                                                  \
        wIvlResult.m_xIvl.m_eType =                                        \
        eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType];\
        *paW++ = wIvlResult.m_iAsI8;                                       \
      }                                                                    \
   }                                                                       \
 }

#define _DIV_IVDINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (iArg2 == 0) {                                                    \
        *paW++ = C_DATUM_NULL;                                             \
        eErrorCode = EC_DIVIDE_BY_ZERO;                                    \
      }                                                                    \
      else {                                                               \
                                                                           \
       wIvlResult.m_iAsI8 = 0;                                             \
       wIvlResult.m_xIvl.m_eType = ivlArg1.m_xIvl.m_eType;                 \
                                                                           \
       iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                        \
                                                                           \
       if ( (iArg2 < 0 && ! ivlArg1.m_xIvl.m_bIsNeg) ||                    \
            (iArg2 > 0 && ivlArg1.m_xIvl.m_bIsNeg) )                       \
          wIvlResult.m_xIvl.m_bIsNeg = 1;                                  \
                                                                           \
       if (iArg2 < 0)                                                      \
         iArg2 = (-1) * iArg2;                                             \
                                                                           \
       d8 dMicros = (d8)iArg1 / (d8)iArg2;                                 \
       i8 iMicros = 0;                                                     \
       i8 iRound = (dMicros * 10);                                         \
       iRound %= 10;                                                       \
       if ( iRound >= 5)                                                   \
         iMicros = dMicros + 1;                                            \
       else                                                                \
         iMicros = dMicros;                                                \
                                                                           \
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;                  \
        *paW++ = wIvlResult.m_iAsI8;                                       \
      }                                                                    \
    }                                                                      \
                                                                           \
  }

#define _DIV_IVYINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (iArg2 == 0) {                                                    \
        *paW++ = C_DATUM_NULL;                                             \
        eErrorCode = EC_DIVIDE_BY_ZERO;                                    \
      }                                                                    \
      else {                                                               \
                                                                           \
       wIvlResult.m_iAsI8 = 0;                                             \
       wIvlResult.m_xIvl.m_eType = ivlArg1.m_xIvl.m_eType;                 \
                                                                           \
       iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                        \
                                                                           \
       if ( (iArg2 < 0 && ! ivlArg1.m_xIvl.m_bIsNeg) ||                    \
            (iArg2 > 0 && ivlArg1.m_xIvl.m_bIsNeg) )                       \
          wIvlResult.m_xIvl.m_bIsNeg = 1;                                  \
                                                                           \
       if (iArg2 < 0)                                                      \
         iArg2 = (-1) * iArg2;                                             \
                                                                           \
       i8 iMonths = iArg1 / iArg2;                                         \
                                                                           \
       wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iMonths;                   \
       *paW++ = wIvlResult.m_iAsI8;                                        \
     }                                                                     \
   }                                                                       \
 }


#define _MUL_INTIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    iArg1 = *(--paW);                                                      \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg2.m_iAsI8 == C_DATUM_NULL ||                                 \
        iArg1 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      wIvlResult.m_iAsI8 = 0;                                              \
      wIvlResult.m_xIvl.m_eType = ivlArg2.m_xIvl.m_eType;                  \
                                                                           \
      if (iArg1 < 0 || ivlArg2.m_xIvl.m_bIsNeg)                            \
         wIvlResult.m_xIvl.m_bIsNeg = 1;                                   \
                                                                           \
      if (iArg1 < 0)                                                       \
        iArg1 = (-1) * iArg1;                                              \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
                                                                           \
      i8 iMicros = iArg1 * iArg2;                                          \
                                                                           \
      eErrorCode = _OUT_OF_RANGE_IVD;                                      \
                                                                           \
      if (eErrorCode != EC_NONE) {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;                  \
        *paW++ = wIvlResult.m_iAsI8;                                       \
      }                                                                    \
    }                                                                      \
  }

#define _MUL_INTIVY(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    iArg1 = *(--paW);                                                      \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg2.m_iAsI8 == C_DATUM_NULL ||                                 \
        iArg1 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      wIvlResult.m_iAsI8 = 0;                                              \
      wIvlResult.m_xIvl.m_eType = ivlArg2.m_xIvl.m_eType;                  \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      if (iArg1 < 0 || ivlArg2.m_xIvl.m_bIsNeg)                            \
         wIvlResult.m_xIvl.m_bIsNeg = 1;                                   \
                                                                           \
      if (iArg1 < 0)                                                       \
        iArg1 = (-1) * iArg1;                                              \
                                                                           \
      i8 iMonths = iArg1 * iArg2;                                          \
                                                                           \
      eErrorCode = _OUT_OF_RANGE_IVY;                                      \
                                                                           \
      if (eErrorCode != EC_NONE) {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iMonths;                  \
        *paW++ = wIvlResult.m_iAsI8;                                       \
      }                                                                    \
    }                                                                      \
  }

#define _MUL_IVDINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      wIvlResult.m_iAsI8 = 0;                                              \
      wIvlResult.m_xIvl.m_eType = ivlArg1.m_xIvl.m_eType;                  \
                                                                           \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
                                                                           \
      if (iArg2 < 0 || ivlArg1.m_xIvl.m_bIsNeg)                            \
         wIvlResult.m_xIvl.m_bIsNeg = 1;                                   \
                                                                           \
      if (iArg2 < 0)                                                       \
        iArg2 = (-1) * iArg2;                                              \
                                                                           \
      i8 iMicros = iArg1 * iArg2;                                          \
                                                                           \
      eErrorCode = _OUT_OF_RANGE_IVD;                                      \
                                                                           \
      if (eErrorCode != EC_NONE) {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;                  \
        *paW++ = wIvlResult.m_iAsI8;                                       \
      }                                                                    \
    }                                                                      \
  }

#define _MUL_IVYINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      wIvlResult.m_iAsI8 = 0;                                              \
      wIvlResult.m_xIvl.m_eType = ivlArg1.m_xIvl.m_eType;                  \
                                                                           \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      if (iArg2 < 0 || ivlArg1.m_xIvl.m_bIsNeg)                            \
         wIvlResult.m_xIvl.m_bIsNeg = 1;                                   \
                                                                           \
      if (iArg2 < 0)                                                       \
        iArg2 = (-1) * iArg2;                                              \
                                                                           \
      i8 iMonths = iArg1 * iArg2;                                          \
                                                                           \
      eErrorCode = _OUT_OF_RANGE_IVY;                                      \
                                                                           \
      if(eErrorCode != EC_NONE)  {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = (u8)iMonths;              \
        *paW++ = wIvlResult.m_iAsI8;                                       \
     }                                                                     \
   }                                                                       \
  }

#define _NEG_IVD(eErrorCode)                                               \
  {                                                                        \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        ivlArg1.m_xIvl.m_bIsNeg = 0;                                       \
      else                                                                 \
        ivlArg1.m_xIvl.m_bIsNeg = 1;                                       \
                                                                           \
      *paW++ = ivlArg1.m_iAsI8;                                            \
   }                                                                       \
 }

#define _NEG_IVY(eErrorCode)                                               \
  {                                                                        \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        ivlArg1.m_xIvl.m_bIsNeg = 0;                                       \
      else                                                                 \
        ivlArg1.m_xIvl.m_bIsNeg = 1;                                       \
                                                                           \
      *paW++ = ivlArg1.m_iAsI8;                                            \
   }                                                                       \
 }

#define _SUB_IVDIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * iArg1;                                              \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * iArg2;                                              \
                                                                           \
      i8 iMicros = iArg1 - iArg2;                                          \
                                                                           \
      eErrorCode = _OUT_OF_RANGE_IVD;                                      \
                                                                           \
      if (eErrorCode != EC_NONE) {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        wIvlResult.m_iAsI8 = 0;                                            \
                                                                           \
        if (iMicros < 0) {                                                 \
          wIvlResult.m_xIvl.m_bIsNeg = 1;                                  \
          wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = -1*iMicros;             \
        }                                                                  \
        else {                                                             \
          wIvlResult.m_xIvl.m_bIsNeg = 0;                                  \
          wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros = iMicros;                \
       }                                                                   \
       wIvlResult.m_xIvl.m_eType =                                         \
        eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType]; \
       *paW++ = wIvlResult.m_iAsI8;                                         \
     }                                                                      \
    }                                                                       \
  }



#define _SUB_IVYIVY(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * iArg1;                \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * iArg2;                \
                                                                           \
      i8 iMonths = iArg1 - iArg2;                                          \
                                                                           \
      eErrorCode = _OUT_OF_RANGE_IVY;                                      \
                                                                           \
      if (eErrorCode != EC_NONE) {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        wIvlResult.m_iAsI8 = 0;                                            \
                                                                           \
        if (iMonths < 0) {                                                 \
          wIvlResult.m_xIvl.m_bIsNeg = 1;                                  \
          wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = -1*iMonths;             \
        }                                                                  \
        else {                                                             \
          wIvlResult.m_xIvl.m_bIsNeg = 0;                                  \
          wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iMonths;                \
       }                                                                   \
       wIvlResult.m_xIvl.m_eType =                                         \
        eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType]; \
       *paW++ = wIvlResult.m_iAsI8;                                         \
     }                                                                      \
    }                                                                       \
  }

// ignore NULLS
//
#define _SUM_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (iArg1 == C_DATUM_NULL) {                                      \
      *paW++ = iArg2;                                                      \
    }                                                                      \
    else if (iArg2 == C_DATUM_NULL) {                                      \
      *paW++ = iArg1;                                                      \
    }                                                                      \
    else {                                                                 \
      eErrorCode = _OF_I8ADD;                                              \
      if(eErrorCode != EC_NONE)  {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        *paW++ = iArg1 + iArg2;                                            \
      }                                                                    \
    }                                                                      \
  }

#define _DIV_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (iArg2 == 0) {                                                    \
         *paW++ = C_DATUM_NULL;                                            \
         eErrorCode = EC_DIVIDE_BY_ZERO;                                   \
      }                                                                    \
      else {                                                               \
        eErrorCode = _OF_I8DIV;                                            \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          *paW++ = iArg1 / iArg2;                                          \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

//TODO: OVERFLOW/UNDERFLOW
#define _MOD_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (iArg2 == 0) {                                                    \
         *paW++ = C_DATUM_NULL;                                            \
         eErrorCode = EC_DIVIDE_BY_ZERO;                                   \
      }                                                                    \
      else {                                                               \
        *paW++ = iArg1 % iArg2;                                            \
      }                                                                    \
    }                                                                      \
  }

#define _MOD_UNKANY(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _MOD_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }


#define _MUL_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      eErrorCode = _OF_I8MUL;                                              \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        *paW++ = iArg1 * iArg2;                                            \
      }                                                                    \
    }                                                                      \
  }

#define _NEG_INT(eErrorCode)                                               \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      iArg1 = 0;                                                           \
      eErrorCode = _OF_I8SUB;                                              \
      if(eErrorCode != EC_NONE)                                            \
      {                                                                    \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else                                                                 \
      {                                                                    \
        *paW++ = -iArg2;                                                   \
      }                                                                    \
    }                                                                      \
  }


#define _SUB_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      eErrorCode = _OF_I8SUB;                                              \
      if(eErrorCode != EC_NONE)                                            \
      {                                                                    \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else                                                                 \
      {                                                                    \
        *paW++ = iArg1 - iArg2;                                            \
      }                                                                    \
    }                                                                      \
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
  }

#define _ADD_TMEIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xTme1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg1 = ((xTme1.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +                  \
              (xTme1.m_uMinute*_C_SECS_PER_MIN*_C_1M) +                    \
              (xTme1.m_uSecond*_C_1M) +                                    \
              xTme1.m_uMicro;                                              \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * iArg2;                                              \
                                                                           \
     iArg2 %= _C_SECS_PER_DAY  * _C_1M;                                    \
     i8 iMicros = iArg1 + iArg2;                                           \
                                                                           \
    _MICROS_TO_TIME;                                                       \
  }                                                                        \
}

#define _ADD_IVDTME(eErrorCode)                                            \
  {                                                                        \
    xTme2.m_iAsI8 = *(--paW);                                              \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTme2.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg2 = ((xTme2.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +                  \
              (xTme2.m_uMinute*_C_SECS_PER_MIN*_C_1M) +                    \
              (xTme2.m_uSecond*_C_1M) +                                    \
              xTme2.m_uMicro;                                              \
                                                                           \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * iArg1;                                              \
      else                                                                 \
                                                                           \
     iArg1 %= _C_SECS_PER_DAY  * _C_1M;                                    \
     i8 iMicros = iArg1 + iArg2;                                           \
                                                                           \
    _MICROS_TO_TIME;                                                       \
  }                                                                        \
}

#define _SUB_TMEIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xTme1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg1 = ((xTme1.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +                  \
              (xTme1.m_uMinute*_C_SECS_PER_MIN*_C_1M) +                    \
              (xTme1.m_uSecond*_C_1M) +                                    \
              xTme1.m_uMicro;                                              \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * iArg2;                                              \
      else                                                                 \
                                                                           \
      iArg2 %= _C_SECS_PER_DAY  * _C_1M;                                   \
                                                                           \
     if (iArg2 > iArg1)                                                    \
      iArg1 += _C_SECS_PER_DAY *  _C_1M;                                   \
                                                                           \
     i8 iMicros = iArg1 - iArg2;                                           \
    _MICROS_TO_TIME;                                                       \
  }                                                                        \
}

#define _SUB_TMETME(eErrorCode)                                            \
  {                                                                        \
    xTme2.m_iAsI8 = *(--paW);                                              \
    xTme1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg1 = ((xTme1.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +                  \
              (xTme1.m_uMinute*_C_SECS_PER_MIN*_C_1M) +                    \
              (xTme1.m_uSecond*_C_1M) +                                    \
              xTme1.m_uMicro;                                              \
                                                                           \
      iArg2 = ((xTme2.m_uHour%24)*_C_SECS_PER_HR*_C_1M) +                  \
              (xTme2.m_uMinute*_C_SECS_PER_MIN*_C_1M) +                    \
              (xTme2.m_uSecond*_C_1M) +                                    \
              xTme2.m_uMicro;                                              \
                                                                           \
     if (iArg1 < iArg2) {                                                  \
       ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros = iArg2 - iArg1;                \
       ivlArg1.m_xIvl.m_bIsNeg = 1;                                        \
     }                                                                     \
     else {                                                                \
       ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros = iArg1 - iArg2;                \
       ivlArg1.m_xIvl.m_bIsNeg = 0;                                        \
     }                                                                     \
     ivlArg1.m_xIvl.m_eType =  IVL_HS;                                     \
                                                                           \
    *paW++ = ivlArg1.m_iAsI8;                                              \
  }                                                                        \
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

#define _ADD_TSPIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xTsp1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * iArg2;                                              \
                                                                           \
      struct tm xDateTime = {0} ;                                          \
                                                                           \
      xDateTime.tm_year = xTsp1.m_uYearDelta;                              \
      xDateTime.tm_mon  = xTsp1.m_uMonth - 1;                              \
      xDateTime.tm_mday = xTsp1.m_uDay;                                    \
      xDateTime.tm_hour = xTsp1.m_uHour;                                   \
      xDateTime.tm_min  = xTsp1.m_uMinute;                                 \
      xDateTime.tm_isdst  = 0;                                             \
                                                                           \
      i8 iSecs = iArg2/_C_1M;                                              \
      i8 iMicroSecs = iArg2%_C_1M + xTsp1.m_uMicro;                        \
                                                                           \
      _ADJUST_SECS_MICROS_                                                 \
                                                                           \
      xDateTime.tm_sec  = xTsp1.m_uSecond + iSecs;                         \
      time_t iSecondsSincePosixEpoch = timegm(&xDateTime);                 \
                                                                           \
      xTsp2.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp2.m_uYearDelta = xDateTime.tm_year;                              \
      xTsp2.m_uMonth     = xDateTime.tm_mon + 1;                           \
      xTsp2.m_uDay       = xDateTime.tm_mday;                              \
      xTsp2.m_uHour      = xDateTime.tm_hour;                              \
      xTsp2.m_uMinute    = xDateTime.tm_min;                               \
      xTsp2.m_uSecond    = xDateTime.tm_sec;                               \
      xTsp2.m_uMicro     = iMicroSecs;                                     \
                                                                           \
      *paW++ = xTsp2.m_iAsI8;                                              \
  }                                                                        \
}

#define _ADD_IVDTSP(eErrorCode)                                            \
  {                                                                        \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp2.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
      \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * iArg1;                                              \
                                                                           \
      struct tm xDateTime = {0} ;                                          \
                                                                           \
      xDateTime.tm_year = xTsp2.m_uYearDelta;                              \
      xDateTime.tm_mon  = xTsp2.m_uMonth - 1;                              \
      xDateTime.tm_mday = xTsp2.m_uDay;                                    \
      xDateTime.tm_hour = xTsp2.m_uHour;                                   \
      xDateTime.tm_min  = xTsp2.m_uMinute;                                 \
      xDateTime.tm_isdst  = 0;                                             \
                                                                           \
      i8 iSecs = iArg1/_C_1M;                                              \
      i8 iMicroSecs = iArg1%_C_1M + xTsp2.m_uMicro;                        \
                                                                           \
      _ADJUST_SECS_MICROS_                                                 \
                                                                           \
      xDateTime.tm_sec  = xTsp2.m_uSecond + iSecs;                         \
      time_t iSecondsSincePosixEpoch = timegm(&xDateTime);                 \
                                                                           \
      xTsp1.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp1.m_uYearDelta = xDateTime.tm_year;                              \
      xTsp1.m_uMonth     = xDateTime.tm_mon + 1;                           \
      xTsp1.m_uDay       = xDateTime.tm_mday;                              \
      xTsp1.m_uHour      = xDateTime.tm_hour;                              \
      xTsp1.m_uMinute    = xDateTime.tm_min;                               \
      xTsp1.m_uSecond    = xDateTime.tm_sec;                               \
      xTsp1.m_uMicro     = iMicroSecs;                                     \
                                                                           \
      *paW++ = xTsp1.m_iAsI8;                                              \
  }                                                                        \
}


#define _ADD_TSPIVY(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xTsp1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      i8 iYear_arg2 = iArg2/12;                                            \
      i8 iMonth_arg2 = iArg2%12;                                           \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg) {                                       \
        iYear_arg2 = -1 * iYear_arg2;                                      \
        iMonth_arg2 = (-1) * iMonth_arg2;                                  \
      }                                                                    \
                                                                           \
      i8 iYearDelta;                                                       \
      i8 iMonth;                                                           \
      i8 iDay;                                                             \
      iYearDelta = xTsp1.m_uYearDelta + iYear_arg2;                        \
      iMonth     = xTsp1.m_uMonth + iMonth_arg2;                           \
      iDay       = xTsp1.m_uDay;                                           \
                                                                           \
      if (iYearDelta > C_YEAR_RANGE) {                                     \
        eErrorCode = EC_DATE_OUT_OF_RANGE;                                 \
      }                                                                    \
      else {                                                               \
        _CONVERT_TO_VALID_DATE                                             \
                                                                           \
      xTsp2.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp2.m_uYearDelta = iYearDelta;                                     \
      xTsp2.m_uMonth     = iMonth;                                         \
      xTsp2.m_uDay       = iDay;                                           \
      xTsp2.m_uHour      = xTsp1.m_uHour;                                  \
      xTsp2.m_uMinute    = xTsp1.m_uMinute;                                \
      xTsp2.m_uSecond    = xTsp1.m_uSecond;                                \
      xTsp2.m_uMicro     = xTsp1.m_uMicro;                                 \
                                                                           \
      *paW++ = xTsp2.m_iAsI8;                                              \
    }                                                                      \
  }                                                                        \
}


#define _ADD_IVYTSP(eErrorCode)                                            \
  {                                                                        \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp2.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      i8 iYear_arg1 = iArg1/12;                                            \
      i8 iMonth_arg1 = iArg1%12;                                           \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg) {                                       \
        iYear_arg1 = -1 * iYear_arg1;                                      \
        iMonth_arg1 = (-1) * iMonth_arg1;                                  \
      }                                                                    \
                                                                           \
      i8 iYearDelta;                                                       \
      i8 iMonth;                                                           \
      i8 iDay;                                                             \
      iYearDelta = xTsp2.m_uYearDelta + iYear_arg1;                        \
      iMonth     = xTsp2.m_uMonth + iMonth_arg1;                           \
      iDay       = xTsp2.m_uDay;                                           \
                                                                           \
      if (iYearDelta > C_YEAR_RANGE) {                                     \
        eErrorCode = EC_DATE_OUT_OF_RANGE;                                 \
      }                                                                    \
      else {                                                               \
        _CONVERT_TO_VALID_DATE                                             \
                                                                           \
      xTsp1.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp1.m_uYearDelta = iYearDelta;                                     \
      xTsp1.m_uMonth     = iMonth;                                         \
      xTsp1.m_uDay       = iDay;                                           \
      xTsp1.m_uHour      = xTsp2.m_uHour;                                  \
      xTsp1.m_uMinute    = xTsp2.m_uMinute;                                \
      xTsp1.m_uSecond    = xTsp2.m_uSecond;                                \
      xTsp1.m_uMicro     = xTsp2.m_uMicro;                                 \
                                                                           \
      *paW++ = xTsp1.m_iAsI8;                                              \
    }                                                                      \
  }                                                                        \
}

#define _SUB_TSPIVD(eErrorCode)                                            \
  {                                                                        \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xTsp1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                         \
      if ( ! ivlArg2.m_xIvl.m_bIsNeg )                                     \
        iArg2 = (-1) * iArg2;                                              \
                                                                           \
      struct tm xDateTime = {0} ;                                          \
                                                                           \
      xDateTime.tm_year = xTsp1.m_uYearDelta;                              \
      xDateTime.tm_mon  = xTsp1.m_uMonth - 1;                              \
      xDateTime.tm_mday = xTsp1.m_uDay;                                    \
      xDateTime.tm_hour = xTsp1.m_uHour;                                   \
      xDateTime.tm_min  = xTsp1.m_uMinute;                                 \
      xDateTime.tm_isdst  = 0;                                             \
                                                                           \
      i8 iSecs = iArg2/_C_1M;                                              \
      i8 iMicroSecs = iArg2%_C_1M + xTsp1.m_uMicro;                        \
      _ADJUST_SECS_MICROS_                                                 \
                                                                           \
      xDateTime.tm_sec  = xTsp1.m_uSecond + iSecs;                         \
      time_t iSecondsSincePosixEpoch = timegm(&xDateTime);                 \
                                                                           \
      xTsp2.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp2.m_uYearDelta = xDateTime.tm_year;                              \
      xTsp2.m_uMonth     = xDateTime.tm_mon + 1;                           \
      xTsp2.m_uDay       = xDateTime.tm_mday;                              \
      xTsp2.m_uHour      = xDateTime.tm_hour;                              \
      xTsp2.m_uMinute    = xDateTime.tm_min;                               \
      xTsp2.m_uSecond    = xDateTime.tm_sec;                               \
      xTsp2.m_uMicro     = iMicroSecs;                                     \
      xTsp2.m_uLocal     = _C_TSP_LOCAL;                                   \
                                                                           \
      *paW++ = xTsp2.m_iAsI8;                                              \
  }                                                                        \
}

#define _SUB_TSPIVY(eErrorCode)                                            \
 {                                                                         \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
    xTsp1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                         \
                                                                           \
      i8 iYear_arg2 = iArg2/12;                                            \
      i8 iMonth_arg2 = iArg2%12;                                           \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg) {                                       \
        iYear_arg2 = (-1) * iYear_arg2;                                    \
        iMonth_arg2 = (-1) * iMonth_arg2;                                  \
      }                                                                    \
                                                                           \
      i8 iYearDelta;                                                       \
      i8 iMonth;                                                           \
      i8 iDay;                                                             \
      iYearDelta = xTsp1.m_uYearDelta - iYear_arg2;                        \
      iMonth     = xTsp1.m_uMonth - iMonth_arg2;                           \
      iDay       = xTsp1.m_uDay;                                           \
                                                                           \
      if (iYearDelta > C_YEAR_RANGE) {                                     \
        eErrorCode = EC_DATE_OUT_OF_RANGE;                                 \
      }                                                                    \
      else {                                                               \
        _CONVERT_TO_VALID_DATE                                             \
                                                                           \
      xTsp2.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp2.m_uYearDelta = iYearDelta;                                     \
      xTsp2.m_uMonth     = iMonth;                                         \
      xTsp2.m_uDay       = iDay;                                           \
      xTsp2.m_uHour      = xTsp1.m_uHour;                                  \
      xTsp2.m_uMinute    = xTsp1.m_uMinute;                                \
      xTsp2.m_uSecond    = xTsp1.m_uSecond;                                \
      xTsp2.m_uMicro     = xTsp1.m_uMicro;                                 \
                                                                           \
      *paW++ = xTsp2.m_iAsI8;                                              \
    }                                                                      \
  }                                                                        \
}


#define _SUB_TSPTSP(eErrorCode)                                            \
  {                                                                        \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    xTsp1.m_iAsI8 = *(--paW);                                              \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp2.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
     struct tm xDateTime1 = {0} ;                                          \
                                                                           \
     struct tm xDateTime2 = {0} ;                                          \
                                                                           \
     xDateTime1.tm_year = xTsp1.m_uYearDelta;                              \
     xDateTime1.tm_mon  = xTsp1.m_uMonth - 1 ;                             \
     xDateTime1.tm_mday = xTsp1.m_uDay;                                    \
     xDateTime1.tm_hour = xTsp1.m_uHour;                                   \
     xDateTime1.tm_min  = xTsp1.m_uMinute;                                 \
     xDateTime1.tm_sec  = xTsp1.m_uSecond;                                 \
     xDateTime1.tm_isdst = 0;                                              \
                                                                           \
     time_t iSecondsSincePosixEpoch1 = timegm(&xDateTime1);                \
                                                                           \
     xDateTime2.tm_year = xTsp2.m_uYearDelta;                              \
     xDateTime2.tm_mon  = xTsp2.m_uMonth - 1;                              \
     xDateTime2.tm_mday = xTsp2.m_uDay;                                    \
     xDateTime2.tm_hour = xTsp2.m_uHour;                                   \
     xDateTime2.tm_min  = xTsp2.m_uMinute;                                 \
     xDateTime2.tm_sec  = xTsp2.m_uSecond;                                 \
     xDateTime2.tm_isdst = 0;                                              \
                                                                           \
    time_t iSecondsSincePosixEpoch2 = timegm(&xDateTime2);                 \
                                                                           \
    i8 iSeconds = iSecondsSincePosixEpoch1 - iSecondsSincePosixEpoch2;     \
                                                                           \
    wIvlResult.m_xIvl.m_eType = IVL_DS;                                    \
                                                                           \
    if (iSeconds < 0) {                                                    \
      wIvlResult.m_xIvl.m_bIsNeg = 1;                                      \
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  (-1)*iSeconds*_C_1M +            \
      (xTsp2.m_uMicro - xTsp1.m_uMicro);                                   \
      }                                                                      \
    else {                                                                 \
      wIvlResult.m_xIvl.m_bIsNeg = 0;                                      \
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  iSeconds*_C_1M +            \
      (xTsp1.m_uMicro - xTsp2.m_uMicro);                                   \
      }                                                                      \
    *paW++ = wIvlResult.m_iAsI8;                                           \
  }                                                                        \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// UNKNOWN and ANY
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ADD_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _DIV_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _MUL_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _SUB_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _SUM_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    *paW++ = iArg1;                                                        \
  }


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  ANY and UNKNOWN
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ADD_UNKANY(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _DIV_UNKANY(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _MUL_UNKANY(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _SUB_UNKANY(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _SUM_UNKANY(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    *paW++ = iArg2;                                                        \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
