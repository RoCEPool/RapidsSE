/*============================================================================*/
/* Scheduler.hpp -- Job/Task/Thread Scheduler Mark II                         */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Scheduler
  {
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // self task transition
  
  public:
    static void vTaskLoopTransition(Thread* pThread);
  private:
    static void vTaskSelf_BideForOperand(Task* pTask);
    static void vTaskSelf_DeleteThisTask(Task* pTask);
    static void vTaskSelf_SignalCompletion(Task* pTask);
    static void vTaskSelf_SleepSomeTime(Task* pTask);
    static void vTaskSelf_WaitForResume(Task* pTask);
    static void vTaskSelf_YieldForOthers(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // cross task
  
  public:
    static bool bTaskCross_AckWaiting(Task* pTaskWaiting, Task* pAckingTask);
    static void vTaskCross_TriggerBiding(Task* pTaskBiding, Task* pTriggeringTask);
    static void vTaskCross_WakeSleepers(Task* pWakerTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task api
  
  public:
    static void vTaskApi_AddDedicated(Task* pTask);
    static void vTaskApi_AddGeneral(Task* pTask);
    static void vTaskApi_ExecuteDedicated(Task* pTask, Thread* pThread);
    static void vTaskApi_RequiredAcks(Task* pTask, i8 nAcksRequired);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // job interface
  
  public:
    static void vJob_AddOriginating(Job* pJob);
    static void vJob_AddParticipating(Job* pJob);
    static void vJob_DumpOriginating(void);
    static void vJob_DumpParticipating(void);
    static Job* pJob_FindOriginating(i8 iJobId);
    static Job* pJob_FindParticipating(i8 iJobId);
    static bool bJob_RemoveOriginating(Job* pJob, bool bDump=false);
    static bool bJob_RemoveParticipating(Job* pJob, bool bDump=false);
   
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread interface
  
  public:
    static void vThread_AddDedicatedThread(Thread* pThread);
    static void vThread_AddGeneralThread(Thread* pThread);
    static void vThread_BeginThreadLoop(Thread* pThread);
    static void vThread_EndThreadLoop(Thread* pThread);
    static void vThread_NapTillGeneralThreads(void);
    static void vThread_RemoveDedicatedThread(Thread* pThread);
    static void vThread_RemoveGeneralThread(Thread* pThread);
    static void vThread_WaitForTaskAssignment(Thread* pThread);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation for threads
  
  private:
    static void vThread_AppendIdle(Thread* pThread);
    static bool bThread_AssignIdle(Task* pTask);
    static void vThread_ContinueSame(Thread* pThread, Task* pTask);
    static void vThread_UnassignTask(Thread* pThread);
    static void vThread_WaitAssignment(Thread* pThread);
  
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   // core task scheduler
   
   private:
     static void vScheduleTask(Task* pTask);
  

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
