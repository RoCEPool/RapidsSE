/*============================================================================*/
/* Thread.cpp -- thread                                                       */
/*============================================================================*/

#include "BASE.h"

#include "PsxThd.hpp"  //?
#include "TagConstants.h"

Atomic gixThread;

extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Thread,CN_THREAD)
__aNewFunction(Thread,CN_THREAD)
__vDeleteFunction(Thread,CN_THREAD)
__aFooterFunction(Thread)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Thread::Thread(void)
: Object(CN_THREAD)
, m_oMutex(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  // m_xThreadId
  
  m_eDedicated = CN_NONE;
  m_eThreadState = HS_NONE;

  m_pPsxThd = new PsxThd(this);
  m_pTask = nullptr;
  
  m_pObject0 = nullptr;
  m_pObject1 = nullptr;
  
  // m_oMutex

  m_nAssignedTasks = 0;
  
  m_pThreadScratch = new ThreadScratch;

  _checkInitialised_(CN_THREAD);
  
  m_sThreadNm.vSprint("Thread_%02lld"_Stz,iThreadNo());
  
  ++ gixThread;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Thread::~Thread(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  //RASSERT(! m_pObject0);
  //RASSERT(! m_pObject1);

  delete m_pPsxThd;
  
  delete m_pThreadScratch;
  
  -- gixThread;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dedicated thread entry point                                               */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint_dedicated(void* pArg)
{
  try {
    
    Thread* pThread = _pThread(pArg);
    RASSERT(pThread);
    
    pThread->vThreadEntry();
    pThread->m_pPsxThd->vThreadEntry();
    
    Scheduler::vThread_AddDedicatedThread(pThread);
    
    if (AM::TRACE::bVerbose(C_VB_THREAD))
      vTrace("Thread::aEntryPoint_dedicated %p %lld\n"_Stz,
             pThread,pThread->iThreadNo());
    
    //pThread->vBindCore();
    
    // -- -- -- -- -- -- -- -- -- --

    pThread->vLoop_dedicated();
    
    // -- -- -- -- -- -- -- -- -- --
    
    if (AM::TRACE::bVerbose(C_VB_THREAD))
      vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
             pThread,pThread->iThreadNo());
        
    Scheduler::vThread_RemoveDedicatedThread(pThread);
    
    pThread->m_pPsxThd->vThreadExit();
    
    AM::THREAD::vDeleteThread(pThread);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  catch (Exception xException) {
    
        fflush(stderr); fflush(stdout);
    printf("Thread-level exception caught in func %s() file %s line %d\n",
           __func__,__FILE__,__LINE__);
        fflush(stderr); fflush(stdout);
    xException.vPrint();
        fflush(stderr); fflush(stdout);
    
    Process::vExitFailure();
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* general thread entry point                                                 */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint_general(void* pArg)
{
  //try {
    
    Thread* pThread = _pThread(pArg);
    RASSERT(pThread);

    pThread->vThreadEntry();
    pThread->m_pPsxThd->vThreadEntry();

    Scheduler::vThread_AddGeneralThread(pThread);
    
    if (AM::TRACE::bVerbose(C_VB_THREAD))
      vTrace("    Thread::aEntryPoint_general %p %lld\n"_Stz,
             pThread,pThread->iThreadNo());
        
    //pThread->vBindCore();
    
    // -- -- -- -- -- -- -- -- -- --
    
    pThread->vLoop_general();
    
    // -- -- -- -- -- -- -- -- -- --

    if (AM::TRACE::bVerbose(C_VB_THREAD))
      vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
             pThread,pThread->iThreadNo());
        
    Scheduler::vThread_RemoveGeneralThread(pThread);
    
    pThread->m_pPsxThd->vThreadExit();

    AM::THREAD::vDeleteThread(pThread);
  //}
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
//  catch (Exception xException) {
//
//        fflush(stderr); fflush(stdout);
//    printf("Thread-level exception caught in func %s() file %s line %d\n",
//           __func__,__FILE__,__LINE__);
//        fflush(stderr); fflush(stdout);
//    xException.vPrint();
//        fflush(stderr); fflush(stdout);
//
//    Process::vExitFailure();
//  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dummy thread entry point                                                   */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint_dummy(void* pArg)
{
  try {
    
    Thread* pThread = _pThread(pArg);
    RASSERT(pThread);

    pThread->vSetIdle();
    
    pThread->m_pPsxThd->vThreadEntry();
    
    if (AM::TRACE::bVerbose(C_VB_THREAD))
      vTrace("Thread::aEntryPoint_dummy %p %lld\n"_Stz,
             pThread,pThread->iThreadNo());
    
    while (true) {
      
      // sleep for a bit to generate a context switch
      pThread->m_pPsxThd->vSleepUsec(AM::nDummySleep(),CN_THREAD);
      
      // burn some cpu
      i8 iSum = 0;
      for (i8 i_Loop = 0; i_Loop < AM::nDummyLoop(); i_Loop++)
        iSum += TickingDevice::iGetTick();
    }
    
    //pThread->m_pPsxThd->vThreadExit();
    
    //AM::THREAD::vDeleteThread(pThread);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  catch (Exception xException) {
    
        fflush(stderr); fflush(stdout);
    printf("Thread-level exception caught in func %s() file %s line %d\n",
           __func__,__FILE__,__LINE__);
        fflush(stderr); fflush(stdout);
    xException.vPrint();
        fflush(stderr); fflush(stdout);
    
    Process::vExitFailure();
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get this thread                                                            */
/*----------------------------------------------------------------------------*/
Thread* Thread::pThisTLSThread(void)
{
  if (C_UNKNOWN == (i8)giTLS_ThreadKey)
    return nullptr;
  
  Thread* pThread = static_cast<Thread*>(PsxThd::pTlsGet(giTLS_ThreadKey));

  if (C_UNKNOWN == (i8)pThread)
    return nullptr;

  _pThread(pThread);
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dedicated thread loop                                                      */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_dedicated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Scheduler::vThread_BeginThreadLoop(this);

  Scheduler::vTaskApi_BeginExecutingDedicated(this);
  
  m_eThreadState = HS_ASSIGNED;
  
  while (m_pTask && (m_pTask->pOp() || m_pTask->pJob()))
    m_pTask->vTaskLoop();

  Scheduler::vThread_EndThreadLoop(this);

  vTrace("Exiting dedicated thread loop\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* general thread loop                                                        */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_general(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
        RASSERT(HS_IDLE == m_eThreadState);
        RASSERT(! m_pTask);
  Scheduler::vThread_BeginThreadLoop(this);

        RASSERT(HS_UNASSIGNED == m_eThreadState);
        RASSERT(! m_pTask);
  Scheduler::vThread_WaitForTaskAssignment(this);
  
        RASSERT(HS_ASSIGNED == m_eThreadState);
        RASSERT(m_pTask);
        RASSERT(TR_NONE == m_pTask->eTransition());
        RASSERT(this == m_pTask->pvThread_DONT_DEREF());

  for (;;) {

                RASSERT(HS_ASSIGNED == m_eThreadState);
                ASSERT(m_pTask);
                ASSERT(TR_NONE == m_pTask->eTransition());
                RASSERT(this == m_pTask->pvThread_DONT_DEREF());
        
    m_pTask->vTaskLoop();
    
                RASSERT(HS_ASSIGNED == m_eThreadState);
                ASSERT(m_pTask);
                ASSERT(TR_NONE != m_pTask->eTransition());
                RASSERT(this == m_pTask->pvThread_DONT_DEREF());
    Scheduler::vTaskSelf_LoopTransition(m_pTask,this);

    if (m_pTask) {
      RASSERT(HS_ASSIGNED == m_eThreadState);
      RASSERT(m_pTask);
      continue;
    }
    
                RASSERT(HS_UNASSIGNED == m_eThreadState);
                RASSERT(! m_pTask);
    Scheduler::vThread_WaitForTaskAssignment(this);
    
                RASSERT(HS_ASSIGNED == m_eThreadState);
                RASSERT(m_pTask);
                RASSERT(TR_NONE == m_pTask->eTransition());
                RASSERT(this == m_pTask->pvThread_DONT_DEREF());
  }
     
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* get number of tasks assigned so far                                        */
/*----------------------------------------------------------------------------*/
i8 Thread::nAssignedTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_nAssignedTasks;
}

/*----------------------------------------------------------------------------*/
/* bind the os/posix thread to given core                                     */
/*----------------------------------------------------------------------------*/
void Thread::vBind(i8 iCore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bBindThread(iCore);
  ASSERT(bResult);
  UNUSED(bResult);
}

/*----------------------------------------------------------------------------*/
/* bind calling thread to a core                                              */
/*----------------------------------------------------------------------------*/
void Thread::vBindCore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  i8 nCores = Hardware::iCoreCount();
  
  i8 iBindToCore = nCores - iThreadNo();
  
  vTrace("Thread %lld binding to core %lld\n"_Stz,iThreadNo(),iBindToCore);
  
  vBind(iBindToCore);
}

/*----------------------------------------------------------------------------*/
/* get the cpu this thread is bound to                                        */
/*----------------------------------------------------------------------------*/
i8 Thread::iCpu(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_pPsxThd->iThreadCpu();
}

/*----------------------------------------------------------------------------*/
/* thread dump                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vContentDump(void)
{
  vTrace("Thread\n"_Stz);
  vTrace("------\n"_Stz);
         
  vTrace("%p: m_eThreadState=%s m_eDedicated=%s m_nAssignedTasks=%lld\n"_Stz,
         this,
         
         aHS[m_eThreadState],
         aCN[m_eDedicated],
         m_nAssignedTasks,
         
         C_VARGS_END );
}

/*----------------------------------------------------------------------------*/
/* get dedicated                                                              */
/*----------------------------------------------------------------------------*/
eCN Thread::eDedicated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_eDedicated;
}

/*----------------------------------------------------------------------------*/
/* set dedicated                                                              */
/*----------------------------------------------------------------------------*/
void Thread::vDedicated(eCN eDedicated)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_eDedicated = eDedicated;
}

/*----------------------------------------------------------------------------*/
/* get mutex                                                                  */
/*----------------------------------------------------------------------------*/
Mutex* Thread::pMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  return & m_oMutex;
}

/*----------------------------------------------------------------------------*/
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Thread::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
}

/*----------------------------------------------------------------------------*/
/* get general purpose object 0                                               */
/*----------------------------------------------------------------------------*/
Object* Thread::pObject0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  return m_pObject0;
}

/*----------------------------------------------------------------------------*/
/* set general purpose object 0                                               */
/*----------------------------------------------------------------------------*/
void Thread::vObject0(Object* pObject0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  m_pObject0 = pObject0;
}

/*----------------------------------------------------------------------------*/
/* get general purpose object 1                                               */
/*----------------------------------------------------------------------------*/
Object* Thread::pObject1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  return m_pObject1;
}

/*----------------------------------------------------------------------------*/
/* set general purpose object 1                                               */
/*----------------------------------------------------------------------------*/
void Thread::vObject1(Object* pObject1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  m_pObject1 = pObject1;
}

/*----------------------------------------------------------------------------*/
/* get posix thread                                                           */
/*----------------------------------------------------------------------------*/
PsxThd* Thread::pPsxThd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  return m_pPsxThd;
}

/*----------------------------------------------------------------------------*/
/* get thread scratch area                                                    */
/*----------------------------------------------------------------------------*/
ThreadScratch* Thread::pThreadScratch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  return m_pThreadScratch;
}

/*----------------------------------------------------------------------------*/
/* set thread state as idle                                                   */
/*----------------------------------------------------------------------------*/
void Thread::vSetIdle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_eThreadState = HS_IDLE;
}

/*----------------------------------------------------------------------------*/
/* spawn a os/posix pthread associated with this thread                       */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPsxThd(void* pFun(void*), void* pArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bSpawnThread(pFun,pArg);
  UNUSED(bResult);

  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this thread                     */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPsxThd_dedicated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bSpawnThread(Thread::aEntryPoint_dedicated,this);
  UNUSED(bResult);

  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this dummy thread               */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPsxThd_dummy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bSpawnThread(Thread::aEntryPoint_dummy,this);
  UNUSED(bResult);

  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this thread                     */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPsxThd_general(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bSpawnThread(Thread::aEntryPoint_general,this);
  UNUSED(bResult);

  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* get current task                                                           */
/*----------------------------------------------------------------------------*/
Task* Thread::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set current task                                                           */
/*----------------------------------------------------------------------------*/
void Thread::vTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pTask = pTask;
  
  if (m_pTask) {
    m_eThreadState = HS_ASSIGNED;
    m_nAssignedTasks++;
  }
}

/*----------------------------------------------------------------------------*/
/* set this thread into TLS of current pthread                                */
/*----------------------------------------------------------------------------*/
void Thread::vThisTLSThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  PsxThd::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PsxThd::pTlsGet(giTLS_ThreadKey) == this);
}

/*----------------------------------------------------------------------------*/
/* thread entry intialisation                                                 */
/*----------------------------------------------------------------------------*/
void Thread::vThreadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* get thread nm                                                              */
/*----------------------------------------------------------------------------*/
Stz* Thread::szThreadNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
 
  return m_sThreadNm;
}

/*----------------------------------------------------------------------------*/
/* get our thread number                                                      */
/*----------------------------------------------------------------------------*/
i8 Thread::iThreadNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
 
  return BitLib::uMaskOffUpper16(* m_xThreadId);
}

/*----------------------------------------------------------------------------*/
/* get thread state                                                           */
/*----------------------------------------------------------------------------*/
eHS Thread::eThreadState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_eThreadState;
}

/*----------------------------------------------------------------------------*/
/* set thread state                                                           */
/*----------------------------------------------------------------------------*/
void Thread::vThreadState(eHS eThreadState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_eThreadState = eThreadState;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
