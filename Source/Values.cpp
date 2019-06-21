/*============================================================================*/
/* Values.cpp -- Values namespace functions                                   */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Values::vCheckAssumptions(void)
{
  ASSERT(8 == sizeof(ForthValue));
  RASSERT(sizeof(Dtm) == sizeof(Word));

  ASSERT(sizeof(ForthValue) + sizeof(i8)     == sizeof(IntVal));
  ASSERT(sizeof(ForthValue) + sizeof(void*)  == sizeof(FipVal));
  ASSERT(sizeof(ForthValue) + sizeof(void*)  == sizeof(PtrVal));
//  ASSERT(sizeof(ForthValue) + sizeof(i8)     == sizeof(StrVal));
  
//  ASSERT(sizeof(StrVal) + sizeof(Footer) + 32    == sizeof(s32));
//  ASSERT(sizeof(StrVal) + sizeof(Footer) + 256   == sizeof(s256));
//  ASSERT(sizeof(StrVal) + sizeof(Footer) + C_48K == sizeof(s48K));
//  ASSERT(sizeof(StrVal) + sizeof(Footer) + C_1M  == sizeof(s1M));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
