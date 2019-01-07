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

    ThreadVector* m_pThreadVector; // vector of threads in the system
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // tasks

    Atomic m_ziQueueOrder;        // task ordering no
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are checked in round robin fashion
  
    TaskQueue* m_pSleepingQueue;   // set of queued sleeping tasks
    // QR_SLEEPING_QUEUE           // (ordered by wakeup time)
  
    TaskList*  m_pReadyQueue;      //  set of queued tasks ready to be executed
    // QR_READY_QUEUE              // (ordered by arrival)
  
    TaskList* m_pReadySlotQueues[C_SLOTS_PER_NODE];
    // QR_READY_QUEUE + 1 ..       // tasks queued for partitioned execution
    //   gxArguments.m_nSlots - 1  // (ordered by arrival)
  
    TaskList* m_pReadySendQueues[C_MAX_NODES];
    // gxArguments.m_nSlots ..     // set of tasks queued for socket sending
    //   gxArguments.m_nSlots +    // (ordered by arrival)
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
    
    void vAddToExecutingQueue(Task* pTask);
    void vAddToReadyQueue(Task* pTask);
    void vAddToReadySlotQueue(Task* pTask);
    void vAddToSendQueue(Task* pTask);
    void vAddToSleepingQueue(Task* pTask);
    void vAddToWaitingQueue(Task* pTask);
  
    void vAddToExecutingQueue_NL(Task* pTask);
    void vAddToReadyQueue_NL(Task* pTask);
    void vAddToReadySlotQueue_NL(Task* pTask);
    void vAddToSendQueue_NL(Task* pTask);
    void vAddToSleepingQueue_NL(Task* pTask);
    void vAddToWaitingQueue_NL(Task* pTask);
  
    void vDumpExecutingSet(void);
    void vDumpReadyQueue(void);
    void vDumpReadySendQueues(void);
    void vDumpReadySlotQueues(void);
    void vDumpSleepingQueue(void);
    void vDumpWaitingSet(void);
    
    Thread* pFindIdleThread(void);

    bool bIsReadyQueue(i8 iRRqueue);
    bool bIsReadySendQueue(i8 iRRqueue);
    bool bIsReadySlotQueue(i8 iRRqueue);
    bool bIsSleepingQueue(i8 iRRqueue);
  
    bool bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread);

    Task* pPopReadyQueue(Thread* pCallingThread);
    Task* pPopReadySendQueue(Thread* pCallingThread);
    Task* pPopReadySlotQueue(Thread* pCallingThread);
    Task* pPopSleepingQueue(Thread* pCallingThread);
  
    i8 nQueuedExecutingTasks(void);
    i8 nQueuedReadyTasks(void);
    i8 nQueuedReadySlotTasks(i8 iSlot);
    i8 nQueuedSleepingTasks(void);
    i8 nQueuedWaitingTasks(void);
  
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
 
    ThreadVector* pThreadVector(void);
  
    i8 i_ToReadySendQueue(i8 i_RRqueue);
    i8 i_ToReadySlotQueue(i8 i_RRqueue);
  
    Footer* aFooter(void);
  };
  
  Queues* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
