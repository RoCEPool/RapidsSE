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
void Api::vExecuteJob(Job* pJob)
{
  _static_
  
  Session* pSession = pJob->pSession();
  Api::bSpawnJob_session(pJob);
}

/*----------------------------------------------------------------------------*/
/* prepare job                                                                */
/*----------------------------------------------------------------------------*/
eEC Api::ePrepareJob(Session*  pSession,
                     TokenVec* pCmdTknVec,
                     TokenVec* pLitTknVec,
                     TagSet*   pExtraTagSet)
{
  _static_

  RASSERT(pSession);
  RASSERT(! pSession->m_pSsnJob);
    
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
  
  if ( ! AM::bParseCmd(pCmdTknVec,pExtraTagSet) )
    return EC_INVALID_COMMAND;
      
      
  if (! AM::bPassesLowMem())
    return EC_LOW_MEMORY;
  
  if (pLitTknVec)
    pSession->m_pSsnJob = new Job(&pSession->m_xReqTuple, pLitTknVec);
  else
    pSession->m_pSsnJob = new Job(&pSession->m_xReqTuple);
  
  pSession->m_xReqTuple.m_iReqNo++;

  // To GO : For DEMO
  // create tags in the job for the params from the literal token vector
  if (pLitTknVec && pLitTknVec->nItems()) {
    
    
    for (i8 i_Tkn = 0; i_Tkn < pLitTknVec->nItems(); i_Tkn++) {
      
      Token* pTkn = pLitTknVec->pGet(i_Tkn);
      
      TagSet* pJobTags = pSession->m_pSsnJob->pJobTags();

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
  
  pSession->m_pSsnJob->ePopulateJob(pExtraTagSet);

  pSession->m_pSsnJob->pJobTags()->vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add <cmd> into job ename-set
  
  sSTMT sCmd;
  pCmdTknVec->vVecToString(sCmd,0,true);
  
  //vTrace("sCmd: %s\n %lld\n"_Stz,sCmd.szStz(),sCmd.nSize());

  pSession->m_pSsnJob->pJobTags()->vInsert(C_TAG_CMD,sCmd);
  
  
  ASSERT(! pSession->m_pSsnJob->pTask());
  
  if (AM::bVerbose())
    pSession->m_pSsnJob->pJobTags()->vDumpSet();
  
  // this should go
  if (pSession->m_pSsnJob->bJobHasError()) {
    eRS eResponse = (eRS)pSession->m_pSsnJob->pJobTags()->iValue("<rspKind>"_Stz);
    pSession->m_pSsnJob->vPrintResult(eResponse);
    Api::vDeleteJob(&pSession->m_pSsnJob);
    return EC_ZZZ_OTHER;
  }
  

  if (pSession->m_pSsnJob->bHasTag(C_TAG_LIFE)) {
    SP_TagSet spTagSet;
    pSession->m_pSsnJob->vPrint("Job New: %s"_Stz,spTagSet.sz(pSession->m_pSsnJob->pJobTags()));
    //pJob->pTagSet()->vPrint();
  }
  
  RASSERT(JS_LOCKED != pSession->m_pSsnJob->eJobState());
        
  pSession->m_pSsnJob->vSeed(SS_SEED_REQ);
        
  //! ALKA: Come back to this : 3 feb 2020
  //pJob->vJobClass(nullptr, (char*)__func__);
  
  
  TagSet* pSsnTagSet = &pSession->m_oSsnTagSet;
   pSession->m_pSsnJob->pJobTags()->vCopy(pSsnTagSet);
  
  if ( pSession->m_pSsnJob->bHasTag(C_TAG_TAGS))
     pSession->m_pSsnJob->pJobTags()->vDumpSet();

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
  
  pSession->m_oSsnMutex.vLock(pSession);
        
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
  
  pSession->m_oSsnMutex.vUnlock(pSession);
}

/*----------------------------------------------------------------------------*/
/* wait for session mutex to be signalled                                     */
/*----------------------------------------------------------------------------*/
void Api::vMutexWaitCond(Session* pSession, Job* pJob)
{
  _static_
  
  do {
    
    pSession->m_oSsnMutex.vWait(pSession);
    
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
  
  Session* pSession = pJob->pSession();
  Api::vMutexLock(pSession,pJob);

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
  
  pJob->vTimerStart();
  Scheduler::vTaskApi_AddPrompt(pJob->pTask());

  if (! pJob->bHasTag_noWait()) {
    pSession->m_oSsnMutex.vObserve(true);
    Api::vMutexWaitCond(pSession,pJob);
    pJob->vTimerUpdate();
  }
  
  Api::vMutexUnlock(pSession,pJob);

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

//! TO GO

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
