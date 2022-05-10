/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

#include "BASE.h"

#include "PsxThd.hpp" //?
#include "TagConstants.h" //?
#include "Job.hpp" //?
#include "Op.hpp" //?
#include "Grid.hpp" //?

#include "SP_Lock.hpp" //?
#include "SP_Op.hpp" //?
#include "SP_Timer.hpp" //?
#include "SP_Word.hpp" //?
#include "LogicUnit.hpp" //?

#include "MY.hpp"
#include "HeadsUp.hpp"

extern pthread_key_t giTLS_TaskKey;

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
Task::Task(Stz* szTaskNm, Job* pJob)
: Object(CN_TASK)
, m_okLock(this,false)
, m_oReadLocks(this)
, m_oWriteLocks(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vTask();
  
  m_sTaskNm = szTaskNm;
  m_pJob    = pJob;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(Stz* szTaskNm, Op* pOp)
: Object(CN_TASK)
, m_okLock(this,false)
, m_oReadLocks(this)
, m_oWriteLocks(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  vTask();

  m_sTaskNm = szTaskNm;
  m_pOp     = pOp;
}

/*----------------------------------------------------------------------------*/
/* common init                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_iDebugMark   = 0;
  m_pProfile     = nullptr;

  m_pInvokingTask = nullptr;
  m_pThread      = nullptr;
  
  m_bIsStart     = false;
  m_bIsImmediate = false;

  m_eState       = TS_NONE;
  m_eTransition  = TR_NONE;
  m_eQueue       = QK_NONE;
  m_eEntryPoint  = EP_NONE;

  m_oAcksRmg.vCheckInit();
  // m_oWakeup
  
  m_iTraceMask   = C_TRACE_NONE;
 
  m_nOperandYield = AM::SYSARGS::nOperandYield(); //10;

  m_pRecoveryWord  = nullptr;
  
  // m_oDicStack
  // m_oSaveDicStack

  m_aInxPtr      = nullptr;
  
  m_pAuxStack    = new TypedStack(KK_AUX_STACK);
  m_pDataStack   = new TypedStack(KK_DATA_STACK);
  m_pReturnStack = new TypedStack(KK_RETURN_STACK);

  m_pLocalsFrame = nullptr;

  m_pSenCompiler = new SenCompiler;
  m_pInput       = nullptr;
  
  m_pLogicUnit = nullptr;
  
  _checkInitialised_(CN_TASK);
  
  m_pLogicUnit = new LogicUnit;
    
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

  ASSERT(! m_oWriteLocks.nItems());

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
  while (m_pInput) {
    vDeleteInput();
  }
  
  delete m_pProfile;
  m_pProfile = nullptr;
  
  delete m_pLogicUnit;
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
  
  vVerifyTaskComposition();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  while (TR_NONE == m_eTransition) {
    
    // UNUSUAL CODE:
    //   get address of member containing intinsic function pointer and
    //   dereference it to get the intrinsic function to be called
    fpMEMBERFN fpStepFn = * ( (m_aInxPtr ++)->m_afpStepFn );
    
    // call the non-virtual member/step function against this task
    try {
            
      (this->*fpStepFn) ();

      //this->vAssertSelf(this);

      //
      // for example if the step function is:
      //   void Step::vIntAdd(void) { ... }
      //
      // then it is the dynamic equivalent of:
      //   this->vIntAdd();
      //
    }
    
    // catch exception
    catch (Exception xException) {
      
      if (AM::SHOW::bException()) {
        fprintf(stderr,"\n");
        fprintf(stderr,
                // 0         1         2         3
                // 0123456789012345678901234567890123456789"
                   "@CATCH @CATCH @CATCH @CATCH @CATCH @CATC" // 40
                   "H @CATCH @CATCH @CATCH @CATCH @CATCH @CA" // 80
                   "TCH @CATCH @CATCH @CATCH @CATCH @CATCH" // 120
                   "\n"
                );
        fprintf(stderr,"\n");
      }
      
      m_xException = xException;
      
      if (AM::SHOW::bException()) {
        vDumpOnException();
      }
      
      vExceptionHandler();
            
      if (AM::SHOW::bException()) {
        fprintf(stderr,"\n");
        fprintf(stderr,
                // 0         1         2         3
                // 0123456789012345678901234567890123456789"
                  "@RECOVER @RECOVER @RECOVER @RECOVER @REC" // 40
                  "OVER @RECOVER @RECOVER @HANDLER @HANDLER" // 80
                  " @RECOVER @RECOVER @RECOVER @RECOVER" // 120
                  "\n"
                );
        fprintf(stderr,"\n");
      }
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  PsxThd::vTlsSet(giTLS_TaskKey,nullptr);
}

/*----------------------------------------------------------------------------*/
/* verify task is composed correctly and therefore capable of being executed  */
/*----------------------------------------------------------------------------*/
void Task::vVerifyTaskComposition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RASSERT(EC_NONE == eValidHeader());
  RASSERT(CN_TASK == eGetClassNo());
  aFooter()->vAssertFooter();
  
  if (m_pJob.bIsNotNull()) {
    RASSERT(m_pOp.bIsNull());
    RASSERT(m_sTaskNm.bSame(m_pJob->szJobNm()));
    m_pJob->vVerifyJobComposition();
  }
  else
  if (m_pOp.bIsNotNull()) {
    RASSERT(m_pJob.bIsNull());
    RASSERT(m_sTaskNm.bSame(m_pOp->szOpNm()));
    m_pOp->vVerifyOpComposition();
  }
  else {
    NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* caught an exception in vTaskLoop catch(Exception xException) {...}         */
/*----------------------------------------------------------------------------*/
void Task::vExceptionHandler(void)
{
  _noPreconditions_
 
  if (AM::SHOW::bException()) {
    fprintf(stderr,"\n");
    fprintf(stderr,
            // 0         1         2         3
            // 0123456789012345678901234567890123456789"
              "@HANDLER @HANDLER @HANDLER @HANDLER @HAN" // 40
              "DLER @HANDLER @HANDLER @HANDLER @HANDLER" // 80
              " @HANDLER @HANDLER @HANDLER @HANDLER" // 120
              "\n"
            );
    fprintf(stderr,"\n");
  }
  
  vSetSenRecoverContext();
  
  if (AM::SHOW::bException()) {
    vSeeFrameStack();
  
    Word* pWord = m_oDicStack.pLocateRoutine(m_aInxPtr);
    if (pWord)
      AM::WORD::vDisassemble(pWord,&m_oDicStack,m_aInxPtr);
  }
}

/*----------------------------------------------------------------------------*/
/* detailed dump of task on exception                                         */
/*----------------------------------------------------------------------------*/
void Task::vDumpOnException(void)
{
  _noPreconditions_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("\n"_Stz);
  vTrace("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"_Stz);
  vTrace("Dump of Task=%p\n"_Stz,this);

  Now oNow;
  
  SP_Word spWord;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("\n"_Stz);
  vTrace("Now %s\n"_Stz,oNow.sz());
 
  vTrace("\n"_Stz);

  if (m_pJob.bIsNotNull()) {
    vTrace("1) Task (for Job)\n"_Stz);
    vTrace("m_pJob=%p %s\n"_Stz,*m_pJob,m_pJob->szJobNm());

  }
  else
  if (m_pOp.bIsNotNull()) {
    vTrace("1) Task (for Op)\n"_Stz);
    SP_Op spOp;
    vTrace("m_pOp=%p (%s); %lld queued operands\n"_Stz
           ,*m_pOp
           ,spOp.szSpot(m_pOp)
           ,m_pOp->nQueuedOperands());
  }

  vTrace("m_aInxPtr=%p "_Stz,m_aInxPtr);
  Word* pWord = m_oDicStack.pLocateRoutine(m_aInxPtr);
  if (pWord)                                                             // WORD
    vTrace("in word=%p %s %s %s "_Stz
           ,pWord
           ,aWK[pWord->eWord()]
           ,pWord->szWordNm()
           ,pWord->pCategory() ? pWord->pCategory()->szWordNm()
                               : ""_Stz
           );
  vTrace("\n"_Stz);

  vTrace("m_pRecoveryWord=%s\n"_Stz,spWord.sz(m_pRecoveryWord));
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  if (m_pJob.bIsNotNull()) {
    vTrace("\n"_Stz);
    vTrace("2) Job\n"_Stz);
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  else
  if (m_pOp.bIsNotNull()) {
    vTrace("\n"_Stz);
    vTrace("2) Op\n"_Stz);
  

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("\n"_Stz);
  vTrace("3) Event Handler \n"_Stz);
  
  if (m_pOp) {
    vTrace("EventHandler %lld:%s (+%lld times)\n"_Stz,
         (i8)m_pOp->eCurEvent(),
         aZK[m_pOp->eCurEvent()],
         m_pOp->nEventCount(m_pOp->eCurEvent())
         );
  }
  
  pWord = m_oDicStack.pLocateRoutine(m_aInxPtr);                         // WORD
  if (pWord)
    AM::WORD::vDisassemble(pWord,&m_oDicStack,m_aInxPtr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("\n"_Stz);
  vTrace("Event Frames\n"_Stz);
    
  Task::vSeeFrameStack();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("\n"_Stz);
  vTrace("End Dump of Task\n"_Stz);
  vTrace("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"_Stz);

    vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* trace the task on exception                                                */
/*----------------------------------------------------------------------------*/
void Task::vExceptionTrace(void)
{
  _noPreconditions_
    
  //
  // trace what is relevant for debugging.  could be quite substantial
  //
  
  vTrace("\n\n"_Stz);
  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  vTrace("Exception Trace of Task %p\n"_Stz
         
         ,this
         );
  
  
  AM::vTaskDump();
  vContentDump();
  AM::vTaskFullTrace(this);
  
  vTrace("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"_Stz);
  vTrace("\n\n"_Stz);
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
         pTask->szTaskNm(),
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
  
  if (AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("Task: Setting acks remaining %lld\n"_Stz,nAcksRmg);

  m_oAcksRmg.vSetRemaining(nAcksRmg);
}

/*----------------------------------------------------------------------------*/
/* decrement acks remaining and return if caller is decider                   */
/*----------------------------------------------------------------------------*/
bool Task::bAcksRmgDec(i8& nAcksWereRemaining)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  bool bAmDecider = m_oAcksRmg.bDecAndDecide(nAcksWereRemaining);
  
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
/* get current executing word                                                 */
/*----------------------------------------------------------------------------*/
Word* Task::pCurWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pCurWord = m_oDicStack.pLocateRoutine(m_aInxPtr);;
  
  return pCurWord;
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
/* cycle task lock                                                            */
/*----------------------------------------------------------------------------*/
void Task::vCycleTaskLock(Stz* szFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vGrabTaskLock(szFunc);
  vDropTaskLock(szFunc);
}

/*----------------------------------------------------------------------------*/
/* delete input                                                               */
/*----------------------------------------------------------------------------*/
void Task::vDeleteInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  m_pInput = m_pInput->pDeleteInput();
}

/*----------------------------------------------------------------------------*/
/* . print                                                                    */
/*----------------------------------------------------------------------------*/
void Task::vDotPrint(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  sSTMT sFormatted;
  
  va_list vaArgs;
  va_start(vaArgs,szFormat);
  sFormatted.vSprintVA(szFormat,vaArgs);
  va_end(vaArgs);
  
  if (m_pJob.bIsNotNull()) {
    m_pJob->vDotPrint(sFormatted);
  }
  else
  {
    m_pOp->vDotPrint(sFormatted);
  }

}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  AM::MASTERLOCK::vGrabTraceMulti();
  
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

  vTrace("m_pRecoveryWord=%s %p\n"_Stz,
         m_pRecoveryWord ? m_pRecoveryWord->szWordNm() : C_NO_NAME,
         m_pRecoveryWord);

  m_pDataStack->vDump(&m_oDicStack,  "m_pDataStack:"_Stz);
  m_pReturnStack->vDump(&m_oDicStack,"m_pReturnStack:"_Stz);
  m_pAuxStack->vDump(&m_oDicStack,   "m_pAuxStack:"_Stz);

  if (m_pLocalsFrame)
    m_pLocalsFrame->vDump();
  else
    vTrace("LocalsChain: none\n"_Stz);
  
  m_oDicStack.vContentDump();
  
  if (pWord)
    AM::WORD::vDisassemble(pWord,&m_oDicStack);


  // dump the logicUnit registers
  // revised job/op model
  if (m_pLogicUnit)
    m_pLogicUnit->vDumpRegisters();
  
  vTrace("== End Task Dump ============================================\n"_Stz);
  
  AM::MASTERLOCK::vDropTraceMulti();
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
/* get event hander                                                           */
/*----------------------------------------------------------------------------*/
Word* Task::pEventHandler(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  Word* pEventHandler = nullptr;
  CodeCell* aFip = nullptr;
  Word* pWord = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pWord = m_oDicStack.pLocateRoutine(m_aInxPtr);
  if (pWord &&
      pWord->pCategory() &&
      pWord->pCategory()->szWordNm()->bSame("\\handler"_Stz)) {
    pEventHandler = pWord;
    
    return pEventHandler;
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nItems = m_pReturnStack->nValues();
  for (i8 i_Item = nItems; i_Item >= 0; i_Item--) {
    TypedCell* aPeekedItem = m_pReturnStack->aPeekItem(i_Item);
    eCN eType = aPeekedItem->eType();
    if (CN_FIPVAL != eType)
      continue;
    
    aFip = aPeekedItem->aFip();
    pWord = m_oDicStack.pLocateRoutine(aFip);
    
    if (pWord &&
        pWord->pCategory() &&
        pWord->pCategory()->szWordNm()->bSame("\\handler"_Stz)) {
      pEventHandler = pWord;
      break;
    }
  }
  
//  if (! pEventHandler) {
//    m_pReturnStack->vDump(&m_oDicStack,
//                          "ReturnStack:"_Stz);
//
//  }
  
  return pEventHandler;
}

/*----------------------------------------------------------------------------*/
/* drop all locks held by this task                                           */
/*----------------------------------------------------------------------------*/
void Task::vDropAllLocks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  i8 i_Item;
  Lock* p_Lock;

  // drop write locks
  while (C_UNKNOWN != (i_Item = m_oWriteLocks.iFirst())) {
    p_Lock = m_oWriteLocks.pGet(i_Item);
    p_Lock->vDropWrite();
  }
  
  // drop read locks
  while (C_UNKNOWN != (i_Item = m_oReadLocks.iFirst())) {
   p_Lock = m_oReadLocks.pGet(i_Item);
   p_Lock->vDropRead();
  }
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vDropTaskLock(Stz* szFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Stz* szCaller = MY::szCallerNm();
  
//  if (m_sTaskNm.bSame("supervisor"_Stz)) {
//
//    vTrace("777777777777777777777777777777777777777777777777777 DROPPING\n"_Stz);
//    vTrace("vDropTaskLock: %s by %s func %s\n"_Stz
//           ,m_sTaskNm.szStz()
//           ,szCaller
//           ,szFunc
//           );
//
//    HeadsUp oHeadsUp((Stz*)__func__);
//
//    vThreadCallStackDump();
//    vTrace("7777777777777777777777777777777777777777777777777777 DROPPED\n"_Stz);
//    vTrace("\n"_Stz);
//  }
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vGrabTaskLock(Stz* szFunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Stz* szCaller = MY::szCallerNm();

  m_okLock.vGrabWrite();


//  if (m_sTaskNm.bSame("supervisor"_Stz)) {
//
//    vTrace("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL GRABBING\n"_Stz);
//    vTrace("vGrabTaskLock: %s by %s func %s\n"_Stz
//           ,m_sTaskNm.szStz()
//           ,szCaller
//           ,szFunc
//           ,szFunc
//           );
//
//    HeadsUp oHeadsUp((Stz*)__func__);
//
//    vThreadCallStackDump();
//    vTrace("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL GRABBED\n"_Stz);
//
//    vTrace("\n"_Stz);
//  }
//
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
/* get the invoking task (super-op/pipe) of this task (sub-op/pipe)           */
/*----------------------------------------------------------------------------*/
Task* Task::pInvokingTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pInvokingTask;
}

/*----------------------------------------------------------------------------*/
/* set the invoking task (super-op/pipe) of this task (sub-op/pipe)           */
/*----------------------------------------------------------------------------*/
void Task::vInvokingTask(Task* pInvokingTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  RASSERT(pInvokingTask);
  m_pInvokingTask = pInvokingTask;
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

  RASSERT(pJob);
  m_pJob = pJob;
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
/* push a locals frame                                                        */
/*----------------------------------------------------------------------------*/
LogicUnit* Task::pLogicUnit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  return m_pLogicUnit;
}

/*----------------------------------------------------------------------------*/
/* new input                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vNewInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  m_pInput = new Input(m_pInput);
}

/*----------------------------------------------------------------------------*/
/* get the oo of this task                                                    */
/*----------------------------------------------------------------------------*/
Op* Task::pOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pOp;
}

/*----------------------------------------------------------------------------*/
/* set the op of this task                                                    */
/*----------------------------------------------------------------------------*/
void Task::vOp(Op* pOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  m_pOp = pOp;
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
/* get read lock set                                                          */
/*----------------------------------------------------------------------------*/
LockSet* Task::pReadLocks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  return & m_oReadLocks;
}

/*----------------------------------------------------------------------------*/
/* reset sen execution context                                                */
/*----------------------------------------------------------------------------*/
void Task::vSetSenRecoverContext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  m_pAuxStack->vReset();
  m_pDataStack->vReset();
  
  //Task::vSeeFrameStack();
  
  while (m_pReturnStack->nValues() > 2) {
    if (m_pLocalsFrame
        && m_pLocalsFrame->aDiscardPoint() ==
             (TypedCell*)m_pReturnStack->aTopVal())
      vLocalsPop();
    m_pReturnStack->vDiscardTop();
  }
  
  //Task::vSeeFrameStack();
  

  m_aInxPtr = m_pRecoveryWord->aFirstCell();
}

/*----------------------------------------------------------------------------*/
/* release all locks in the lock sets                                         */
/*----------------------------------------------------------------------------*/
void Task::vReleaseLockSets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  SP_Lock spLock;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // drop all read locks held by this task
  
  if (AM::SHOW::bException())
    vTrace("Release Read Lock Set\n"_Stz);
  
  for (i8 i_Lock = m_oReadLocks.iFirst();
       C_UNKNOWN != i_Lock;
       i_Lock = m_oReadLocks.iNext(i_Lock)) {
    Lock* pLock = m_oReadLocks.pGet(i_Lock);
    
    if (AM::SHOW::bException())
      vTrace("  Releasing read lock %p\n"_Stz,spLock.sz(pLock));
    
    pLock->vDropRead();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // drop all write locks held by this task

  if (AM::SHOW::bException())
    vTrace("Release Write Lock Set\n"_Stz);
  
  for (i8 i_Lock = m_oWriteLocks.iFirst();
       C_UNKNOWN != i_Lock;
       i_Lock = m_oWriteLocks.iNext(i_Lock)) {
    Lock* pLock = m_oWriteLocks.pGet(i_Lock);
    
    if (AM::SHOW::bException())
      vTrace("  Releasing write lock %p\n"_Stz,spLock.sz(pLock));
    
    pLock->vDropWrite();
  }
}

/*----------------------------------------------------------------------------*/
/* reset sen execution context                                                */
/*----------------------------------------------------------------------------*/
void Task::vResetSenContext(void)
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
/* retire                                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRetire(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RASSERT(QK_NONE == eQueue());
  RASSERT(TR_NONE == eTransition());

  RASSERT(TS_DESISTED == eState());
  
  vState(TS_RETIRED);
  
  AM::TASK::vRetire(this);
}

/*----------------------------------------------------------------------------*/
/* get save path stack                                                        */
/*----------------------------------------------------------------------------*/
DicStack* Task::pSaveDicStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  return & m_oSaveDicStack;
}

/*----------------------------------------------------------------------------*/
/* see event handler frames (aka return stack)                                */
/*----------------------------------------------------------------------------*/
void Task::vSeeFrameStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  SP_Word spWord;
  
  Word* pWord = m_oDicStack.pLocateRoutine(m_aInxPtr);                   // WORD

  vTrace("\n"_Stz,spWord.sz(pWord));
  vTrace("Current Frame =%s\n"_Stz,spWord.sz(pWord));
  vTrace("\n"_Stz,spWord.sz(pWord));

  m_pReturnStack->vDump(&m_oDicStack,"Frame Stack"_Stz);
}

/*----------------------------------------------------------------------------*/
/* see lock sets                                                              */
/*----------------------------------------------------------------------------*/
void Task::vSeeLockSets(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  SP_Lock spLock;
  
  vTrace("Read Lock Set\n"_Stz);
  for (i8 i_Lock = m_oReadLocks.iFirst();
       C_UNKNOWN != i_Lock;
       i_Lock = m_oReadLocks.iNext(i_Lock)) {
    Lock* pLock = m_oReadLocks.pGet(i_Lock);
    vTrace("  Read Lock %p\n"_Stz,spLock.sz(pLock));
  }

  vTrace("Write Lock Set\n"_Stz);
  for (i8 i_Lock = m_oWriteLocks.iFirst();
       C_UNKNOWN != i_Lock;
       i_Lock = m_oWriteLocks.iNext(i_Lock)) {
    Lock* pLock = m_oWriteLocks.pGet(i_Lock);
    vTrace("  Write Lock %p\n"_Stz,spLock.sz(pLock));
  }
}

/*----------------------------------------------------------------------------*/
/* see lock sets                                                              */
/*----------------------------------------------------------------------------*/
void Task::vSeeLogicUnitRegisters(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  m_pLogicUnit->vDumpRegisters();
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
/* set the tasks instruction pointer to run the entry word                    */
/*----------------------------------------------------------------------------*/
void Task::vSetInxPtr(Stz* szEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Dic* pDicRoot = AM::DIC::pRoot();
  
  Word* pEntryWord = pDicRoot->pFindWord(szEntry);
  if (! pEntryWord) {
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Unable to find task entry point: %s\n"_Stz,szEntry);
  }
  
  vSetInxPtr(pEntryWord);
}

/*----------------------------------------------------------------------------*/
/* set the tasks instruction pointer to run the entry word                    */
/*----------------------------------------------------------------------------*/
void Task::vSetInxPtr(Word* pEntryWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  m_aInxPtr = pEntryWord->aFirstCell();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Dic* pDicRoot = AM::DIC::pRoot();
  
  Word* pRecoveryWord = pDicRoot->pFindWord("std_recoveryWord"_Stz);
  if (pRecoveryWord) {
    m_pRecoveryWord = pRecoveryWord;
  }
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
    case TS_COMPLETED: pStats->Task.m_nCompleted++; break;
    case TS_EXECUTING: pStats->Task.m_nExecuting++; break;
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
  
  return * m_xTaskId;
}

/*----------------------------------------------------------------------------*/
/* get task name                                                              */
/*----------------------------------------------------------------------------*/
Stz* Task::szTaskNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_sTaskNm;
}

/*----------------------------------------------------------------------------*/
/* set task name                                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNm(Stz* szTaskNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_sTaskNm = szTaskNm;
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
        
         AM::NODE::szNodeNm(),
         AM::NODE::iThisNodeId(),
         szTaskNm(),
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
eTR Task::eTransition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eTransition;
}

/*----------------------------------------------------------------------------*/
/* clear scheduler action                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTranstionClear()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eTransition = TR_NONE;
}

/*----------------------------------------------------------------------------*/
/* get wake up                                                                */
/*----------------------------------------------------------------------------*/
i8 Task::iWakeup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  u8 uSecond = m_oWakeup.m_wWakeup.T.m_uSecond;
  u8 uMicro  = m_oWakeup.m_wWakeup.T.m_uMicro;

  i8 iWakeup = uSecond * C_1M + uMicro;
  
  return iWakeup;
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
/* get wakeup                                                                 */
/*----------------------------------------------------------------------------*/
Wakeup& Task::oWakeup(void)
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
/* get write lock set                                                         */
/*----------------------------------------------------------------------------*/
LockSet* Task::pWriteLocks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  return & m_oWriteLocks;
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
/* compare two tasks by their wakeup time                                     */
/*----------------------------------------------------------------------------*/
bool bTaskCompareWakeup(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  Task* pInsertEntry = _pTask(pObjectInsert);
  Task* pScanEntry   = _pTask(pObjectScan);
  
  RASSERT(pScanEntry->eGetClassNo() == CN_TASK);
  RASSERT(pInsertEntry->eGetClassNo() == CN_TASK);
  
  if ( ! pObjectScan )
    return false;
  
  if (pInsertEntry->iWakeup() == pScanEntry->iWakeup())
    bMatch = true;
  else
    bMatch = false;
  
  if (pInsertEntry->iWakeup() > pScanEntry->iWakeup())
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
