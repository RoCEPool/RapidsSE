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
  
    TaskQueue* m_pSleepingQueue;   // queue of sleeping tasks
                                   // (ordered by wakeup time)
                                   // these tasks have been partially executed
  
    TaskQueue* m_pReadyQueue;      // queue of ordinary tasks ready to be
                                   // executed (ordered by arrival)
                                   // these tasks have not been executed
  
    TaskQueue* m_pReadySlotQueues[C_SLOTS_PER_NODE];
                                   // set of tasks queued for partitoned
                                   // execution (ordered by arrival)
                                   // these tasks have not been executed
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // queues which are not checked in the round robin
  
    TaskQueue* m_pExecutingQueue;  // queue of partition tasks currently
                                   // executing (ordered by arrival)
                                   // these tasks have been partially executed
  
    TaskQueue* m_pWaitingQueue;    // set of suspended tasks which are
                                   // waiting to be signaled (ordered by arrival)
                                   // these tasks have been partially executed
  
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
