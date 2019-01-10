/*============================================================================*/
/* Values.cpp -- Values namespace functions                                   */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Values::vCheckAssumptions(void)
{
  ASSERT(8 == sizeof(Value));
  
  ASSERT(sizeof(Value) + sizeof(i8) + 8 == sizeof(BinVal));
  ASSERT(sizeof(Value) + sizeof(i8)     == sizeof(IntVal));
  ASSERT(sizeof(Value) + sizeof(void*)  == sizeof(IpVal));
  ASSERT(sizeof(Value) + sizeof(void*)  == sizeof(PtrVal));
  ASSERT(sizeof(Value) + sizeof(i8)     == sizeof(StrVal));
  
  ASSERT(sizeof(Value) + sizeof(i8)*2 + sizeof(void*) + (C_1K*63)
         ==  sizeof(ValSet));


  ASSERT(sizeof(Value) + sizeof(WspVbl) == sizeof(StrVal));
  
  ASSERT(sizeof(StrVal) + sizeof(Footer) + 32    == sizeof(s32));
  ASSERT(sizeof(StrVal) + sizeof(Footer) + 256   == sizeof(s256));
  ASSERT(sizeof(StrVal) + sizeof(Footer) + C_48K == sizeof(s48K));
  ASSERT(sizeof(StrVal) + sizeof(Footer) + C_1M  == sizeof(s1M));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
