/*============================================================================*/
/* Thread.cpp -- thread                                                       */
/*============================================================================*/

#include "Threads.h"

#include "StorageEngine.h"

extern AttachMap*    gpAttachMap;
extern Arguments     gxArguments;

extern pthread_key_t giTLS_TaskKey;
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
: Object(CN_THREAD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  static Atomic ixThreadNo;
  
  m_iThreadNo    = ++ixThreadNo;
  m_aStackBase   = nullptr;
  m_iStackSize   = 0;
  
  m_eThreadState = TD_NONE;
  m_pTask        = nullptr;
  m_eDedicated   = CN_NONE;

  m_pPthread     = new Pthread;
  m_pMutexThread = new Mutex;
    
  m_nAssignedTasks = 0;

  _checkInitialised_(CN_THREAD);
  
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
  
  pThread->m_aStackBase = Pthread::aStackBase();
  pThread->m_iStackSize = Pthread::iStackSize();
  
  pThread->m_eThreadState = TD_IDLE;
  
  pThread->m_pPthread->vThreadEntry();
  
  Scheduler::vAddDedicatedThread(pThread);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Thread::aEntryPoint %p %lld\n"_Stz,
           pThread,pThread->iThreadNo());
  
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
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
           pThread,pThread->iThreadNo());
  
  gpAttachMap->m_pQueues->vRemoveDedicatedThread(pThread);
  
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
  
  pThread->m_aStackBase = Pthread::aStackBase();
  pThread->m_iStackSize = Pthread::iStackSize();

  pThread->m_eThreadState = TD_IDLE;
  
  pThread->m_pPthread->vThreadEntry();

  Scheduler::vAddGeneralThread(pThread);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Thread::aEntryPoint %p %lld\n"_Stz,
           pThread,pThread->iThreadNo());
  
  //pThread->vBindCore();
    
  try {
#ifdef WITH_EVENT_MODEL
    pThread->vLoop_event();
#else
    pThread->vLoop_polling();
#endif
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
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
           pThread,pThread->iThreadNo());
  
  gpAttachMap->m_pQueues->vRemoveGeneralThread(pThread);
  
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
  
  pThread->m_eThreadState = TD_IDLE;
  
  pThread->m_pPthread->vThreadEntry();
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Thread::aEntryPointDummy %p %lld\n"_Stz,
           pThread,pThread->iThreadNo());
  
  while (true) {
    
    // sleep for a bit to generate a context switch
    Pthread::vSleepUsec(gpAttachMap->Arg.m_nDummySleep,CN_THREAD);
    
    // burn some cpu
    i8 iSum = 0;
    for (i8 i_Loop = 0; i_Loop < gpAttachMap->Arg.m_nDummyLoop; i_Loop++)
      iSum += Tsc::iRdTscp();
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
/* dedicated thread loop (part of event-based model)                          */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_dedicated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Pthread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(Pthread::pTlsGet(giTLS_ThreadKey) == this);
  
  // thread has mutex while it is running
  m_eThreadState = TD_RUNNING;
  m_pMutexThread->vMutexLock();
  
  // increment m_ixThreads to indicate thread is running
  ++gpAttachMap->m_ixThreads;
  
  while (m_pTask && m_pTask->pJob())
    vTaskCall();
  
  m_pMutexThread->vMutexUnlock();
  
  vTrace("Exiting dedicated thread loop\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* event-based thread loop (fully event-based model)                          */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_event(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Pthread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(Pthread::pTlsGet(giTLS_ThreadKey) == this);
  
  // thread has mutex while it is running
  m_eThreadState = TD_RUNNING;
  m_pMutexThread->vMutexLock();

  // increment m_ixThreads to indicate thread is running
  ++gpAttachMap->m_ixThreads;
  
  for (;;) {
    
    // while waiting the mutex is dropped
    m_eThreadState = TD_IDLE;
    Scheduler::vIdleThread(this);
    m_pMutexThread->vCondWaitIndefinite();
    m_eThreadState = TD_RUNNING;

   // attempt again if no task
   if (! m_pTask)
     continue;
    
    // call the task
    vTaskCall();
    
    if (m_pTask->pJob() && m_pTask->pJob()->m_bMarkActivity)
      ActivityMap::vMarkJob(m_pTask->pJob(),
                            "Returned from task call: %s %p"_Stz,
                            aCN[m_pTask->pJob()->eClass()],
                            m_pTask->pJob());

    // handle task return event
    Scheduler::vTaskReturn(&m_pTask);
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* polling-based thread loop (original loop, which is limited cond wait)      */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_polling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Pthread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(Pthread::pTlsGet(giTLS_ThreadKey) == this);
  
  // thread has mutex while it is running
  m_eThreadState = TD_RUNNING;
  m_pMutexThread->vMutexLock();
  
  // increment m_ixThreads to indicate thread is running
  ++gpAttachMap->m_ixThreads;
  
  for (;;) {
    
    // get next task
    if (! m_pTask && ! bQueuePeek()) {
      
      // calc limited period to be given a signal
      i8 iWaitPeriodUsec = C_1K + Random::iInteger(0,C_10K);
      
      // while waiting the mutex is dropped
      m_eThreadState = TD_IDLE;
      m_pMutexThread->vCondWaitUsec(iWaitPeriodUsec);
      m_eThreadState = TD_RUNNING;
      
      // attempt again if no task
      if (! m_pTask)
        continue;
    }
    
    // call the task
    vTaskCall();
    
    if (m_pTask->pJob() && m_pTask->pJob()->m_bMarkActivity)
      ActivityMap::vMarkJob(m_pTask->pJob(),
                            "Returned from task call: %s %p"_Stz,
                            aCN[m_pTask->pJob()->eClass()],
                            m_pTask->pJob());
    
    // handle task return event
    Scheduler::vTaskReturn(&m_pTask);
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
  
  i8 iBindToCore = nCores - m_iThreadNo;
  
  vTrace("Thread %lld binding to core %lld\n"_Stz,m_iThreadNo,iBindToCore);
  
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
/* signal thread to execute given task                                        */
/*----------------------------------------------------------------------------*/
void Thread::vExecuteTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
    
  // task should be ready
  RASSERT(TS_READY == pTask->eState());
  //pTask->vState(TS_EXECUTING);
  
  // thread should be idle
  //RASSERT(TD_IDLE == m_eThreadState);
  RASSERT(TD_RUNNING == m_eThreadState);
  
  // thread should have no task
  RASSERT(! m_pTask);
  
  // set task to be executed
  m_pTask = pTask;
  
  //m_eThreadState = TD_RUNNING;
  
  // signal the thread
  //vSignalThread();
  
  m_pMutexThread->vCondSignal();
  m_pMutexThread->vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* attempt to get a task to run by peeking at the queues                      */
/*---------------------------------------------------------------------------*/
bool Thread::bQueuePeek(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Task* pPoppedTask;
  bool bPopped = gpAttachMap->m_pQueues->bPopQueuedTask(&pPoppedTask,this);
  
  if (! bPopped)
    return false;

  if (TS_READY != pPoppedTask->eState())
    pPoppedTask->vState(TS_READY);
  
  m_pTask = pPoppedTask;

  return bPopped;
}

/*----------------------------------------------------------------------------*/
/* re-execute same task (tried to sleep or yield but could continue)          */
/*----------------------------------------------------------------------------*/
void Thread::vReExecuteTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  // task should be ready or yielded
  RASSERT(TS_READY == pTask->eState() || TS_YIELDED == pTask->eState());
  //pTask->vState(TS_EXECUTING);
  
  // thread should be idle
  RASSERT(TD_RUNNING == m_eThreadState);
  
  // thread should have no task
  RASSERT(! m_pTask);
  
  // set task to be executed
  m_pTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix pthread associated with this thread                     */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnPthread_dedicated(Task *pTask)
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
/* get our thread number                                                      */
/*----------------------------------------------------------------------------*/
i8 Thread::iThreadNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
 
  return m_iThreadNo;
}

/*----------------------------------------------------------------------------*/
/* get thread state                                                           */
/*----------------------------------------------------------------------------*/
eTD Thread::eThreadState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_eThreadState;
}

/*----------------------------------------------------------------------------*/
/* call the assocated task                                                    */
/*----------------------------------------------------------------------------*/
void Thread::vTaskCall(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_eThreadState = TD_RUNNING;
  ASSERT(m_pTask);
  
  Pthread::vTlsSet(giTLS_TaskKey,m_pTask);
  ASSERT(Pthread::pTlsGet(giTLS_TaskKey));
  
  RASSERT(TS_READY == m_pTask->eState() || TS_YIELDED == m_pTask->eState());
  m_pTask->vState(TS_EXECUTING);
  m_pTask->vThread(this);
  
  // TODO: shouldn't have to refind it everytime!!!
  m_pTask->vRefindJobClass();
  
  if ( ! m_pTask->bSeeded() ) {
    m_pTask->vSeedJob();
  }
  
  // add this task to the executing task queue
  Scheduler::vAppendExecutingSet(m_pTask);
  
  // record time of call
  // ...
  
  m_pTask->vTaskLoop();

  // record time of return & calculate running total times
  // ...
}

/*----------------------------------------------------------------------------*/
/* attempt to grab this thread as an idle thread                             */
/*----------------------------------------------------------------------------*/
bool Thread::bGrabIfIdle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  // attempt to lock threads mutex
  if (! m_pMutexThread->bMutexLock())
    return false;
  
  // if this thread is idle return with mutex locked
  if (TD_IDLE == m_eThreadState) {
    m_eThreadState = TD_RUNNING;
    return true;
  }
  
  // thread is not idle, so unlock mutex
  m_pMutexThread->vMutexUnlock();

  return false;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Thread::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
