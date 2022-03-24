/*============================================================================*/
/* MathMacros.h -- Math Macros                                                */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Utilities
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Overflow detection bcd
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _OF_BCDADD                                                         \
(                                                                          \
  (bcdArg1.m_iMantissa > 0 &&                                              \
  bcdArg2.m_iMantissa > C_BCD_MAX - bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  (bcdArg1.m_iMantissa < 0 &&                                              \
  bcdArg2.m_iMantissa < C_BCD_MIN - bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_UNDERFLOW :                                                \
  EC_NONE                                                                  \
)

#define _OF_BCDSUB                                                         \
(                                                                          \
  (bcdArg1.m_iMantissa < 0 &&                                              \
  bcdArg2.m_iMantissa > C_BCD_MAX + bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_OVERFLOW :                                                 \
  (bcdArg1.m_iMantissa > 0 &&                                              \
  bcdArg2.m_iMantissa < C_BCD_MIN + bcdArg1.m_iMantissa) ?                 \
  EC_ARITHMETIC_UNDERFLOW :                                                \
  EC_NONE                                                                  \
)

#define _OF_BCDMUL                                                         \
(                                                                          \
  EC_NONE                                                                  \
)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Overflow detection i8
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _OF_I8ADD                                                          \
(                                                                          \
  (iArg1 > 0 && iArg2 > C_I8_MAX - iArg1) ? EC_ARITHMETIC_OVERFLOW :       \
  (iArg1 < 0 && iArg2 < C_I8_MIN - iArg1) ? EC_ARITHMETIC_UNDERFLOW :      \
  EC_NONE                                                                  \
)

#define _OF_I8SUB                                                          \
(                                                                          \
  (iArg1 < 0 && iArg2 > C_I8_MAX + iArg1) ? EC_ARITHMETIC_OVERFLOW :       \
  (iArg1 > 0 && iArg2 < C_I8_MIN + iArg1) ? EC_ARITHMETIC_UNDERFLOW :      \
  EC_NONE                                                                  \
)

//If one number is -1 and another is INT_MIN, multiplying them we get abs(INT_MIN) which is 1 higher than INT_MAX

#define _OF_I8MUL                                                          \
(                                                                          \
  (iArg1 == 0 || iArg2 == 0) ? EC_NONE :                                   \
  (iArg1 == -1 && iArg2 == C_I8_MIN) ? EC_ARITHMETIC_OVERFLOW :            \
  (iArg1 == C_I8_MIN && iArg2 == -1) ? EC_ARITHMETIC_OVERFLOW :            \
  (iArg2 > C_I8_MAX / iArg1) ? EC_ARITHMETIC_OVERFLOW :                    \
  (iArg1 != -1 && iArg2 < C_I8_MIN / iArg1) ? EC_ARITHMETIC_UNDERFLOW :    \
  EC_NONE                                                                  \
)

//For division (except for the INT_MIN and -1 special case), there isn't any possibility of going over INT_MIN or INT_MAX
#define _OF_I8DIV                                                          \
(                                                                          \
  (iArg1 == C_I8_MIN && iArg2 == -1) ? EC_ARITHMETIC_OVERFLOW :            \
  EC_NONE                                                                  \
)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BCD
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _ADD_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
        if(bcdArg1.m_iScale < bcdArg2.m_iScale) {                          \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          _RESCALE_BCD(eErrorCode)                                         \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
        else if(bcdArg1.m_iScale > bcdArg2.m_iScale) {                     \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          _RESCALE_BCD(eErrorCode)                                         \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
        if(eErrorCode != EC_NONE) {                                        \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        eErrorCode = _OF_BCDADD;                                           \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          bcdArg1.m_iMantissa = bcdArg1.m_iMantissa + bcdArg2.m_iMantissa; \
          *paW++ = bcdArg1.m_iAsI8;                                        \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

#define _DIV_BCDBCD(eErrorCode)                                            \
  NEVER_REACH;

#define _MUL_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
      eErrorCode = _OF_BCDMUL;                                             \
      if(eErrorCode != EC_NONE) {                                          \
          *paW++ = C_DATUM_NULL;                                           \
      }                                                                    \
      else {                                                               \
        i8 max_col_decimal = TMP_TBLCOL_SCALE;                             \
        bcdArg1.m_iMantissa = bcdArg1.m_iMantissa * bcdArg2.m_iMantissa;   \
        i8 tmpScale = bcdArg1.m_iScale + bcdArg2.m_iScale;                 \
        if(tmpScale > max_col_decimal) {                                   \
          bcdArg1.m_iMantissa /= iaPow10[tmpScale - max_col_decimal - 1];  \
          if(bcdArg1.m_iMantissa % 10 >= 5) {                              \
            bcdArg1.m_iMantissa = (bcdArg1.m_iMantissa / 10) + 1;          \
          }                                                                \
          else {                                                           \
            bcdArg1.m_iMantissa = bcdArg1.m_iMantissa / 10;                \
          }                                                                \
          bcdArg1.m_iScale = max_col_decimal;                              \
        }                                                                  \
        else {                                                             \
          bcdArg1.m_iScale = tmpScale;                                     \
        }                                                                  \
        *paW++ = bcdArg1.m_iAsI8;                                          \
      }                                                                    \
    }                                                                      \
  }

#define _SUB_BCDBCD(eErrorCode) \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE ||                                           \
        bcdArg1.m_iAsI8 == C_DATUM_NULL ||                                 \
        bcdArg2.m_iAsI8 == C_DATUM_NULL) {                                 \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else {                                                                 \
        if(bcdArg1.m_iScale < bcdArg2.m_iScale) {                          \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          _RESCALE_BCD(eErrorCode)                                         \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
        else if(bcdArg1.m_iScale > bcdArg2.m_iScale) {                     \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          _RESCALE_BCD(eErrorCode)                                         \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
        if(eErrorCode != EC_NONE) {                                        \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        eErrorCode = _OF_BCDSUB;                                           \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          bcdArg1.m_iMantissa = bcdArg1.m_iMantissa - bcdArg2.m_iMantissa; \
          *paW++ = bcdArg1.m_iAsI8;                                        \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

#define _SUM_BCDBCD(eErrorCode)                                            \
  {                                                                        \
    bcdArg2.m_iAsI8 = *(--paW);                                            \
    bcdArg1.m_iAsI8 = *(--paW);                                            \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (bcdArg1.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = bcdArg2.m_iAsI8;                                            \
    }                                                                      \
    else if (bcdArg2.m_iAsI8 == C_DATUM_NULL) {                            \
      *paW++ = bcdArg1.m_iAsI8;                                            \
    }                                                                      \
    else {                                                                 \
        if(bcdArg1.m_iScale < bcdArg2.m_iScale) {                          \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          _RESCALE_BCD(eErrorCode)                                         \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
        else if(bcdArg1.m_iScale > bcdArg2.m_iScale) {                     \
          *paW++ = bcdArg1.m_iAsI8;                                        \
          *paW++ = bcdArg2.m_iAsI8;                                        \
          _RESCALE_BCD(eErrorCode)                                         \
          bcdArg2.m_iAsI8 = *(--paW);                                      \
          bcdArg1.m_iAsI8 = *(--paW);                                      \
        }                                                                  \
        if(eErrorCode != EC_NONE) {                                        \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
        eErrorCode = _OF_BCDADD;                                           \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          bcdArg1.m_iMantissa = bcdArg1.m_iMantissa + bcdArg2.m_iMantissa; \
          *paW++ = bcdArg1.m_iAsI8;                                        \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// FLOAT
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



#define _ADD_FLTFLT(eErrorCode) \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    eErrorCode = EC_NONE;                                                \
    if(eErrorCode != EC_NONE)                                            \
    {                                                                    \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else { \
      if(fltArg1.m_dFlt >= C_FLT_MAX ||                                  \
        fltArg2.m_dFlt >= C_FLT_MAX) {  \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else if(fltArg1.m_dFlt <  (d8)-C_FLT_MAX ||                          \
        fltArg2.m_dFlt < (d8)-C_FLT_MAX) {                                 \
          eErrorCode = EC_ARITHMETIC_UNDERFLOW;                            \
          *paW++ = C_DATUM_NULL;                                           \
      }                                                                    \
      else                                                                 \
      {                                                                    \
        fltArg1.m_dFlt += fltArg2.m_dFlt;                                  \
        if(fltArg1.m_dFlt > C_FLT_MAX) {                                   \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
          *paW++ = C_DATUM_NULL;                        \
        }                        \
        else                        \
          *paW++ = fltArg1.m_iAsI8;                                          \
      }                                                                    \
    }                                                                      \
  }                                                                        \
}


#define _SUM_FLTFLT(eErrorCode)                                          \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE) {                                           \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else if (fltArg1.m_iAsI8 == C_DATUM_NULL) {                            \
    *paW++ = fltArg2.m_iAsI8;                                            \
  }                                                                      \
  else if (fltArg2.m_iAsI8 == C_DATUM_NULL) {                            \
     *paW++ = fltArg1.m_iAsI8;                                           \
  }                                                                      \
  else {                                                                 \
    if (fltArg1.m_dFlt >= C_FLT_MAX ||                                   \
        fltArg2.m_dFlt >= C_FLT_MAX) {                                   \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
      printf("OVERFLOW\n"); \
      *paW++ = C_DATUM_NULL;                                             \
    }                                                                    \
    else if (fltArg1.m_dFlt <  (d8)C_FLT_MIN ||                          \
      fltArg2.m_dFlt < (d8)C_FLT_MIN) {                                  \
      eErrorCode = EC_ARITHMETIC_UNDERFLOW;                              \
      *paW++ = C_DATUM_NULL;                                             \
    printf("%g %g %g UNDERFLOW\n", fltArg1.m_dFlt, fltArg2.m_dFlt, C_FLT_MIN); \
    }                                                                    \
    else {                                                               \
      fltArg1.m_dFlt += fltArg2.m_dFlt;                                  \
      if(fltArg1.m_dFlt > C_FLT_MAX) {                                   \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
        *paW++ = C_DATUM_NULL;                                           \
      }                                                                  \
      else                                                               \
        *paW++ = fltArg1.m_iAsI8;                                        \
    }                                                                    \
  }                                                                      \
}


#define _DIV_FLTFLT(eErrorCode)                                            \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                                      \
  fltArg1.m_iAsI8 = *(--paW);                                                      \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                           \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                           \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    if (fltArg2.m_iAsI8 == 0) {                                          \
      *paW++ = C_DATUM_NULL;                                             \
      eErrorCode = EC_DIVIDE_BY_ZERO;                                    \
    }                                                                    \
    else {                                                               \
      if(fltArg1.m_dFlt > C_FLT_MAX ||                                   \
        fltArg2.m_dFlt > C_FLT_MAX) {  \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                             \
        *paW++ = C_DATUM_NULL;                                           \
      }                                                                  \
      else if(fltArg1.m_dFlt <  C_FLT_MIN ||                             \
        fltArg2.m_dFlt < C_FLT_MIN) {                                    \
          eErrorCode = EC_ARITHMETIC_UNDERFLOW;                          \
          *paW++ = C_DATUM_NULL;                                         \
      }                                                                  \
      else {                                                             \
        *paW++ = fltArg1.m_iAsI8 / fltArg2.m_iAsI8;                      \
      }                                                                  \
    }                                                                    \
  }                                                                      \
}


#define _SUB_FLTFLT(eErrorCode)                                          \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                            \
  fltArg1.m_iAsI8 = *(--paW);                                            \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                   \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                   \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
      if (fltArg2.m_iAsI8 == 0) {                                        \
        *paW++ = C_DATUM_NULL;                                           \
        eErrorCode = EC_DIVIDE_BY_ZERO;                                  \
      }                                                                  \
      else {                                                             \
        if(fltArg1.m_dFlt > C_FLT_MAX ||                                 \
          fltArg2.m_dFlt > C_FLT_MAX) {  \
          eErrorCode = EC_ARITHMETIC_OVERFLOW;                           \
          *paW++ = C_DATUM_NULL;                                         \
      }                                                                  \
      else if(fltArg1.m_dFlt <  C_FLT_MIN ||                             \
      fltArg2.m_dFlt < C_FLT_MIN) {                                      \
        eErrorCode = EC_ARITHMETIC_UNDERFLOW;                            \
        *paW++ = C_DATUM_NULL;                                           \
      }                                       \
      else {                                                             \
        fltArg1.m_dFlt -= fltArg2.m_dFlt;                                                                   \
        *paW++ = fltArg1.m_iAsI8;                      \
      }                                                                  \
    }                                                                    \
  }                                                                      \
}



#define _MUL_FLTFLT(eErrorCode) \
{                                                                        \
  fltArg2.m_iAsI8 = *(--paW);                                                      \
  fltArg1.m_iAsI8 = *(--paW);                                                      \
  if (eErrorCode != EC_NONE ||                                           \
    fltArg1.m_iAsI8 == C_DATUM_NULL ||                                           \
    fltArg2.m_iAsI8 == C_DATUM_NULL) {                                           \
    *paW++ = C_DATUM_NULL;                                               \
  }                                                                      \
  else {                                                                 \
    if (fltArg2.m_iAsI8 == 0) {                                                    \
        *paW++ = C_DATUM_NULL;                                            \
        eErrorCode = EC_DIVIDE_BY_ZERO;                                   \
    }                                                                    \
    else {                                                               \
      if(fltArg1.m_dFlt > C_FLT_MAX ||                              \
        fltArg2.m_dFlt > C_FLT_MAX) {  \
        eErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else if(fltArg1.m_dFlt <  (d8)-C_FLT_MAX ||                         \
      fltArg2.m_dFlt < (d8)-C_FLT_MAX) { \
        eErrorCode = EC_ARITHMETIC_UNDERFLOW;                              \
        *paW++ = C_DATUM_NULL;                                             \
      }                                       \
      else {                                                             \
        fltArg1.m_dFlt *= fltArg2.m_dFlt;                        \
        if(fltArg1.m_dFlt > C_FLT_MAX) {                                   \
          eErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
          *paW++ = C_DATUM_NULL;                        \
        }                        \
        else                                              \
        if(fltArg1.m_dFlt < C_FLT_MIN) {                                   \
          eErrorCode = EC_ARITHMETIC_OVERFLOW;                               \
          *paW++ = C_DATUM_NULL;                        \
        }                                                \
        else                    \
          *paW++ = fltArg1.m_iAsI8 ;                      \
      }                                                                  \
    }                                                                    \
  }                                                                      \
}


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
      eErrorCode = _OF_I8ADD;                                          \
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

// ignore NULLS
//
#define _SUM_INTINT(eErrorCode)                                            \
  {                                                                        \
    iArg2 = *(--paW);                                                      \
    iArg1 = *(--paW);                                                      \
    if (eErrorCode != EC_NONE) {                                           \
      *paW++ = C_DATUM_NULL;                                               \
    }                                                                      \
    else if (iArg1 == C_DATUM_NULL) {                                      \
      *paW++ = iArg2;                                                      \
    }                                                                      \
    else if (iArg2 == C_DATUM_NULL) {                                      \
      *paW++ = iArg1;                                                      \
    }                                                                      \
    else {                                                                 \
      eErrorCode = _OF_I8ADD;                                              \
      if(eErrorCode != EC_NONE)  {                                         \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else {                                                               \
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
        eErrorCode = _OF_I8DIV;                                        \
        if(eErrorCode != EC_NONE) {                                        \
          *paW++ = C_DATUM_NULL;                                           \
        }                                                                  \
        else {                                                             \
          *paW++ = iArg1 / iArg2;                                          \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  }

//TODO: OVERFLOW/UNDERFLOW
#define _MOD_INTINT(eErrorCode)                                            \
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
        *paW++ = iArg1 % iArg2;                                            \
      }                                                                    \
    }                                                                      \
  }


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
      eErrorCode = _OF_I8MUL;                                          \
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
      eErrorCode = _OF_I8SUB;                                          \
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
