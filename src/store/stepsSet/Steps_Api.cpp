/*============================================================================*/
/* Steps_Api.cpp - Api step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Api.hpp"
#include "Session.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "Procedure.hpp"

#include "ePP.h"
#include "TagConstants.h"

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
  UNUSED(pTkn0);

  //Stz* ssProcNm = pDmnTknVec->pGet(1)->szChars();
  
  Procedure::vAddProcedure(pDmnTknVec);
}

/*----------------------------------------------------------------------------*/
/* api::executeNested -- execute a nested request (internally)                */
/*----------------------------------------------------------------------------*/
void Step::vApiExecuteNested(void)
{
  ENTRY();

  //( m_pJob->pLogicUnit_job() )->m_xpOp->pOpTagSet()->vDumpSet();
  
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
  
  eError = pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);

  TokenVec* pCmdTknVec = pSession->m_pInput->pTokenVec();

  xReqTuple.m_i_Session = pSession->m_i_Session;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCmdTknVec->vTokenise(sRequest);
  
//  TagSet oTagSet;
//  oTagSet.vReplace(C_TAG_PLPROTOCOL,PP_RETURN_PIPELINE);
  TagSet* pTagSet =  & pSession->m_oJobTagSet;
  pTagSet->vReplace(C_TAG_PLPROTOCOL,PP_RETURN_PIPELINE);

  //eError = Api::ePrepareJob(pSession,pReqTknVec,nullptr,pTagSet);
  eError = Api::ePrepareJob(pSession);//,nullptr);

  if (eError) {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Job* pJob = pSession->m_pSsnJob;

  pJob->m_pOuterOp = m_pJob->m_pOp;
  
  TagSet* pJobTagSet = pJob->pJobTags();
  
  Stz* szTagPath = m_pJob->pJobTags()->szValue(C_TAG_PATH);
  pJobTagSet->vAppend(C_TAG_PATH,szTagPath);

  pJobTagSet->vReplace(C_TAG_REQNO,iReqNo);
  pJobTagSet->vReplace(C_TAG_REQOF,iReqOf);

//  TokenVec* pWithTagStrTknVec = TokenVec::pTokenise(sWithTagStr);
//  Api::vPopulateTagSet(pJobTagSet,pWithTagStrTknVec);
  
  ( m_pJob->pLogicUnit_job() )->m_xpOp->pOpTagSet()->vDumpSet();
  pJobTagSet->vCopy(( m_pJob->pLogicUnit_job() )->m_xpOp->pOpTagSet());
  
  if (AM::bVerbose())
    pJobTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
  pJobTagSet->vReplace(C_TAG_NOWAIT,1);
  bool bSpawned = Api::bSpawnJob_session(pJob);
  UNUSED(bSpawned);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  eRS eResponse = (eRS)pJob->pJobTags()->iValue("<rspKind>"_Stz);
//  pJob->vPrintResult(eResponse);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  Api::vDeleteJob(&pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(true);
  
//  Api::vFreeSession(pSession);
  
  //delete pReqTknVec;
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pop request and tag string
  
  s48K sRequest;
  m_pDataStack->vPopStr(sRequest,__func__);

  s48K sWithTagStr;
  m_pDataStack->vPopStr(sWithTagStr,__func__);

  if (AM::bVerbose()) {
    vTrace("ExecuteRequest: %s\n"_Stz,sRequest.szStz());
    vTrace("WithTags: %s\n"_Stz,sWithTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot a session
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  
  eError = pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);

  xReqTuple.m_i_Session = pSession->m_i_Session;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenise the request
  
  //TokenVec* pReqTknVec = TokenVec::pTokenise(sRequest);
  TokenVec* pCmdTknVec = pSession->m_pInput->pTokenVec();
  pCmdTknVec->vTokenise(sRequest);
//  TagSet oTagSet;
//  oTagSet.vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);

  TagSet* pTagSet =  & pSession->m_oJobTagSet;
  pTagSet->vReplace(C_TAG_PLPROTOCOL,PP_SIGNAL_SESSION);

  eError = Api::ePrepareJob(pSession);
  
  if (eError) {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // update the job tag set
  
  Job* pJob = pSession->m_pSsnJob;

  TagSet* pJobTagSet = pJob->pJobTags();
  
  Stz* szTagPath = m_pJob->pJobTags()->szValue(C_TAG_PATH);
  pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  
  TokenVec* pWithTagStrTknVec = TokenVec::pTokenise(sWithTagStr);
  Api::vPopulateTagSet(pJobTagSet,pWithTagStrTknVec);
  
  
  Now xNow;
  xNow.vSetNow();

  s32 sHostName;
  Hardware::vHostName(sHostName);

  pJobTagSet->vInsert(C_TAG_FREEPCT,gpBufferPool->iFreePercent());
  pJobTagSet->vInsert(C_TAG_GRID,AM::pGrid()->szGridNm());
  pJobTagSet->vInsert(C_TAG_HOST,sHostName);
  pJobTagSet->vInsert(C_TAG_NODE,AM::pGrid()->pThisNode()->szNodeNm());
  pJobTagSet->vInsert(C_TAG_REQUEST,pSession->m_xReqTuple.m_iReqNo);
  pJobTagSet->vInsert(C_TAG_SHM,gpBufferPool->szShmNm());
  pJobTagSet->vInsert(C_TAG_WHEN,xNow.sz());

  
  
  if (AM::bVerbose())
    pJobTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn job and wait for completion
  
//  bool bSpawned = Api::bSpawnJob_session(pJob);
//  UNUSED(bSpawned);
  Api::vExecuteJob(pSession);
  
  RASSERT(! pJob->bJobHasError() );
  //eRS eResponse = (eRS)pJob->pJobTags()->iValue("<rspKind>"_Stz);
  //pJob->vPrintResult(pSession,eResponse);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete job and free session
  Api::vDeleteJob(pSession);

  Api::vFreeSession(pSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //push result
  
  m_pDataStack->vPushInt(true);
  
  //delete pReqTknVec;
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

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  eEC eError = pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);

  //TokenVec* pDmnTknVec = TokenVec::pTokenise(sPipeline);
  TokenVec* pCmdTknVec = pSession->m_pInput->pTokenVec();
  pCmdTknVec->vTokenise(sPipeline);

//  TagSet oTagSet;
//  oTagSet.vReplace(C_TAG_PLPROTOCOL,PP_NONE);
//  oTagSet.vDumpSet();
//  eError = Api::ePrepareJob(pSession,pDmnTknVec,nullptr,&oTagSet);
  
  TagSet* pTagSet =  & pSession->m_oJobTagSet;
  pTagSet->vReplace(C_TAG_PLPROTOCOL,PP_NONE);
  //pTagSet->vDumpSet();
  eError = Api::ePrepareJob(pSession);


  if (eError) {
    Api::vFreeSession(pSession);
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pJob = pSession->m_pSsnJob;

  TagSet* pJobTagSet = pJob->pJobTags();
  
  Stz* szTagPath = m_pJob->pJobTags()->szValue(C_TAG_PATH);
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
  
  //delete pDmnTknVec;
  delete pTagStrTknVec;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
