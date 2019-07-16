/*============================================================================*/
/* Queues.hpp -- Collection of the Systems' Queues                            */
/*============================================================================*/
#pragma once

namespace Scheduling
{
  class Queues : public Object
  {
  friend Scheduler;
  
  public:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // threads

    ThreadVec*   m_pDedicatedThreadVec; // vector of dedicated threads
    ThreadVec*   m_pGeneralThreadVec;   // vector of general threads
    ThreadQueue* m_pIdleThreadQueue;    // queue of idle threads
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task queues
  
    TaskList*  m_pReadyQueue;      // set of queued tasks ready to be executed
    TaskList*  m_pReadyQueueLP;    // set of low priority queued tasks ready to run
    TaskQueue* m_pSleepingQueue;   // set of queued sleeping tasks

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task sets ordered by arrival
  
    Atomic     m_ixQueueOrder;     // task arrival ordering no
    TaskList*  m_pExecutingSet;    // set of tasks currently executing
    TaskList*  m_pWaitingSet;      // suspended tasks which waiting to be signaled
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Queues(void);
    ~Queues(void) noexcept(false);
  
    static void vThreadsReport(void);
  
  // - - - - - - - - - - - - - - - - - - - - - - -
  // task
  private:
    void vAddToExecutingSet(Task* pTask);
    void vAddToReadyQueue(Task* pTask);
    void vAddToReadyQueueLP(Task* pTask);
    void vAddToSleepingQueue(Task* pTask);
    void vAddToWaitingSet(Task* pTask);
  
    Task* pPopReadyQueue(void);
    Task* pPopReadyQueueLP(void);
    Task* pPopSleepingQueue(void);
  
    void vRemoveFromExecutingSet(Task* pTask);
    void vRemoveFromWaitingSet(Task* pTask);
  
  // - - - - - - - - - - - - - - - - - - - - - - -
  // thread
  private:
    void vAddThreadDedicated(Thread* pThread);
    void vAddThreadGeneral(Thread* pThread);
    Thread* pPopIdleThread(void);
    void vAppendIdleThread(Thread* pThread);
    void vRemoveDedicatedThread(Thread* pThread);
    void vRemoveGeneralThread(Thread* pThread);
  
  // - - - - - - - - - - - - - - - - - - - - - - -
  public:
    void vDumpExecutingSet(void);
    void vDumpReadyQueue(void);
    void vDumpReadyQueueLP(void);
    void vDumpSleepingQueue(void);
    void vDumpWaitingSet(void);
  
    void vMonitorPortrait(s48K& sPortrait);
    void vMonitorThread(s48K& sPortrait);
  
    i8   nTasksExecuting(void);
    i8   nTasksReady(void);
    i8   nTasksReadyLP(void);
    i8   nTasksSleeping(void);
    i8   nTasksWaiting(void);
    
    Footer* aFooter(void);
  };
  
  Queues* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
