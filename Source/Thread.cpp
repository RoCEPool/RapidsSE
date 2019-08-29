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
/* new a thread from existing memory                                          */
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
/* delete a thread                                                            */
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
: Focal(CN_THREAD,AM::iNextThreadId())
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_aStackBase   = nullptr;
  m_iStackSize   = 0;
  
  m_eThreadState = HS_NONE;
  m_pTask        = nullptr;
  m_eDedicated   = CN_NONE;

  m_pPthread     = new Pthread;
  m_pMutexThread = new Mutex(MK_THREAD,this,nullptr);
    
  m_nAssignedTasks = 0;

  _checkInitialised_(CN_THREAD);
  
  ++ gixThread;
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
  
  delete m_pMutexThread;
  delete m_pPthread;
  
  -- gixThread;
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
  Thread* pThread = static_cast<Thread*>(pArg);
  
  pThread->vThreadEntry();
  pThread->m_pPthread->vThreadEntry();
  
  Scheduler::vAddDedicatedThread(pThread);
  
  if (AM::bVerbose())
    vTrace("Thread::aEntryPoint_dedicated %p %lld\n"_Stz,
           pThread,pThread->iThreadNo());
  
  AM::vLogRes(SL_INF,pThread->m_pPthread,RA_THD_ENTRY,
              "Thead entry (dedicated) : %p"_Stz, //!format-LogRes
              pThread);

  //pThread->vBindCore();
  
  try {
    pThread->vLoop_dedicated();
  }
  
  catch (Error* pError) {
    
    char caFileName[256];
    const char* ps;
    
    ps = StrLib::sLast(pError->aFileName()->aChar());
    bool bBackSlash = StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
    if (bBackSlash) {
      StrLib::vCopy(caFileName,ps+1);
    }
    else {
      StrLib::vCopy(caFileName, pError->aFileName()->aChar());
    }
    vFatal("ERROR: [ThreadLoop] %s: line %lld in %s\n"_Stz,
           pError->aErrorMsg(),
           pError->iLineNo(),
           caFileName);
  }
  
  if (AM::bVerbose())
    vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
           pThread,pThread->iThreadNo());
  
  AM::vLogRes(SL_INF,pThread->m_pPthread,RA_THD_EXIT,
              "Thead exit (dedicated) : %p"_Stz, //!format-LogRes
              pThread);
  
  Scheduler::vRemoveDedicatedThread(pThread);
  
  pThread->m_pPthread->vThreadExit();
  
  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* thread entry point                                                         */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint_general(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  pThread->vThreadEntry();
  pThread->m_pPthread->vThreadEntry();

  Scheduler::vAddGeneralThread(pThread);
  
  if (AM::bVerbose())
    vTrace("Thread::aEntryPoint_general %p %lld\n"_Stz,
           pThread,pThread->iThreadNo());
  
  AM::vLogRes(SL_INF,pThread->m_pPthread,RA_THD_ENTRY,
              "Thead entry (general) : %p"_Stz, //!format-LogRes
              pThread);
  
  //pThread->vBindCore();
    
  try {
    pThread->vLoop_general();
  }
  
  catch (Error* pError) {
    
    char caFileName[256];
    const char* ps;
    
    ps = StrLib::sLast(pError->aFileName()->aChar());
    bool bBackSlash = StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
    if (bBackSlash) {
      StrLib::vCopy(caFileName,ps+1);
    }
    else {
      StrLib::vCopy(caFileName, pError->aFileName()->aChar());
    }
    vFatal("ERROR: [ThreadLoop] %s: line %lld in %s\n"_Stz,
           pError->aErrorMsg(),
           pError->iLineNo(),
           caFileName);
  }
  
  if (AM::bVerbose())
    vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
           pThread,pThread->iThreadNo());
  
  AM::vLogRes(SL_INF,pThread->m_pPthread,RA_THD_EXIT,
              "Thead exit (general) : %p"_Stz, //!format-LogRes
              pThread);
  
  Scheduler::vRemoveGeneralThread(pThread);
  
  pThread->m_pPthread->vThreadExit();

  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dummy thread entry point                                                   */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint_dummy(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  pThread->m_eThreadState = HS_IDLE;
  
  pThread->m_pPthread->vThreadEntry();
  
  if (AM::bVerbose())
    vTrace("Thread::aEntryPoint_dummy %p %lld\n"_Stz,
           pThread,pThread->iThreadNo());
  
  while (true) {
    
    // sleep for a bit to generate a context switch
    pThread->m_pPthread->vSleepUsec(AM::nDummySleep(),CN_THREAD);
    
    // burn some cpu
    i8 iSum = 0;
    for (i8 i_Loop = 0; i_Loop < AM::nDummyLoop(); i_Loop++)
      iSum += Tick::iRdTscp();
  }
  
  //pThread->m_pPthread->vThreadExit();
  
  //delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get this thread                                                            */
/*----------------------------------------------------------------------------*/
Thread* Thread::pThisTLSThread(void)
{
  Thread *pThread = static_cast<Thread*>(Pthread::pTlsGet(giTLS_ThreadKey));
  
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
    m_pTask->vTaskLoop_job();

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
    
    // call the task
    if (TK_JOB == m_pTask->eKind())
      m_pTask->vTaskLoop_job();
    else
      m_pTask->vTaskLoop_op();

    // handle task return event
    Scheduler::vTaskLoopTransition(this);
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* bind the os/posix thread to given core                                     */
/*----------------------------------------------------------------------------*/
void Thread::vBind(i8 iCore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPthread->bBindThread(iCore);
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
  
  return m_pPthread->iThreadCpu();
}

/*----------------------------------------------------------------------------*/
/* thread dump                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vContentDump(void) const
{
  vTrace("Thread %p: OpSysThreadId %lld PthreadId %llx\n"_Stz,
         this,
         m_pPthread->m_iOpSysThreadId,
         m_pPthread->m_iPthreadId);
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
void Thread::vSpawnPthread_dedicated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPthread->bSpawnThread(Thread::aEntryPoint_dedicated,this);
  
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this dummy thread               */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPthread_dummy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPthread->bSpawnThread(Thread::aEntryPoint_dummy,this);
  
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this thread                     */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPthread_general(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPthread->bSpawnThread(Thread::aEntryPoint_general,this);
  
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
  Pthread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(Pthread::pTlsGet(giTLS_ThreadKey) == this);
}

/*----------------------------------------------------------------------------*/
/* thread entry intialisation                                                 */
/*----------------------------------------------------------------------------*/
void Thread::vThreadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  m_aStackBase = Pthread::aStackBase();
  m_iStackSize = Pthread::iStackSize();
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
