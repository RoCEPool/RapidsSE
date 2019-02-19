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
Job::Job(eCN eClassNo, Session* pSession)
: Object(eClassNo), JobDicPath(pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //vTrace("%s: CREATING JOB %p %s\n"_Stz,
  //       gpAttachMap->m_pGrid->pThisNode()->aNodeName(), this, aCN[eClass()]);
  
  m_iJobId    = ++gpAttachMap->m_ixNextJobId;
  m_iParentId = C_UNKNOWN;
  m_eSeed     = SS_ENUMS;
  m_i_State   = C_UNKNOWN;
  m_i_Slot    = C_UNKNOWN;
  m_eJobPhase = JP_NONE;
  m_eJobList  = JL_NONE;
  m_eJobState = JS_RUNNING;
  m_bNoWait   = false;
  m_bWasReceived = false;
  
  //m_oAcksRemaining
  
  m_eJobTrace = JT_NONE;
  if (gpAttachMap->m_bMarkJobAll || gpAttachMap->m_bMarkJob[eClass()])
    m_bMarkActivity = true;
  else
    m_bMarkActivity = false;
  m_i_Activity   = C_UNKNOWN;
  
  if ( gpAttachMap->m_oMarkAction.bAll() ||
       gpAttachMap->m_oMarkAction.bFlagged(eClass()) )
    m_bMarkAction  = true;
  else
    m_bMarkAction  = false;
  m_i_ActionRoot = C_UNKNOWN;

  if ( gpAttachMap->m_oMarkTrace.bAll() ||
      gpAttachMap->m_oMarkTrace.bFlagged(eClass()) )
    m_bMarkTrace  = true;
  else
    m_bMarkTrace  = false;

  m_pJobClass        = nullptr;
  m_pExecutingTask   = nullptr;
  m_pOriginatingTask = nullptr;
  
  m_iParticipantNode  = C_UNKNOWN;
  m_oAcksRemaining.vCheckInit();
  
  m_pCurrentBuffer = nullptr;
  m_pSendWith = nullptr;
  
  if (CN_JOB_NODEINFO == eClass())
    m_pMutexJob = nullptr;
  else
    m_pMutexJob = new Mutex;

  if (m_bMarkActivity)
    ActivityMap::vMarkInfo("::Job"_Stz,""_Stz);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ActionSequence::vAdd(this,AN_DESTRUCTOR);
  
  delete m_pMutexJob;
  
  if (m_bMarkActivity) {
    ActivityMap::vMarkInfo("~Job %s"_Stz,aCN[eClass()]);
    Job::vMarkJob();
  }
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this a foreign job?                                                     */
/*----------------------------------------------------------------------------*/
bool Job::bIsForeign(i8 iJobId)
{
  if (C_UNKNOWN == iJobId)
    return false;
  
  i8 iOrigin = Job::iOriginatorNode(iJobId);
  
  bool bIsForeign =
   iOrigin == gpAttachMap->m_pGrid->m_iThisNode ? false : true;
  
  return bIsForeign;
}

/*----------------------------------------------------------------------------*/
/* get the job no component this job                                          */
/*----------------------------------------------------------------------------*/
i8 Job::iJobNo(i8 iJobId)
{
  i8 iJobNo = BitLib::uMaskOffUpper16(iJobId);
  
  return iJobNo;
}

/*----------------------------------------------------------------------------*/
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iNodeNo(i8 iJobId)
{
  i8 iNodeNo = BitLib::uMaskOffLower48(iJobId);
  
  iNodeNo >>= 48;
  
  return iNodeNo;
}

/*----------------------------------------------------------------------------*/
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iOriginatorNode(i8 iJobId)
{
  i8 iOriginatorNode = BitLib::uMaskOffLower48(iJobId);
  iOriginatorNode >>= 48;
  
  return iOriginatorNode;
}

/*----------------------------------------------------------------------------*/
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iAsNodeNo_Bits(i8 iNodeNo)
{
  i8 NodeNo_Bits = iNodeNo << 48;
  
  return NodeNo_Bits;
}

/*----------------------------------------------------------------------------*/
/* state diagram                                                              */
/*----------------------------------------------------------------------------*/
void Job::vStateDiagram(s256& sDiagram, eCN eJobClass)
{
  TaskList* pExecuting     = gpAttachMap->m_pQueues->m_pExecutingSet;
  JobList*  pOriginating   = gpAttachMap->m_pOriginatingJobs;
  JobList*  pParticipating = gpAttachMap->m_pParticipatingJobs;

  sDiagram = ""_Stz;
  
  switch (eJobClass) {
     
    // generate state diagram for export job
    case Base::CN_JOB_EXPORT: {
      
      i8 iDiagram = ++gpAttachMap->m_ixNextDiagram;
      
      // whats in the system relevant to export?
      bool bKE_received   = pExecuting->bHasJobClass(CN_JOB_KIDEXPORT,false);
      bool bE_neutral     = pExecuting->bHasJobClass(CN_JOB_EXPORT,false);
      bool bE_originator  = pOriginating->bHasJobClass(CN_JOB_EXPORT);
      bool bE_participant = pParticipating->bHasJobClass(CN_JOB_EXPORT);
      bool bE_received    = pExecuting->bHasJobClass(CN_JOB_EXPORT,true);
      bool bSE_neutral    = pExecuting->bHasJobClass(CN_JOB_SUBEXPORTER,false);
      
      // form the state diagram
      sDiagram.vSprint("%03lld# "_Stz,iDiagram);
      
      if (bKE_received)         { sDiagram.vConcat("KE`"_Stz);  }
      if (bE_originator)        { sDiagram.vConcat("[E]"_Stz);  }
      if (! bE_originator &&
          bE_neutral)           { sDiagram.vConcat("E"_Stz);    }
      if (bE_participant)       { sDiagram.vConcat("(E`)"_Stz); }
      if (! bE_participant &&
          bE_received)          { sDiagram.vConcat("E`"_Stz);   }

      if (bSE_neutral)          { sDiagram.vConcat("SE"_Stz);   }
      
      sDiagram.vConcat(":"_Stz);
      
      if (bE_participant)       { sDiagram.vConcat("(E`)"_Stz); }
      if (! bE_participant &&
          bE_received)          { sDiagram.vConcat("E`"_Stz);   }
      if (bSE_neutral)          { sDiagram.vConcat("SE"_Stz);   }
      
      return;
    }
      
    default:
      sDiagram.vSprint("Warning: No diagram for class %lld %s"_Stz,
                       eJobClass,aCN[eJobClass]);
  }
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
/* write an activity map string about this job                                */
/*----------------------------------------------------------------------------*/
void Job::vMarkJob(void)
{
  s48K sJobMark;
  
  sJobMark.vSprintCat("Job %03lld %s state %lld slot %lld\n"_Stz,
                      m_iJobId,
                      aCN[eClass()],
                      m_i_State,
                      m_i_Slot);
  sJobMark.vSprintCat("  phase %s state %s list %s"_Stz,
                      aJP[m_eJobPhase],
                      aJS[m_eJobState],
                      aJL[m_eJobList]);
  if (m_bWasReceived)
    sJobMark.vSprintCat(" (recv from %s)"_Stz,
                        szNodeNm());

  sJobMark.vConcat("\n"_Stz);

  ActivityMap::vMarkString(m_iJobId,sJobMark);
}

/*----------------------------------------------------------------------------*/
/* write a mark about this job                                                */
/*----------------------------------------------------------------------------*/
void Job::vMarkJob(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);

#ifdef WITH_ACTIVITY_MAP
  if ( ! gpActivityMap )
    return;
  
  va_list xArgs;
  
  va_start(xArgs,szFormat);
  
  ActivityMap::vMarkJob(this,szFormat,xArgs);
  
  va_end(xArgs);
#endif
  
}

/*----------------------------------------------------------------------------*/
/* lock the mutex                                                             */
/*----------------------------------------------------------------------------*/
void Job::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pMutexJob->vMutexLock();
  
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
  
  m_pMutexJob->vCondSignal();

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

  m_pMutexJob->vMutexUnlock();

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
 
  m_pMutexJob->vCondWait();
  
//  i8 rCode = pthread_cond_wait(&m_xThreadCond,&m_xThreadMutex);
//
//  if (0 != rCode)
//    vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* get name of node this came from                                            */
/*----------------------------------------------------------------------------*/
Stz* Job::szNodeNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 i_Node = iOriginatorNode();
  
  Stz* szNodeNm = gpAttachMap->m_pGrid->pGetNode(i_Node)->aNodeName();
  
  return szNodeNm;
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
//void Job::vShow(s48K& s)
//{
//  _precondition_(_ADDRESS_,_MAGIC_);
//  
//  vTrace("%s: %s"_Stz,
//         m_pJobClass ? (Stz*)m_pJobClass->m_aName : "<no jobClass>"_Stz,
//         s.aStz());
//}
void Job::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vTrace("%s %s parentId %03lld seed %s "
         "i_State %03lld i_Slot %lld WasReceived %lld\n"_Stz,
         aCN[eClass()],
         m_pJobClass ? (Stz*)m_pJobClass->m_aName : "<no jobClass>"_Stz,
         m_iParentId,
         aSS[m_eSeed],
         
         m_i_State, m_i_Slot, (i8)m_bWasReceived
         );
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* check if the cluster has started                                           */
/*----------------------------------------------------------------------------*/
eEC Jobs::eJob_checkCluster(Interface::Session* pSession)
{
  if (gpAttachMap->m_pGrid->m_bHasStarted)
    return EC_NONE;
  else
    return EC_CLUSTER_NOT_CONNECTED;
}

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
  pJob->m_pMutexJob      = nullptr;
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
