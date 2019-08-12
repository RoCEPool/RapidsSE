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
  
    TaskList*  m_pReadyQueue;      // queue of tasks ready to be executed
    TaskList*  m_pReadyQueueLP;    // queue of low priority tasks ready to exe
    TaskQueue* m_pSleepingQueue;   // queue of sleeping tasks

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task sets (ordered by arrival)
  
    Atomic     m_ixTaskArrival;    // task ordering within set
    TaskList*  m_pBidingSet;       // set of op tasks biding to be triggered
    TaskList*  m_pExecutingSet;    // set of tasks currently executing
    TaskList*  m_pPausedSet;       // set of op tasks paused to be continued
    TaskList*  m_pWaitingSet;      // set of suspended tasks waiting to be signaled
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // jobs
  
    JobList*   m_pOriginatingJobs;
    JobList*   m_pParticipatingJobs;

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
    void vAddBidingSet(Task* pTask);
    void vAddExecutingSet(Task* pTask);
    void vAddPausedSet(Task* pTask);
    void vAddWaitingSet(Task* pTask);
    void vAddSleepingQueue(Task* pTask);

    void vAppendReadyQueue(Task* pTask);
    void vAppendReadyQueueLP(Task* pTask);
  
    Task* pPopReadyQueue(void);
    Task* pPopReadyQueueLP(void);
    Task* pPopSleepingQueue(void);
  
    void vRemoveBidingSet(Task* pTask);
    void vRemoveExecutingSet(Task* pTask);
    void vRemovePausedSet(Task* pTask);
    void vRemoveWaitingSet(Task* pTask);
  
  // - - - - - - - - - - - - - - - - - - - - - - -
  // thread
  private:
    void vAddDedicatedThread(Thread* pThread);
    void vAddGeneralThread(Thread* pThread);
    void vThreadAppendIdle(Thread* pThread);
    Thread* pThreadPopIdleNL(void);
    void vRemoveDedicatedThread(Thread* pThread);
    void vRemoveGeneralThread(Thread* pThread);
  
  // - - - - - - - - - - - - - - - - - - - - - - -
  // misc
  public:
    void vDropThreadIdleQueue(void);
    void vDumpBidingSet(void);
    void vDumpExecutingSet(void);
    void vDumpReadyQueue(void);
    void vDumpReadyQueueLP(void);
    void vDumpSleepingQueue(void);
    void vDumpWaitingSet(void);
    void vGrabThreadIdleQueue(void);
  
    void vMonitorPortrait(s48K& sPortrait);
    void vMonitorThread(s48K& sPortrait);
  
    i8   nTasksBiding(void);
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
