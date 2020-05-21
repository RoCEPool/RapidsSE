/*============================================================================*/
/* Steps_Api.cpp - Api step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Api(void)
{
  vAddStep("api::spawnDemon"_Stz, &Step::vApiSpawnDemon);
}

/*----------------------------------------------------------------------------*/
/* api::spawnDemon -- spawn a demon                                           */
/*----------------------------------------------------------------------------*/
void Step::vApiSpawnDemon(void)
{
  ENTRY();
  
  s48K sPipeline;
  m_pDataStack->vPopStr(sPipeline,__func__);
  
  vTrace("SpawnDemon: %s\n"_Stz,sPipeline.szStz());
  
  
  TokenVec* pDmnTknVec = TokenVec::pTokenise(sPipeline);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
  
  Stz* szTagPath = m_pTask->m_pJob->pJobTags()->szValue(C_TAG_PATH);
  pJob->pJobTags()->vAppend(C_TAG_PATH,szTagPath);
  
  Api::vJob_2_SpawnJob(pJob,false);

  m_pDataStack->vPushInt(true);

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
