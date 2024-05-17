/*============================================================================*/
/* ThreadList.hpp -- SkipList of Threads                                      */
/*============================================================================*/

#include "BASE.h"
#include "ThreadList.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ThreadList* ThreadList::_pThreadList(void* aAddr)
{
  ThreadList* pThreadList = reinterpret_cast<ThreadList*>(aAddr);
  
  RASSERT_TYPE(pThreadList,CN_THREADLIST);
  
  return pThreadList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ThreadList::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_THREADLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ThreadList::operator delete(void* pAddr)
{
  ThreadList* pThreadList = reinterpret_cast<ThreadList*>(pAddr);
  
  Heaps::vFree(pThreadList);
}

/*----------------------------------------------------------------------------*/
/* Comparator function                                                        */
/*----------------------------------------------------------------------------*/
static bool bCompareFn(void* aSearchEntry, void* pListEntry, bool& bMatch)
{
  Thread* pSearchThread = _pThread(aSearchEntry);
  Thread* pListThread   = _pThread(pListEntry);

  if (!aSearchEntry || !pListEntry) {
    return false;
  }

  RASSERT(pSearchThread->eGetClassNo() == CN_THREAD);
  RASSERT(pListThread->eGetClassNo() == CN_THREAD);

  if (pSearchThread->iThreadNo() > pListThread->iThreadNo()) {
    bMatch = false;
    return true;

  } else if (pSearchThread->iThreadNo() < pListThread->iThreadNo()) {
    bMatch = false;
    return false;

  } else {
    bMatch = true;
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ThreadList::ThreadList(void)
: Object(CN_THREADLIST),
  m_okLock(this)
{
  _precondition_(_MAGIC_,CN_THREADLIST,_FOOTER_);

  this->m_pSkipList = new SkipListNL(true, bCompareFn);

  _checkInitialised_(CN_THREADLIST);
  
  _postcondition_(_MAGIC_,CN_THREADLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ThreadList::~ThreadList(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_THREADLIST,_FOOTER_);
  delete m_pSkipList;
}

/*----------------------------------------------------------------------------*/
/* drop write lock                                                            */
/*----------------------------------------------------------------------------*/
void ThreadList::vDropLock(void)
{
  _precondition_(_MAGIC_,CN_THREADLIST,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab write lock                                                            */
/*----------------------------------------------------------------------------*/
void ThreadList::vGrabLock(void)
{
  _precondition_(_MAGIC_,CN_THREADLIST,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* Iterate the skiplist                                                       */
/*----------------------------------------------------------------------------*/
void ThreadList::vIterate(const std::function<void(Thread* pListObj)>& vfVisitorFn)
{
  _precondition_(_MAGIC_,CN_THREADLIST,_FOOTER_);

  m_okLock.vGrabRead();

  for (SkipNode* pNode = m_pSkipList->pFirst(0);
       pNode;
       pNode = m_pSkipList->pNext(pNode,0)) {
    Thread* pThread = _pThread(pNode->m_pObject);

    vfVisitorFn(pThread);
  }

  m_okLock.vDropRead();
}

/*----------------------------------------------------------------------------*/
/* Insert a thread into the skiplist                                          */
/*----------------------------------------------------------------------------*/
void ThreadList::vInsert(Thread* pThread)
{
  _precondition_(_MAGIC_, CN_THREADLIST, _FOOTER_);
  m_okLock.vGrabWrite();
  bool bResult = m_pSkipList->bInsert(pThread);
  ASSERT(bResult);
  UNUSED(bResult);
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* Remove a thread from the skiplist                                          */
/*----------------------------------------------------------------------------*/
void ThreadList::vRemove(Thread* pThread)
{
  _precondition_(_MAGIC_, CN_THREADLIST, _FOOTER_);
  m_okLock.vGrabWrite();
  bool bResult = m_pSkipList->bRemove(pThread);
  ASSERT(bResult);
  UNUSED(bResult);
  m_okLock.vDropWrite();
}
/*----------------------------------------------------------------------------*/
/* Get a count of items in the skiplist                                       */
/*----------------------------------------------------------------------------*/
i8 ThreadList::nItems()
{
  _precondition_(_MAGIC_, CN_THREADLIST, _FOOTER_);
  i8 iCount;
  m_okLock.vGrabRead();
  iCount = m_pSkipList->nItems();
  m_okLock.vDropRead();

  return iCount;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ThreadList::aFooter(void)
{
  _precondition_(_MAGIC_,CN_THREADLIST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
