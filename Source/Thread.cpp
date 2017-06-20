/*============================================================================*/
/* Thread.cpp -- thread                                                       */
/*============================================================================*/

#include "Threads.h"

#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern ThreadList*  gpThreadList;
extern TaskList*    gpTaskList;

extern Scheduler*   gpScheduler;

extern NP_Task      gnpRootTask;

extern bool         gbVerbose;
extern bool         gbProgress;

extern pthread_key_t  giTLS_TaskKey;
extern pthread_key_t  giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Thread* Threads::_pThread(void* aAddr)
{
  Thread* pThread = reinterpret_cast<Thread*>(aAddr);
  
  RASSERT( !pThread || CN_THREAD == pThread->eClass() );
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* new a thread from existing memory                                          */
/*----------------------------------------------------------------------------*/
void* Thread::operator new(size_t iSize, Thread* pThread)
{
  RASSERT(iSize);
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* threads are never delete (for the moment)                                  */
/*----------------------------------------------------------------------------*/
void Thread::operator delete(void* pTxAddress)
{
  RASSERT(pTxAddress);
  vFatal("fatal: Thread class cannot be deleted"_Str);

  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Thread::Thread(void)
: Object(CN_THREAD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  ASSERT( gpThreadList );
  
  static Atomic zThreadNo;
  
  m_iThreadNo    = ++zThreadNo;
  m_sName        = "<no thread name>"_Str;
  m_eThread      = TH_NONE;
  m_pPosixThread = new PosixThread;
  m_pSemaphore   = new Semaphore;
  m_pScheduler   = gpScheduler;
  m_pTaskSet     = nullptr;
  m_npTask       = nullptr;

  Thread* pThread = this;
  gpThreadList->vAppend(pThread);
  
  _checkInitialised_(CN_THREAD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Thread::~Thread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  delete m_pPosixThread;
  delete m_pSemaphore;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* thread entry                                                               */
/*----------------------------------------------------------------------------*/
void* Thread::vEntry(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  if (gbVerbose)
    vTrace("[[%s]] Thread::vEntry %p\n"_Str,pThread->sName().s(),pThread);
  
  try {
    pThread->vLoop();
  }
  catch (Error* pError) {
    char wFileName[256];
    
    const char* ps;
    
    ps = StrLib::sLast(pError->aFileName()->s());
    bool bBackSlash = StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
    if (bBackSlash) {
      StrLib::vCopy(wFileName,ps+1);
    }
    else {
      StrLib::vCopy(wFileName, pError->aFileName()->s());
    }
    vFatal("ERROR: [ThreadLoop] %s: line %lld in %s\n"_Str,
          pError->aErrorMsg(),
          pError->iLineNo(),
          wFileName);
  }


  if (gbVerbose)
    vTrace("[[%s]] Thread:;vEntry %p exiting\n"_Str,pThread->sName().s(),pThread);
  
  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* thread entry                                                               */
/*----------------------------------------------------------------------------*/
void* Thread::vEntryV2(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  if (gbVerbose || gbProgress)
    vTrace("[[%s]] Thread::vEntryV2 %p\n"_Str,pThread->sName().s(),pThread);
  
  try {
    pThread->vLoopV2();
  }
  catch (Error* pError) {
    char wFileName[256];
    
    const char* ps;
    
    ps = StrLib::sLast(pError->aFileName()->s());
    bool bBackSlash = StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
    if (bBackSlash) {
      StrLib::vCopy(wFileName,ps+1);
    }
    else {
      StrLib::vCopy(wFileName, pError->aFileName()->s());
    }
    vFatal("ERROR: [ThreadLoop] %s: line %lld in %s\n"_Str,
           pError->aErrorMsg(),
           pError->iLineNo(),
           wFileName);
  }
  
  
  if (gbVerbose)
    vTrace("[[%s]] Thread:;vEntry %p exiting\n"_Str,pThread->sName().s(),pThread);
  
  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get this thread                                                            */
/*----------------------------------------------------------------------------*/
Thread* Thread::pThread(void)
{
  Thread *pThread = static_cast<Thread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* run given task                                                             */
/*----------------------------------------------------------------------------*/
void Thread::vRunTask(MP_Task mpTask)
{
  PosixThread::vTlsSet(giTLS_TaskKey,*mpTask);
  ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
  mpTask->vState(TS_RUNNING);
  mpTask->vThread(nullptr);
  mpTask->vLoop();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append task to queue                                                       */
/*----------------------------------------------------------------------------*/
void Thread::vAppend(MP_Task mpTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
 
  //m_pTxQueue->vAppend(pTx);

  m_pTaskSet->vAppend(mpTask);
}

/*----------------------------------------------------------------------------*/
/* bind the os/posix thread to a cpu                                          */
/*----------------------------------------------------------------------------*/
void Thread::vBind(i8 iCpu)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bBindThread(iCpu);
  ASSERT(bResult);
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
  vTrace("Thread %p: OpSysThreadId %lld PosixThreadId %llx\n"_Str,
         this,
         m_pPosixThread->m_iOpSysThreadId,
         m_pPosixThread->m_iPosixThreadId);
}

/*----------------------------------------------------------------------------*/
/* thread loop                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  if (gbVerbose || gbProgress)
    vTrace("Thread %p: %s loop\n"_Str,this,m_sName.s());
  
  m_pPosixThread->vSetOSThreadId();
  
//  vTrace("Thread %p: priority %lld\n"_Str,
//         this,
//         m_pPosixThread->iThreadPriority()
//         );
  
  //m_pPosixThread->vThreadBoost(C_THREAD_BOOST);                //! RANDOM ERROR
  
//  vTrace("Thread %p: priority %lld\n"_Str,
//         this,
//         m_pPosixThread->iThreadPriority()
//         );
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  while (true) {

    if ( ! m_pTaskSet ) {
      PosixThread::vSleepNano(10_KB);
      continue;
    }
    
    m_npTask = m_pTaskSet->npPop();
    
    if ( ! m_npTask ) {
      PosixThread::vSleepMilli(1);
      continue;
    }
    
    PosixThread::vTlsSet(giTLS_TaskKey,*m_npTask);
    ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
    m_npTask->vState(TS_RUNNING);
    m_npTask->vThread(this);
    m_npTask->vLoop();
    
    if (TS_WAITING_ACK != m_npTask->eState())
      m_npTask->vThread(nullptr);
    
    if (TS_YIELDED == m_npTask->eState()) {
      MP_Task mpTask = *m_npTask;
      vAppend(mpTask);
      m_npTask = nullptr;
    }
    else if (TS_EXITED == m_npTask->eState()) {
      MP_Task mpTask = *m_npTask;
      if ( (*gnpRootTask != *m_npTask)  && (!mpTask->bWordBody()) ) {
        delete *m_npTask;
        m_npTask = nullptr;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* thread loop                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vLoopV2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  if (gbVerbose || gbProgress)
    vTrace("Thread %p: %s loopV2\n"_Str,this,m_sName.s());
  
  m_pPosixThread->vSetOSThreadId();
  
  //  vTrace("Thread %p: priority %lld\n"_Str,
  //         this,
  //         m_pPosixThread->iThreadPriority()
  //         );
  
  //m_pPosixThread->vThreadBoost(C_THREAD_BOOST);                //! RANDOM ERROR
  
  //  vTrace("Thread %p: priority %lld\n"_Str,
  //         this,
  //         m_pPosixThread->iThreadPriority()
  //         );
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  while (true) {
    
    // get a task to run
    ASSERT( ! *m_npTask );
    bool b = m_pScheduler->bGetTask(this,true);
    ASSERT( b && *m_npTask );
    
    // set up this thread to run task
    PosixThread::vTlsSet(giTLS_TaskKey,*m_npTask);
    ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
    
    m_npTask->vState(TS_RUNNING);
    m_npTask->vThread(this);
    
    // run the task
    m_npTask->vLoop();
    
    // thread no longer running the task
    PosixThread::vTlsSet(giTLS_TaskKey,nullptr);
    
    // when task yields, task is put into task set
    if (TS_YIELDED == m_npTask->eState()) {
      m_npTask->vThread(nullptr);
      MP_Task mpTask = *m_npTask;
      vAppend(mpTask);
      m_npTask = nullptr;
      continue;
    }
    
    // unless waiting on ack, task forgets thread
    if (TS_WAITING_ACK == m_npTask->eState())
      continue;
    
    // when task exits, it is deleted (unless its a word)
    if (TS_EXITED == m_npTask->eState()) {
      m_npTask->vThread(nullptr);
      MP_Task mpTask = *m_npTask;
      if ( (*gnpRootTask != *m_npTask)  && (!mpTask->bWordBody()) ) {
        delete *m_npTask;
      }
      m_npTask = nullptr;
      continue;
    }

    ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* get name of this thread                                                    */
/*----------------------------------------------------------------------------*/
StrVal& Thread::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this thread                                                    */
/*----------------------------------------------------------------------------*/
void Thread::vName(StrVal& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_sName = sName;
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::vEntry,this);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawnV2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::vEntryV2,this);
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
/*-- Semaphore ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* signal a thread (should not be calling thread)                             */
/*----------------------------------------------------------------------------*/
void Thread::vSignalThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  m_pSemaphore->vSignalThread();
}

/*----------------------------------------------------------------------------*/
/* condition broadcast                                                        */
/*----------------------------------------------------------------------------*/
void Thread::vCondBroadcast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  m_pSemaphore->vCondBroadcast();
}

/*----------------------------------------------------------------------------*/
/* condition signal                                                           */
/*----------------------------------------------------------------------------*/
void Thread::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  m_pSemaphore->vCondSignal();
}

/*----------------------------------------------------------------------------*/
/* condition wait (thread to wait will be calling thread)                     */
/*----------------------------------------------------------------------------*/
void Thread::vCondWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  m_pSemaphore->vCondWait();
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (should be calling thread)                    */
/*----------------------------------------------------------------------------*/
void Thread::vCondTimedWait(i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  m_pSemaphore->vCondTimedWait(iMicroSeconds);
}

/*----------------------------------------------------------------------------*/
/* lock a threads mutex (blocking)                                            */
/*----------------------------------------------------------------------------*/
void Thread::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  m_pSemaphore->vMutexLock();
}

/*----------------------------------------------------------------------------*/
/* try and lock a threads mutex (non-blocking)                                */
/*----------------------------------------------------------------------------*/
bool Thread::bMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  bool b = m_pSemaphore->bMutexLock();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* unlock a threads mutex                                                     */
/*----------------------------------------------------------------------------*/
void Thread::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  m_pSemaphore->vMutexUnlock();
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
