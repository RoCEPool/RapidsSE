/*============================================================================*/
/* Steps_Api.cpp - Api step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Api(void)
{
  vAddStep("api::addProcedure"_Stz,   &Step::vApiAddProcedure);
  vAddStep("api::executeNested"_Stz,  &Step::vApiExecuteNested);
  vAddStep("api::executeRequest"_Stz, &Step::vApiExecuteRequest);
  vAddStep("api::spawnDemon"_Stz,     &Step::vApiSpawnDemon);
}

/*----------------------------------------------------------------------------*/
/* api::addProcedure -- add a procedure                                       */
/*----------------------------------------------------------------------------*/
void Step::vApiAddProcedure(void)
{
  ENTRY();
  
  s48K sRequest;
  m_pDataStack->vPopStr(sRequest,__func__);
  
  s48K sWithTagStr;
  m_pDataStack->vPopStr(sWithTagStr,__func__);
  
  if (AM::bVerbose()) {
    vTrace("AddProcedure: %s\n"_Stz,sRequest.szStz());
    vTrace("WithTags: %s\n"_Stz,sWithTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TokenVec* pDmnTknVec = TokenVec::pTokenise(sRequest);
  
  pDmnTknVec->vIntegrityCheck();
  
  Token* pTkn0 = pDmnTknVec->pGet(0);
  ASSERT(pTkn0->bSame("procedure"_Stz) || pTkn0->bSame("procedureLocal"_Stz));

  //Stz* ssProcNm = pDmnTknVec->pGet(1)->szChars();
  
  Procedure::vAddProcedure(pDmnTknVec);
}

/*----------------------------------------------------------------------------*/
/* api::executeNested -- execute a nested request (internally)                */
/*----------------------------------------------------------------------------*/
void Step::vApiExecuteNested(void)
{
  ENTRY();

  m_pLogicUnit->m_xpOp->pOpTagSet()->vDumpSet();
  
  eEC eError;
  ReqTuple xReqTuple;

  s48K sRequest;
  m_pDataStack->vPopStr(sRequest,__func__);

  i8 iReqNo = m_pDataStack->iPopInt(__func__);
  i8 iReqOf = m_pDataStack->iPopInt(__func__);

//  s48K sWithTagStr;
//  m_pDataStack->vPopStr(sWithTagStr,__func__);
//
//  if (AM::bVerbose()) {
//    vTrace("ExecuteRequest: %s\n"_Stz,sRequest.szStz());
//    vTrace("WithTags: %s\n"_Stz,sWithTagStr.szStz());
//  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  
  eError = pSession->eInit("rapidsse"_Stz,"public"_Stz);

  xReqTuple.m_i_Session = pSession->m_i_Session;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pReqTknVec = TokenVec::pTokenise(sRequest);
  
  TagSet oTagSet;
  oTagSet.vReplace(C_TAG_PLPROTOCOL,PP_RETURN_PIPELINE);

  eError = Api::ePrepareJob(pSession,pReqTknVec,nullptr,&oTagSet);
  
  if (eError) {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Job* pJob = pSession->m_pSsnJob;

  TagSet* pJobTagSet = pJob->pJobTags();
  
  Stz* szTagPath = m_pTask->m_pJob->pJobTags()->szValue(C_TAG_PATH);
  pJobTagSet->vAppend(C_TAG_PATH,szTagPath);

//  TokenVec* pWithTagStrTknVec = TokenVec::pTokenise(sWithTagStr);
//  Api::vPopulateTagSet(pJobTagSet,pWithTagStrTknVec);
  
  m_pLogicUnit->m_xpOp->pOpTagSet()->vDumpSet();
  pJobTagSet->vCopy(m_pLogicUnit->m_xpOp->pOpTagSet());
  
//  if (AM::bVerbose())
    pJobTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
  bool bSpawned = Api::bSpawnJob_session(pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  eRS eResponse = (eRS)pJob->pJobTags()->iValue("<rspKind>"_Stz);
//  pJob->vPrintResult(eResponse);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  Api::vDeleteJob(&pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(true);
  
//  Api::vFreeSession(pSession);
  
  delete pReqTknVec;
  //delete pWithTagStrTknVec;
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::executeRequest -- execute a request (internally)                      */
/*----------------------------------------------------------------------------*/
void Step::vApiExecuteRequest(void)
{
  ENTRY();

  eEC eError;
  ReqTuple xReqTuple;

  s48K sRequest;
  m_pDataStack->vPopStr(sRequest,__func__);

  s48K sWithTagStr;
  m_pDataStack->vPopStr(sWithTagStr,__func__);

  if (AM::bVerbose()) {
    vTrace("ExecuteRequest: %s\n"_Stz,sRequest.szStz());
    vTrace("WithTags: %s\n"_Stz,sWithTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  
  eError = pSession->eInit("rapidsse"_Stz,"public"_Stz);

  xReqTuple.m_i_Session = pSession->m_i_Session;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pReqTknVec = TokenVec::pTokenise(sRequest);
  
  TagSet oTagSet;
  oTagSet.vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);

  eError = Api::ePrepareJob(pSession,pReqTknVec,nullptr,&oTagSet);
  
  if (eError) {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Job* pJob = pSession->m_pSsnJob;

  TagSet* pJobTagSet = pJob->pJobTags();
  
  Stz* szTagPath = m_pTask->m_pJob->pJobTags()->szValue(C_TAG_PATH);
  pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  
  TokenVec* pWithTagStrTknVec = TokenVec::pTokenise(sWithTagStr);
  Api::vPopulateTagSet(pJobTagSet,pWithTagStrTknVec);
  
  if (AM::bVerbose())
    pJobTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
  bool bSpawned = Api::bSpawnJob_session(pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eRS eResponse = (eRS)pJob->pJobTags()->iValue("<rspKind>"_Stz);
  pJob->vPrintResult(eResponse);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Api::vDeleteJob(&pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(true);
  
  Api::vFreeSession(pSession);
  
  delete pReqTknVec;
  delete pWithTagStrTknVec;
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::spawnDemon -- spawn a demon                                           */
/*----------------------------------------------------------------------------*/
void Step::vApiSpawnDemon(void)
{
  ENTRY();
  
  s48K sPipeline;
  m_pDataStack->vPopStr(sPipeline,__func__);
 
  bool bDedicated = m_pDataStack->iPopInt(__func__);
  
  s48K sTagStr;
  m_pDataStack->vPopStr(sTagStr,__func__);

  if (AM::bVerbose()) {
    vTrace("SpawnDemon: %s\n"_Stz,sPipeline.szStz());
    vTrace("WithTags: %s\n"_Stz,sTagStr.szStz());
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pDmnTknVec = TokenVec::pTokenise(sPipeline);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  TagSet oTagSet;
  oTagSet.vReplace(C_TAG_PLPROTOCOL,PP_NONE);

  Session* pSession = Api::pAllocSession(SK_INTERNAL);

  eEC eError = Api::ePrepareJob(pSession,pDmnTknVec,nullptr,&oTagSet);
  

  if (eError) {
    Api::vFreeSession(pSession);
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pJob = pSession->m_pSsnJob;

  TagSet* pJobTagSet = pJob->pJobTags();
  
  Stz* szTagPath = m_pTask->m_pJob->pJobTags()->szValue(C_TAG_PATH);
  pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  
  TokenVec* pTagStrTknVec = TokenVec::pTokenise(sTagStr);
  Api::vPopulateTagSet(pJobTagSet,pTagStrTknVec);
  if (AM::bVerbose())
    pJobTagSet->vDumpSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDedicated)
    Api::vSpawnJob_dedicated(pJob);
  else
    Api::vSpawnJob_service(pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Api::vFreeSession(pSession);

  m_pDataStack->vPushInt(true);
  
  delete pDmnTknVec;
  delete pTagStrTknVec;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
