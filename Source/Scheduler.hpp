/*============================================================================*/
/* Scheduler.hpp -- Thread/Task Event Scheduler                               */
/*============================================================================*/
#pragma once

namespace Scheduling
{
  class Scheduler
  {
  public:
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task
  
    static bool bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vAddTaskDedicated(Task* pTask);
    static void vAddTaskGeneral(Task* pTask);
    static void vCheckSleepingTasks(void);
    static void vScheduleReadyTask(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread
  
    static void vAddThreadDedicated(Thread* pThread);
    static void vAddThreadGeneral(Thread* pThread);
    static void vAssignDedicatedThread(Thread* pThread);
    static void vBeginThread(Thread* pThread);
    static void vEndThread(Thread* pThread);
    static void vPauseTillGeneralThreads(void);
    static void vRemoveDedicatedThread(Thread* pThread);
    static void vRemoveGeneralThread(Thread* pThread);
    static void vReturnTaskLoop(Thread* pThread);
    static void vWaitForTaskAssignment(Thread* pThread);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation
  
  private:
    static bool bAck_WaitingTask(Task* pTaskBeingAcked, Task* pTaskAcking);
    static bool bAssign_IdleThread(Task* pTask);
    static void vDeassign_RunningThread(Thread* pThread);
    static void vDedicate_TaskThread(Task* pTask, Thread* pThread);
    static void vResume_AckedTask(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // support
    static void vAdd_ExecutingSet(Task* pTask);
    static void vAdd_WaitingSet(Task* pTask);
  
    static void vAppend_ReadyQueue(Task* pTask);
    static void vAppend_ReadyQueueLP(Task* pTask);

    static Task* pPop_ReadyQueue(void);
    static Task* pPop_ReadyQueueLP(void);
    static Task* pPop_SleepingQueue(void);

    static void vQueue_ReadyTask(Task* pTask);
  
    static void vRemove_ExecutingSet(Task* pTask);
    static void vRemove_WaitingSet(Task* pTask);

    static void vTask_Exited(Task* pTask);
    static void vTask_Executing(Task* pTask);
    static void vTask_New(Task* pTask);
    static void vTask_Return(Task* pTask);
    static void vTask_Sleeping(Task* pTask);
    static void vTask_Waiting(Task* pTask);
    static void vTask_Yielded(Task* pTask);

    static void vThread_Beginning(Thread* pThread);
    static void vThread_Ending(Thread* pThread);
    static void vThread_Idle(Thread* pThread);
    static void vThread_Running(Thread* pThread);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
