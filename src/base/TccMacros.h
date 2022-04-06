/*============================================================================*/
/* TccMacros.h -- Function Macros                                             */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PUSHES

#define _PSH_COL(iArg) \
  *paW++ = (i8)iArg;

#define _PSH_BOL(iArg)  \
  *paW++ = (i8)iArg;

#define _PSH_BCD(iArg)  \
  *paW++ = iArg;

#define _PSH_DTE(iArg)  \
  *paW++ = iArg;

#define _PSH_FLT(dArg)        \
{                             \
  fltArg1.m_dFlt =   dArg;    \
  *paW++ = fltArg1.m_iAsI8;   \
}

#define _PSH_INT(iArg) \
  *paW++ = iArg;

#define _PSH_IVL(iArg)  \
  *paW++ = iArg;

#define _PSH_IVY(iArg)  \
  *paW++ = iArg;

#define _PSH_TME(iArg)  \
  *paW++ = iArg;

#define _PSH_TSP(iArg)  \
  *paW++ = iArg;

#define _PSH_UNK(iArg)  \
  *paW++ = iArg;

#define _PSH_VCH(szArg1)     \
  *paW++ = (i8)szArg1;

#define _ALLOC_VCH(szDstArea)                   \
{                                               \
  szArg1 = (char*)*(--paW);                     \
  if ((i8)szArg1 == C_DATUM_NULL) {             \
    *paW++ = C_DATUM_NULL;                      \
  }                                             \
  else {                                        \
    nSrcSize = strlen(szArg1) + 1;              \
    strncpy(szDstArea, szArg1, nSrcSize);       \
    *paW++ = (i8)szDstArea;                     \
    szDstArea += nSrcSize + 1;                  \
  }                                             \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// POPS

#define _POPINT()  \
  *(--paW);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ASSIGNS

#define _ASSIGN(iArgTo) \
  iArgTo = *(--paW);

#define _ASSIGN_IFTRUE(iArgTo)                                              \
 {                                                                          \
  iArg1 = *(--paW);                                                         \
  if (iArg1 && iArg1 != C_DATUM_NULL)                                       \
    iArgTo = *(--paW);                                                      \
  else                                                                      \
    --paW;                                                                  \
 }

#define _ASSIGN_TO_ENTINT(iArgTo)                                           \
  iArg1 = *(--paW);                                                         \
  vEntWrk_appendElementEntInt(iArgTo,iArg1);

#define _ASSIGN_TO_ENTSTZ(iArgTo)                                           \
  szArg1 = (char*)*(--paW);                                                 \
  vEntWrk_appendElementEntStz(iArgTo,szArg1);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BETWEEN
#define _BET_BCDBCD(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg3.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 ==  C_DATUM_NULL ||                                \
        bcdArg3.m_iAsI8 == C_DATUM_NULL) {                                 \
      *(paW++) = 0;                                                        \
    }                                                                      \
    else {                                                                 \
       if (bcdArg1.m_iMantissa > bcdArg2.m_iMantissa) {                                                  \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (bcdArg1.m_iMantissa < bcdArg3.m_iMantissa)                                                  \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                    \
   }                                                                       \
}


#define _BET_INTINT(eErrorCode)                                             \
  {                                                                         \
    iArg1 = *(--paW);                                                       \
    iArg2 = *(--paW);                                                       \
    iArg3 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL ||                                            \
        iArg2 ==  C_DATUM_NULL ||                                           \
        iArg3 == C_DATUM_NULL) {                                            \
      *(paW++) = 0;                                                         \
    }                                                                       \
    else {                                                                  \
      if (iArg1 > iArg2) {                                                  \
        *(paW++) = 0;                                                       \
      }                                                                     \
      else {                                                                \
        if (iArg1 < iArg3)                                                  \
          *(paW++) = 0;                                                     \
        else                                                                \
          *(paW++) = 1;                                                     \
      }                                                                     \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INEXPR

#define _INX_INTINT(eErrorCode)                                             \
  {                                                                         \
    bFilterResult = 0;                                                      \
    iArg1 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL) {                                            \
      *(paW++) = 0;                                                         \
    }                                                                       \
    else {                                                                  \
      iInCount = *(--paW);                                                  \
      while(iInCount--) {                                                   \
        iArg2 = *(--paW);                                                   \
        if (iArg1 == iArg2) {                                               \
           bFilterResult = 1;                                               \
           break;                                                           \
        }                                                                   \
      }                                                                     \
      while(iInCount-- > 0) {                                               \
        iArg2 = *(--paW);                                                   \
      }                                                                     \
      *(paW++) = bFilterResult;                                             \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INEXPR

#define _INX_VCHVCH(eErrorCode)                                             \
  {                                                                         \
    bFilterResult = 0;                                                      \
    iArg1 = *(--paW);                                                       \
    if (eErrorCode != EC_NONE ||                                            \
        iArg1 == C_DATUM_NULL) {                                            \
      *(paW++) = 0;                                                         \
    }                                                                       \
    else {                                                                  \
      iInCount = *(--paW);                                                  \
      while(iInCount--) {                                                   \
        *(paW++) = iArg1;                                                   \
        _EQ_VCHVCH(eErrorCode)                                              \
        bFilterResult = *(--paW);                                           \
        if (bFilterResult) {                                                \
          printf("EQUAL\n");                                                \
          break;                                                            \
          }      \
      }                                                                      \
      while(iInCount-- > 0) {                                               \
        iArg2 = *(--paW);                                                   \
      }                                                                     \
      *(paW++) = bFilterResult;                                             \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DUPS

#define _DUP_FLT(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_BCD(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_DTE(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_INT(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_TME(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _DUP_TSP(eErrorCode)                                                \
  iArg1 = *(--paW);                                                         \
  *(paW++) = iArg1;                                                         \
  *(paW++) = iArg1;

#define _COPYSTR(iArgTo, szAreaTo)                                          \
  {                                                                         \
    iArg1 = *(--paW);                                                       \
    if (iArg1 == C_DATUM_NULL)                                              \
      arg = iArg1;                                                          \
    else {                                                                  \
        nSrcSize = strlen((char*)a) + 1;                                    \
        szDstStz = szAreaTo;                                                \
        strncpy(szDstStz, (char*)a, nSrcSize);                              \
        iArgTo = (i8)szDstStz;                                              \
        aCombinedT->m_racStrAreaP += nSrcSize;                              \
      }                                                                     \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT DAY

#define _DAY_DTE(eErrorCode)                                                  \
  {                                                                           \
    xDte1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xDte1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xDte1.m_uDay;                                                \
    }                                                                         \
  }

#define _DAY_TSP(eErrorCode)                                                  \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uDay;                                                \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT HOUR

#define _HOUR_TME(eErrorCode)                                                 \
  {                                                                           \
    xTme1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTme1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTme1.m_uHour;                                               \
    }                                                                         \
  }

#define _HOUR_TSP(eErrorCode)                                                 \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uHour;                                               \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT MINUTE

#define _MINUTE_TME(eErrorCode)                                                 \
  {                                                                           \
    xTme1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTme1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTme1.m_uMinute;                                             \
    }                                                                         \
  }

#define _MINUTE_TSP(eErrorCode)                                                 \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uMinute;                                             \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT SECOND

#define _SECOND_TME(eErrorCode)                                                 \
  {                                                                           \
    xTme1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTme1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTme1.m_uSecond;                                             \
    }                                                                         \
  }

#define _SECOND_TSP(eErrorCode)                                                 \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uSecond;                                             \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT MONTH

#define _MONTH_DTE(eErrorCode)                                                \
  {                                                                           \
    xDte1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xDte1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xDte1.m_uMonth;                                              \
    }                                                                         \
  }

#define _MONTH_TSP(eErrorCode)                                                \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = xTsp1.m_uMonth;                                              \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// EXTRACT YEAR

#define _YEAR_DTE(eErrorCode)                                                 \
  {                                                                           \
    xDte1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xDte1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = C_YEAR_BASE + xDte1.m_uYearDelta;                            \
    }                                                                         \
  }

#define _YEAR_TSP(eErrorCode)                                                 \
  {                                                                           \
    xTsp1.m_iAsI8 = *(--paW);                                                 \
    if (eErrorCode != EC_NONE ||                                              \
        xTsp1.m_iAsI8 == C_DATUM_NULL) {                                      \
        *(paW++) = C_DATUM_NULL;                                              \
    }                                                                         \
    else {                                                                    \
      *(paW++) = C_YEAR_BASE + xTsp1.m_uYearDelta;                            \
    }                                                                         \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NULL TESTS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// do we need separate macros for all datatypes??? Not sure

#define _ISNOTNULL(eErrorCode)                                               \
  {                                                                          \
  iArg1 = *(--paW);                                                          \
   if (eErrorCode != EC_NONE)                                                \
    (*paW++) = C_DATUM_NULL;                                                 \
  else                                                                       \
    (*paW++) = (iArg1 != C_DATUM_NULL ? 1 : 0);                              \
}

#define _ISNULL(eErrorCode)                                                  \
  iArg1 = *(--paW);                                                          \
  if (eErrorCode != EC_NONE)                                                 \
    (*paW++) = C_DATUM_NULL;                                                 \
  else    {                                                                  \
   (*paW++) = (iArg1 == C_DATUM_NULL ? 1 : 0);                               \
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// STRING FUNCTIONS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _LIKE2(eErrorCode)                                                  \
  {                                                                         \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      *(paW++) = StrLib_iLike2(szArg2, szArg1);                             \
    }                                                                       \
  }

#define _LIKE3(eErrorCode)                                                  \
  {                                                                         \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    szArg3 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      *(paW++) = StrLib_iLike3(szArg3, szArg2, *szArg1);                    \
    }                                                                       \
  }


#define _MATCH_STRSTR(str,substr,len,bMatch)                                \
  {                                                                         \
    char* a = str;                                                          \
    char* b = substr;                                                       \
    i8 l = len;                                                             \
                                                                            \
    while(*a != '\0') {                                                     \
      if (*a != *b) {                                                       \
        a++;                                                                \
        continue;                                                           \
      }                                                                     \
                                                                            \
      bMatch = 1;                                                           \
                                                                            \
      char* s = a;                                                          \
      while (l--) {                                                         \
        if (*s++ != *b++) {                                                 \
          bMatch = 0;                                                       \
          break;                                                            \
        }                                                                   \
      }                                                                     \
      if (bMatch) {                                                         \
        break;                                                              \
      }                                                                     \
      b = substr;                                                           \
      l = len;                                                              \
      a++;                                                                  \
    }                                                                       \
                                                                            \
    if ( bMatch) {                                                          \
      str = a+len;                                                          \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// _MATCH_LEADING
// matches given string with leading characters (before %) of the pattern
// e.g pattern is : ABC% ; matches first 3 characters with ABC
// parameters:
// szArg2 : string
// szArg1 : regular expression
// iArg1  : length of the regular expression
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _MATCH_LEADING(eErrorCode)                                          \
  {                                                                         \
    iArg1  = (i8)*(--paW);                                                  \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      char* r = szArg1;                                                     \
      char* s = szArg2;                                                     \
                                                                            \
      while (iArg1--)                                                       \
      {                                                                     \
        if (*r++ != *s++) {                                                 \
          *(paW++) = 0;                                                     \
          break;                                                            \
        }                                                                   \
      }                                                                     \
      if (iArg1 == 0)                                                       \
        *(paW++) = 1;                                                       \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// _MATCH_MIDDLE
// matches given string with middle characters (between 2 %) of the pattern
// e.g pattern is : %ABC% ; matches middle 3 characters with ABC
// parameters:
// szArg2 : string
// szArg1 : regular expression
// iArg1  : length of the regular expression between the two %'s
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _MATCH_MIDDLE(eErrorCode)                                           \
  {                                                                         \
    iArg1  = (i8)*(--paW);                                                  \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      i8 bMatch = 0;                                                        \
      i8 iLen = iArg1;                                                      \
      char* sub = szArg1 + 1;                                               \
      char* str = szArg2;                                                   \
      _MATCH_STRSTR(str, sub, iLen, bMatch)                                 \
      *(paW++) = bMatch;                                                    \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// _MATCH_MIDDLE1
// matches given string with middle characters (with one % in middle) of the pattern
// e.g pattern is : %ABC%DEF% ; matches middle 3 characters with ABC
// parameters:
// szArg2 : string
// szArg1 : regular expression
// iArg1  : length of the regular expression between first two %'s
// iArg2  : length of the regular expression between last two %'s
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _MATCH_MIDDLE1(eErrorCode)                                          \
  {                                                                         \
    iArg1  = (i8)*(--paW);                                                  \
    iArg2  = (i8)*(--paW);                                                  \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      i8 bMatch = 0;                                                        \
      i8 iLen = iArg2;                                                      \
      char* sub = szArg1 + 1;                                               \
      char* str = szArg2;                                                   \
      _MATCH_STRSTR(str, sub, iLen, bMatch)                                 \
      if (bMatch) {                                                         \
        bMatch = 0;                                                         \
        sub = szArg1 + iLen + 2;                                            \
        iLen = iArg1;                                                       \
        _MATCH_STRSTR(str, sub, iLen, bMatch)                               \
      }                                                                     \
      *(paW++) = bMatch;                                                    \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// _MATCH_TRAILING
// matches given string with trailing characters (following %) of the pattern
// e.g pattern is : %ABC ; matches last 3 characters with ABC
// parameters:
// szArg2 : string
// szArg1 : regular expression
// iArg1  : length of the regular expression
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _MATCH_TRAILING(eErrorCode)                                         \
  {                                                                         \
    iArg1  = (i8)*(--paW);                                                  \
    szArg1 = (char*)*(--paW);                                               \
    szArg2 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        szArg1  == (char*)C_DATUM_NULL ||                                   \
        szArg2  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      char* r = szArg1 + iArg1 - 1;                                         \
      char* s = szArg2;                                                     \
                                                                            \
      while (*s)                                                            \
        s++;                                                                \
      s--;                                                                  \
                                                                            \
      while (*r != '%')                                                     \
      {                                                                     \
        if (*r-- != *s--) {                                                 \
          *(paW++) = 0;                                                     \
          break;                                                            \
        }                                                                   \
      }                                                                     \
      if (*(++r) == '%')                                                    \
        *(paW++) = 1;                                                       \
    }                                                                       \
  }

#define _STR_SUBSTRING2(szDstArea, eErrorCode)                              \
  {                                                                         \
    i8 iStart = *(--paW);                                                   \
    szArg1 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        iStart  == C_DATUM_NULL ||                                          \
        szArg1  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      char* szDstStz;                                                       \
      if (iStart == 0) {                                                    \
        iStart = 1;                                                         \
      }                                                                     \
      szSrcStz = (char*)(szArg1 + iStart - 1);                              \
      szDstStz = szDstArea;                                                 \
      while (*szSrcStz)                                                     \
        *szDstStz++ = *szSrcStz++;                                          \
      *szDstStz = '\0';                                                     \
      *(paW++) = (i8)szDstArea;                                             \
    }                                                                       \
  }

#define _STR_SUBSTRING3(szDstArea, eErrorCode)                              \
  {                                                                         \
    i8 iLength = *(--paW);                                                  \
    i8 iStart = *(--paW);                                                   \
    szArg1 = (char*)*(--paW);                                               \
    if (eErrorCode != EC_NONE ||                                            \
        iLength == C_DATUM_NULL ||                                          \
        iStart  == C_DATUM_NULL ||                                          \
        szArg1  == (char*)C_DATUM_NULL) {                                   \
      *(paW++) = C_DATUM_NULL;                                              \
    }                                                                       \
    else {                                                                  \
      char* szDstStz;                                                       \
      if (iStart == 0) {                                                    \
        iStart = 1;                                                         \
        iLength--;                                                          \
      }                                                                     \
      szSrcStz = (char*)(szArg1 + iStart - 1);                              \
      szDstStz = szDstArea;                                                 \
      while (*szSrcStz && iLength--)                                        \
        *szDstStz++ = *szSrcStz++;                                          \
      *szDstStz = '\0';                                                     \
      *(paW++) = (i8)szDstArea;                                             \
    }                                                                       \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  INVALID TYPES
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _OPR_INVINV(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    eErrorCode = EC_INVALID_TYPES;                                         \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _OPR_INV(eErrorCode)                                               \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    eErrorCode = EC_INVALID_TYPES;                                         \
    *paW++ = C_DATUM_NULL;                                                 \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
