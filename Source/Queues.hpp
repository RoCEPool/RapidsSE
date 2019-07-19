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
    // task sets (ordered by arrival)
  
    Atomic     m_ixQueueOrder;     // task arrival ordering no
    TaskList*  m_pBidingSet;       // op tasks biding to be triggered
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
    void vAddExecutingSet(Task* pTask);
    void vAddWaitingSet(Task* pTask);
    void vAddSleepingQueue(Task* pTask);

    void vAppendReadyQueue(Task* pTask);
    void vAppendReadyQueueLP(Task* pTask);
  
    Task* pPopReadyQueue(void);
    Task* pPopReadyQueueLP(void);
    Task* pPopSleepingQueue(void);
  
    void vRemoveExecutingSet(Task* pTask);
    void vRemoveWaitingSet(Task* pTask);
  
  // - - - - - - - - - - - - - - - - - - - - - - -
  // thread
  private:
    void vThreadAddDedicated(Thread* pThread);
    void vThreadAddGeneral(Thread* pThread);
    void vThreadAppendIdle(Thread* pThread);
    Thread* pThreadPopIdle(void);
    void vThreadRemoveDedicated(Thread* pThread);
    void vThreadRemoveGeneral(Thread* pThread);
  
  // - - - - - - - - - - - - - - - - - - - - - - -
  // misc
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
