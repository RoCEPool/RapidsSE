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
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
Session* SessionList::pAllocSession(void)
{
  _poolAddress_(this);

  Session* pSession = reinterpret_cast<Session*>(gpSmallHeap->pAllocObject());
  
  vAppend(pSession);
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
void SessionList::vFreeSession(Session* pSession)
{
  _poolAddress_(this);
  
  vRemove(pSession);
  
  gpSmallHeap->vFreeObject(pSession);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/