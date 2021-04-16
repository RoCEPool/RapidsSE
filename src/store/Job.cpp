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
#include "JobLog.hpp"
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
  
  if (pJob && ! _bIsValidJob(pJob->eGetClassNo()) )
      vTrace("_pJob got class %s\n"_Stz,aCN[pJob->eGetClassNo()]);
      
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
Job::Job(ReqTuple* aReqTuple)
: Object(CN_JOB)
, CRM<Job>(this)
, m_xReqTuple(aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
    
  // -- a -------------------
  // m_xReqTuple
  // m_sJobNm;
  m_iParentId = C_UNKNOWN;
  m_oTimer.vStart();

  // -- b -------------------
  m_bWasReceived     = false;
  m_iParticipantNode = C_UNKNOWN;
  m_bNormalSides     = true;
  
  // -- c -------------------
  m_eJobKind  = JK_NONE;
  m_eJobList  = JL_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobState = JS_RUNNING;
  m_eError    = EC_NONE;
  
  // -- d -------------------
  m_pLogicUnit_job = nullptr;
  // m_oJobTags
  // m_xResult

  // -- e -------------------
  m_pPipeline = nullptr;
  m_pOp       = nullptr;
  m_pOuterOp  = nullptr;
  m_pInputOp  = nullptr;

  // -- f -------------------
  // m_ixErrors;
  m_pOriginatingTask = nullptr;
  m_iOriginatingId   = C_UNKNOWN;

  // -- g -------------------
  m_oAcksRmg_2pc.vCheckInit();
  m_eSeed      = SS_ENUMS;
  m_i_TablePtn = C_UNKNOWN;
  m_pTaskJ     = nullptr;

  // -- -- -- -- -- --
      
  ++ gixJob;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB);
  
  AM::LOG::vEvent(EV_CONSTRUCT,"Job constructor"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor (for job__find)                                  TO BE REMOVED */
/*----------------------------------------------------------------------------*/
Job::Job(eCN eClassNo)
: Object(eClassNo)
, CRM<Job>(this)
, m_xReqTuple(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  //vTrace("%s: CREATING JOB %p %s\n"_Stz,
  //       AM::pGrid()->pThisNode()->aNodeName(), this, aCN[eGetClassNo()]);
  
  // m_xReqTuple
  
  m_iParentId = C_UNKNOWN;
  m_eSeed     = SS_ENUMS;
  m_i_TablePtn    = C_UNKNOWN;
  
  m_eJobKind  = JK_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobList  = JL_NONE;
  m_eJobState = JS_RUNNING;
  
  m_bNormalSides = true; //
  m_bWasReceived = false;
  
  m_oTimer.vStart();
  
  //m_oAcksRmg
  
  m_pTaskJ           = nullptr;
  m_pOriginatingTask = nullptr;
  m_iOriginatingId   = C_UNKNOWN;

  m_iParticipantNode = C_UNKNOWN;
  m_oAcksRmg_2pc.vCheckInit();
  
  m_pPipeline = nullptr;
  m_pOp       = nullptr;
  m_pOuterOp  = nullptr;
  m_pInputOp  = nullptr;
  
  
  m_pLogicUnit_job = nullptr;

  //m_xResult
  // -- -- -- -- -- --
  
  SP_Gid spId;
    
  ++ gixJob;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  
  AM::LOG::vEvent(EV_CONSTRUCT,"Job constructor"_Stz);
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
  
  if (m_pPipeline)
    delete m_pPipeline;
  
  delete m_pLogicUnit_job;
  
  //if (m_pTaskJ)
  //  vDebugPoint();
  
  //AM::vTaskDelete(m_pTaskJ);
  
  AM::LOG::vEvent(EV_DESTRUCT,"Job destructor"_Stz);

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
void Job::vPostRecv(void* aJob)
{
  _static_
  
  Job* pJob = _pJob(aJob);
  
  pJob->vWasReceived(true);
  
  pJob->vJobList(JL_NONE);
  
  pJob->vTask(nullptr);
    
  LogicUnit* pLogicUnit = nullptr;
  pJob->vLogicUnit_job( & pLogicUnit);
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
  
  //RASSERT(! AM::iTaskAcksRmg_assert_only_(m_pTaskJ));
  
  i8 iAcksOriginal = m_oAcksRmg_2pc.nOriginalAcks();
  
  return iAcksOriginal;
}

/*----------------------------------------------------------------------------*/
/* get the number of acks required by the job                                 */
/*----------------------------------------------------------------------------*/
i8 Job::iAcksRmg_printOnlyValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //RASSERT(! AM::iTaskAcksRmg_assert_only_(m_pTaskJ));
  
  i8 iAcksRmg = m_oAcksRmg_2pc.iPrintOnlyValue();
  
  return iAcksRmg;
}

/*----------------------------------------------------------------------------*/
/* set the number of acks required by the job                                 */
/*----------------------------------------------------------------------------*/
void Job::vAcksRmg(i8 nAcks)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //RASSERT(! AM::iTaskAcksRmg_assert_only_(m_pTaskJ));
  
  m_oAcksRmg_2pc.vSetRemaining(nAcks);
}

/*----------------------------------------------------------------------------*/
/* decrement+decide acks remaining for this job                               */
/*----------------------------------------------------------------------------*/
bool Job::bAcksRmgDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bAmDecider = m_oAcksRmg_2pc.bDecAndDecide();
  
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
         "m_iParentId=%lld m_iOriginatingId=%lld\n"
         "m_nxReferences=%lld\n"
         "m_pPipeline=%p\n"_Stz,
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
         
         m_iParentId,
         m_iOriginatingId,
         
         nReferences(),
         m_pPipeline,
         C_VARGS_END
         );
  
  if ( ! m_sJobNm.bSame("pipeline"_Stz) && m_pPipeline) {
    
    vTrace("\n"_Stz);
    vTrace("Pipeline Job\n"_Stz);
    vTrace("---\n"_Stz);
    
    if (m_pPipeline->pPipelineJob() != this)
      m_pPipeline->pPipelineJob()->vContentDump();
  }
  
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
bool Job::bHasTag(Stz* szEnameTag)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (m_oJobTags.bHasName(szEnameTag))
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
/* get the input operator                                                     */
/*----------------------------------------------------------------------------*/
Op* Job::pInputOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pInputOp;
}

/*----------------------------------------------------------------------------*/
/* set the input operator                                                     */
/*----------------------------------------------------------------------------*/
void Job::vInputOp(Op* pInputOp)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pInputOp = pInputOp;
}

/*----------------------------------------------------------------------------*/
/* is this a pipeline job?                                                    */
/*----------------------------------------------------------------------------*/
bool Job::bIsPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (AM::szJobName(this)->bSame("pipeline"_Stz) ||
      AM::szJobName(this)->bSame("pipelineLocal"_Stz))
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

  m_eError = eErrorCode;

//  if (m_xResult.m_xJobResponse.m_bJobHasError) {
//    //vTrace("Job got another error: '%s' (already had error: '%s') \n"_Stz,
//    //  aEC[eErrorCode], aEC[m_xResult.m_xJobResponse.m_eErrorCode]);
//  }
//  else {
//    m_xResult.m_xJobResponse.m_eErrorCode = eErrorCode;
//    m_xResult.m_xJobResponse.m_bJobHasError = true;
//  }
}

/*----------------------------------------------------------------------------*/
/* get job error                                                              */
/*----------------------------------------------------------------------------*/
eEC Job::eJobError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eError;
}

/*----------------------------------------------------------------------------*/
/* get job has error                                                          */
/*----------------------------------------------------------------------------*/
bool Job::bJobHasError(void)
{
  return m_eError == EC_NONE ? false : true;
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
LogicUnit* Job::pLogicUnit_job(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pLogicUnit_job;
}

/*----------------------------------------------------------------------------*/
/* set the logic unit                                                         */
/*----------------------------------------------------------------------------*/
void Job::vLogicUnit_job(LogicUnit** ppLogicUnit)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ASSERT(! * ppLogicUnit || ! m_pLogicUnit_job);
  
  m_pLogicUnit_job = *ppLogicUnit;
  
  *ppLogicUnit = nullptr;
}

/*----------------------------------------------------------------------------*/
/* write an activity map string about this job                                */
/*----------------------------------------------------------------------------*/
void Job::vMarkJob(void)
{
  s48K sJobMark;
  
  sJobMark.vSprintCat("Job %03lld %s session %lld tablePtn %lld\n"_Stz,
                      iJobId(),
                      aCN[eGetClassNo()],
                      m_xReqTuple.m_i_Session,
                      m_i_TablePtn);
  sJobMark.vSprintCat("  phase %s state %s list %s"_Stz,
                      aJP[m_eJobPhase],
                      aJS[m_eJobState],
                      aJL[m_eJobList]);
  if (m_bWasReceived)
    sJobMark.vSprintCat(" (recv from %s)"_Stz,
                        szNodeNm());

  sJobMark.vConcat("\n"_Stz);

  AM::vLogText(iJobId(),sJobMark);
}

/*----------------------------------------------------------------------------*/
/* write a mark about this job                                                */
/*----------------------------------------------------------------------------*/
void Job::vMarkJob(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  UNUSED(szFormat);

#ifdef WITH_SHARED_LOG
  if (! AM::bLogging())
    return;
  
  va_list xArgs;
  
  va_start(xArgs,szFormat);
  
  JobLog::vNote(this,szFormat,xArgs);
  
  va_end(xArgs);
#endif
  
}

/*----------------------------------------------------------------------------*/
/* get name of node this came from                                            */
/*----------------------------------------------------------------------------*/
Stz* Job::szNodeNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 i_Node = iOriginatorNode();
  
  Stz* szNodeNm = AM::pGrid()->pGetNode(i_Node)->szNodeNm();
  
  return szNodeNm;
}

/*----------------------------------------------------------------------------*/
/* return side semantics                                                      */
/*----------------------------------------------------------------------------*/
bool Job::bNormalSides(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_bNormalSides;
}

/*----------------------------------------------------------------------------*/
/* wait for job mutex to be signalled                                         */
/*----------------------------------------------------------------------------*/
void Job::vNormalSides(bool bNormalSides)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bNormalSides = bNormalSides;
}

/*----------------------------------------------------------------------------*/
/* operand queue report                                                       */
/*----------------------------------------------------------------------------*/
void Job::vOperandQueueReport(File* pFile, bool bStdOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
  
  if (! m_pPipeline)
    return;
  
  if (bStdOut)
    vTrace("\nJob %p: %s\n"_Stz,this,m_sJobNm.szStz());
  pFile->vPrint("\nJob %p: %s\n"_Stz,this,m_sJobNm.szStz());
  
  s48K sOpTags;
  i8 nStages = m_pPipeline->nItems();
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    
    Stage* pStage = m_pPipeline->pGet(i_Stage);
    
    i8 nStreams = pStage->nItems();
    for (i8 i_Stream = 0; i_Stream < nStreams; i_Stream++) {
 
      Stream* pStream= pStage->pGet(i_Stream);
      
      i8 nOps = pStream->nItems();
      for (i8 i_Op = 0; i_Op < nOps; i_Op++) {

        Op* pOp = pStream->pGet(i_Op);
        
        OperandQueue* pQueueLeft = pOp->pQueueLeft();
        
        i8 nOpdLeft = pQueueLeft->nQueuedOperands();
        
        pOp->pOpTagSet()->vSprint(sOpTags);

        if (bStdOut)
          vTrace("%s %s[%lld] : %s%s ___ %lld\n"_Stz
                 ,m_pPipeline->szPipelineNm()
                 ,pStage->szStageNm()
                 ,i_Stream
                 
                 ,pOp->szOpNm()
                 ,sOpTags.szStz()
                 ,nOpdLeft
                 );
        
        pFile->vPrint("%s %s[%lld] : %s%s ___ %lld\n"_Stz
          ,m_pPipeline->szPipelineNm()
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
/* get the parent job id                                                      */
/*----------------------------------------------------------------------------*/
i8 Job::iParentId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iParentId;
}

/*----------------------------------------------------------------------------*/
/* set the parent job id                                                      */
/*----------------------------------------------------------------------------*/
void Job::vParentId(i8 iParentId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iParentId = iParentId;
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
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Job::vPrint(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  sSTMT sFormatted;
  
  SP_Timer spTimer;
  
  va_list vaArgs;
  
  va_start(vaArgs,szFormat);
  
  m_oTimer.vUpdate();
  
  sFormatted.vSprintVA(szFormat,vaArgs);
  
  vTrace("[%s %s %s %s] %s\n"_Stz,
         AM::pGrid()->pThisNode()->szNodeNm(),
         szSide(),
         m_sJobNm.szStz(),
         spTimer.sz(&m_oTimer),
         
         sFormatted.szStz(),
         
         C_VARGS_END);

  va_end(vaArgs);
}

/*----------------------------------------------------------------------------*/
/* populate node                                                              */
/*----------------------------------------------------------------------------*/
eEC Job::ePopulateJob(TagSet* pTagSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
  
  //  bool b = CmdHelper::bParseCmd(pJobTags(),pTknVec);
  //  if (! b) {
  //    //vTrace("Unable to match job syntax\n"_Stz);
  //    vParticipantHasError(true);
  //    return EC_INVALID_COMMAND;
  //  }
  //pTagSet->vDumpSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the distid
  pJobTags()->vInsert(C_TAG_DISTID,iJobId());
  
  // copy the tagset into the job tag set
  pJobTags()->vCopy(pTagSet);
  
  eJK eJobKind = (eJK) pJobTags()->iValue(C_TAG_JOBKIND);
  vJobKind(eJobKind);
  
  Stz* szJobNm = pJobTags()->szValue(C_TAG_JOBNM);
  vJobNm(szJobNm);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* reset job for cascade reuse                                                */
/*----------------------------------------------------------------------------*/
void Job::vResetForReuse(Session* pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vDebugPoint();
  
  // -- a -------------------
  // m_xReqTuple
  // m_sJobNm;
  // m_iParentId = C_UNKNOWN;
  // m_oTimer.vStart();

  // -- b -------------------
  m_bWasReceived     = false;
  m_iParticipantNode = C_UNKNOWN;
  m_bNormalSides     = true;

  // -- c -------------------
  m_eJobKind  = JK_NONE;
  m_eJobList  = JL_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobState = JS_RUNNING;

  // -- d -------------------
  delete m_pLogicUnit_job; m_pLogicUnit_job = nullptr;
  // m_oJobTags
  // m_xResult ???
  
  // -- e -------------------
  delete m_pPipeline; m_pPipeline = nullptr;
  delete m_pOp;       m_pOp       = nullptr;
  delete m_pOuterOp;  m_pOuterOp  = nullptr;
  delete m_pInputOp;  m_pInputOp  = nullptr;

  // -- f -------------------
  m_ixErrors = 0;
  m_pOriginatingTask = nullptr;
  m_iOriginatingId   = C_UNKNOWN;

  // -- g -------------------
  m_oAcksRmg_2pc.vReset();
  m_eSeed      = SS_ENUMS;
  m_i_TablePtn = C_UNKNOWN;
  m_pTaskJ = nullptr;

  pSession->m_pOutput->vDrainBufferQueue(pSession);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* report this job                                                            */
/*----------------------------------------------------------------------------*/
void Job::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("Job %llx id %lls ackRemaining %lld\n"_Stz,
         this,
         iJobId(),
         m_oAcksRmg_2pc.iPrintOnlyValue());
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
  
  Stz* szSeed;
  
  switch (m_eSeed) {
    case SS_SEED_RUN: szSeed = m_oJobTags.szValue("<seedRun>"_Stz); break;
    case SS_SEED_REQ: szSeed = m_oJobTags.szValue("<seedReq>"_Stz); break;
    case SS_SEED_RCV: szSeed = m_oJobTags.szValue("<seedRcv>"_Stz); break;
    case SS_SEED_EXE: szSeed = m_oJobTags.szValue("<seedExe>"_Stz); break;
    case SS_SEED_END: szSeed = m_oJobTags.szValue("<seedEnd>"_Stz); break;
    case SS_SEED_CAN: szSeed = m_oJobTags.szValue("<seedCan>"_Stz); break;
    default: NEVER_REACH;
  }

  ASSERT(szSeed)
  
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
/* get pSession                                                               */
/*----------------------------------------------------------------------------*/
Session* Job::pSession(void)
{
  Session* pSession = AM::pSessionGet(&m_xReqTuple);

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
/* get which side the job is on (originator or participant)                   */
/*----------------------------------------------------------------------------*/
Stz* Job::szSide(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (m_bWasReceived) {
    if (m_bNormalSides)
      return "partpt"_Stz;
    else
      return "origin"_Stz;
  }
  else {
    if (m_bNormalSides)
      return "origin"_Stz;
    else
      return "partpt"_Stz;
  }
}

/*----------------------------------------------------------------------------*/
/* get table partition on this node this sub job processes                    */
/*----------------------------------------------------------------------------*/
i8 Job::i_TablePtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_i_TablePtn;
}

/*----------------------------------------------------------------------------*/
/* set table partition on this node this sub job processes                    */
/*----------------------------------------------------------------------------*/
void Job::vTablePtn(i8 i_TablePtn)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_i_TablePtn = i_TablePtn;
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

  vTrace("%s parentId %03lld seed %s "
         "i_Session %03lld i_TablePtn %lld WasReceived %lld\n"_Stz,
         aCN[eGetClassNo()],
         m_iParentId,
         aSS[m_eSeed],
         
         m_xReqTuple.m_i_Session,
         m_i_TablePtn,
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
Task* Job::pTask(void)
{
  return m_pTaskJ;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Job::vTask(Task* pTask)
{
  ASSERT(! pTask || ! m_pTaskJ);
  m_pTaskJ = pTask;
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
  if (AM::pGrid()->bHasStarted())
    return EC_NONE;
  else
    return EC_CLUSTER_NOT_CONNECTED;
}

/*----------------------------------------------------------------------------*/
/* pre-send job function                                                      */
/*----------------------------------------------------------------------------*/
void STORE::vJob_preSend(Job* pJob)
{
  UNUSED(pJob);

}

/*----------------------------------------------------------------------------*/
/* post-recv job function  (when is this run ????)                            */
/*----------------------------------------------------------------------------*/
void STORE::vJob_postRecv(Job* pJob)
{
  pJob->vWasReceived(true);
  
  pJob->vJobList(JL_NONE);
  
  pJob->vTask(nullptr);
    
  LogicUnit* pLogicUnit = nullptr;
  pJob->vLogicUnit_job( & pLogicUnit);
}

/*----------------------------------------------------------------------------*/
/* compare two jobs by their jobId                                            */
/*----------------------------------------------------------------------------*/
bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch)
{
  Job* pInsertEntry = _pJob(pObjectInsert);
  Job* pScanEntry   = _pJob(pObjectScan);
  
  eCN eClassScanEntry = pScanEntry->eGetClassNo();
  
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
