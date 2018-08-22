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
  
  RASSERT( !pThread || CN_THREAD == pThread->eClass() );
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* new a thread from existing memory                                          */
/*----------------------------------------------------------------------------*/
void* Thread::operator new(size_t iSize)
{
  RASSERT(iSize);
  void* pObject = Heap::pAllocate(CN_THREAD);
  
  return pObject;
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
  vFatal("fatal: Thread class cannot be deleted"_Stz);

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Thread::Thread(void)
: Object(CN_THREAD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);

  ASSERT( gpAttachMap->m_pThreadList );
  
  static Atomic zThreadNo;
  
  m_iThreadNo    = ++zThreadNo;
  m_sName        = "<no thread name>"_Stz;
  m_xLock.vInitLock(LK_THREAD);
  m_eActorModel  = AC_NONE;
  m_eThread      = TH_NONE;
  m_eThreadState = TC_NONE;
  m_iPartition   = -1;
  m_pPosixThread = new PosixThread;
  m_pSemaphore   = new Semaphore;
  m_pScheduler   = gpAttachMap->m_pServerScheduler;
  m_pTaskFifo    = nullptr;
  m_pTaskSet     = nullptr;
  m_npTask       = nullptr;

  Thread* pThread = this;
  //gpAttachMap->m_pThreadList->vAppend(pThread);
  
   m_bGeneralThead = false;
   m_iLastRRqueue  = QR_SLEEPING_QUEUE - 1;
   m_iStartRRqueue = QR_NONE;
  
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
void* Thread::vEntry_Drone(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("[[%s]] Thread::vEntry_Drone %p\n"_Stz,pThread->sName().aChar(),pThread);
  
  try {
    pThread->vLoop_Drone();
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
    vTrace("[[%s]] Thread::vEntry_Drone %p exiting\n"_Stz,pThread->sName().aChar(),pThread);
  
  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* thread entry                                                               */
/*----------------------------------------------------------------------------*/
void* Thread::vEntry_Partition(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("[[%s]] Thread::vEntry_Partition %p\n"_Stz,pThread->sName().aChar(),pThread);
  
  try {
    pThread->vLoop_Partition2();
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
    vTrace("[[%s]] Thread::vEntry_Partition %p exiting\n"_Stz,pThread->sName().aChar(),pThread);
  
  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* thread entry                                                               */
/*----------------------------------------------------------------------------*/
void* Thread::vEntry_Server(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  if (gpAttachMap->m_bVerbose || gpAttachMap->m_bProgress)
    vTrace("[[%s]] Thread::vEntry_Server %p\n"_Stz,pThread->sName().aChar(),pThread);
  
  //pThread->m_pPosixThread->bBindThread(pThread->m_iThreadNo % 2);
  
  try {
    pThread->vLoop_Server();
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
    vTrace("[[%s]] Thread::vEntry_Server %p exiting\n"_Stz,pThread->sName().aChar(),pThread);
  
  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* thread entry                                                               */
/*----------------------------------------------------------------------------*/
void* Thread::vEntry_Worker(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("[[%s]] Thread::vEntry_Worker %p\n"_Stz,pThread->sName().aChar(),pThread);
    
    try {
      pThread->vLoop_Worker();
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
    vTrace("[[%s]] Thread::vEntry_Worker %p exiting\n"_Stz,pThread->sName().aChar(),pThread);
    
    delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* thread entry point (R3.5 model)                                            */
/*----------------------------------------------------------------------------*/
void* Thread::pEntryPoint(void* pArg)
{
  //
  // this is the entry point for the new thread model
  //
  
  
  Thread* pThread = static_cast<Thread*>(pArg);
  
  pThread->vThreadState(TC_IDLE);
  
  gpAttachMap->m_pQueues->vAddThread(pThread);
  
  //if (gpAttachMap->m_bVerbose)
    vTrace("\n[[%s]] Thread::vEntryPoint %p\n"_Stz,pThread->sName().aChar(),pThread);
  
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
    vTrace("[[%s]] Thread::vEntry_Worker %p exiting\n"_Stz,
           pThread->sName().aChar(),pThread);
  
  gpAttachMap->m_pQueues->vRemoveThread(pThread);
  
  delete pThread;
  
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
/* run given task                                                             */
/*----------------------------------------------------------------------------*/
void Thread::vRunTask(MP_Task mpTask)
{
  PosixThread::vTlsSet(giTLS_TaskKey,*mpTask);
  ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
  mpTask->vState(TS_EXECUTING);
  mpTask->vThread(nullptr);
  mpTask->vTaskLoop();
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
  vTrace("Thread %p: OpSysThreadId %lld PosixThreadId %llx\n"_Stz,
         this,
         m_pPosixThread->m_iOpSysThreadId,
         m_pPosixThread->m_iPosixThreadId);
}

/*----------------------------------------------------------------------------*/
/* drop the write lock on this thread                                         */
/*----------------------------------------------------------------------------*/
void Thread::vDropLock(void)
{
  ASSERT(m_xLock.bIsLocked());
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* signal thread to execute given task                                        */
/*----------------------------------------------------------------------------*/
void Thread::vExecuteTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(bMutexIsLocked());
  
  // task should be ready
  RASSERT(TS_READY == pTask->eState());
  pTask->vState(TS_EXECUTING);
  
  // thread should be idle
  RASSERT(TC_IDLE == m_eThreadState);
  
  // thread should have no task
  RASSERT(! *m_npTask);
  
  // set task to be executed
  m_npTask = pTask;
  
  m_eThreadState = TC_RUNNING;
  
  // signal the thread
  vSignalThread();
}

/*----------------------------------------------------------------------------*/
/* is this a general thread                                                   */
/*----------------------------------------------------------------------------*/
bool Thread::bGeneralThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_bGeneralThead;
}

/*----------------------------------------------------------------------------*/
/* set general thread                                                         */
/*----------------------------------------------------------------------------*/
void Thread::vGeneralThread(bool bGeneralThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_bGeneralThead = bGeneralThread;
}

/*----------------------------------------------------------------------------*/
/* grab a write lock on this thread                                           */
/*----------------------------------------------------------------------------*/
bool Thread::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool b = m_pSemaphore->bMutexLock();

  ASSERT( ! b || m_pSemaphore->bMutexIsLocked() );
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* this thread waits till its given a task by a handler                       */
/*----------------------------------------------------------------------------*/
void Thread::vHandlerWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // under mutex lock
  
  // will not have a task
  ASSERT(! *m_npTask);
  
  // this thread's mutex should be locked
  ASSERT(m_pSemaphore->bMutexIsLocked());
  
  m_eThreadState = TC_IDLE;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vCondTimedWait(5000); // while waiting the mutex is dropped

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // under mutex lock
  
  // this thread's mutex should be locked
  ASSERT(m_pSemaphore->bMutexIsLocked());
  
  // will have a task
  //ASSERT(*m_npTask);
  
  if (*m_npTask)
    m_eThreadState = TC_RUNNING;
}

/*----------------------------------------------------------------------------*/
/* is thread idle?                                                            */
/*----------------------------------------------------------------------------*/
bool Thread::bIsIdle(void)
{
  
  if (TC_IDLE == m_eThreadState)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this thread the tsl thread ?                                            */
/*----------------------------------------------------------------------------*/
bool Thread::bIsTLSThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Thread* pTLSThread = pThisTLSThread();
  
  if (this == pThisTLSThread())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get last round robin queue                                                 */
/*----------------------------------------------------------------------------*/
i8 Thread::iLastRRqueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_iLastRRqueue;
}

/*----------------------------------------------------------------------------*/
/* set last round robin queue                                                 */
/*----------------------------------------------------------------------------*/
void Thread::vLastRRqueue(i8 iLastRRqueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_iLastRRqueue = iLastRRqueue;
}

/*----------------------------------------------------------------------------*/
/* thread loop (RDP R3.5 model)                                               */
/*----------------------------------------------------------------------------*/
void Thread::vLoop(void)
{
  //
  // this is the thread loop for the new thread model
  //
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey) == this);
  
  // sleep infinite
  //m_pPosixThread->vSleepSec(100000000);
  
  
  vMutexLock();

  if (++gpAttachMap->m_ziR35Threads == gxArguments.m_nThreads) {
    RapidsSE::vStartSystemTasks();
  }
  
  for (;;) {
   
    // wont have a task
    //ASSERT(! *m_npTask);
    
    // get next task
    if (! *m_npTask && ! bQueuePeek()) {
      vHandlerWait();
      
      if ( ! *m_npTask)
        continue;
    }
   
    // will have a task
    ASSERT(*m_npTask);
    
    // task will be running R3.5 model
    ASSERT(m_npTask->bR35model());

    // call the task
    vTaskCall();
   
    // handle thread level event
    Handlers::vThreadLoopHandler(&m_npTask);

    // wont have a task
    //ASSERT(! *m_npTask);
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* drone loop (calls task loop just once)                                     */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_Drone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(m_eActorModel == AC_DRONE);
  
  if (gpAttachMap->m_bVerbose || gpAttachMap->m_bProgress)
    vTrace("Thread %p: %s loop\n"_Stz,this,m_sName.aChar());
  
  m_pPosixThread->vSetOSThreadId();
  
  // expect a task
  if (! *m_npTask) {
    vTrace("Thread %p: %s has no task\n"_Stz,this,m_sName.aChar());
    ASSERT(*m_npTask);
  }

  // don't expect a taskSet
  if (m_pTaskSet) {
    vTrace("Thread %p: %s has a taskset\n"_Stz,this,m_sName.aChar());
    ASSERT(! m_pTaskSet);
  }
  
  //  vTrace("Thread %p: priority %lld\n"_Stz,
  //         this,
  //         m_pPosixThread->iThreadPriority()
  //         );
  
  //m_pPosixThread->vThreadBoost(C_THREAD_BOOST);               //! RANDOM ERROR
  
  //  vTrace("Thread %p: priority %lld\n"_Stz,
  //         this,
  //         m_pPosixThread->iThreadPriority()
  //         );
  
  ASSERT(m_npTask->eActorModel() == AC_DRONE);
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  PosixThread::vTlsSet(giTLS_TaskKey,*m_npTask);
  ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
  
  m_npTask->vState(TS_EXECUTING);
  m_npTask->vThread(this);
  m_npTask->vTaskLoop();
  
  ASSERT(TS_EXITED == m_npTask->eState());
  
  MP_Task mpTask = *m_npTask;
  if ( (*gpAttachMap->m_npBootTask != *m_npTask)  && (!mpTask->bWordBody()) ) {
    delete *m_npTask;
    m_npTask = nullptr;
  }
  
  m_pPosixThread->vSleepSec(100000000);                                 //! temp
}

/*----------------------------------------------------------------------------*/
/* thread loop (sleep loop)                                                   */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_Partition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(m_eActorModel == AC_PARTITION);
  
  if (gpAttachMap->m_bVerbose || gpAttachMap->m_bProgress)
    vTrace("Thread %p: %s loop\n"_Stz,this,m_sName.aChar());
  
  m_pPosixThread->vSetOSThreadId();
  
//  vTrace("Thread %p: priority %lld\n"_Stz,
//         this,
//         m_pPosixThread->iThreadPriority()
//         );
  
  //m_pPosixThread->vThreadBoost(C_THREAD_BOOST);                //! RANDOM ERROR
  
//  vTrace("Thread %p: priority %lld\n"_Stz,
//         this,
//         m_pPosixThread->iThreadPriority()
//         );
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  // sleep loop: sleep waiting for next task
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
    
    ASSERT(m_npTask->eActorModel() == AC_PARTITION);
    
    PosixThread::vTlsSet(giTLS_TaskKey,*m_npTask);
    ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
    m_npTask->vState(TS_EXECUTING);
    m_npTask->vThread(this);
    m_npTask->vTaskLoop();
    
    if (TS_WAITING_ACK != m_npTask->eState())
      m_npTask->vThread(nullptr);
    
    if (TS_YIELDED == m_npTask->eState()) {
      MP_Task mpTask = *m_npTask;
      vAppend(mpTask);
      m_npTask = nullptr;
    }
    else if (TS_EXITED == m_npTask->eState()) {
      MP_Task mpTask = *m_npTask;
      if ( (*gpAttachMap->m_npBootTask != *m_npTask)  && (!mpTask->bWordBody()) ) {
        delete *m_npTask;
        m_npTask = nullptr;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* partition loop (wait for signal loop)                                      */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_Partition2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(m_eActorModel == AC_PARTITION);
  
  if (gpAttachMap->m_bVerbose || gpAttachMap->m_bProgress)
    vTrace("Thread %p: %s Partition\n"_Stz,this,m_sName.aChar());
  
  m_pScheduler = gpAttachMap->m_pPartitionScheduler;
  
  m_pPosixThread->vSetOSThreadId();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // validate model
  
  // expect a valid partition
  if (-1 == m_iPartition) {
    vTrace("Thread %p: %s does not have a valid partition\n"_Stz,this,m_sName.aChar());
    ASSERT(! *m_npTask);
  }
  
  // don't expect a task
  if (*m_npTask) {
    vTrace("Thread %p: %s has a task\n"_Stz,this,m_sName.aChar());
    ASSERT(! *m_npTask);
  }
  
  // expect a taskSet
  if (! m_pTaskSet) {
    vTrace("Thread %p: %s does not have a taskset\n"_Stz,this,m_sName.aChar());
    ASSERT(m_pTaskSet);
  }
  
  // expect a scheduler
  if (! m_pScheduler) {
    vTrace("Thread %p: %s does not have a scheduler\n"_Stz,this,m_sName.aChar());
    ASSERT(m_pScheduler);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set up tls
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  // loop, wait for signal of next task
  while (true) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get a task to run
    
    m_npTask = m_pTaskSet->npPop();               //! need to have a max taskset
    if (! *m_npTask) {
      ASSERT( ! *m_npTask );
      bool b = ((PartitionScheduler*)m_pScheduler)->bGetTask_Mutex(m_iPartition,this,true);
      ASSERT( b && *m_npTask );
    }
    
    ASSERT(m_npTask->eActorModel() == AC_PARTITION);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // set up this thread to run task
    
    PosixThread::vTlsSet(giTLS_TaskKey,*m_npTask);
    ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
    
    m_npTask->vState(TS_EXECUTING);
    m_npTask->vThread(this);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // run the task
    
    m_npTask->vTaskLoop();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread no longer running the task
    
    PosixThread::vTlsSet(giTLS_TaskKey,nullptr);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // when task yields, task is put into task set
    
    if (TS_YIELDED == m_npTask->eState()) {
      m_npTask->vThread(nullptr);
      MP_Task mpTask = *m_npTask;
      vAppend(mpTask);
      m_npTask = nullptr;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // unless waiting on ack, task forgets thread
    
    if (TS_WAITING_ACK == m_npTask->eState()) {
      m_npTask->vThread(nullptr);
      m_npTask = nullptr;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // when task exits, it is deleted (unless its a word)
    
    if (TS_EXITED == m_npTask->eState()) {
      m_npTask->vThread(nullptr);
      MP_Task mpTask = *m_npTask;
      if ( (*gpAttachMap->m_npBootTask != *m_npTask)  && (!mpTask->bWordBody()) ) {
        delete *m_npTask;
      }
      m_npTask = nullptr;
      continue;
    }
    
    NEVER_REACH;
    
  } // while
}

/*----------------------------------------------------------------------------*/
/* server loop (wait for signal loop)                                         */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_Server(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(m_eActorModel == AC_SERVER);
  
  if (gpAttachMap->m_bVerbose || gpAttachMap->m_bProgress)
    vTrace("Thread %p: %s Server\n"_Stz,this,m_sName.aChar());
  
  m_pPosixThread->vSetOSThreadId();
  
  //  vTrace("Thread %p: priority %lld\n"_Stz,
  //         this,
  //         m_pPosixThread->iThreadPriority()
  //         );
  
  //m_pPosixThread->vThreadBoost(C_THREAD_BOOST);               //! RANDOM ERROR
  
  //  vTrace("Thread %p: priority %lld\n"_Stz,
  //         this,
  //         m_pPosixThread->iThreadPriority()
  //         );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // validate model
  
  // don't expect a task
  if (*m_npTask) {
    vTrace("Thread %p: %s has a task\n"_Stz,this,m_sName.aChar());
    ASSERT(! *m_npTask);
  }
  
  // expect a taskSet
  if (! m_pTaskSet) {
    vTrace("Thread %p: %s does not have a taskset\n"_Stz,this,m_sName.aChar());
    ASSERT(m_pTaskSet);
  }
  
  // expect a scheduler
  if (! m_pScheduler) {
    vTrace("Thread %p: %s does not have a scheduler\n"_Stz,this,m_sName.aChar());
    ASSERT(m_pScheduler);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set up tls
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  // loop, wait for signal of next task
  while (true) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get a task to run
    
    m_npTask = m_pTaskSet->npPop();               //! need to have a max taskset
    if (! *m_npTask) {
      ASSERT( ! *m_npTask );
      bool b = ((ServerScheduler*)m_pScheduler)->bGetTask_Mutex(this,true);
      ASSERT( b && *m_npTask );
    }
    
    ASSERT(m_npTask->eActorModel() == AC_SERVER);
    //vTrace("Thread %p: Task %p is executing Que:%lld\n"_Stz,this,*m_npTask, m_pTaskSet->iCount());

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // set up this thread to run task
    
    PosixThread::vTlsSet(giTLS_TaskKey,*m_npTask);
    ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
    
    m_npTask->vState(TS_EXECUTING);
    m_npTask->vThread(this);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // run the task
    
    m_npTask->vTaskLoop();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // thread no longer running the task
    
    PosixThread::vTlsSet(giTLS_TaskKey,nullptr);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // when task yields, task is put into task set
    
    if (TS_YIELDED == m_npTask->eState()) {
      //vTrace("Thread %p: Task %p has yielded\n"_Stz,this,*m_npTask);
      m_npTask->vThread(nullptr);
      MP_Task mpTask = *m_npTask;
      //vAppend(mpTask);
      ((ServerScheduler*)m_pScheduler)->vEnqueueTask_Mutex(*mpTask);
      m_npTask = nullptr;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // unless waiting on ack, task forgets thread
    
    if (TS_WAITING_ACK == m_npTask->eState()) {
      m_npTask->vThread(nullptr);
      m_npTask = nullptr;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // when task exits, it is deleted (unless its a word)
    
    if (TS_EXITED == m_npTask->eState()) {
      m_npTask->vThread(nullptr);
      MP_Task mpTask = *m_npTask;
      if ( (*gpAttachMap->m_npBootTask != *m_npTask)  && (!mpTask->bWordBody()) ) {
        delete *m_npTask;
      }
      m_npTask = nullptr;
      continue;
    }

    vTrace("vLoop_Server: NEVER_REACH point; m_npTask->eState() %lld\n"_Stz,
           (i8)m_npTask->eState());
    
    NEVER_REACH;
    
  } // while
}

/*----------------------------------------------------------------------------*/
/* worker loop                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vLoop_Worker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(m_eActorModel == AC_WORKER);

  if (gpAttachMap->m_bVerbose || gpAttachMap->m_bProgress)
    vTrace("Thread %p: %s loop\n"_Stz,this,m_sName.aChar());
  
  m_pPosixThread->vSetOSThreadId();
  
  //  vTrace("Thread %p: priority %lld\n"_Stz,
  //         this,
  //         m_pPosixThread->iThreadPriority()
  //         );
  
  //m_pPosixThread->vThreadBoost(C_THREAD_BOOST);                //! RANDOM ERROR
  
  //  vTrace("Thread %p: priority %lld\n"_Stz,
  //         this,
  //         m_pPosixThread->iThreadPriority()
  //         );
  
  PosixThread::vTlsSet(giTLS_ThreadKey,this);
  ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  // sleep loop: sleep waiting for next task
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
    
    ASSERT(m_npTask->eActorModel() == AC_WORKER);
    
    PosixThread::vTlsSet(giTLS_TaskKey,*m_npTask);
    ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
    m_npTask->vState(TS_EXECUTING);
    m_npTask->vThread(this);
    m_npTask->vTaskLoop();
    
    if (TS_WAITING_ACK != m_npTask->eState())
      m_npTask->vThread(nullptr);
    else {
      ASSERT(m_npTask->pThread());
    }
    
    if (TS_YIELDED == m_npTask->eState()) {
      MP_Task mpTask = *m_npTask;
      vAppend(mpTask);
      m_npTask = nullptr;
    }
    else if (TS_EXITED == m_npTask->eState()) {
      MP_Task mpTask = *m_npTask;
      if ( (*gpAttachMap->m_npBootTask != *m_npTask)  && (!mpTask->bWordBody()) ) {
        delete *m_npTask;
        m_npTask = nullptr;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
i8 Thread::iPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* set partition                                                              */
/*----------------------------------------------------------------------------*/
void Thread::vPartition(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_iPartition = iPartition;
}

/*----------------------------------------------------------------------------*/
/* atetmpt to get a task to run by peeking at the queues                      */
/*---------------------------------------------------------------------------*/
bool Thread::bQueuePeek(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  Task* pPoppedTask;
  bool bPopped = gpAttachMap->m_pQueues->bPopQueuedTask(&pPoppedTask,this);
  
  m_npTask = pPoppedTask;
  return bPopped;
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
/* re-excute same task                                                        */
/*----------------------------------------------------------------------------*/
void Thread::vReExecuteTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  // task should be ready
  RASSERT(TS_READY == pTask->eState() || TS_YIELDED == pTask->eState());
  pTask->vState(TS_EXECUTING);
  
  // thread should be idle
  RASSERT(TC_RUNNING == m_eThreadState);
  
  // thread should have no task
  RASSERT(! *m_npTask);
  
  // set task to be executed
  m_npTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* advance to next queue in round robin sequence                              */
/*----------------------------------------------------------------------------*/
bool Thread::bRRnext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_iLastRRqueue++;
  
  if (m_iLastRRqueue > gpAttachMap->m_pQueues->i_QueueLast())
    m_iLastRRqueue = gpAttachMap->m_pQueues->i_QueueFirst();
  
  if (m_iLastRRqueue == m_iStartRRqueue)
    return false; // end of round robin sequence
  else {
    if (m_iLastRRqueue == gpAttachMap->m_pQueues->i_QueueFirst())
      m_iStartRRqueue = gpAttachMap->m_pQueues->i_QueueFirst();
    return true;  // have advanced to next queue
  }
}

/*----------------------------------------------------------------------------*/
/* start another round robin sequence                                         */
/*----------------------------------------------------------------------------*/
void Thread::vRRstart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
 
  m_iStartRRqueue = m_iLastRRqueue - 1;
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawn_Drone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::vEntry_Drone,this);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawn_Partition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::vEntry_Partition,this);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawn_Server(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::vEntry_Server,this);
  
  
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawn_Thread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::pEntryPoint,this);
  
  
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawn_Worker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_pPosixThread->bSpawnThread(Thread::vEntry_Worker,this);
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
/* set the task in the thread                                                 */
/*----------------------------------------------------------------------------*/
void Thread::vTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_npTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* call the assocated task                                                    */
/*----------------------------------------------------------------------------*/
void Thread::vTaskCall(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_eThreadState = TC_RUNNING;
  ASSERT(*m_npTask);
  
  PosixThread::vTlsSet(giTLS_TaskKey,*m_npTask);
  ASSERT(PosixThread::pTlsGet(giTLS_TaskKey));
  
  m_npTask->vState(TS_EXECUTING);
  m_npTask->vThread(this);
  
  // add this task to the executing task queue
  gpAttachMap->m_pQueues->vAddToExecutingQueue(*m_npTask);
  
  // record time of call
  // ...
  
  m_npTask->vTaskLoop_R35();

  // record time of return & calculate running total times
  // ...
}

/*----------------------------------------------------------------------------*/
/* get this threads task fifo                                                 */
/*----------------------------------------------------------------------------*/
TaskFifo* Thread::pTaskFifo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_pTaskFifo;
}

/*----------------------------------------------------------------------------*/
/* get this threads task fifo                                                 */
/*----------------------------------------------------------------------------*/
void Thread::vTaskFifo(TaskFifo* pTaskFifo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pTaskFifo = pTaskFifo;
}


/*----------------------------------------------------------------------------*/
/*-- Semaphore ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* signal a thread (should not be calling thread)                             */
/*----------------------------------------------------------------------------*/
void Thread::vSignalThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  ASSERT(bMutexIsLocked());
  m_pSemaphore->vSignalThread();
}

/*----------------------------------------------------------------------------*/
/* condition broadcast                                                        */
/*----------------------------------------------------------------------------*/
void Thread::vCondBroadcast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pSemaphore->vCondBroadcast();
}

/*----------------------------------------------------------------------------*/
/* condition signal                                                           */
/*----------------------------------------------------------------------------*/
void Thread::vCondSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pSemaphore->vCondSignal();
}

/*----------------------------------------------------------------------------*/
/* condition wait (thread to wait will be calling thread)                     */
/*----------------------------------------------------------------------------*/
void Thread::vCondWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pSemaphore->vCondWait_Mutex();
}

/*----------------------------------------------------------------------------*/
/* this thread waits for signal (should be calling thread)                    */
/*----------------------------------------------------------------------------*/
void Thread::vCondTimedWait(i8 iMicroSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pSemaphore->vCondTimedWait(iMicroSeconds);
}

/*----------------------------------------------------------------------------*/
/* test if a threads mutex is locked (non-blocking)                           */
/*----------------------------------------------------------------------------*/
bool Thread::bMutexIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool b = m_pSemaphore->bMutexIsLocked();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* lock a threads mutex (blocking)                                            */
/*----------------------------------------------------------------------------*/
void Thread::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  m_pSemaphore->vMutexLock();
  
  ASSERT(m_pSemaphore->bMutexIsLocked());
}

/*----------------------------------------------------------------------------*/
/* try and lock a threads mutex (non-blocking)                                */
/*----------------------------------------------------------------------------*/
bool Thread::bMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool b = m_pSemaphore->bMutexLock();
  
  ASSERT( ! b || m_pSemaphore->bMutexIsLocked() );
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* unlock a threads mutex                                                     */
/*----------------------------------------------------------------------------*/
void Thread::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_POSIXTHREAD,_FOOTER_);
  
  ASSERT(m_pSemaphore->bMutexIsLocked());
  
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
