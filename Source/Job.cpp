/*============================================================================*/
/* Job.cpp -- a job in the system                                             */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

Atomic gzJobSeq;

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
  
  eCN eClassScanEntry = pScanEntry->eClass();
  
  RASSERT(_bIsValidJob(eClassScanEntry));
  
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
Job* JobModel::_pJob(void* aAddr)
{
  Job* pJob = reinterpret_cast<Job*>(aAddr);
  
  if (pJob && ! _bIsValidJob(pJob->eClass()) )
      vTrace("_pJob got class %s\n"_Stz,aCN[pJob->eClass()]);
      
  RASSERT( ! pJob || _bIsValidJob(pJob->eClass()) );
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* is given class a valid job?                                                */
/*----------------------------------------------------------------------------*/
bool JobModel::_bIsValidJob(Base::eCN eClass)
{
  if (eClass >= CN_JOB__BEGUN && eClass <= CN_JOB_WRITEROW)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job* JobModel::_pJob(i8 iAddr)
{
  Job* pJob = reinterpret_cast<Job*>(iAddr);

  //  if ( pJob && ! _bIsValidJob(pJob->eClass() )
  //    i8 iDebug = 1;

  RASSERT( ! pJob || _bIsValidJob(pJob->eClass()) );

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
  
  if (m_bMark)
    ActivityMap::vMark(m_iJobId,"::Job %s"_Stz,aCN[eClass()]);

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(eCN eClassNo, Session* pSession)
: Object(eClassNo), JobDicPath(pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //vTrace("%s: CREATING JOB %p %s\n"_Stz,
  //       gpAttachMap->m_pGrid->pThisNode()->aNodeName(), this, aCN[eClass()]);
  
  vInit();
  
  if (m_bMark)
    ActivityMap::vMark(m_iJobId,"::Job %s"_Stz,aCN[eClass()]);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  pthread_mutex_destroy(&m_xThreadMutex);
  pthread_mutexattr_destroy(&m_xThreadMutexAttr);
  
  pthread_cond_destroy(&m_xThreadCond);
  pthread_condattr_destroy(&m_xThreadCondAttr);
  
  if (m_bMark)
    ActivityMap::vMark(m_iJobId,"~Job %s"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* initialize the job                                                         */
/*----------------------------------------------------------------------------*/
void Job::vInit(void)
{
  m_iJobId   = C_UNKNOWN;
  m_iJobSeq  = ++gzJobSeq;
  m_iSession = C_UNKNOWN;
  
  m_pJobClass = nullptr;
  m_eSeed     = SS_ENUMS;
  m_eJobTrace = JT_NONE;
  
  if (gpAttachMap->m_bMarkJobAll || gpAttachMap->m_bMarkJob[eClass()])
    m_bMark = true;
  else
    m_bMark = false;

  m_pExecutingTask    = nullptr;
  m_pOriginatingTask  = nullptr;

  m_iParticipantNode  = C_UNKNOWN;
  m_xAcksRemaining.vCheckInit();
  m_pCurrentBuffer = nullptr;
  
  m_eState = JS_RUNNING;
  
  // initialise mutex attribute
  MemLib::vZero(&m_xThreadMutexAttr,sizeof(m_xThreadMutexAttr),CN_SEMAPHORE);
  i8 iResult = pthread_mutexattr_init(&m_xThreadMutexAttr);
  ASSERT( ! iResult);
  
  // set mutex attribute to be shared
  iResult = pthread_mutexattr_setpshared(&m_xThreadMutexAttr,PTHREAD_PROCESS_SHARED);
  ASSERT( ! iResult);
  
  // initialize the mutex
  MemLib::vZero(&m_xThreadMutex,sizeof(m_xThreadMutex),CN_SEMAPHORE);
  pthread_mutex_init(&m_xThreadMutex, &m_xThreadMutexAttr);
  
  
  // initialise condition attribute
  MemLib::vZero(&m_xThreadCondAttr,sizeof(m_xThreadCondAttr),CN_SEMAPHORE);
  pthread_condattr_init(&m_xThreadCondAttr);
  
  // set condition attribute to be shared
  pthread_condattr_setpshared(&m_xThreadCondAttr, PTHREAD_PROCESS_SHARED);
  
  // initialize the condition variable
  MemLib::vZero(&m_xThreadCond,sizeof(m_xThreadCond),CN_SEMAPHORE);
  pthread_cond_init(&m_xThreadCond, &m_xThreadCondAttr);
  
  
  m_bNoWait = false;
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
  
  Stz* sSeed = m_pJobClass->sSeed(m_eSeed);
  
  return sSeed;
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

/*----------------------------------------------------------------------------*/
/* report this job                                                            */
/*----------------------------------------------------------------------------*/
void Job::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  vTrace("Job %llx id %lls ackRemaining %lld\n"_Stz,
         this,
         m_iJobId,
         m_xAcksRemaining.iPrintOnlyValue());
}

/*----------------------------------------------------------------------------*/
/* set the job seed                                                           */
/*----------------------------------------------------------------------------*/
void Job::vSeed(eSS eSeed)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eSeed = eSeed;
    
  m_pJobClass->m_bSeedsSpawned[eSeed] = true;
}

/*----------------------------------------------------------------------------*/
/* set the job trace                                                          */
/*----------------------------------------------------------------------------*/
void Job::vShow(s48K& s)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("%s: %s"_Stz,
         m_pJobClass ? (Stz*)m_pJobClass->m_aName : "<no jobClass>"_Stz,
         s.aStr());
}

/*----------------------------------------------------------------------------*/
/* pre-send job function                                                      */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_send(Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* post-recv job function                                                     */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_recv(Job* pJob)
{
  pJob->m_pExecutingTask = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
