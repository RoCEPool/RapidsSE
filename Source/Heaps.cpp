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
  m_pTransientJumbo  = new Heap(CN_JUMBOTRANSIENT,  C_SIZE_JUMBO_OBJECT);
  m_pTransientLarge  = new Heap(CN_LARGETRANSIENT,  C_SIZE_LARGE_OBJECT);
  m_pTransientMedium = new Heap(CN_MEDIUMTRANSIENT, C_SIZE_MEDIUM_OBJECT);
  m_pTransientSmall  = new Heap(CN_SMALLTRANSIENT,  C_SIZE_SMALL_OBJECT);
  
  // set up the jumbo heap
  m_pTransientJumbo->vFixedHeap(true);
  
  Buffer* pJumbo = nullptr;
  while ( (pJumbo = gpBufferPool->pNextJumbo(pJumbo)) ) {
    
    pJumbo->vSetMagic();
    pJumbo->vSetClass(CN_FREECHUNK);
    
    m_pTransientJumbo->vAddFree((FreeChunk*)pJumbo);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
