/*============================================================================*/
/* Forth.cpp - Forth namespace functions                                      */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void SenVM::vCheckAssumptions(void)
{
  ASSERT(sizeof(AnyVal) == 16);
  ASSERT(sizeof(AnyVal) == sizeof(IntVal));
  ASSERT(sizeof(AnyVal) == sizeof(FipVal));
  ASSERT(sizeof(AnyVal) == sizeof(PtrVal));

  ASSERT(sizeof(Cell)   == 8);
  
  ASSERT(sizeof(Dic)     < C_SIZE_SMALL_OBJECT);
  
  ASSERT(sizeof(Dtm)    == sizeof(SingleWord));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
