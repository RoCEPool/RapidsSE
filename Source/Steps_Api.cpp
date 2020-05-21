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
}

/*----------------------------------------------------------------------------*/
/* api::spawnDemon -- spawn a demon                                           */
/*----------------------------------------------------------------------------*/
void Step::vApiExecuteRequest(void)
{
  ENTRY();

   s48K sRequest;
   m_pDataStack->vPopStr(sRequest,__func__);
  
   s48K sTagStr;
   m_pDataStack->vPopStr(sTagStr,__func__);

   vTrace("ExecuteRequest: %s\n"_Stz,sRequest.szStz());
   vTrace("WithTags: %s\nn"_Stz,sTagStr.szStz());

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

  vTrace("SpawnDemon: %s\n"_Stz,sPipeline.szStz());
  vTrace("WithTags: %s\nn"_Stz,sTagStr.szStz());

  
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
  pJobTagSet->vDumpSet();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bDedicated)
    Api::vJob_2_SpawnJob_dedicated(pJob);
  else
    Api::vJob_2_SpawnJob_service(pJob);

  m_pDataStack->vPushInt(true);
  
  delete pDmnTknVec;
  delete pTagStrTknVec;
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
