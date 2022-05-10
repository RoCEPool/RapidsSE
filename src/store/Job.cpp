/*============================================================================*/
/* Job.cpp -- a job in the system                                             */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Op.hpp"

#include "Job.hpp"
#include "File.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "Output.hpp"
#include "Api.hpp"

#include "TagConstants.h"

#include "SP_Gid.hpp"
#include "SP_Timer.hpp"

#include "SP_Pointer.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Job,CN_JOB)
__aNewFunction(Job,CN_JOB)
__vDeleteFunction(Job,CN_JOB)
__aFooterFunction(Job)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(Stz* szJobNm, JobId tParentId, JobId tOriginId, JobId tDistId)
: Operand(CN_JOB)
, CRM<Job>(this)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
 
  m_tJobId.vNextJobId();
  m_tParentId = tParentId;
  m_tDistId   = tDistId;
  m_tOriginId = tOriginId;

  m_sJobNm   = szJobNm;
  m_eJobKind = JK_NONE;

  m_bReapable = false;
  
  m_bWasForked = false;
  // -- a -------------------
  

  m_p_ResponseOp = nullptr;
  m_oResponseTimer.vStart();

  // -- b -------------------
  m_bWasReceived     = false;
  m_iParticipantNodeId = C_UNKNOWN;
  
  // -- c -------------------
  m_eJobList  = JL_NONE;
  m_eJobPhase = JP_NONE;
  m_eMutex = MX_ACTIVE;
  m_eError_job    = EC_NONE;
  m_pOutput_job = nullptr;
  
  // m_oJobTags
  // m_xResult

  // -- e -------------------
  m_pSingularOp  = nullptr;

  // -- g -------------------
  m_pTask     = nullptr;

  m_iTscParticipant = 0;
  
  m_iTscBeginning   = 0;
  m_iTscBegun       = 0;
  m_iTscExecuting   = 0;
  m_iTscExecuted    = 0;
  m_iTscEnding      = 0;
  m_iTscEnded       = 0;
  m_iTscCanceling   = 0;
  m_iTscCanceled    = 0;

  // -- -- -- -- -- --
      
  m_pJobMetricsChain  = nullptr;
  
  m_pMutex_job     = nullptr;
  m_bSystemCloser  = false;

  m_pStmtList = nullptr;
  
  _checkInitialised_(CN_JOB);
  
  AM::JOBS::vJobsActiveInc();
  
  m_oJobTags.vInsert(C_TAG_PATH,C_SE_PATH);

  AM::JLIST::vInsertJob(this);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (AM::JLIST::bFindJob(this))
    AM::JLIST::vRemoveJob(this);

//  if (bHasTag(C_TAG_LIFE) || JK_GENERIC == m_eJobKind) {
//
//    SP_TagSet spTagSet;
//    vPrint("Job Delete: %s\n"_Stz,spTagSet.sz(&m_oJobTags));
//    //m_oJobTags.vPrint();
//  }
    
  // -- -- -- -- -- --
  
  //! this assert may fail if a job completes before its references
  //! are completely returned
  if (JK_GENERIC != m_eJobKind && JK_NONE != m_eJobKind) {
    while (nReferences() || TS_RETIRED != m_pTask->eState() ) {
      vTrace("Napping\n"_Stz);
      AM::vNapMsec(10, eGetClassNo());
    }
  }

  RASSERT(JK_GENERIC == m_eJobKind ||
          JK_NONE    == m_eJobKind ||
          m_bReapable);
  
  if (m_pJobMetricsChain) {
    while (m_pJobMetricsChain) {
      JobMetrics* pJobMetricsNext = m_pJobMetricsChain->pNext();
      delete m_pJobMetricsChain;
      m_pJobMetricsChain = pJobMetricsNext;
    }

  }
    
  if (m_pTask) {
    m_pTask->vGrabTaskLock((Stz*)__func__);
    m_pTask->vState(TS_DELETING);
    m_pTask->vDropTaskLock((Stz*)__func__);
    delete m_pTask;

  }
  
  AM::JOBS::vJobsActiveDec();

  if (m_pMutex_job)
    vMutexDestroy();
  if (m_pOutput_job)
    vOutputDestroy();
  
  if (m_pStmtList)
    vStmtListDestroy();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Job::vPostRecv(void* aReceivedJob)
{
  _static_
  
  Job* pReceivedJob = _pJob(aReceivedJob);
  
  pReceivedJob->vPostRecv();
  
}

/*----------------------------------------------------------------------------*/
/* if the key is greater than object on the right, then move right            */
/*----------------------------------------------------------------------------*/
void Job::vPrintJobNode(SkipNode* pNode, int i_Level)
{
  _static_
  
  RASSERT(pNode);

  Job* pJob = _pJob(pNode->m_pObject);
  
  JobId tJobId = pJob->tJobId();
  
  vTrace("@%lld %p : %lld.%lld -> %p\n"_Stz
         ,i_Level
         ,pNode
         ,(i8)tJobId.uNodeId()
         ,(i8)tJobId.uJobNo()
         ,pNode->m_pRight[i_Level]
         );
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the number of active ops                                               */
/*----------------------------------------------------------------------------*/
i8 Job::nActiveOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  i8 nActive = * m_noActiveOps;
  
  return nActive;
}

/*----------------------------------------------------------------------------*/
/* decrement number of active op's                                            */
/*----------------------------------------------------------------------------*/
void Job::vActiveOpsDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  -- m_noActiveOps;
}

/*----------------------------------------------------------------------------*/
/* icrement number of active op's                                             */
/*----------------------------------------------------------------------------*/
void Job::vActiveOpsInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  ++ m_noActiveOps;
}

/*----------------------------------------------------------------------------*/
/* reset job for cascade reuse                                                */
/*----------------------------------------------------------------------------*/
void Job::vCascadeReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vDebugPoint();
  
  // -- a -------------------
  // m_sJobNm;
  // m_oTimer.vStart();

  // -- b -------------------
  m_bWasReceived     = false;
  m_iParticipantNodeId = C_UNKNOWN;

  // -- c -------------------
  m_eJobKind  = JK_NONE;
  m_eJobList  = JL_NONE;
  m_eJobPhase = JP_NONE;
  m_eMutex = MX_ACTIVE;
  
  // m_oJobTags
  // m_xResult ???
  
  // -- e -------------------
  if (m_pSingularOp) {
    delete m_pSingularOp;
    m_pSingularOp  = nullptr;
  }

  m_pTask->vState(TS_NONE);
  m_pTask->vSetInxPtr("job::entryPoint"_Stz);

  m_pOutput_job->vDrainBufferQueue(this);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
Stz* Job::szCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  Stz* szCmd = m_oJobTags.szValue(C_TAG_CMD);
  
  return szCmd;
}

/*----------------------------------------------------------------------------*/
/* set command                                                                */
/*----------------------------------------------------------------------------*/
void Job::vCmd(Stz* szCmd)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_oJobTags.vReplace(C_TAG_CMD,szCmd);
}

/*----------------------------------------------------------------------------*/
/* get distributed job id                                                     */
/*----------------------------------------------------------------------------*/
JobId Job::tDistId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  return m_tDistId;
}

/*----------------------------------------------------------------------------*/
/* set distributed job id                                                     */
/*----------------------------------------------------------------------------*/
void Job::vDistId(JobId tDistId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tDistId = tDistId;
}

/*----------------------------------------------------------------------------*/
/* generate distributed job id                                                */
/*----------------------------------------------------------------------------*/
void Job::vDistIdGenerate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tDistId.vNextJobId();
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Job::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* sen . print                                                                */
/*----------------------------------------------------------------------------*/
void Job::vDotPrint(Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  SP_Timer spTimer;
  
  m_oResponseTimer.vUpdate();
  
  vTrace("[%s: job %s %s] "
         "%s"
         "\n"_Stz
         
         ,AM::pGrid()->pThisNode()->szNodeNm()
         ,m_sJobNm.szStz()
         ,spTimer.sz(&m_oResponseTimer)
         
         ,szText
         
         ,C_VARGS_END
         );
}

/*----------------------------------------------------------------------------*/
/* get the sen entry code                                                     */
/*----------------------------------------------------------------------------*/
Stz* Job::szEntryPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sEntryPoint;
}

/*----------------------------------------------------------------------------*/
/* set the sen entry code                                                     */
/*----------------------------------------------------------------------------*/
void Job::vEntryPoint(Stz* szEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sEntryPoint = szEntry;
}

/*----------------------------------------------------------------------------*/
/* get folder path                                                            */
/*----------------------------------------------------------------------------*/
Stz* Job::szFolderPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sFolderPath;
}

/*----------------------------------------------------------------------------*/
/* set folder path                                                            */
/*----------------------------------------------------------------------------*/
void Job::vFolderPath(Stz* szFolderPath)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sFolderPath = szFolderPath;
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Job::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* test if this job has given ename tag                                       */
/*----------------------------------------------------------------------------*/
bool Job::bHasTag(Stz* szTagNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (m_oJobTags.bHasName(szTagNm))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test if this job has a <byPrompt> tag                                      */
/*----------------------------------------------------------------------------*/
bool Job::bHasByPromptTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bByPrompt = bHasTag(C_TAG_BYPROMPT);
  
  return bByPrompt;
}

/*----------------------------------------------------------------------------*/
/* get bHasStmtList                                                           */
/*----------------------------------------------------------------------------*/
bool Job::bHasStmtList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (m_pStmtList && m_pStmtList->nItems())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is job in error?                                                           */
/*----------------------------------------------------------------------------*/
bool Job::bIsInError(void)
{
  return m_eError_job == EC_NONE ? false : true;
}

/*----------------------------------------------------------------------------*/
/* are we on the origin?                                                      */
/*----------------------------------------------------------------------------*/
bool Job::bIsOnOrigin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iOrigin = iOriginNodeId();
  i8 iParticipant = iParticipantNodeId();
 
  if (iParticipant == iOrigin)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is this an originator job?                                                 */
/*----------------------------------------------------------------------------*/
bool Job::bIsOriginator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (JL_ORIGINATING == m_eJobList)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this an participant job?                                                 */
/*----------------------------------------------------------------------------*/
bool Job::bIsParticipant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (JL_PARTICIPATING == m_eJobList)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a pipeline job?                                                    */
/*----------------------------------------------------------------------------*/
bool Job::bIsPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (m_sJobNm.bSame("pipeline"_Stz) ||
      m_sJobNm.bSame("pipelineLocal"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this a local pipeline job?                                              */
/*----------------------------------------------------------------------------*/
bool Job::bIsPipelineLocal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (m_sJobNm.bSame("pipelineLocal"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set job error                                                              */
/*----------------------------------------------------------------------------*/
void Job::vJobError(eEC eErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_eError_job = eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* get job error                                                              */
/*----------------------------------------------------------------------------*/
eEC Job::eJobError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eError_job;
}

/*----------------------------------------------------------------------------*/
/* get the job id                                                             */
/*----------------------------------------------------------------------------*/
JobId Job::tJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_tJobId;
}

/*----------------------------------------------------------------------------*/
/* set the job id from distributed id                                         */
/*----------------------------------------------------------------------------*/
void  Job::vJobIdFromDist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tJobId = m_tDistId;
}

/*----------------------------------------------------------------------------*/
/* get the job kind                                                           */
/*----------------------------------------------------------------------------*/
eJK Job::eJobKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eJobKind;
}

/*----------------------------------------------------------------------------*/
/* set the job kind                                                           */
/*----------------------------------------------------------------------------*/
void Job::vJobKind(eJK eJobKind)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eJobKind = eJobKind;
}

/*----------------------------------------------------------------------------*/
/* get the job list                                                           */
/*----------------------------------------------------------------------------*/
eJL Job::eJobList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eJobList;
}

/*----------------------------------------------------------------------------*/
/* set the job list                                                           */
/*----------------------------------------------------------------------------*/
void Job::vJobList(eJL eJobList)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eJobList = eJobList;
}

/*----------------------------------------------------------------------------*/
/* add job metrics to chain                                                   */
/*----------------------------------------------------------------------------*/
void Job::vJobMetricsAdd(JobMetrics *pJobMetrics)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  pJobMetrics->vNext(m_pJobMetricsChain);
  
  m_pJobMetricsChain = pJobMetrics;
}

/*----------------------------------------------------------------------------*/
/* get job metrics chain                                                      */
/*----------------------------------------------------------------------------*/
JobMetrics* Job::pJobMetricsChain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  return m_pJobMetricsChain;
}

/*----------------------------------------------------------------------------*/
/* get the job name                                                           */
/*----------------------------------------------------------------------------*/
Stz* Job::szJobNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sJobNm;
}

/*----------------------------------------------------------------------------*/
/* set the job name                                                           */
/*----------------------------------------------------------------------------*/
void Job::vJobNm(Stz* szJobNm)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sJobNm = szJobNm;
}

/*----------------------------------------------------------------------------*/
/* get the job phase                                                          */
/*----------------------------------------------------------------------------*/
eJP Job::eJobPhase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eJobPhase;
}

/*----------------------------------------------------------------------------*/
/* set the job phase                                                          */
/*----------------------------------------------------------------------------*/
void Job::vJobPhase(eJP eJobPhase)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eJobPhase = eJobPhase;
}

/*----------------------------------------------------------------------------*/
/* get the job state                                                          */
/*----------------------------------------------------------------------------*/
eMX Job::eJobState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eMutex;
}

/*----------------------------------------------------------------------------*/
/* set the job state                                                          */
/*----------------------------------------------------------------------------*/
void Job::vJobState(eMX eJobState)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eMutex = eJobState;
}

/*----------------------------------------------------------------------------*/
/* get ename set                                                              */
/*----------------------------------------------------------------------------*/
TagSet* Job::pJobTags(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return & m_oJobTags;
}

/*----------------------------------------------------------------------------*/
/* get last job id                                                            */
/*----------------------------------------------------------------------------*/
JobId Job::tLastJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_tLastJobId;
}

/*----------------------------------------------------------------------------*/
/* set last job id                                                            */
/*----------------------------------------------------------------------------*/
void Job::vLastJobId(JobId tLastJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tLastJobId = tLastJobId;
}

/*----------------------------------------------------------------------------*/
/* create a new mutex for this job                                            */
/*----------------------------------------------------------------------------*/
void Job::vMutexCreate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(! m_pMutex_job);
  
  m_pMutex_job = new Mutex(this);
}

/*----------------------------------------------------------------------------*/
/* destroy this job's mutex                                                   */
/*----------------------------------------------------------------------------*/
void Job::vMutexDestroy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(m_pMutex_job);
  
  delete m_pMutex_job;
  m_pMutex_job = nullptr;
}

/*----------------------------------------------------------------------------*/
/* lock this job's mutex                                                      */
/*----------------------------------------------------------------------------*/
void Job::vMutexLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_pMutex_job->vLock(this);
}

/*----------------------------------------------------------------------------*/
/* signal this job's mutex                                                    */
/*----------------------------------------------------------------------------*/
void Job::vMutexSignal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_pMutex_job->vSignal(this);
}

/*----------------------------------------------------------------------------*/
/* unlock this job's mutex                                                    */
/*----------------------------------------------------------------------------*/
void Job::vMutexUnlock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_pMutex_job->vUnlock(this);
}

/*----------------------------------------------------------------------------*/
/* wait on this job's mutex                                                   */
/*----------------------------------------------------------------------------*/
void Job::vMutexWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_pMutex_job->vWait(this);
}

/*----------------------------------------------------------------------------*/
/* get output processor                                                       */
/*----------------------------------------------------------------------------*/
Output* Job::pOutput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pOutput_job;
}

/*----------------------------------------------------------------------------*/
/* begin output processor                                                     */
/*----------------------------------------------------------------------------*/
void Job::vOutputBeginProc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pOutput_job->vBeginProcOneCmd();
}

/*----------------------------------------------------------------------------*/
/* create output processor                                                    */
/*----------------------------------------------------------------------------*/
void Job::vOutputCreate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(! m_pOutput_job);
  
  m_pOutput_job = new Output(this);
}

/*----------------------------------------------------------------------------*/
/* destroy output processor                                                   */
/*----------------------------------------------------------------------------*/
void Job::vOutputDestroy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  delete m_pOutput_job;
  m_pOutput_job = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set if output is enabled                                                   */
/*----------------------------------------------------------------------------*/
void Job::vOutputEnabled(bool bEnabled)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* end output processor                                                       */
/*----------------------------------------------------------------------------*/
void Job::vOutputEndProc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pOutput_job->vEndProcOneCmd();
}

/*----------------------------------------------------------------------------*/
/* get the parent originating job (on origin only)                            */
/*----------------------------------------------------------------------------*/
Job* Job::pOriginatingParentJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
      
  //RASSERT(bIsOnOrigin());
  
  Job* pParentJob = AM::OLIST::pFindJob(m_tDistId);
  
  return pParentJob;
}

/*----------------------------------------------------------------------------*/
/* get origin job id                                                          */
/*----------------------------------------------------------------------------*/
JobId Job::tOriginId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  return m_tOriginId;
}

/*----------------------------------------------------------------------------*/
/* set distributed job id                                                     */
/*----------------------------------------------------------------------------*/
void Job::vOriginId(JobId tOriginId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tOriginId = tOriginId;
}

/*----------------------------------------------------------------------------*/
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iOriginNodeId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  i8 iOriginNodeId = m_tOriginId.uNodeId();
  
  return iOriginNodeId;
}

/*----------------------------------------------------------------------------*/
/* get name of node this came from                                            */
/*----------------------------------------------------------------------------*/
Stz* Job::szOriginNodeNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 i_Node = iOriginNodeId();
  
  Stz* szNodeNm = AM::pGrid()->pFindNodeById(i_Node)->szNodeNm();
  
  return szNodeNm;
}

/*----------------------------------------------------------------------------*/
/* get parent job id                                                          */
/*----------------------------------------------------------------------------*/
JobId Job::tParentId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  return m_tParentId;
}

/*----------------------------------------------------------------------------*/
/* set parent job id                                                          */
/*----------------------------------------------------------------------------*/
void Job::vParentId(JobId tParentId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tParentId =tParentId;
}

/*----------------------------------------------------------------------------*/
/* get the participant node                                                   */
/*----------------------------------------------------------------------------*/
i8 Job::iParticipantNodeId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iParticipantNodeId;
}

/*----------------------------------------------------------------------------*/
/* set the job id                                                             */
/*----------------------------------------------------------------------------*/
void Job::vParticipantNodeId(i8 iNodeId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_iParticipantNodeId = iNodeId;
}

/*----------------------------------------------------------------------------*/
/* get pipeline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* Job::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Task* pTask = this->pTask();
  LogicUnit* pLogicUnit = pTask->pLogicUnit();

  Pipeline* pPipeline = (* pLogicUnit->m_xpPipeline);
    
  return pPipeline;
}

/*----------------------------------------------------------------------------*/
/* see job dump                                                               */
/*----------------------------------------------------------------------------*/
void Job::vPrintJobContents(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  SP_Gid spJobId;
  SP_Timer spTimer;

  SP_Pointer spvPipeline;
  SP_Pointer spvSingularOp;
  SP_Pointer spvOriginatingTask;
  SP_Pointer spvTask;
  SP_Pointer spvJobLogicUnit;

  Task* pTask = this->pTask();
  LogicUnit* pLogicUnit = pTask->pLogicUnit();

  vTrace("See Job %p::\n"_Stz,this);

  vTrace(
         
         "            sJobId "  "%s"    " \n"  // 0
         "            sJobNm "  "%s"    " \n"  // 1
         "          sJobKind "  "%s"    " \n"  // 2

         "              sCmd "  "%s"    " \n"  // 3
         
         "     iOriginNodeId "  "%lld"  " \n"  // 4

         "            iCmdNo "  "%lld"  " \n"  // 6

         "      bWasReceived "  "%s"    " \n"  // 7
         "     i_Participant "  "%lld"  " \n"  // 8

         "         eJobState "  "%s"    " \n"  // 10
         "         eJobPhase "  "%s"    " \n"  // 11
         "          eJobList "  "%s"    " \n"  // 12
         
         "            oTimer "  "%s"    " \n"  // 13
         "            eError "  "%s"    " \n"  // 15

         "       pSingularOp "  "%s"    " \n"  // 17

 
         "             pTask "  "%s"    " \n"  // 21
        
         "        pLogicUnit "  "%s"    " \n"  // 22

         ""_Stz
         
         ,spJobId.sz(m_tJobId.iAsI8())     // 0
         ,m_sJobNm.szStz()           // 1
         ,aJK[m_eJobKind]            // 2
         ,szCmd()                    // 3
         ,iOriginNodeId()            // 4

         ,m_tParentId.uJobNo()          // 6
         ,m_bWasReceived ? "true" : "false" // 7
         ,m_iParticipantNodeId            // 8
         ,aMX[m_eMutex]           // 10
         ,aJP[m_eJobPhase]           // 11
         ,aJL[m_eJobList]            // 12
         ,spTimer.sz(& m_oResponseTimer)     // 13
         ,aEC[m_eError_job]          // 15
         ,spvSingularOp.sz(m_pSingularOp)  // 17
         ,spvTask.sz(m_pTask)        // 21
         ,spvJobLogicUnit.sz(pLogicUnit) // 22
         
         );
  
  m_oJobTags.vContentDump();

  vTrace(";;\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Job::vPostRecv(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  m_bWasReceived = true;
  m_eJobList     = JL_NONE;
    
  m_pTask        = nullptr;
  m_p_ResponseOp = nullptr;
  m_pOutput_job  = nullptr;
  m_pSingularOp  = nullptr;
  m_pMutex_job   = nullptr;
  m_pJobMetricsChain = nullptr;
  m_pStmtList = nullptr;

  m_okLock.vResetLock();
  m_okLock.vResetStats();

  if (bHasTag(C_SEE_JOB))
    vPrintJobContents();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Job::vPrintJobLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  SP_Gid spJobGid;
      
  vTrace("  Job %s %s "_Stz
         ,spJobGid.sz(this->tJobId().iAsI8())
         ,this->szJobNm()
         );

  vTrace("originNodeId %lld command %lld "_Stz
         ,this->iOriginNodeId()
         ,this->m_tParentId.uJobNo()
         );

  if (C_UNKNOWN != this->iParticipantNodeId())
    vTrace("partNode %lld "_Stz
           ,this->iParticipantNodeId()
           );

  LogicUnit* pLogicUnit = m_pTask->pLogicUnit();
  
  if (pLogicUnit) {
    if (pLogicUnit->m_xpVertexSpec.bIsNotNull())
      vTrace("%s "_Stz
             ,pLogicUnit->m_xpVertexSpec->szVertexNm()
             );
    vTrace("%s "_Stz
           ,aLH[pLogicUnit->m_eDdlLocked]
           );
  }
  
  vTrace("AcksRmg %lld (out of %lld)"_Stz
         ,this->pTask()->iAcksRmg_assert_only_()
         ,this->pTask()->iAcksOriginal()
         );

  vTrace("logicUnit %p task %p"_Stz
         ,pLogicUnit
         ,m_pTask
         );

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get progress timer                                                         */
/*----------------------------------------------------------------------------*/
Timer* Job::pProgressTimer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return & m_oProgressTimer;
}

/*----------------------------------------------------------------------------*/
/* is job reapable?                                                           */
/*----------------------------------------------------------------------------*/
bool Job::bReapable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_bReapable;
}

/*----------------------------------------------------------------------------*/
/* set job reapable?                                                          */
/*----------------------------------------------------------------------------*/
void Job::vReapable(bool bReapable)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bReapable = bReapable;
}

/*----------------------------------------------------------------------------*/
/* get pResponseOp                                                             */
/*----------------------------------------------------------------------------*/
Op* Job::p_ResponseOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_p_ResponseOp;
}

/*----------------------------------------------------------------------------*/
/* set pResponseOp                                                            */
/*----------------------------------------------------------------------------*/
void Job::vResponseOp(Op* p_ResponseOp)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_p_ResponseOp = p_ResponseOp;
}

/*----------------------------------------------------------------------------*/
/* get elapsed time in milliseconds                                           */
/*----------------------------------------------------------------------------*/
i8 Job::iResponseTimerMsec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oResponseTimer.iMilliseconds();
}

/*----------------------------------------------------------------------------*/
/* get timer start                                                            */
/*----------------------------------------------------------------------------*/
i8 Job::iResponseTimerStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oResponseTimer.iStart();
}

/*----------------------------------------------------------------------------*/
/* start/reset timer                                                          */
/*----------------------------------------------------------------------------*/
void Job::vResponseTimerStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_oResponseTimer.vStart();
}

/*----------------------------------------------------------------------------*/
/* update timer                                                               */
/*----------------------------------------------------------------------------*/
void Job::vResponseTimerUpdate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_oResponseTimer.vUpdate();
}

/*----------------------------------------------------------------------------*/
/* get the singular operator                                                  */
/*----------------------------------------------------------------------------*/
Op* Job::pSingularOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pSingularOp;
}

/*----------------------------------------------------------------------------*/
/* set the singular operator                                                  */
/*----------------------------------------------------------------------------*/
void Job::vSingularOp(Op* pSingularOp)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pSingularOp = pSingularOp;
}

/*----------------------------------------------------------------------------*/
/* append cascade to statement list                                           */
/*----------------------------------------------------------------------------*/
void Job::vStmtListAppend(Stz* szStmt)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //vTrace("vStmtListAppend: %s\n"_Stz,szStmt);
  Statement* pStatement = new Statement(szStmt);
  
  m_pStmtList->vAppend(pStatement);
}

/*----------------------------------------------------------------------------*/
/* create a statement list for this job                                       */
/*----------------------------------------------------------------------------*/
void Job::vStmtListCreate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(! m_pStmtList);
  
  m_pStmtList = new StmtList;
}

/*----------------------------------------------------------------------------*/
/* destroy this job's statement list                                          */
/*----------------------------------------------------------------------------*/
void Job::vStmtListDestroy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(m_pStmtList);
  
  delete m_pStmtList;
  m_pStmtList = nullptr;
}

/*----------------------------------------------------------------------------*/
/* pop top statement from statement list                                      */
/*----------------------------------------------------------------------------*/
void Job::vStmtListPop(String* pString)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Statement* pStatement = m_pStmtList->pPop();
  Stz* szStmt = pStatement->szText();
  
  pString->vCopy(szStmt);
  
  delete pStatement;
}

/*----------------------------------------------------------------------------*/
/* is system closer?                                                          */
/*----------------------------------------------------------------------------*/
bool Job::bSystemCloser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_bSystemCloser;
}

/*----------------------------------------------------------------------------*/
/* set system closer?                                                         */
/*----------------------------------------------------------------------------*/
void Job::vSystemCloser(bool bSystemCloser)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bSystemCloser = bSystemCloser;
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
Task* Job::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Job::vTask(Task* pTask)
{
  ASSERT(! pTask || ! m_pTask);
  
  if (! pTask)
    vDebugPoint();
  
  m_pTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* create task for job                                                        */
/*----------------------------------------------------------------------------*/
void Job::vTaskCreate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pTask = new Task(szJobNm(),this);
    
  m_pTask->vSetInxPtr("job::entryPoint"_Stz);
  
  vResponseTimerStart();
}

/*----------------------------------------------------------------------------*/
/* get the participant tsc                                                    */
/*----------------------------------------------------------------------------*/
i8 Job::iTscBeginning(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscBeginning;
}

/*----------------------------------------------------------------------------*/
/* set the beginning tsc                                                      */
/*----------------------------------------------------------------------------*/
void Job::vTscBeginning(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscBeginning = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the beginning tsc                                                      */
/*----------------------------------------------------------------------------*/
i8 Job::iTscBegun(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscBegun;
}

/*----------------------------------------------------------------------------*/
/* set the begun tsc                                                          */
/*----------------------------------------------------------------------------*/
void Job::vTscBegun(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscBegun = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the canceled tsc                                                       */
/*----------------------------------------------------------------------------*/
i8 Job::iTscCanceled(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscCanceled;
}

/*----------------------------------------------------------------------------*/
/* set the canceled tsc                                                       */
/*----------------------------------------------------------------------------*/
void Job::vTscCanceled(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscCanceled = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the cancelling tsc                                                     */
/*----------------------------------------------------------------------------*/
i8 Job::iTscCanceling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscCanceling;
}

/*----------------------------------------------------------------------------*/
/* set the canceling tsc                                                      */
/*----------------------------------------------------------------------------*/
void Job::vTscCanceling(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscCanceling = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the ended tsc                                                          */
/*----------------------------------------------------------------------------*/
i8 Job::iTscEnded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscEnded;
}

/*----------------------------------------------------------------------------*/
/* set the ended tsc                                                          */
/*----------------------------------------------------------------------------*/
void Job::vTscEnded(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscEnded = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the ending tsc                                                         */
/*----------------------------------------------------------------------------*/
i8 Job::iTscEnding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscEnding;
}

/*----------------------------------------------------------------------------*/
/* set the ending tsc                                                         */
/*----------------------------------------------------------------------------*/
void Job::vTscEnding(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscEnding = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the executed tsc                                                       */
/*----------------------------------------------------------------------------*/
i8 Job::iTscExecuted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscExecuted;
}

/*----------------------------------------------------------------------------*/
/* set the executed tsc                                                       */
/*----------------------------------------------------------------------------*/
void Job::vTscExecuted(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscExecuted = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the executing tsc                                                      */
/*----------------------------------------------------------------------------*/
i8 Job::iTscExecuting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscExecuting;
}

/*----------------------------------------------------------------------------*/
/* set the executing tsc                                                      */
/*----------------------------------------------------------------------------*/
void Job::vTscExecuting(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscExecuting = iTsc;
}

/*----------------------------------------------------------------------------*/
/* get the participant tsc                                                    */
/*----------------------------------------------------------------------------*/
i8 Job::iTscParticipant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iTscParticipant;
}

/*----------------------------------------------------------------------------*/
/* set the participant tsc                                                    */
/*----------------------------------------------------------------------------*/
void Job::vTscParticipant(i8 iTsc)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iTscParticipant = iTsc;
}

/*----------------------------------------------------------------------------*/
/* verify job is composed correctly and therefore capable of being executed   */
/*----------------------------------------------------------------------------*/
void Job::vVerifyJobComposition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);

  RASSERT(EC_NONE == eValidHeader());
  RASSERT(CN_JOB == eGetClassNo());
  aFooter()->vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* was job forked (or executed)?                                              */
/*----------------------------------------------------------------------------*/
bool Job::bWasForked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_bWasForked;
}

/*----------------------------------------------------------------------------*/
/* set job forked                                                             */
/*----------------------------------------------------------------------------*/
void Job::vWasForked(bool bWasForked)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bWasForked = bWasForked;
}

/*----------------------------------------------------------------------------*/
/* get was received flag                                                      */
/*----------------------------------------------------------------------------*/
bool Job::bWasReceived(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_bWasReceived;
}

/*----------------------------------------------------------------------------*/
/* set ws received flag                                                       */
/*----------------------------------------------------------------------------*/
void Job::vWasReceived(bool bWasReceived)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bWasReceived = bWasReceived;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
