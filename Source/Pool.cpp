/*============================================================================*/
/* Pool.cpp - Pool namespace functions                                        */
/*============================================================================*/

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Pool::vCheckAssumptions(void)
{
  //ASSERT(sizeof(Object) + sizeof(bn)*2 + sizeof(void*) == sizeof(PoolBuffer));
  //ASSERT(sizeof(PoolBuffer) + sizeof(void*)*2          == sizeof(HeapBuffer));
  
  ASSERT(sizeof(HeapBuffer) == 1 * sizeof(CacheLine));
  
  ASSERT(CacheLine::bMultipleCacheLines(C_SIZE_TINY_OBJECT));
  ASSERT(CacheLine::bMultipleCacheLines(C_SIZE_SMALL_OBJECT));
  ASSERT(CacheLine::bMultipleCacheLines(C_SIZE_MEDIUM_OBJECT));
  ASSERT(CacheLine::bMultipleCacheLines(C_SIZE_LARGE_OBJECT));

  HeapMem::vValidateEnums();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
