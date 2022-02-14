/*============================================================================*/
/* RelationalMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// FLOAT
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_INTINT(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 == iArg1;                                             \
    }                                                                      \
  }

#define _GE_INTINT(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 >= iArg1;                                             \
    }                                                                      \
  }

#define _GT_INTINT(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 > iArg1;                                             \
    }                                                                      \
  }

#define _LE_INTINT(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 <= iArg1;                                             \
    }                                                                      \
  }

#define _LT_INTINT(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 < iArg1;                                              \
    }                                                                      \
  }

#define _NE_INTINT(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 != iArg1;                                             \
    }                                                                      \
  }

#define _LT_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 < iArg1;                                              \
    }                                                                      \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
