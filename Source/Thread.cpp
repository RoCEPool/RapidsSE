/*============================================================================*/
/* Thread.cpp - thread                                                        */
/*============================================================================*/

#include "Threads.h"

extern BufferPool*  gpBufferPool;
extern ThreadList*  gpThreadList;

extern pthread_key_t  giTLS_TaskKey;
extern pthread_key_t  giTLS_ThreadKey;

/*----------------------------------------------------------------------------*/
/* new a thread from existing memory                                          */
/*----------------------------------------------------------------------------*/
void* Thread::operator new(size_t iSize, Thread* pThread)
{
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* threads are never delete (for the moment)                                  */
/*----------------------------------------------------------------------------*/
void Thread::operator delete(void* pTxAddress)
{
  vFatal("fatal: Thread class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Thread::Thread(void)
: Object(CN_THREAD)
{
  _poolAddress_(this);

  ASSERT( gpThreadList );
  
  m_sName    = "<no thread name>";
  m_pTaskSet = nullptr;
  m_npTask   = nullptr;

  Thread* pThread = this;
  gpThreadList->vAppend(pThread);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Thread::~Thread(void)
{

}

/*----------------------------------------------------------------------------*/
/* thread entry                                                               */
/*----------------------------------------------------------------------------*/
void* Thread::vEntry(void* pArg)
{
  Thread* pThread = static_cast<Thread*>(pArg);
  
  vTrace("[[%s]] Thread::vEntry %p\n",pThread->sName().a(),pThread);
  
  pThread->vLoop();

  vTrace("[[%s]] Thread:;vEntry %p exiting\n",pThread->sName().a(),pThread);
  
  delete pThread;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* append task to queue                                                       */
/*----------------------------------------------------------------------------*/
void Thread::vAppend(NNP_Task nnpTask)
{
  _poolAddress_(this);
 
  //m_pTxQueue->vAppend(pTx);

  m_pTaskSet->vAppend(nnpTask);
}

/*----------------------------------------------------------------------------*/
/* bind the os/posix thread to a cpu                                          */
/*----------------------------------------------------------------------------*/
void Thread::vBind(i8 iCpu)
{
  _poolAddress_(this);
  
  bool bResult = m_xPosixThread.bBindThread(iCpu);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* get the cpu this thread is bound to                                        */
/*----------------------------------------------------------------------------*/
i8 Thread::iCpu(void)
{
  _poolAddress_(this);
  
  return m_xPosixThread.iThreadCpu();
}

/*----------------------------------------------------------------------------*/
/* thread dump                                                                */
/*----------------------------------------------------------------------------*/
void Thread::vDump(void) const
{
  _poolAddress_(this);
  
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
  _poolAddress_(this);
  
//  vTrace("Thread %p: loop\n",
//         this
//         );
  
  m_xPosixThread.vSetOSThreadId();
  
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
        if (bWarn)
          vTrace("[[%s]] Thread::vLoop no task\n",m_sName.a());
        bWarn = false;
        //m_xPosixThread.vNanoSleep(10_MB);
        //m_xPosixThread.vSecSleep(1);
        PosixThread::vSleepNano(10_KB);
        continue;
      }
      
      bWarn = true;
      
      m_xPosixThread.vTlsSet(giTLS_TaskKey,*m_npTask);
      ASSERT(m_xPosixThread.pTlsGet(giTLS_TaskKey));
      
      m_npTask->vState(TS_RUNNING);
      
      vTrace("[[%s]] Thread::vLoop running [%s] %p\n",m_sName.a(),m_npTask->sName().a(),*m_npTask);
      
      // set rdtscp
      m_npTask->vLoop();
      // clear rdtscp
      
      if (TS_SWITCH == m_npTask->eState()) {
        NNP_Task nnpTask = *m_npTask;
        vAppend(nnpTask);
        m_npTask = nullptr;
      }
      else if (TS_EXITED == m_npTask->eState()) {
        delete *m_npTask;
        m_npTask = nullptr;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get name of this thread                                                    */
/*----------------------------------------------------------------------------*/
s& Thread::sName(void)
{
  _poolAddress_(this);

  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this thread                                                    */
/*----------------------------------------------------------------------------*/
void Thread::vName(s& sName)
{
  _poolAddress_(this);
  
  m_sName = sName;
}

/*----------------------------------------------------------------------------*/
/* spawn the os/posix thread associated with this thread                      */
/*----------------------------------------------------------------------------*/
void Thread::vSpawn(void)
{
  _poolAddress_(this);
  
  bool bResult = m_xPosixThread.bSpawnThread(Thread::vEntry,this);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* get this thread                                                            */
/*----------------------------------------------------------------------------*/
Thread* Thread::pThread(void)
{
  Thread *pThread = static_cast<Thread*>(PosixThread::pTlsGet(giTLS_ThreadKey));
  
  return pThread;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/