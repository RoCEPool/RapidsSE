/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Api.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Op.hpp"
#include "LogicUnit.hpp"
#include "Procedure.hpp"
#include "Output.hpp"
#include "EventHandlers.hpp"
#include "Grid.hpp"

#include "TagConstants.h"

#include "SP_TagSet.hpp"

extern AttachMap*   gpAttachMap;
extern BufferPool*  gpBufferPool;

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* alloc a new job                                                            */
/*----------------------------------------------------------------------------*/
Job* Api::pAllocJob(Stz* szJobNm)
{
  _static_

  JobId tParentId;
  tParentId.vNextJobId();
  
  Job* pJob = new Job(szJobNm,tParentId);

  return pJob;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute job (spawn job and wait for completion)                            */
/*----------------------------------------------------------------------------*/
void Api::vExecuteJob(Job* pJob, TagSet* pCmdTagSet)
{
  _static_

  pJob->vMutexCreate();
  pJob->vOutputCreate();
  pJob->vOutputBeginProc();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //JobId tJobId = pJob->tJobId();
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (;;) {
  
    i8 iPid = AM::PROCESS::iSePid();
    if (!Process::bTestPid(iPid)) {
      // SE is not running anymore!
      vTrace("Cannot find pid %lld\n"_Stz, iPid);
      return;
    }
    
    vSubmitJob(pJob);

    if (pJob->bHasTag(C_TAG_NOWAIT)) {
      Api::vMutexUnlock(pJob);
    }
    else {
      vWaitJob(pJob);
    }
    
    if (! pJob->bHasCascade()) {
      return;
    }
          
    pJob->vCascadeReset();
    
    pJob->vCascadeSet(pJob->szCascade());
    
    pJob->vCascadePop();

    TagSet* pJobTagSet = pJob->pJobTags();
    //Stz* szEntry = pCmdTagSet->szValue(C_TAG_ENTRYPOINT);
    Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
    //vTrace("szEntry: %s\n"_Stz,szEntry);

    pJob->vEntryPoint(szEntry);

    //AM::pHeaps()->vSetTraceFree(false);
  }
}

/*----------------------------------------------------------------------------*/
/* form given job (as demon)                                                  */
/*----------------------------------------------------------------------------*/
void Api::vForkDemon(Job*& rpJob)
{
  _static_
  
  RASSERT(! rpJob->pTask());

  rpJob->vTaskCreate();
  Scheduler::vTaskApi_AddService(rpJob->pTask());
  
  rpJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* fork job (spawn job but don't wait for completion)                         */
/*----------------------------------------------------------------------------*/
void Api::vForkJob(Job*& rpJob, Op* pResponseOp)
{
  _static_
  
  RASSERT(! rpJob->pTask());

  Api::vMutexLock(rpJob);
  RASSERT(JS_LOCKED == rpJob->eJobState());
  
  rpJob->vTaskCreate();
  
  rpJob->vResponseTimerStart();
  
  Scheduler::vTaskApi_AddPrompt(rpJob->pTask());
  
  Api::vMutexUnlock(rpJob);
}

/*----------------------------------------------------------------------------*/
/* extract paths                                                              */
/*----------------------------------------------------------------------------*/
void Api::vExtractCmdPaths(TokenVec* pCmdTknVec, TagSet* pCmdTagSet)
{
  _static_
    
  i8 i_Paths = pCmdTknVec->iFindToken("paths"_Stz,TN_NAME);

  if (C_UNKNOWN == i_Paths)
    return;
  
  pCmdTknVec->vRemoveToken(i_Paths);

  s32 sName;
  
  i8 nPaths;
  for (nPaths = 0; i_Paths < pCmdTknVec->nTokens(); nPaths++) {
    sName.vSprint("<path%lld>"_Stz,nPaths);
    Token* pTkn = pCmdTknVec->pGet(i_Paths);
    pCmdTagSet->vAppend(sName,pTkn->szChars());
    pCmdTknVec->vRemoveToken(i_Paths);
  }
  
  pCmdTagSet->vAppend("<paths>"_Stz,nPaths);

}

/*----------------------------------------------------------------------------*/
/* extract tags                                                               */
/*----------------------------------------------------------------------------*/
bool Api::bExtractCmdTags(TokenVec* pCmdTknVec, TagSet* pCmdTagSet)
{
  _static_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract leading job control tags (such as /time or /nowait)
  
  Token* pTkn0 = pCmdTknVec->pGet(0,true);
  
  s32 sTag;
  
  while ('/' == * pTkn0->azChars() ) {
    
    Token* pTkn1 = pCmdTknVec->pGet(1,true);
    sTag.vSprint("<%s>"_Stz,pTkn1->szChars());
    
    pCmdTagSet->vInsert(sTag,1);
    
    pCmdTknVec->vRemoveToken(0);
    pCmdTknVec->vRemoveToken(0);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract trailing job supplemtntal tags ( such as <field_delimiter> = | )

  i8 i_TagBegin = pCmdTknVec->iFirstTag2();
  if (C_UNKNOWN == i_TagBegin)
    return true;

  Token* pTkn_begin = pCmdTknVec->pGet(i_TagBegin);
  UNUSED(pTkn_begin);
  
  i8 nTkns = pCmdTknVec->nTokens();

  i8 i_Scan = i_TagBegin;
  while (i_Scan < nTkns) {
  
    Token* pScan = pCmdTknVec->pGet(i_Scan++);        // <
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsLessThan())
      return false;
    
    Token* pTkn_name = pCmdTknVec->pGet(i_Scan++);    // name
    if (i_Scan >= nTkns)
      return false;
    if (! pTkn_name->bIsName())
      return false;

    pScan = pCmdTknVec->pGet(i_Scan++);               // >
    if (! pScan->bIsGreaterThan())
      return false;
    
    pScan = pCmdTknVec->pGet(i_Scan++);               // =
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsAssign())
      return false;

    Token* pTkn_value = pCmdTknVec->pGet(i_Scan++);   // value
    //if (! pTkn_value->bIsString())
    //  return false;

    s32 sName;
    sName.vSprint("<%s>"_Stz,pTkn_name->szChars());
    pCmdTagSet->vAppend(sName,pTkn_value->szChars());
  }
    
  pCmdTknVec->vTrimTokens(i_TagBegin);
  
  return true;

}

/*----------------------------------------------------------------------------*/
/* parse command                                                              */
/*----------------------------------------------------------------------------*/
eEC Api::eParseCmd(TokenVec* pCmdTknVec, TagSet* pCmdTagSet)
{
  _static_

  vExtractCmdPaths(pCmdTknVec,pCmdTagSet);
  
  bool b = bExtractCmdTags(pCmdTknVec,pCmdTagSet);
  UNUSED(b);
  
  if (bShortForm(pCmdTknVec)) {
    if (! bExpandToLongForm(pCmdTknVec)) {
      return EC_INVALID_COMMAND;
    }
  }
  else {
    sSTMT sPipelineSmt;
    if (Procedure::bTranslateToPipeline(sPipelineSmt.pString(),
                                          pCmdTknVec)) {
      
      pCmdTknVec->vSoftReset();
      pCmdTknVec->vTokeniseSen(sPipelineSmt);
    }
  }
    
  if (! AM::bParseCmd(pCmdTknVec,pCmdTagSet))
    return EC_INVALID_COMMAND;


  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* new a job and prepare it to execute a command                              */
/*----------------------------------------------------------------------------*/
eEC Api::ePrepareJob(Job*& rpJob, Stz* szJobNm,
                     Stz* szCmd, Stz* szCmdTagStr,
                     bool bParsed, TokenVec*& rpCmdTknVec, TagSet*& rpCmdTagSet)
{
  _static_

  eEC eError = EC_NONE;

  
  //RASSERT(bParsed);

  
  if (! bParsed) {
   rpCmdTknVec = TokenVec::pTokeniseSen(szCmd);
   rpCmdTagSet = new TagSet;

    TokenVec* pTagStrTknVec = TokenVec::pTokeniseSen(szCmdTagStr);
    Api::vPopulateTagSet(rpCmdTagSet,pTagStrTknVec);
    delete pTagStrTknVec;

    eEC eError = Api::eParseCmd(rpCmdTknVec,rpCmdTagSet);
    if (EC_NONE != eError) {
      delete rpCmdTknVec;
      rpCmdTknVec = nullptr;
      delete rpCmdTagSet;
      rpCmdTknVec = nullptr;
      return eError;
    }
  }
  
  if (! rpJob) {
    
    eError = Api::eParseCmd(rpCmdTknVec,rpCmdTagSet);
    if (EC_NONE != eError) {
      return eError;
    }
    
    Stz* szJobNm = rpCmdTagSet->szValue(C_TAG_JOBNM);
    rpJob = Api::pAllocJob(szJobNm);
  }
  
  
  
  
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(rpJob);

  TagSet* pJobTagSet = rpJob->pJobTags();

  // parse cmd
  sSTMT sStmt;
  rpCmdTknVec->vVecToString(sStmt,0,true);
  rpJob->vCmd(sStmt);
  
  pJobTagSet->vCopy(rpCmdTagSet);
  //pCmdTagSet->vDumpSet(true);

  if (AM::TRACE::bVerbose(C_VB_API))
    pJobTagSet->vDumpSet();

  if (rpJob->bHasTag(C_TAG_LIFE)) {
    SP_TagSet spTagSet;
    vTrace("Job New: %s"_Stz,spTagSet.sz(pJobTagSet));
    //pJob->pTagSet()->vPrint();
  }
  
  if ( rpJob->bHasTag(C_SHOW_CMD_TAGS))
    pJobTagSet->vDumpSet(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eJK eJobKind = (eJK) pJobTagSet->iValue(C_TAG_JOBKIND);
  rpJob->vJobKind(eJobKind);
    
  if (pJobTagSet->bHasName(C_SEE_JOB))
    rpJob->vPrintJobContents();
  
  
  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  
  rpJob->vEntryPoint(szEntry);
  
    
  if (AM::TRACE::bVerbose(C_VB_API)) {
    //rpCmdTknVec->vDumpVec(false);
    //rpCmdTagSet->vDumpSet(true);
    pJobTagSet->vDumpSet();
  }
      
  if (AM::bDeveloper() && rpCmdTknVec->nTokens() > C_1KB) {
    i8 nTokens = rpCmdTknVec->nTokens();
    i8 nUsed   = rpCmdTknVec->nUsedByteSpace();
    if (nTokens > C_1KB)
       vTrace("TokenVec usage: nTokens=%lld nUsed=%lld(B)\n"_Stz,nTokens,nUsed);
  }

  return EC_NONE;
}



/*----------------------------------------------------------------------------*/
/* new a job for given command and prepare to execute it                      */
/*----------------------------------------------------------------------------*/
eEC Api::ePrepareJob_2(Job*& rpJob, TokenVec* pCmdTknVec, TagSet* pCmdTagSet)
{
  _static_
  
  //vTrace("Api::ePrepareJob_2\n"_Stz);

  TagSet* pSynTagSet = new TagSet;

  eEC eError = Api::eParseCmd(pCmdTknVec,pSynTagSet);
  if (EC_NONE != eError) {
    delete pSynTagSet;
    return eError;
  }

  Stz* szJobNm = pSynTagSet->szValue(C_TAG_JOBNM);
  RASSERT(szJobNm);
  
  rpJob = Api::pAllocJob(szJobNm);

  TagSet* pJobTagSet = rpJob->pJobTags();
  pJobTagSet->vCopy(pSynTagSet);
  pJobTagSet->vCopy(pCmdTagSet);
  //pCmdTagSet->vCopy(pSynTagSet);
  
  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  rpJob->vCmd(sCmd);

  Stz* szEntry = pJobTagSet->szValue(C_TAG_ENTRYPOINT);
  //vTrace("szEntry: %s\n"_Stz,szEntry);
  rpJob->vEntryPoint(szEntry);

  eJK eJobKind = (eJK) pJobTagSet->iValue(C_TAG_JOBKIND);
  rpJob->vJobKind(eJobKind);

  delete pSynTagSet;

  return EC_NONE;
}


/*----------------------------------------------------------------------------*/
/* set job as reapable                                                        */
/*----------------------------------------------------------------------------*/
void Api::vReapableJob(Job*& rpJob)
{
  _static_
      
  if (rpJob && rpJob->bIsPipeline() && ! rpJob->pOutput()->m_bExitMarker)
    rpJob->pOutput()->vProcessBufferQueue(rpJob);

  rpJob->pOutput()->m_pOutputQueue->vReset();
  rpJob->pOutput()->vEndProcOneCmd();
  
  rpJob->vReapable(true);
      
  
  
  if (rpJob->pOutput()->m_pStreamDef_result) {
    StreamDef* pStreamDef = rpJob->pOutput()->m_pStreamDef_result;
    rpJob->pOutput()->m_pStreamDef_result = nullptr;
    delete pStreamDef;
  }
  
  rpJob->pOutput()->m_bExitMarker      = false;
  rpJob->pOutput()->m_bExitEarlyMarker = false;
  
  rpJob = nullptr;
}

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
void Api::vReapAllJobs(bool bForce)
{
  _static_
  
  if (bForce || AM::SYSTEM::bReapEnabled())
    while (AM::pRetiredJobs()->nJobs())
      while (Api::bReapJob())
        {;}
}

/*----------------------------------------------------------------------------*/
/* attempt to reap one job from the retired list                              */
/*----------------------------------------------------------------------------*/
void Api::vReapJobsOnce(bool bForce)
{
  _static_
  
  if (bForce || AM::SYSTEM::bReapEnabled())
    while (Api::bReapJob())
      {;}
}

/*----------------------------------------------------------------------------*/
/* fork internal command with given tag string                                */
/*----------------------------------------------------------------------------*/
eEC Api::eForkCommand(Stz* szCmd, Stz* szWithTagStr, Stz* szTagPath,
                      Op* pResponseOp, JobId& rtJobId)
{
  _static_
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenise the command
  
  TokenVec* pCmdTknVec = nullptr;;
  TagSet* pCmdTagSet = nullptr;

  Job* pJob = nullptr;
  eEC eError = Api::ePrepareJob(pJob,"JobNm"_Stz,szCmd,szWithTagStr,false,pCmdTknVec,pCmdTagSet);

  if (eError) {
    return eError;
  }
  
  rtJobId = pJob->tJobId();
   
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // update the job tag set
  

  TagSet* pJobTagSet = pJob->pJobTags();
  
  if (szTagPath) {
    pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  }
  
  TokenVec* pWithTagStrTknVec = TokenVec::pTokeniseSen(szWithTagStr);
  Api::vPopulateTagSet(pJobTagSet,pWithTagStrTknVec);
  
  
  Now xNow;
  xNow.vSetNow();

  s32 sHostName;
  Hardware::vHostName(sHostName);

  pJobTagSet->vInsert(C_TAG_FREEPCT,gpBufferPool->iFreePercent());
  pJobTagSet->vInsert(C_TAG_GRID,AM::pGrid()->szGridNm());
  pJobTagSet->vInsert(C_TAG_HOST,sHostName);
  pJobTagSet->vInsert(C_TAG_NODE,AM::pGrid()->pThisNode()->szNodeNm());
  pJobTagSet->vInsert(C_TAG_SHM,gpBufferPool->szShmNm());
  pJobTagSet->vInsert(C_TAG_WHEN,xNow.sz());


  
  if (AM::TRACE::bVerbose(C_VB_API))
    pJobTagSet->vDumpSet();

  pJob->vResponseTimerStart();
  
  Api::vForkJob(pJob,pResponseOp);

  
  
  
  
  //delete pReqTknVec;
  delete pWithTagStrTknVec;

  return EC_NONE;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* submit job (spawn job but don't wait for completion)                       */
/*----------------------------------------------------------------------------*/
void Api::vSubmitJob(Job* pJob)
{
  _static_
  
  RASSERT(! pJob->pTask());
  
  Api::vMutexLock(pJob);
  RASSERT(JS_LOCKED == pJob->eJobState());
  
  pJob->vTaskCreate();
  
  pJob->vResponseTimerStart();
  
  Scheduler::vTaskApi_AddPrompt(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* wait for job completion                                                    */
/*----------------------------------------------------------------------------*/
void Api::vWaitJob(Job* pJob)
{
  _static_
    
  //pJob->m_oMutex.vObserve(true);
  Api::vMutexWaitCond(pJob);
  
  pJob->vResponseTimerUpdate();
  
  Api::vMutexUnlock(pJob);
}

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
  
  AM::MASTERLOCK::vGrabReap(nullptr);
  
  pRetiredJobs->vGrabLock();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pReapableJob = nullptr;
  
  for (SkipNode* pCursor = pRetiredJobs->pFirst();
       pCursor;
       pCursor = pRetiredJobs->pNext(pCursor)) {
    
    Job* pRetiredJob = _pJob(pCursor->m_pObject);
    
    if (pRetiredJob) {
      i8 nActiveOps = pRetiredJob->nActiveOps();
//      vTrace("REAP CANDIDATE Job %p Reapable %lld Active Ops %lld\n"_Stz
//             ,pRetiredJob,(i8)pRetiredJob->m_bReapable,nActiveOps);
      if (pRetiredJob->bReapable() && ! nActiveOps) {
        pReapableJob = pRetiredJob;
        break;
      }
      else {
        vTrace("Cant reap retired job %p %lld "
               "%s %s "
               "activeOps %lld %s "
               "\n"_Stz

               ,pRetiredJob,pRetiredJob->tJobId().uJobNo()
               ,aJK[pRetiredJob->eJobKind()],pRetiredJob->szJobNm()
               ,nActiveOps,pRetiredJob->bReapable() ? "reapable" : "not-reapable"
               );
      }
    }
  }

  pRetiredJobs->vDropLock();
  
  if (! pReapableJob) {
    AM::MASTERLOCK::vDropReap(nullptr);
    return FALSE(nothing_to_reap);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pRetiredJobs->vRemoveJob(pReapableJob);

  AM::MASTERLOCK::vDropReap(nullptr);

  // delete the reapavle job
  
  RASSERT(pReapableJob->pTask());
  pReapableJob->pTask()->vGrabTaskLock((Stz*)__func__);
  pReapableJob->pTask()->vDropTaskLock((Stz*)__func__);


  delete pReapableJob;

  return TRUE(reaped_a_job);
}

/*----------------------------------------------------------------------------*/
/* is the command a shortform?                                                */
/*----------------------------------------------------------------------------*/
bool Api::bShortForm(TokenVec* pCmdTknVec)
{
  _static_
  
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
  _static_
  
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
  
  pCmdTknVec->vTokeniseSen(sLongForm);
  
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
void Api::vCallBootSystemJob(void)
{
  _static_
  
  JobId tParentId;
  tParentId.vNextJobId();
  
  Job* pBootJob = new Job("bootSystem"_Stz,tParentId);
  
  Stz* szJobNm = pBootJob->szJobNm();
  Task* pBootTask = new Task(szJobNm,pBootJob);
  pBootJob->vTask(pBootTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     
  pBootTask->vAddAxioms();
  pBootTask->vAddMunged();
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("[[mainThread]] Compiling dictionaries...\n"_Stz);
  
  pBootTask->vSetInxPtr("bootSystem::bootSystem"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TagSet* pJobTags = pBootJob->pJobTags();
  //pJobTags->vDumpSet();

  // (no cmd to parse)

  pJobTags->vReplace(C_TAG_PATH,"/"_Stz);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pBootJob->vResponseTimerStart();
  vCallTaskImmediate(pBootTask);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pBootTask->vState(TS_RETIRED);
  pBootTask->vDropTaskLock((Stz*)__func__);

  Api::vReapJobsOnce();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::HANDLERS::pEventHandlers()->vInitialise();
}

/*----------------------------------------------------------------------------*/
/* call system start job to define procedures & internal tables, spawn demons */
/*----------------------------------------------------------------------------*/
void Api::vCallStartSystemJob(void)
{
  _static_

  // this is the main thread
  PsxThd::vMarkNonSeThread();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  JobId tParentId;
  tParentId.vNextJobId();
  
  Job* pStartJob = new Job("startSystem"_Stz,tParentId);
  
  Stz* szJobNm = pStartJob->szJobNm();
  Task* pStartTask = new Task(szJobNm,pStartJob);
  pStartTask->vIsStart(true);

  pStartJob->vTask(pStartTask);
  
  pStartTask->vSetInxPtr("startSystem::startSystem"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  TagSet* pJobTags = pStartJob->pJobTags();
  //pJobTags->vDumpSet();

  // (no cmd to parse)

  pJobTags->vReplace(C_TAG_PATH,"/"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pStartJob->vResponseTimerStart();
  vCallTaskImmediate(pStartTask);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pStartTask->vState(TS_RETIRED);
  pStartTask->vDropTaskLock((Stz*)__func__);
  
  Api::vReapJobsOnce();
}

/*----------------------------------------------------------------------------*/
/* run task immediately                                                       */
/*----------------------------------------------------------------------------*/
void Api::vCallTaskImmediate(Task* pImmediateTask)
{
  _static_
  
  Task* pSaveTask = _pTask(PsxThd::pTlsGet(giTLS_TaskKey));

  pImmediateTask->vIsImmediate(true);
  
  pImmediateTask->vState(TS_EXECUTING);
  
  pImmediateTask->vTaskLoop();
  
  PsxThd::vTlsSet(giTLS_TaskKey,pSaveTask);
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
void Api::vSpawnTask_dedicatedOp(Op* pOp)
{
  _static_
  
  //RASSERT(! pJob->pTask());
  
  vCreateTaskForOp(pOp);
  Scheduler::vTaskApi_AddDedicated(pOp->pTask());
}


/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnTask_receivedJob(Job* pJob)
{
  _static_
  
  pJob->vTaskCreate();
  Scheduler::vTaskApi_AddReceived(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* schedule next phase of participant                                         */
/*----------------------------------------------------------------------------*/
void Api::vScheduleNextPhase(Job* pParticipantJob, Task* pActiviating2PcTask)
{
  _static_
  
  vResetTaskForJob(pParticipantJob);
  
  Task* pTaskParticipant = pParticipantJob->pTask();
  Scheduler::vTaskCross_ScheduleNextPhase(pTaskParticipant, pActiviating2PcTask);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* reset task for given job                                                   */
/*----------------------------------------------------------------------------*/
void Api::vResetTaskForJob(Job* pJob)
{
  _static_
    
  Stz* szEntry = "job::entryPoint"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Task* pTask = pJob->pTask();
    
  pTask->vSetInxPtr(szEntry);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pJob->vResponseTimerStart();
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
/* lock mutex and mark job as locked                                          */
/*----------------------------------------------------------------------------*/
void Api::vMutexLock(Job* pJob)
{
  _static_
      
  pJob->vMutexLock();
  pJob->vJobState(JS_LOCKED);
}

/*----------------------------------------------------------------------------*/
/* mark job as running and unlock mutex                                      */
/*----------------------------------------------------------------------------*/
void Api::vMutexUnlock(Job* pJob)
{
  _static_
  
  RASSERT(JS_ACTIVE != pJob->eJobState());
    
  pJob->vJobState(JS_ACTIVE);
  
  pJob->vMutexUnlock();
}

/*----------------------------------------------------------------------------*/
/* wait for mutex to be signalled                                             */
/*----------------------------------------------------------------------------*/
void Api::vMutexWaitCond(Job* pJob)
{
  _static_
  
  do {
        
    pJob->vMutexWait();
    
  } while (pJob->eJobState() != JS_SIGNALLED);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
