/*============================================================================*/
/* CastMacros.h -- Cast Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// CAST
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
#define _BCD_TOINT(iErrorCode) \
  NEVER_REACH;

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
#define _DTE_TOTSP(iErrorCode) \
  NEVER_REACH;

#define _TME_TOTSP(iErrorCode) \
  NEVER_REACH;

#define _VCH_TOTSP(iErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TO VCH
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _BOL_TOVCH(iErrorCode) \
  NEVER_REACH;

#define _BCD_TOVCH(iErrorCode) \
  NEVER_REACH;

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

#define _TSP_TOVCH(iErrorCode) \
  NEVER_REACH;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
