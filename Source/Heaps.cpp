/*============================================================================*/
/* Heaps.cpp -- Set of Heaps in Pool                                          */
/*============================================================================*/

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* initialise the various heaps                                               */
/*----------------------------------------------------------------------------*/
void Heaps::vInit(void)
{
  // create the transient heaps
  Transient.m_pJumbo  = new Heap(CN_JUMBOTRANSIENT,  C_SIZE_JUMBO_OBJECT);
  Transient.m_pLarge  = new Heap(CN_LARGETRANSIENT,  C_SIZE_LARGE_OBJECT);
  Transient.m_pMedium = new Heap(CN_MEDIUMTRANSIENT, C_SIZE_MEDIUM_OBJECT);
  Transient.m_pSmall  = new Heap(CN_SMALLTRANSIENT,  C_SIZE_SMALL_OBJECT);
  
  // set up the jumbo heap
  Transient.m_pJumbo->vFixedHeap(true);
  
  Buffer* pJumbo = nullptr;
  while ( (pJumbo = gpBufferPool->pNextJumbo(pJumbo)) ) {
    
    pJumbo->vSetMagic();
    pJumbo->vSetClass(CN_FREECHUNK);
    
    Transient.m_pJumbo->vAddFree((FreeChunk*)pJumbo);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
