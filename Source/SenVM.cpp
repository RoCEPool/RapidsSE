/*============================================================================*/
/* Forth.cpp - Forth namespace functions                                      */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void SenVM::vCheckAssumptions(void)
{
  
  ASSERT(16 == sizeof(AnyVal));
  RASSERT(sizeof(Dtm) == sizeof(SingleWord));
  
  ASSERT(sizeof(AnyVal)   == sizeof(IntVal));
  ASSERT(sizeof(AnyVal)   == sizeof(FipVal));
  ASSERT(sizeof(AnyVal)   == sizeof(PtrVal));
  //  ASSERT(sizeof(AnyVal) + sizeof(i8)     == sizeof(StrVal));
  
  //  ASSERT(sizeof(StrVal) + sizeof(Footer) + 32    == sizeof(s32));
  //  ASSERT(sizeof(StrVal) + sizeof(Footer) + 256   == sizeof(s256));
  //  ASSERT(sizeof(StrVal) + sizeof(Footer) + C_48K == sizeof(s48K));
  //  ASSERT(sizeof(StrVal) + sizeof(Footer) + C_1M  == sizeof(s1M));

  ASSERT(sizeof(Cell) == 8);
  
  ASSERT(sizeof(Dic) < C_SIZE_SMALL_OBJECT);
  //ASSERT(sizeof(AnyVal) + C_64K + 16     == sizeof(GenVar));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
