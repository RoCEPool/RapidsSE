/*============================================================================*/
/* MathMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ADD_BCDBCD(eErrorCode) \
  RASSERT(0)

#define _DIV_BCDBCD(eErrorCode) \
  RASSERT(0)

#define _MUL_BCDBCD(eErrorCode) \
  RASSERT(0)

#define _SUB_BCDBCD(eErrorCode) \
  RASSERT(0)


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ADD_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg1 + iArg2;                                              \
    }                                                                      \
  }

#define _DIV_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
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
        *paW++ = iArg1 / iArg2;                                            \
      }                                                                    \
  }

#define _MOD_INTINT(eErrorCode)                                            \
  RASSERT(0)

#define _MUL_INTINT(eErrorCode)                                            \
  RASSERT(0)

#define _SUB_INTINT(eErrorCode)                                            \
  RASSERT(0)



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
