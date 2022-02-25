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

#define _PSH_FLT(dArg)  \
  *paW++ = (i8)dArg;

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

#define _PSH_VCH(szArg1)  \
  (char*)(*paW++) = szArg1;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// POPS

#define _POPINT()  \
  *(--paW);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ASSIGNS

#define _ASSIGN(iArgTo)                                                     \
  iArgTo = *(--paW);

#define _ASSIGN_IFTRUE(iArgTo)                                              \
 {                                                                          \
  iArg1 = *(--paW);                                                         \
  if (iArg1)                                                                \
    iArgTo = *(--paW);                                                      \
  else                                                                      \
    --paW;                                                                  \
 }

#define _ASSIGN_TO_ENTINT(iArgTo)                                           \
  iArg1 = *(--paW);                                                         \
  vEntWrk_appendElementEntInt(iArgTo,iArg1)

#define _ASSIGN_TO_ENTSTZ(iArgTo)                                           \
  szArg1 = (char*)*(--paW);                                                 \
  vEntWrk_appendElementEntStz(iArgTo,szArg1);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BETWEEN

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
        iArg2 = *(--paW);                                                   \
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

#define _ASSIGN_IFTRUE(iArgTo)                                              \
 {                                                                          \
  iArg1 = *(--paW);                                                         \
  if (iArg1)                                                                \
    iArgTo = *(--paW);                                                      \
  else                                                                      \
    --paW;                                                                  \
 }

#define _ASSIGN_TO_ENTINT(iArgTo)                                           \
  iArg1 = *(--paW);                                                         \
  vEntWrk_appendElementEntInt(iArgTo,iArg1)

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
// NULL TESTS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// do we need separate macros for all datatypes??? Not sure

#define _ISNOTNULL() \
  {                  \
  iArg1 = *(--paW);                                                         \
  (*paW++) = (iArg1 != C_DATUM_NULL ? 1 : 0);                               \
}

#define _ISNULL()                                                           \
  iArg1 = *(--paW);                                                         \
  (*paW++) = (iArg1 == C_DATUM_NULL ? 1 : 0);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
