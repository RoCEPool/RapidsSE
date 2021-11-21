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
#include "Session.hpp"

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
Job::Job(Stz* szJobNm,
         i8 i_Origin,     // originated on which node
         i8 i_Session,    // which session in session array
         i8 iCmdNo        // the sequence no of the request from that session)
        )
: Operand(CN_JOB)
, CRM<Job>(this)
, LifeGuard_t<Job>(this,CN_JOB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
 
  m_tJobId.vNextJobId();
  m_sJobNm   = szJobNm;
  m_eJobKind = JK_NONE;

  
  m_bReapable = false;
  
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

  // -- g -------------------
  m_oAcksRmg_job.vCheckInit();
  m_pTask     = nullptr;

  // -- -- -- -- -- --
      
  
  _checkInitialised_(CN_JOB);

  vNewJobLogicUnit();
  
  m_oJobTags.vInsert(C_TAG_DISTID,m_tJobId.iAsI8());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB);
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
  if (JK_GENERIC != m_eJobKind && JK_NONE != m_eJobKind) {
    while (nReferences() || TS_RETIRED != m_pTask->eState() ) {
      vTrace("Napping\n"_Stz);
      AM::vNapMsec(10, eGetClassNo());
    }
  }

  RASSERT(JK_GENERIC == m_eJobKind ||
          JK_NONE    == m_eJobKind ||
          m_bReapable);
  
  if (m_pJobLogicUnit) {
    //AM::LOGICUNIT::vDelete(&m_pJobLogicUnit);
    m_pJobLogicUnit->vGuardedDelete(CN_LOGICUNIT,&m_pJobLogicUnit,
                                    CN_JOB,this,(Stz*)__func__);
  }
  else if (C_UNKNOWN != m_i_Origin) {
    vDebugPoint();
  }
  
  if (m_pTask) {
    m_pTask->vGrabTaskLock((Stz*)__func__);
    m_pTask->vState(TS_DELETING);
    m_pTask->vDropTaskLock((Stz*)__func__);
    m_pTask->vGuardedDelete(CN_TASK,&m_pTask,
                            CN_JOB,this,(Stz*)__func__);

  }
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

  pReceivedJob->vInductIntoNode(pReceivedJob);
  
  pReceivedJob->vWasReceived(true);
  
  pReceivedJob->vJobList(JL_NONE);
    
  pReceivedJob->vJobTask(nullptr);

  // revised job/op model
  pReceivedJob->vNullJobLogicUnit();

  pReceivedJob->m_pSingularOp = nullptr;


  
  if (pReceivedJob->bHasTag(C_SEE_JOB))
    pReceivedJob->vPrintJobContents();
  
  vDebugPoint();
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
         ,(i8)tJobId.uNodeNo()
         ,(i8)tJobId.uJobNo()
         ,pNode->m_pRight[i_Level]
         );
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
/* get the number of active ops                                               */
/*----------------------------------------------------------------------------*/
i8 Job::nActiveOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  i8 nActive = * m_noActiveOps;
  
  return nActive;
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
  
  if (AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("Job: Setting acks remaining %lld\n"_Stz,nAcks);

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
/* get the sen entry code                                                     */
/*----------------------------------------------------------------------------*/
Stz* Job::szEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sEntry;
}

/*----------------------------------------------------------------------------*/
/* set the sen entry code                                                     */
/*----------------------------------------------------------------------------*/
void Job::vEntry(Stz* szEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sEntry = szEntry;
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
/* decrement number of active op's                                            */
/*----------------------------------------------------------------------------*/
void Job::vDecActiveOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  -- m_noActiveOps;
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
/* icrement number of active op's                                             */
/*----------------------------------------------------------------------------*/
void Job::vIncActiveOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  ++ m_noActiveOps;
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
JobId Job::tJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_tJobId;
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
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iOriginatorNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  i8 i_OriginatorNode = m_tJobId.uNodeNo();
  
  return i_OriginatorNode;
}

/*----------------------------------------------------------------------------*/
/* set the job id                                                             */
/*----------------------------------------------------------------------------*/
void Job::vParticipantNode(i8 i_Node)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_i_Participant = i_Node;
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

 
         "             pTask "  "%s"    " \n"  // 21
        
         "     pJobLogicUnit "  "%s"    " \n"  // 22

         ""_Stz
         
         ,spJobId.sz(* m_tJobId)     // 0
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
         ,spvTask.sz(m_pTask)        // 21
         ,spvJobLogicUnit.sz(m_pJobLogicUnit) // 22
         
         );
  
  m_oJobTags.vContentDump();

  vTrace(";;\n\n"_Stz);
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

  vTrace("origin %lld session %lld command %lld "_Stz
         ,this->m_i_Origin
         ,this->m_i_Session
         ,this->m_iCmdNo
         );

  if (C_UNKNOWN != this->iParticipantNode())
    vTrace("partNode %lld "_Stz
           ,this->iParticipantNode()
           );

  LogicUnit* pLogicUnit = this->pJobLogicUnit();
  
  if (pLogicUnit) {
    if (pLogicUnit->m_xpPipelineSpec.bIsNotNull())
      vTrace("%s "_Stz
             ,pLogicUnit->m_xpPipelineSpec->szPipelineNm()
             );
    vTrace("%s "_Stz
           ,aLH[pLogicUnit->m_eDdlLocked]
           );
  }
  
  vTrace("AcksRmg %lld (out of %lld)"_Stz
         ,this->iAcksRmg_printOnlyValue()
         ,this->iAcksOriginal()
         );

  vTrace("logicUnit %p task %p"_Stz
         ,this->m_pJobLogicUnit
         ,this->pJobTask()
         );

  vTrace("\n"_Stz);
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

  // -- g -------------------
  m_oAcksRmg_job.vReset();

  m_pTask = nullptr;

  pSession->m_pOutput->vDrainBufferQueue(pSession);

  vDebugPoint();
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

  //m_oJobTags.vDumpSet(true);
}




/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



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
                      std::function<void(Object* pObject, i8 iDepth)> f_vClosure)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB,_FOOTER_);
    
  f_vClosure(this,iDepth);

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  Pipeline* pPipeline = this->pPipeline_job();
  pPipeline->vWalkLambda(iDepth + 1,f_vClosure);
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
