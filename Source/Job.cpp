/*============================================================================*/
/* Job.cpp -- a job in the system                                             */
/*============================================================================*/

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern ActivityMap* gpActivityMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job* Jobs::_pJob(void* aAddr)
{
  Job* pJob = reinterpret_cast<Job*>(aAddr);
  
  if (pJob && ! _bIsValidJob(pJob->eClass()) )
      vTrace("_pJob got class %s\n"_Stz,aCN[pJob->eClass()]);
      
  RASSERT( ! pJob || _bIsValidJob(pJob->eClass()) );
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job* Jobs::_pJob(i8 iAddr)
{
  Job* pJob = reinterpret_cast<Job*>(iAddr);
  
  //  if ( pJob && ! _bIsValidJob(pJob->eClass() )
  //    i8 iDebug = 1;
  
  RASSERT( ! pJob || _bIsValidJob(pJob->eClass()) );
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* is given class a valid job?                                                */
/*----------------------------------------------------------------------------*/
bool Jobs::_bIsValidJob(Base::eCN eClass)
{
  if (eClass >= CN_JOB__BEGUN && eClass <= CN_JOB_WRITEROW)
    return true;
  else
    return false;
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
    ActivityMap::vMark(0,"::Job %s"_Stz,aCN[eClass()]);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  delete m_pMutex;
  
  if (m_bMark)
    ActivityMap::vMark(m_iJobId,"~Job %s"_Stz,aCN[eClass()]);
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
/* write a mark about this task                                               */
/*----------------------------------------------------------------------------*/
void Job::vMarkJob(Stz* szLeadingText, Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);

#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityMap )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,szFormat);
  
  ActivityMap::vMarkJob(this,szLeadingText,szFormat,xArgs);
  
  va_end(xArgs);
#endif
  
}

/*----------------------------------------------------------------------------*/
/* lock the mutex                                                             */
/*----------------------------------------------------------------------------*/
void Job::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pMutex->vMutexLock();
  
//  i8 rCode = pthread_mutex_lock(&m_xThreadMutex);
//
//  if (0 != rCode)
//    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* signal the mutex condition variable                                        */
/*----------------------------------------------------------------------------*/
void Job::vMutexSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pMutex->vCondSignal();

//  i8 rCode = pthread_cond_signal(&m_xThreadCond);
//
//  if (0 != rCode)
//    vWarn("pthread_cond_signal() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* unlock the mutex                                                           */
/*----------------------------------------------------------------------------*/
void Job::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pMutex->vMutexUnlock();

//  i8 rCode = pthread_mutex_unlock(&m_xThreadMutex);
//
//  if (0 != rCode)
//    vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* wait for mutex condition variable to be signalled                          */
/*----------------------------------------------------------------------------*/
void Job::vMutexWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
 
  m_pMutex->vCondWait();
  
//  i8 rCode = pthread_cond_wait(&m_xThreadCond,&m_xThreadMutex);
//
//  if (0 != rCode)
//    vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
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
/* report this job                                                            */
/*----------------------------------------------------------------------------*/
void Job::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  vTrace("Job %llx id %lls ackRemaining %lld\n"_Stz,
         this,
         m_iJobId,
         m_oAcksRemaining.iPrintOnlyValue());
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
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set the job trace                                                          */
/*----------------------------------------------------------------------------*/
void Job::vShow(s48K& s)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("%s: %s"_Stz,
         m_pJobClass ? (Stz*)m_pJobClass->m_aName : "<no jobClass>"_Stz,
         s.aStz());
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialize the job                                                         */
/*----------------------------------------------------------------------------*/
void Job::vInit(void)
{
  m_iJobId   = C_UNKNOWN;
  m_i_State  = C_UNKNOWN;
  m_eJobList = JL_NONE;
  
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
  m_oAcksRemaining.vCheckInit();
  m_pCurrentBuffer = nullptr;
  
  m_eState = JS_RUNNING;
  m_bWasReceived = false;
  
  if (CN_JOB_NODEINFO == eClass())
    m_pMutex = nullptr;
  else
    m_pMutex = new Mutex;
  
  m_bNoWait = false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* pre-send job function                                                      */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_send(Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* post-recv job function                                                     */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_recv(Job* pJob)
{
  pJob->m_pExecutingTask = nullptr;
  pJob->m_eJobList       = JL_NONE;
  pJob->m_pMutex         = nullptr;
  pJob->m_bWasReceived   = true;
}

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
