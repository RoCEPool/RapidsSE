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
#include "OpSpec.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "LogicUnit.hpp"

#include "Procedure.hpp"

#include "MY.hpp"
#include "ePP.h"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Api(void)
{
  vAddStep("api::addProcedure"_Stz,   &Step::vApiAddProcedure);
  vAddStep("api::executeCommand"_Stz, &Step::vApiExecuteCommand);
  vAddStep("api::reapJobs"_Stz,       &Step::vApiReapJobs);
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
/* api::executeCommand -- execute a command (internally)                      */
/*----------------------------------------------------------------------------*/
void Step::vApiExecuteCommand(void)
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
    return;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // update the job tag set
  
  Job* pJob = pSession->m_pSsnJob;

  TagSet* pJobTagSet = pJob->pJobTags();
  
  if (m_pJob_task) {
    Stz* szTagPath = m_pJob_task->pJobTags()->szValue(C_TAG_PATH);
    pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  }
  
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
  
//  bool bSpawned = Api::bSpawnTask_session(pJob);
//  UNUSED(bSpawned);
  pJob->vTimerStart();
  
  Api::vExecuteJob(pSession);
  
  pJob->vTimerUpdate();

//  vTrace("Job Timer: %lldus\n"_Stz
//         ,pJob->m_oTimer.iMicroseconds());
  
  RASSERT(! pJob->bJobHasError() );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete job and free session
  Api::vDeleteJob(pSession);

  Api::vFreeSession(pSession);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //push result
  
  i8 iJobNo = (i8)pJob->tJobId().uJobNo();
  
  m_pDataStack->vPushInt(iJobNo);
  m_pDataStack->vPushInt(true);
  
  //delete pReqTknVec;
  delete pWithTagStrTknVec;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::reapJobs -- reap retired jobs                                         */
/*----------------------------------------------------------------------------*/
void Step::vApiReapJobs(void)
{
  ENTRY();
  
  i8 iForce = (i8)m_pDataStack->iPopInt(__func__);

  Api::vReapJobsOnce(iForce);

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
  
  Job* pSsnJob = pSession->m_pSsnJob;
  TagSet* pSsnJobTags = pSsnJob->pJobTags();
  
  Job* pPipelineJob = MY::pJob();
  TagSet* pPipelineJobTags = pPipelineJob->pJobTags();
  
  Stz* szTagPath = pPipelineJobTags->szValue(C_TAG_PATH);
  pSsnJobTags->vAppend(C_TAG_PATH,szTagPath);
  
  TokenVec* pTagStrTknVec = TokenVec::pTokenise(sTagStr);
  Api::vPopulateTagSet(pSsnJobTags,pTagStrTknVec);
  if (AM::TRACE::bVerbose(C_VB_API))
    pSsnJobTags->vDumpSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDedicated)
    Api::vSpawnTaskDedicated_job(pSsnJob);
  else
    Api::vSpawnTask_demonJob(pSsnJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Api::vFreeSession(pSession);

  m_pDataStack->vPushInt(true);
  
  delete pTagStrTknVec;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
