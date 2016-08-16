/*============================================================================*/
/* SessionList.cpp - Session List                                             */
/*============================================================================*/

#include "Interface.h"

extern Heap*        gpSmallHeap;
extern SessionList* gpSessionList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SessionList::operator new(size_t iSize, SessionList* pSessionList)
{
  return pSessionList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SessionList::operator delete(void* pBufferAddress)
{
  vFatal("fatal: SessionList class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SessionList::SessionList(void)
: Object(CN_SESSIONLIST)
{
  _poolAddress_(this);
  
  ASSERT( ! gpSessionList );
  
  gpSessionList = this;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SessionList::~SessionList(void)
{
  _poolAddress_(this);
}

/*----------------------------------------------------------------------------*/
/* allocate a seitf                                                           */
/*----------------------------------------------------------------------------*/
SEitf* SessionList::pAllocSEitf(void)
{
  _poolAddress_(this);

  SEitf* pSEitf = reinterpret_cast<SEitf*>(gpSmallHeap->pAllocObject());
  
  vAppend(pSEitf);
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* free a seitf                                                               */
/*----------------------------------------------------------------------------*/
void SessionList::vFreeSEitf(SEitf* pSEitf)
{
  _poolAddress_(this);
  
  vRemove(pSEitf);
  
  gpSmallHeap->vFreeObject(pSEitf);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/