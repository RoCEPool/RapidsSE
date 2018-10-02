/*============================================================================*/
/* Pool.cpp - Buffer Pool functions                                           */
/*============================================================================*/

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Pool::vCheckAssumptions(void)
{
  ASSERT(sizeof(Object) + sizeof(bn)*2 + sizeof(void*) == sizeof(Buffer));
  ASSERT(sizeof(Buffer) + sizeof(void*)*2              == sizeof(HeapBuffer));
  
  Heaps::vValidateTables();
  
#ifdef _DEBUG
  vTrace("C_SIZE_POOL_BUFFER %llx\n"_Stz,C_SIZE_POOL_BUFFER);
  vTrace("C_OBJECT_TO_BUFFER_MASK %llx\n"_Stz,C_OBJECT_TO_BUFFER_MASK);
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
