/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

#include "Tasking.h"

#include "PsxThd.hpp" //?

Atomic gixTask;

extern pthread_key_t giTLS_TaskKey;

// MARK: Life Cycle

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* Tasking::_pTask(void* aAddr)
{
  Task* pTask = reinterpret_cast<Task*>(aAddr);
  
  RASSERT_TYPE(pTask,CN_TASK);

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* Tasking::_pTask(i8 iAddr)
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
: Focal(CN_TASK,AM::iNextTaskId(),pJob->m_iFocalId)
, m_okLock(LK_TASK,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pProfile     = nullptr;

  m_pJob         = pJob;
  m_pThread      = nullptr;
  
  m_eState       = TS_NONE;
  m_eTransition  = TR_NONE;
  m_eQueue       = QK_NONE;
  m_eEntryPoint  = EP_NONE;

  m_oAcksRmg.vCheckInit();
  // m_oWakeup
  
  m_pForth       = nullptr;
  m_pLogicUnit   = nullptr;

  m_iTraceMask   = C_TRACE_NONE;
  m_eWhatJob     = pJob->eClass();

  m_nOperandYield = 10;

  _checkInitialised_(CN_TASK);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the executing task in job
  
  m_pJob->vTask(this);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //if (pJob->m_bWasReceived)
  //  vDebugPoint();
  
  vIncStepNo(); ++ gixTask;
  AM::vLogTsk(SL_INF,this,TA_TSK_CONSTRUCT,
              "Task -%s%s |gixTask=%lld|"_Stz, //!format-LogTsk
              aCN[m_pJob->eClass()],
              pJob->bWasReceived() ? " was-received"_Stz : ""_Stz,
              * gixTask);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Task::~Task(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  RASSERT(TS_DELETING == m_eState);
  //RASSERT(! bIsLocked());
  RASSERT(! m_pThread);

  //RASSERT(! m_pJob);

  delete m_pForth;
  
  delete m_pLogicUnit;
  
  if (m_pJob) {
    Job* pJob = _pJob(m_pJob);
    delete pJob;
  }
    
  delete m_pProfile;
  m_pProfile = nullptr;
  
  -- gixTask;
  vIncStepNo();
  AM::vLogTsk(SL_INF,this,TA_TSK_DESTRUCT,
              "Task destruct |gixTask=%lld|"_Stz, //!format-LogTsk
              * gixTask);
}

/*----------------------------------------------------------------------------*/
/* this executing / tls task                                                  */
/*----------------------------------------------------------------------------*/
Task* Task::pThisTLSTask(void)
{
  if (C_UNKNOWN == (i8)giTLS_TaskKey)
    return nullptr;
  
  Task* pTask = static_cast<Task*>(PsxThd::pTlsGet(giTLS_TaskKey));
  
  return pTask;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* task loop, execute one step at a time                                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  PsxThd::vTlsSet(giTLS_TaskKey,this);
  ASSERT(PsxThd::pTlsGet(giTLS_TaskKey) == this);

  //RASSERT(! bIsLocked());

  ASSERT(! m_pJob || ! m_pJob->pJobTags()->szValue(C_TAG_PATH)->bSame(""_Stz));
  
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
    StepFp fpStep = * ( (m_pForth->m_aInxPtr ++)->m_afpStep );
    
    // call the intinsic
    try {
      ( ((Step*) (*m_pForth))->*fpStep)();
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
  
  PsxThd::vTlsSet(giTLS_TaskKey,nullptr);
}

/*----------------------------------------------------------------------------*/
/* assert task is structurally correct                                        */
/*----------------------------------------------------------------------------*/
void Task::vAssertStructure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  // must have these
  RASSERT(m_pForth);
  RASSERT(m_pLogicUnit);
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
i8 Task::iAcksOriginal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  i8 iOriginal = m_oAcksRmg.nOriginalAcks();
  
  return iOriginal;
}

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
  
  m_pForth->m_aInxPtr = aAdd<ForthCell*>(m_pForth->m_aInxPtr,nBytes);
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
  vTrace("Task::vCaught(xException) (Node:%s) for job %s\n"_Stz,
         AM::pGrid()->pThisNode()->szNodeNm()->azChar(),
         aCN[m_pJob->eClass()]
         );
  
  xException.vCaught();

  // dump the call stack
  vThreadCallStackDump();
  
  // write activity log
  AM::vWriteLog();
  
  // dump api state
  //AM::vDumpApi();

  vContentDump();
  
  if (AM::aReqTuple_tls()) AM::aReqTuple_tls()->vContentDump();

  AM::vJobContentDump(m_pJob);
  
  m_pLogicUnit->vContentDump();

  m_pForth->vContentDump();
  
  AM::pGrid()->vDsv();

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
  
  return m_pForth->pCompiler();
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
         "m_eState=%s m_eTransition=%s m_eQueue=%s\n"_Stz,
         
         this,
         
         aTS[m_eState]->azChar(),
         aTR[m_eTransition]->azChar(),
         aQK[m_eQueue]->azChar(),

         C_VARGS_END
         );
  
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  _poolAddress_(this);
  vTrace("\n"_Stz);
  vTrace("== Task Dump ================================================\n"_Stz);
  
  vContentDump();
  
  if (AM::aReqTuple_tls())
    AM::aReqTuple_tls()->vContentDump();
  
  AM::vJobContentDump(m_pJob);
  
  if (m_pLogicUnit->m_xpOp.bIsNotNull())
    m_pLogicUnit->m_xpOp->vDump();
  
  m_pLogicUnit->vContentDump();
  
  m_pForth->vContentDump();
  
  vTrace("== End Task Dump ============================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get entry point                                                            */
/*----------------------------------------------------------------------------*/
eEP Task::eEntryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eEntryPoint;
}

/*----------------------------------------------------------------------------*/
/* set entry point                                                            */
/*----------------------------------------------------------------------------*/
void Task::vEntryPoint(eEP eEntryPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eEntryPoint = eEntryPoint;
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
                   m_pLogicUnit->m_eDdlLocked ? " DdlLocked"_Stz : ""_Stz
                   );
  
  vDropTaskLock();
}

/*----------------------------------------------------------------------------*/
/* get forth                                                                  */
/*----------------------------------------------------------------------------*/
Forth* Task::pForth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pForth;
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
  
  m_pProfile = new Profile(m_pJob->m_pOp ? m_pJob->m_pOp->szOpNm() : ""_Stz);
  m_pLogicUnit  = new LogicUnit(this);
  m_pForth   = new Forth(this,m_pLogicUnit);
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

  m_pJob = pJob;
}

/*----------------------------------------------------------------------------*/
/* get the job name of this task                                              */
/*----------------------------------------------------------------------------*/
Stz* Task::szJobName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_pJob)
    return aCN[m_pJob->eClass()];
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
  if (JL_NONE == m_pJob->eJobList())
    sPortrait.vSprint("%s"_Stz,aCN[m_pJob->eClass()]);
  else
  if (JL_ORIGINATING == m_pJob->eJobList())
    sPortrait.vSprint("%s:o"_Stz,aCN[m_pJob->eClass()]);
  else
  if (JL_PARTICIPATING == m_pJob->eJobList())
    sPortrait.vSprint("%s:p"_Stz,aCN[m_pJob->eClass()]);


//  sPortrait.vSprint("%s %p"_Stz,
//                    aCN[m_pJob->eClass()],
//                    m_pJob
//                    );
}

/*----------------------------------------------------------------------------*/
/* get next repeatable random number subscript                                */
/*----------------------------------------------------------------------------*/
i8& Task::i_NextRnd(void)
{
  return m_pLogicUnit->m_i_NextRnd;
}

/*----------------------------------------------------------------------------*/
/* get register set                                                           */
/*----------------------------------------------------------------------------*/
Profile* Task::pProfile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pProfile;
}

/*----------------------------------------------------------------------------*/
/* set register set                                                           */
/*----------------------------------------------------------------------------*/
void Task::vProfile(Profile* pProfile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pProfile = pProfile;
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
/* get register set                                                           */
/*----------------------------------------------------------------------------*/
LogicUnit* Task::pLogicUnit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pLogicUnit;
}

/*----------------------------------------------------------------------------*/
/* render text portrait of task                                               */
/*----------------------------------------------------------------------------*/
void Task::vRenderPortrait(s256& sTaskPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (TS_SLEEPING == m_eState) {
    
    Now sDateTime;
    
    sTaskPortrait.vSprint("Task %p %s %s wakeup %lld"_Stz,
                          this,
                          aCN[m_pJob->eClass()],
                          aTS[m_eState],
                          m_oWakeup.m_wWakeupIvl.m_iAsI8);
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
  
  Dic* pRootDic = AM::pRootDic();
  
  Stz* szSeed = m_pJob->szSeed();
  
  //vTrace("SEED %s\n"_Stz,szSeed);
  Word* pSeedWord = pRootDic->pFindWord(szSeed);
  
  if (! pSeedWord)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Unable to find seed word: %s\n"_Stz,szSeed);
  
  m_pForth->vSeedForth(pSeedWord);
}

/*----------------------------------------------------------------------------*/
/* get send node                                                              */
/*----------------------------------------------------------------------------*/
Grids::Node* Task::pSendNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pLogicUnit->m_pSendNode;
}

/*----------------------------------------------------------------------------*/
/*set send node                                                               */
/*----------------------------------------------------------------------------*/
void Task::vSendNode(Grids::Node* pNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pLogicUnit->m_pSendNode = pNode;
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
    
    AnyVal* aAnyVal = pStack->aPeekVal(iValue);
    
    eCN eClass = aAnyVal->eClass();
    
    if (CN_INTVAL == eClass) {
      sStackContents.vCatSprint("%lld"_Stz,_aIntVal(aAnyVal)->iInt());
    }
    else if (CN_PTRVAL == eClass) {
      if (_aTypedCell(aAnyVal)->pObj()) {
        
        Object* pPtr = _aTypedCell(aAnyVal)->pObj();
        
        eCN ePtrClass = _aTypedCell(aAnyVal)->pObj()->eClass();
        
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
                                  aCN[aAnyVal->eClass()],
                                  _aTypedCell(aAnyVal)->pObj());
    }
    else if (CN_FIPVAL == eClass) {                                      
      sStackContents.vCatSprint("%p"_Stz,
                                _aTypedCell(aAnyVal)->aFip());
    }
    else if (CN_STZVAL == eClass) {
      sStackContents.vCatSprint("\"%lls\""_Stz,
                                _aTypedCell(aAnyVal)->azChar());
    }
    else {
      sStackContents.vCatSprint("<unknown:class=%lld>"_Stz,
                                aAnyVal->eClass());
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
  
  return m_iFocalId;
}

/*----------------------------------------------------------------------------*/
/* get thread                                                                 */
/*----------------------------------------------------------------------------*/
Tasking::Thread* Task::pvThread_DONT_DEREF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pThread;
}

/*----------------------------------------------------------------------------*/
/* set thread                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThread(Tasking::Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pThread = pThread;
}

/*----------------------------------------------------------------------------*/
/* trace current point                                                        */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(Stz* aMark)
{
//  if (AM::nBootNodes() > 1) {
//    i8 iPause = Random::iInteger(999,99999);
//    PsxThd::vSleepUsec(iPause,CN_TASK);
//  }
  
  s256 sRoutine;
  
  ForthCell* aHasCell = m_pForth->m_aInxPtr;
  
  Word* pLocatedWord = m_pForth->m_oDicStack.pLocateRoutineWord(aHasCell);
  if (pLocatedWord)
    sRoutine = pLocatedWord->szWordNm();
  else
    sRoutine = "<unknown>"_Stz;
  
  i8 iAuxValues    = m_pForth->m_pAuxStack->nValues();
  i8 iDataValues   = m_pForth->m_pDataStack->nValues();
  i8 iReturnValues = m_pForth->m_pReturnStack->nValues();
  
  vTrace("[%s(N%d):%8s\\%lld %-11s] %-16s : a%2lld="_Stz,
        
         AM::szNodeNm(),
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
  
  ForthCell* aHasCell = m_pForth->m_aInxPtr;
  Word* pLocatedWord = m_pForth->m_oDicStack.pLocateRoutineWord(aHasCell);
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
/* schedule bide                                                              */
/*----------------------------------------------------------------------------*/
void Task::vScheduleBide(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RASSERT(bIsLocked());
  m_eTransition = TR_BIDE_FOR_OPERAND;
}

/*----------------------------------------------------------------------------*/
/* schedule delete                                                            */
/*----------------------------------------------------------------------------*/
void Task::vScheduleDelete(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RASSERT(bIsLocked());
  m_eTransition = TR_DELETE_THIS_TASK;
}

/*----------------------------------------------------------------------------*/
/* schedule sleep                                                             */
/*----------------------------------------------------------------------------*/
void Task::vScheduleSleep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RASSERT(bIsLocked());
  m_eTransition = TR_SLEEP_SOME_TIME;
}

/*----------------------------------------------------------------------------*/
/* schedule wait                                                              */
/*----------------------------------------------------------------------------*/
void Task::vScheduleWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RASSERT(bIsLocked());
  m_eTransition = TR_WAIT_FOR_RESUME;
}

/*----------------------------------------------------------------------------*/
/* schedule yield                                                             */
/*----------------------------------------------------------------------------*/
void Task::vScheduleYield(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RASSERT(bIsLocked());
  m_eTransition = TR_YIELD_FOR_OTHERS;
}

/*----------------------------------------------------------------------------*/
/* get acheduler action                                                       */
/*----------------------------------------------------------------------------*/
eTR Task::eTranstionRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eTransition;
}

/*----------------------------------------------------------------------------*/
/* clear scheduler action                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTranstionRequestClear()
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
void Task::vValSprint(s48K& sFormat, s48K& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  char* acFmt = sFormat.azChar();
  char* acDst = sString.azChar();
  
  while (*acFmt) {
  
    // test for ordinary character
    if ('%' != *acFmt) {
      *acDst++ = *acFmt++;
      continue;
    }
        
    // we have a '%'
    acFmt++;
    
    // test for the %% case
    if ('%' == *acFmt) {
      *acDst++ = *acFmt++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // we have a %[width]v field
    
    i8 nWidth = 0;

    // record where output for this value began
    char* acFieldBegan = acDst;
    
    // do we have a %v of the form: %widthV ?
    if (StrLib::bIsDigit( * acFmt )) {
      i8 nDigits = 0;
      StrLib::bExtractInteger(&acFmt,&nWidth,nDigits);
      ASSERT(nDigits);
    }
        
    // should be %v or %V
    ASSERT('v' == *acFmt || 'V' == *acFmt);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // handle a StzVal, IntVal or a PtrVal
    
    AnyVal* aAnyVal0 = m_pForth->m_pDataStack->aPeekVal(0);
    eCN eClass = aAnyVal0->eClass();
    
    // test for StzVal
    if (CN_STZVAL == eClass) {
      
      s48K sStr;
      m_pForth->m_pDataStack->vPopStr(sStr,__func__);
      
      if ('V' == *acFmt)
        *acDst++ = '"';
      
      for (char* pv = sStr.azChar(); *pv; )
        *acDst++ = *pv++;
      
      if ('V' == *acFmt)
        *acDst++ = '"';
      
      acFmt++;
    }
    
    // test for IntVal
    else
    if (CN_INTVAL == eClass) {
      
      i8 iInt = m_pForth->m_pDataStack->iPopInt(__func__);
      
      char caValue[32];
      StrLib::wToString(iInt,caValue);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    // test for PtrVal
    else
    if (CN_PTRVAL == eClass) {
      
      Object* pObject = m_pForth->m_pDataStack->pPopPtr(__func__);
      
      char caValue[32];
      StrLib::wToPointer(pObject,caValue);
      
      for (char* pv = caValue; *pv; )
        *acDst++ = *pv++;
      
      acFmt++;
    }
    
    // just copy % ???
    else {
      *acDst++ = '%';
      *acDst++ = *acFmt++;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // handle optional field width case
    
    // test for field width
    if (! nWidth)
      continue;
    
    i8 nDiff = nDifference(acDst,acFieldBegan);
    
    if (nDiff >= nWidth)
      continue;
    
    i8 nPad = nWidth - nDiff;
    StrLib::vPad(acDst,nPad,' ');
    acDst += nPad;
  }
  
  // final termination
  *acDst = '\0';
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
