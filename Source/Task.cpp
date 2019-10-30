/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

#include "Tasks.h"
#include "AM.hpp"

Atomic gixTask;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* Tasks::_pTask(void* aAddr)
{
  Task* pTask = reinterpret_cast<Task*>(aAddr);
  
  RASSERT_TYPE(pTask,CN_TASK);

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* Tasks::_pTask(i8 iAddr)
{
  Task* pTask = reinterpret_cast<Task*>(iAddr);
  
  RASSERT_TYPE(pTask,CN_TASK);
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Task::operator new(size_t)
{
  Task* pTask = reinterpret_cast<Task*>(HeapMem::pAllocate(CN_TASK));
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Task::operator delete(void* pTaskAddress)
{
  Task* pTask = reinterpret_cast<Task*>(pTaskAddress);
  
  HeapMem::vFree(pTask);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(Job* pJob)
: Focal(CN_TASK,AM::iNextTaskId(),pJob->m_iUniqueId)
, m_xReqTuple(&pJob->m_xReqTuple)
, m_okLock(LK_TASK,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vInit();
  
  m_pJob      = pJob;

  _checkInitialised_(CN_TASK);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the executing task in job
  
  m_pJob->vTask(this);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //if (pJob->m_bWasReceived)
  //  vDebugPoint();
  
  vIncActNo(); ++ gixTask;
  AM::vLogTsk(SL_INF,this,TA_TSK_CONSTRUCT,
              "Task -%s%s |gixTask=%lld|"_Stz, //!format-LogTsk
              aCN[m_pJob->eClass()],
              pJob->m_bWasReceived ? " was-received"_Stz : ""_Stz,
              * gixTask);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* init task                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pJob         = nullptr;
  m_pOp          = nullptr;

  m_pThread      = nullptr;
  m_pMutex       = new Mutex(MK_TASK,this,&m_xReqTuple);
  
  // m_oDicStack

  m_eState       = TS_NONE;
  m_eTransition  = TR_NONE;
  m_eQueue       = QK_NONE;
  m_oAcksRmg.vCheckInit();
  m_eDdlLocked   = LH_HAVE_NO_LOCK;
  // m_oWakeup
  
  m_iTraceMask     = C_TRACE_NONE;
  
  m_pForth       = nullptr;
  m_pRegSet      = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Task::~Task(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  RASSERT(! m_pThread);

  RASSERT(TS_COMPLETED == m_eState);
  //RASSERT(! m_pJob);
  RASSERT(! m_eDdlLocked);
  RASSERT(! bWordBody());

  if (m_pJob)
    Job::vDeleteJobViaClass(m_pJob);
  
  delete m_pForth;
  delete m_pRegSet;
  
  delete m_pMutex;
  

  
  -- gixTask;
  vIncActNo();
  AM::vLogTsk(SL_INF,this,TA_TSK_DESTRUCT,
              "Task destruct |gixTask=%lld|"_Stz, //!format-LogTsk
              * gixTask);
}

/*----------------------------------------------------------------------------*/
/* this executing / tls task                                                  */
/*----------------------------------------------------------------------------*/
Task* Task::pThisTLSTask(void)
{
  Task* pTask = AM::pThisTLSTask();
  
  return pTask;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* task loop                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vTaskLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
            
  // TODO: shouldn't have to refind it everytime!!!
  vRefindJobClass();
  
  if (! m_pForth)
    vInitExeContext();

  if (! bSeeded())
    vSeedTask();
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAssertStructure();
  
  while (TR_NONE == m_eTransition) {
    
    // UNUSUAL CODE:
    //   get address of member containing intinsic function pointer and
    //   dereference it to get the intrinsic function to be called
    StepFp fpStep = * ( (m_pForth->m_aInxPtr ++)->m_afpIntrinsic );
    
    // call the intinsic
    try {
      ((Step*) (*m_pForth)->*fpStep)();
    }
    
    // catch any exception
    catch (Exception xException) {
      xException.vJob(m_pJob);
      vCaught(xException);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pJob && m_pJob->m_xReqTuple.m_iDetail)
    JobLog::vNote(m_pJob,
                          "Returned from task call: %s %p"_Stz,
                          aCN[m_pJob->eClass()],
                          m_pJob);
}

/*----------------------------------------------------------------------------*/
/* assert task is structurally correct                                        */
/*----------------------------------------------------------------------------*/
void Task::vAssertStructure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  // must have these
  RASSERT(m_pForth);
  RASSERT(m_pJob);
  RASSERT(m_pMutex);
  RASSERT(m_pRegSet);
  
  RASSERT((m_pJob->eClass() == CN_JOB_BOOT && ! m_pThread) || m_pThread);
  // RASSERT((m_pJob->eClass() == CN_JOB_BOOT && ! m_pThread) || false);
  // RASSERT(m_pThread && m_pThread->bStructurallyCorrect());

  // might have these
  // RASSERT(m_pOp      || (m_pOp && m_pOp->bStructurallyCorrect()));
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Task* pTask = _pTask(pObject);
  
  vTrace("%s %p state %s txId %lld\n"_Stz,
         pTask->szJobName(),
         pTask,
         aTS[pTask->m_eState]->azChar()
         );
}

/*----------------------------------------------------------------------------*/
/* monitor profile (for statistics)                                           */
/*----------------------------------------------------------------------------*/
void Task::vMonitorProfile_stats(s48K& sPortrait)
{
  Statistics* pStats = AM::pStatistics();
  
  sPortrait.vSprint("Task: acks %lld executing %lld exited %lld ready %lld "
                          "sleeping %lld waiting %lld\n"_Stz,
                    pStats->Task.m_nAcks,
                    pStats->Task.m_nExecuting,
                    pStats->Task.m_nExited,
                    pStats->Task.m_nReady,
                    pStats->Task.m_nSleeping,
                    pStats->Task.m_nWaiting
                    );
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the number of acks remaining (for assert/print use only)               */
/*----------------------------------------------------------------------------*/
i8 Task::iAcksRmg_assert_only_(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  i8 iRmg = m_oAcksRmg.iPrintOnlyValue();
  
  return iRmg;
}

/*----------------------------------------------------------------------------*/
/* set the number of acks remaining                                           */
/*----------------------------------------------------------------------------*/
void Task::vAcksRmg(i8 nAcksRmg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  ASSERT(bIsTaskLocked());
  
  m_oAcksRmg.vSetRemaining(nAcksRmg);
}

/*----------------------------------------------------------------------------*/
/* decrement acks remaining and return if caller is decider                   */
/*----------------------------------------------------------------------------*/
bool Task::bAcksRmgDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  bool bAmDecider = m_oAcksRmg.bDecAndDecide();
  
  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* adjust forth instruction pointer                                           */
/*----------------------------------------------------------------------------*/
void Task::vAdjustFIP(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pForth->m_aInxPtr = aAdd<Cell*>(m_pForth->m_aInxPtr,nBytes);
}

/*----------------------------------------------------------------------------*/
/* get Object at IP                                                           */
/*----------------------------------------------------------------------------*/
Object* Task::pArgObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Object* pObject = reinterpret_cast<Object*>(m_pForth->m_aInxPtr);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* begin operator functionality                                               */
/*----------------------------------------------------------------------------*/
void Task::vBeginOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* caught an exception in vTaskLoop catch(Exception xException) {...}         */
/*----------------------------------------------------------------------------*/
void Task::vCaught(Exception xException)
{
  vTrace("\n\n"_Stz);
  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  vTrace("Task::vCaught(xException) for job %s\n"_Stz,
         aCN[m_pJob->eClass()]
         );
  
  xException.vCaught();

  // dump the call stack
  vStackDump();
  
  // write activity log
  AM::vWriteLog();
  
  // dump api state
  AM::vDumpApi();

  vContentDump();
  
  m_xReqTuple.vContentDump();

  AM::vJobContentDump(m_pJob);
  
  m_pRegSet->vContentDump();

  m_pForth->vContentDump();

  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pForth->vReset();
  
  m_pForth->m_aInxPtr = m_pForth->m_pExceptionWord->aFirstCell();

  //vAbortProcess();
}

/*----------------------------------------------------------------------------*/
/* debug                                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDebug(void)
{
}

/*----------------------------------------------------------------------------*/
/* get the job of this task                                                   */
/*----------------------------------------------------------------------------*/
Compiler* Task::pCompiler(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pForth->m_pCompiler;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vContentDump(void)
{
  _poolAddress_(this);
  
  vTrace("\n"_Stz);
  vTrace("Task\n"_Stz);
  vTrace("----\n"_Stz);

  vTrace("%p:\n"
         "m_eState=%s m_eTransition=%s m_eQueue=%s m_eDdlLock=%s\n"_Stz,
         
         this,
         
         aTS[m_eState]->azChar(),
         aTR[m_eTransition]->azChar(),
         aQK[m_eQueue]->azChar(),
         aLH[m_eDdlLocked]->azChar(),

         C_ENDARGS
         );
  
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vDropTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Task::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vGrabTaskLock();
  
  sOutline.vSprint("%s %s%s"_Stz,
                                      
                   aTS[m_eState],
                   aQK[m_eQueue],
                   m_eDdlLocked ? " DdlLocked"_Stz : ""_Stz
                   );
  
  vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vGrabTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* initialise the execution context of this task                              */
/*----------------------------------------------------------------------------*/
void Task::vInitExeContext(void)
{
  if (m_pForth)
    return;
  
  m_pForth  = new Forth(this);
  m_pRegSet = new RegSet(this);
  
  m_pForth->m_pCompiler = (CN_JOB_BOOT == m_pJob->eClass()) ? new Compiler : nullptr;

}

/*----------------------------------------------------------------------------*/
/* is task locked?                                                            */
/*----------------------------------------------------------------------------*/
bool Task::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  bool b = m_okLock.bIsLocked();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is task locked?                                                            */
/*----------------------------------------------------------------------------*/
bool Task::bIsTaskLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  bool b = m_okLock.bIsWriteLocked();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get the job of this task                                                   */
/*----------------------------------------------------------------------------*/
Job* Task::pJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pJob;
}

/*----------------------------------------------------------------------------*/
/* set the job of this task                                                   */
/*----------------------------------------------------------------------------*/
void Task::vJob(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (pJob && pJob->eClass() == CN_JOB_PUBLISH) {
    vTrace("Task::vJob: CN_JOB_PUBLISH task %p job %p\n"_Stz,
           this,pJob);
  }

  m_pJob = pJob;
}

/*----------------------------------------------------------------------------*/
/* get the job name of this task                                              */
/*----------------------------------------------------------------------------*/
Stz* Task::szJobName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_pJob && m_pJob->m_aJobClass)
    return aCN[m_pJob->m_aJobClass->m_eClass];
  else if (! m_pJob)
    return "<no job>"_Stz;
  else
    return "<no jobClass>"_Stz;
}

/*----------------------------------------------------------------------------*/
/* pop locals frame                                                           */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  LocalChain* pLocalsframe = m_pForth->m_pLocalsFrame;
  
  m_pForth->m_pLocalsFrame = m_pForth->m_pLocalsFrame->pNext();
  
  delete pLocalsframe;
}

/*----------------------------------------------------------------------------*/
/* push a locals frame                                                        */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPush(void)
{
  LocalChain* pLocalsframe = new LocalChain;
  
  pLocalsframe->vNext(m_pForth->m_pLocalsFrame);
  
  m_pForth->m_pLocalsFrame = pLocalsframe;
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Task::vMonitorPortrait(s48K& sPortrait)
{
  if (JL_NONE == m_pJob->m_eJobList)
    sPortrait.vSprint("%s"_Stz,aCN[m_pJob->eClass()]);
  else
  if (JL_ORIGINATING == m_pJob->m_eJobList)
    sPortrait.vSprint("%s:o"_Stz,aCN[m_pJob->eClass()]);
  else
  if (JL_PARTICIPATING == m_pJob->m_eJobList)
    sPortrait.vSprint("%s:p"_Stz,aCN[m_pJob->eClass()]);


//  sPortrait.vSprint("%s %p"_Stz,
//                    aCN[m_pJob->eClass()],
//                    m_pJob
//                    );
}

/*----------------------------------------------------------------------------*/
/* lock the task mutex                                                        */
/*----------------------------------------------------------------------------*/
void Task::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  m_pMutex->vLock(this);
}

/*----------------------------------------------------------------------------*/
/* signal the task mutex                                                      */
/*----------------------------------------------------------------------------*/
void Task::vMutexSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  m_pMutex->vSignal(this);
}

/*----------------------------------------------------------------------------*/
/* unlock the task mutex                                                      */
/*----------------------------------------------------------------------------*/
void Task::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  m_pMutex->vUnlock(this);
}

/*----------------------------------------------------------------------------*/
/* wait on the task mutex                                                     */
/*----------------------------------------------------------------------------*/
void Task::vMutexWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pMutex->vWait(this);
}

/*----------------------------------------------------------------------------*/
/* get next repeatable random number subscript                                */
/*----------------------------------------------------------------------------*/
i8& Task::i_NextRnd(void)
{
  return m_pRegSet->m_i_NextRnd;
}

/*----------------------------------------------------------------------------*/
/* get the op of this task                                                    */
/*----------------------------------------------------------------------------*/
Op* Task::pOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pOp;
}

/*----------------------------------------------------------------------------*/
/* get task queue                                                             */
/*----------------------------------------------------------------------------*/
eQK Task::eQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eQueue;
}

/*----------------------------------------------------------------------------*/
/* set task queue                                                             */
/*----------------------------------------------------------------------------*/
void Task::vQueue(eQK eQueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eQueue = eQueue;
}

/*----------------------------------------------------------------------------*/
/* set task queue                                                             */
/*----------------------------------------------------------------------------*/
void Task::vRefindJobClass(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  // find the job class on this node
  eCN eClass = m_pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  m_pJob->m_aJobClass = pJobClass;
  
  //m_pJob->m_aJobClass->vJobCat();
}

/*----------------------------------------------------------------------------*/
/* get register set                                                           */
/*----------------------------------------------------------------------------*/
RegSet* Task::pRegSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pRegSet;
}

/*----------------------------------------------------------------------------*/
/* get request tuple                                                          */
/*----------------------------------------------------------------------------*/
ReqTuple* Task::aReqTuple(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return & m_xReqTuple;
}

/*----------------------------------------------------------------------------*/
/* render text portrait of task                                               */
/*----------------------------------------------------------------------------*/
void Task::vRenderPortrait(s256& sTaskPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (TS_SLEEPING == m_eState) {
    
    Now sDateTime;
    
    sTaskPortrait.vSprint("Task %p %s %s wakeup %s"_Stz,
                          this,
                          aCN[m_pJob->eClass()],
                          aTS[m_eState],
                          sDateTime.sz(&m_oWakeup.m_wWakeupTsp)
                        );
  }
  else if (QK_NONE != m_eQueue) {
    sTaskPortrait.vSprint("Task %p %s %s"_Stz,
                          this,
                          aCN[m_pJob->eClass()],
                          aTS[m_eState]);
  }
  else {
    sTaskPortrait.vSprint("Task %p %s %s"_Stz,
                          this,
                          aCN[m_pJob->eClass()],
                          aTS[m_eState]
                          );
  }
}

/*----------------------------------------------------------------------------*/
/* reseed task                                                                */
/*----------------------------------------------------------------------------*/
void Task::vReseed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  m_pForth->vReset();

  vSeedTask();
}

/*----------------------------------------------------------------------------*/
/* retry after exception                                                      */
/*----------------------------------------------------------------------------*/
void Task::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pForth->m_aInxPtr = m_pForth->m_pCompiler->m_pRetryIP;
  m_pForth->m_pCompiler->m_pBeginIP = nullptr;
  
  m_pForth->m_pCompiler->m_pTokeniser->sToken() = ""_Stz;
  
  m_pForth->m_pDataStack->vRetry();
  m_pForth->m_pReturnStack->vRetry();
}

/*----------------------------------------------------------------------------*/
/* is task seeded to run                                                      */
/*----------------------------------------------------------------------------*/
bool Task::bSeeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_pForth && m_pForth->m_aInxPtr)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* seed task                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSeedTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Stz* szSeed = m_pJob->szSeed();
  
  Dic* pRootDic = AM::pRootDic();

  Word* pSeedWord = pRootDic->pFindWord(szSeed);
  
  if (! pSeedWord)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Unable to find seed word: %s\n"_Stz,szSeed);
  
  m_pForth->vSeedForth(pSeedWord);
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::enter                                                   */
/*----------------------------------------------------------------------------*/
Grids::Node* Task::pSendNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pRegSet->m_pSendNode;
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::enter                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSendNode(Grids::Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pRegSet->m_pSendNode = pNode;
}

/*----------------------------------------------------------------------------*/
/* sleep for microseconds                                                     */
/*----------------------------------------------------------------------------*/
void Task::vSleepUsec(i8 iMicroseconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_oWakeup.vSleepFor(iMicroseconds);
}

/*----------------------------------------------------------------------------*/
/* stack contents as string                                                   */
/*----------------------------------------------------------------------------*/
void Task::vStackContents(Stack* pStack, s48K& sStackContents)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  sStackContents = ""_Stz;
  
  i8 iParamValues = pStack->nValues();
  
  sStackContents.vCatSprint("("_Stz);
  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
    
    sStackContents.vCatSprint(" "_Stz);
    
    ForthValue* aValue = pStack->aPeekValue(iValue);
    
    eCN eClass = aValue->eClass();
    
    if (CN_INTVAL == eClass) {
      sStackContents.vCatSprint("%lld"_Stz,_aIntVal(aValue)->iInt());
    }
    else if (CN_PTRVAL == eClass) {
      if (_aPtrVal(aValue)->pObj()) {
        
        Object* pPtr = _aPtrVal(aValue)->pObj();
        
        eCN ePtrClass = _aPtrVal(aValue)->pObj()->eClass();
        
        if (CN_WORD == ePtrClass)
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    ((Word*)pPtr)->szWordNm());
        else if (CN_QUEUE == ePtrClass) {
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    "queue"_Stz);
        }
        else if (CN_TASK == ePtrClass) {
          Task* pTask = _pTask(pPtr);
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    pTask->szJobName());
        }
        else {
          if (pPtr->bValidMagic() && pPtr->eClass() < CN_ENUMS)
            sStackContents.vCatSprint("^%s=%p"_Stz,
                                      aCN[ePtrClass],
                                      pPtr);
          //else
          //  sStackContents.vCatSprint("%p"_Stz,
          //                     pValWrk->pPtr());
        }
      }
      else
        sStackContents.vCatSprint("^%s=%p"_Stz,
                                  aCN[aValue->eClass()],
                                  _aPtrVal(aValue)->pObj());
    }
    else if (CN_FIPVAL == eClass) {                                      
      sStackContents.vCatSprint("%p"_Stz,
                                _aFipVal(aValue)->aFipPtr());
    }
    else if (CN_STRVAL == eClass) {
      sStackContents.vCatSprint("\"%lls\""_Stz,
                                _aStrVal(aValue)->azChar());
    }
    else {
      sStackContents.vCatSprint("<unknown:class=%lld>"_Stz,
                                aValue->eClass());
    }
  }
  
  sStackContents.vCatSprint(" )"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get task state                                                             */
/*----------------------------------------------------------------------------*/
eTS Task::eState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eState;
}

/*----------------------------------------------------------------------------*/
/* set task state                                                             */
/*----------------------------------------------------------------------------*/
void Task::vState(eTS eState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

#ifdef WITH_STATISTICS_COUNTERS
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  
  switch (eState) {
    case TS_EXECUTING: pStats->Task.m_nExecuting++; break;
    case TS_COMPLETED:    pStats->Task.m_nExited++;    break;
    case TS_READY:     pStats->Task.m_nReady++;     break;
    case TS_SLEEPING:  pStats->Task.m_nSleeping++;  break;
    case TS_WAITING:   pStats->Task.m_nWaiting++;   break;
    default:
      NEVER_REACH;
  }
#endif
  
  m_eState = eState;
}

/*----------------------------------------------------------------------------*/
/* get task id                                                                */
/*----------------------------------------------------------------------------*/
i8 Task::iTaskId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_iUniqueId;
}

/*----------------------------------------------------------------------------*/
/* get thread                                                                 */
/*----------------------------------------------------------------------------*/
Threads::Thread* Task::pThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pThread;
}

/*----------------------------------------------------------------------------*/
/* set thread                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThread(Threads::Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pThread = pThread;
}

/*----------------------------------------------------------------------------*/
/* trace current point                                                        */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(Stz* aMark)
{
//  if (AM::nDefinedNodes() > 1) {
//    i8 iPause = Random::iInteger(999,99999);
//    PsxThd::vSleepUsec(iPause,CN_TASK);
//  }
  
  s256 sRoutine;
  
  Cell* aHasCell = m_pForth->m_aInxPtr;
  
  Word* pLocatedWord = m_pForth->m_oDicStack.pLocateWord(aHasCell);
  if (pLocatedWord)
    sRoutine = pLocatedWord->szWordNm();
  else
    sRoutine = "<unknown>"_Stz;
  
  i8 iAuxValues    = m_pForth->m_pAuxStack->nValues();
  i8 iDataValues   = m_pForth->m_pDataStack->nValues();
  i8 iReturnValues = m_pForth->m_pReturnStack->nValues();
  
  vTrace("[%s(N%d):%8s\\%lld %-11s] %-16s : a%2lld="_Stz,
        
         AM::szNodeName(),
         AM::iThisNodeNo(),
         szJobName(),
         m_oAcksRmg.iPrintOnlyValue(),
         
         sRoutine.azChar(),
         aMark,
         iAuxValues);
         
  s48K sStackContents;
  vStackContents(m_pForth->m_pAuxStack, sStackContents);
  
  vTrace("%s"_Stz,sStackContents.azChar());
  
  vTrace(" r%2lld  d%2lld="_Stz, iReturnValues, iDataValues);

  vStackContents(m_pForth->m_pDataStack, sStackContents);
  
  vTrace("%s\n"_Stz,sStackContents.azChar());
}

/*----------------------------------------------------------------------------*/
/* trace step                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTraceStep(Stz* aStepNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  s256 sRoutine;
  
  Cell* aHasCell = m_pForth->m_aInxPtr;
  Word* pLocatedWord = m_pForth->m_oDicStack.pLocateWord(aHasCell);
  if (pLocatedWord)
    sRoutine = pLocatedWord->szWordNm();
  else
    sRoutine = "<unknown>"_Stz;
  
  i8 iDataValues   = m_pForth->m_pDataStack->nValues();
  i8 iReturnValues = m_pForth->m_pReturnStack->nValues();
  vTrace("%-15s %15s : r%lld ; d%2d="_Stz,
         sRoutine.azChar(),
         aStepNm,
         iReturnValues,
         iDataValues);
  
  s48K sStackContents;
  vStackContents(m_pForth->m_pDataStack, sStackContents);
  vTrace("%s\n"_Stz,sStackContents.azChar());
}

/*----------------------------------------------------------------------------*/
/* get transition request                                                     */
/*----------------------------------------------------------------------------*/
eTR Task::eTransition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eTransition;
}

/*----------------------------------------------------------------------------*/
/* set transition request                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTransition(eTR eTransition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eTransition = eTransition;
}

/*----------------------------------------------------------------------------*/
/* clear transition request                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTransitionClear()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eTransition = TR_NONE;
}

/*----------------------------------------------------------------------------*/
/* unlink task and job                                                        */
/*----------------------------------------------------------------------------*/
void Task::vUnlinkJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_pJob) {
    m_pJob->vTask(nullptr);
    m_pJob = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* wakeup the task                                                            */
/*----------------------------------------------------------------------------*/
void Task::vWakeupClear(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_oWakeup.vClear();
}

/*----------------------------------------------------------------------------*/
/* how long before task can be woken up (in microseconds)                     */
/*----------------------------------------------------------------------------*/
u8 Task::uWakeupInUsec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  u8 uInUsec = m_oWakeup.uWakeupInUsec();
  
  return uInUsec;
}

/*----------------------------------------------------------------------------*/
/* can task to be woken?                                                      */
/*----------------------------------------------------------------------------*/
bool Task::bWakeupNow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_oWakeup.bWakeupNow())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get sleep time                                                             */
/*----------------------------------------------------------------------------*/
Sleep& Task::oWakeupSleep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  return m_oWakeup;
}

/*----------------------------------------------------------------------------*/
/* get wakeup time                                                            */
/*----------------------------------------------------------------------------*/
u8 Task::uWakeupTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  u8 uMicroseconds = m_oWakeup.uWakeupInUsec();
  
  return uMicroseconds;
}

/*----------------------------------------------------------------------------*/
/* sprint of stack using given format string                                  */
/*----------------------------------------------------------------------------*/
void Task::vSprint(s48K& sFormat, s48K& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  char* pf = sFormat.azChar();
  char* ps = sString.azChar();
  
  while (*pf) {
    
    // test for ordinary character
    if ('%' != *pf) {
      *ps++ = *pf++;
      continue;
    }
    
    // we have a '%'
    pf++;
    
    // test for %%
    if ('%' == *pf) {
      *ps++ = *pf++;
      continue;
    }
    
    // should be %v or %V
    ASSERT('v' == *pf || 'V' == *pf);
    
    ForthValue* aValue0 = m_pForth->m_pDataStack->aPeekValue(0);
    eCN eClass = aValue0->eClass();
    
    // test for StrVal
    if (CN_STRVAL == eClass) {
      
      //*pf = 's';                                                    //! Defect??
      
      s48K sValue;
      m_pForth->m_pDataStack->bPopS48K(sValue);
      
      if ('V' == *pf)
        *ps++ = '"';
      
      for (char* pv = sValue.azChar(); *pv; )
        *ps++ = *pv++;
      
      if ('V' == *pf)
        *ps++ = '"';
      
      pf++;
      
      continue;
    }
    
    // test for intval
    if (CN_INTVAL == eClass) {
      
      //*pf = 'i';                                                    //! Defect??
      
      i8 iValue;
      m_pForth->m_pDataStack->bPopI8(iValue);
      
      char caValue[32];
      StrLib::wToString(iValue,caValue);
      
      for (char* pv = caValue; *pv; )
        *ps++ = *pv++;
      
      pf++;
      
      continue;
    }
    
    // test for ptrval
    if (CN_PTRVAL == eClass) {
      
      //*pf = 'i';                                                    //! Defect??
      
      Object* pValue;
      m_pForth->m_pDataStack->bPopObject(pValue);
      
      char caValue[32];
      StrLib::wToPointer(pValue,caValue);
      
      for (char* pv = caValue; *pv; )
        *ps++ = *pv++;
      
      pf++;
      
      continue;
    }
    
    // just copy %?
    *ps++ = '%';
    *ps++ = *pf++;
  }
  
  //*pf = '\0';                                                       //! Defect??
  *ps = '\0';
  
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test for correct layout                                                    */
/*----------------------------------------------------------------------------*/
__bCorrectLayout(Task,CN_TASK)

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
__aFooterFunction(Task);


/*----------------------------------------------------------------------------*/
/*-- Compare -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare two tasks by their wakeuptime                                      */
/*----------------------------------------------------------------------------*/
bool bTaskCompareWakeupTime(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  Task* pInsertEntry = _pTask(pObjectInsert);
  Task* pScanEntry   = _pTask(pObjectScan);
  
  RASSERT(pScanEntry->eClass() == CN_TASK);
  RASSERT(pInsertEntry->eClass() == CN_TASK);
  
  if ( ! pObjectScan )
    return false;
  
  if (pInsertEntry->uWakeupTime() == pScanEntry->uWakeupTime())
    bMatch = true;
  else
    bMatch = false;
  
  if (pInsertEntry->uWakeupTime() > pScanEntry->uWakeupTime())
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
