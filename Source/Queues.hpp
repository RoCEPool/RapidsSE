/*============================================================================*/
/* Queues.hpp -- Collection of the Systems' Queues                            */
/*============================================================================*/
#pragma once

namespace Scheduling
{
  class Queues : public Object
  {
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
  
    void vAddDedicatedThread(Thread* pThread);
    void vAddGeneralThread(Thread* pThread);

    void vAddToExecutingSet(Task* pTask);
    void vAddToReadyQueue(Task* pTask);
    void vAddToReadyQueueLP(Task* pTask);
    void vAddToSleepingQueue(Task* pTask);
    void vAddToWaitingSet(Task* pTask);
  
    void vAddToExecutingSet_NL(Task* pTask);
    void vAddToReadyQueue_NL(Task* pTask);
    void vAddToReadyQueueLP_NL(Task* pTask);
    void vAddToSleepingQueue_NL(Task* pTask);
    void vAddToWaitingSet_NL(Task* pTask);
  
    void vAppendIdleThread(Thread* pThread);

    void vCheckSleeping(void);
  
    void vDumpExecutingSet(void);
    void vDumpReadyQueue(void);
    void vDumpReadyQueueLP(void);
    void vDumpSleepingQueue(void);
    void vDumpWaitingSet(void);
    
    Thread* pFindIdleThread(void);
 
    void vMonitorPortrait(s48K& sPortrait);
    void vMonitorThread(s48K& sPortrait);

    void vPauseForGeneralThreads(void);
  
    bool bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread);
    Thread* pPopIdleThread(void);

    Task* pPopReadyQueue(void);
    Task* pPopReadyQueueLP(void);
    Task* pPopSleepingQueue(void);
   
    void vRemoveDedicatedThread(Thread* pThread);
    void vRemoveGeneralThread(Thread* pThread);
  
    void vRemoveFromExecutingSet(Task* pTask);
    void vRemoveFromWaitingSet(Task* pTask);
  
    void vScheduleExitedTask(Task* pTask);
    void vScheduleReadyTask(Task* pTask);
    void vScheduleSleepingTask(Task* pTask);
    void vScheduleSpawnedTask(Task* pTask);
    void vScheduleWaitingTask(Task* pTask);
    void vScheduleWokenTask(Task* pTask);
    void vScheduleYieldedTask(Task* pTask);
 
    i8 nTasksExecuting(void);
    i8 nTasksReady(void);
    i8 nTasksReadyLP(void);
    i8 nTasksSleeping(void);
    i8 nTasksWaiting(void);
  
    Footer* aFooter(void);
  };
  
  Queues* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
