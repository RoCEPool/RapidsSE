/*============================================================================*/
/* LogicalMacros.h -- Tcc Macros                                              */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BOOLEAN LOGICAL OPEARTORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define _AND_BOLBOL(arg1, arg2, piErrorCode) \
  (arg1 == C_DATUM_NULL ?         \
    ( ! arg2 ? 0 : C_DATUM_NULL)  \
    : arg2 == C_DATUM_NULL ?      \
    ( ! arg1 ? 0 : C_DATUM_NULL) : arg1 && arg2 )

#define _NOT_BOLBOL(arg1, piErrorCode) \
  (arg1 == C_DATUM_NULL ? C_DATUM_NULL : ! arg1)

#define _OR_BOLBOL(arg1, arg2, piErrorCode)  \
  ( arg1 == C_DATUM_NULL ?        \
    ( arg2 == C_DATUM_NULL ? C_DATUM_NULL : \
      arg2 ? 1 : C_DATUM_NULL) \
    : arg2 == C_DATUM_NULL ?     \
     ( arg1 ? 1 : C_DATUM_NULL) : arg1 || arg2)

#define _XOR_BOLBOL(arg1, arg2, piErrorCode) \
  (arg1 == C_DATUM_NULL || arg2 == C_DATUM_NULL ? 0 : \
    (!arg1 && arg2) || (arg1 && !arg2))

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
