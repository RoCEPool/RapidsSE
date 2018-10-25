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
  
    //TaskQueue* m_pReadyQueue;      // set of queued tasks ready to be executed
    TaskList*  m_pReadyList;       // 
    // QR_READY_QUEUE              // (ordered by arrival)
  
    //TaskQueue* m_pReadySlotQueues[C_SLOTS_PER_NODE];
    TaskList* m_pReadySlotLists[C_SLOTS_PER_NODE];
    // QR_READY_QUEUE + 1 ..       // tasks queued for partitioned execution
    //   gxArguments.m_nSlots - 1  // (ordered by arrival)
  
    //TaskQueue* m_pReadySendQueues[C_MAX_NODES];
    TaskList* m_pReadySendLists[C_MAX_NODES];
    // gxArguments.m_nSlots ..     // set of tasks queued for socket sending
    //   gxArguments.m_nSlots +    // (ordered by arrival)
    //   gxArguments.m_nNodes - 1
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are not checked in the round robin
  
    //TaskQueue* m_pExecutingQueue;  // queue of tasks currently executing
    TaskList*  m_pExecutingList;   //
                                   // (ordered by arrival)
  
    //TaskQueue* m_pWaitingQueue;    // suspended tasks which waiting to be signaled
    TaskList*  m_pWaitingList;     //
                                   // (ordered by arrival)
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Queues(void);
    ~Queues(void);
  
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
  
    void vDumpExecutingQueue(void);
    void vDumpReadyQueue(void);
    void vDumpReadySendQueues(void);
    void vDumpReadySlotQueues(void);
    void vDumpSleepingQueue(void);
    void vDumpWaitingQueue(void);
    
    i8   nExecutingQueueTasks(void);
    i8   nReadyQueueTasks(void);
    i8   nReadySlotQueueTasks(i8 iSlot);
    i8   nSleepingQueueTasks(void);
    i8   nQWaitingQueueTasks(void);
    
    Thread* pGrabIdleThread(void);

    Task* pInspectReadyQueue(Thread* pCallingThread);
    Task* pInspectReadySendQueue(Thread* pCallingThread);
    Task* pInspectReadySlotQueue(Thread* pCallingThread);
    Task* pInspectSleepingQueue(Thread* pCallingThread);

    bool  bIsReadyQueue(i8 iRRqueue);
    bool  bIsReadySendQueue(i8 iRRqueue);
    bool  bIsReadySlotQueue(i8 iRRqueue);
    bool  bIsSleepingQueue(i8 iRRqueue);
  
    bool  bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread);

    i8    i_QueueFirst(void);
    i8    i_QueueLast(void);
  
    void vRemoveFromQueue(Task* pTask); //! RAY TO COMPLETE
    void vRemoveThread(Thread* pThread);
  
    void vRemoveFromExecutingQueue(Task* pTask);
    void vRemoveFromWaitingQueue(Task* pTask);
  
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
  
    i8   i_ToReadySendQueue(i8 iRRqueue);
    i8   i_ToReadySlotQueue(i8 iRRqueue);
  
    Footer* aFooter(void);
  };
  
  Queues* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
