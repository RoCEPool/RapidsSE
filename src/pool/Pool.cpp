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
  
  ALWAYS(sizeof(HeapBuffer) == 1 * sizeof(CacheLine));
  
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_TINY_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_SMALL_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_MEDIUM_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_LARGE_OBJECT));

  HeapMem::vValidateEnums();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
