/*============================================================================*/
/* RelationalMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//Check overflow when multiplying two bcd mantissas
#define _OF_BCDMUL(eErrorCode)                                                                    \
  {                                                                                               \
    bcdArg1.m_iAsI8 = *(--paW);                                                                   \
    bcdArg2.m_iAsI8 = *(--paW);                                                                   \
    if(bcdArg1.m_iMantissa == 0 || bcdArg2.m_iMantissa == 0) {                                    \
      eErrorCode = EC_NONE;                                                                       \
    } else if(bcdArg1.m_iMantissa == -1 && bcdArg2.m_iMantissa == C_BCD_MIN) {                      \
      eErrorCode = EC_ARITHMETIC_OVERFLOW;                                                        \
    } else if(bcdArg1.m_iMantissa == C_BCD_MIN && bcdArg2.m_iMantissa == -1) {                      \
      eErrorCode = EC_ARITHMETIC_OVERFLOW;                                                        \
    } else if(bcdArg2.m_iMantissa > C_BCD_MAX / bcdArg1.m_iMantissa) {                              \
      eErrorCode = EC_ARITHMETIC_OVERFLOW;                                                        \
    } else if(bcdArg1.m_iMantissa != -1 && bcdArg2.m_iMantissa < C_BCD_MIN / bcdArg1.m_iMantissa) { \
      eErrorCode = EC_ARITHMETIC_OVERFLOW;                                                        \
    }                                                                                             \
    *paW++ = bcdArg2.m_iAsI8;                                                                     \
    *paW++ = bcdArg1.m_iAsI8;                                                                     \
  }

//Rescale macro assumes we put the larger scaled argument on the stack first, and then the smaller scaled value.
//So bcdArg1 will be rescaled to a higher scale since it's popped off the stack first.
#define _RESCALE_BCD(eErrorCode)                                           \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    iScaleDiff = bcdArg2.m_iScale - bcdArg1.m_iScale;                      \
    bcdArg1.m_iScale = bcdArg2.m_iScale;                                   \
    *paW++ = bcdArg2.m_iAsI8;                                              \
    if(iScaleDiff < 0 || iScaleDiff > 14) {                                \
      eErrorCode = EC_SCALE_OUT_OF_BOUNDS;                                 \
    } else {                                                               \
      *paW++ = bcdArg1.m_iAsI8;                                            \
      *paW++ = iaPow10[iScaleDiff];                                        \
      _OF_BCDMUL(eErrorCode);                                              \
      if(eErrorCode == EC_NONE) {                                          \
        --paW;                                                             \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iMantissa = bcdArg1.m_iMantissa * iaPow10[iScaleDiff];   \
      }                                                                    \
    }                                                                      \
    *paW++ = bcdArg1.m_iAsI8;                                              \
  }

#define _EQ_BCDBCD(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = 0;                                                          \
    } else {                                                               \
      if(bcdArg1.m_iScale < bcdArg2.m_iScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_iScale < bcdArg1.m_iScale) {                     \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = 0;                                                        \
      } else {                                                             \
        *paW++ = bcdArg2.m_iMantissa == bcdArg1.m_iMantissa;               \
      }                                                                    \
    }                                                                      \
  }

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
      *paW++ = iArg2 > iArg1 ? 1 : 0;                                      \
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
      *paW++ = iArg2 < iArg1 ? 1 : 0;                                      \
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
