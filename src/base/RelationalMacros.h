/*============================================================================*/
/* RelationalMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
      bcdArg2.m_iMantissa = iaPow10[iScaleDiff];                           \
      eErrorCode = _OF_BCDMUL;                                             \
      if(eErrorCode == EC_NONE) {                                          \
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

#define _GE_BCDBCD(eErrorCode)                                             \
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
        *paW++ = bcdArg2.m_iMantissa != bcdArg1.m_iMantissa;               \
      }                                                                    \
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
    *paW++ = 0;                                                            \
  }                                                                        \
  else {                                                                   \
  *paW++ = fltArg2.m_dFlt == fltArg1.m_dFlt ? 1 : 0;                       \
  }                                                                        \
}

#define _GE_FLTFLT(eErrorCode)                                           \
{                                                                        \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = 0;                                                          \
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
    *paW++ = 0;                                                          \
  }                                                                      \
  else {                                                                 \
    *paW++ = fltArg2.m_dFlt > fltArg1.m_dFlt ? 1 : 0;                    \
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
    *paW++ = 0;                                                          \
  }                                                                      \
  else {                                                                 \
    *paW++ = fltArg2.m_dFlt <= fltArg1.m_dFlt ? 1 : 0;                   \
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
    *paW++ = 0;                                                          \
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
    *paW++ = 0;                                                          \
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
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 == xTme1.m_iAsI8;                             \
    }                                                                      \
  }

#define _NE_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTme2.m_iAsI8 != xTme1.m_iAsI8;                             \
    }                                                                      \
  }

#define _GE_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xTme2.m_iAsI8 == xTme1.m_iAsI8) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xTme2.m_uHour > xTme1.m_uHour) {                                  \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xTme2.m_uHour == xTme1.m_uHour) {                            \
        if(xTme2.m_uMinute > xTme1.m_uMinute) {                            \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xTme2.m_uMinute == xTme1.m_uMinute) {                      \
          if(xTme2.m_uSecond > xTme1.m_uSecond) {                          \
            *paW++ = 1;                                                    \
          }                                                                \
          else if(xTme2.m_uSecond == xTme1.m_uSecond) {                    \
            if(xTme2.m_uMicro >= xTme1.m_uMicro) {                         \
              *paW++ = 1;                                                  \
            }                                                              \
            else {                                                         \
              *paW++ = 0;                                                  \
            }                                                              \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _GT_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xTme2.m_iAsI8 == xTme1.m_iAsI8) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xTme2.m_uHour > xTme1.m_uHour) {                                  \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xTme2.m_uHour == xTme1.m_uHour) {                            \
        if(xTme2.m_uMinute > xTme1.m_uMinute) {                            \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xTme2.m_uMinute == xTme1.m_uMinute) {                      \
          if(xTme2.m_uSecond > xTme1.m_uSecond) {                          \
            *paW++ = 1;                                                    \
          }                                                                \
          else if(xTme2.m_uSecond == xTme1.m_uSecond) {                    \
            if(xTme2.m_uMicro > xTme1.m_uMicro) {                          \
              *paW++ = 1;                                                  \
            }                                                              \
            else {                                                         \
              *paW++ = 0;                                                  \
            }                                                              \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _LE_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xTme2.m_iAsI8 == xTme1.m_iAsI8) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xTme2.m_uHour < xTme1.m_uHour) {                                  \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xTme2.m_uHour == xTme1.m_uHour) {                            \
        if(xTme2.m_uMinute < xTme1.m_uMinute) {                            \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xTme2.m_uMinute == xTme1.m_uMinute) {                      \
          if(xTme2.m_uSecond < xTme1.m_uSecond) {                          \
            *paW++ = 1;                                                    \
          }                                                                \
          else if(xTme2.m_uSecond == xTme1.m_uSecond) {                    \
            if(xTme2.m_uMicro <= xTme1.m_uMicro) {                         \
              *paW++ = 1;                                                  \
            }                                                              \
            else {                                                         \
              *paW++ = 0;                                                  \
            }                                                              \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _LT_TMETME(eErrorCode)                                             \
  {                                                                        \
    xTme1.m_iAsI8 = *(--paW);                                              \
    xTme2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTme1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTme2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xTme2.m_iAsI8 == xTme1.m_iAsI8) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xTme2.m_uHour < xTme1.m_uHour) {                                  \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xTme2.m_uHour == xTme1.m_uHour) {                            \
        if(xTme2.m_uMinute < xTme1.m_uMinute) {                            \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xTme2.m_uMinute == xTme1.m_uMinute) {                      \
          if(xTme2.m_uSecond < xTme1.m_uSecond) {                          \
            *paW++ = 1;                                                    \
          }                                                                \
          else if(xTme2.m_uSecond == xTme1.m_uSecond) {                    \
            if(xTme2.m_uMicro < xTme1.m_uMicro) {                          \
              *paW++ = 1;                                                  \
            }                                                              \
            else {                                                         \
              *paW++ = 0;                                                  \
            }                                                              \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
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
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 == xTsp1.m_iAsI8;                             \
    }                                                                      \
  }

#define _NE_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xTsp2.m_iAsI8 != xTsp1.m_iAsI8;                             \
    }                                                                      \
  }

#define _GE_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xTsp2.m_iAsI8 == xTsp1.m_iAsI8) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xTsp2.m_uYearDelta > xTsp1.m_uYearDelta) {                        \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xTsp2.m_uYearDelta == xTsp1.m_uYearDelta) {                  \
        if(xTsp2.m_uMonth > xTsp1.m_uMonth) {                              \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xTsp2.m_uMonth == xTsp1.m_uMonth) {                        \
          if(xTsp2.m_uDay > xTsp1.m_uDay) {                                \
            *paW++ = 1;                                                    \
          }                                                                \
          else if(xTsp2.m_uDay == xTsp1.m_uDay) {                          \
            if(xTsp2.m_uHour > xTsp1.m_uHour) {                            \
              *paW++ = 1;                                                  \
            }                                                              \
            else if(xTsp2.m_uHour == xTsp1.m_uHour) {                      \
              if(xTsp2.m_uMinute > xTsp1.m_uMinute) {                      \
                *paW++ = 1;                                                \
              }                                                            \
              else if(xTsp2.m_uMinute == xTsp1.m_uMinute) {                \
                if(xTsp2.m_uSecond > xTsp1.m_uSecond) {                    \
                  *paW++ = 1;                                              \
                }                                                          \
                else if(xTsp2.m_uSecond == xTsp1.m_uSecond) {              \
                  if(xTsp2.m_uMicro >= xTsp1.m_uMicro) {                   \
                    *paW++ = 1;                                            \
                  }                                                        \
                  else {                                                   \
                    *paW++ = 0;                                            \
                  }                                                        \
                }                                                          \
                else {                                                     \
                  *paW++ = 0;                                              \
                }                                                          \
              }                                                            \
              else {                                                       \
                *paW++ = 0;                                                \
              }                                                            \
            }                                                              \
            else {                                                         \
              *paW++ = 0;                                                  \
            }                                                              \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _GT_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xTsp2.m_iAsI8 == xTsp1.m_iAsI8) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xTsp2.m_uYearDelta > xTsp1.m_uYearDelta) {                        \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xTsp2.m_uYearDelta == xTsp1.m_uYearDelta) {                  \
        if(xTsp2.m_uMonth > xTsp1.m_uMonth) {                              \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xTsp2.m_uMonth == xTsp1.m_uMonth) {                        \
          if(xTsp2.m_uDay > xTsp1.m_uDay) {                                \
            *paW++ = 1;                                                    \
          }                                                                \
          else if(xTsp2.m_uDay == xTsp1.m_uDay) {                          \
            if(xTsp2.m_uHour > xTsp1.m_uHour) {                            \
              *paW++ = 1;                                                  \
            }                                                              \
            else if(xTsp2.m_uHour == xTsp1.m_uHour) {                      \
              if(xTsp2.m_uMinute > xTsp1.m_uMinute) {                      \
                *paW++ = 1;                                                \
              }                                                            \
              else if(xTsp2.m_uMinute == xTsp1.m_uMinute) {                \
                if(xTsp2.m_uSecond > xTsp1.m_uSecond) {                    \
                  *paW++ = 1;                                              \
                }                                                          \
                else if(xTsp2.m_uSecond == xTsp1.m_uSecond) {              \
                  if(xTsp2.m_uMicro > xTsp1.m_uMicro) {                    \
                    *paW++ = 1;                                            \
                  }                                                        \
                  else {                                                   \
                    *paW++ = 0;                                            \
                  }                                                        \
                }                                                          \
                else {                                                     \
                  *paW++ = 0;                                              \
                }                                                          \
              }                                                            \
              else {                                                       \
                *paW++ = 0;                                                \
              }                                                            \
            }                                                              \
            else {                                                         \
              *paW++ = 0;                                                  \
            }                                                              \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _LE_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xTsp2.m_iAsI8 == xTsp1.m_iAsI8) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xTsp2.m_uYearDelta < xTsp1.m_uYearDelta) {                        \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xTsp2.m_uYearDelta == xTsp1.m_uYearDelta) {                  \
        if(xTsp2.m_uMonth < xTsp1.m_uMonth) {                              \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xTsp2.m_uMonth == xTsp1.m_uMonth) {                        \
          if(xTsp2.m_uDay < xTsp1.m_uDay) {                                \
            *paW++ = 1;                                                    \
          }                                                                \
          else if(xTsp2.m_uDay == xTsp1.m_uDay) {                          \
            if(xTsp2.m_uHour < xTsp1.m_uHour) {                            \
              *paW++ = 1;                                                  \
            }                                                              \
            else if(xTsp2.m_uHour == xTsp1.m_uHour) {                      \
              if(xTsp2.m_uMinute < xTsp1.m_uMinute) {                      \
                *paW++ = 1;                                                \
              }                                                            \
              else if(xTsp2.m_uMinute == xTsp1.m_uMinute) {                \
                if(xTsp2.m_uSecond < xTsp1.m_uSecond) {                    \
                  *paW++ = 1;                                              \
                }                                                          \
                else if(xTsp2.m_uSecond == xTsp1.m_uSecond) {              \
                  if(xTsp2.m_uMicro <= xTsp1.m_uMicro) {                   \
                    *paW++ = 1;                                            \
                  }                                                        \
                  else {                                                   \
                    *paW++ = 0;                                            \
                  }                                                        \
                }                                                          \
                else {                                                     \
                  *paW++ = 0;                                              \
                }                                                          \
              }                                                            \
              else {                                                       \
                *paW++ = 0;                                                \
              }                                                            \
            }                                                              \
            else {                                                         \
              *paW++ = 0;                                                  \
            }                                                              \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _LT_TSPTSP(eErrorCode)                                             \
  {                                                                        \
    xTsp1.m_iAsI8 = *(--paW);                                              \
    xTsp2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xTsp1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xTsp2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xTsp2.m_iAsI8 == xTsp1.m_iAsI8) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xTsp2.m_uYearDelta < xTsp1.m_uYearDelta) {                        \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xTsp2.m_uYearDelta == xTsp1.m_uYearDelta) {                  \
        if(xTsp2.m_uMonth < xTsp1.m_uMonth) {                              \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xTsp2.m_uMonth == xTsp1.m_uMonth) {                        \
          if(xTsp2.m_uDay < xTsp1.m_uDay) {                                \
            *paW++ = 1;                                                    \
          }                                                                \
          else if(xTsp2.m_uDay == xTsp1.m_uDay) {                          \
            if(xTsp2.m_uHour < xTsp1.m_uHour) {                            \
              *paW++ = 1;                                                  \
            }                                                              \
            else if(xTsp2.m_uHour == xTsp1.m_uHour) {                      \
              if(xTsp2.m_uMinute < xTsp1.m_uMinute) {                      \
                *paW++ = 1;                                                \
              }                                                            \
              else if(xTsp2.m_uMinute == xTsp1.m_uMinute) {                \
                if(xTsp2.m_uSecond < xTsp1.m_uSecond) {                    \
                  *paW++ = 1;                                              \
                }                                                          \
                else if(xTsp2.m_uSecond == xTsp1.m_uSecond) {              \
                  if(xTsp2.m_uMicro < xTsp1.m_uMicro) {                    \
                    *paW++ = 1;                                            \
                  }                                                        \
                  else {                                                   \
                    *paW++ = 0;                                            \
                  }                                                        \
                }                                                          \
                else {                                                     \
                  *paW++ = 0;                                              \
                }                                                          \
              }                                                            \
              else {                                                       \
                *paW++ = 0;                                                \
              }                                                            \
            }                                                              \
            else {                                                         \
              *paW++ = 0;                                                  \
            }                                                              \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
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
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 == xDte1.m_iAsI8;                             \
    }                                                                      \
  }

#define _NE_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      *paW++ = xDte2.m_iAsI8 != xDte1.m_iAsI8;                             \
    }                                                                      \
  }

#define _GE_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xDte2.m_iAsI8 == xDte1.m_iAsI8) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xDte2.m_uYearDelta > xDte1.m_uYearDelta) {                        \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xDte2.m_uYearDelta == xDte1.m_uYearDelta) {                  \
        if(xDte2.m_uMonth > xDte1.m_uMonth) {                              \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xDte2.m_uMonth == xDte1.m_uMonth) {                        \
          if(xDte2.m_uDay >= xDte1.m_uDay) {                               \
            *paW++ = 1;                                                    \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _GT_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xDte2.m_iAsI8 == xDte1.m_iAsI8) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xDte2.m_uYearDelta > xDte1.m_uYearDelta) {                        \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xDte2.m_uYearDelta == xDte1.m_uYearDelta) {                  \
        if(xDte2.m_uMonth > xDte1.m_uMonth) {                              \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xDte2.m_uMonth == xDte1.m_uMonth) {                        \
          if(xDte2.m_uDay > xDte1.m_uDay) {                                \
            *paW++ = 1;                                                    \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _LE_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xDte2.m_iAsI8 == xDte1.m_iAsI8) {                              \
      *paW++ = 1;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xDte2.m_uYearDelta < xDte1.m_uYearDelta) {                        \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xDte2.m_uYearDelta == xDte1.m_uYearDelta) {                  \
        if(xDte2.m_uMonth < xDte1.m_uMonth) {                              \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xDte2.m_uMonth == xDte1.m_uMonth) {                        \
          if(xDte2.m_uDay <= xDte1.m_uDay) {                                \
            *paW++ = 1;                                                    \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
  }

#define _LT_DTEDTE(eErrorCode)                                             \
  {                                                                        \
    xDte1.m_iAsI8 = *(--paW);                                              \
    xDte2.m_iAsI8 = *(--paW);                                              \
    if (eErrorCode != EC_NONE ||                                           \
        xDte1.m_iAsI8 == C_DATUM_NULL ||                                   \
        xDte2.m_iAsI8 == C_DATUM_NULL) {                                   \
      *paW++ = 0;                                                          \
    }                                                                      \
    else if(xDte2.m_iAsI8 == xDte1.m_iAsI8) {                              \
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if(xDte2.m_uYearDelta < xDte1.m_uYearDelta) {                        \
        *paW++ = 1;                                                        \
      }                                                                    \
      else if(xDte2.m_uYearDelta == xDte1.m_uYearDelta) {                  \
        if(xDte2.m_uMonth < xDte1.m_uMonth) {                              \
          *paW++ = 1;                                                      \
        }                                                                  \
        else if(xDte2.m_uMonth == xDte1.m_uMonth) {                        \
          if(xDte2.m_uDay < xDte1.m_uDay) {                                \
            *paW++ = 1;                                                    \
          }                                                                \
          else {                                                           \
            *paW++ = 0;                                                    \
          }                                                                \
        }                                                                  \
        else {                                                             \
          *paW++ = 0;                                                      \
        }                                                                  \
      }                                                                    \
      else {                                                               \
        *paW++ = 0;                                                        \
      }                                                                    \
    }                                                                      \
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
      *paW++ = 0;                                                          \
    }                                                                      \
    else {                                                                 \
      if (strcmp((char*)iArg1, (char*)iArg2) == 0)                         \
        *paW++ = 1;                                                        \
      else                                                                 \
        *paW++ = 8;                                                        \
    }                                                                      \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/