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
  vAddStep("api::spawnThread"_Stz,    &Step::vApiSpawnThread);
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
  
  if (AM::TRACE::bVerbose(C_VB_BOOT)) {
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

  //m_pJob->pLogicUnit_job()->m_xpOp->pOpTagSet()->vDumpSet();
  
  eEC eError;

  s48K sRequest;
  m_pDataStack->vPopStr(sRequest,__func__);

  i8 iReqNo = m_pDataStack->iPopInt(__func__);
  i8 iReqOf = m_pDataStack->iPopInt(__func__);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  
  eError = pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);

  TokenVec* pCmdTknVec = pSession->m_pInput->pTokenVec();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCmdTknVec->vTokenise(sRequest);
  
  TagSet* pTagSet =  & pSession->m_oJobTagSet;
  pTagSet->vReplace(C_TAG_PLPROTOCOL,PP_RETURN_PIPELINE);

  eError = Api::ePrepareJob(pSession);

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
  
  m_pJob->pLogicUnit_job()->m_xpOp->pOpTagSet()->vDumpSet();
  pJobTagSet->vCopy(m_pJob->pLogicUnit_job()->m_xpOp->pOpTagSet());
  
  if (AM::TRACE::bVerbose(C_VB_PROCEDURE))
    pJobTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
  pJobTagSet->vReplace(C_TAG_NOWAIT,1);
  bool bSpawned = Api::bSpawnJob_session(pJob);
  UNUSED(bSpawned);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //  Api::vDeleteJob(&pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(true);
  
//  Api::vFreeSession(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::executeRequest -- execute a request (internally)                      */
/*----------------------------------------------------------------------------*/
void Step::vApiExecuteRequest(void)
{
  ENTRY();

  eEC eError;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pop request and tag string
  
  s48K sRequest;
  m_pDataStack->vPopStr(sRequest,__func__);

  s48K sWithTagStr;
  m_pDataStack->vPopStr(sWithTagStr,__func__);

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("ExecuteRequest: %s\n"_Stz,sRequest.szStz());
    vTrace("WithTags: %s\n"_Stz,sWithTagStr.szStz());
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot a session
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  
  eError = pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);

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
  pJobTagSet->vInsert(C_TAG_REQUEST,pSession->m_iCmdNo);
  pJobTagSet->vInsert(C_TAG_SHM,gpBufferPool->szShmNm());
  pJobTagSet->vInsert(C_TAG_WHEN,xNow.sz());

  
  
  if (AM::TRACE::bVerbose(C_VB_API))
    pJobTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn job and wait for completion
  
//  bool bSpawned = Api::bSpawnJob_session(pJob);
//  UNUSED(bSpawned);
  Api::vExecuteJob(pSession);
  
  RASSERT(! pJob->bJobHasError() );

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

  if (AM::TRACE::bVerbose(C_VB_API)) {
    vTrace("SpawnDemon: %s\n"_Stz,sPipeline.szStz());
    vTrace("WithTags: %s\n"_Stz,sTagStr.szStz());
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  eEC eError = pSession->eInitaliseSession("rapidsse"_Stz,"public"_Stz);

  TokenVec* pCmdTknVec = pSession->m_pInput->pTokenVec();
  pCmdTknVec->vTokenise(sPipeline);
  
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
  if (AM::TRACE::bVerbose(C_VB_API))
    pJobTagSet->vDumpSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDedicated)
    Api::vSpawnJob_dedicated(pJob);
  else
    Api::vSpawnJob_service(pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Api::vFreeSession(pSession);

  m_pDataStack->vPushInt(true);
  
  delete pTagStrTknVec;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::spawnThread -- spawn a dedicated thread for op                        */
/*----------------------------------------------------------------------------*/
void Step::vApiSpawnThread(void)
{
  ENTRY();
  
  Op* pOp = _pOp(m_pDataStack->pPopPtr(__func__));

  Job* pOpJob = pOp->pOpJob();
  AM::vSpawnJob_dedicated(pOpJob);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
