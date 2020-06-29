/*============================================================================*/
/* Job.cpp -- a job in the system                                             */
/*============================================================================*/

#include "Jobs.h"


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
      
  //RASSERT( ! pJob || _bIsValidJob(pJob->eClass()) );
  
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
  static_cast<Job*>(HeapMem::pAllocate(CN_JOB));
  
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
  
  HeapMem::vFree(pJob);
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
  
  m_iParentId = C_UNKNOWN;
  m_eSeed     = SS_ENUMS;
  m_i_Slot    = C_UNKNOWN;
  
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
  
  //m_xResult
  // -- -- -- -- -- --
  
  SP_Gid spId;
  
  vIncStepNo(); ++ gixJob;
  AM::vLogJob(SL_INF,this,JA_JOB_CONSTRUCT,
              "|gixJob=%lld|"_Stz, //!format-LogJob
              * gixJob);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job::Job(ReqTuple* aReqTuple)
: Focal(CN_JOB,AM::iNextJobId(),aReqTuple->m_iForId)
, m_xReqTuple(aReqTuple)
, CRM<Job>(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB);
  
  // m_xReqTuple

  m_iParentId = C_UNKNOWN;
  m_eSeed     = SS_ENUMS;
  m_i_Slot    = C_UNKNOWN;
  
  m_eJobKind  = JK_NONE;
  m_eJobPhase = JP_NONE;
  m_eJobList  = JL_NONE;
  m_eJobState = JS_RUNNING;
  
  m_bNormalSides = true; //
  m_bWasReceived = false;
  
  m_oTimer.vStart();
  //m_oAcksRmg
  
  m_pTaskJ = nullptr;
  m_pOriginatingTask = nullptr;
  m_iOriginatingId = C_UNKNOWN;

  m_iParticipantNode  = C_UNKNOWN;
  m_oAcksRmg_2pc.vCheckInit();
  
  m_pPipeline = nullptr;
  m_pOp       = nullptr;
    
  //m_xResult
  // -- -- -- -- -- --
  
  SP_Gid spId;
  
  vIncStepNo(); ++ gixJob;
  AM::vLogJob(SL_INF,this,JA_JOB_CONSTRUCT,
              "|gixJob=%lld|"_Stz, //!format-LogJob
              * gixJob);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job::~Job(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //vTrace("Deleting job: %s\n"_Stz,aCN[eClass()]);
  
//  if (bHasTag(C_TAG_LIFE) || JK_GENERIC == m_eJobKind) {
//
//    SP_TagSet spTagSet;
//    vPrint("Job Delete: %s\n"_Stz,spTagSet.sz(&m_oJobTags));
//    //m_oJobTags.vPrint();
//  }
  
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
    AM::vNapMsec(10, eClass());
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
/* add rows processed                                                         */
/*----------------------------------------------------------------------------*/
void Job::vAddRowsProcessed(i8 iRowsProcessed)
{
  m_xResult.m_xJobResponse.m_ixRowsProcessed += iRowsProcessed;
}

/*----------------------------------------------------------------------------*/
/* add rows processed                                                         */
/*----------------------------------------------------------------------------*/
void Job::vAddRowsInvalid(i8 iRowsInvalid)
{
  m_xResult.m_xJobResponse.m_ixRowsInvalid += iRowsInvalid;
}

/*----------------------------------------------------------------------------*/
/* add rows skipped                                                           */
/*----------------------------------------------------------------------------*/
void Job::vAddRowsSkipped(i8 iRowsISkipped)
{
  m_xResult.m_xJobResponse.m_ixRowsSkipped += iRowsISkipped;
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
/* set the buffer queue in the result                                         */
/*----------------------------------------------------------------------------*/
void Job::vBufferQueue(Queue* pBufferQueue)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_xResult.m_pOriginBufferQueue = pBufferQueue;
}

/*----------------------------------------------------------------------------*/
/* get the buffer queue in the result                                         */
/*----------------------------------------------------------------------------*/
Queue* Job::pBufferQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_xResult.m_pOriginBufferQueue;
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
         "m_pBufferQueue=%p\n"
         "m_pPipeline=%p\n"_Stz,
         this,
         aCN[eClass()],
         
         m_sJobNm.azChar(),
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
         m_xResult.m_pOriginBufferQueue,
         m_pPipeline,
         C_VARGS_END
         );
  
  if ( ! m_sJobNm.bSame("pipeline"_Stz) && m_pPipeline) {
    
    vTrace("\n"_Stz);
    vTrace("Pipeline Job\n"_Stz);
    vTrace("---\n"_Stz);
    
    if (m_pPipeline->pJob() != this)
      m_pPipeline->pJob()->vContentDump();
  }
  
  m_oJobTags.vContentDump();
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
bool Job::bHasTag(Stz* szEnameTag)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (m_oJobTags.bHasName(szEnameTag))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* inc rows processed                                                         */
/*----------------------------------------------------------------------------*/
void Job::vIncRowsProcessed(void)
{
  m_xResult.m_xJobResponse.m_ixRowsProcessed.iInc();
}

/*----------------------------------------------------------------------------*/
/* inc rows processed                                                         */
/*----------------------------------------------------------------------------*/
void Job::vIncRowsInvalid(void)
{
  m_xResult.m_xJobResponse.m_ixRowsInvalid.iInc();
}

/*----------------------------------------------------------------------------*/
/* set job error                                                              */
/*----------------------------------------------------------------------------*/
void Job::vJobError(eEC eErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (m_xResult.m_bJobHasError) {
    vTrace("Job got another error: '%s' (already had error: '%s') \n"_Stz,
      aEC[eErrorCode], aEC[m_xResult.m_xJobResponse.m_eErrorCode]);
  }
  else {
    m_xResult.m_xJobResponse.m_eErrorCode = eErrorCode;
    m_xResult.m_bJobHasError = true;
  }
}

/*----------------------------------------------------------------------------*/
/* get job error                                                              */
/*----------------------------------------------------------------------------*/
eEC Job::eJobError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_xResult.m_xJobResponse.m_eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* get job has error                                                          */
/*----------------------------------------------------------------------------*/
bool Job::bJobHasError(void)
{
  return m_xResult.m_bJobHasError;
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
/* get the job kind                                                           */
/*----------------------------------------------------------------------------*/
eJK Job::eJobKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eJobKind;
}

/*----------------------------------------------------------------------------*/
/* set the job kind                                                          */
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
/* set the job list                                                          */
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
/* set the job name                                                          */
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
  
  bool bNoWait = bHasTag(C_TAG_NOWAIT);
  
  return bNoWait;
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
  
  va_list args;
  
  va_start(args,szFormat);
  
  m_oTimer.vUpdate();
  
  sFormatted.vSprint(args,szFormat);
  
  vTrace("[%s %s %s %s] %s\n"_Stz,
         AM::pGrid()->pThisNode()->szNodeNm(),
         szSide(),
         m_sJobNm.szStz(),
         spTimer.sz(&m_oTimer),
         
         sFormatted.szStz(),
         
         C_VARGS_END);

  va_end(args);
}

/*----------------------------------------------------------------------------*/
/* test if this job has an error                                              */
/*----------------------------------------------------------------------------*/
bool Job::bParticipantHasError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_xResult.m_bPartHasError;
}

/*----------------------------------------------------------------------------*/
/* set job has an error                                                       */
/*----------------------------------------------------------------------------*/
void Job::vParticipantHasError(bool bHasError)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_xResult.m_bPartHasError = bHasError;
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
  pJobTags()->vInsert(C_TAG_DISTID,m_iFocalId);
  
  // copy the tagset into the job tag set
  pJobTags()->vCopy(pTagSet);
  
  eJK eJobKind = (eJK) pJobTags()->iValue(C_TAG_JOBKIND);
  vJobKind(eJobKind);
  
  Stz* szJobNm = pJobTags()->szValue(C_TAG_JOBNM);
  vJobNm(szJobNm);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* print result                                                               */
/*----------------------------------------------------------------------------*/
void Job::vPrintResult(eRS eResponse)
{
  m_xResult.vPrint(eResponse);
}

/*----------------------------------------------------------------------------*/
/* get response                                                               */
/*----------------------------------------------------------------------------*/
Response* Job::pResponse(void)
{
  return &m_xResult.m_xJobResponse;
}

/*----------------------------------------------------------------------------*/
/* get response string                                                        */
/*----------------------------------------------------------------------------*/
Stz* Job::szResponse(void)
{
  Stz* szResponse = m_xResult.m_xJobResponse.m_sResponse;
  
  return szResponse;
}

/*----------------------------------------------------------------------------*/
/* set response string                                                        */
/*----------------------------------------------------------------------------*/
void Job::vResponse(Stz* sResponse)
{
  m_xResult.m_xJobResponse.m_sResponse = sResponse;
}

/*----------------------------------------------------------------------------*/
/* set origin response vector                                                 */
/*----------------------------------------------------------------------------*/
void Job::vOriginResponseVec(ResponseVec* pResonseVec)
{
  m_xResult.m_pOriginResponseVec = pResonseVec;
}

/*----------------------------------------------------------------------------*/
/* set origin response vector                                                 */
/*----------------------------------------------------------------------------*/
ResponseVec* Job::pOriginResponseVec(void)
{
  return m_xResult.m_pOriginResponseVec;
}

/*----------------------------------------------------------------------------*/
/* get rows skipped                                                           */
/*----------------------------------------------------------------------------*/
i8 Job::iRowsProcessed(void)
{
  return *m_xResult.m_xJobResponse.m_ixRowsProcessed;
}

/*----------------------------------------------------------------------------*/
/* set rows skipped                                                           */
/*----------------------------------------------------------------------------*/
void Job::vRowsProcessed(i8 iRowsProcessed)
{
  m_xResult.m_xJobResponse.m_ixRowsProcessed = iRowsProcessed;
}

/*----------------------------------------------------------------------------*/
/* get rows skipped                                                           */
/*----------------------------------------------------------------------------*/
i8 Job::iRowsSkipped(void)
{
  return *m_xResult.m_xJobResponse.m_ixRowsSkipped;
}

/*----------------------------------------------------------------------------*/
/* set rows skipped                                                           */
/*----------------------------------------------------------------------------*/
void Job::vRowsSkipped(i8 iRowsSkipped)
{
  m_xResult.m_xJobResponse.m_ixRowsSkipped = iRowsSkipped;
}

/*----------------------------------------------------------------------------*/
/* get rows skipped                                                           */
/*----------------------------------------------------------------------------*/
i8 Job::iRowsInvalid(void)
{
  return *m_xResult.m_xJobResponse.m_ixRowsInvalid;
}

/*----------------------------------------------------------------------------*/
/* set rows skipped                                                           */
/*----------------------------------------------------------------------------*/
void Job::vRowsInvalid(i8 iRowsInvalid)
{
  m_xResult.m_xJobResponse.m_ixRowsInvalid = iRowsInvalid;
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

  if ( ! szSeed )
    szSeed = m_oJobTags.szValue("<seed>"_Stz);
  
  return szSeed;
}

/*----------------------------------------------------------------------------*/
/* set the seed to be spawned                                                 */
/*----------------------------------------------------------------------------*/
void Job::vSeed(eSS eSeed)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eSeed = eSeed;
  
  if (m_oJobTags.bHasName("<seed>"_Stz))
    m_oJobTags.vReplace("<seed>"_Stz,aSS[eSeed]);
  else
    m_oJobTags.vInsert("<seed>"_Stz,aSS[eSeed]);
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
void Job::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  vTrace("%s parentId %03lld seed %s "
         "i_Session %03lld i_Slot %lld WasReceived %lld\n"_Stz,
         aCN[eClass()],
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
  
  bool bTime = bHasTag(C_TAG_TIME);
  
  return bTime;
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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* check if the cluster has started                                           */
/*----------------------------------------------------------------------------*/
eEC Jobs::eJob_checkCluster(void)
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
  eRS eResponse = (eRS)pJob->pJobTags()->iValue("<rspKind>"_Stz);
  pJob->vPrintResult(eResponse);
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
  pJob->vWasReceived(true);
  
  pJob->vJobList(JL_NONE);
  
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
