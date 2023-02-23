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
        if (bcdArg1.m_iScale != bcdArg2.m_iScale) {                        \
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
      if (bcdArg1.m_iScale != bcdArg2.m_iScale) {                          \
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
          *paW++ = bcdArg1.m_iScale + 1;                                 \
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
      i8 iScaleNew = bcdArg1.m_iScale > bcdArg2.m_iScale ? bcdArg1.m_iScale : bcdArg2.m_iScale;  \
      i8 iResult = bcdArg1.m_iMantissa * bcdArg2.m_iMantissa;            \
      iScale = bcdArg1.m_iScale + bcdArg2.m_iScale;                      \
      if (iScale != iScaleNew) {                                         \
        bcdArg1.m_iMantissa = iResult / iaPow10[iScale - iScaleNew - 1]; \
        bcdArg1.m_iScale = iScaleNew + 1;                                \
        *paW++ = bcdArg1.m_iAsI8;                                        \
        _ROUND_BCD(eErrorCode)                                           \
      }                                                                  \
      else {                                                             \
        bcdArg1.m_iMantissa = iResult;                                   \
        bcdArg1.m_iScale = iScaleNew;                                    \
       *paW++ = bcdArg1.m_iAsI8;                                         \
       }                                                                 \
    }                                                                    \
  }                                                                      \
}

#define _NEG_BCD(eErrorCode)                                               \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
       bcdArg1.m_iAsI8 == C_DATUM_NULL) {                                  \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      bcdArg2.m_iMantissa = - bcdArg2.m_iMantissa;                         \
      *paW++ = bcdArg2.m_iAsI8;                                            \
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
        if (bcdArg1.m_iScale != bcdArg2.m_iScale) {                        \
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
       if (bcdArg1.m_iScale != bcdArg2.m_iScale) {                         \
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
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                \
      else                                                                 \
        iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                       \
                                                                           \
    struct tm xDateTime = {0} ;                                            \
                                                                           \
    xDateTime.tm_year = xDte1.m_uYearDelta;                                \
    xDateTime.tm_mon  = xDte1.m_uMonth - 1;                                \
    xDateTime.tm_mday = xDte1.m_uDay;                                      \
    xDateTime.tm_hour = 0;                                                 \
    xDateTime.tm_min  = 0;                                                 \
    xDateTime.tm_sec  = 0;                                                 \
                                                                           \
   time_t iSecondsSincePosixEpoch = mktime(&xDateTime);                    \
                                                                           \
   iSecondsSincePosixEpoch += iArg2/_C_1M;                                 \
                                                                           \
   aLocalDateTime = localtime(&iSecondsSincePosixEpoch);                   \
   xDte2.m_uYearDelta = aLocalDateTime->tm_year;                           \
   xDte2.m_uMonth     = aLocalDateTime->tm_mon + 1;                        \
   xDte2.m_uDay       = aLocalDateTime->tm_mday;                           \
   xDte2.m_uSpare       = 0;                                               \
    *paW++ = xDte2.m_iAsI8;                                                \
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
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                \
      else                                                                 \
        iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                       \
                                                                           \
   aLocalDateTime = localtime(&iSecondsSincePosixEpoch);                   \
     xDte2.m_uYearDelta = xDte1.m_uYearDelta;                              \
     xDte2.m_uMonth     = xDte1.m_uMonth;                                  \
     xDte2.m_uDay       = xDte1.m_uDay;                                    \
     xDte2.m_uSpare       = 0;                                             \
                                                                           \
    iArg1 = xDte2.m_uMonth + iArg2;                                        \
    if (iArg1 > 12) {                                                      \
      xDte2.m_uYearDelta +=  iArg1/12;                                     \
      xDte2.m_uMonth = iArg1%12;                                           \
    }                                                                      \
    *paW++ = xDte2.m_iAsI8;                                                \
  }                                                                        \
}

#define _ADD_IVDDTE(eErrorCode)                                            \
  {                                                                        \
    xDte2.m_iAsI8 = *(--paW);                                              \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                \
      else                                                                 \
        iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                       \
                                                                           \
    struct tm xDateTime = {0} ;                                            \
                                                                           \
    xDateTime.tm_year = xDte2.m_uYearDelta;                                \
    xDateTime.tm_mon  = xDte2.m_uMonth - 1;                                \
    xDateTime.tm_mday = xDte2.m_uDay;                                      \
    xDateTime.tm_hour = 0;                                                 \
    xDateTime.tm_min  = 0;                                                 \
    xDateTime.tm_sec  = 0;                                                 \
                                                                           \
   time_t iSecondsSincePosixEpoch = mktime(&xDateTime);                    \
                                                                           \
   iSecondsSincePosixEpoch += iArg1/1000000;                               \
                                                                           \
   aLocalDateTime = localtime(&iSecondsSincePosixEpoch);                   \
     xDte1.m_uYearDelta = aLocalDateTime->tm_year;                         \
     xDte1.m_uMonth     = aLocalDateTime->tm_mon + 1;                      \
     xDte1.m_uDay       = aLocalDateTime->tm_mday;                         \
     xDte1.m_uSpare       = 0;                                             \
    *paW++ = xDte1.m_iAsI8;                                                \
  }                                                                        \
}

#define _ADD_IVYDTE(eErrorCode)                                            \
  {                                                                        \
    xDte2.m_iAsI8 = *(--paW);                                              \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                \
      else                                                                 \
        iArg2 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                       \
                                                                           \
   aLocalDateTime = localtime(&iSecondsSincePosixEpoch);                   \
     xDte1.m_uYearDelta = xDte2.m_uYearDelta;                              \
     xDte1.m_uMonth     = xDte2.m_uMonth;                                  \
     xDte1.m_uDay       = xDte2.m_uDay;                                    \
     xDte1.m_uSpare       = 0;                                             \
                                                                           \
    iArg1 = xDte1.m_uMonth + iArg2;                                        \
    if (iArg1 > 12) {                                                      \
      xDte1.m_uYearDelta +=  iArg1/12;                                     \
      xDte1.m_uMonth = iArg1%12;                                           \
    }                                                                      \
    *paW++ = xDte1.m_iAsI8;                                                \
  }                                                                        \
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
      time_t iSecondsSincePosixEpoch1 = mktime(&xDateTime1);               \
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
      time_t iSecondsSincePosixEpoch2 = mktime(&xDateTime2);               \
                                                                           \
      i8 iSeconds = iSecondsSincePosixEpoch1 - iSecondsSincePosixEpoch2;   \
                                                                           \
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

/*
 INTERVALS
*/

#define _ADD_IVLIVL(eErrorCode)                                            \
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
        iArg1 = (-1) * ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                \
                                                                           \
      iArg3 = iArg1 + iArg2;                                               \
                                                                           \
      if (iArg3 < 0) {                                                     \
        wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = -1*iArg3;                 \
      }                                                                    \
      else {                                                               \
        wIvlResult.m_xIvl.m_bIsNeg = 0;                                    \
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iArg3;                    \
      }                                                                    \
      wIvlResult.m_xIvl.m_eType =                                          \
        eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType];\
    }                                                                      \
    *paW++ = wIvlResult.m_iAsI8;                                           \
  }

#define _NEG_IVL(eErrorCode)                                               \
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

#define _SUB_IVLIVL(eErrorCode)                                            \
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
        iArg1 = (-1) * ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                \
                                                                           \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                \
                                                                           \
      iArg3 = iArg1 - iArg2;                                               \
      if (iArg3 < 0) {                                                     \
        wIvlResult.m_xIvl.m_bIsNeg = 1;                                    \
        wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = -1*iArg3;                 \
      }                                                                    \
      else {                                                               \
         wIvlResult.m_xIvl.m_bIsNeg = 0;                                    \
         wIvlResult.m_xIvl.u.m_xIvlYM.m_uMonths = iArg3;                   \
      }                                                                    \
      wIvlResult.m_xIvl.m_eType =                                          \
        eMacro_AddSubResultIVL[ivlArg1.m_xIvl.m_eType][ivlArg2.m_xIvl.m_eType];\
    }                                                                      \
    *paW++ = wIvlResult.m_iAsI8;                                           \
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
    xTme2.m_uHour   = (iMicros / (_C_SECS_PER_HR  * _C_1M)) % _C_HOURS_PER_DAY;  \
    xTme2.m_uMinute = (iMicros % (_C_SECS_PER_HR  * _C_1M)) / (_C_SECS_PER_MIN * _C_1M);   \
    xTme2.m_uSecond = (iMicros % (_C_SECS_PER_MIN * _C_1M)) / _C_1M;       \
    xTme2.m_uMicro  = iMicros % _C_1M;                                     \
    xTme2.m_uSpare = 0;                                                    \
                                                                           \
    *paW++ = xTme2.m_iAsI8;                                                \
  }                                                                        \

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
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                \
      else                                                                 \
        iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                       \
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
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                \
      else                                                                 \
        iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                       \
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
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                \
      else                                                                 \
        iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                       \
                                                                           \
     iArg2 %= _C_SECS_PER_DAY  * _C_1M;                                    \
     i8 iMicros = iArg1 - iArg2;                                           \
                                                                           \
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
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                \
      else                                                                 \
        iArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                       \
                                                                           \
    struct tm xDateTime = {0} ;                                            \
                                                                           \
    xDateTime.tm_year = xTsp1.m_uYearDelta;                                \
    xDateTime.tm_mon  = xTsp1.m_uMonth - 1;                                \
    xDateTime.tm_mday = xTsp1.m_uDay;                                      \
    xDateTime.tm_hour = xTsp1.m_uHour;                                     \
    xDateTime.tm_min  = xTsp1.m_uMinute;                                   \
    xDateTime.tm_sec  = xTsp1.m_uSecond;                                   \
                                                                           \
    time_t iSecondsSincePosixEpoch = mktime(&xDateTime);                   \
                                                                           \
    iSecondsSincePosixEpoch += iArg2/_C_1M;                                \
                                                                           \
   aLocalDateTime = localtime(&iSecondsSincePosixEpoch);                   \
   xTsp2.m_uYearDelta = aLocalDateTime->tm_year;                           \
   xTsp2.m_uMonth     = aLocalDateTime->tm_mon + 1;                        \
   xTsp2.m_uDay       = aLocalDateTime->tm_mday;                           \
   xTsp2.m_uHour      = aLocalDateTime->tm_hour;                           \
   xTsp2.m_uMinute    = aLocalDateTime->tm_min;                            \
   xTsp2.m_uSecond    = aLocalDateTime->tm_sec;                            \
   xTsp2.m_uSecond    = aLocalDateTime->tm_sec;                            \
   xTsp2.m_uMicro     = iArg2%_C_1M;                                       \
                                                                           \
   *paW++ = xTsp2.m_iAsI8;                                                 \
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
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                \
      else                                                                 \
        iArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                       \
                                                                           \
    struct tm xDateTime = {0} ;                                            \
                                                                           \
    xDateTime.tm_year = xTsp2.m_uYearDelta;                                \
    xDateTime.tm_mon  = xTsp2.m_uMonth - 1;                                \
    xDateTime.tm_mday = xTsp2.m_uDay;                                      \
    xDateTime.tm_hour = xTsp2.m_uHour;                                     \
    xDateTime.tm_min  = xTsp2.m_uMinute;                                   \
    xDateTime.tm_sec  = xTsp2.m_uSecond;                                   \
                                                                           \
    time_t iSecondsSincePosixEpoch = mktime(&xDateTime);                   \
                                                                           \
    iSecondsSincePosixEpoch += iArg1/_C_1M;                                \
                                                                           \
   aLocalDateTime = localtime(&iSecondsSincePosixEpoch);                   \
   xTsp1.m_uYearDelta = aLocalDateTime->tm_year;                           \
   xTsp1.m_uMonth     = aLocalDateTime->tm_mon + 1;                        \
   xTsp1.m_uDay       = aLocalDateTime->tm_mday;                           \
   xTsp1.m_uHour      = aLocalDateTime->tm_hour;                           \
   xTsp1.m_uMinute    = aLocalDateTime->tm_min;                            \
   xTsp1.m_uSecond    = aLocalDateTime->tm_sec;                            \
   xTsp1.m_uSecond    = aLocalDateTime->tm_sec;                            \
   xTsp1.m_uMicro     = iArg1%_C_1M;                                       \
                                                                           \
   *paW++ = xTsp1.m_iAsI8;                                                 \
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
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iArg2 = (-1) * ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                 \
      else                                                                 \
        iArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                        \
                                                                           \
    struct tm xDateTime = {0} ;                                            \
                                                                           \
    xDateTime.tm_year = xTsp1.m_uYearDelta;                                \
    xDateTime.tm_mon  = xTsp1.m_uMonth - 1 + iArg2;                        \
    xDateTime.tm_mday = xTsp1.m_uDay;                                      \
    xDateTime.tm_hour = xTsp1.m_uHour;                                     \
    xDateTime.tm_min  = xTsp1.m_uMinute;                                   \
    xDateTime.tm_sec  = xTsp1.m_uSecond;                                   \
                                                                           \
    time_t iSecondsSincePosixEpoch = mktime(&xDateTime);                   \
                                                                           \
    aLocalDateTime = localtime(&iSecondsSincePosixEpoch);                  \
                                                                           \
    xTsp2.m_uYearDelta = aLocalDateTime->tm_year;                          \
    xTsp2.m_uMonth     = aLocalDateTime->tm_mon + 1;                       \
    xTsp2.m_uDay       = aLocalDateTime->tm_mday;                          \
    xTsp2.m_uHour      = aLocalDateTime->tm_hour;                          \
    xTsp2.m_uMinute    = aLocalDateTime->tm_min;                           \
    xTsp2.m_uSecond    = aLocalDateTime->tm_sec;                           \
    xTsp2.m_uSecond    = aLocalDateTime->tm_sec;                           \
    xTsp2.m_uMicro     = 0;                                                \
                                                                           \
    *paW++ = xTsp2.m_iAsI8;                                                \
  }                                                                        \
}

#define _ADD_IVYTSP(eErrorCode)                                            \
  {                                                                        \
     xTsp2.m_iAsI8 = *(--paW);                                             \
     ivlArg1.m_iAsI8 = *(--paW);                                           \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp2.m_iAsI8 == C_DATUM_NULL ||                                   \
        ivlArg1.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
                                                                           \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iArg1 = (-1) * ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                \
      else                                                                 \
        iArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                       \
                                                                           \
     struct tm xDateTime = {0} ;                                           \
                                                                           \
     xDateTime.tm_year = xTsp2.m_uYearDelta;                               \
     xDateTime.tm_mon  = xTsp2.m_uMonth - 1 + iArg1;                       \
     xDateTime.tm_mday = xTsp2.m_uDay;                                     \
     xDateTime.tm_hour = xTsp2.m_uHour;                                    \
     xDateTime.tm_min  = xTsp2.m_uMinute;                                  \
     xDateTime.tm_sec  = xTsp2.m_uSecond;                                  \
                                                                           \
     time_t iSecondsSincePosixEpoch = mktime(&xDateTime);                  \
                                                                           \
     aLocalDateTime = localtime(&iSecondsSincePosixEpoch);                 \
                                                                           \
     xTsp1.m_uYearDelta = aLocalDateTime->tm_year;                         \
     xTsp1.m_uMonth     = aLocalDateTime->tm_mon + 1;                      \
     xTsp1.m_uDay       = aLocalDateTime->tm_mday;                         \
     xTsp1.m_uHour      = aLocalDateTime->tm_hour;                         \
     xTsp1.m_uMinute    = aLocalDateTime->tm_min;                          \
     xTsp1.m_uSecond    = aLocalDateTime->tm_sec;                          \
     xTsp1.m_uSecond    = aLocalDateTime->tm_sec;                          \
     xTsp1.m_uMicro     = 0;                                               \
                                                                           \
     *paW++ = xTsp1.m_iAsI8;                                               \
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
                                                                           \
     time_t iSecondsSincePosixEpoch1 = mktime(&xDateTime1);                \
                                                                           \
     xDateTime2.tm_year = xTsp2.m_uYearDelta;                              \
     xDateTime2.tm_mon  = xTsp2.m_uMonth - 1;                              \
     xDateTime2.tm_mday = xTsp2.m_uDay;                                    \
     xDateTime2.tm_hour = xTsp2.m_uHour;                                   \
     xDateTime2.tm_min  = xTsp2.m_uMinute;                                 \
     xDateTime2.tm_sec  = xTsp2.m_uSecond;                                 \
                                                                           \
    time_t iSecondsSincePosixEpoch2 = mktime(&xDateTime2);                 \
                                                                           \
    i8 iSeconds = iSecondsSincePosixEpoch1 - iSecondsSincePosixEpoch2;     \
                                                                           \
    wIvlResult.m_xIvl.m_eType = IVL_DS;                                    \
                                                                           \
    if (iSeconds < 0) {                                                    \
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  -1 * iSeconds * _C_1M;     \
      wIvlResult.m_xIvl.m_bIsNeg = 1;                                      \
    }                                                                      \
    else {                                                                 \
      wIvlResult.m_xIvl.u.m_xIvlDT.m_uMicros =  iSeconds*_C_1M;            \
      wIvlResult.m_xIvl.m_bIsNeg = 0;                                      \
    }                                                                      \
                                                                           \
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
