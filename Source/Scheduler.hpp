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
    static void vTaskSelf_LoopTransition(Task* pSelfTask, Thread* pThread);
  private:
    static void vTaskSelf_BideForOperand(Task* pSelfTask, Thread* pThread);
    static void vTaskSelf_DeleteThisTask(Task* pSelfTask, Thread* pThread);
    static void vTaskSelf_SignalCompletion(Task* pSelfTask, Thread* pThread);
    static void vTaskSelf_SleepSomeTime(Task* pSelfTask, Thread* pThread);
    static void vTaskSelf_WaitForResume(Task* pSelfTask, Thread* pThread);
    static void vTaskSelf_YieldForOthers(Task* pSelfTask, Thread* pThread);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // cross task
  
  public:
    static bool bTaskCross_AckWaiting(Task* pTaskWaiting, Task* pAckingTask);
    static void vTaskCross_TriggerBiding(Task* pTaskBiding, Task* pTriggeringTask);
    static void vTaskCross_WakeSleepers(Task* pWakerTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task api (pTask is not known to scheduler)
  
  public:
    static void vTaskApi_AddDedicated(Task* pNewTask);
    static void vTaskApi_AddGeneral(Task* pNewTask);
    static void vTaskApi_AddGeneral_connector(Task* pNewTask);
    static void vTaskApi_AddGeneral_internal(Task* pNewTask);
    static void vTaskApi_AddGeneral_prompt(Task* pNewTask);
    static void vTaskApi_AddGeneral_received(Task* pNewTask);
    static void vTaskApi_AddGeneral_2pc(Task* pNewTask);
    static void vTaskApi_BeginExecutingDedicated(Thread* pThread);

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
    static bool bThread_AssignIdle(Task* pKnownTask);
    static void vThread_WaitAssignment(Thread* pThread);
  
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   // core task scheduler
   
   private:
     static void vScheduleTask(Task* pKnownTask);
  

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
