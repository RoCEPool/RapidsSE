/*============================================================================*/
/* RelationalMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _ROUND_BCD(eErrorCode)                                       \
  {                                                                  \
    bcdRound.m_iAsI8 = *(--paW);                                     \
    if (bcdRound.m_iMantissa % 10 >= 5) {                            \
      bcdRound.m_iMantissa = (bcdRound.m_iMantissa / 10) + 1;        \
    }                                                                \
    else {                                                           \
      bcdRound.m_iMantissa = bcdRound.m_iMantissa / 10;              \
    }                                                                \
    bcdRound.m_uScale--;                                             \
    *paW++ = bcdRound.m_iAsI8;                                       \
  }

// rescale BCD to max scale of the two bcd's
// CHECK FOR OVERFLOW !!!
#define _RESCALE_BCD_MAX_OF_TWO(eErrorCode)                           \
  {                                                                   \
    bcdReScale2.m_iAsI8 = *(--paW);                                   \
    bcdReScale1.m_iAsI8 = *(--paW);                                   \
    iScaleDiff = (i8) bcdReScale1.m_uScale - (i8) bcdReScale2.m_uScale; \
                                                                      \
    if (iScaleDiff > 0) {                                             \
      bcdReScale2.m_uScale = bcdReScale1.m_uScale;                    \
      bcdReScale2.m_iMantissa = bcdReScale2.m_iMantissa * aDynamicT->m_iaPow10[iScaleDiff]; \
    }                                                                 \
    else {                                                            \
      bcdReScale1.m_uScale = bcdReScale2.m_uScale;                    \
      bcdReScale1.m_iMantissa = bcdReScale1.m_iMantissa * aDynamicT->m_iaPow10[-iScaleDiff]; \
    }                                                                   \
                                                                        \
    *paW++ = bcdReScale1.m_iAsI8;                                       \
    *paW++ = bcdReScale2.m_iAsI8;                                       \
  }

//Rescale macro assumes we put the larger scaled argument on the stack first, and then the smaller scaled value.
//So bcdArg1 will be rescaled to a higher scale since it's popped off the stack first.
#define _RESCALE_BCD(eErrorCode)                                           \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    iScaleDiff = (i8) bcdArg2.m_uScale - (i8) bcdArg1.m_uScale;            \
    bcdArg1.m_uScale = bcdArg2.m_uScale;                                   \
    *paW++ = bcdArg2.m_iAsI8;                                              \
    if(iScaleDiff < 0 || iScaleDiff > C_MAX_BCD_SCALE) {                   \
      eErrorCode = EC_SCALE_OUT_OF_BOUNDS;                                 \
    } else {                                                               \
      bcdArg2.m_iMantissa = aDynamicT->m_iaPow10[iScaleDiff];              \
      eErrorCode = _OF_BCDMUL;                                             \
      if(eErrorCode == EC_NONE) {                                          \
        bcdArg1.m_iMantissa = bcdArg1.m_iMantissa * aDynamicT->m_iaPow10[iScaleDiff];   \
      }                                                                    \
    }                                                                      \
    *paW++ = bcdArg1.m_iAsI8;                                              \
  }

// Rescale a BCD on the stack to a given scale
#define _RESCALE_BCD_TO_SCALE(eErrorcode, iScale)                          \
  {                                                                        \
    bcdReScale1.m_iAsI8 = *(--paW);                                        \
    iScaleDiff = (iScale) - (i8) bcdReScale1.m_uScale;                     \
    if (iScaleDiff > 0) {                                                  \
      /* Need to check for overflow */                                     \
      if (C_I8_MAX / aDynamicT->m_iaPow10[iScaleDiff] < bcdReScale1.m_iMantissa) { \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
      } else {                                                             \
        bcdReScale1.m_uScale = (u8) (iScale);                              \
        bcdReScale1.m_iMantissa =                                          \
          bcdReScale1.m_iMantissa * aDynamicT->m_iaPow10[iScaleDiff];      \
      }                                                                    \
    } else if (iScaleDiff < 0) {                                           \
      /* No over/underflow check required for division. */                 \
      /* Adjust BCD, then round the last digit as needed. */               \
                                                                           \
      if (iScaleDiff < -1) {                                               \
        bcdReScale1.m_uScale = (u8) (iScale) + 1;                          \
        bcdReScale1.m_iMantissa =                                          \
          bcdReScale1.m_iMantissa / aDynamicT->m_iaPow10[(-iScaleDiff)-1]; \
      }                                                                    \
                                                                           \
      /* Now round the number */                                           \
      *paW++ = bcdReScale1.m_iAsI8;                                        \
      _ROUND_BCD(eErrorCode)                                               \
      bcdReScale1.m_iAsI8 = *(--paW);                                      \
    }                                                                      \
                                                                           \
    *paW++ = bcdReScale1.m_iAsI8;                                          \
  }
                                                                           \
#define _EQ_BCDBCD(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    } else {                                                               \
      if(bcdArg1.m_uScale < bcdArg2.m_uScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_uScale < bcdArg1.m_uScale) {                     \
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

#define _GE_BCDBCD(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    } else {                                                               \
      if(bcdArg1.m_uScale < bcdArg2.m_uScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_uScale < bcdArg1.m_uScale) {                     \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = 0;                                                        \
      } else {                                                             \
        *paW++ = bcdArg2.m_iMantissa >= bcdArg1.m_iMantissa;               \
      }                                                                    \
    }                                                                      \
  }

#define _GT_BCDBCD(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    } else {                                                               \
      if(bcdArg1.m_uScale < bcdArg2.m_uScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_uScale < bcdArg1.m_uScale) {                     \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = 0;                                                        \
      } else {                                                             \
        *paW++ = bcdArg2.m_iMantissa > bcdArg1.m_iMantissa;                \
      }                                                                    \
    }                                                                      \
  }

#define _GTA_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (bcdArg1.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (bcdArg2.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if(bcdArg1.m_uScale < bcdArg2.m_uScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_uScale < bcdArg1.m_uScale) {                     \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = 0;                                                        \
      } else {                                                             \
        *paW++ = bcdArg2.m_iMantissa > bcdArg1.m_iMantissa;                \
      }                                                                    \
    }                                                                      \
  }

#define _LE_BCDBCD(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    } else {                                                               \
      if(bcdArg1.m_uScale < bcdArg2.m_uScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_uScale < bcdArg1.m_uScale) {                     \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = 0;                                                        \
      } else {                                                             \
        *paW++ = bcdArg2.m_iMantissa <= bcdArg1.m_iMantissa;               \
      }                                                                    \
    }                                                                      \
  }

#define _LT_BCDBCD(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    } else {                                                               \
      if(bcdArg1.m_uScale < bcdArg2.m_uScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_uScale < bcdArg1.m_uScale) {                     \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = 0;                                                        \
      } else {                                                             \
        *paW++ = bcdArg2.m_iMantissa < bcdArg1.m_iMantissa;                \
      }                                                                    \
    }                                                                      \
  }

#define _LTA_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (bcdArg1.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (bcdArg2.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if(bcdArg1.m_uScale < bcdArg2.m_uScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_uScale < bcdArg1.m_uScale) {                     \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = 0;                                                        \
      } else {                                                             \
        *paW++ = bcdArg2.m_iMantissa < bcdArg1.m_iMantissa;                \
      }                                                                    \
    }                                                                      \
  }

#define _NE_BCDBCD(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    } else {                                                               \
      if(bcdArg1.m_uScale < bcdArg2.m_uScale) {                            \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
      } else if(bcdArg2.m_uScale < bcdArg1.m_uScale) {                     \
        *paW++ = bcdArg1.m_iAsI8;                                          \
        *paW++ = bcdArg2.m_iAsI8;                                          \
        _RESCALE_BCD(eErrorCode)                                           \
        bcdArg2.m_iAsI8 = *(--paW);                                        \
        bcdArg1.m_iAsI8 = *(--paW);                                        \
      }                                                                    \
      if(eErrorCode != EC_NONE) {                                          \
        *paW++ = 0;                                                        \
      } else {                                                             \
        *paW++ = bcdArg2.m_iMantissa != bcdArg1.m_iMantissa;               \
      }                                                                    \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BOOLEAN
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_BOLBOL(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 == iArg1;                                             \
    }                                                                      \
  }

#define _GE_BOLBOL(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 >= iArg1;                                             \
    }                                                                      \
  }

#define _GT_BOLBOL(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 > iArg1 ? 1 : 0;                                      \
    }                                                                      \
  }

#define _GTA_BOLBOL(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (iArg1 == C_DATUM_NULL) {                                      \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (iArg2 == C_DATUM_NULL) {                                      \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 > iArg1 ? 1 : 0;                                      \
    }                                                                      \
  }

#define _LE_BOLBOL(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 <= iArg1;                                             \
    }                                                                      \
  }

#define _LT_BOLBOL(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 < iArg1 ? 1 : 0;                                      \
    }                                                                      \
  }
#define _LTA_BOLBOL(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (iArg1 == C_DATUM_NULL) {                                      \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (iArg2 == C_DATUM_NULL) {                                      \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 < iArg1 ? 1 : 0;                                      \
    }                                                                      \
  }

#define _NE_BOLBOL(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 != iArg1 ? 1 : 0;                                     \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// FLOAT
#define AS_FLT(x) \
(    *(d8*)&x   )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define _EQ_FLTFLT(eErrorCode)                                             \
{                                                                          \
  fltArg1.m_iAsI8 = *(--paW);                                              \
  fltArg2.m_iAsI8 = *(--paW);                                              \
  if (eErrorCode != EC_NONE ||                                             \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                     \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                     \
    *paW++ = C_DATUM_NULL;                                                 \
  }                                                                        \
  else {                                                                   \
    *paW++ = fltArg2.m_dFlt == fltArg1.m_dFlt ? 1 : 0;                     \
  }                                                                        \
}

#define _GE_FLTFLT(eErrorCode)                                           \
{                                                                        \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
  *paW++ = fltArg2.m_dFlt >= fltArg1.m_dFlt ? 1 : 0;                     \
  }                                                                      \
}

#define _GT_FLTFLT(eErrorCode)                                           \
{                                                                        \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  fltArg2.m_iAsI8 = *(--paW);                                            \
                                                                         \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    *paW++ = fltArg2.m_dFlt > fltArg1.m_dFlt ? 1 : 0;                    \
  }                                                                      \
}

#define _GTA_FLTFLT(eErrorCode)                                            \
  {                                                                        \
    fltArg1.m_iAsI8 = *(--paW);                                            \
    fltArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (fltArg1.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (fltArg2.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = fltArg2.m_iAsI8 > fltArg1.m_iAsI8 ? 1 : 0;                  \
    }                                                                      \
  }

#define _LE_FLTFLT(eErrorCode)                                           \
{                                                                        \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  fltArg2.m_iAsI8 = *(--paW);                                            \
                                                                         \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    *paW++ = fltArg2.m_dFlt <= fltArg1.m_dFlt ? 1 : 0;                   \
  }                                                                      \
}

#define _LTA_FLTFLT(eErrorCode)                                            \
  {                                                                        \
    fltArg1.m_iAsI8 = *(--paW);                                            \
    fltArg2.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (fltArg1.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (fltArg2.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = fltArg2.m_dFlt < fltArg1.m_dFlt ? 1 : 0;                  \
    }                                                                      \
  }

// printf("INT:  %lld ? %lld \n", fltArg2.m_iAsI8, fltArg1.m_iAsI8) ;
// printf("FLOAT1:  %g ? %g \n", fltArg2.m_dFlt, fltArg1.m_dFlt) ;
#define _LT_FLTFLT(eErrorCode)                                           \
{                                                                        \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    *paW++ = fltArg2.m_dFlt < fltArg1.m_dFlt ? 1 : 0;                    \
  }                                                                      \
}

#define _NE_FLTFLT(eErrorCode)                                           \
{                                                                        \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    *paW++ = fltArg2.m_dFlt != fltArg1.m_dFlt ? 1: 0;                    \
  }                                                                      \
}

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
      *paW++ = C_DATUM_NULL;                                               \
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
      *paW++ = C_DATUM_NULL;                                               \
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
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 > iArg1 ? 1 : 0;                                      \
    }                                                                      \
  }

#define _GTA_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (iArg1 == C_DATUM_NULL) {                                      \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (iArg2 == C_DATUM_NULL) {                                      \
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
      *paW++ = C_DATUM_NULL;                                               \
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
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 < iArg1 ? 1 : 0;                                      \
    }                                                                      \
  }
#define _LTA_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (iArg1 == C_DATUM_NULL) {                                      \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (iArg2 == C_DATUM_NULL) {                                      \
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
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = iArg2 != iArg1 ? 1 : 0;                                     \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DATE-TIME INTERVAL
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_IVDIVD(eErrorCode)                                             \
  {                                                                        \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
      *paW++ = iIvlArg2 == iIvlArg1;                                       \
    }                                                                      \
  }

#define _GE_IVDIVD(eErrorCode)                                             \
  {                                                                        \
  ivlArg1.m_iAsI8 = *(--paW);                                              \
  ivlArg2.m_iAsI8 = *(--paW);                                              \
                                                                           \
  if (eErrorCode != EC_NONE ||                                             \
    ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                     \
    ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                     \
    *paW++ = C_DATUM_NULL;                                                 \
  }                                                                        \
  else {                                                                   \
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                     \
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                     \
    if (ivlArg1.m_xIvl.m_bIsNeg)                                           \
      iIvlArg1 = (-1) * iIvlArg1;                                          \
    if (ivlArg2.m_xIvl.m_bIsNeg)                                           \
      iIvlArg2 = (-1) * iIvlArg2;                                          \
    *paW++ = iIvlArg2 >= iIvlArg1;                                         \
  }                                                                        \
}

#define _GT_IVDIVD(eErrorCode)                                           \
{                                                                        \
  ivlArg1.m_iAsI8 = *(--paW);                                            \
  ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                         \
  if (eErrorCode != EC_NONE ||                                           \
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
    if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
      iIvlArg1 = (-1) * iIvlArg1;                                        \
    if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
      iIvlArg2 = (-1) * iIvlArg2;                                        \
    *paW++ = iIvlArg2 > iIvlArg1;                                        \
  }                                                                      \
}

#define _GTA_IVDIVD(eErrorCode) \
 {                                                                        \
   ivlArg1.m_iAsI8 = *(--paW);                                            \
   ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                          \
    if (eErrorCode != EC_NONE) {                                          \
      *paW++ = C_DATUM_NULL;                                              \
    }                                                                     \
    else if (ivlArg1.m_iAsI8 == C_DATUM_NULL) {                           \
      *paW++ = 1;                                                         \
    }                                                                     \
    else if (ivlArg2.m_iAsI8 == C_DATUM_NULL) {                           \
      *paW++ = 0;                                                         \
    }                                                                     \
    else {                                                                \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                  \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                  \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                        \
        iIvlArg1 = (-1) * iIvlArg1;                                       \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                        \
        iIvlArg2 = (-1) * iIvlArg2;                                       \
      *paW++ = iIvlArg2 > iIvlArg1;                                       \
    }                                                                     \
  }

#define _LE_IVDIVD(eErrorCode)                                           \
  {                                                                      \
  ivlArg1.m_iAsI8 = *(--paW);                                            \
  ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                         \
  if (eErrorCode != EC_NONE ||                                           \
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
     i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                  \
     i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                  \
     if (ivlArg1.m_xIvl.m_bIsNeg)                                        \
       iIvlArg1 = (-1) * iIvlArg1;                                       \
     if (ivlArg2.m_xIvl.m_bIsNeg)                                        \
       iIvlArg2 = (-1) * iIvlArg2;                                       \
     *paW++ = iIvlArg2 <= iIvlArg1;                                      \
  }                                                                      \
}

#define _LT_IVDIVD(eErrorCode)                                             \
  {                                                                        \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
      *paW++ = iIvlArg2 < iIvlArg1;                                        \
    }                                                                      \
 }

#define _LTA_IVDIVD(eErrorCode)                                            \
  {                                                                        \
   ivlArg1.m_iAsI8 = *(--paW);                                             \
   ivlArg2.m_iAsI8 = *(--paW);                                             \
                                                                           \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (ivlArg1.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (ivlArg2.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
      *paW++ = iIvlArg2 < iIvlArg1;                                        \
    }                                                                      \
  }

#define _NE_IVDIVD(eErrorCode)                                             \
  {                                                                        \
  ivlArg1.m_iAsI8 = *(--paW);                                              \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlDT.m_uMicros;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
      *paW++ = iIvlArg2 != iIvlArg1;                                       \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// YEAR-MONTH INTERVAL
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_IVYIVY(eErrorCode)                                             \
  {                                                                        \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
      *paW++ = iIvlArg2 == iIvlArg1;                                       \
    }                                                                      \
  }

#define _GE_IVYIVY(eErrorCode)                                             \
  {                                                                        \
  ivlArg1.m_iAsI8 = *(--paW);                                              \
  ivlArg2.m_iAsI8 = *(--paW);                                              \
                                                                           \
  if (eErrorCode != EC_NONE ||                                             \
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                                 \
  }                                                                        \
  else {                                                                   \
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                     \
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                     \
    if (ivlArg1.m_xIvl.m_bIsNeg)                                           \
      iIvlArg1 = (-1) * iIvlArg1;                                          \
    if (ivlArg2.m_xIvl.m_bIsNeg)                                           \
      iIvlArg2 = (-1) * iIvlArg2;                                          \
    *paW++ = iIvlArg2 >= iIvlArg1;                                         \                                                    \
  }                                                                        \
}

#define _GT_IVYIVY(eErrorCode)                                             \
{                                                                          \
  ivlArg1.m_iAsI8 = *(--paW);                                              \
  ivlArg2.m_iAsI8 = *(--paW);                                              \
                                                                           \
  if (eErrorCode != EC_NONE ||                                             \
      ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
      ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                                 \
  }                                                                        \
  else {                                                                   \
    i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                     \
    i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                     \
    if (ivlArg1.m_xIvl.m_bIsNeg)                                           \
      iIvlArg1 = (-1) * iIvlArg1;                                          \
    if (ivlArg2.m_xIvl.m_bIsNeg)                                           \
      iIvlArg2 = (-1) * iIvlArg2;                                          \
    *paW++ = iIvlArg2 > iIvlArg1;                                          \
  }                                                                        \
}

#define _GTA_IVYIVY(eErrorCode) \
 {                                                                        \
   ivlArg1.m_iAsI8 = *(--paW);                                            \
   ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                          \
    if (eErrorCode != EC_NONE) {                                          \
      *paW++ = C_DATUM_NULL;                                              \
    }                                                                     \
    else if (ivlArg1.m_iAsI8 == C_DATUM_NULL) {                           \
      *paW++ = 1;                                                         \
    }                                                                     \
    else if (ivlArg2.m_iAsI8 == C_DATUM_NULL) {                           \
      *paW++ = 0;                                                         \
    }                                                                     \
    else {                                                                \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                  \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                  \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                        \
        iIvlArg1 = (-1) * iIvlArg1;                                       \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                        \
        iIvlArg2 = (-1) * iIvlArg2;                                       \
      *paW++ = iIvlArg2 > iIvlArg1;                                       \
    }                                                                     \
  }

#define _LE_IVYIVY(eErrorCode)                                             \
  {                                                                        \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
      *paW++ = iIvlArg2 <= iIvlArg1;                                       \
    }                                                                      \
  }

#define _LT_IVYIVY(eErrorCode)                                             \
  {                                                                        \
    ivlArg1.m_iAsI8 = *(--paW);                                            \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
      *paW++ = iIvlArg2 < iIvlArg1;                                        \
    }                                                                      \
 }

#define _LTA_IVYIVY(eErrorCode)                                            \
  {                                                                        \
   ivlArg1.m_iAsI8 = *(--paW);                                             \
   ivlArg2.m_iAsI8 = *(--paW);                                             \
                                                                           \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (ivlArg1.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (ivlArg2.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
    *paW++ = iIvlArg2 < iIvlArg1;                                          \
    }                                                                      \
  }

#define _NE_IVYIVY(eErrorCode)                                             \
  {                                                                        \
  ivlArg1.m_iAsI8 = *(--paW);                                              \
    ivlArg2.m_iAsI8 = *(--paW);                                            \
                                                                           \
    if (eErrorCode != EC_NONE ||                                           \
        ivlArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        ivlArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      i8 iIvlArg1 = ivlArg1.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      i8 iIvlArg2 = ivlArg2.m_xIvl.u.m_xIvlYM.m_uMonths;                   \
      if (ivlArg1.m_xIvl.m_bIsNeg)                                         \
        iIvlArg1 = (-1) * iIvlArg1;                                        \
      if (ivlArg2.m_xIvl.m_bIsNeg)                                         \
        iIvlArg2 = (-1) * iIvlArg2;                                        \
      *paW++ = iIvlArg2 != iIvlArg1;                                       \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TIME
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 == xTme1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _NE_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 != xTme1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _GE_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 >= xTme1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _GT_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 > xTme1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

#define _GTA_TMETME(eErrorCode)                                            \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (xTme1.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (xTme2.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 > xTme1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

#define _LE_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 <= xTme1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _LT_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 < xTme1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

#define _LTA_TMETME(eErrorCode)                                            \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (xTme1.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (xTme2.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 < xTme1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// TIMESTAMP
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 == xTsp1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _NE_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 != xTsp1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _GE_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 >= xTsp1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _GT_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 > xTsp1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

#define _GTA_TSPTSP(eErrorCode)                                            \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (xTsp1.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (xTsp2.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 > xTsp1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

#define _LE_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 <= xTsp1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _LT_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 < xTsp1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

#define _LTA_TSPTSP(eErrorCode)                                            \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (xTsp1.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (xTsp2.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 < xTsp1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// DATE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 == xDte1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _NE_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 != xDte1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _GE_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
    *paW++ = xDte2.m_iAsI8 >= xDte1.m_iAsI8 ? 1 : 0;                       \
    }                                                                      \
  }

#define _GT_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if(xDte2.m_iAsI8 == xDte1.m_iAsI8) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 > xDte1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

#define _GTA_DTEDTE(eErrorCode)                                            \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (xDte1.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (xDte2.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 > xDte1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

#define _LE_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ =C_DATUM_NULL;                                                \
    }                                                                      \
    else if(xDte2.m_iAsI8 == xDte1.m_iAsI8) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 <= xDte1.m_iAsI8 ? 1 : 0;                     \
    }                                                                      \
  }

#define _LT_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 < xDte1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }                                                                        \

#define _LTA_DTEDTE(eErrorCode)                                            \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (xDte1.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (xDte2.m_iAsI8 == C_DATUM_NULL) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 < xDte1.m_iAsI8 ? 1 : 0;                      \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// UNKNOWN
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_ANYUNK(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _GT_ANYUNK(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _GTA_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = 1;                                                            \
  }

#define _GE_ANYUNK(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }
  
#define _LT_ANYUNK(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _LTA_ANYUNK(eErrorCode)                                            \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = 1;                                                            \
  }

#define _LE_ANYUNK(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }
  
#define _NE_ANYUNK(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _EQ_UNKANY(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _GT_UNKANY(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _GTA_UNKANY(eErrorCode)                                            \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = 0;                                                            \
  }

#define _GE_UNKANY(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _LT_UNKANY(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _LTA_UNKANY(eErrorCode)                                            \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = 0;                                                            \
  }

#define _LE_UNKANY(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }

#define _NE_UNKANY(eErrorCode)                                             \
  {                                                                        \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    *paW++ = C_DATUM_NULL;                                                 \
  }
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// VARCHAR
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _EQ_VCHVCH(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                                          \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg2, (char*)iArg1) == 0)                         \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _GE_VCHVCH(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg2, (char*)iArg1) >= 0)                         \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _GT_VCHVCH(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg2, (char*)iArg1) > 0)                          \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _GTA_VCHVCH(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (iArg1 == C_DATUM_NULL) {                                      \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (iArg2 == C_DATUM_NULL) {                                      \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
       if (strcmp((char*)iArg2, (char*)iArg1) > 0)                         \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _GTA_VCHVCH_PL(eErrorCode)                                         \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (*(i8*)iArg1 == C_DATUM_NULL) {                                \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (*(i8*)iArg1 == C_DATUM_NULL) {                                \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
       if (strcmp((char*)iArg2, (char*)iArg1) > 0)                         \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _LE_VCHVCH(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg2, (char*)iArg1) <= 0)                         \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _LT_VCHVCH(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg2, (char*)iArg1) < 0)                          \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _LTA_VCHVCH(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (iArg1 == C_DATUM_NULL) {                                      \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (iArg2 == C_DATUM_NULL) {                                      \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg2, (char*)iArg1) < 0)                          \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _LTA_VCHVCH_PL(eErrorCode)                                         \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (*(i8*)iArg1 == C_DATUM_NULL) {                                \
      *paW++ = 1;                                                          \
    }                                                                      \
    else if (*(i8*)iArg2 == C_DATUM_NULL) {                                \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg2, (char*)iArg1) < 0)                          \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

#define _NE_VCHVCH(eErrorCode)                                             \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE ||                                           \
        iArg1 == C_DATUM_NULL ||                                           \
        iArg2 == C_DATUM_NULL) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg2, (char*)iArg1) != 0)                         \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 0;                                                        \
    }                                                                      \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/