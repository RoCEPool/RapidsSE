/*============================================================================*/
/* Schedule.hpp -- Collection of the Systems' Schedule                            */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Schedule : public Object
  {
  friend Scheduler;
  
  public:
  
    Lock m_okLock;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // threads

    Atomic       m_ixThreads;

    ThreadVec*   m_pDedicatedThreadVec; // vector of dedicated threads
    ThreadVec*   m_pGeneralThreadVec;   // vector of general threads
  
    ThreadQueue* m_pIdleThreadQueue;    // queue of idle threads
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task queues
  
    TaskList*  m_pReadyQueue;      // queue of tasks ready to be executed
    TaskList*  m_pReadyQueueLP;    // queue of low priority tasks ready to exe
    TaskQueue* m_pSleepingQueue;   // queue of sleeping tasks order by wakeup

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task sets
  
    TaskList*  m_pBidingSet;       // set of op tasks biding to be triggered
    TaskList*  m_pExecutingSet;    // set of tasks currently executing
    TaskList*  m_pWaitingSet;      // set of suspended tasks waiting to be signaled
    TaskList*  m_pRetiredSet;      // set of retired tasks waiting to be deleted
                                   //   (this may go, adding initially for
                                   //    consistency)

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Schedule(void);
   ~Schedule(void) noexcept(false);
  
    static void vThreadsReport(void);
  
    void vOutline(s4K& sOutline);

  // - - - - - - - - - - - - - - - - - - - - - - -
  // task
  private:
    void vAddBidingSet(Task* pTask);
    void vAddExecutingSet(Task* pTask);
    void vAddRetiredSet(Task* pTask);
    void vAddSleepingQueue(Task* pTask);
    void vAddWaitingSet(Task* pTask);

    void vAppendReadyQueue(Task* pTask);
    void vAppendReadyQueueLP(Task* pTask);
  
    Task* pPopReadyQueue(void);
    Task* pPopReadyQueueLP(void);
    Task* pPopSleepingQueue(void);
  
    void vRemoveBidingSet(Task* pTask);
    void vRemoveExecutingSet(Task* pTask);
    void vRemoveRetiredSet(Task* pTask);
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
  
  public:
    ThreadVec* pDedicatedThreadVec(void);
    ThreadVec* pGeneralThreadVec(void);

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
    
    i8   nTasksBiding(void);
    i8   nTasksExecuting(void);
    i8   nTasksReady(void);
    i8   nTasksReadyLP(void);
    i8   nTasksSleeping(void);
    i8   nTasksWaiting(void);
    
    Footer* aFooter(void);
  };
  
  Schedule* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
