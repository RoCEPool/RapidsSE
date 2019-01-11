/*============================================================================*/
/* Steps_JobNodeInfo.cpp - job_nodeInfo step group                            */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobNodeInfo(void)
{
  vAddStep("job_nodeInfo.nodeName@"_Stz,     &Step::vJob_NodeInfoNodeNameFetch);
}

/*----------------------------------------------------------------------------*/
/* job_nodeInfo.nodeName@ -- fetch node name                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_NodeInfoNodeNameFetch(MP_Task mpTask)
{
  ENTRY("vJob_NodeInfoNodeNameFetch"_Stz);
  
  Object* pPtrJob_NodeInfo;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_NodeInfo) )
    THROW_INVALID_DSTACK("job_nodeInfo.nodeName@"_Stz);
  Job_NodeInfo* pJob_NodeInfo = _pJob_NodeInfo(pPtrJob_NodeInfo);
  
  mpTask->m_pDataStack->vPushS32(pJob_NodeInfo->m_sNodeName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
