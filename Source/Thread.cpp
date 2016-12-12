/*============================================================================*/
/* Thread.cpp - thread                                                        */
/*============================================================================*/

#include "Threads.h"
#include "Arguments.hpp"

extern BufferPool*  gpBufferPool;
extern ThreadList*  gpThreadList;
extern TaskList*    gpTaskList;

extern NP_Task      gnpRootTask;

extern Arguments    gxArguments;

extern pthread_key_t  giTLS_TaskKey;
extern pthread_key_t  giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Thread* Threads::_pThread(void* pAddr)
{
  Thread* pThread = reinterpret_cast<Thread*>(pAddr);
  
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
  vFatal("fatal: Thread class cannot be deleted");

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
  
  m_iThreadNo = ++zThreadNo;
  m_sName     = "<no thread name>";
  m_pTaskSet  = nullptr;
  m_npTask    = nullptr;

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
}

/*----------------------------------------------------------------------------*/
/* thread entry                                                               */
/*----------------------------------------------------------------------------*/
void* Thread::vEntry(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  if (gxArguments.m_bVerbose)
    vTrace("[[%s]] Thread::vEntry %p\n",pThread->sName().a(),pThread);
  
  try {
    pThread->vLoop();
  }
  catch (Error* pError) {
    char wFileName[256];
    
    const char* ps;
    
    ps = String::sLast(pError->pFileName());
    bool bBackSlash = String::bBackward(&ps,pError->pFileName(),'/');
    if (bBackSlash) {
      String::vCopy(wFileName,ps+1);
    }
    else {
      String::vCopy(wFileName, pError->pFileName());
    }
    vFatal("ERROR: [ThreadLoop] %s: line %lld in %s\n",
          pError->pErrorMsg(),
          pError->iLineNumber(),
          wFileName);
  }


  if (gxArguments.m_bVerbose)
    vTrace("[[%s]] Thread:;vEntry %p exiting\n",pThread->sName().a(),pThread);
  
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
/* append task to queue                                                       */
/*----------------------------------------------------------------------------*/
void Thread::vAppend(NNP_Task nnpTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
 
  //m_pTxQueue->vAppend(pTx);

  m_pTaskSet->vAppend(nnpTask);
}

/*----------------------------------------------------------------------------*/
/* bind the os/posix thread to a cpu                                          */
/*----------------------------------------------------------------------------*/
void Thread::vBind(i8 iCpu)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  bool bResult = m_xPosixThread.bBindThread(iCpu);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* get the cpu this thread is bound to                                        */
/*----------------------------------------------------------------------------*/
i8 Thread::iCpu(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_xPosixThread.iThreadCpu();
}

/*----------------------------------------------------------------------------*/
/* thread dump                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vDump(void) const
{
  vTrace("Thread %p: OpSysThreadId %lld PosixThreadId %llx\n",
         this,
         m_xPosixThread.m_iOpSysThreadId,
         m_xPosixThread.m_iPosixThreadId);
}

/*----------------------------------------------------------------------------*/
/* thread loop                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  if (gxArguments.m_bVerbose)
    vTrace("Thread %p: %s loop\n",this,m_sName.a());
  
  m_xPosixThread.vSetOSThreadId();
  
//  vTrace("Thread %p: priority %lld\n",
//         this,
//         m_xPosixThread.iThreadPriority()
//         );
  
  //m_xPosixThread.vThreadBoost(C_THREAD_BOOST);                //! RANDOM ERROR
  
//  vTrace("Thread %p: priority %lld\n",
//         this,
//         m_xPosixThread.iThreadPriority()
//         );
  
  bool bWarn = true;

  m_xPosixThread.vTlsSet(giTLS_ThreadKey,this);
  ASSERT(m_xPosixThread.pTlsGet(giTLS_ThreadKey));
  
  if (m_npTask != nullptr) {
    m_xPosixThread.vTlsSet(giTLS_TaskKey,*m_npTask);
    ASSERT(m_xPosixThread.pTlsGet(giTLS_TaskKey));
    
    m_npTask->vState(TS_RUNNING);
    
    m_npTask->vLoop();
  }
  else {
    while (true) {

      if ( ! m_pTaskSet ) {
        PosixThread::vSleepNano(10_KB);
        continue;
      }
      
      m_npTask = m_pTaskSet->npPop();
      
      if ( ! m_npTask ) {
        if (bWarn && gxArguments.m_bVerbose)
          vTrace("[[%s]] Thread::vLoop no task\n",m_sName.a());
        bWarn = false;
        //m_xPosixThread.vNanoSleep(10_MB);
        //m_xPosixThread.vSecSleep(1);
        PosixThread::vSleepMilli(1);
        continue;
      }
      
      bWarn = true;
      
      m_xPosixThread.vTlsSet(giTLS_TaskKey,*m_npTask);
      ASSERT(m_xPosixThread.pTlsGet(giTLS_TaskKey));
      
      m_npTask->vState(TS_RUNNING);
      
//      if (gxArguments.m_bVerbose)
//        vTrace("[[%s]] Thread::vLoop running [%s] %p\n",m_sName.a(),m_npTask->sName().a(),*m_npTask);
      
      // set rdtscp
      m_npTask->vThread(this);
      m_npTask->vLoop();
      m_npTask->vThread(nullptr);
      // clear rdtscp
      
      if (TS_YIELD == m_npTask->eState()) {
        NNP_Task nnpTask = *m_npTask;
        vAppend(nnpTask);
        m_npTask = nullptr;
      }
      else if (TS_EXITED == m_npTask->eState()) {
        if (*gnpRootTask != *m_npTask) {
          delete *m_npTask;
          m_npTask = nullptr;
        }
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get name of this thread                                                    */
/*----------------------------------------------------------------------------*/
s& Thread::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this thread                                                    */
/*----------------------------------------------------------------------------*/
void Thread::vName(s& sName)
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
  
  bool bResult = m_xPosixThread.bSpawnThread(Thread::vEntry,this);
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Thread::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREAD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
