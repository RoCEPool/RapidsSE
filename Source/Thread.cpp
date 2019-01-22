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
/* threads are never delete (for the moment)                                  */
/*----------------------------------------------------------------------------*/
void Thread::operator delete(void*)
{
  vFatal("fatal: Thread class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Thread::Thread(void)
: Object(CN_THREAD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  static Atomic zThreadNo;
  
  m_iThreadNo    = ++zThreadNo;
  m_aStackBase   = nullptr;
  m_iStackSize   = 0;
  
  m_oLock.vInitLock(LK_THREAD);
  m_eThreadState = TC_NONE;
  m_pPosixThread = new PosixThread;
  m_pMutex   = new Mutex;

  m_pTask       = nullptr;

  Thread* pThread = this;
  
  m_iLastQueueRR  = QR_SLEEPING_QUEUE - 1;
  m_iStartQueueRR = QR_NONE;
  
  m_eDedicated = CN_NONE;
  
  _checkInitialised_(CN_THREAD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Thread::~Thread(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  delete m_pPosixThread;
  delete m_pMutex;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* thread entry point                                                         */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPoint(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  pThread->m_aStackBase = PosixThread::aStackBase();
  pThread->m_iStackSize = PosixThread::iStackSize();

  pThread->vThreadState(TC_IDLE);
  
  gpAttachMap->m_pQueues->vAddThread(pThread);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Thread::aEntryPoint %p %lld\n"_Stz,
           pThread,pThread->iThreadNo());
  
  if (gxArguments.m_bBindThreads)
    pThread->vBindCore();
    
  try {
    pThread->vLoop();
  }
  
  catch (Error* pError) {
    
    char wFileName[256];
    const char* ps;
    
    ps = StrLib::sLast(pError->aFileName()->aChar());
    bool bBackSlash = StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
    if (bBackSlash) {
      StrLib::vCopy(wFileName,ps+1);
    }
    else {
      StrLib::vCopy(wFileName, pError->aFileName()->aChar());
    }
    vFatal("ERROR: [ThreadLoop] %s: line %lld in %s\n"_Stz,
           pError->aErrorMsg(),
           pError->iLineNo(),
           wFileName);
  }
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Thread::aEntryPoint %p %lld exiting\n"_Stz,
           pThread,pThread->iThreadNo());
  
  gpAttachMap->m_pQueues->vRemoveThread(pThread);
  
  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dummy thread entry point                                                   */
/*----------------------------------------------------------------------------*/
void* Thread::aEntryPointDummy(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  pThread->vThreadState(TC_IDLE);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("Thread::pDummyEntryPoint %p %lld\n"_Stz,
           pThread,pThread->iThreadNo());
  
  while (true) {
    
    // sleep for a bit to generate a context switch
    PosixThread::vSleepMicro(gxArguments.m_nDummySleep);
    
    // burn some cpu
    i8 iSum = 0;
    for (i8 i_Loop = 0; i_Loop < gxArguments.m_nDummyLoop; i_Loop++)
      iSum += Synch::iRdTscp();
  }
  
  //delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get this thread                                                            */
/*----------------------------------------------------------------------------*/
Thread* Thread::pThisTLSThread(void)
{
  Thread *pThread = static_cast<Thread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* thread loop                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey) == this);
  
  vMutexLock();
  
  ++gpAttachMap->m_ziThreads;
  
  for (;;) {
    
    // get next task
    if (! m_pTask && ! bQueuePeek()) {
      
      // wait upto 5ms to be given a signal
      vHandlerTimedWait();
      
      // attempt again if no task
      if ( ! m_pTask)
        continue;
    }
    
    // will have a task
    ASSERT(m_pTask);
    
    // call the task
    vTaskCall();
    
    if (m_pTask->pJob() && m_pTask->pJob()->m_bMark)
      m_pTask->vWriteMark("Return from task call:"_Stz);
    
    // handle thread level event
    Handlers::vThreadLoop(&m_pTask);
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* bind the os/posix thread to given core                                     */
/*----------------------------------------------------------------------------*/
void Thread::vBind(i8 iCore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bBindThread(iCore);
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
  
  return m_pPosixThread->iThreadCpu();
}

/*----------------------------------------------------------------------------*/
/* thread dump                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vContentDump(void) const
{
  vTrace("Thread %p: OpSysThreadId %lld PosixThreadId %llx\n"_Stz,
         this,
         m_pPosixThread->m_iOpSysThreadId,
         m_pPosixThread->m_iPosixThreadId);
}

/*----------------------------------------------------------------------------*/
/* signal thread to execute given task                                        */
/*----------------------------------------------------------------------------*/
void Thread::vExecuteTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(m_pMutex->bMutexLock());
  
  // task should be ready
  RASSERT(TS_READY == pTask->eState());
  //pTask->vState(TS_EXECUTING);
  
  // thread should be idle
  //RASSERT(TC_IDLE == m_eThreadState);
  RASSERT(TC_RUNNING == m_eThreadState);
  
  // thread should have no task
  RASSERT(! m_pTask);
  
  // set task to be executed
  m_pTask = pTask;
  
  //m_eThreadState = TC_RUNNING;
  
  // signal the thread
  vSignalThread();
}

/*----------------------------------------------------------------------------*/
/* this thread waits till its given a task by a handler or wait time over     */
/*----------------------------------------------------------------------------*/
void Thread::vHandlerTimedWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // under mutex lock
  
  // will not have a task
  ASSERT(! m_pTask);
  
  // this thread's mutex should be locked
  ASSERT(m_pMutex->bMutexIsLocked());
  
  m_eThreadState = TC_IDLE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait 5ms
  
  vCondWait_Timed(5000); // while waiting the mutex is dropped

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // under mutex lock
  
  // this thread's mutex should be locked
  ASSERT(m_pMutex->bMutexIsLocked());
  
  // will have a task
  //ASSERT(*m_pTask);
  
  if (m_pTask)
    m_eThreadState = TC_RUNNING;
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
  RASSERT(TC_RUNNING == m_eThreadState);
  
  // thread should have no task
  RASSERT(! m_pTask);
  
  // set task to be executed
  m_pTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* advance to next queue in round robin sequence                              */
/*----------------------------------------------------------------------------*/
bool Thread::bRRnext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_iLastQueueRR++;
  
  if (m_iLastQueueRR > gpAttachMap->m_pQueues->i_QueueLast())
    m_iLastQueueRR = gpAttachMap->m_pQueues->i_QueueFirst();
  
  if (m_iLastQueueRR == m_iStartQueueRR)
    return false; // end of round robin sequence
  else {
    if (m_iLastQueueRR == gpAttachMap->m_pQueues->i_QueueFirst())
      m_iStartQueueRR = gpAttachMap->m_pQueues->i_QueueFirst();
    return true;  // have advanced to next queue
  }
}

/*----------------------------------------------------------------------------*/
/* start another round robin sequence                                         */
/*----------------------------------------------------------------------------*/
void Thread::vRRstart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
 
  m_iStartQueueRR = m_iLastQueueRR - 1;
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::aEntryPoint,this);
  
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn a dummy thread                                                       */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnDummyThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::aEntryPointDummy,this);
  
  ASSERT(bResult);
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
eTC Thread::eThreadState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_eThreadState;
}

/*----------------------------------------------------------------------------*/
/* set the thread state                                                       */
/*----------------------------------------------------------------------------*/
void Thread::vThreadState(eTC eThreadState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_eThreadState = eThreadState;
}

/*----------------------------------------------------------------------------*/
/* call the assocated task                                                    */
/*----------------------------------------------------------------------------*/
void Thread::vTaskCall(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_eThreadState = TC_RUNNING;
  ASSERT(m_pTask);
  
  PosixThread::vTlsSet(giTLS_TaskKey,m_pTask);
  ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
  
  RASSERT(TS_READY == m_pTask->eState() || TS_YIELDED == m_pTask->eState());
  m_pTask->vState(TS_EXECUTING);
  m_pTask->vThread(this);
  
  // TODO: shouldn't have to refind it everytime!!!
  m_pTask->vRefindJobClass();
  
  if ( ! m_pTask->bSeeded() ) {
    m_pTask->vSeedJob();
  }
  
  // add this task to the executing task queue
  gpAttachMap->m_pQueues->vAddToExecutingSet(m_pTask);
  
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
  if (! bMutexLock())
    return false;
  
  // if this thread idle return with mutex locked
  if (TC_IDLE == m_eThreadState) {
    m_eThreadState = TC_RUNNING;
    return true;
  }
  
  // thread is not idle, so unlock mutex
  vMutexUnlock();
  
  return false;
}


/*----------------------------------------------------------------------------*/
/*-- Mutex -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* signal a thread (should not be calling thread)                             */
/*----------------------------------------------------------------------------*/
void Thread::vSignalThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(m_pMutex->bMutexLock());

  m_pMutex->vCondSignal();
  m_pMutex->vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* condition broadcast                                                        */
/*----------------------------------------------------------------------------*/
void Thread::vCondBroadcast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pMutex->vCondBroadcast();
}

/*----------------------------------------------------------------------------*/
/* condition signal                                                           */
/*----------------------------------------------------------------------------*/
void Thread::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pMutex->vCondSignal();
}

/*----------------------------------------------------------------------------*/
/* condition wait (thread to wait will be calling thread)                     */
/*----------------------------------------------------------------------------*/
void Thread::vCondWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pMutex->vCondWait();
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (should be calling thread)                    */
/*----------------------------------------------------------------------------*/
void Thread::vCondWait_Timed(i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pMutex->vCondWait_Timed(iMicroSeconds);
}

/*----------------------------------------------------------------------------*/
/* lock a threads mutex (blocking)                                            */
/*----------------------------------------------------------------------------*/
void Thread::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pMutex->vMutexLock();
  ASSERT(m_pMutex->bMutexIsLocked());
}

/*----------------------------------------------------------------------------*/
/* try and lock a threads mutex (non-blocking)                                */
/*----------------------------------------------------------------------------*/
bool Thread::bMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool b = m_pMutex->bMutexLock();
  
  ASSERT( ! b || m_pMutex->bMutexIsLocked() );
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* unlock a threads mutex                                                     */
/*----------------------------------------------------------------------------*/
void Thread::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(m_pMutex->bMutexIsLocked());
  
  m_pMutex->vMutexUnlock();
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
