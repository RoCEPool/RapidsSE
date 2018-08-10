/*============================================================================*/
/* Job.cpp -- a job in the system                                             */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compare two jobs by their jobId                                            */
/*----------------------------------------------------------------------------*/
bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  Job* pInsertEntry = _pJob(pObjectInsert);
  Job* pScanEntry   = _pJob(pObjectScan);
  
  if ( ! pObjectScan )
    return false;
  
  if (pInsertEntry->iJobId() == pScanEntry->iJobId())
    bMatch = true;
  else
    bMatch = false;
  
  if (pInsertEntry->iJobId() > pScanEntry->iJobId())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job* JobsModel::_pJob(void* aAddr)
{
  Job* pJob = reinterpret_cast<Job*>(aAddr);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  vInit();
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(eCN eClassNo, SEitf* pSEitf)
: Object(eClassNo), JobDicPath(pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  vInit();
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  pthread_mutex_destroy(&m_xThreadMutex);
  pthread_mutexattr_destroy(&m_xThreadMutexAttr);
  
  pthread_cond_destroy(&m_xThreadCond);
  pthread_condattr_destroy(&m_xThreadCondAttr);
}

/*----------------------------------------------------------------------------*/
/* initialize the job                                                         */
/*----------------------------------------------------------------------------*/
void Job::vInit(void)
{
  m_iJobId   = -1;
  m_eState   = JS_NONE;
  m_sSeed    = ""_Stz;
  m_bCheckResources   = false;
  m_bDetached         = false;
  m_eJobTrace   = JT_NONE;
  m_iParticipantNode  = -1;
  m_ziAcksRemaining   = 0;
  m_pOriginatingTask  = nullptr;
  m_pPromptTask = nullptr;
  m_pAncillary = nullptr;
  m_pWorkingBuffer = nullptr;
  
  // initialise attribute to mutex
  MemLib::vZero(&m_xThreadMutexAttr,sizeof(m_xThreadMutexAttr),CN_SEMAPHORE);
  i8 iResult = pthread_mutexattr_init(&m_xThreadMutexAttr);
  ASSERT( ! iResult);
  
  // set attribute to be shared
  iResult = pthread_mutexattr_setpshared(&m_xThreadMutexAttr,PTHREAD_PROCESS_SHARED);
  ASSERT( ! iResult);
  
  // initialize the mutex
  MemLib::vZero(&m_xThreadMutex,sizeof(m_xThreadMutex),CN_SEMAPHORE);
  pthread_mutex_init(&m_xThreadMutex, &m_xThreadMutexAttr);
  
  // initialise attribute to condition
  MemLib::vZero(&m_xThreadCondAttr,sizeof(m_xThreadCondAttr),CN_SEMAPHORE);
  pthread_condattr_init(&m_xThreadCondAttr);
  
  // set attribute to be shared
  pthread_condattr_setpshared(&m_xThreadCondAttr, PTHREAD_PROCESS_SHARED);
  
  // initialize the mutex
  MemLib::vZero(&m_xThreadCond,sizeof(m_xThreadCond),CN_SEMAPHORE);
  pthread_cond_init(&m_xThreadCond, &m_xThreadCondAttr);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the job seed to be executed                                            */
/*----------------------------------------------------------------------------*/
Stz* Job::sJobSeed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sSeed;
}

/*----------------------------------------------------------------------------*/
/* get the job id                                                             */
/*----------------------------------------------------------------------------*/
i8 Job::iJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iJobId;
}

/*----------------------------------------------------------------------------*/
/* set the job id                                                             */
/*----------------------------------------------------------------------------*/
void Job::vJobId(i8 iJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iJobId = iJobId;
}

/*----------------------------------------------------------------------------*/
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iOriginatorNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 iMaskOffLower48Bits = 0xFFFF000000000000;
  
  i8 iOriginatorNode = m_iJobId & iMaskOffLower48Bits;
  iOriginatorNode >>= 48;
  
  return iOriginatorNode;
}

/*----------------------------------------------------------------------------*/
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iOriginatorNode(i8 iJobId)
{
  i8 iMaskOffLower48Bits = 0xFFFF000000000000;
  
  i8 iOriginatorNode = iJobId & iMaskOffLower48Bits;
  iOriginatorNode >>= 48;
  
  return iOriginatorNode;
}

/*----------------------------------------------------------------------------*/
/* set the job id                                                             */
/*----------------------------------------------------------------------------*/
void Job::vParticipantNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iParticipantNode = iNode;
}

/*----------------------------------------------------------------------------*/
/* get the participant node                                                   */
/*----------------------------------------------------------------------------*/
i8 Job::iParticipantNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iParticipantNode;
}

/*----------------------------------------------------------------------------*/
/* get the job state                                                          */
/*----------------------------------------------------------------------------*/
eJS Job::eJobState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eState;
}

/*----------------------------------------------------------------------------*/
/* set the job state                                                          */
/*----------------------------------------------------------------------------*/
void Job::vJobState(eJS eState)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eState = eState;
}

/*----------------------------------------------------------------------------*/
/* get the job trace                                                          */
/*----------------------------------------------------------------------------*/
eJT Job::eJobTrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eJobTrace;
}

/*----------------------------------------------------------------------------*/
/* set the job trace                                                          */
/*----------------------------------------------------------------------------*/
void Job::vJobTrace(eJT eJobTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eJobTrace = eJobTrace;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
