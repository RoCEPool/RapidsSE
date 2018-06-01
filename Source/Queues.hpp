/*============================================================================*/
/* Queues.hpp -- Collection of the Systems' Queues                            */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Queues : public Object
  {
  private:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // threads

    ThreadVector* m_pThreadVector;  // vector of threads in the system
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are checked in round robin fashion
  
    TaskQueue*  m_pSleepingQueue;   // queue of sleeping tasks
                                    // (ordered by wakeup time)
                                    // these tasks have been partially executed
  
    TaskQueue*  m_pGeneralQueue;    // queue of ordinary tasks waiting to be
                                    // executed (ordered by arrival)
                                    // these tasks have not been executed
  
                                    // set of tasks queued for partitoned
                                    // execution (ordered by arrival)
                                    // these tasks have not been executed
    TaskQueue*  m_pSlotQueues[C_SLOTS_PER_NODE];
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are not checked in the round robin
  
    TaskQueue*  m_pBlockedQueue;    // set of tasks which are currently blocked
                                    // (ordered by arrival)
                                    // these tasks have been partially executed
  
    TaskQueue*  m_pSuspendedQueue;  // set of tasks which are currently
                                    // susended (ordered by arrival)
                                    // these tasks have been partially executed
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Queues(void);
    ~Queues(void);
  
    void vAddThread(Thread* pThread);
    bool bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread);
    void vRemoveThread(Thread* pThread);

    ThreadVector* pThreadVector(void);
    
    Footer* aFooter(void);
  };
  
  Queues* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
