/*============================================================================*/
/* ThreadCursor.cpp -- Cursor over Threads in ThreadList                      */
/*============================================================================*/

#include "Threads.h"

extern Heap* gpSmallHeap;

extern ThreadList* gpThreadList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadCursor::operator new(size_t iSize)
{
  ASSERT(iSize);
  Object* pObject = gpSmallHeap->pAllocObject(CN_THREADCURSOR);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadCursor::operator new(size_t iSize, ThreadCursor* pThreadCursor)
{
  ASSERT(iSize);
  return pThreadCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadCursor::operator delete(void* pThreadAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pThreadAddress);
  
  gpSmallHeap->vFreeObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadCursor::ThreadCursor(List_t<Thread>* pList)
: Object(CN_THREADCURSOR),
  ListCursor_t<Thread>(pList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADCURSOR,_FOOTER_);
  
  Memory::vCheckInitialised(this,sizeof(ThreadCursor));

  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREADCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadCursor::~ThreadCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void ThreadCursor::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ThreadCursor::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/