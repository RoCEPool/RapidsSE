/*============================================================================*/
/* TccMacros.h -- Function Macros                                             */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PUSHES

#define _PSH_BOL(iArg)  \
  *paW++ = (i8)iArg;

#define _PSH_BCD(iArg)  \
  *paW++ = iArg;

#define _PSH_DTE(iArg)  \
  *paW++ = iArg;

#define _PSH_FLT(dArg)  \
  *paW++ = (i8)dArg;

#define _PSH_INT(iArg)  \
  *paW++ = iArg;

#define _PSH_IVL(iArg)  \
  *paW++ = iArg;

#define _PSH_IVY(iArg)  \
  *paW++ = iArg;

#define _PSH_TME(iArg)  \
  *paW++ = iArg;

#define _PSH_TSP(iArg)  \
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

#define _ASSIGN_TO_ENTINT(iArgTo)                                           \
  iArg1 = *(--paW);                                                        \
  vEntWrk_appendElementEntInt(iArgTo,iArg1)

#define _ASSIGN_TO_ENTSTZ(iArgTo)                                           \
  szArg1 = (char*)*(--paW);                                                 \
  vEntWrk_appendElementEntStz(iArgTo,szArg1);

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

#define FM_ISNOTNULL(arg1, piErrorCode) \
  (arg1 != C_DATUM_NULL ? 1 : 0)

#define FM_ISNULL(arg1, piErrorCode) \
  (arg1 == C_DATUM_NULL ? 1 : 0)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
