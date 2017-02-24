/*============================================================================*/
/* Values.cpp -- Self-Descriing Values                                        */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Values::vCheckAssumptions(void)
{
  ASSERT(sizeof(Object) == sizeof(Value));
  
  ASSERT(sizeof(Value) + sizeof(i8) + 8 == sizeof(BinVal));
  ASSERT(sizeof(Value) + sizeof(i8)     == sizeof(IntVal));
  ASSERT(sizeof(Value) + sizeof(void*)  == sizeof(IpVal));
  ASSERT(sizeof(Value) + sizeof(i8)     == sizeof(ObjVal));
  ASSERT(sizeof(Value) + sizeof(void*)  == sizeof(PtrVal));
  ASSERT(sizeof(Value) + sizeof(i8)     == sizeof(sRefactor));
  ASSERT(sizeof(Value) + C_8KB          == sizeof(ValReg));
  ASSERT(sizeof(Value) + C_64K + 8      == sizeof(ValWsp));
  
  ASSERT(sizeof(Value) + sizeof(i8)*2 + sizeof(void*) + (C_1K*63)
         ==  sizeof(ValSet));


  ASSERT(24 == sizeof(sRefactor));
  
  ASSERT(sizeof(sRefactor) + sizeof(Footer) + 32    == sizeof(s32));
  ASSERT(sizeof(sRefactor) + sizeof(Footer) + 256   == sizeof(s256));
  ASSERT(sizeof(sRefactor) + sizeof(Footer) + C_48K == sizeof(s48K));
  ASSERT(sizeof(sRefactor) + sizeof(Footer) + C_1M  == sizeof(s1M));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
