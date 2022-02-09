/*============================================================================*/
/* TccMacros.h -- Function Macros                                             */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// PUSHES

#define _PUSHINT(iArg)  \
  *paW++ = iArg;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// POPS

#define _POPINT()  \
  *(--paW);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ASSIGNS

#define _ASSIGN(iArgTo)                                                    \
  iArgTo =*(--paW);

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
