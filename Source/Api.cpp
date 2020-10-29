/*============================================================================*/
/* Api.cpp -- Api functions                                                   */
/*============================================================================*/

#include "Interface.h"


extern BufferPool*  gpBufferPool;
extern Log* gpLog;


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* allocate a session                                                         */
/*----------------------------------------------------------------------------*/
Session* Api::pAllocSession(eSK eSession)
{
  _static_
  
  
  SessionArray* pSessionArray = AM::pSessionArray();
  
  Session* pSession = pSessionArray->pAllocSession(eSession);
    
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* free a session                                                             */
/*----------------------------------------------------------------------------*/
void Api::vFreeSession(Session* pSession)
{
  _static_
  
  SessionArray* pSessionArray = AM::pSessionArray();

  pSessionArray->vFreeSession(pSession);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete job  =                                                              */
/*----------------------------------------------------------------------------*/
void Api::vDeleteJob(Job** ppJob)
{
  _static_
  
  Job* pJob = * ppJob;
  * ppJob = nullptr;

  delete pJob;
}


/*----------------------------------------------------------------------------*/
/* execute job (spawn job and wait for completion)                            */
/*----------------------------------------------------------------------------*/
void Api::vExecuteJob(Session* pSession, Job* pJob)
{
  _static_
  
  Api::vMutexLock(pSession,pJob);
    
  // spawn the job
  if (pJob->bHasTag_noWait()) {
    Api::bSpawnJob_session(pJob);
    Api::vMutexUnlock(pSession,pJob);
    return;
  }
  else {
    Api::bSpawnJob_session(pJob);
  }
  
  Api::vMutexWaitCond(pSession,pJob);
  
  Api::vMutexUnlock(pSession,pJob);
}


/*----------------------------------------------------------------------------*/
/* prepare job                                                                     */
/*----------------------------------------------------------------------------*/
eEC Api::ePrepareJob(Job**     ppJob,
                     TokenVec* pCmdTknVec,
                     TokenVec* pLitTknVec,
                     TagSet*   pTagSet,
                     ReqTuple* axReqTuple)
{
  _static_

  *ppJob = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if ( ! AM::bParseCmd(pTagSet,pCmdTknVec) )
    return EC_INVALID_COMMAND;
      
      
  if (! Api::bPassesLowMem())
    return EC_LOW_MEMORY;
  
  Job* pJob;
  if (pLitTknVec)
    pJob = new Job(axReqTuple, pLitTknVec);
  else
    pJob = new Job(axReqTuple);
  
  // To GO : For DEMO
  // create tags in the job for the params from the literal token vector
  if (pLitTknVec && pLitTknVec->nItems()) {
    
    TagSet* pJobTags = pJob->pJobTags();
    
    for (i8 i_Tkn = 0; i_Tkn < pLitTknVec->nItems(); i_Tkn++) {
      
      Token* pTkn = pLitTknVec->pGet(i_Tkn);
      
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
        pJobTags->vInsert(szTag,pLitTknVec->pGet(i_Tkn)->szChars());
        break;
      case TN_INTEGER:
        pJobTags->vInsert(szTag,pLitTknVec->pGet(i_Tkn)->iToI8());
        break;
      default:
        RASSERT(0);
      }

    }
    
    //pJobTags->vContentDump();
  }
  
  pJob->ePopulateJob(pTagSet);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add <cmd> into job ename-set
  
  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  
  //vTrace("sCmd: %s\n %lld\n"_Stz,sCmd.szStz(),sCmd.nSize());

  pJob->pJobTags()->vInsert(C_TAG_CMD,sCmd);
  
  
  ASSERT(! pJob->pTask());
  
  if (AM::bVerbose())
    pJob->pJobTags()->vDumpSet();
  
  // this should go
  if (pJob->bJobHasError()) {
    eRS eResponse = (eRS)pJob->pJobTags()->iValue("<rspKind>"_Stz);
    pJob->vPrintResult(eResponse);
    Api::vDeleteJob(&pJob);
    return EC_ZZZ_OTHER;
  }
  

  if (pJob->bHasTag(C_TAG_LIFE)) {
    SP_TagSet spTagSet;
    pJob->vPrint("Job New: %s"_Stz,spTagSet.sz(pJob->pJobTags()));
    //pJob->pTagSet()->vPrint();
  }
  
  RASSERT(JS_LOCKED != pJob->eJobState());
        
  pJob->vSeed(SS_SEED_REQ);
        
  //! ALKA: Come back to this : 3 feb 2020
  //pJob->vJobClass(nullptr, (char*)__func__);

  *ppJob = pJob;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




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
  
//  Token* pTkn0 = pCmdTknVec->pGet(0);
//
//  Dic* pRootDic = AM::pRootDic();
//
//  Word* pOperatorCatg = pRootDic->pFindWord("\\operator"_Stz);
//
//  s32 sOpReq = pTkn0->szChars();
//  sOpReq.vConcat("_req"_Stz);
//
//  Word* pOp_maybe = pRootDic->pFindWord(sOpReq);
//
//  if (pOp_maybe && pOp_maybe->pCategory() == pOperatorCatg)
//    return true;
//  else
//    return false;
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
      "    start | periodicOp once | stageB "
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
      "    start | periodicOp once | /ON_ORIGIN/ | stageB hashed "
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
/* passes the low-memory condition?                                           */
/*----------------------------------------------------------------------------*/
bool Api::bPassesLowMem(void)
{
  _static_

  //! ALKA: while cleaning up JobClass all jobs had m_eFlags
  //! as JF_LOWMEM hence returning true
  return true;
  
//  if (aJobCLass->m_eFlags != JF_LOWMEM)
//    return true;
//  
//  if (! gpBufferPool->bLowMemory())
//    return true;
//  
//  if (aJobCLass->m_aCommand2)
//    vTrace("Error: unable to run %s %s under low memory conditions\n"_Stz,
//           aJobCLass->m_aCommand1,aJobCLass->m_aCommand2);
//  else
//    vTrace("Error: unable to run %s under low memory conditions\n"_Stz,
//           aJobCLass->m_aCommand1);

//  return false;
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
    if (i_Scan >= nTkns)
      break;
    //if (! pTkn_equals->bIsAssign())
    //  break;

    Token* pTkn_value = pTknVec->pGet(i_Scan++);   // value
    
    pTagSet->vReplace(pTkn_name->szChars(),pTkn_value->szChars());
  }

}

/*----------------------------------------------------------------------------*/
/* lock mutex and mark job as locked                                          */
/*----------------------------------------------------------------------------*/
void Api::vMutexLock(Session* pSession, Job* pJob)
{
  _static_
  
  pSession->m_pSsnMutex->vLock(pSession);
        
  pJob->vJobState(JS_LOCKED);
}

/*----------------------------------------------------------------------------*/
/* mark job as running and unlock session mutex                               */
/*----------------------------------------------------------------------------*/
void Api::vMutexUnlock(Session* pSession, Job* pJob)
{
  _static_
  
  RASSERT(JS_RUNNING != pJob->eJobState());
    
  pJob->vJobState(JS_RUNNING);
  
  pSession->m_pSsnMutex->vUnlock(pSession);
}

/*----------------------------------------------------------------------------*/
/* wait for session mutex to be signalled                                     */
/*----------------------------------------------------------------------------*/
void Api::vMutexWaitCond(Session* pSession, Job* pJob)
{
  _static_
  
  do {
    
    pSession->m_pSsnMutex->vWait(pSession);
    
  } while (pJob->eJobState() != JS_SIGNALLED);

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
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
    case AC_GetRecordBuffer:     pStats->Api.m_nGetRecordBuffer++;      break;
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
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Api::vMonitorPortrait(s48K& sPortrait)
{
  _static_
  
  
  s48K s;
  i8 nCats = 0;
  i8 nBreak = 80;
  i8 iPrev = 0;
  
  Statistics* pStats = AM::pStatistics();

  if (pStats->Api.m_nBeginFetch) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginFetch %lld "_Stz,pStats->Api.m_nBeginFetch);
  }
  if (pStats->Api.m_nBeginInsert) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginInsert %lld "_Stz,pStats->Api.m_nBeginInsert);
  }
  if (pStats->Api.m_nBeginSubInserter) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("beginSubInserter %lld "_Stz,pStats->Api.m_nBeginSubInserter);
  }
  if (pStats->Api.m_nCheckVersion) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("checkVersion %lld "_Stz,pStats->Api.m_nCheckVersion);
  }
  if (pStats->Api.m_nDeleteSession) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("deleteSession %lld "_Stz,pStats->Api.m_nDeleteSession);
  }
  if (pStats->Api.m_nEndInsert) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("endInsert %lld "_Stz,pStats->Api.m_nEndInsert);
  }
  if (pStats->Api.m_nEndJobs) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("endJobs %lld "_Stz,pStats->Api.m_nEndJobs);
  }
  if (pStats->Api.m_nEndSubInserter) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("endSubInserter %lld "_Stz,pStats->Api.m_nEndSubInserter);
  }
  if (pStats->Api.m_nExecuteCommand) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("executeCommand %lld "_Stz,pStats->Api.m_nExecuteCommand);
  }
  if (pStats->Api.m_nFetchNextBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("fetchNextBuffer %lld "_Stz,pStats->Api.m_nFetchNextBuffer);
  }
  if (pStats->Api.m_nFetchTable) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("fetchTable %lld "_Stz,pStats->Api.m_nFetchTable);
  }
  if (pStats->Api.m_nGetFirstBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getFirstBuffer %lld "_Stz,pStats->Api.m_nGetFirstBuffer);
  }
  if (pStats->Api.m_nGetNextBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getNextBuffer %lld "_Stz,pStats->Api.m_nGetNextBuffer);
  }
  if (pStats->Api.m_nGetLoadResult) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getLoadResult %lld "_Stz,pStats->Api.m_nGetLoadResult);
  }
  if (pStats->Api.m_nGetRecordBuffer) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRecordBuffer %lld "_Stz,pStats->Api.m_nGetRecordBuffer);
  }
  if (pStats->Api.m_nGetRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRow %lld "_Stz,pStats->Api.m_nGetRow);
  }
  if (pStats->Api.m_nGetRowCount) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getRowCount %lld "_Stz,pStats->Api.m_nGetRowCount);
  }
  if (pStats->Api.m_nGetTableColumnRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("getTableColumnRow %lld "_Stz,pStats->Api.m_nGetTableColumnRow);
  }
  if (pStats->Api.m_nGetTableColumnRowCount) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("GetTableColumnRowCount %lld "_Stz,pStats->Api.m_nGetTableColumnRowCount);
  }
  if (pStats->Api.m_nInsertRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("insertRow %lld "_Stz,pStats->Api.m_nInsertRow);
  }
  if (pStats->Api.m_nIsInvalidSession) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("isInvalidSession %lld "_Stz,pStats->Api.m_nIsInvalidSession);
  }
  if (pStats->Api.m_nNewSession) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("newSession %lld "_Stz,pStats->Api.m_nNewSession);
  }
  if (pStats->Api.m_nPoolOpen) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("poolOpen %lld "_Stz,pStats->Api.m_nPoolOpen);
  }
  if (pStats->Api.m_nNextRow) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("nextRow %lld "_Stz,pStats->Api.m_nNextRow);
  }
  if (pStats->Api.m_nTranslateError) {
    if (nCats++ && s.bBreak(nBreak,iPrev)) s.vConcat("\n     "_Stz);
    s.vSprintCat("translareError %lld "_Stz,pStats->Api.m_nTranslateError);
  }

  if (nCats) {
    sPortrait.vCopy("Api: "_Stz);
    sPortrait.vConcat(s);
    sPortrait.vConcat("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
bool Api::bSpawnJob_connector(Job* pJob)
{
  _static_
  
  

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
  
  Scheduler::vTaskApi_AddConnector(pJob->pTask());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJob_dedicated(Job* pJob)
{
  _static_
  
  RASSERT(! pJob->pTask());
  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddDedicated(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJob_op(Job* pJob)
{
  _static_
  
  
  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddOp(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJob_service(Job* pJob)
{
  _static_
  
  RASSERT(! pJob->pTask());

  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddService(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
bool Api::bSpawnJob_session(Job* pJob)
{
  _static_
  
  RASSERT(JS_LOCKED == pJob->eJobState());

  
  RASSERT(! pJob->pTask());
  
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
  
  Scheduler::vTaskApi_AddPrompt(pJob->pTask());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJob_received(Job* pJob)
{
  _static_
  
  
  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddReceived(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJob_system(Job* pJob)
{
  _static_
  
  
  vCreateTaskForJob(pJob);
  Scheduler::vTaskApi_AddSystem(pJob->pTask());
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vScheduleNextPhase(Job* pParticipantJob, Task* pActiviating2PcTask)
{
  _static_
  
  
  // THIS NOW DOESNT READ CORRECTLY
  vCreateTaskForJob(pParticipantJob);
  
  Task* pTaskParticipant = pParticipantJob->pTask();
  Scheduler::vTaskCross_ScheduleNextPhase(pTaskParticipant, pActiviating2PcTask);
}

/*----------------------------------------------------------------------------*/
/* spawn given job                                                            */
/*----------------------------------------------------------------------------*/
void Api::vCreateTaskForJob(Job* pJob)
{
  _static_
  
  ASSERT(SS_ENUMS != pJob->eSeed());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pTask = pJob->pTask();
  if ( pTask) {
    RASSERT(QK_NONE == pTask->eQueue());
    RASSERT(TR_NONE == pTask->eTranstionRequest());
  }
  else {
    pTask = new Task(pJob);
    RASSERT(QK_NONE == pTask->eQueue());
    RASSERT(TR_NONE == pTask->eTranstionRequest());
  }
  
  Job* pThisJob = AM::pThisTLSJob();
  
  if (pJob->bHasTag(C_TAG_TAGS) ||
      (pThisJob && pThisJob->bHasTag(C_TAG_TAGS)))
    pJob->pJobTags()->vDumpSet();

  if (! pJob->bHasTag(C_TAG_TAGS))
    Correctness::vDebugPoint();
  
  pJob->vTimerStart();
}

/*----------------------------------------------------------------------------*/
/* Spawn amd wait for job to complete                                         */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJobAndWait(Job* pJob)
{
  _static_
  
  
  // lock this job's session mutex
  Session* pSession = pJob->pSession();
  pSession->m_pSsnMutex->vLock(pJob);
  
  //  i8 rCode = pthread_mutex_lock(&pJob->m_xThreadMutex);
  //  if (0 != rCode)
  //    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  
  //! ApiState::vMarkApiLoc(pSession,106);
  
  // set the job state to be locked
  pJob->vJobState(JS_LOCKED);
  
  pJob->vSeed(SS_SEED_REQ);
    
  pJob->vTimerStart();
  

  // spawn the insertTable job
  IGNORE_RETURN(Api::bSpawnJob_session(pJob));
  //! ApiState::vMarkApiLoc(pSession,107);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for condition & state
  
  // wait till signalled
  do {
    
    RASSERT( _bIsValidJob(pJob->eClass()) );
    
    //    vTrace("ThreadCond:\n"_Stz);
    //    MemLib::vHexDump(&pJob->m_xThreadCond,  sizeof(pthread_cond_t));
    //
    //    vTrace("ThreadMutex:\n"_Stz);
    //    MemLib::vHexDump(&pJob->m_xThreadMutex, sizeof(pthread_mutex_t));
    
       pSession->m_pSsnMutex->vWait(pJob);
    //    rCode = pthread_cond_wait(&pJob->m_xThreadCond,
    //                              &pJob->m_xThreadMutex);
    //    if (0 != rCode)
    //      vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
    
    RASSERT( _bIsValidJob(pJob->eClass()) );
    
    
  } while (pJob->eJobState() != JS_SIGNALLED);
  
  RASSERT( _bIsValidJob(pJob->eClass()) );
  
  //! ApiState::vMarkApiLoc(pSession,108);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // set the job state back to running
  pJob->vJobState(JS_RUNNING);
  
  // unlock this job's session mutex
  pSession->m_pSsnMutex->vUnlock(pJob);
  
  //  rCode = pthread_mutex_unlock(&pJob->m_xThreadMutex);
  //  if (0 != rCode)
  //    vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
}

/*----------------------------------------------------------------------------*/
/* spawn dedicated job                                                        */
/*----------------------------------------------------------------------------*/
void Api::vSpawnJobDedicated(Job* pJob)
{
  _static_
  
  ASSERT(SS_ENUMS != pJob->eSeed());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pJob->pTask())
    AM::vLogWriteCsv(false);
  
  ASSERT(! pJob->pTask());
  Task* pNewTask = new Task(pJob);
  
  pJob->vTimerStart();
  
  Scheduler::vTaskApi_AddDedicated(pNewTask);
}

/*----------------------------------------------------------------------------*/
/* validate the SE session                                                    */
/*----------------------------------------------------------------------------*/
eEC Api::eValidateSession(i8 iTrace, Session* pSession, eAC eApiCall)
{
  _static_
  
  

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
  if (pSession->eClass() != CN_SESSION) {
    TRACE_API_ERROR("eValidateSession(c)", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* validate the SE session                                                    */
/*----------------------------------------------------------------------------*/
void Api::vWaitForAllJobsToComplete(void)
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

//! TO GO

/*----------------------------------------------------------------------------*/
/* reseed job                                                                 */
/*----------------------------------------------------------------------------*/
bool Api::bReseedJob(Session* pSession, Job* pJob)
{
  _static_
  
  ASSERT(SS_ENUMS != pJob->eSeed());
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = AM::iPid();
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSession)
      vWarn("Cannot find pid %lld\n"_Stz, iPid);
    vWarn("Cannot find pid %lld\n"_Stz, iPid);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! pJob->pTask())
    AM::vLogWriteCsv(false);
  
  ASSERT(pJob->pTask());
  Task* pTask = pJob->pTask();
  pTask->vReseed();
  
  AM::vLogSsn(SL_INF,pSession,AA_API_SPAWN_JOB,
              "pJob=%p"_Stz, //!format-LogPrm
              pJob,
              C_VARGS_END);
  
  
  //Scheduler::vAddTaskGeneral(pTask);
  
  return true;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
