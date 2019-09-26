/*============================================================================*/
/* Steps_Node.cpp - node step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Node(void)
{
  vAddStep("node::new"_Stz,        &Step::vNodeNew);
  
  vAddStep("node.forget"_Stz,      &Step::vNodeForget);
  vAddStep("node.IP"_Stz,          &Step::vNodeIP);
  vAddStep("node.name"_Stz,        &Step::vNodeName);
  vAddStep("node.no"_Stz,          &Step::vNodeNo);
  vAddStep("node.partitions"_Stz,  &Step::vNodePartitions);
  vAddStep("node.port"_Stz,        &Step::vNodePort);
  vAddStep("node.recvSocket!"_Stz, &Step::vNodeRecvSocketStore);
  vAddStep("node.recvSocket@"_Stz, &Step::vNodeRecvSocketFetch);
  vAddStep("node.sendSocket!"_Stz, &Step::vNodeSendSocketStore);
  vAddStep("node.sendSocket@"_Stz, &Step::vNodeSendSocketFetch);
  vAddStep("node.size"_Stz,        &Step::vNodeSize);
  vAddStep("node.start"_Stz,       &Step::vNodeStartNode);
  vAddStep("node.state!"_Stz,      &Step::vNodeStateStore);
  vAddStep("node.state@"_Stz,      &Step::vNodeStateFetch);
  vAddStep("node.status"_Stz,      &Step::vNodeStatus);
}

/*----------------------------------------------------------------------------*/
/* node::new -- create a new node                                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeNew(MP_Task mpTask)
{
  ENTRY("vNodeNew"_Stz);
  
  i8 nNodes = AM::nDefinedNodes();
  
  Node* pNode_copy = AM::pGrid()->pGetNode(nNodes - 1);

  Job_DefineNode* pJob_CreateNode = _pJob_CreateNode(AM::pThisTLSJob());
  EnmSet* pEnmSet = & pJob_CreateNode->m_oEnmSet;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s32 sNodeNm;
  
  if (! mpTask->m_pDataStack->bPop(sNodeNm))
    THROW_INVALID_DSTACK("node::new"_Stz);

  Node* pNode = new Node(pNode_copy->iNodeNo() + 1,
                         sNodeNm,
                         pEnmSet->szValue("IPADDR"_Stz),
                         pEnmSet->iValue("PORTNO"_Stz));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::pGrid()->vAddNode(pNode);
  
  mpTask->m_pDataStack->vPushPointer(pNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* node.forget -- remove node from node list and delete                       */
/*----------------------------------------------------------------------------*/
void Step::vNodeForget(MP_Task mpTask)
{
  ENTRY("vNodeForget"_Stz);
  
  Object* pPtrNode;
  
  if (! mpTask->m_pDataStack->bPop(pPtrNode))
    THROW_INVALID_DSTACK("node.forget"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  Grid* pGrid = AM::pGrid();
  
  pGrid->vRemoveNode(pNode);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* node.IP -- push IP of node                                                 */
/*----------------------------------------------------------------------------*/
void Step::vNodeIP(MP_Task mpTask)
{
  ENTRY("vNodeIP"_Stz);
  
  Object* pPtrNode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.IP"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sIP;
  sIP = pNode->szIPAddr();
  mpTask->m_pDataStack->vPushS256(sIP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.name -- push name of node                                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeName(MP_Task mpTask)
{
  ENTRY("vNodeName"_Stz);
  
  Object* pPtrNode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.name"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sName;
  sName = pNode->szNodeName();
  mpTask->m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeNo(MP_Task mpTask)
{
ENTRY("vNodeNo"_Stz);
  
  Object* pPtrNode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.no"_Stz);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  mpTask->m_pDataStack->vPushI8(pNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.partitions -- get no of partitions of this node (of a grid)           */
/*----------------------------------------------------------------------------*/
void Step::vNodePartitions(MP_Task mpTask)
{
  ENTRY("vNodePartitions"_Stz);
  
  mpTask->m_pDataStack->vPushI8(AM::pGrid()->nSlotsPerNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.port -- push port number of node (of a grid)                          */
/*----------------------------------------------------------------------------*/
void Step::vNodePort(MP_Task mpTask)
{
  ENTRY("vNodeNo"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.port"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  mpTask->m_pDataStack->vPushI8(pNode->iPortNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.recvSocket! -- set receive socket for node                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeRecvSocketStore(MP_Task mpTask)
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
void Step::vNodeRecvSocketFetch(MP_Task mpTask)
{
  ENTRY("vNodeRecvSocketFetch"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.recvSocket@"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  Socket* pSocket = pNode->pRecvSocket();
  
  mpTask->m_pDataStack->vPushPointer(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket! -- set send socket for node                               */
/*----------------------------------------------------------------------------*/
void Step::vNodeSendSocketStore(MP_Task mpTask)
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
void Step::vNodeSendSocketFetch(MP_Task mpTask)
{
  ENTRY("vNodeSendSocketFetch"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.sendSocket"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  Socket* pSocket = pNode->pSendSocket();
  
  mpTask->m_pDataStack->vPushPointer(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.size -- get size of this node (of a grid)                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeSize(MP_Task mpTask)
{
  ENTRY("vNodeSize"_Stz);
  
  mpTask->m_pDataStack->vPushI8(AM::iRegionSize());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.start -- start node                                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeStartNode(MP_Task mpTask)
{
  ENTRY("vNodeStartNode"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.start"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  mpTask->m_pDataStack->vPushBool(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.state! -- set state of node                                           */
/*----------------------------------------------------------------------------*/
void Step::vNodeStateStore(MP_Task mpTask)
{
  ENTRY("vNodeStateStore"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.state!"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  i8 iState;
  if ( ! mpTask->m_pDataStack->bPop(iState) )
    THROW_INVALID_DSTACK("node.state!"_Stz);
  
  pNode->vState((eNS)iState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.state@ -- get state of node                                           */
/*----------------------------------------------------------------------------*/
void Step::vNodeStateFetch(MP_Task mpTask)
{
  ENTRY("vNodeStateFetch"_Stz);
  
  Object* pPtrNode;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.state"_Stz);
  Node* pNode = _pNode(pPtrNode);
  
  eNS eState = pNode->eState();
  
  mpTask->m_pDataStack->vPushI8(eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.status -- get status of this node (of a grid)                         */
/*----------------------------------------------------------------------------*/
void Step::vNodeStatus(MP_Task mpTask)
{
  ENTRY("vNodeStatus"_Stz);

  s256 s;
  
  Grid* pGrid = AM::pGrid();
  
  s32 sRegionNm(AM::szRegionName());
  
  if (Region::bTest(sRegionNm))
    s.vSprint("row: ('%s', '%s', 'running')"_Stz,
            pGrid->pThisNode()->szNodeName(),
            sRegionNm.aChar());
  else
    s.vSprint("row: ('%s', '%s', 'not running')"_Stz,
            pGrid->pThisNode()->szNodeName(),
            sRegionNm.aChar());
  
  mpTask->m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
