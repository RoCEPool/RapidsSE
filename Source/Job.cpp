/*============================================================================*/
/* Job.cpp -- a job in the system                                             */
/*============================================================================*/

#include "Jobs.h"
#include "AM.hpp"

Atomic gixJob;

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
bool Jobs::_bIsValidJob(eCN eClass)
{
  if (eClass >= CN_JOB__BEGUN && eClass <= CN_JOB_WRITEROW)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* constructor (for job__find)                                                */
/*----------------------------------------------------------------------------*/
Job::Job(eCN eClassNo)
: Focal(eClassNo,C_UNKNOWN,C_UNKNOWN)
, m_xReqTuple(nullptr)
, CRM<Job>(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  //vTrace("%s: CREATING JOB %p %s\n"_Stz,
  //       AM::pGrid()->pThisNode()->aNodeName(), this, aCN[eClass()]);
  
  // m_xReqTuple
  
  m_aJobClass = nullptr;
  
  m_iParentId = C_UNKNOWN;
  m_eSeed     = SS_ENUMS;
  m_i_Slot    = C_UNKNOWN;
  
  m_eJobType  = JT_NONE;
  m_eJobKind  = JK_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobList  = JL_NONE;
  m_eJobState = JS_RUNNING;
  
  m_bNormalSides = true; //
  m_bWasReceived = false;
  
  //m_oAcksRmg
  
  m_aJobClass        = nullptr;
  m_pTaskJ = nullptr;
  m_pOriginatingTask = nullptr;
  m_iOriginatingId = C_UNKNOWN;

  m_iParticipantNode  = C_UNKNOWN;
  m_oAcksRmg_2pc.vCheckInit();
  
  m_pPipeline = nullptr;
  m_pOp = nullptr;
  m_pOperand   = nullptr;
  

  
  // -- -- -- -- -- --
  
  SP_Id spId;
  
  vIncStepNo(); ++ gixJob;
  AM::vLogJob(SL_INF,this,JA_JOB_CONSTRUCT,
              "|gixJob=%lld|"_Stz, //!format-LogJob
              * gixJob);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(JobClass* aJobClass, ReqTuple* aReqTuple)
: Focal(aJobClass->m_eClass,AM::iNextJobId(),aReqTuple->m_iForId)
, m_xReqTuple(aReqTuple)
, CRM<Job>(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,aJobClass->m_eClass);
    
  // m_xReqTuple
  
  m_aJobClass = aJobClass;

  m_iParentId = C_UNKNOWN;
  m_eSeed     = SS_ENUMS;
  m_i_Slot    = C_UNKNOWN;
  
  m_eJobType  = JT_NONE;
  m_eJobKind  = JK_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobList  = JL_NONE;
  m_eJobState = JS_RUNNING;
  
  m_bNormalSides = true; //
  m_bWasReceived = false;
  
  //m_oAcksRmg
  
  m_pTaskJ = nullptr;
  m_pOriginatingTask = nullptr;
  m_iOriginatingId = C_UNKNOWN;

  m_iParticipantNode  = C_UNKNOWN;
  m_oAcksRmg_2pc.vCheckInit();
  
  m_pPipeline = nullptr;
  m_pOp       = nullptr;
  m_pOperand  = nullptr;
  
  
  // -- -- -- -- -- --
  
  SP_Id spId;
  
  vIncStepNo(); ++ gixJob;
  AM::vLogJob(SL_INF,this,JA_JOB_CONSTRUCT,
              "|gixJob=%lld|"_Stz, //!format-LogJob
              * gixJob);
  
  _postcondition_(_ADDRESS_,_MAGIC_,aJobClass->m_eClass);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(Job* pParentJob, JobClass* aJobClass, ReqTuple* aReqTuple)
: Focal(aJobClass->m_eClass,AM::iNextJobId(),aReqTuple->m_iForId)
, m_xReqTuple(aReqTuple)
, CRM<Job>(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,aJobClass->m_eClass);
  
  // m_xReqTuple
  
  m_aJobClass = aJobClass;

  m_iParentId = C_UNKNOWN;
  m_eSeed     = SS_ENUMS;
  m_i_Slot    = C_UNKNOWN;
  
  m_eJobType  = JT_NONE;
  m_eJobKind  = JK_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobList  = JL_NONE;
  m_eJobState = JS_RUNNING;
  
  m_bNormalSides = true; //
  m_bWasReceived = false;
  
  //m_oAcksRmg
  
  m_pTaskJ = nullptr;
  m_pOriginatingTask = nullptr;
  m_iOriginatingId = C_UNKNOWN;
  
  m_iParticipantNode  = C_UNKNOWN;
  m_oAcksRmg_2pc.vCheckInit();
  
  m_pPipeline = nullptr;
  m_pOp       = nullptr;
  m_pOperand  = nullptr;
  
  // -- -- -- -- -- --
  
  SP_Id spId;
  
  vIncStepNo(); ++ gixJob;
  AM::vLogJob(SL_INF,this,JA_JOB_CONSTRUCT,
              "|gixJob=%lld|"_Stz, //!format-LogJob
              * gixJob);
  
  _postcondition_(_ADDRESS_,_MAGIC_,aJobClass->m_eClass);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //vTrace("Deleting job: %s\n"_Stz,aCN[eClass()]);
  
  if (bHasEnameTag(C_TAG_LIFE)) {
    
    SP_EnmSet spEnameSet;
    vPrint("Job Delete: %s\n"_Stz,spEnameSet.sz(&m_oEnameSet));
    //m_oEnameSet.vPrint();
  }

  vIncStepNo();
  if (m_bWasReceived) {
    // do not decrement gixJob
    AM::vLogJob(SL_INF,this,JA_JOB_DESTRUCT,
                "|gixJob=%lld|"_Stz, //!format-LogJob
                * gixJob);
  }
  else {
    -- gixJob;
    AM::vLogJob(SL_INF,this,JA_JOB_DESTRUCT,
                "|gixJob=%lld|"_Stz, //!format-LogJob
                * gixJob);
  }
  
  // -- -- -- -- -- --
  
  //! this assert may fail if a job completes before its references
  //! are completely returned
  while (nReferences()) {
    vTrace("Napping\n"_Stz);
    AM::vNap(10, eClass());
  }
  
  if (m_pPipeline)
    delete m_pPipeline;
  
  //if (m_pTaskJ)
  //  vDebugPoint();
  
  //AM::vTaskDelete(m_pTaskJ);

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
/* delete given job using job class info                                      */
/*----------------------------------------------------------------------------*/
void Job::vDeleteJobViaClass(Job* pJob)
{
  RASSERT(pJob && pJob->m_aJobClass);
  
  pJob->m_aJobClass->vDeleteJob(pJob);
}

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
/* get the ename set of the named (universal) job                             */
/*----------------------------------------------------------------------------*/
EnameSet* Job::pJobInfo_enameSet(Stz* szJobNm)
{
  s32 sJobNm_cls = szJobNm;
  sJobNm_cls.vConcat("_cls"_Stz);
  
  Word* pWord_jobInfo = AM::pRootDic()->pFindWord(sJobNm_cls);
  
  if (!pWord_jobInfo)
    return nullptr;
  
  EnameSet* pEnameSet = pWord_jobInfo->pEnameSet();
  
  return pEnameSet;
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
/* test if this job has a /byPrompt ename tag                                 */
/*----------------------------------------------------------------------------*/
bool Job::bByPromptTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bByPrompt = bHasEnameTag(C_TAG_BYPROMPT);
  
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
         "m_nxReferences=%lld\n"_Stz,

         this,
         aCN[eClass()],
         
         aJT[m_eJobType]->azChar(),
         aJK[m_eJobKind]->azChar(),
         
         aJL[m_eJobList],
         aJP[m_eJobPhase],
         aJS[m_eJobState],
         
         bByPromptTag(),
         bNoWaitTag(),
         m_bWasReceived,
         
         m_iParentId,
         m_iOriginatingId,
         
         nReferences(),
         
         C_ENDARGS
         );

  m_oEnameSet.vContentDump();
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Job::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* test if this job has given ename tag                                       */
/*----------------------------------------------------------------------------*/
bool Job::bHasEnameTag(Stz* szEnameTag)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (m_oEnameSet.bHasName(szEnameTag))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get the job id                                                             */
/*----------------------------------------------------------------------------*/
i8 Job::iJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iFocalId;
}

/*----------------------------------------------------------------------------*/
/* get the job id                                                             */
/*----------------------------------------------------------------------------*/
EnameSet* Job::pJobInfoEnameSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Stz* szJobNm = m_oEnameSet.szValue("<jobNm>"_Stz);
  
  // did we get a job name ?
  if ( szJobNm->bEqual(""_Stz) ) {
    // would have been a startup problem (unable to match job syntax !!!)
    ASSERT(m_bHasError);
    return nullptr;
  }
  
  s32 sJobInfoNm(szJobNm);
  
  StrLib::vConcat(sJobInfoNm.azChar(),(const char*)"_cls"_Stz);
  
  Word* pJobInfo = AM::pRootDic()->pFindWord(sJobInfoNm);
  
  EnameSet* pJobInfoEnameSet = pJobInfo->pEnameSet();

  return pJobInfoEnameSet;
}

/*----------------------------------------------------------------------------*/
/* write an activity map string about this job                                */
/*----------------------------------------------------------------------------*/
void Job::vMarkJob(void)
{
  s48K sJobMark;
  
  sJobMark.vSprintCat("Job %03lld %s session %lld slot %lld\n"_Stz,
                      m_iFocalId,
                      aCN[eClass()],
                      m_xReqTuple.m_i_Session,
                      m_i_Slot);
  sJobMark.vSprintCat("  phase %s state %s list %s"_Stz,
                      aJP[m_eJobPhase],
                      aJS[m_eJobState],
                      aJL[m_eJobList]);
  if (m_bWasReceived)
    sJobMark.vSprintCat(" (recv from %s)"_Stz,
                        szNodeNm());

  sJobMark.vConcat("\n"_Stz);

  AM::vLogText(m_iFocalId,sJobMark);
}

/*----------------------------------------------------------------------------*/
/* write a mark about this job                                                */
/*----------------------------------------------------------------------------*/
void Job::vMarkJob(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);

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
/* test if this job has a /noWait ename tag                                   */
/*----------------------------------------------------------------------------*/
bool Job::bNoWaitTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bNoWait = bHasEnameTag(C_TAG_NOWAIT);
  
  return bNoWait;
}

/*----------------------------------------------------------------------------*/
/* lock the job mutex                                                         */
/*----------------------------------------------------------------------------*/
void Job::vLockJobMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  AM::vTaskMutexLock(m_pTaskJ);
}

/*----------------------------------------------------------------------------*/
/* signal the job mutex                                                       */
/*----------------------------------------------------------------------------*/
void Job::vSignalJobMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  AM::vTaskMutexSignal(m_pTaskJ);
}

/*----------------------------------------------------------------------------*/
/* unlock the job mutex                                                       */
/*----------------------------------------------------------------------------*/
void Job::vUnlockJobMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  AM::vTaskMutexUnlock(m_pTaskJ);
}

/*----------------------------------------------------------------------------*/
/* wait for job mutex to be signalled                                         */
/*----------------------------------------------------------------------------*/
void Job::vWaitJobMutex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
 
  AM::vTaskMutexWait(m_pTaskJ);
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
/* get the originator node no of this job from the job id                     */
/*----------------------------------------------------------------------------*/
i8 Job::iOriginatorNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 iMaskOffLower48Bits = 0xFFFF000000000000;
  
  i8 iOriginatorNode = m_iFocalId & iMaskOffLower48Bits;
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
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void Job::vPrint(Stz* szFormat, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  s4K sFormatted;
  
  SP_Timer spTimer;
  
  va_list args;
  
  va_start(args,szFormat);
  
  m_oTimer.vUpdate();
  
  sFormatted.vSprint(args,szFormat);
  
  vTrace("[%s %s %s] %s\n"_Stz,
         AM::pGrid()->pThisNode()->szNodeNm(),
         m_sJobNm.szStz(),
         spTimer.sz(&m_oTimer),
         
         sFormatted.szStz(),
         
         C_ENDARGS);

  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* report this job                                                            */
/*----------------------------------------------------------------------------*/
void Job::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  vTrace("Job %llx id %lls ackRemaining %lld\n"_Stz,
         this,
         m_iFocalId,
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
  
  Stz* szSeed = m_aJobClass->sSeed(m_eSeed);
  
  return szSeed;
}

/*----------------------------------------------------------------------------*/
/* set the seed to be spawned                                                 */
/*----------------------------------------------------------------------------*/
void Job::vSeed(eSS eSeed)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eSeed = eSeed;
  
  if (m_oEnameSet.bHasName("<seed>"_Stz))
    m_oEnameSet.vReplace("<seed>"_Stz,aSS[eSeed]);
  else
    m_oEnameSet.vAdd("<seed>"_Stz,aSS[eSeed]);

  m_aJobClass->m_bSeedsSpawned[eSeed] = true;
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
/* get slot on this node this sub job processes                               */
/*----------------------------------------------------------------------------*/
i8 Job::i_Slot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_i_Slot;
}

/*----------------------------------------------------------------------------*/
/* set slot on this node this sub job processes                               */
/*----------------------------------------------------------------------------*/
void Job::vSlot(i8 i_Slot)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_i_Slot = i_Slot;
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
//         m_aJobClass ? (Stz*)m_aJobClass->m_aName : "<no jobClass>"_Stz,
//         s.szStz());
//}
void Job::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vTrace("%s %s parentId %03lld seed %s "
         "i_Session %03lld i_Slot %lld WasReceived %lld\n"_Stz,
         aCN[eClass()],
         m_aJobClass ? aCN[m_aJobClass->m_eClass] : "<no jobClass>"_Stz,
         m_iParentId,
         aSS[m_eSeed],
         
         m_xReqTuple.m_i_Session,
         m_i_Slot,
         (i8)m_bWasReceived
         );
}

Task* Job::pTask(void)
{
  return m_pTaskJ;
}

void Job::vTask(Task* pTask)
{
  ASSERT(! pTask || ! m_pTaskJ);
  m_pTaskJ = pTask;
}

/*----------------------------------------------------------------------------*/
/* test if this job has a /time ename tag                                     */
/*----------------------------------------------------------------------------*/
bool Job::bTimeTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bTime = bHasEnameTag(C_TAG_TIME);
  
  return bTime;
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
eEC Jobs::eJob_checkCluster(Session* pSession)
{
  if (AM::pGrid()->m_bHasStarted)
    return EC_NONE;
  else
    return EC_CLUSTER_NOT_CONNECTED;
}

/*----------------------------------------------------------------------------*/
/* post-execute job function                                                  */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_post(Job* pJob)
{
  eCN eClass = pJob->eClass();
  
  //! why wouldn't this be set by now??
  JobClass* pJobClass = JobClass::pFind(eClass);
  pJob->m_aJobClass = pJobClass;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eRS eResponse = RS_NONE;
  
  if (CN_JOB_UNIVERSAL == eClass) {
    
    EnameSet* pJobInfoEnameSet = pJob->pJobInfoEnameSet();
    if (! pJobInfoEnameSet)
      return;
    eResponse = (eRS) pJobInfoEnameSet->iValue("<?rspKind?>"_Stz);

    //eResponse = pJob->m_aJobClass->m_eResponse;
  }
  else {
    eResponse = pJob->m_aJobClass->m_eResponse;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  switch (eResponse) {
      
    case RS_NONE         : /* do nothing */            break;
    case RS_DATA_ROWS    : RASSERT(0);                 break;
    case RS_LOAD_ROWS    : pJob->vPrintLoadRows();     break;
    case RS_LOCAL_ERRORS : pJob->vPrintLocalErrors();  break;
    case RS_LOCAL_ROWS   : pJob->vPrintLocalRows();    break;
    case RS_LOCAL_ZERO   : pJob->vPrintLocalZero();    break;
    case RS_NODE_ERRORS  : pJob->vPrintNodeErrors();   break;
    case RS_NODE_ROWS    : pJob->vPrintNodeRows();     break;
    case RS_NODE_ZERO    : pJob->vPrintNodeZero();     break;
    case RS_QUEUE_ROWS   : pJob->vPrintQueueRows();     break;
      
    default:
      vTrace("Invalid response in job %s : %s\n"_Stz,
             aCN[pJob->m_aJobClass->m_eClass],
             aRS[pJob->m_aJobClass->m_eResponse]);
  }
}

/*----------------------------------------------------------------------------*/
/* pre-send job function                                                      */
/*----------------------------------------------------------------------------*/
JOBS_fn(vJob_send)
{

}

/*----------------------------------------------------------------------------*/
/* post-recv job function  (when is this run ????)                            */
/*----------------------------------------------------------------------------*/
JOBS_fn(vJob_recv)
{
  pJob->m_bWasReceived   = true;
  
  pJob->m_eJobList       = JL_NONE;
  
  pJob->vTask(nullptr);
  
  pJob->m_i_Focal        = C_UNKNOWN;
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
