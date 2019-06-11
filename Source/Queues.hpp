/*============================================================================*/
/* Queues.hpp -- Collection of the Systems' Queues                            */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Queues : public Object
  {
  public:
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // threads

    ThreadVec* m_pThreadVec; // vector of threads in the system
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // tasks

    Atomic m_ixQueueOrder;        // task ordering no
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are checked in round robin fashion
  
    TaskQueue* m_pSleepingQueue;   // set of queued sleeping tasks
    // QR_SLEEPING_QUEUE           // (ordered by wakeup time)
  
    TaskList*  m_pReadyQueue;      //  set of queued tasks ready to be executed
    // QR_READY_QUEUE              // (ordered by arrival)
  
    TaskList*  m_pReadyQueueLP;    //  set of low priority queued tasks ready to run
    
    TaskList* m_pReadySlotQueues[C_MAX_SLOTS];
    // QR_READY_QUEUE + 1 ..       // tasks queued for partitioned execution
    //   gxArguments.m_nSlotsPerNode - 1  // (ordered by arrival)
  
    TaskList* m_pReadySendQueues[C_MAX_NODES];
    // gxArguments.m_nSlotsPerNode ..     // set of tasks queued for socket sending
    //   gxArguments.m_nSlotsPerNode +    // (ordered by arrival)
    //   gxArguments.m_nNodes - 1
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are not checked in the round robin
  
    TaskList*  m_pExecutingSet;    // set of tasks currently executing
                                   // (ordered by arrival)
  
    TaskList*  m_pWaitingSet;      // suspended tasks which waiting to be signaled
                                   // (ordered by arrival)
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Queues(void);
    ~Queues(void) noexcept(false);
  
    static void vThreadsReport(void);
  
    void vAddThread(Thread* pThread);
    
    void vAddToExecutingSet(Task* pTask);
    void vAddToReadyQueue(Task* pTask);
    void vAddToReadyQueueLP(Task* pTask);
    void vAddToReadySlotQueue(Task* pTask);
    void vAddToSendQueue(Task* pTask);
    void vAddToSleepingQueue(Task* pTask);
    void vAddToWaitingSet(Task* pTask);
  
    void vAddToExecutingSet_NL(Task* pTask);
    void vAddToReadyQueue_NL(Task* pTask);
    void vAddToReadyQueueLP_NL(Task* pTask);
    void vAddToReadySlotQueue_NL(Task* pTask);
    void vAddToSendQueue_NL(Task* pTask);
    void vAddToSleepingQueue_NL(Task* pTask);
    void vAddToWaitingSet_NL(Task* pTask);
  
    void vDumpExecutingSet(void);
    void vDumpReadyQueue(void);
    void vDumpReadyQueueLP(void);
    void vDumpReadySendQueues(void);
    void vDumpReadySlotQueues(void);
    void vDumpSleepingQueue(void);
    void vDumpWaitingSet(void);
    
    Thread* pFindIdleThread(void);

    bool bIsReadyQueue(i8 i_QueueRR);
    bool bIsReadyQueueLP(i8 i_QueueRR);
    bool bIsReadySendQueue(i8 i_QueueRR);
    bool bIsReadySlotQueue(i8 i_QueueRR);
    bool bIsSleepingQueue(i8 i_QueueRR);
  
    void vMonitorPortrait(s48K& sPortrait);
    void vMonitorThread(s48K& sPortrait);

    bool bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread);

    Task* pPopReadyQueue(Thread* pCallingThread);
    Task* pPopReadyQueueLP(Thread* pCallingThread);
    Task* pPopReadySendQueue(Thread* pCallingThread);
    Task* pPopReadySlotQueue(Thread* pCallingThread);
    Task* pPopSleepingQueue(Thread* pCallingThread);
  
    i8 i_QueueFirst(void);
    i8 i_QueueLast(void);
  
    void vRemoveThread(Thread* pThread);
  
    void vRemoveFromExecutingSet(Task* pTask);
    void vRemoveFromWaitingSet(Task* pTask);
  
    void vScheduleAckedTask(Task* pTask);
    void vScheduleExitedTask(Task* pTask);
    void vScheduleReadyTask(Task* pTask);
    void vScheduleReadySlotTask(Task* pTask);
    void vScheduleSendTask(Task* pTask);
    void vScheduleSleepingTask(Task* pTask);
    void vScheduleSpawnedTask(Task* pTask);
    void vScheduleWaitingTask(Task* pTask);
    void vScheduleYieldedTask(Task* pTask);
 
    i8 nTasksExecuting(void);
    i8 nTasksReady(void);
    i8 nTasksReadyLP(void);
    i8 nTasksReadySlot(i8 iSlot);
    i8 nTasksSleeping(void);
    i8 nTasksWaiting(void);
  
    i8 i_ToReadySendQueue(i8 i_QueueRR);
    i8 i_ToReadySlotQueue(i8 i_QueueRR);
  
    Footer* aFooter(void);
  };
  
  Queues* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
