/*============================================================================*/
/* Steps_Node.cpp - node step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern bool            gbMetadata;

// node: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Node(void)
{
  vAddPrimitive("node.init"_Stz,       &vNodeInit,         false);
  vAddPrimitive("node.IP"_Stz,         &vNodeIP,           false);
  vAddPrimitive("node.name"_Stz,       &vNodeName,         false);
  vAddPrimitive("node::new"_Stz,       &vNodeNew,          false);
  vAddPrimitive("node.no"_Stz,         &vNodeNo,           false);
  vAddPrimitive("node.partitions"_Stz, &vNodePartitions,   false);
  vAddPrimitive("node.port"_Stz,       &vNodePort,         false);
  vAddPrimitive("node.size"_Stz,       &vNodeSize,         false);
  vAddPrimitive("node.status"_Stz,     &vNodeStatus,       false);
}

/*----------------------------------------------------------------------------*/
/* node.init -- initialise a node (of a grid)                                 */
/*----------------------------------------------------------------------------*/
void Task::vNodeInit(MP_Task mpTask)
{
ENTRY("vNodeInit"_Stz);
  
  Object* pPtrNode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.init"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  new(pNode) Node;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.IP -- push IP of node                                                 */
/*----------------------------------------------------------------------------*/
void Task::vNodeIP(MP_Task mpTask)
{
  ENTRY("vNodeIP"_Stz);
  
  Object* pPtrNode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.IP"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sIP;
  sIP = pNode->aNodeIP();
  mpTask->m_pDataStack->vPushS256(sIP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node::new -- new a node (of a grid)                                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeNew(MP_Task mpTask)
{
ENTRY("vNodeNew"_Stz);
  
  i8 iNodeNo;
  
  if ( ! mpTask->m_pDataStack->bPop(iNodeNo) )
    THROW_INVALID_DSTACK("node::new"_Stz);
  
  Node* pNode = new Node(iNodeNo,
                         gxArguments.m_xNodes[iNodeNo].m_sNodeName,
                         gxArguments.m_xNodes[iNodeNo].m_sIPAddress,
                         gxArguments.m_xNodes[iNodeNo].m_iPortNo);
  ASSERT(CN_NODE == pNode->eClass());

  mpTask->m_pDataStack->vPushObject(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.name -- push name of node                                             */
/*----------------------------------------------------------------------------*/
void Task::vNodeName(MP_Task mpTask)
{
  ENTRY("vNodeName"_Stz);
  
  Object* pPtrNode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.name"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sName;
  sName = pNode->aNodeName();
  mpTask->m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Task::vNodeNo(MP_Task mpTask)
{
ENTRY("vNodeNo"_Stz);
  
  Object* pPtrNode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.no"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  mpTask->m_pDataStack->vPushInt(pNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.partitions -- get no of partitions of this node (of a grid)           */
/*----------------------------------------------------------------------------*/
void Task::vNodePartitions(MP_Task mpTask)
{
  ENTRY("vNodePartitions"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpGrid->m_nSlots);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.port -- push port number of node (of a grid)                          */
/*----------------------------------------------------------------------------*/
void Task::vNodePort(MP_Task mpTask)
{
  ENTRY("vNodeNo"_Stz);
  
  Object* pPtrNode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.port"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  mpTask->m_pDataStack->vPushInt(pNode->iNodePortNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.size -- get size of this node (of a grid)                             */
/*----------------------------------------------------------------------------*/
void Task::vNodeSize(MP_Task mpTask)
{
  ENTRY("vNodeSize"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_iRegionSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.status -- get status of this node (of a grid)                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeStatus(MP_Task mpTask)
{
  ENTRY("vNodeStatus"_Stz);

  s256 s;
  
  if (gbMetadata) {
    if (Region::bTest(gxArguments.m_sRegionName))
      s.vSprint("row: ('%s', '%s', 'running')"_Stz,
              gpGrid->pThisNode()->aNodeName(),
              gxArguments.m_sRegionName.aChar());
    else
      s.vSprint("row: ('%s', '%s', 'not running')"_Stz,
              gpGrid->pThisNode()->aNodeName(),
              gxArguments.m_sRegionName.aChar());
  }
  else {
    if (Region::bTest(gxArguments.m_sRegionName))
      s.vSprint("RapidsSE instance %s running"_Stz,
                gxArguments.m_sRegionName.aChar());
    else
      s.vSprint("RapidsSE instance %s not running"_Stz,
                gxArguments.m_sRegionName.aChar());
  }
  
  mpTask->m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
