/*============================================================================*/
/* LogicalMacros.h -- Logical Macros                                          */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// BOOLEAN LOGICAL OPEARTORS
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static inline void _AND_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));
  if (iArg1 == C_DATUM_NULL) {
    *(*ppaW)++ = ! iArg2 ? 0 : C_DATUM_NULL;
  }
  else {
    if (iArg2 == C_DATUM_NULL)  {
      *(*ppaW)++ = ! iArg1 ? 0 : C_DATUM_NULL;
    }
    else
      *(*ppaW)++ = iArg1 && iArg2;
  }
}

static inline void _NOT_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  if (iArg1 == C_DATUM_NULL)  {
      *(*ppaW)++ = C_DATUM_NULL;
    }
    else
      *(*ppaW)++ = ! iArg1;
}

static inline void _OR_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
   i8 iArg1 = *(--(*ppaW));
   i8 iArg2 = *(--(*ppaW));
   if (iArg1 == C_DATUM_NULL && iArg2 == C_DATUM_NULL) {
     *(*ppaW)++ = C_DATUM_NULL;
   }
   else {
     if (iArg1 == C_DATUM_NULL) {
        if (iArg2 == 1)
          *(*ppaW)++ = 1;
        else
          *(*ppaW)++ = C_DATUM_NULL;
     }
     else if (iArg2 == C_DATUM_NULL) {
        if (iArg1 == 1)
          *(*ppaW)++ = 1;
        else
          *(*ppaW)++ = C_DATUM_NULL;
     }
     else
       *(*ppaW)++ = iArg1 || iArg2;
   }
}

static inline void _XOR_BOLBOL(i8** ppaW, struct Dynamic_T* aDynamicT)
{
  i8 iArg1 = *(--(*ppaW));
  i8 iArg2 = *(--(*ppaW));

  if (iArg1 == C_DATUM_NULL || iArg2 == C_DATUM_NULL) {
     *(*ppaW)++ = C_DATUM_NULL;
  }
  else {
    *(*ppaW)++ =  (!iArg1 && iArg2) ||
        (iArg1 && !iArg2) ? 1 : 0;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
