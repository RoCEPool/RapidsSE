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
    // task interface
  
    static bool bAckTask(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vAddTaskDedicated(Task* pTask);
    static void vAddTaskGeneral(Task* pTask);
    static void vReturnTaskLoop(Thread* pThread);
    static void vScheduleReadyTask(Task* pTask);
    static void vWakeSleepingTasks(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread interface
  
    static void vAddThreadDedicated(Thread* pThread);
    static void vAddThreadGeneral(Thread* pThread);
    static void vAssignDedicatedThread(Thread* pThread);
    static void vBeginThread(Thread* pThread);
    static void vEndThread(Thread* pThread);
    static void vPauseTillGeneralThreads(void);
    static void vRemoveDedicatedThread(Thread* pThread);
    static void vRemoveGeneralThread(Thread* pThread);
    static void vWaitForTaskAssignment(Thread* pThread);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation
  
  private:
    static bool bAck_WaitingTask(Task* pTaskBeingAcked, Task* pTaskAcking);
    static void vExit_Task(Task* pTask);
    static void vResume_AckedTask(Task* pTask);
    static void vSpawn_NewTask(Task* pTask);
    static void vSleep_Task(Task* pTask);
    static void vWait_Task(Task* pTask);
    static void vWake_SleepingTasks(void);
    static void vYield_Task(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // support for task queues
  
    static void vAdd_ExecutingSet(Task* pTask);
    static void vAdd_WaitingSet(Task* pTask);

    static void vAppend_ReadyQueue(Task* pTask);
    static void vAppend_ReadyQueueLP(Task* pTask);

    static void vInsert_SleepingQueue(Task* pTask);
  
    static Task* pPop_ReadyQueue(void);
    static Task* pPop_ReadyQueueLP(void);
    static Task* pPop_SleepingQueue(void);

    static void vQueue_ReadyTask(Task* pTask);
  
    static void vRemove_ExecutingSet(Task* pTask);
    static void vRemove_WaitingSet(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // support for thread transitions
  
    static void vAdd_DedicatedThread(Thread* pThread);
    static void vAdd_GeneralThread(Thread* pThread);
    static bool bAssign_IdleThread(Task* pTask);
    static void vAppend_IdleQueue(Thread* pThread);
    static void vDeassign_RunningThread(Thread* pThread);
    static void vDedicate_ThreadTask(Thread* pThread, Task* pTask);
  
    static void vThread_Beginning(Thread* pThread);
    static void vThread_Ending(Thread* pThread);
    static void vThread_Idle(Thread* pThread);
    static void vThread_Running(Thread* pThread);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
