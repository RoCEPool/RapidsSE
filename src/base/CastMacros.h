/*============================================================================*/
/* CastMacros.h -- Cast Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CAST
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  TO BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _FLT_TOBCD(iErrorCode) \
  NEVER_REACH;

#define _INT_TOBCD(iErrorCode) \
  NEVER_REACH;

#define _VCH_TOBCD(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  TO FLOAT
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _BCD_TOFLT(iErrorCode) \
  NEVER_REACH;

#define _INT_TOFLT(iErrorCode) \
  NEVER_REACH;

#define _VCH_TOFLT(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _BCD_TOINT(iErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (iErrorCode != EC_NONE ||                                           \
      bcdArg1.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = bcdArg1.m_iMantissa / iaPow10[bcdArg1.m_iScale];            \
    }                                                                      \
  }

#define _INT_TOINT(iErrorCode) \
  NEVER_REACH;

#define _VCH_TOINT(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO DATE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _TSP_TODTE(iErrorCode) \
  NEVER_REACH;

#define _VCH_TODTE(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO IVL
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _VCH_TOIVL(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO IVY
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _VCH_TOIVY(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO TME
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _TSP_TOTME(iErrorCode) \
  NEVER_REACH;

#define _VCH_TOTME(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO TSP
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _DTE_TOTSP(iErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xDte1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      xTsp1.m_iAsI8 = 0;                                                   \
                                                                           \
      xTsp1.m_uDay = xDte1.m_uDay;                                         \
      xTsp1.m_uMonth = xDte1.m_uMonth;                                     \
      xTsp1.m_uYearDelta = xDte1.m_uYearDelta;                             \
      *paW++ = xTsp1.m_iAsI8;                                              \
    }                                                                      \
  }

#define _TME_TOTSP(iErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xTme1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      xTsp1.m_uMicro = xTme1.m_uMicro;                                     \
      xTsp1.m_uSecond = xTme1.m_uSecond;                                   \
      xTsp1.m_uMinute = xTme1.m_uMinute;                                   \
      xTsp1.m_uHour = xTme1.m_uHour;                                       \
                                                                           \
      xTsp1.m_uDay = 1;                                                    \
      xTsp1.m_uMonth = 1;                                                  \
      xTsp1.m_uYearDelta = 0;                                              \
      *paW++ = xTsp1.m_iAsI8;                                              \
    }                                                                      \
  }

#define _VCH_TOTSP(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO VCH
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _BOL_TOVCH(iErrorCode) \
  NEVER_REACH;

#define _BCD_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (iErrorCode != EC_NONE ||                                           \
      bcdArg1.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 max_col_decimal = TMP_TBLCOL_SCALE;                               \
      i8 bNegative = 0;                                                    \
                                                                           \
      if (bcdArg1.m_iMantissa < 0) {                                       \
        bcdArg1.m_iMantissa = -bcdArg1.m_iMantissa;                        \
        bNegative = 1;                                                     \
      }                                                                    \
                                                                           \
      char *p = szDstArea, *q = szDstArea;                                 \
      if(bcdArg1.m_iScale == 0) {                                          \
        while(max_col_decimal) {                                           \
          *p++ = '0';                                                      \
          max_col_decimal--;                                               \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        while (bcdArg1.m_iScale) {                                         \
          *p++ = (bcdArg1.m_iMantissa % 10) + '0';                         \
          bcdArg1.m_iMantissa /= 10ll;                                     \
          bcdArg1.m_iScale--;                                              \
        }                                                                  \
      }                                                                    \
      *p++ = '.';                                                          \
      while (bcdArg1.m_iMantissa) {                                        \
        *p++ = (bcdArg1.m_iMantissa % 10) + '0';                           \
        bcdArg1.m_iMantissa /= 10ll;                                       \
      }                                                                    \
      if (bNegative)                                                       \
        *p++ = '-';                                                        \
                                                                           \
        *p = '\0';                                                         \
        while (--p > q) {                                                  \
          char c = *p;                                                     \
          *p = *q;                                                         \
          *q++ = c;                                                        \
        }                                                                  \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
  }

#define _TME_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xTme1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      char *p = szDstArea;                                                 \
                                                                           \
      u8 tmp = xTme1.m_uHour;                                              \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTme1.m_uHour;                                                 \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ':';                                                          \
                                                                           \
      tmp = xTme1.m_uMinute;                                               \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTme1.m_uMinute;                                               \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ':';                                                          \
                                                                           \
      tmp = xTme1.m_uSecond;                                               \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTme1.m_uSecond;                                               \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      if(xTme1.m_uMicro != 0) {                                            \
        *p++ = ':';                                                        \
                                                                           \
        tmp = xTme1.m_uMicro;                                              \
        tmp /= 10llu;                                                      \
        *p++ = (tmp % 10) + '0';                                           \
                                                                           \
        tmp = xTme1.m_uMicro;                                              \
        *p++ = (tmp % 10) + '0';                                           \
      }                                                                    \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
}

#define _DTE_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xDte1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      u8 tmpYear = C_YEAR_BASE + xDte1.m_uYearDelta;                       \
      char *p = szDstArea;                                                 \
                                                                           \
      u8 tmp = tmpYear;                                                    \
      tmp /= 1000llu;                                                      \
      *p++ = tmp + '0';                                                    \
                                                                           \
      tmp = tmpYear;                                                       \
      tmp /= 100llu;                                                       \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = tmpYear;                                                       \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = tmpYear;                                                       \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = '-';                                                          \
                                                                           \
      tmp = xDte1.m_uMonth;                                                \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xDte1.m_uMonth;                                                \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = '-';                                                          \
                                                                           \
      tmp = xDte1.m_uDay;                                                  \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xDte1.m_uDay;                                                  \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
}
#define _FLT_TOVCH(iErrorCode) \
  NEVER_REACH;

#define _INT_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    if (iErrorCode != EC_NONE ||                                           \
      iArg1 == C_DATUM_NULL) {                                             \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 bNegative = 0;                                                    \
                                                                           \
      if (iArg1 < 0) {                                                     \
        iArg1 = -iArg1;                                                    \
        bNegative = 1;                                                     \
      }                                                                    \
                                                                           \
      char *p = szDstArea, *q = szDstArea;                                 \
      do {                                                                 \
        *p++ = (iArg1 % 10) + '0';                                         \
        iArg1 /= 10ll;                                                     \
      } while (iArg1);                                                     \
                                                                           \
      if (bNegative)                                                       \
        *p++ = '-';                                                        \
                                                                           \
        *p = '\0';                                                         \
        while (--p > q) {                                                  \
          char c = *p;                                                     \
          *p = *q;                                                         \
          *q++ = c;                                                        \
        }                                                                  \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
  }

#define _IVL_TOVCH(iErrorCode) \
  NEVER_REACH;

#define _IVY_TOVCH(iErrorCode) \
  NEVER_REACH;

#define _TSP_TOVCH(iErrorCode, szDstArea)                                  \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    if (iErrorCode != EC_NONE ||                                           \
      xTsp1.m_iAsI8 == C_DATUM_NULL) {                                     \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      u8 tmpYear = C_YEAR_BASE + xTsp1.m_uYearDelta;                       \
      char *p = szDstArea;                                                 \
                                                                           \
      u8 tmp = tmpYear;                                                    \
      tmp /= 1000llu;                                                      \
      *p++ = tmp + '0';                                                    \
                                                                           \
      tmp = tmpYear;                                                       \
      tmp /= 100llu;                                                       \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = tmpYear;                                                       \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = tmpYear;                                                       \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = '-';                                                          \
                                                                           \
      tmp = xTsp1.m_uMonth;                                                \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uMonth;                                                \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = '-';                                                          \
                                                                           \
      tmp = xTsp1.m_uDay;                                                  \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uDay;                                                  \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ' ';                                                          \
                                                                           \
      tmp = xTsp1.m_uHour;                                                 \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uHour;                                                 \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ':';                                                          \
                                                                           \
      tmp = xTsp1.m_uMinute;                                               \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uMinute;                                               \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      *p++ = ':';                                                          \
                                                                           \
      tmp = xTsp1.m_uSecond;                                               \
      tmp /= 10llu;                                                        \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      tmp = xTsp1.m_uSecond;                                               \
      *p++ = (tmp % 10) + '0';                                             \
                                                                           \
      if(xTsp1.m_uMicro != 0) {                                            \
        *p++ = ':';                                                        \
                                                                           \
        tmp = xTsp1.m_uMicro;                                              \
        tmp /= 10llu;                                                      \
        *p++ = (tmp % 10) + '0';                                           \
                                                                           \
        tmp = xTsp1.m_uMicro;                                              \
        *p++ = (tmp % 10) + '0';                                           \
      }                                                                    \
                                                                           \
      *paW++ = (i8)szDstArea;                                              \
      szDstArea += strlen(szDstArea) + 1;                                  \
    }                                                                      \
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
