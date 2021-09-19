/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "ApiMacros.h"
#include "Api.hpp"
#include "Session.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Op.hpp"
#include "LogicUnit.hpp"
#include "Procedure.hpp"
#include "Sessions.hpp"
#include "Output.hpp"

#include "TagConstants.h"

#include "SP_TagSet.hpp"

extern BufferPool*  gpBufferPool;

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;

/*----------------------------------------------------------------------------*/
/*-- parsimonious API --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a new instance of an operand                                           */
/*----------------------------------------------------------------------------*/
Operand* Api::pGetOperand(eZK eOperand)
{
  _static_
  
  switch (eOperand) {
    default: NEVER_REACH;
  }
  
  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
Session* Api::pAllocSession(eSK eSession)
{
  _static_
  
  
  Sessions* pSessions = AM::pSessions();
  
  Session* pSession = pSessions->pAllocSession(eSession);
    
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
void Api::vFreeSession(Session* pSession)
{
  _static_
  
  Sessions* pSessions = AM::pSessions();

  pSessions->vFreeSession(pSession);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete job  =                                                              */
/*----------------------------------------------------------------------------*/
void Api::vDeleteJob(Session* pSession)
{
  _static_
    
  Job* pJob = pSession->m_pSsnJob;

  if (pJob && pJob->bIsPipeline() && ! pSession->m_pOutput->m_bExitMarker)
    pSession->m_pOutput->vProcessBufferQueue(pSession);

  pSession->m_pOutput->m_pOutputQueue->vReset();

  pSession->m_pSsnJob = nullptr;
  
  
  pJob->vReapable(true);
      
  
  
  if (pSession->m_pOutput->m_pStreamDef_result) {
    StreamDef* pStreamDef = pSession->m_pOutput->m_pStreamDef_result;
    pSession->m_pOutput->m_pStreamDef_result = nullptr;
    delete pStreamDef;
  }
  
  pSession->m_pOutput->m_bExitMarker      = false;
  pSession->m_pOutput->m_bExitEarlyMarker = false;
}

/*----------------------------------------------------------------------------*/
/* examine command                                                            */
/*----------------------------------------------------------------------------*/
void Api::vExamineCommand(Session* pSession)
{
  _static_
    
}

/*----------------------------------------------------------------------------*/
/* execute job (spawn job and wait for completion)                            */
/*----------------------------------------------------------------------------*/
void Api::vExecuteJob(Session* pSession)
{
  _static_
  
  // AM::pHeaps()->vSetTraceFree(CN_PIPELINE);

  pSession->m_pOutput->vAssertEmptyQueue();

  Job* pJob = pSession->m_pSsnJob;
  TagSet* pJobTagSet = & pSession->m_oJobTagSet;
  TokenVec* pInpTknVec = pSession->m_pInput->pTokenVec();

  //pInpTknVec->vDumpVec(false);
  //pJobTagSet->vDumpSet(true);

//  if (pJobTagSet->bHasName(C_TAG_TRACEAPI)) {
//    vTrace("TagSet:\n"_Stz);
//    pJobTagSet->vDumpSet(true);
//  }
  
  if (AM::TRACE::bVerbose(C_VB_API)) {
    pInpTknVec->vDumpVec(false);
    pJobTagSet->vDumpSet(true);
  }
      
  if (AM::bDeveloper() && pInpTknVec->nTokens() > C_1KB) {
    i8 nTokens = pInpTknVec->nTokens();
    i8 nUsed   = pInpTknVec->nUsedByteSpace();
    if (nTokens > C_1KB)
       vTrace("TokenVec usage: nTokens=%lld nUsed=%lld(B)\n"_Stz,nTokens,nUsed);
  }

  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (;;) {
  
    bool bSpawned = Api::bSpawnTask_session(pJob);
    UNUSED(bSpawned);

    if (! pSession->bHasCascade())
      return;
  
    if (AM::TRACE::bVerbose(C_VB_API)) {
      vTrace("<vb_api>:: cascading: \n"
             "%s\n"
             "end <vb_api>\n"_Stz
             ,pSession->m_sCascade.azChar());
    }
    
    //AM::pHeaps()->vSetTraceFree(true,MY::pTask());
    
    pJob->vResetForCascade(pSession);
    
    pJob->vSetFromCascade(pSession->m_sCascade);
    
    pSession->vHasCascade(false);
    pSession->vCascade(""_Stz);
    
    Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
    
    pJob->vEntry(szEntry);

    //AM::pHeaps()->vSetTraceFree(false);
  }
}

/*----------------------------------------------------------------------------*/
/* extract paths                                                              */
/*----------------------------------------------------------------------------*/
void Api::vExtractCmdPaths(Session* pSession)
{
  _static_
  
  TokenVec* pInpTknVec = pSession->m_pInput->pTokenVec();;
  TagSet* pJobTagSet = & pSession->m_oJobTagSet;;
  
  i8 i_Paths = pInpTknVec->iFindToken("paths"_Stz,TN_NAME);

  if (C_UNKNOWN == i_Paths)
    return;
  
  pInpTknVec->vRemoveToken(i_Paths);

  s32 sName;
  
  i8 nPaths;
  for (nPaths = 0; i_Paths < pInpTknVec->nTokens(); nPaths++) {
    sName.vSprint("<path%lld>"_Stz,nPaths);
    Token* pTkn = pInpTknVec->pGet(i_Paths);
    pJobTagSet->vAppend(sName,pTkn->szChars());
    pInpTknVec->vRemoveToken(i_Paths);
  }
  
  pJobTagSet->vAppend("<paths>"_Stz,nPaths);

}

/*----------------------------------------------------------------------------*/
/* extract tags                                                               */
/*----------------------------------------------------------------------------*/
bool Api::bExtractCmdTags(Session* pSession)
{
  _static_
  
  TokenVec* pInpTknVec = pSession->m_pInput->pTokenVec();;
  TagSet* pJobTagSet = & pSession->m_oJobTagSet;;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract leading job control tags (such as /time or /nowait)
  
  Token* pTkn0 = pInpTknVec->pGet(0,true);
  
  s32 sTag;
  
  while ('/' == * pTkn0->azChars() ) {
    
    Token* pTkn1 = pInpTknVec->pGet(1,true);
    sTag.vSprint("<%s>"_Stz,pTkn1->szChars());
    
    pJobTagSet->vInsert(sTag,1);
    
    pInpTknVec->vRemoveToken(0);
    pInpTknVec->vRemoveToken(0);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract trailing job supplemtntal tags ( such as <field_delimiter> = | )

  i8 i_TagBegin = pInpTknVec->iFirstTag2();
  if (C_UNKNOWN == i_TagBegin)
    return true;

  Token* pTkn_begin = pInpTknVec->pGet(i_TagBegin);
  UNUSED(pTkn_begin);
  
  i8 nTkns = pInpTknVec->nTokens();

  i8 i_Scan = i_TagBegin;
  while (i_Scan < nTkns) {
  
    Token* pScan = pInpTknVec->pGet(i_Scan++);        // <
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsLessThan())
      return false;
    
    Token* pTkn_name = pInpTknVec->pGet(i_Scan++);    // name
    if (i_Scan >= nTkns)
      return false;
    if (! pTkn_name->bIsName())
      return false;

    pScan = pInpTknVec->pGet(i_Scan++);               // >
    if (! pScan->bIsGreaterThan())
      return false;
    
    pScan = pInpTknVec->pGet(i_Scan++);               // =
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsAssign())
      return false;

    Token* pTkn_value = pInpTknVec->pGet(i_Scan++);   // value
    //if (! pTkn_value->bIsString())
    //  return false;

    s32 sName;
    sName.vSprint("<%s>"_Stz,pTkn_name->szChars());
    pJobTagSet->vAppend(sName,pTkn_value->szChars());
  }
    
  pInpTknVec->vTrimTokens(i_TagBegin);
  
  return true;

}

/*----------------------------------------------------------------------------*/
/* new a and prepare it to execute the command in session                     */
/*----------------------------------------------------------------------------*/
eEC Api::ePrepareJob(Session* pSession, TokenVec* pDemoLiterals)
{
  _static_

  RASSERT(pSession);
  RASSERT(! pSession->m_pSsnJob);
    
  // get the command proper and the tag set
  TagSet*   pSsnTags   = & pSession->m_oSsnTagSet;
  TokenVec* pCmdTknVec = pSession->m_pInput->pTokenVec();
  TagSet*   pCmdTags   = & pSession->m_oJobTagSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vExtractCmdPaths(pSession);
  
  bool b = bExtractCmdTags(pSession);
  
  if (bShortForm(pCmdTknVec)) {
    if (! bExpandToLongForm(pCmdTknVec)) {
      return EC_INVALID_COMMAND;
    }
  }
  else {
    Word* pProcWord = Procedure::pFindProcedure(pCmdTknVec);
    if (pProcWord) {
      sSTMT sPipelineSmt;
      if (Procedure::bTranslateProcedure(sPipelineSmt.pString(),
                                         pCmdTknVec,pProcWord)) {
      
        pCmdTknVec->vSoftReset();
        pCmdTknVec->vTokenise(sPipelineSmt);
      }
    }
  }
    
  if (! AM::bParseCmd(pCmdTknVec,pCmdTags))
    return EC_INVALID_COMMAND;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //pCmdTags->vDumpSet();

  Stz* szJobNm = pCmdTags->szValue(C_TAG_JOBNM);

  Job* pPrepareJob =
         pSession->m_pSsnJob =
           new Job(szJobNm,
                   AM::GRID::iThisNodeNo(),
                   pSession->m_i_Session,
                   pSession->m_iCmdNo++
                   );
  pPrepareJob->vSeed(SS_SEED_REQ);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TagSet* pPrepareJobTags = pPrepareJob->pJobTags();
  //pPrepareJobTags->vDumpSet();


  // parse cmd
    
  pCmdTknVec->vVecToString(pPrepareJob->m_sCmd,0,true);


  // / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / ///
  //                          For DEMO (to be removed)                        //
  // / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / ///
  // create tags in the job for the params from the literal token vector
  if (pDemoLiterals && pDemoLiterals->nItems()) {
    
    for (i8 i_Tkn = 0; i_Tkn < pDemoLiterals->nItems(); i_Tkn++) {
      
      Token* pTkn = pDemoLiterals->pGet(i_Tkn);
      
      Stz* szTag;
      switch(i_Tkn) {
        case 0: szTag = C_TAG_PARAM0; break;
        case 1: szTag = C_TAG_PARAM1; break;
        case 2: szTag = C_TAG_PARAM2; break;
        case 3: szTag = C_TAG_PARAM3; break;
        case 4: szTag = C_TAG_PARAM4; break;
        default:
          RASSERT(0);
      }
      
      switch(pTkn->eToken()) {
      case TN_STRING:
        pPrepareJobTags->vInsert(szTag,pDemoLiterals->pGet(i_Tkn)->szChars());
        break;
      case TN_INTEGER:
        pPrepareJobTags->vInsert(szTag,pDemoLiterals->pGet(i_Tkn)->iToI8());
        break;
      default:
        RASSERT(0);
      }

    }
    
  }
  // / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /

  
  pPrepareJobTags->vInsert(C_TAG_CMD,pPrepareJob->m_sCmd);
  
  pPrepareJobTags->vCopy(pSsnTags);

  //pCmdTags->vDumpSet(true);
  
  pPrepareJobTags->vCopy(pCmdTags);
  
  if (AM::TRACE::bVerbose(C_VB_API))
    pPrepareJobTags->vDumpSet();

  if (pPrepareJob->bHasTag(C_TAG_LIFE)) {
    SP_TagSet spTagSet;
    vTrace("Job New: %s"_Stz,spTagSet.sz(pPrepareJobTags));
    //pJob->pTagSet()->vPrint();
  }
  
  if ( pPrepareJob->bHasTag(C_SHOW_CMD_TAGS))
    pPrepareJobTags->vDumpSet(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eJK eJobKind = (eJK) pPrepareJobTags->iValue(C_TAG_JOBKIND);
  pPrepareJob->vJobKind(eJobKind);
    
  if (pPrepareJobTags->bHasName(C_SEE_JOB))
    pPrepareJob->vSee();
  
  
  //pPrepareJobTags->vDumpSet(true);

  
  Dic* pDicRoot = AM::DIC::pRoot();
  
  //Stz* szSeed = pPrepareJob->szSeed();
  Stz* szEntry = pPrepareJobTags->szValue(C_TAG_ENTRYPOINT);
  
  pPrepareJob->vEntry(szEntry);

  Stz* szSeed = "job::job"_Stz;
  Word* pSeedWord = pDicRoot->pFindWord(szSeed);

  if (! pSeedWord) {
 
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Unable to find seed word: %s\n"_Stz,szSeed);
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* submit job (spawn job but don't wait for completion)                       */
/*----------------------------------------------------------------------------*/
void Api::vSubmitJob(Session* pSession)
{
  _static_
  
  Job* pSubmitJob = pSession->m_pSsnJob;

  Api::vMutexLock(pSession,pSubmitJob);

  RASSERT(JS_LOCKED == pSubmitJob->eJobState());

  
  RASSERT(! pSubmitJob->pJobTask());
  
  vCreateTaskForJob(pSubmitJob);
  
  pSubmitJob->vTimerStart();
  
  Scheduler::vTaskApi_AddPrompt(pSubmitJob->pJobTask());
}

/*----------------------------------------------------------------------------*/
/* wait for job completion                                                    */
/*----------------------------------------------------------------------------*/
void Api::vWaitJob(Session* pSession)
{
  Job* pJob = pSession->m_pSsnJob;
  
  //pSession->m_oMutex.vObserve(true);
  Api::vMutexWaitCond(pSession,pJob);
  pJob->vTimerUpdate();
  
  Api::vMutexUnlock(pSession,pJob);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* wait for job completion                                                    */
/*----------------------------------------------------------------------------*/
void Api::vPushOperand(Session* pSession, Operand* pOperand)
{
  Job* pJob = pSession->m_pSsnJob;

  Op* pInputOp = pJob->pSingularOp();

  pInputOp->vOutboundOperand(pOperand);
  pOperand = nullptr;
  while (pInputOp->pOutboundOperand())
    AM::vNapUsec(100,CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
bool Api::bReapJob(void)
{
  _static_
   
  
  
  
  JobList* pRetiredJobs = AM::pRetiredJobs();

  if (! pRetiredJobs->nJobs()) {
    //vTrace("Empty retired list\n"_Stz);
    return FALSE(empty_list);
  }
  
  pRetiredJobs->vGrabLock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pReapableJob = nullptr;
  
  for (SkipNode* pCursor = pRetiredJobs->pFirst();
       pCursor;
       pCursor = pRetiredJobs->pNext(pCursor)) {
    
    Job* pRetiredJob = _pJob(pCursor->m_pObject);
    
    if (pRetiredJob) {
      i8 nActiveOps = pRetiredJob->nActiveOps();
//      vTrace("REAP CANDIDATE Job %p Active Ops %lld\n"_Stz
//             ,pRetiredJob,nActiveOps);
      if (pRetiredJob->m_bReapable && ! nActiveOps) {
        pReapableJob = pRetiredJob;
        break;
      }
      else {
        vTrace("Cant reap retired job %p %lld "
               "%s %s "
               "activeOps %lld %s "
               "\n"_Stz
               
               ,pRetiredJob,pRetiredJob->iJobId() & 0xFFFFFF
               ,aJK[pRetiredJob->eJobKind()],pRetiredJob->szJobNm()
               ,nActiveOps,pRetiredJob->m_bReapable ? "reapable" : "not-reapable"
               );
      }
    }
  }

  if (! pReapableJob) {
    pRetiredJobs->vDropLock();
    return FALSE(nothing_to_reap);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bWasRemoved = pRetiredJobs->bRemoveJob(pReapableJob);
  RASSERT(bWasRemoved);
  
  pRetiredJobs->vDropLock();
  
  // delete the retired job
  
  RASSERT(pReapableJob->pJobTask());
  pReapableJob->pJobTask()->vGrabTaskLock((Stz*)__func__);
  pReapableJob->pJobTask()->vDropTaskLock((Stz*)__func__);


  pReapableJob->vGuardedDelete(CN_JOB,&pReapableJob,
                               CN_API,nullptr,(Stz*)__func__);
  
//  pSelfTask->vGuardedDelete(CN_TASK,&pSelfTask,
//                            CN_SCHEDULER,nullptr,(Stz*)__func__);

  return TRUE(reaped_a_job);
}

/*----------------------------------------------------------------------------*/
/* is the command a shortform?                                                */
/*----------------------------------------------------------------------------*/
bool Api::bShortForm(TokenVec* pCmdTknVec)
{

  i8 i_Fan = pCmdTknVec->iFindToken("*|"_Stz);
  if (C_UNKNOWN != i_Fan)
    return true;

  i_Fan = pCmdTknVec->iFindToken("|#"_Stz);
  if (C_UNKNOWN != i_Fan)
    return true;

  return false;
  
}

/*----------------------------------------------------------------------------*/
/* expand shortform into longform                                             */
/*----------------------------------------------------------------------------*/
bool Api::bExpandToLongForm(TokenVec* pCmdTknVec)
{
  TokenVec* pAllPtnVec;
  TokenVec* pOnOriginVec;
  
  s48K sLongForm;
  
  
  i8 nTkn = pCmdTknVec->nTokens();
  
  i8 i_Fan = pCmdTknVec->iFindToken("*|"_Stz);
  
  if (C_UNKNOWN != i_Fan) {
    pAllPtnVec   = TokenVec::pGetSubVec(pCmdTknVec,0,i_Fan-1);
    pOnOriginVec = TokenVec::pGetSubVec(pCmdTknVec,i_Fan+1,nTkn-1);
    
    sLongForm =
      "pipeline fanin "
      "begin "
      " "
      "  stage stageA on origin : "
      "    start | pulse once | stageB "
      " "
      "  stage stageB on all partitions : "
      "    /ALL_PARTITIONS/ | stageC "
      " "
      "  stage stageC on origin : "
      "    /ON_ORIGIN/ | stageD "
      " "
      "  stage stageD on origin : "
      "    sink | stageA "
      " "
      "end ;"_Stz;
  }
  else {
    i_Fan = pCmdTknVec->iFindToken("|#"_Stz);
    pOnOriginVec = TokenVec::pGetSubVec(pCmdTknVec,0,i_Fan-1);
    pAllPtnVec   = TokenVec::pGetSubVec(pCmdTknVec,i_Fan+1,nTkn-1);
    
    sLongForm =
      "pipeline fanout "
      "begin "
      " "
      "  stage stageA on origin : "
      "    start | pulse once | /ON_ORIGIN/ | stageB hashcast "
      " "
      "  stage stageB on all partitions : "
      "    /ALL_PARTITIONS/ | stageC "
      " "
      "  stage stageC on origin : "
      "    sink | stageA "
      " "
      "end ;"_Stz;
  }
  
  s4K sAllPtn;
  s4K sOnOrigin;

  pAllPtnVec->vToTknString(sAllPtn);
  pOnOriginVec->vToTknString(sOnOrigin);
  
  vTrace("%s\n"_Stz,sAllPtn.azChar());
  vTrace("%s\n"_Stz,sOnOrigin.azChar());

  StrLib::bReplace(sLongForm.azChar(),
                   "/ALL_PARTITIONS/",
                   sAllPtn.azChar());
  
  StrLib::bReplace(sLongForm.azChar(),
                   "/ON_ORIGIN/",
                   sOnOrigin.azChar());
  
  vTrace("%s\n"_Stz,sLongForm.azChar());
  
  pCmdTknVec->vSoftReset();
  
  pCmdTknVec->vTokenise(sLongForm);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* populate tagset from given token vector                                    */
/*----------------------------------------------------------------------------*/
void Api::vPopulateTagSet(TagSet* pTagSet, TokenVec* pTknVec)
{
  _static_
  
  i8 nTkns = pTknVec->nTokens();

  i8 i_Scan = 0;
  while (i_Scan < nTkns) {
    
    Token* pTkn_name = pTknVec->pGet(i_Scan++);    // <tagNm>
    if (i_Scan >= nTkns)
      break;
    if (TN_TAG != pTkn_name->eToken())
      break;

    Token* pTkn_equals = pTknVec->pGet(i_Scan++);  // =
    UNUSED(pTkn_equals);
    
    if (i_Scan >= nTkns)
      break;
    //if (! pTkn_equals->bIsAssign())
    //  break;

    Token* pTkn_value = pTknVec->pGet(i_Scan++);   // value
    
    pTagSet->vReplace(pTkn_name->szChars(),pTkn_value->szChars());
  }

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inc statistics                                                             */
/*----------------------------------------------------------------------------*/
void Api::vIncStatistics(eAC eApiCall)
{
  _static_
  
  
  if (! AM::bAttached())
    return;
  
  Statistics* pStats = AM::pStatistics();

  switch (eApiCall) {
    case AC_BeginFetch:          pStats->Api.m_nBeginFetch++;          break;
    case AC_BeginInsert:         pStats->Api.m_nBeginInsert++;          break;
    case AC_BeginSubInserter:    pStats->Api.m_nBeginSubInserter++;     break;
    case AC_CheckVersion:        pStats->Api.m_nCheckVersion++;         break;
    case AC_DeleteSession:       pStats->Api.m_nDeleteSession++;        break;
    case AC_EndInsert:           pStats->Api.m_nEndInsert++;            break;
    case AC_EndJobs:             pStats->Api.m_nEndJobs++;              break;
    case AC_EndSubInserter:      pStats->Api.m_nEndSubInserter++;       break;
    case AC_ExecuteCommand:      pStats->Api.m_nExecuteCommand++;       break;
    case AC_FetchNextBuffer:     pStats->Api.m_nFetchNextBuffer++;      break;
    case AC_FetchTable:          pStats->Api.m_nFetchTable++;           break;
    case AC_GetFirstBuffer:      pStats->Api.m_nGetFirstBuffer++;       break;
    case AC_GetLoadResult:       pStats->Api.m_nGetLoadResult++;        break;
    case AC_GetNextBuffer:       pStats->Api.m_nGetNextBuffer++;        break;
    case AC_GetRow:              pStats->Api.m_nGetRow++;               break;
    case AC_GetRowCount:         pStats->Api.m_nGetRowCount++;          break;
    case AC_GetTableColumnRow:   pStats->Api.m_nGetTableColumnRow++;    break;
    case AC_GetTableColumnRowCount: pStats->Api.m_nGetTableColumnRowCount++;  break;
    case AC_InsertRow:           pStats->Api.m_nInsertRow++;            break;
    case AC_IsInvalidSession:    pStats->Api.m_nIsInvalidSession++;     break;
    case AC_NewSession:          pStats->Api.m_nNewSession++;           break;
    case AC_NextRow:             pStats->Api.m_nNextRow++;              break;
    case AC_OpenPool:            pStats->Api.m_nPoolOpen++;             break;
    case AC_TranslateError:      pStats->Api.m_nTranslateError++;       break;
    default:
      NEVER_REACH;
  }
}


/*----------------------------------------------------------------------------*/
/* call the system boot job to compile the main dictionary                    */
/*----------------------------------------------------------------------------*/
void Api::vCallBootSystem(void)
{
  _static_
  
  Job* pJob = new Job("bootSystem"_Stz,
                      AM::GRID::iThisNodeNo(),
                      AM::SESSION::iInternalSession(),
                      1
                      );
  pJob->vSeed(SS_SEED_RUN);

  pJob->m_sJobNm = "boot"_Stz;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TagSet* pJobTags = pJob->pJobTags();
  //pJobTags->vDumpSet();

  // (no cmd to parse)

  pJobTags->vInsert(C_TAG_PATH,"/"_Stz);
  pJobTags->vReplace(C_TAG_SEEDRUN,"bootSystem::bootSystem"_Stz);
  
  Stz* szSeed = pJob->szSeed();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Task* pBootTask = new Task(pJob->szJobNm(),pJob);

  pJob->vJobTask(pBootTask);
  
  
  pBootTask->vAddAxioms();
  pBootTask->vAddMunged();
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("[[mainThread]] Compiling dictionaries...\n"_Stz);
  
  vSetTaskInxPtr(pBootTask,szSeed);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pJob->vTimerStart();
  
  vCallImmediate(pBootTask);
  
  pBootTask->vState(TS_RETIRED);
  pBootTask->vDropTaskLock((Stz*)__func__);

  while (Api::bReapJob())
    {;}

}

/*----------------------------------------------------------------------------*/
/* call the system start job                                                  */
/*----------------------------------------------------------------------------*/
void Api::vCallStartSystem(void)
{
  _static_

//  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
//    vInfo("\n+\n+ %lld) Start System Agents\n+\n"_Stz,giProgressPoint++);

  // this is the main thread
  PsxThd::vMarkNonSeThread();

  Job* pJob = new Job("StartSys"_Stz,
                      AM::GRID::iThisNodeNo(),
                      AM::SESSION::iInternalSession(),
                      1
                      );
  pJob->vSeed(SS_SEED_RUN);
  
  pJob->vEntry("startSystem::startSystem HAZARD"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TagSet* pJobTags = pJob->pJobTags();
  //pJobTags->vDumpSet();

  // (no cmd to parse)

  pJobTags->vInsert(C_TAG_PATH,"/"_Stz);
  pJobTags->vReplace(C_TAG_SEEDRUN,"startSystem::startSystem"_Stz);

  Stz* szSeed = pJob->szSeed();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Stz* szJobNm = pJob->szJobNm();
  Task* pStartTask = new Task(szJobNm,pJob);

  pJob->vJobTask(pStartTask);
    
  vSetTaskInxPtr(pStartTask,szSeed);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pStartTask->vIsStart(true);

  pJob->vTimerStart();

  vCallImmediate(pStartTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pStartTask->vState(TS_RETIRED);
  pStartTask->vDropTaskLock((Stz*)__func__);
  
    while (Api::bReapJob())
      {;}

}

/*----------------------------------------------------------------------------*/
/* run task immediately                                                       */
/*----------------------------------------------------------------------------*/
void Api::vCallImmediate(Task* pImmediateTask)
{
  Task* pSaveTask = _pTask(PsxThd::pTlsGet(giTLS_TaskKey));

  pImmediateTask->vIsImmediate(true);
  
  pImmediateTask->vState(TS_EXECUTING);
  
  pImmediateTask->vTaskLoop();
  
  PsxThd::vTlsSet(giTLS_TaskKey,pSaveTask);
}

/*----------------------------------------------------------------------------*/
/* delete task immediate                                                      */
/*----------------------------------------------------------------------------*/
void Api::vDeleteImmediate(Task** ppTask)
{
  NEVER_REACH;
  
  Task* pTask = *ppTask;
  *ppTask = nullptr;

  pTask->vState(TS_COMPLETED);
  Job* pJob = pTask->pJob_task();
  pTask->vJob_task(nullptr);
  
  //delete pJob;
  if (pJob)
    pJob->vGuardedDelete(CN_JOB,&pJob,
                         CN_API,nullptr,(Stz*)__func__);
  
  
  //delete pTask;
}

/*----------------------------------------------------------------------------*/
/* spawn task for given job                                                   */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTask_job(Job* pJob)
{
  _static_
  
  
  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddOp(pJob->pJobTask());
}

/*----------------------------------------------------------------------------*/
/* spawn task for given op                                                    */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTask_op(Op* pOp)
{
  _static_
  
  vCreateTaskForOp(pOp);
  
  Scheduler::vTaskApi_AddOp(pOp->pTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTaskDedicated_job(Job* pJob)
{
  _static_
  
  //RASSERT(! pJob->pTask());
  
  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddDedicated(pJob->pJobTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTaskDedicated_op(Op* pOp)
{
  _static_
  
  //RASSERT(! pJob->pTask());
  
  vCreateTaskForOp(pOp);
  Scheduler::vTaskApi_AddDedicated(pOp->pTask());
}


/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTask_demonJob(Job* pJob)
{
  _static_
  
  RASSERT(! pJob->pJobTask());

  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddService(pJob->pJobTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTask_receivedJob(Job* pJob)
{
  _static_
  
  
  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddReceived(pJob->pJobTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
bool Api::bSpawnTask_session(Job* pJob)
{
  _static_
  
  Session* pSession = pJob->pSession();
  
//  if (pSession->m_pInput)
//    vTrace("API szRawInput() :: %s\n"_Stz,pSession->m_pInput->szRawInput());
//
  
  Api::vMutexLock(pSession,pJob);

  RASSERT(JS_LOCKED == pJob->eJobState());

  
  RASSERT(! pJob->pJobTask());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = AM::iPid();
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    vWarn("Cannot find pid %lld\n"_Stz, iPid);
    return false;
  }
  
  vCreateTaskForJob(pJob);
  
  pJob->vTimerStart();
  Scheduler::vTaskApi_AddPrompt(pJob->pJobTask());

  if (! pJob->bHasTag_noWait()) {
    //pSession->m_oMutex.vObserve(true);
    Api::vMutexWaitCond(pSession,pJob);
    pJob->vTimerUpdate();
  }
  
  Api::vMutexUnlock(pSession,pJob);

  return true;
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vScheduleNextPhase(Job* pParticipantJob, Task* pActiviating2PcTask)
{
  _static_
  
  
  // THIS NOW DOESNT READ CORRECTLY
  vCreateTaskForJob(pParticipantJob);
  
  Task* pTaskParticipant = pParticipantJob->pJobTask();
  Scheduler::vTaskCross_ScheduleNextPhase(pTaskParticipant, pActiviating2PcTask);
}

/*----------------------------------------------------------------------------*/
/* validate the SE session                                                    */
/*----------------------------------------------------------------------------*/
eEC Api::eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall)
{
  _static_
  UNUSED(iTrace);
  UNUSED(eApiCall);

  // pSession cannot be null
  if (pSession == nullptr) {
    TRACE_API_ERROR("eValidateSession(a)", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSession should be a pool address
  if (!gpBufferPool->bValidAddress(pSession)) {
    TRACE_API_ERROR("eValidateSession(b)", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSession should be an SESSION class
  if (pSession->eGetClassNo() != CN_SESSION) {
    TRACE_API_ERROR("eValidateSession(c)", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* spi wait till all jobs completed                                           */
/*----------------------------------------------------------------------------*/
void Api::vSpinTillAllJobsCompleted(void)
{
  _static_
  
  // wait till no local jobs
  AM::pLocalJobs()->vGrabLock();
  while (AM::pLocalJobs()->nJobs())
    ;
  AM::pLocalJobs()->vDropLock();
  
  // wait till no participating jobs
  AM::pParticipatingJobs()->vGrabLock();
  while (AM::pParticipatingJobs()->nJobs())
    ;
  AM::pParticipatingJobs()->vDropLock();
  
  // wait till no originating jobs
  AM::pOriginatingJobs()->vGrabLock();
  while (AM::pOriginatingJobs()->nJobs())
    ;
  AM::pOriginatingJobs()->vDropLock();
  
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create task for given job                                                  */
/*----------------------------------------------------------------------------*/
void Api::vCreateTaskForJob(Job* pJob)
{
  _static_
  
  //ASSERT(SS_ENUMS != pJob->eSeed());
  
  //Stz* szSeed = pJob->szSeed();
  Stz* szSeed = "job::job"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Task* pTask = new Task(pJob->szJobNm(),pJob);

  pJob->vJobTask(pTask);
    
  vSetTaskInxPtr(pTask,szSeed);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pJob->vTimerStart();
  
  if (! pJob->pJobLogicUnit()) {
    pJob->vNewJobLogicUnit();
  }

}

/*----------------------------------------------------------------------------*/
/* create task for given op                                                   */
/*----------------------------------------------------------------------------*/
void Api::vCreateTaskForOp(Op* pOp)
{
  _static_
    
  Task* pTask = pOp->pTask();
  RASSERT(! pTask);

  Word* pEntryWord = pOp->pEntryWord();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pTask = new Task(pOp->szOpNm(),pOp);

  pOp->vTask(pTask);

  pTask->vSetInxPtr(pEntryWord);
}

/*----------------------------------------------------------------------------*/
/* set instruction pointer for task given entry point name                    */
/*----------------------------------------------------------------------------*/
void Api::vSetTaskInxPtr(Task* pTask, Stz* szEntryPt)
{
  _static_
  
  Dic* pDicRoot = AM::DIC::pRoot();
  
  
  Word* pEntryWord = pDicRoot->pFindWord(szEntryPt);

  if (! pEntryWord) {

    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Unable to find task entry point: %s\n"_Stz,szEntryPt);
  }
  
  pTask->vSetInxPtr(pEntryWord);

}

/*----------------------------------------------------------------------------*/
/* lock mutex and mark job as locked                                          */
/*----------------------------------------------------------------------------*/
void Api::vMutexLock(Session* pSession, Job* pJob)
{
  _static_
  
  pSession->m_oMutex.vLock(pSession);
        
  pJob->vJobState(JS_LOCKED);
}

/*----------------------------------------------------------------------------*/
/* mark job as running and unlock session mutex                               */
/*----------------------------------------------------------------------------*/
void Api::vMutexUnlock(Session* pSession, Job* pJob)
{
  _static_
  
  RASSERT(JS_ACTIVE != pJob->eJobState());
    
  pJob->vJobState(JS_ACTIVE);
  
  pSession->m_oMutex.vUnlock(pSession);
}

/*----------------------------------------------------------------------------*/
/* wait for session mutex to be signalled                                     */
/*----------------------------------------------------------------------------*/
void Api::vMutexWaitCond(Session* pSession, Job* pJob)
{
  _static_
  
  do {
    
    //vTrace("WAIT session %p mutex %p\n"_Stz,pSession,&pSession->m_oMutex);

    pSession->m_oMutex.vWait(pSession);
    
  } while (pJob->eJobState() != JS_SIGNALLED);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
