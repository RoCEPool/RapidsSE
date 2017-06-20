/*============================================================================*/
/* TaskSet.cpp - set of tasks                                                 */
/*============================================================================*/

#include "Tasks.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskSet::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  TaskSet* pTaskSet = (TaskSet*)Heap::pAllocate(CN_TASKSET);
  
  return pTaskSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskSet::operator new(size_t iSize, TaskSet* pTaskSet)
{
  RASSERT(iSize);
  return pTaskSet;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TaskSet::operator delete(void* aAddress)
{
  RASSERT(aAddress);
  vFatal("fatal: TaskSet class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskSet::TaskSet(void)
: Object(CN_TASKSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKSET,_FOOTER_);
  
  m_xLock.vInit("TaskSet::m_xLock"_Str);
  
  m_iCount = 0;
  m_npHead = nullptr;
  m_npTail = nullptr;
  
  _checkInitialised_(CN_TASKSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append task to this sequence                                               */
/*----------------------------------------------------------------------------*/
void TaskSet::vAppend(NNP_Task nnpTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKSET,_FOOTER_);
  
  m_xLock.vGrabWrite();
  
  if ( ! m_npHead) {
    // queue is empty
    ASSERT(0 == m_iCount);
    ASSERT( ! m_npTail );
    
    m_npHead  = *nnpTask;
    m_npTail  = *nnpTask;
    
    nnpTask->vNextInSet(nullptr);
    nnpTask->vPrevInSet(nullptr);
    
    m_iCount = 1;
  }
  else {
    // queue has task's
    ASSERT(m_iCount);
    ASSERT(m_npHead);
    ASSERT(m_npTail);
    
    nnpTask->vNextInSet(nullptr);
    nnpTask->vPrevInSet(m_npTail);
    
    m_npTail->vNextInSet(*nnpTask);
    m_npTail = *nnpTask;

    m_iCount++;
  }
  
  vVerify();
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump this task set                                                         */
/*----------------------------------------------------------------------------*/
void TaskSet::vContentDump(void)
{
  _poolAddress_(this);
  
  m_xLock.vGrabWrite();

  vTrace("TaskSet %p: Count %lld Head %pd Tail %p\n"_Str,
         this,
         m_iCount,
         m_npHead,
         m_npTail
         );
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* pop next tx from this sequence                                             */
/*----------------------------------------------------------------------------*/
NP_Task TaskSet::npPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKSET,_FOOTER_);
  
  NP_Task npTask = nullptr;

  m_xLock.vGrabWrite();
  
  if (m_npHead != nullptr) {
    
    npTask = m_npHead;
    
    m_npHead = m_npHead->npNextInSet();
    
    if (m_npHead != nullptr)
      m_npHead->vPrevInSet(nullptr);
    else
      m_npTail = nullptr;
    
    m_iCount--;
  }
  
  vVerify();
  
  m_xLock.vDropWrite();

  return npTask;
}

/*----------------------------------------------------------------------------*/
/* remove specific tx from this sequence                                      */
/*----------------------------------------------------------------------------*/
void TaskSet::vRemove(NNP_Task nnpTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKSET,_FOOTER_);
  
  m_xLock.vGrabWrite();
  
  NP_Task npTaskNext = nnpTask->npNextInSet();
  NP_Task npTaskPrev = nnpTask->npPrevInSet();
  
  if (npTaskPrev != nullptr)
    npTaskPrev->vNextInSet(npTaskNext);

  if (npTaskNext != nullptr)
    npTaskNext->vPrevInSet(npTaskPrev);

  if (*nnpTask == *m_npHead)
    m_npHead = npTaskNext;

  if (*nnpTask == *m_npTail)
    m_npTail = npTaskPrev;
  
  m_iCount--;
  
  vVerify();
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* verify sequence                                                            */
/*----------------------------------------------------------------------------*/
void TaskSet::vVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKSET,_FOOTER_);
  
  m_xLock.vCheckWrite();
  
  if (m_npHead != nullptr) {
    ASSERT(!m_npHead->npPrevInSet());
  }
  else {
    ASSERT(!m_npTail);
  }

  if (m_npTail != nullptr) {
    ASSERT(!m_npTail->npNextInSet());
  }
  else {
    ASSERT(!m_npHead);
  }
  
  i8 iCount = 0;
  for (NP_Task npTask = m_npHead; npTask != nullptr ; npTask = npTask->npNextInSet()) {
    iCount++;
    if (npTask->npNextInSet() != nullptr) {
      ASSERT(*npTask == *(npTask->npNextInSet()->npPrevInSet()));
    }
    if (npTask->npPrevInSet() != nullptr) {
      ASSERT(*npTask == *(npTask->npPrevInSet()->npNextInSet()));
    }
  }
  
  ASSERT(iCount == m_iCount);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TaskSet::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKSET);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
