/*============================================================================*/
/* FunMacros.h -- Function Macros                                             */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BOOLEAN LOGICAL OPEARTORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define FM_AND_BOLBOL(arg1, arg2) \
  (arg1 == C_DATUM_NULL ?         \
    ( ! arg2 ? 0 : C_DATUM_NULL)  \
    : arg2 == C_DATUM_NULL ?      \
    ( ! arg1 ? 0 : C_DATUM_NULL) : arg1 && arg2 )

#define FM_NOT_BOLBOL(arg1) \
  (arg1 == C_DATUM_NULL ? C_DATUM_NULL : ! arg1)

#define FM_OR_BOLBOL(arg1, arg2)  \
  ( arg1 == C_DATUM_NULL ?        \
    ( arg2 == C_DATUM_NULL ? C_DATUM_NULL : \
      arg2 ? 1 : C_DATUM_NULL) \
    : arg2 == C_DATUM_NULL ?     \
     ( arg1 ? 1 : C_DATUM_NULL) : arg1 || arg2)

#define FM_XOR_BOLBOL(arg1, arg2) \
  (arg1 == C_DATUM_NULL || arg2 == C_DATUM_NULL ? 0 : \
    (!arg1 && arg2) || (arg1 && !arg2))


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// INTEGER
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define FM_ADD_INTINT(arg1, arg2) \
  (arg1 == C_DATUM_NULL || arg2 == C_DATUM_NULL ? C_DATUM_NULL : arg1 + arg2)

#define FM_DIV_INTINT(arg1, arg2) \
  (arg1 == C_DATUM_NULL || arg2 == C_DATUM_NULL ? C_DATUM_NULL : arg1 / arg2)

#define FM_MOD_INTINT(arg1, arg2) \
  (arg1 == C_DATUM_NULL || arg2 == C_DATUM_NULL ? C_DATUM_NULL : arg1 % arg2)

#define FM_MUL_INTINT(arg1, arg2) \
  (arg1 == C_DATUM_NULL || arg2 == C_DATUM_NULL ? C_DATUM_NULL : arg1 * arg2)

#define FM_SUB_INTINT(arg1, arg2) \
  (arg1 == C_DATUM_NULL || arg2 == C_DATUM_NULL ? C_DATUM_NULL : arg1 - arg2)



//
// TO BE REWORKED !!!
//
#define FM_DIV_INTBCD(arg1, arg2) \
  (arg1 / arg2)

#define FM_MUL_INTBCD(arg1, arg2) \
  (arg1 * arg2)




#define FM_ADD_BCDBCD(arg1, arg2) \
  (arg1 + arg2)

#define FM_DIV_BCDBCD(arg1, arg2) \
  (arg1 / arg2)

#define FM_MUL_BCDBCD(arg1, arg2) \
  (arg1 * arg2)

#define FM_SUB_BCDBCD(arg1, arg2) \
  (arg1 - arg2)



#define FM_ADD_BCDINT(arg1, arg2) \
  (arg1 + arg2)

#define FM_SUB_BCDINT(arg1, arg2) \
  (arg1 - arg2)


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
