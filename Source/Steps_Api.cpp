/*============================================================================*/
/* Steps_Api.cpp - Api step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Api(void)
{
  vAddStep("api::executeRequest"_Stz, &Step::vApiExecuteRequest);
  vAddStep("api::spawnDemon"_Stz,     &Step::vApiSpawnDemon);
  vAddStep("api::startDone"_Stz,      &Step::vApiStartDone);
}

/*----------------------------------------------------------------------------*/
/* api::executeRequest -- extecute a request (internally)                     */
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

  vTrace("ExecuteRequest: %s\n"_Stz,sRequest.szStz());
  vTrace("WithTags: %s\nn"_Stz,sWithTagStr.szStz());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = Api::pAllocSession(SK_INTERNAL);
  pSession->vPartition(0);
  
  eError = pSession->eInit("rapidsse"_Stz,"public"_Stz);

  xReqTuple.m_i_Session = pSession->m_i_Session;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pDmnTknVec = TokenVec::pTokenise(sRequest);
  
  Job* pJob = nullptr;
  TagSet oTagSet;
  
  eError = Api::eJob_1_PreJob(&pJob,pDmnTknVec,&oTagSet,&xReqTuple);
  
  if (eError) {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TagSet* pJobTagSet = pJob->pJobTags();
  
  Stz* szTagPath = m_pTask->m_pJob->pJobTags()->szValue(C_TAG_PATH);
  pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  
  TokenVec* pWithTagStrTknVec = TokenVec::pTokenise(sWithTagStr);
  Api::vPopulateTagSet(pJobTagSet,pWithTagStrTknVec);
  
  if (AM::bVerbose())
    pJobTagSet->vDumpSet();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Api::vMutexLock(pSession,pJob);
  
  Api::vJob_2_SpawnJob_prompt(pJob);

  Api::vMutexWaitCond(pSession,pJob);
  
  Api::vMutexUnlock(pSession,pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Api::vJob_3_PostJob(pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Api::vJob_4_DeleteJob(&pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDataStack->vPushInt(true);
  
  Api::vFreeSession(pSession);
  
  delete pDmnTknVec;
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
    vTrace("WithTags: %s\nn"_Stz,sTagStr.szStz());
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TokenVec* pDmnTknVec = TokenVec::pTokenise(sPipeline);
  
  Job* pJob = nullptr;
  TagSet oTagSet;
  ReqTuple xReqTuple;
  
  eEC eError = Api::eJob_1_PreJob(&pJob,pDmnTknVec,&oTagSet,&xReqTuple);
  
  if (eError) {
    m_pDataStack->vPushInt(eError);
    m_pDataStack->vPushInt(false);
    EXIT();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  TagSet* pJobTagSet = pJob->pJobTags();
  
  Stz* szTagPath = m_pTask->m_pJob->pJobTags()->szValue(C_TAG_PATH);
  pJobTagSet->vAppend(C_TAG_PATH,szTagPath);
  
  TokenVec* pTagStrTknVec = TokenVec::pTokenise(sTagStr);
  Api::vPopulateTagSet(pJobTagSet,pTagStrTknVec);
  if (AM::bVerbose())
    pJobTagSet->vDumpSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDedicated)
    Api::vJob_2_SpawnJob_dedicated(pJob);
  else
    Api::vJob_2_SpawnJob_service(pJob);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(true);
  
  delete pDmnTknVec;
  delete pTagStrTknVec;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* api::start done -- mark start as done                                      */
/*----------------------------------------------------------------------------*/
void Step::vApiStartDone(void)
{
  ENTRY();
  
  Api::vStartDone(true);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
