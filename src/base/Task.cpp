/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

#include "BASE.h"

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
Task* BASE::_pTask(void* aAddr)
{
  Task* pTask = reinterpret_cast<Task*>(aAddr);
  
  RASSERT_TYPE(pTask,CN_TASK);

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* BASE::_pTask(i8 iAddr)
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
  Task* pTask = reinterpret_cast<Task*>(Heaps::pAllocate(CN_TASK));
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Task::operator delete(void* pTaskAddress)
{
  Task* pTask = reinterpret_cast<Task*>(pTaskAddress);
  
  Heaps::vFree(pTask);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(Job* pJob)
: Loggable(CN_TASK)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RASSERT(pJob);
  
  m_pProfile     = nullptr;

  m_pJob         = pJob;
  m_pThread      = nullptr;

  m_bIsStart     = false;
  m_bIsImmediate = false;

  m_eState       = TS_NONE;
  m_eTransition  = TR_NONE;
  m_eQueue       = QK_NONE;
  m_eEntryPoint  = EP_NONE;

  m_oAcksRmg.vCheckInit();
  // m_oWakeup
  
  m_bLogMemory = false;

  m_iTraceMask   = C_TRACE_NONE;
 
  m_nOperandYield = 10;

  m_pSeedWord       = nullptr;
  m_pExceptionWord  = nullptr;
  
  m_bSeedInitially  = false;
  m_bDoneFinally    = false;

  // m_oDicStack
  // m_oSaveDicStack

  m_aInxPtr      = nullptr;
  
  m_pAuxStack    = new TypedStack(KK_AUX_STACK);
  m_pDataStack   = new TypedStack(KK_DATA_STACK);
  m_pReturnStack = new TypedStack(KK_RETURN_STACK);

  m_pLocalsFrame = nullptr;

  m_pSenCompiler = new SenCompiler;
  m_pInput       = nullptr;

  
  _checkInitialised_(CN_TASK);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  vEventNoInc();
  
  ++ gixTask;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vLog_TBR(EV_CONSTRUCT,"Task constructor"_Stz);
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

  RASSERT(m_bSeedInitially);
  RASSERT(m_bDoneFinally);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete m_pDataStack;
  delete m_pReturnStack;
  delete m_pAuxStack;
  
  while (m_pLocalsFrame) {
    
    LocalChain* pLocalsFrame = m_pLocalsFrame;
    m_pLocalsFrame = m_pLocalsFrame->pNext();
    
    delete pLocalsFrame;
  }
  
  delete m_pSenCompiler;
  delete m_pInput;

    
  if (m_pJob) {
    Job* pJob = *m_pJob;
    m_pJob = nullptr;
    AM::vJobDelete(&pJob);
  }
    
  delete m_pProfile;
  m_pProfile = nullptr;
  
  -- gixTask;
  vEventNoInc();

  vLog_TBR(EV_DESTRUCT,"Task destructor"_Stz);
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
  
  if (! bSeeded()) {
    vInitExeContext();
    
    AM::vJobInitExeContext(pJob());
    
    vSeedTask();
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vAssertStructure();
  
  while (TR_NONE == m_eTransition) {
    
    // UNUSUAL CODE:
    //   get address of member containing intinsic function pointer and
    //   dereference it to get the intrinsic function to be called
    fpMEMBERFN fpStepFn = * ( (m_aInxPtr ++)->m_afpStepFn );
    
    // call the intinsic
    try {
      
      ( ((Object*)this)->*fpStepFn ) ();

    }
    
    // catch exception
    catch (Exception xException) {
      xException.vJob(m_pJob);
      vCaught(xException);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  PsxThd::vTlsSet(giTLS_TaskKey,nullptr);
}

/*----------------------------------------------------------------------------*/
/* assert task is structurally correct                                        */
/*----------------------------------------------------------------------------*/
void Task::vAssertStructure(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);

  Task* pTask = _pTask(pObject);
  
  vTrace("%s %p state %s txId %lld\n"_Stz,
         pTask->szJobName(),
         pTask,
         aTS[pTask->m_eState]->azChar()
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
  
  m_aInxPtr = aAdd<CodeCell*>(m_aInxPtr,nBytes);
}

/*----------------------------------------------------------------------------*/
/* get Object at IP                                                           */
/*----------------------------------------------------------------------------*/
Object* Task::pArgObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Object* pObject = reinterpret_cast<Object*>(m_aInxPtr);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* caught an exception in vTaskLoop catch(Exception xException) {...}         */
/*----------------------------------------------------------------------------*/
void Task::vCaught(Exception xException)
{
  xException.vCaught();
  
  AM::vTaskOnExceptionTrace(this);
  
  vReset();
  
  m_aInxPtr = m_pExceptionWord->aFirstCell();

  //vAbortProcess();
}

/*----------------------------------------------------------------------------*/
/* check for break point                                                      */
/*----------------------------------------------------------------------------*/
void Task::vCheckBreakPt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  if (! m_aInxPtr)
    return;
  
  Word* pThisWord = CodeBlock::pToWord(m_aInxPtr);
  
  if (! pThisWord->bBreakPt())
    return;
  
  BASE::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* debug                                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDebug(void)
{
}

/*----------------------------------------------------------------------------*/
/* get dictionary stack                                                       */
/*----------------------------------------------------------------------------*/
DicStack* Task::pDicStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  return & m_oDicStack;
}

/*----------------------------------------------------------------------------*/
/* get the job of this task                                                   */
/*----------------------------------------------------------------------------*/
SenCompiler* Task::pSenCompiler(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pSenCompiler;
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
/* delete input                                                               */
/*----------------------------------------------------------------------------*/
void Task::vDeleteInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  delete m_pInput;
  
  m_pInput = nullptr;
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
  
 
  
  AM::vTaskDump(this);
  
  
  Word* pWord = m_oDicStack.pLocateRoutine(m_aInxPtr);

  vTrace("\n"_Stz);
  vTrace("Execution Context\n"_Stz);
  vTrace("-----------------\n"_Stz);

  vTrace("%p:\n"
         "m_aInxPtr=%s %p\n"_Stz,
         
         this,
         
         pWord ? pWord->szWordNm() : C_NO_WORD,
         m_aInxPtr);

  vTrace("m_pSeedWord=%s %p\n"_Stz,
         m_pSeedWord->szWordNm(),
         m_pSeedWord);
  vTrace("m_pExceptionWord=%s %p\n"_Stz,
         m_pExceptionWord ? m_pExceptionWord->szWordNm() : C_NO_NAME,
         m_pExceptionWord);

  m_pDataStack->vDump(&m_oDicStack,  "m_pDataStack:"_Stz);
  m_pReturnStack->vDump(&m_oDicStack,"m_pReturnStack:"_Stz);
  m_pAuxStack->vDump(&m_oDicStack,   "m_pAuxStack:"_Stz);

  m_oDicStack.vContentDump();
  
  if (pWord)
    AM::vWordSee(pWord,&m_oDicStack);

  
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
  if (m_aInxPtr)
    return;
  
  Stz* szProfileNm = AM::szJobOpNm(m_pJob);
  if (!szProfileNm)
    szProfileNm = AM::szJobNm(m_pJob);
  
  m_pProfile = new Profile(szProfileNm);
}

/*----------------------------------------------------------------------------*/
/* get input stream                                                           */
/*----------------------------------------------------------------------------*/
Input* Task::pInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  return m_pInput;
}

/*----------------------------------------------------------------------------*/
/* immediate task?                                                            */
/*----------------------------------------------------------------------------*/
bool Task::bIsImmediate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  return m_bIsImmediate;
}

/*----------------------------------------------------------------------------*/
/* set immediate task                                                         */
/*----------------------------------------------------------------------------*/
void Task::vIsImmediate(bool bIsImmediate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  m_bIsImmediate = bIsImmediate;
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
/* is the start task?                                                         */
/*----------------------------------------------------------------------------*/
bool Task::bIsStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  return m_bIsStart;
}

/*----------------------------------------------------------------------------*/
/* set start task                                                             */
/*----------------------------------------------------------------------------*/
void Task::vIsStart(bool bIsStart)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  m_bIsStart = bIsStart;
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
    return aCN[AM::eJobClass(m_pJob)];
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
  
  LocalChain* pLocalsframe = m_pLocalsFrame;
  
  m_pLocalsFrame = m_pLocalsFrame->pNext();
  
  delete pLocalsframe;
}

/*----------------------------------------------------------------------------*/
/* push a locals frame                                                        */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPush(void)
{
  LocalChain* pLocalsframe = new LocalChain;
  
  pLocalsframe->vNext(m_pLocalsFrame);
  
  m_pLocalsFrame = pLocalsframe;
}

/*----------------------------------------------------------------------------*/
/* log memory?                                                                */
/*----------------------------------------------------------------------------*/
bool Task::bLogMemory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  return m_bLogMemory;
}

/*----------------------------------------------------------------------------*/
/* set log memory                                                             */
/*----------------------------------------------------------------------------*/
void Task::vLogMemory(bool bLogMemory)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  m_bLogMemory = bLogMemory;
}

/*----------------------------------------------------------------------------*/
/* new input                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vNewInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  RASSERT(! m_pInput);
    
  m_pInput = new Input;
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
/* reseed task                                                                */
/*----------------------------------------------------------------------------*/
void Task::vReseed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  vReset();

  vSeedTask();
}

/*----------------------------------------------------------------------------*/
/* reset forth execution context                                              */
/*----------------------------------------------------------------------------*/
void Task::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  m_pAuxStack->vReset();
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
  
  while (m_pLocalsFrame) {
    
    LocalChain* pLocalsFrame = m_pLocalsFrame;
    m_pLocalsFrame = m_pLocalsFrame->pNext();
    
    delete pLocalsFrame;
  }
  
  m_aInxPtr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get save path stack                                                       */
/*----------------------------------------------------------------------------*/
DicStack* Task::pSaveDicStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  return & m_oSaveDicStack;
}

/*----------------------------------------------------------------------------*/
/* is task seeded to run                                                      */
/*----------------------------------------------------------------------------*/
bool Task::bSeeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_aInxPtr)
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
  
  Dic* pDicRoot = AM::pDicRoot();
  
  Stz* szSeed = AM::szJobSeed(m_pJob);
  
  //vTrace("SEED %s\n"_Stz,szSeed);
  Word* pSeedWord = pDicRoot->pFindWord(szSeed);
  
  if (! pSeedWord)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Unable to find seed word: %s\n"_Stz,szSeed);
  


  m_pSeedWord = pSeedWord;
  m_aInxPtr   = pSeedWord->aFirstCell();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Word* pExceptionWord = pDicRoot->pFindWord(pSeedWord->szExceptionHook());
  if (pExceptionWord) {
    m_pExceptionWord = pExceptionWord;
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Word* pExceptionDef = pDicRoot->pFindWord("std_exceptionHook"_Stz);
  if (pExceptionDef) {
    m_pExceptionWord = pExceptionDef;
    return;
  }


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
    case TS_COMPLETED: pStats->Task.m_nExited++;    break;
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
  
  return m_iObjectId;
}

/*----------------------------------------------------------------------------*/
/* get thread                                                                 */
/*----------------------------------------------------------------------------*/
Thread* Task::pvThread_DONT_DEREF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pThread;
}

/*----------------------------------------------------------------------------*/
/* set thread                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThread(Thread* pThread)
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
  
  s256 sWordWithCodeCell;
  
  CodeCell* aHasCell = m_aInxPtr;
  
  Word* pLocatedWord = m_oDicStack.pLocateRoutine(aHasCell);
  if (pLocatedWord)
    sWordWithCodeCell = pLocatedWord->szWordNm();
  else
    sWordWithCodeCell = "<unknown>"_Stz;
  
  i8 iAuxValues    = m_pAuxStack->nValues();
  i8 iDataValues   = m_pDataStack->nValues();
  i8 iReturnValues = m_pReturnStack->nValues();
  
  vTrace("[%s(N%d):%8s\\%lld %-11s] %-16s : a%2lld="_Stz,
        
         AM::szNodeNm(),
         AM::iThisNodeNo(),
         szJobName(),
         m_oAcksRmg.iPrintOnlyValue(),
         
         sWordWithCodeCell.azChar(),
         aMark,
         iAuxValues);
         
  s48K sStackContents;
  m_pAuxStack->vStackContents(sStackContents);
  
  vTrace("%s"_Stz,sStackContents.azChar());
  
  vTrace(" r%2lld  d%2lld="_Stz, iReturnValues, iDataValues);

  m_pDataStack->vStackContents(sStackContents);
  
  vTrace("%s\n"_Stz,sStackContents.azChar());
}

/*----------------------------------------------------------------------------*/
/* trace step                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTraceStep(Stz* aStepNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  s256 sWordWithCodeCell;
  
  CodeCell* aHasCell = m_aInxPtr;
  Word* pLocatedWord = m_oDicStack.pLocateRoutine(aHasCell);
  if (pLocatedWord)
    sWordWithCodeCell = pLocatedWord->szWordNm();
  else
    sWordWithCodeCell = "<unknown>"_Stz;
  
  i8 iDataValues   = m_pDataStack->nValues();
  i8 iReturnValues = m_pReturnStack->nValues();
  vTrace("%-15s %15s : r%lld ; d%2d="_Stz,
         sWordWithCodeCell.azChar(),
         aStepNm,
         iReturnValues,
         iDataValues);
  
  s48K sStackContents;
  m_pDataStack->vStackContents(sStackContents);
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
  
  RASSERT(pScanEntry->eGetClassNo() == CN_TASK);
  RASSERT(pInsertEntry->eGetClassNo() == CN_TASK);
  
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
