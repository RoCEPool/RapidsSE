/*============================================================================*/
/* Thread.cpp -- thread                                                       */
/*============================================================================*/

#include "Threads.h"
#include "Scheduler.hpp"
#include "AM.hpp"

Atomic gixThread;

extern pthread_key_t giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Thread* Threads::_pThread(void* aAddr)
{
  Thread* pThread = reinterpret_cast<Thread*>(aAddr);
  
  RASSERT_TYPE(pThread,CN_THREAD);
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Thread::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_THREAD);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new a thread from existing memory                                          */
/*----------------------------------------------------------------------------*/
void* Thread::operator new(size_t, Thread* pThread)
{
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Thread::operator delete(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Thread::Thread(void)
: Focal(CN_THREAD,AM::iNextThreadId(),C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_eThreadState = HS_NONE;
  m_eDedicated   = CN_NONE;
  m_pMutex       = new Mutex(MK_THREAD,this,nullptr);

  m_pTask        = nullptr;
  m_nAssignedTasks = 0;

  m_pPsxThd     = new PsxThd;
  
  _checkInitialised_(CN_THREAD);
  
  vIncActNo(); ++ gixThread;
  AM::vLogThd(SL_INF,this,HA_THD_CONSTRUCT,
              "Thread construct : %p (%lld threads exist)"_Stz, //!format-LogThd
              this,* gixThread);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Thread::~Thread(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  delete m_pMutex;
  delete m_pPsxThd;
  
  vIncActNo(); -- gixThread;
  AM::vLogThd(SL_INF,this,HA_THD_DESTRUCT,
              "Thread destruct : %p (%lld threads exist)"_Stz, //!format-LogThd
              this,* gixThread);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* thread entry point                                                         */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint_dedicated(void* pArg)
{
  try {
    
    Thread* pThread = _pThread(pArg);
    RASSERT(pThread);
    
    pThread->vThreadEntry();
    pThread->m_pPsxThd->vThreadEntry();
    
    Scheduler::vAddDedicatedThread(pThread);
    
    if (AM::bVerbose())
      vTrace("Thread::aEntryPoint_dedicated %p %lld\n"_Stz,
             pThread,pThread->iThreadNo());
    
    pThread->m_pPsxThd->vIncActNo();
    AM::vLogRes(SL_INF,pThread->m_pPsxThd,RA_THD_ENTRY,
                "dedicated thead entry: %p"_Stz, //!format-LogRes
                pThread);

    //pThread->vBindCore();
    
    // -- -- -- -- -- -- -- -- -- --

    pThread->vLoop_dedicated();
    
    // -- -- -- -- -- -- -- -- -- --
    
    if (AM::bVerbose())
      vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
             pThread,pThread->iThreadNo());
    
    pThread->m_pPsxThd->vIncActNo();
    AM::vLogRes(SL_INF,pThread->m_pPsxThd,RA_THD_EXIT,
                "Thead exit (dedicated) : %p"_Stz, //!format-LogRes
                pThread);
    
    Scheduler::vRemoveDedicatedThread(pThread);
    
    pThread->m_pPsxThd->vThreadExit();
    
    delete pThread;
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
/* thread entry point                                                         */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint_general(void* pArg)
{
  try {
    
    Thread* pThread = _pThread(pArg);
    RASSERT(pThread);

    pThread->vThreadEntry();
    pThread->m_pPsxThd->vThreadEntry();

    Scheduler::vAddGeneralThread(pThread);
    
    if (AM::bVerbose())
      vTrace("Thread::aEntryPoint_general %p %lld\n"_Stz,
             pThread,pThread->iThreadNo());
    
    pThread->m_pPsxThd->vIncActNo();
    AM::vLogRes(SL_INF,pThread->m_pPsxThd,RA_THD_ENTRY,
                "general thead entry: %p"_Stz, //!format-LogRes
                pThread);
    
    //pThread->vBindCore();
    
    // -- -- -- -- -- -- -- -- -- --
    
    pThread->vLoop_general();
    
    // -- -- -- -- -- -- -- -- -- --

    if (AM::bVerbose())
      vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
             pThread,pThread->iThreadNo());
    
    pThread->m_pPsxThd->vIncActNo();
    AM::vLogRes(SL_INF,pThread->m_pPsxThd,RA_THD_EXIT,
                "Thead exit (general) : %p"_Stz, //!format-LogRes
                pThread);
    
    Scheduler::vRemoveGeneralThread(pThread);
    
    pThread->m_pPsxThd->vThreadExit();

    delete pThread;
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
/* dummy thread entry point                                                   */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint_dummy(void* pArg)
{
  try {
    
    Thread* pThread = _pThread(pArg);
    RASSERT(pThread);

    pThread->m_eThreadState = HS_IDLE;
    
    pThread->m_pPsxThd->vThreadEntry();
    
    if (AM::bVerbose())
      vTrace("Thread::aEntryPoint_dummy %p %lld\n"_Stz,
             pThread,pThread->iThreadNo());
    
    while (true) {
      
      // sleep for a bit to generate a context switch
      pThread->m_pPsxThd->vSleepUsec(AM::nDummySleep(),CN_THREAD);
      
      // burn some cpu
      i8 iSum = 0;
      for (i8 i_Loop = 0; i_Loop < AM::nDummyLoop(); i_Loop++)
        iSum += Tick::iRdTscp();
    }
    
    //pThread->m_pPsxThd->vThreadExit();
    
    //delete pThread;
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
  Thread *pThread = static_cast<Thread*>(PsxThd::pTlsGet(giTLS_ThreadKey));
  
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
  
  Scheduler::vBeginThreadLoop(this);

  Scheduler::vExecuteDedicatedTask(m_pTask,this);
  
  while (m_pTask && m_pTask->pJob())
    m_pTask->vTaskLoop();

  Scheduler::vEndThreadLoop(this);

  vTrace("Exiting dedicated thread loop\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* general thread loop                                                        */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_general(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Scheduler::vBeginThreadLoop(this);
  
  for (;;) {
    
    // wait for task to be assigned to this thread
    if (! m_pTask)
      Scheduler::vWaitForTaskAssignment(this);
    ASSERT(m_pTask);
    
    // ensure the task has this thread
    m_pTask->vThread(this);
    
    // call the task loop
    m_pTask->vTaskLoop();

    // handle task return event
    Scheduler::vTaskLoopTransition(this);
    
    // either this thread no longer has a task
    //     or the task no longer has this thread
    ASSERT( (! m_pTask) || (! m_pTask->pThread()) );
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* bind the os/posix thread to given core                                     */
/*----------------------------------------------------------------------------*/
void Thread::vBind(i8 iCore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bBindThread(iCore);
  ASSERT(bResult);
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
         
         C_ENDARGS );
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Thread::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this thread                     */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPsxThd_dedicated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bSpawnThread(Thread::aEntryPoint_dedicated,this);
  
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this dummy thread               */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPsxThd_dummy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bSpawnThread(Thread::aEntryPoint_dummy,this);
  
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this thread                     */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPsxThd_general(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPsxThd->bSpawnThread(Thread::aEntryPoint_general,this);
  
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
/* get our thread number                                                      */
/*----------------------------------------------------------------------------*/
i8 Thread::iThreadNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
 
  return BitLib::uMaskOffUpper16(m_iUniqueId);
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Thread);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
