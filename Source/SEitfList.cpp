/*============================================================================*/
/* SEitfList.cpp - SEitf List                                                 */
/*============================================================================*/

#include "Interface.h"

extern Heap*        gpSmallHeap;
extern SEitfList*   gpSEitfList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitfList::operator new(size_t iSize, SEitfList* pSEitfList)
{
  return pSEitfList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEitfList::operator delete(void* pBufferAddress)
{
  vFatal("fatal: SEitfList class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEitfList::SEitfList(void)
: Object(CN_SEITFLIST)
{
  _poolAddress_(this);
  
  ASSERT( ! gpSEitfList );
  
  gpSEitfList = this;
  
  Memory::vCheckInitialised(this,sizeof(SEitfList));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitfList::~SEitfList(void)
{
  _poolAddress_(this);
}

/*----------------------------------------------------------------------------*/
/* allocate a seitf                                                           */
/*----------------------------------------------------------------------------*/
SEitf* SEitfList::pAllocSEitf(void)
{
  _poolAddress_(this);

  SEitf* pSEitf = reinterpret_cast<SEitf*>(gpSmallHeap->pAllocObject());
  
  vAppend(pSEitf);
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* free a seitf                                                               */
/*----------------------------------------------------------------------------*/
void SEitfList::vFreeSEitf(SEitf* pSEitf)
{
  _poolAddress_(this);
  
  vRemove(pSEitf);
  
  gpSmallHeap->vFreeObject(pSEitf);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/