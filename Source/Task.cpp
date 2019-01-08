/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

//#define MUNGED

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

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
: Object(CN_TASK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_oLock.vInitLock(LK_TASK);
  
  m_pJob         = pJob;
  
  m_iQueueOrder  = C_UNKNOWN;
  m_iPartition   = C_UNKNOWN;
  
  m_eState       = TS_NONE;
  m_eQueue       = QR_NONE;
  m_oAcksRemaining.vCheckInit();
  m_pThread      = nullptr;
  
  m_pDataStack   = nullptr;
  m_pReturnStack = nullptr;
  m_pAuxStack    = nullptr;
  
  m_pLocalsFrame = nullptr;
  m_pOpFrame     = nullptr;
  
  IP.m_pCell     = nullptr;
  m_pEntryCell   = nullptr;
  
  m_pCompiler    = (CN_JOB_BOOT == pJob->eClass()) ? new Compiler : nullptr;
  
  m_pWord        = nullptr;
  m_iTracing     = C_TRACE_NONE;
  
  m_pSession     = nullptr;
  
  m_pPlan        = nullptr;
  
  m_iDdlLocked   = 0;
  
  m_pSendNode    = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the executing task in job
  
  m_pJob->m_pExecutingTask = this;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Task::~Task(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  // delete the stacks
  delete m_pDataStack;
  delete m_pReturnStack;
  delete m_pAuxStack;
  
  delete m_pCompiler;
  
  while (m_pLocalsFrame) {

    RegFrame* pLocalsFrame = m_pLocalsFrame;
    m_pLocalsFrame = m_pLocalsFrame->pNext();
  
    delete pLocalsFrame;
  }
  
  RASSERT(! m_pJob);
  RASSERT(! m_iDdlLocked);
}

/*----------------------------------------------------------------------------*/
/* task loop                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vTaskLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (! m_pDataStack )
    vNewStacks();
  
  while (TS_EXECUTING == m_eState) {
    
    TaskStep pTaskStep = *IP.m_pCell->m_ppEntryStep;
    
    m_pEntryCell = IP.m_pCell->m_ppEntryStep + 1;
    
    //vTrace("pTaskStep %p\n"_Stz,pTaskStep);
    
    IP.m_pCell++;
    
    try {
      (pTaskStep)(this);
    }
    
    catch (i8 iException) {
      char wFileName[256];
      
      const char* ps;
      
      ps = StrLib::sLast(m_sFile.aChar());
      StrLib::bBackward(&ps,m_sFile.aChar(),'/');
      bool bBackSlash = StrLib::bBackward(&ps,m_sFile.aChar(),'/');
      if (bBackSlash) {
        StrLib::vCopy(wFileName,ps+1);
      }
      else {
        StrLib::vCopy(wFileName, m_sFile.aChar());
      }
      vError("[%s] %s : line %lld in %s\n"_Stz,
             aJobName(),
             m_sException.aChar(),
             iException,
             wFileName);
      vRetry();
    }
    
    catch (Error* pError) {
      char wFileName[256];
      
      const char* ps;
      
      ps = StrLib::sLast(pError->aFileName()->aChar());
      StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
      StrLib::vCopy(wFileName,ps+1);
      vError("[%s] %s: line %lld in %s\n"_Stz,
             aJobName(),
             pError->aErrorMsg(),
             pError->iLineNo(),
             wFileName);
      vRetry();
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decrement ack and return if caller is decider                              */
/*----------------------------------------------------------------------------*/
bool Task::bAckAndDecide(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  bool bAmDecider = m_oAcksRemaining.bDecAndDecide();
  
  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* adjust next pointer                                                        */
/*----------------------------------------------------------------------------*/
void Task::vAdjustNext(i8 iBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  IP.m_pCell = pAdd<Cell*>(IP.m_pCell,iBytes);
}

/*----------------------------------------------------------------------------*/
/* get Object at IP                                                           */
/*----------------------------------------------------------------------------*/
Object* Task::pArgObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Object* pObject = reinterpret_cast<Object*>(IP.m_pCell);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* debug                                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDebug(void)
{
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Task* pTask = _pTask(pObject);
  
  vTrace("%s %p state %s txId %lld\n"_Stz,
         pTask->aJobName(),
         pTask,
         aTS[pTask->m_eState]->aChar()
         );
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vContentDump(void)
{
  _poolAddress_(this);
  
  vTrace("%sTask %p: %16s state %8s txId %lld\n"_Stz,
         aDoubleTab(),
         this,
         aJobName(),
         aTS[m_eState]->aChar()
         );
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* set exception                                                              */
/*----------------------------------------------------------------------------*/
void Task::vException(StrVal& sFile, StrVal& eException)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_sFile = sFile;
  m_sException = eException;
}

/*----------------------------------------------------------------------------*/
/*form activity mark                                                          */
/*----------------------------------------------------------------------------*/
void Task::vFormMark(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  if (TS_SLEEPING == m_eState) {
    Time oDateTime;
    oDateTime.vSetLocal(&m_oWakeupTime);
    s32 sDateTime;
    char* z = sDateTime.aChar();
    oDateTime.vAppendLocal(&z);
    m_sMark.vSprint("Task %p %s %s wakeup %s"_Stz,
                    this,
                    aCN[m_pJob->eClass()],
                    aTS[m_eState],
                    sDateTime.aChar()
                    );
  }
  else {
    m_sMark.vSprint("Task %p %s %s"_Stz,
                    this,
                    aCN[m_pJob->eClass()],
                    aTS[m_eState]
                    );
  }
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_oLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is task locked?                                                            */
/*----------------------------------------------------------------------------*/
bool Task::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  bool b = m_oLock.bIsLocked();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is this a non-partitioned task?                                            */
/*----------------------------------------------------------------------------*/
bool Task::bIsNonPartitioned(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (-1 == m_iPartition)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a partitioned task?                                                */
/*----------------------------------------------------------------------------*/
bool Task::bIsPartitioned(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (-1 != m_iPartition)
    return true;
  else
    return false;
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
Stz* Task::aJobName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_pJob && m_pJob->m_pJobClass)
    return (Stz*)m_pJob -> m_pJobClass->m_aName;
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
  
  RegFrame* pLocalsframe = m_pLocalsFrame;
  
  m_pLocalsFrame = m_pLocalsFrame->pNext();
  
  delete pLocalsframe;
}

/*----------------------------------------------------------------------------*/
/* push a locals frame                                                        */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPush(void)
{
  RegFrame* pLocalsframe = new RegFrame;
  
  pLocalsframe->vNext(m_pLocalsFrame);
  
  m_pLocalsFrame = pLocalsframe;
}

/*----------------------------------------------------------------------------*/
/* new the stacks                                                             */
/*----------------------------------------------------------------------------*/
void Task::vNewStacks(void)
{
  if (m_pDataStack)
    return;
  
  m_pDataStack   = new Stack;
  m_pReturnStack = new Stack;
  m_pAuxStack    = new Stack;
}

/*----------------------------------------------------------------------------*/
/* pop int                                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopInt(i8& iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! m_pDataStack->bPop(iInt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop ptr to object                                                          */
/*----------------------------------------------------------------------------*/
bool Task::bPopObject(Object*& pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! m_pDataStack->bPop(pObject) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop str                                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopStr(s48K& sStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! m_pDataStack->bPop(sStr) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push int                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPushInt(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pDataStack->vPushInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* push object                                                                */
/*----------------------------------------------------------------------------*/
void Task::vPushObject(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pDataStack->vPushObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* push str                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPushStr(Stz* aStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  s48K sStr = aStr;
  
  m_pDataStack->vPushS48K(sStr);
}

/*----------------------------------------------------------------------------*/
/* push str                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPushStr(s48K& sStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pDataStack->vPushS48K(sStr);
}

/*----------------------------------------------------------------------------*/
/* get task queue                                                             */
/*----------------------------------------------------------------------------*/
eQR Task::eQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eQueue;
}

/*----------------------------------------------------------------------------*/
/* set task queue                                                             */
/*----------------------------------------------------------------------------*/
void Task::vQueue(eQR eQueue)
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
  m_pJob->m_pJobClass = pJobClass;
  
  //m_pJob->m_pJobClass->vJobCat();
}

/*----------------------------------------------------------------------------*/
/* rertry after exception                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  IP.m_pCell = m_pCompiler->m_pRetryIP;
  m_pCompiler->m_pBeginIP = nullptr;
  
  m_pCompiler->m_pTokeniser->sToken() = ""_Stz;
  
  m_pDataStack->vRetry();
  m_pReturnStack->vRetry();
}

/*----------------------------------------------------------------------------*/
/* is task seeded to run                                                      */
/*----------------------------------------------------------------------------*/
bool Task::bSeeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (IP.m_pCell)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* seed boot run job                                                          */
/*----------------------------------------------------------------------------*/
void Task::vSeedJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Stz* sSeed = m_pJob->sJobSeed();
  
  Word* pWord = gpAttachMap->m_xMainDic.pFindWord(sSeed);
  
  IP.m_pCell = pWord->pFirstCell();
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::enter                                                   */
/*----------------------------------------------------------------------------*/
GridModel::Node* Task::pSendNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pSendNode;
}

/*----------------------------------------------------------------------------*/
/* get session                                                                */
/*----------------------------------------------------------------------------*/
Interface::Session* Task::pSession(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pSession;
}

/*----------------------------------------------------------------------------*/
/* set session                                                                */
/*----------------------------------------------------------------------------*/
void Task::vSession(Interface::Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pSession = pSession;
}

/*----------------------------------------------------------------------------*/
/* sleep for microseconds                                                     */
/*----------------------------------------------------------------------------*/
void Task::vSleepUs(i8 iMicroseconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_oWakeupTime.vNow();
  m_oWakeupTime.vAdd(iMicroseconds);
}

/*----------------------------------------------------------------------------*/
/* get slot this tasks belongs to                                             */
/*----------------------------------------------------------------------------*/
i8 Task::iSlot(void)
{
  i8 iSlot = TablePtn::iPartitionToSlot(m_iPartition);
  
  return iSlot;
}

/*----------------------------------------------------------------------------*/
/* stack contents as string                                                   */
/*----------------------------------------------------------------------------*/
void Task::vStackContents(Stack* pStack, StrVal& sStackContents)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  sStackContents = ""_Stz;
  
  i8 iParamValues = pStack->iValues();
  
  sStackContents.vCatSprint("("_Stz);
  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
    
    sStackContents.vCatSprint(" "_Stz);
    
    Value* pValue = pStack->pPeekValue(iValue);
    
    eCN eClass = pValue->eClass();
    
    if (CN_BINVAL == eClass) {
      Object* pObject = _pBinVal(pValue)->pNewObject();
      sStackContents.vCatSprint("(BINVAL)^%s=%p"_Stz,
                                aCN[pObject->eClass()],
                                pObject);
    }
    else if (CN_INTVAL == eClass) {
      sStackContents.vCatSprint("%lld"_Stz,_pIntVal(pValue)->iInt());
    }
    else if (CN_PTRVAL == eClass) {
      if (_pPtrVal(pValue)->pPtr()) {
        
        Object* pPtr = _pPtrVal(pValue)->pPtr();
        
        eCN ePtrClass = _pPtrVal(pValue)->pPtr()->eClass();
        
        if (CN_WORD == ePtrClass)
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    ((Word*)pPtr)->sName().aChar());
        else if (CN_QUEUE == ePtrClass) {
          Queue* pQueue = _pQueue(pPtr);
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
                                    pTask->aJobName());
        }
        else {
          if (pPtr->iMagic() == C_MAGIC && pPtr->eClass() < CN_ENUMS)
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
                                  pValue->aClass(),
                                  _pPtrVal(pValue)->pPtr());
    }
    else if (CN_IPVAL == eClass) {                                      
      sStackContents.vCatSprint("%p"_Stz,
                                _pIpVal(pValue)->pIpPtr());
    }
    else if (CN_STRVAL == eClass) {
      sStackContents.vCatSprint("\"%lls\""_Stz,
                                _pStrVal(pValue)->aChar());
    }
    else if (CN_VALSET == eClass) {
      sStackContents.vCatSprint("<%s>=%p"_Stz,
                                pValue->aClass(),
                                pValue);
    }
    else {
      sStackContents.vCatSprint("<unknown:class=%lld>"_Stz,
                                pValue->eClass());
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
  
  //m_oLock.vGrabWrite();
  m_eState = eState;
  //m_oLock.vDropWrite();
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
  if (gxArguments.m_nNodes > 1) {
    i8 iPause = Random::iInteger(999,99999);
    PosixThread::vSleepMicro(iPause);
  }
  
  s256 sRoutine;
  
  Cell* pCell = IP.m_pCell;
  
  Word* pLocatedWord = m_oPathStack.pLocate(pCell);
  if (pLocatedWord)
    sRoutine = pLocatedWord->sName();
  else
    sRoutine = "<unknown>"_Stz;
  
  i8 iAuxValues    = m_pAuxStack->iValues();
  i8 iDataValues   = m_pDataStack->iValues();
  i8 iReturnValues = m_pReturnStack->iValues();
  
  vTrace("[%s(N%d):%8s\\%lld %-11s] %-16s : a%2lld="_Stz,
        
         gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
         gpAttachMap->m_pGrid->iThisNode(),
         aJobName(),
         m_oAcksRemaining.iPrintOnlyValue(),
         
         sRoutine.aChar(),
         aMark,
         iAuxValues);
         
  s48K sStackContents;
  vStackContents(m_pAuxStack, sStackContents);
  
  vTrace("%s"_Stz,sStackContents.aChar());
  
  vTrace(" r%2lld  d%2lld="_Stz, iReturnValues, iDataValues);

  vStackContents(m_pDataStack, sStackContents);
  
  vTrace("%s\n"_Stz,sStackContents.aChar());
}

/*----------------------------------------------------------------------------*/
/* trace step                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTraceStep(Stz* aStepNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  s256 sRoutine;
  
  Cell* pCell = IP.m_pCell;
  Word* pLocatedWord = m_oPathStack.pLocate(pCell);
  if (pLocatedWord)
    sRoutine = pLocatedWord->sName();
  else
    sRoutine = "<unknown>"_Stz;
  
  i8 iDataValues  = m_pDataStack->iValues();
  i8 iReturnValues = m_pReturnStack->iValues();
  vTrace("%-15s %15s : r%lld ; d%2d="_Stz,
         sRoutine.aChar(),
         aStepNm,
         iReturnValues,
         iDataValues);
  
  s48K sStackContents;
  vStackContents(m_pDataStack, sStackContents);
  vTrace("%s\n"_Stz,sStackContents.aChar());
}

/*----------------------------------------------------------------------------*/
/* get partiton this tasks nelongs to                                         */
/*----------------------------------------------------------------------------*/
i8 Task::iPartition(void)
{
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* trace performance point                                                    */
/*----------------------------------------------------------------------------*/
void Task::vPerformancePoint(Stz* aWordName, Stz* aPointNote)
{
  if ( ! m_pSession ) {
    vWarn("No session for Performance Point: %s %s\n"_Stz,aWordName,aPointNote);
    return;
  }
  
  PerformanceTrace* pPerformanceTrace = m_pSession->pPerformanceTrace();
  
  if ( ! pPerformanceTrace ) {
    pPerformanceTrace = new PerformanceTrace;
    m_pSession->vPerformanceTrace(pPerformanceTrace);
  }
  
  pPerformanceTrace->vAdd(aWordName,aPointNote);
}

/*----------------------------------------------------------------------------*/
/* trace performance print                                                    */
/*----------------------------------------------------------------------------*/
void Task::vPerformancePrint(void)
{
  PerformanceTrace* pPerformanceTrace = m_pSession->pPerformanceTrace();
  if ( ! pPerformanceTrace )
    return;
  
  pPerformanceTrace->vPrint();
  
  pPerformanceTrace->vReset();
}

/*----------------------------------------------------------------------------*/
/* get task order no                                                          */
/*----------------------------------------------------------------------------*/
i8 Task::iQueueOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_iQueueOrder;
}

/*----------------------------------------------------------------------------*/
/* set task order no                                                          */
/*----------------------------------------------------------------------------*/
void Task::vQueueOrder(i8 iQueueOrder)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_iQueueOrder = iQueueOrder;
}

/*----------------------------------------------------------------------------*/
/* can task to be woken?                                                      */
/*----------------------------------------------------------------------------*/
bool Task::bWakeupNow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  Since oSince;
  
  oSince.vNow();
  
  i8 iTimeNow = oSince.iGetAsMicroseconds();

  if (iWakeupTime() <= iTimeNow)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get wakeup time                                                            */
/*----------------------------------------------------------------------------*/
i8 Task::iWakeupTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  i8 iMicroseconds = m_oWakeupTime.iGetAsMicroseconds();
  
  return iMicroseconds;
}

/*----------------------------------------------------------------------------*/
/* get wakeup time                                                            */
/*----------------------------------------------------------------------------*/
Since& Task::oWakeupTime(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  return m_oWakeupTime;
}

/*----------------------------------------------------------------------------*/
/* wakeup the task                                                            */
/*----------------------------------------------------------------------------*/
void Task::vWakeupTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  // clear the wakeup time
  m_oWakeupTime.vSet(0,0);

  m_eState = TS_READY;
}

/*----------------------------------------------------------------------------*/
/* compute next queue                                                         */
/*----------------------------------------------------------------------------*/
eQR Task::eWhichQueue(void)  //! RAY TO FINISH
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  // examine the state and determine which queue
  
  switch (m_eState) {
    
    case TS_READY:
    case TS_YIELDED:
      if (bIsPartitioned())
        return (eQR)iSlot();
      else
        return QR_READY_QUEUE;
      
    case TS_SLEEPING:
      return QR_SLEEPING_QUEUE;
      
    case TS_WAITING: 
      return QR_WAITING_QUEUE;
      
    default:
      vFatal("Task::eWhichQueue: Unexpected task state: %lld\n"_Stz,
             (i8)m_eState);
  }
  
  return QR_NONE;
}

/*----------------------------------------------------------------------------*/
/* write a mark about this task                                               */
/*----------------------------------------------------------------------------*/
void Task::vWriteMark(Stz* aPrefix)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vFormMark();
  
  //vTrace("MARK: %s %s\n"_Stz,aPrefix,m_sMark.aChar());
  
  ActivityMap::vMark(m_pJob->m_iJobId,
                     "%s %s"_Stz,
                     aPrefix,
                     m_sMark.aChar());
}

/*----------------------------------------------------------------------------*/
/* sprint of stack using given format string                                  */
/*----------------------------------------------------------------------------*/
void Task::vSprint(s48K& sFormat, s48K& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  char* pf = sFormat.aChar();
  char* ps = sString.aChar();
  
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
    
    Value* pValue0 = m_pDataStack->pPeekValue(0);
    eCN eClass = pValue0->eClass();
    
    // test for StrVal
    if (CN_STRVAL == eClass) {
      
      //*pf = 's';                                                    //! Defect??
      
      s48K sValue;
      m_pDataStack->bPop(sValue);
      
      if ('V' == *pf)
        *ps++ = '"';
      
      for (char* pv = sValue.aChar(); *pv; )
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
      m_pDataStack->bPop(iValue);
      
      char aValue[32];
      StrLib::wToString(iValue,aValue);
      
      for (char* pv = aValue; *pv; )
        *ps++ = *pv++;
      
      pf++;
      
      continue;
    }
    
    // test for ptrval
    if (CN_PTRVAL == eClass) {
      
      //*pf = 'i';                                                    //! Defect??
      
      Object* pValue;
      m_pDataStack->bPop(pValue);
      
      char aValue[32];
      StrLib::wToPointer(pValue,aValue);
      
      for (char* pv = aValue; *pv; )
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Task::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Compare -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare two tasks by their taskNo                                          */
/*----------------------------------------------------------------------------*/
bool bTaskCompareQueueOrder(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  Task* pInsertEntry = _pTask(pObjectInsert);
  Task* pScanEntry   = _pTask(pObjectScan);
  
  //RJH vTrace("bTaskCompareQueueOrder1\n"_Stz);
  
  RASSERT(pScanEntry->eClass() == CN_TASK);
  RASSERT(pInsertEntry->eClass() == CN_TASK);
  
  if ( ! pObjectScan ) {
    //RJH vTrace("bTaskCompareQueueOrder2\n"_Stz);
    return false;
  }
  
  if (pInsertEntry->iQueueOrder() == pScanEntry->iQueueOrder())
    bMatch = true;
  else
    bMatch = false;
  
  //RJH vTrace("bTaskCompareQueueOrder3\n"_Stz);
  
  if (pInsertEntry->iQueueOrder() > pScanEntry->iQueueOrder())
    return true;
  else
    return false;
}

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
  
  if (pInsertEntry->iWakeupTime() == pScanEntry->iWakeupTime())
    bMatch = true;
  else
    bMatch = false;
  
  if (pInsertEntry->iWakeupTime() > pScanEntry->iWakeupTime())
    return true;
  else
    return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
