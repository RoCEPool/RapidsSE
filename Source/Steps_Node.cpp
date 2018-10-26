/*============================================================================*/
/* Steps_Node.cpp - node step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;
extern AttachMap*      gpAttachMap;

// node: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Node(void)
{
  vAddStep("node::new"_Stz,        &Task::vNodeNew);
  
  vAddStep("node.init"_Stz,        &Task::vNodeInit);
  vAddStep("node.IP"_Stz,          &Task::vNodeIP);
  vAddStep("node.name"_Stz,        &Task::vNodeName);
  vAddStep("node.no"_Stz,          &Task::vNodeNo);
  vAddStep("node.partitions"_Stz,  &Task::vNodePartitions);
  vAddStep("node.port"_Stz,        &Task::vNodePort);
  vAddStep("node.recvSocket!"_Stz, &Task::vNodeRecvSocketStore);
  vAddStep("node.recvSocket@"_Stz, &Task::vNodeRecvSocketFetch);
  vAddStep("node.sendSocket!"_Stz, &Task::vNodeSendSocketStore);
  vAddStep("node.sendSocket@"_Stz, &Task::vNodeSendSocketFetch);
  vAddStep("node.size"_Stz,        &Task::vNodeSize);
  vAddStep("node.status"_Stz,      &Task::vNodeStatus);
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
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->m_nSlots);
  
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
  
  mpTask->m_pDataStack->vPushInt(pNode->iNodePortNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.recvSocket! -- set receive socket for node                            */
/*----------------------------------------------------------------------------*/
void Task::vNodeRecvSocketStore(MP_Task mpTask)
{
  ENTRY("vNodeRecvSocketStore"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.recvSocket!"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("node.recvSocket!"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  pNode->vRecvSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.recvSocket@ -- get receive socket for node                            */
/*----------------------------------------------------------------------------*/
void Task::vNodeRecvSocketFetch(MP_Task mpTask)
{
  ENTRY("vNodeRecvSocketFetch"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.recvSocket@"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  Socket* pSocket = pNode->pRecvSocket();
  
  mpTask->m_pDataStack->vPushObject(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket! -- set send socket for node                               */
/*----------------------------------------------------------------------------*/
void Task::vNodeSendSocketStore(MP_Task mpTask)
{
  ENTRY("vNodeSendSocketStore"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.sendSocket!"_Stz);
  Node* pNode = _pNode(pPtrNode);

  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("node.sendSocket!"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
    
  pNode->vSendSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket@ -- get send socket for node                               */
/*----------------------------------------------------------------------------*/
void Task::vNodeSendSocketFetch(MP_Task mpTask)
{
  ENTRY("vNodeSendSocketFetch"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.sendSocket"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  Socket* pSocket = pNode->pSendSocket();
  
  mpTask->m_pDataStack->vPushObject(pSocket);
  
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
  
  Grid* pGrid = gpAttachMap->m_pGrid;
  
  if (Region::bTest(gxArguments.m_sRegionName))
    s.vSprint("row: ('%s', '%s', 'running')"_Stz,
            pGrid->pThisNode()->aNodeName(),
            gxArguments.m_sRegionName.aChar());
  else
    s.vSprint("row: ('%s', '%s', 'not running')"_Stz,
            pGrid->pThisNode()->aNodeName(),
            gxArguments.m_sRegionName.aChar());
  
  mpTask->m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
