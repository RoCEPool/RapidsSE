/*============================================================================*/
/* Forth.cpp - Forth namespace functions                                      */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void SenVM::vCheckAssumptions(void)
{
  
  ASSERT(8 == sizeof(StkVal));
  RASSERT(sizeof(Dtm) == sizeof(SingleWord));
  
  ASSERT(sizeof(StkVal) + sizeof(i8)     == sizeof(IntVal));
  ASSERT(sizeof(StkVal) + sizeof(void*)  == sizeof(FipVal));
  ASSERT(sizeof(StkVal) + sizeof(void*)  == sizeof(PtrVal));
  //  ASSERT(sizeof(StkVal) + sizeof(i8)     == sizeof(StrVal));
  
  //  ASSERT(sizeof(StrVal) + sizeof(Footer) + 32    == sizeof(s32));
  //  ASSERT(sizeof(StrVal) + sizeof(Footer) + 256   == sizeof(s256));
  //  ASSERT(sizeof(StrVal) + sizeof(Footer) + C_48K == sizeof(s48K));
  //  ASSERT(sizeof(StrVal) + sizeof(Footer) + C_1M  == sizeof(s1M));

  ASSERT(sizeof(Cell) == 8);
  
  ASSERT(sizeof(Dic) < C_SIZE_SMALL_OBJECT);
  //ASSERT(sizeof(StkVal) + C_64K + 16     == sizeof(GenVar));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
