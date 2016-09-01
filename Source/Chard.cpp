/*============================================================================*/
/* Chard.cpp -- char input stream (with comments removed)                     */
/*============================================================================*/

#include "Common.h"
#include "StorageEngine.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Chard::operator new(size_t, bool bMalloc)
{
  Chard* pChard;
  
  if (bMalloc)
    pChard = reinterpret_cast<Chard*>(malloc(sizeof(Chard)));
  else
    pChard = reinterpret_cast<Chard*>(gpSmallHeap->pAllocObject());
  
  return pChard;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Chard::operator delete(void* pChardAddress)
{
  Chard* pChard = reinterpret_cast<Chard*>(pChardAddress);
  
  if (pChard->m_bMalloc)
    free(pChard);
  else
    gpSmallHeap->vFreeObject((Object*)pChard);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Chard::Chard(void)
: cn(CN_DIR)
{
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Chard::~Chard(void)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/