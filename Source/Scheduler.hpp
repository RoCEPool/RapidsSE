/*============================================================================*/
/* Scheduler.hpp -- Job/Task/Thread Scheduler Mark II                         */
/*============================================================================*/
#pragma once

namespace Threads
{
  class Scheduler
  {
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // task transition
  
  public:
    static void vTaskLoopTransition(Thread* pThread);
  private:
    static void vTaskTransit_BideForOperand(Task* pTask);
    static void vTaskTransit_DeleteThisTask(Task* pTask);
    static void vTaskTransit_SignalCompletion(Task* pTask);
    static void vTaskTransit_SleepSomeTime(Task* pTask);
    static void vTaskTransit_WaitForResume(Task* pTask);
    static void vTaskTransit_YieldForOthers(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // cross task
  
  public:
    static bool bTaskCross_AckWaiting(Task* pTaskWaiting, Task* pTaskAcking);
    static void vTaskCross_TriggerBiding(Task* pTask);
    static void vTaskCross_WakeSleepers(Task* pTask);

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
    static void vAddDedicatedThread(Thread* pThread);
    static void vAddGeneralThread(Thread* pThread);
    static void vBeginThreadLoop(Thread* pThread);
    static void vEndThreadLoop(Thread* pThread);
    static void vPauseTillGeneralThreads(void);
    static void vRemoveDedicatedThread(Thread* pThread);
    static void vRemoveGeneralThread(Thread* pThread);
    static void vWaitForTaskAssignment(Thread* pThread);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation for tasks
  
  private:
    static void vTask_Able(Task* pTask);
    static bool bTask_Ack(Task* pTaskWaiting, Task* pTaskAcking);
    static void vTask_Add(Task* pTask);
    static void vTask_Desist(Task* pTask);
    static void vTask_DdlDropRead(Task* pTask);
    static void vTask_DdlDropWrite(Task* pTask);
    static void vTask_DdlGrabRead(Task* pTask);
    static void vTask_DdlGrabWrite(Task* pTask);
    static void vTask_Execute(Task* pTask);
    static Task* pTask_Pop(Thread* pThread);
    static void vTask_Ready(Task* pTask);
    static void vTask_Remove(Task* pTask);
    static void vTask_Requires(Task* pTask, i8 nAcksRequired);
    static void vTask_Resume(Task* pTask);
    static void vTask_Schedule(Task* pTask);
    static void vTask_Thread(Task* pTask, Thread* pThread);
    static void vTask_Trigger(Task* pTask);
    static void vTask_Wakeup(Task* pTask);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // implementation for threads
  
  private:
    static void vThread_AddDedicated(Thread* pThread);
    static void vThread_AddGeneral(Thread* pThread);
    static void vThread_AppendIdle(Thread* pThread);
    static bool bThread_AssignIdle(Task* pTask);
    static void vThread_BeginLoop(Thread* pThread);
    static void vThread_ContinueSame(Thread* pThread, Task* pTask);
    static void vThread_EndLoop(Thread* pThread);
    static void vThread_ExecuteDedicated(Thread* pThread);
    static void vThread_PauseGeneral(void);
    static void vThread_RemoveDedicated(Thread* pThread);
    static void vThread_RemoveGeneral(Thread* pThread);
    static void vThread_SpawnDedicated(Task* pTask);
    static void vThread_UnassignTask(Thread* pThread);
    static void vThread_WaitAssignment(Thread* pThread);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
