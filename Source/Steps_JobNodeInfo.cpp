/*============================================================================*/
/* Steps_JobNodeInfo.cpp - job_nodeInfo step group                            */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_NodeInfoNodeNameFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_NodeInfoNodeNameFetch"_Stz);
  
  Object* pPtrJob_NodeInfo;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_NodeInfo) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_NodeInfo* pJob_NodeInfo = _pJob_NodeInfo(pPtrJob_NodeInfo);
  
  mpTask->m_pForth->m_pDataStack->vPushS32(pJob_NodeInfo->m_sNodeName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
