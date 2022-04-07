/*============================================================================*/
/* LogicalMacros.h -- Logical Macros                                          */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BOOLEAN LOGICAL OPEARTORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _AND_BOLBOL(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
    if (iArg1 == C_DATUM_NULL) {                                           \
      *paW++ = ! iArg2 ? 0 : C_DATUM_NULL;                                 \
    }                                                                      \
    else {                                                                 \
      if (iArg2 == C_DATUM_NULL)  {                                        \
        *paW++ = ! iArg1 ? 0 : C_DATUM_NULL;                               \
      }                                                                    \
      else                                                                 \
        *paW++ = iArg1 && iArg2;                                           \
    }                                                                      \
  }

#define _NOT_BOLBOL(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    if (iArg1 == C_DATUM_NULL)  {                                          \
        *paW++ = C_DATUM_NULL;                                             \
      }                                                                    \
      else                                                                 \
        *paW++ = ! iArg1;                                                  \
  }

#define _OR_BOLBOL(eErrorCode)                                             \
   {                                                                       \
     iArg1 = *(--paW);                                                     \
     iArg2 = *(--paW);                                                     \
     if (iArg1 == C_DATUM_NULL && iArg2 == C_DATUM_NULL) {                 \
       *paW++ = C_DATUM_NULL;                                              \
     }                                                                     \
     else {                                                                \
       if (iArg1 == C_DATUM_NULL) {                                        \
          if (iArg2 == 1)                                                  \
            *paW++ = 1;                                                    \
          else                                                             \
            *paW++ = C_DATUM_NULL;                                         \
       }                                                                   \
       else if (iArg2 == C_DATUM_NULL) {                                   \
          if (iArg1 == 1)                                                  \
            *paW++ = 1;                                                    \
          else                                                             \
            *paW++ = C_DATUM_NULL;                                         \
       }                                                                   \
       else                                                                \
         *paW++ = iArg1 || iArg2;                                          \
     }                                                                     \
  }

#define _XOR_BOLBOL(eErrorCode)                                            \
  {                                                                        \
    iArg1 = *(--paW);                                                      \
    iArg2 = *(--paW);                                                      \
                                                                           \
    if (iArg1 == C_DATUM_NULL || iArg2 == C_DATUM_NULL) {                  \                                         \
       *paW++ = C_DATUM_NULL;                                              \
    }                                                                      \
    else {                                                                 \
      *paW++ =  !iBolValue1 && iBolValue2) ||                              \
          (iBolValue1 && !iBolValue2) ? 1 : 0;                             \
    }                                                                      \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
