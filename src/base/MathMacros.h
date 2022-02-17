/*============================================================================*/
/* MathMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Utilities
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _OF_SIGNEDADD                                                      \
(                                                                          \
  (iArg1 > 0 && iArg2 > C_I8_MAX - iArg1) ? EC_ARITHMETIC_OVERFLOW :       \
  (iArg1 < 0 && iArg2 < C_I8_MIN - iArg1) ? EC_ARITHMETIC_UNDERFLOW :      \
  EC_NONE                                                                  \
)

#define _OF_SIGNEDSUB                                                      \
(                                                                          \
  (iArg1 < 0 && iArg2 > C_I8_MAX + iArg1) ? EC_ARITHMETIC_OVERFLOW :       \
  (iArg1 > 0 && iArg2 < C_I8_MIN + iArg1) ? EC_ARITHMETIC_UNDERFLOW :      \
  EC_NONE                                                                  \
)

//If one number is -1 and another is INT_MIN, multiplying them we get abs(INT_MIN) which is 1 higher than INT_MAX

#define _OF_SIGNEDMUL                                                      \
(                                                                          \
  (iArg1 == 0 || iArg2 == 0) ? EC_NONE :                                   \
  (iArg1 == -1 && iArg2 == C_I8_MIN) ? EC_ARITHMETIC_OVERFLOW :            \
  (iArg1 == C_I8_MIN && iArg2 == -1) ? EC_ARITHMETIC_OVERFLOW :            \
  (iArg2 > C_I8_MAX / iArg1) ? EC_ARITHMETIC_OVERFLOW :                    \
  (iArg1 != -1 && iArg2 < C_I8_MIN / iArg1) ? EC_ARITHMETIC_UNDERFLOW :    \
  EC_NONE                                                                  \
)

//For division (except for the INT_MIN and -1 special case), there isn't any possibility of going over INT_MIN or INT_MAX
#define _OF_SIGNEDDIV                                                      \
(                                                                          \
  (iArg1 == C_I8_MIN && iArg2 == -1) ? EC_ARITHMETIC_OVERFLOW :            \
  EC_NONE                                                                  \
)
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
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      eErrorCode = _OF_SIGNEDADD;                                          \
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
        eErrorCode = _OF_SIGNEDDIV;                                        \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          *paW++ = iArg1 / iArg2;                                          \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

#define _MOD_INTINT(eErrorCode)                                            \
  NEVER_REACH;

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
      eErrorCode = _OF_SIGNEDMUL;                                          \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        *paW++ = iArg1 * iArg2;                                            \
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
      eErrorCode = _OF_SIGNEDSUB;                                          \
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



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
