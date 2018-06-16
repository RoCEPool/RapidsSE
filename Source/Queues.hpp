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

    ThreadVector* m_pThreadVector; // vector of threads in the system
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // tasks

    Atomic m_ziTaskOrderNo;        // task ordering no
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are checked in round robin fashion
  
    TaskQueue* m_pSleepingQueue;   // set of queued sleeping tasks
                                   // (ordered by wakeup time)
  
    TaskQueue* m_pReadyQueue;      // set of queued tasks ready to be executed
                                   // (ordered by arrival)
  
    TaskQueue* m_pReadySlotQueues[C_SLOTS_PER_NODE];
                                   // tasks queued for partitioned execution
                                   // (ordered by arrival)
  
    TaskQueue* m_pReadySendQueues[C_MAX_NODES];
                                   // set of tasks queued for socket sending
                                   // (ordered by arrival)
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are not checked in the round robin
  
    TaskQueue* m_pExecutingQueue;  // queue of tasks currently executing
                                   // (ordered by arrival)
  
    TaskQueue* m_pWaitingQueue;    // suspended tasks which waiting to be signaled
                                   // (ordered by arrival)
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Queues(void);
    ~Queues(void);
  
    void vAddThread(Thread* pThread);
    
    void vAddToExecutingQueue(Task* pTask);
    void vAddToReadyQueue(Task* pTask);
    void vAddToReadySlotQueue(Task* pTask);
    void vAddToSleepingQueue(Task* pTask);
    void vAddToWaitingQueue(Task* pTask);
  
    void vAddToExecutingQueue_NL(Task* pTask);
    void vAddToReadyQueue_NL(Task* pTask);
    void vAddToReadySlotQueue_NL(Task* pTask);
    void vAddToSleepingQueue_NL(Task* pTask);
    void vAddToWaitingQueue_NL(Task* pTask);
  
    i8   nExecutingQueueTasks(void);
    i8   nReadyQueueTasks(void);
    i8   nReadySlotQueueTasks(i8 iSlot);
    i8   nSleepingQueueTasks(void);
    i8   nQWaitingQueueTasks(void);
    
    Thread* pGrabIdleThread(void);

    Task* pInspectReadyQueue(Thread* pCallingThread);
    Task* pInspectReadySlotQueue(Thread* pCallingThread);
    Task* pInspectSleepingQueue(Thread* pCallingThread);
  
    bool bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread);
  
    void vRemoveFromQueue(Task* pTask); //! RAY TO COMPLETE
    void vRemoveThread(Thread* pThread);
  
    void vRemoveFromExecutingQueue(Task* pTask);
    void vRemoveFromWaitingQueue(Task* pTask);
  
    void vScheduleAckedTask(Task* pTask);
    void vScheduleExitedTask(Task* pTask);
    void vScheduleReadyTask(Task* pTask);
    void vScheduleReadySlotTask(Task* pTask);
    void vScheduleSleepingTask(Task* pTask);
    void vScheduleSpawnedTask(Task* pTask);
    void vScheduleWaitingTask(Task* pTask);
    void vScheduleYieldedTask(Task* pTask);
 
    ThreadVector* pThreadVector(void);
    
    Footer* aFooter(void);
  };
  
  Queues* _pQueues(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
