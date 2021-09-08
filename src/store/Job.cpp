/*============================================================================*/
/* Job.cpp -- a job in the system                                             */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

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
#include "Session.hpp"

#include "TagConstants.h"

#include "SP_Gid.hpp"
#include "SP_Timer.hpp"

#include "SP_Pointer.hpp"

Atomic gixJob;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job* STORE::_pJob(void* aAddr)
{
  Job* pJob = reinterpret_cast<Job*>(aAddr);
  
  if (pJob && ! _bIsValidJob(pJob->eGetClassNo()) ) {
      vTrace("_pJob %p got class %s\n"_Stz,
             pJob,
             aCN[pJob->eGetClassNo()]);
    vDebugPoint();
  }
  //RASSERT( ! pJob || _bIsValidJob(pJob->eGetClassNo()) );
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job* STORE::_pJob(i8 iAddr)
{
  Job* pJob = reinterpret_cast<Job*>(iAddr);
  
  //  if ( pJob && ! _bIsValidJob(pJob->eGetClassNo() )
  //    i8 iDebug = 1;
  
  RASSERT( ! pJob || _bIsValidJob(pJob->eGetClassNo()) );
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* is given class a valid job?                                                */
/*----------------------------------------------------------------------------*/
bool STORE::_bIsValidJob(eCN eClass)
{
  if (eClass >= CN_JOB && eClass <= CN_JOB__FIND)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job::operator new(size_t)
{
  Job* pJob =
  static_cast<Job*>(Heaps::pAllocate(CN_JOB));
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job::operator delete(void* aAddress)
{
  Job* pJob = static_cast<Job*>(aAddress);
  
  Heaps::vFree(pJob);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(i8 i_Origin,     // originated on which node
         i8 i_Session,    // which session in session array
         i8 iCmdNo        // the sequence no of the request from that session)
        )
: Object(CN_JOB)
, LifeGuard_t<Job>(this,CN_JOB)
, CRM<Job>(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
  
  m_bReapable  = false;
  
  m_eSeed      = SS_ENUMS;
  m_pSeedWord  = nullptr;
  
  // -- a -------------------
  
  m_i_Origin     = i_Origin;
  m_i_Session    = i_Session;
  m_iCmdNo = iCmdNo;

  // m_sJobNm;
  m_oTimer.vStart();

  // -- b -------------------
  m_bWasReceived     = false;
  m_i_Participant = C_UNKNOWN;
  
  // -- c -------------------
  m_eJobKind  = JK_NONE;
  m_eJobList  = JL_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobState = JS_ACTIVE;
  m_eError_job    = EC_NONE;
  
  // -- d -------------------
  // revised job/op model
  m_pJobLogicUnit = nullptr;

  
  // m_oJobTags
  // m_xResult

  // -- e -------------------
  m_pSingularOp  = nullptr;

  // -- f -------------------
  m_iOriginatingId     = C_UNKNOWN;

  // -- g -------------------
  m_oAcksRmg_job.vCheckInit();
  m_pTask     = nullptr;

  // -- -- -- -- -- --
      
  
  _checkInitialised_(CN_JOB);

  vNewJobLogicUnit();
  
  m_oJobTags.vInsert(C_TAG_DISTID,iJobId());
  
  
  ++ gixJob;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB);
}

/*----------------------------------------------------------------------------*/
/* constructor (for job__find)                                  TO BE REMOVED */
/*----------------------------------------------------------------------------*/
Job::Job(eCN eClassNo)
: Object(eClassNo)
, CRM<Job>(this)
, LifeGuard_t<Job>(this,CN_JOB)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
    
  m_bReapable  = false;

  m_i_Origin     = C_UNKNOWN;
  m_i_Session    = C_UNKNOWN;
  m_iCmdNo = C_UNKNOWN;

  m_eSeed      = SS_ENUMS;
  m_pSeedWord  = nullptr;

  m_eJobKind  = JK_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobList  = JL_NONE;
  m_eJobState = JS_ACTIVE;
  
  m_bWasReceived = false;
  
  m_oTimer.vStart();
  
  //m_oAcksRmg_task
  
  m_pTask              = nullptr;
  m_iOriginatingId     = C_UNKNOWN;

  m_i_Participant = C_UNKNOWN;
  m_oAcksRmg_job.vCheckInit();
  
  m_pSingularOp  = nullptr;
  
  
  m_pJobLogicUnit = nullptr;
  m_eError_job    = EC_NONE;
 
  //m_xResult
  // -- -- -- -- -- --
      
  ++ gixJob;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  
//  if (bHasTag(C_TAG_LIFE) || JK_GENERIC == m_eJobKind) {
//
//    SP_TagSet spTagSet;
//    vPrint("Job Delete: %s\n"_Stz,spTagSet.sz(&m_oJobTags));
//    //m_oJobTags.vPrint();
//  }
    
  // -- -- -- -- -- --
  
  //! this assert may fail if a job completes before its references
  //! are completely returned
  while (nReferences()) {
    vTrace("Napping\n"_Stz);
    AM::vNapMsec(10, eGetClassNo());
  }
    
  if (m_pJobLogicUnit) {
    //AM::LOGICUNIT::vDelete(&m_pJobLogicUnit);
    m_pJobLogicUnit->vGuardedDelete(CN_LOGICUNIT,&m_pJobLogicUnit,
                                    CN_JOB,this,(Stz*)__func__);
  }
  else if (C_UNKNOWN != m_i_Origin) {
    vDebugPoint();
  }
  
  if (AM::SYSARGS::pTagSet()->bHasName("<with_job_reaping>"_Stz)) {

    if (m_pTask) {
      m_pTask->vState(TS_DELETING);
      m_pTask->vGuardedDelete(CN_TASK,&m_pTask,
                              CN_JOB,this,(Stz*)__func__);

    }
  }
}

/*----------------------------------------------------------------------------*/
/* this executing / tls job                                                   */
/*----------------------------------------------------------------------------*/
Job* Job::pThisTLSJob(void)
{
  Job* pJob = AM::pThisTLSJob();

  return pJob;
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
   iOrigin == AM::iThisNodeNo() ? false : true;
  
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
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Job::vPostRecv(void* aReceivedJob)
{
  _static_
  
  Job* pReceivedJob = _pJob(aReceivedJob);

  pReceivedJob->vInductIntoNode(pReceivedJob);
  
  pReceivedJob->vWasReceived(true);
  
  pReceivedJob->vJobList(JL_NONE);
  
  pReceivedJob->vSeedWord(nullptr);
  
  pReceivedJob->vJobTask(nullptr);

  // revised job/op model
  pReceivedJob->vNullJobLogicUnit();

  pReceivedJob->m_pSingularOp = nullptr;


  
  if (pReceivedJob->bHasTag(C_SEE_JOB))
    pReceivedJob->vSee();
  
  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the number of acks originally required by the job                      */
/*----------------------------------------------------------------------------*/
i8 Job::iAcksOriginal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //RASSERT(! AM::iTaskAcksRmg_assert_only_(m_pTask));
  
  i8 iAcksOriginal = m_oAcksRmg_job.nOriginalAcks();
  
  return iAcksOriginal;
}

/*----------------------------------------------------------------------------*/
/* get the number of acks required by the job                                 */
/*----------------------------------------------------------------------------*/
i8 Job::iAcksRmg_printOnlyValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //RASSERT(! AM::iTaskAcksRmg_assert_only_(m_pTask));
  
  i8 iAcksRmg = m_oAcksRmg_job.iPrintOnlyValue();
  
  return iAcksRmg;
}

/*----------------------------------------------------------------------------*/
/* set the number of acks required by the job                                 */
/*----------------------------------------------------------------------------*/
void Job::vAcksRmg(i8 nAcks)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //RASSERT(! AM::iTaskAcksRmg_assert_only_(m_pTask));
  
  m_oAcksRmg_job.vSetRemaining(nAcks);
}

/*----------------------------------------------------------------------------*/
/* decrement+decide acks remaining for this job                               */
/*----------------------------------------------------------------------------*/
bool Job::bAcksRmgDec(i8& nAcksWereRemaining)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bAmDecider = m_oAcksRmg_job.bDecAndDecide(nAcksWereRemaining);
  
  return bAmDecider;
}

/*----------------------------------------------------------------------------*/
/* test if this job has a <byPrompt> tag                                      */
/*----------------------------------------------------------------------------*/
bool Job::bByPromptTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bByPrompt = bHasTag(C_TAG_BYPROMPT);
  
  return bByPrompt;
}

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void Job::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("\n"_Stz);
  vTrace("Job\n"_Stz);
  vTrace("---\n"_Stz);

  vTrace("%p: %s\n"
         
         "m_eJobType=%s m_eJobKind=%s\n"
         "m_eJobList=%s m_eJobPhase=%s m_eJobState=%s\n"
         "bByPrompt=%d bNoWait=%d m_bWasReceived=%d\n"
         "m_iOriginatingId=%lld\n"
         "m_nxReferences=%lld\n"_Stz,
         this,
         aCN[eGetClassNo()],
         
         m_sJobNm.azChar(),
         aJK[m_eJobKind]->azChar(),
         
         aJL[m_eJobList],
         aJP[m_eJobPhase],
         aJS[m_eJobState],
         
         bByPromptTag(),
         bHasTag_noWait(),
         m_bWasReceived,
         
         m_iOriginatingId,
         
         nReferences(),
         C_VARGS_END
         );
  
  
  m_oJobTags.vContentDump();
}

/*----------------------------------------------------------------------------*/
/* test if this job has a /diary ename tag                                    */
/*----------------------------------------------------------------------------*/
bool Job::bDiaryTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bDiary = bHasTag(C_TAG_DIARY);
  
  return bDiary;
}

/*----------------------------------------------------------------------------*/
/* sen . print                                                                */
/*----------------------------------------------------------------------------*/
void Job::vDotPrint(Stz* szText)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  SP_Timer spTimer;
  
  m_oTimer.vUpdate();
  
  vTrace("[%s: job %s %s] "
         "%s"
         "\n"_Stz
         
         ,AM::pGrid()->pThisNode()->szNodeNm()
         ,m_sJobNm.szStz()
         ,spTimer.sz(&m_oTimer)
         
         ,szText
         
         ,C_VARGS_END
         );
}

/*----------------------------------------------------------------------------*/
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Job::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
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
/* test if this job has a /bg tag                                             */
/*----------------------------------------------------------------------------*/
bool Job::bHasTag_bg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bHas = bHasTag(C_TAG_BG);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/* test if this job has a /noWait tag                                         */
/*----------------------------------------------------------------------------*/
bool Job::bHasTag_noWait(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bHas = bHasTag(C_TAG_NOWAIT);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/* test if this job has a /time tag                                           */
/*----------------------------------------------------------------------------*/
bool Job::bHasTag_time(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bHas = bHasTag(C_TAG_TIME);
  
  return bHas;
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
/* get job has error                                                          */
/*----------------------------------------------------------------------------*/
bool Job::bJobHasError(void)
{
  return m_eError_job == EC_NONE ? false : true;
}

/*----------------------------------------------------------------------------*/
/* get the job id                                                             */
/*----------------------------------------------------------------------------*/
i8 Job::iJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return * m_xJobId;
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
eJS Job::eJobState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eJobState;
}

/*----------------------------------------------------------------------------*/
/* set the job state                                                          */
/*----------------------------------------------------------------------------*/
void Job::vJobState(eJS eJobState)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eJobState = eJobState;
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
/* get the logic unit                                                         */
/*----------------------------------------------------------------------------*/
LogicUnit* Job::pJobLogicUnit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // revised job/op model
  return m_pJobLogicUnit;
}

/*----------------------------------------------------------------------------*/
/* new the logic unit for this job                                            */
/*----------------------------------------------------------------------------*/
void Job::vNewJobLogicUnit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ASSERT(! m_pJobLogicUnit);

  m_pJobLogicUnit = new LogicUnit;
  m_pJobLogicUnit->vJob(this);
}

/*----------------------------------------------------------------------------*/
/* null the logic unit for this job                                           */
/*----------------------------------------------------------------------------*/
void Job::vNullJobLogicUnit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pJobLogicUnit = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of node this came from                                            */
/*----------------------------------------------------------------------------*/
Stz* Job::szNodeNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 i_Node = iOriginatorNode();
  
  Stz* szNodeNm = AM::pGrid()->pFindNode(i_Node)->szNodeNm();
  
  return szNodeNm;
}

/*----------------------------------------------------------------------------*/
/* operand queue report                                                       */
/*----------------------------------------------------------------------------*/
void Job::vOperandQueueReport(File* pFile, bool bStdOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
  
  Pipeline* pPipeline = this->pPipeline_job();

  if (! pPipeline)
    return;
  
  if (bStdOut)
    vTrace("\nJob %p: %s\n"_Stz,this,m_sJobNm.szStz());
  pFile->vPrint("\nJob %p: %s\n"_Stz,this,m_sJobNm.szStz());
  
  s48K sOpTags;
  i8 nStages = pPipeline->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    
    Stage* pStage = pPipeline->pGet(i_Stage);
    
    i8 nStreams = pStage->nItems();
    for (i8 i_Stream = 0; i_Stream < nStreams; i_Stream++) {
 
      Stream* pStream= pStage->pGet(i_Stream);
      
      i8 nOps = pStream->nItems();
      for (i8 i_Op = 0; i_Op < nOps; i_Op++) {

        Op* pOp = pStream->pGet(i_Op);
                
        i8 nOpdLeft = pOp->nQueuedOperands();
        
        pOp->pOpSpec()->pArgTags()->vSprint(sOpTags);

        if (bStdOut)
          vTrace("%s %s[%lld] : %s%s ___ %lld\n"_Stz
                 ,pPipeline->szPipelineNm()
                 ,pStage->szStageNm()
                 ,i_Stream
                 
                 ,pOp->szOpNm()
                 ,sOpTags.szStz()
                 ,nOpdLeft
                 );
        
        pFile->vPrint("%s %s[%lld] : %s%s ___ %lld\n"_Stz
          ,pPipeline->szPipelineNm()
          ,pStage->szStageNm()
          ,i_Stream
          
          ,pOp->szOpNm()
          ,sOpTags.szStz()
          ,nOpdLeft
          );
        
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iOriginatorNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 iMaskOffLower48Bits = 0xFFFF000000000000;
  
  i8 iOriginatorNode = iJobId() & iMaskOffLower48Bits;
  iOriginatorNode >>= 48;
  
  return iOriginatorNode;
}

/*----------------------------------------------------------------------------*/
/* set the job id                                                             */
/*----------------------------------------------------------------------------*/
void Job::vParticipantNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_i_Participant = iNode;
}

/*----------------------------------------------------------------------------*/
/* get the participant node                                                   */
/*----------------------------------------------------------------------------*/
i8 Job::iParticipantNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_i_Participant;
}

/*----------------------------------------------------------------------------*/
/* get pipeline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* Job::pPipeline_job(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Pipeline* pPipeline = * m_pJobLogicUnit->m_xpPipeline;
    
  return pPipeline;
}

/*----------------------------------------------------------------------------*/
/* reset job for cascade reuse                                                */
/*----------------------------------------------------------------------------*/
void Job::vResetForCascade(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vDebugPoint();
  
  // -- a -------------------
  // m_sJobNm;
  // m_oTimer.vStart();

  // -- b -------------------
  m_bWasReceived     = false;
  m_i_Participant = C_UNKNOWN;

  // -- c -------------------
  m_eJobKind  = JK_NONE;
  m_eJobList  = JL_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobState = JS_ACTIVE;

  // -- d -------------------
  if (m_pJobLogicUnit) {
    //delete m_pJobLogicUnit;
    //m_pJobLogicUnit = nullptr;
    m_pJobLogicUnit->vGuardedDelete(CN_LOGICUNIT,&m_pJobLogicUnit,
                                    CN_JOB,this,"vResetForCascade"_Stz);
  }
  // m_oJobTags
  // m_xResult ???
  
  // -- e -------------------
  if (m_pSingularOp) {
    delete m_pSingularOp;
    m_pSingularOp  = nullptr;
  }

  // -- f -------------------
  m_iOriginatingId     = C_UNKNOWN;

  // -- g -------------------
  m_oAcksRmg_job.vReset();
  m_eSeed      = SS_ENUMS;
  m_pSeedWord = nullptr;
  m_pTask = nullptr;

  pSession->m_pOutput->vDrainBufferQueue(pSession);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* report this job                                                            */
/*----------------------------------------------------------------------------*/
void Job::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("Job %llx id %s ackRemaining %lld\n"_Stz,
         this,
         iJobId(),
         m_oAcksRmg_job.iPrintOnlyValue());
}

/*----------------------------------------------------------------------------*/
/* see job dump                                                               */
/*----------------------------------------------------------------------------*/
void Job::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  SP_Gid spJobId;
  SP_Timer spTimer;

  SP_Pointer spvPipeline;
  SP_Pointer spvSingularOp;
  SP_Pointer spvOriginatingTask;
  SP_Pointer spvTask;
  SP_Pointer spvJobLogicUnit;

  vTrace("See Job %p::\n"_Stz,this);

  vTrace(
         
         "            sJobId "  "%s"    " \n"  // 0
         "            sJobNm "  "%s"    " \n"  // 1
         "          sJobKind "  "%s"    " \n"  // 2

         "              sCmd "  "%s"    " \n"  // 3
         
         "          i_Origin "  "%lld"  " \n"  // 4
         "         i_Session "  "%lld"  " \n"  // 5
         "            iCmdNo "  "%lld"  " \n"  // 6

         "      bWasReceived "  "%s"    " \n"  // 7
         "     i_Participant "  "%lld"  " \n"  // 8

         "         eJobState "  "%s"    " \n"  // 10
         "         eJobPhase "  "%s"    " \n"  // 11
         "          eJobList "  "%s"    " \n"  // 12
         
         "            oTimer "  "%s"    " \n"  // 13
         "          oAcksRmg "  "%lld"  " \n"  // 14
         "            eError "  "%s"    " \n"  // 15

         "       pSingularOp "  "%s"    " \n"  // 17

         "    iOriginatingId "  "%lld"  " \n"  // 19
 
         "             eSeed "  "%s"    " \n"  // 20
         "             pTask "  "%s"    " \n"  // 21
        
         "     pJobLogicUnit "  "%s"    " \n"  // 22

         ""_Stz
         
         ,spJobId.sz(* m_xJobId)     // 0
         ,m_sJobNm.szStz()           // 1
         ,aJK[m_eJobKind]            // 2
         ,m_sCmd.szStz()             // 3
         ,m_i_Origin                 // 4
         ,m_i_Session                // 5
         ,m_iCmdNo                   // 6
         ,m_bWasReceived ? "true" : "false" // 7
         ,m_i_Participant            // 8
         ,aJS[m_eJobState]           // 10
         ,aJP[m_eJobPhase]           // 11
         ,aJL[m_eJobList]            // 12
         ,spTimer.sz(& m_oTimer)     // 13
         ,m_oAcksRmg_job.iPrintOnlyValue() // 14
         ,aEC[m_eError_job]          // 15
         ,spvSingularOp.sz(m_pSingularOp)  // 17
         ,m_iOriginatingId           // 19
         ,aSS[m_eSeed]               // 20
         ,spvTask.sz(m_pTask)        // 21
         ,spvJobLogicUnit.sz(m_pJobLogicUnit) // 22
         
         );
  
  m_oJobTags.vContentDump();

  vTrace(";;\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get the seed to be spawned                                                 */
/*----------------------------------------------------------------------------*/
eSS Job::eSeed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eSeed;
}

/*----------------------------------------------------------------------------*/
/* get the seed to be spawned                                                 */
/*----------------------------------------------------------------------------*/
Stz* Job::szSeed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Stz* szSeed = AM::SEED::szDecodeSeed(m_eSeed,&m_oJobTags);
    
  return szSeed;
}

/*----------------------------------------------------------------------------*/
/* set the seed to be spawned                                                 */
/*----------------------------------------------------------------------------*/
void Job::vSeed(eSS eSeed)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eSeed = eSeed;
}

/*----------------------------------------------------------------------------*/
/* get seed word                                                              */
/*----------------------------------------------------------------------------*/
Word* Job::pSeedWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_pSeedWord;
}

/*----------------------------------------------------------------------------*/
/* set seed word                                                              */
/*----------------------------------------------------------------------------*/
void Job::vSeedWord(Word* pSeedWord)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pSeedWord = pSeedWord;
}

/*----------------------------------------------------------------------------*/
/* get pSession                                                               */
/*----------------------------------------------------------------------------*/
Session* Job::pSession(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Session* pSession = AM::pSessionGet(m_i_Origin,m_i_Session);

  return pSession;
}

/*----------------------------------------------------------------------------*/
/* set job to execute cascade command                                         */
/*----------------------------------------------------------------------------*/
void Job::vSetFromCascade(Stz* szCascade)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_oJobTags.vReplace(C_TAG_CMD,szCascade);

  vSeed(SS_SEED_REQ);

  //m_oJobTags.vDumpSet(true);
}

/*----------------------------------------------------------------------------*/
/* find and set seed word for this job                                         */
/*----------------------------------------------------------------------------*/
void Job::vSetSeedWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set the job trace                                                          */
/*----------------------------------------------------------------------------*/
void Job::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vTrace("%s %03lld seed %s "
         "i_Session %03lld WasReceived %lld\n"_Stz,
         aCN[eGetClassNo()],
         aSS[m_eSeed],
         
         m_i_Session,
         (i8)m_bWasReceived
         );
}

/*----------------------------------------------------------------------------*/
/* test if this job has a /summary ename tag                                  */
/*----------------------------------------------------------------------------*/
bool Job::bSummaryTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bSummary = bHasTag(C_TAG_SUMMARY);
  
  return bSummary;
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
Task* Job::pJobTask(void)
{
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Job::vJobTask(Task* pTask)
{
  ASSERT(! pTask || ! m_pTask);
  
  if (! pTask)
    vDebugPoint();
  
  m_pTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* get elapsed time in milliseconds                                           */
/*----------------------------------------------------------------------------*/
i8 Job::iTimerMsec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_oTimer.iMilliseconds();
}

/*----------------------------------------------------------------------------*/
/* start/reset timer                                                          */
/*----------------------------------------------------------------------------*/
void Job::vTimerStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_oTimer.vStart();
}

/*----------------------------------------------------------------------------*/
/* update timer                                                               */
/*----------------------------------------------------------------------------*/
void Job::vTimerUpdate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_oTimer.vUpdate();
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
  
  RASSERT(m_pJobLogicUnit);
}

/*----------------------------------------------------------------------------*/
/* walk                                                                       */
/*----------------------------------------------------------------------------*/
void Job::vWalk(void* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  eCN eClassNo = eGetClassNo();
  
  vTrace("Walk %8s:%ll03d %p\n"_Stz
         
         ,aCN[eClassNo]
         ,(i8)eClassNo
         ,this
         );

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  Pipeline* pPipeline = this->pPipeline_job();
  pPipeline->vWalk(aArg);
}

/*----------------------------------------------------------------------------*/
/* walk                                                                       */
/*----------------------------------------------------------------------------*/
void Job::vWalkLambda(i8 iDepth,
                      std::function<void(Object* pObject, i8 iDepth)> vfClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
    
  vfClosure(this,iDepth);

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  Pipeline* pPipeline = this->pPipeline_job();
  pPipeline->vWalkLambda(iDepth + 1,vfClosure);
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


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check if the cluster has started                                           */
/*----------------------------------------------------------------------------*/
eEC STORE::eJob_checkCluster(void)
{
  if (AM::SYSTEM::bStarted())
    return EC_NONE;
  else
    return EC_CLUSTER_NOT_CONNECTED;
}

/*----------------------------------------------------------------------------*/
/* compare two jobs by their jobId                                            */
/*----------------------------------------------------------------------------*/
bool STORE::bJobCompare(void* aJobInsert, void* aJobScan, bool& bMatch)
{
  Job* pJobInsert = _pJob(aJobInsert);
  Job* pJobScan   = _pJob(aJobScan);
  
  eCN eClassJobScan = pJobScan->eGetClassNo();
  RASSERT(_bIsValidJob(eClassJobScan));
  
  if (! pJobScan)
    return false;
  
  // do the job id's match?
  if (pJobInsert->iJobId() == pJobScan->iJobId())
    bMatch = true;
  else
    bMatch = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is this job to be inserted lexicography after the scan?
  
  if (pJobInsert->iJobId() > pJobScan->iJobId())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
  
  return &m_xFooter;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
