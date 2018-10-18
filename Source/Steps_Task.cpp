/*============================================================================*/
/* Steps_Task.cpp - task step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// task: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Task(void)
{
  vAddStep("task.ack"_Stz,       &Task::vTaskAck);
  vAddStep("task.acks!"_Stz,     &Task::vTaskAcksStore);
  vAddStep("task::breakPoint"_Stz, &Task::vTaskBreakPoint);
  vAddStep("task.delete"_Stz,    &Task::vTaskDelete);
  vAddStep("task.dicStack"_Stz,  &Task::vTaskDicStack);
  vAddStep("task.dump"_Stz,      &Task::vTaskDump);
  vAddStep("task.dropLock"_Stz,  &Task::vTaskDropLock);
  vAddStep("task.grabLock"_Stz,  &Task::vTaskGrabLock);
  vAddStep("task.job@"_Stz,      &Task::vTaskJobFetch);
  vAddStep("task.job!"_Stz,      &Task::vTaskJobStore);
  vAddStep("task.name@"_Stz,     &Task::vTaskNameFetch);
  vAddStep("task.partition@"_Stz,&Task::vTaskPartitionFetch);
  vAddStep("task.partition!"_Stz,&Task::vTaskPartitionStore);
  vAddStep("task.plan@"_Stz,     &Task::vTaskPlanFetch);
  vAddStep("task.plan!"_Stz,     &Task::vTaskPlanStore);
  vAddStep("task.seed"_Stz,      &Task::vTaskSeed);
  vAddStep("task.session@"_Stz,  &Task::vTaskSessionFetch);
  vAddStep("task.session!"_Stz,  &Task::vTaskSessionStore);
  vAddStep("task.sleep"_Stz,     &Task::vTaskSleep);
  vAddStep("task.send"_Stz,      &Task::vTaskSend);
  vAddStep("task.sendNode@"_Stz, &Task::vTaskSendNodeFetch);
  vAddStep("task.sendNode!"_Stz, &Task::vTaskSendNodeStore);
  vAddStep("task.socket@"_Stz,   &Task::vTaskSocketFetch);
  vAddStep("task.socket!"_Stz,   &Task::vTaskSocketStore);
  vAddStep("task.state@"_Stz,    &Task::vTaskStateFetch);
  vAddStep("task.state!"_Stz,    &Task::vTaskStateStore);
  vAddStep("task::this"_Stz,     &Task::vTaskThis);
  vAddStep("task.thread"_Stz,    &Task::vTaskThread);
  vAddStep("task.wait"_Stz,      &Task::vTaskWait);
  vAddStep("task.yield"_Stz,     &Task::vTaskYield);
}

/*----------------------------------------------------------------------------*/
/* task.ack -- acknowledge that task of this tasks completion                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskAck(MP_Task mpTask)
{
  ENTRY("vTaskAck"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.ack"_Stz);
  Task* pTaskBeingAcked = _pTask(pTaskObject);
  
  Handlers::vAckTaskHandler(pTaskBeingAcked,*mpTask);
      
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_xAcksRemaining                */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksStore(MP_Task mpTask)
{
  ENTRY("vTaskAcksStore"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.acks!"_Stz);
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 nAcksRequired;
  if ( ! mpTask->m_pDataStack->bPopAsInt(nAcksRequired) )
    THROW_INVALID_DSTACK("task.acks!"_Stz);
  
  ASSERT(0 == pGivenTask->m_xAcksRemaining.iPrintOnlyValue());

  RASSERT(pGivenTask->bIsLocked());
  
  pGivenTask->m_xAcksRemaining.vSetRemaining(nAcksRequired);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.breakPoint -- break point so we can examine task & thread state       */
/*----------------------------------------------------------------------------*/
void Task::vTaskBreakPoint(MP_Task mpTask)
{
  ENTRY("vTaskBreakPoint"_Stz);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.delete -- delete task on stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskDelete(MP_Task mpTask)
{
  ENTRY("vTaskDelete"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.delete"_Stz);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  delete pThatTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dicStack -- push task dictionary onto data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskDicStack(MP_Task mpTask)
{
  ENTRY("vTaskDicStack"_Stz);
  
  Object* pPtrTask;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("task.init"_Stz);
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  Object* pPtrDicStack = &pThatTask->m_xPathStack;
  
  mpTask->m_pDataStack->vPushObject(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dropLock -- drop lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskDropLock(MP_Task mpTask)
{
  ENTRY("vTaskDrop"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskDump(MP_Task mpTask)
{
ENTRY("vTaskDump"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.dump"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.grabLock -- grab lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskGrabLock(MP_Task mpTask)
{
  ENTRY("vTaskGrab"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.grabLock"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vGrabLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job@ -- push job of given task                                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskJobFetch(MP_Task mpTask)
{
  ENTRY("vTaskJobFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.job@"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Job* pJob = pThatTask->m_pJob;
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job! -- store job into given task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskJobStore(MP_Task mpTask)
{
  ENTRY("vTaskJobStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.job!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pJobObject;
  if ( ! mpTask->m_pDataStack->bPop(pJobObject) )
    THROW_INVALID_DSTACK("task.job!"_Stz);
  Job* pJob = _pJob(pJobObject);
  
  //pThatTask->m_pJob = pJob;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(MP_Task mpTask)
{
ENTRY("vTaskNameFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name@"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  s256 sName((Stz*)pThatTask->aJobName());
  
  mpTask->m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.partition@ -- fetch partition of task                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskPartitionFetch(MP_Task mpTask)
{
  ENTRY("vTaskPartitionFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  mpTask->m_pDataStack->vPushInt(pThatTask->m_iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.partition! -- store partition of task                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskPartitionStore(MP_Task mpTask)
{
  ENTRY("vTaskPartitionStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.name!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("task.partition!"_Stz);
  
  pThatTask->m_iPartition = iPartition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.plan@ -- fetch plan of task onto data stack                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskPlanFetch(MP_Task mpTask)
{
  ENTRY("vTaskPlanFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.plan@"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  Plan* pPlan = pThatTask->m_pPlan;
  
  Object* pPtrPlan= pPlan;
  
  mpTask->m_pDataStack->vPushObject(pPtrPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.plan! -- store plan on data stack into task on data stack             */
/*----------------------------------------------------------------------------*/
void Task::vTaskPlanStore(MP_Task mpTask)
{
  ENTRY("vTaskPlanStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.plan!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("task.plan!"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pThatTask->m_pPlan = pPlan;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(MP_Task mpTask)
{
ENTRY("vTaskSeed"_Stz);
  
//  //                                   sSeed     nTaskArgs
//  if ( ! mpTask->m_pDataStack->bGuard(CN_STRVAL,CN_INTVAL) )
//    THROW_INVALID_DSTACK("task.seed"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sSeed;
  if ( ! mpTask->m_pDataStack->bPop(sSeed) )
    THROW_INVALID_DSTACK("task.seed"_Stz);


  i8 nTaskArgs;
  if ( ! mpTask->m_pDataStack->bPop(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  //ASSERT(! nTaskArgs);
  
  if ( ! mpTask->m_pDataStack->bGuard(nTaskArgs) )
    THROW_INVALID_DSTACK("task.seed"_Stz);
  
  // copy task arguments to given task (preserving stack order)
  ValWrk xValWrk;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    mpTask->m_pDataStack->vPeekValWrk(iTaskArg,xValWrk);
    pThatTask->m_pDataStack->vPushValWrk(xValWrk);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = mpTask->m_pDataStack->bPop(xValWrk);
    ASSERT(bPopped);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.session@ -- fetch session of task on data stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskSessionFetch(MP_Task mpTask)
{
  ENTRY("vTaskSessionFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.session@"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  Session* pSession = pThatTask->m_pSession;
  
  mpTask->m_pDataStack->vPushObject(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.session! -- store session on data stack into task on data stack       */
/*----------------------------------------------------------------------------*/
void Task::vTaskSessionStore(MP_Task mpTask)
{
  ENTRY("vTaskSessionStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.session!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSession;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrSession) )
    THROW_INVALID_DSTACK("task.session!"_Stz);
  
  Session* pSession = _pSession(pPtrSession);
  ASSERT(! pSession || CN_SESSION == pSession->eClass());
  
  pThatTask->m_pSession = pSession;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.send -- this task sends                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSend(MP_Task mpTask)
{
  ENTRY("vTaskSend"_Stz);
  
  mpTask->vState(TS_SEND);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sendNode@ -- fetch send node of task on data stack                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskSendNodeFetch(MP_Task mpTask)
{
  ENTRY("vTaskSendNodeFetch"_Stz);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("task.sendNode@"_Stz);
  Task* pThatTask = _pTask(pObject);
  
  Node* pSendNode = pThatTask->m_pSendNode;
  
  Object* pPtrSendNode = pSendNode;
  
  mpTask->m_pDataStack->vPushObject(pPtrSendNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sendNode! -- store send node on data stack into task on data stack    */
/*----------------------------------------------------------------------------*/
void Task::vTaskSendNodeStore(MP_Task mpTask)
{
  ENTRY("vTaskSendNodeStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.sendNode!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSendNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSendNode) )
    THROW_INVALID_DSTACK("task.sendNode!"_Stz);
  Node* pSendNode = _pNode(pPtrSendNode);
  
  pThatTask->m_pSendNode = pSendNode;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.sleep -- this task sleeps                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskSleep(MP_Task mpTask)
{
  ENTRY("vTaskSleep"_Stz);
  
  i8 nMicroSeconds;
  
  if ( ! mpTask->m_pDataStack->bPop(nMicroSeconds) )
    THROW_INVALID_DSTACK("sleep_s"_Stz);
  
  mpTask->vSleep(nMicroSeconds);
  mpTask->vState(TS_SLEEPING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket@ -- fetch socket of task on data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketFetch(MP_Task mpTask)
{
  ENTRY("vTaskSocketFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket@"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Socket* pSocket = pThatTask->m_pSocket;
  
  Object* pSocketEmb = static_cast<Object*>(pSocket);
  
  mpTask->m_pDataStack->vPushObject(pSocketEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket! -- store socket on data stack into task                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketStore(MP_Task mpTask)
{
  ENTRY("vTaskSocketStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.socket!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrSocket;
  
  if ( ! mpTask->m_pDataStack->bPop(ptrSocket) )
    THROW_INVALID_DSTACK("task.socket!"_Stz);
  
  Socket* pSocket = static_cast<Socket*>(ptrSocket);
  
  pThatTask->m_pSocket = pSocket;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(MP_Task mpTask)
{
ENTRY("vTaskStateFetch"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state@"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  eTS eState = pThatTask->m_eState;
  
  mpTask->m_pDataStack->vPushInt((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state! -- store state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateStore(MP_Task mpTask)
{
ENTRY("vTaskStateStore"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.state!"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iState;
  
  if ( ! mpTask->m_pDataStack->bPop(iState) )
    THROW_INVALID_DSTACK("task.state!"_Stz);
  
  // a task being set to waiting state must be locked
  if (TS_WAITING_ACK == iState) {
    //RASSERT(pThatTask->bIsLocked());
  }
  
  pThatTask->m_eState = (eTS)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::this -- push this task                                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(MP_Task mpTask)
{
ENTRY("vTaskThis"_Stz);
  
  Object* pTask = *mpTask;
  
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskThread(MP_Task mpTask)
{
  ENTRY("vTaskThread"_Stz);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.thread"_Stz);
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  mpTask->m_pDataStack->vPushObject(pThread);
  ASSERT(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskWait(MP_Task mpTask)
{
  ENTRY("vTaskWait"_Stz);
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.yield -- this task yields                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskYield(MP_Task mpTask)
{
  ENTRY("vTaskYield"_Stz);
  
  mpTask->vState(TS_YIELDED);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
