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
void Step::vJob_NodeInfoNodeNameFetch(void)
{
  ENTRY(); // ENTRY("vJob_NodeInfoNodeNameFetch"_Stz);
  
  Job_NodeInfo* pJob_NodeInfo = _pJob_NodeInfo(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushStr(pJob_NodeInfo->m_sNodeName);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
