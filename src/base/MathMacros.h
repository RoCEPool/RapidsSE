/*============================================================================*/
/* MathMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ADD_BCDBCD(eErrorCode) \
  NEVER_REACH;

#define _DIV_BCDBCD(eErrorCode) \
  NEVER_REACH;

#define _MUL_BCDBCD(eErrorCode) \
  NEVER_REACH;

#define _SUB_BCDBCD(eErrorCode) \
  NEVER_REACH;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// FLOAT
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ADD_FLTFLT(eErrorCode) \
  NEVER_REACH;

#define _DIV_FLTFLT(eErrorCode) \
  NEVER_REACH;

#define _MUL_FLTFLT(eErrorCode) \
  NEVER_REACH;

#define _SUB_FLTFLT(eErrorCode) \
  NEVER_REACH;

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
    printf("arg1 = %d; arg2 = %d\n", iArg1, iArg2);                        \
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
  NEVER_REACH;

#define _MUL_INTINT(eErrorCode)                                            \
  NEVER_REACH;

#define _SUB_INTINT(eErrorCode)                                            \
  NEVER_REACH;



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
