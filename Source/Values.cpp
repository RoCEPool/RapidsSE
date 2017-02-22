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
  ASSERT(sizeof(Value) + sizeof(i8) + 8 == sizeof(StrVal));
  ASSERT(sizeof(Value) + C_8KB          == sizeof(ValReg));
  ASSERT(sizeof(Value) + C_64K          == sizeof(ValWsp));
  
  ASSERT(sizeof(Value) + sizeof(i8)*2 + sizeof(void*) + (C_1K*63)
         ==  sizeof(ValSet));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
