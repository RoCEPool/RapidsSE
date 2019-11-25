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
void Step::vNodeNew(void)
{
  ENTRY(); // ENTRY("vNodeNew"_Stz);
  
  i8 nNodes = AM::nDefinedNodes();
  
  Node* pNode_copy = AM::pGrid()->pGetNode(nNodes - 1);

  Job* pJob = AM::pThisTLSJob();
  EnameSet* pEnameSet = pJob->pEnameSet();
  
  pEnameSet->vPrint();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s32 sNodeNm;
  
  if (! m_pDataStack->bPopS32(sNodeNm))
    THROW_DSTACK();

  Node* pNode = new Node(pNode_copy->iNodeNo() + 1,
                         sNodeNm,
                         pEnameSet->szValue("IPADDR"_Stz),
                         pEnameSet->szValue("PORTNO"_Stz)->iToI8());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::pGrid()->vAddNode(pNode);
  
  m_pDataStack->vPushPtr(pNode);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Article* pGridArticle = AM::pLexiconArticle()->pFind("SEgrid"_Stz);
  
  pGridArticle->vAppend(pNode->pArticle());
  pGridArticle->vDumpRecursively(0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* node.forget -- remove node from node list and delete                       */
/*----------------------------------------------------------------------------*/
void Step::vNodeForget(void)
{
  ENTRY(); // ENTRY("vNodeForget"_Stz);
  
  Object* pPtrNode;
  
  if (! m_pDataStack->bPopPtr(pPtrNode))
    THROW_DSTACK();
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  Grid* pGrid = AM::pGrid();
  
  
  Article* pGridArticle = AM::pLexiconArticle()->pFind("SEgrid"_Stz);
  pGridArticle->vRemove(pNode->pArticle());
  pGridArticle->vDumpRecursively(0);

  pGrid->vRemoveNode(pNode);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* node.IP -- push IP of node                                                 */
/*----------------------------------------------------------------------------*/
void Step::vNodeIP(void)
{
  ENTRY(); // ENTRY("vNodeIP"_Stz);
  
  Object* pPtrNode;
  
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sIP;
  sIP = pNode->szIPAddr();
  m_pDataStack->vPushS256(sIP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.name -- push name of node                                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeName(void)
{
  ENTRY(); // ENTRY("vNodeName"_Stz);
  
  Object* pPtrNode;
  
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sName;
  sName = pNode->szNodeNm();
  m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeNo(void)
{
  ENTRY(); // ENTRY("vNodeNo"_Stz);
  
  Object* pPtrNode;
  
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  m_pDataStack->vPushI8(pNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.partitions -- get no of partitions of this node (of a grid)           */
/*----------------------------------------------------------------------------*/
void Step::vNodePartitions(void)
{
  ENTRY(); // ENTRY("vNodePartitions"_Stz);
  
  m_pDataStack->vPushI8(AM::pGrid()->nSlotsPerNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.port -- push port number of node (of a grid)                          */
/*----------------------------------------------------------------------------*/
void Step::vNodePort(void)
{
  ENTRY(); // ENTRY("vNodeNo"_Stz);
  
  Object* pPtrNode;
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  Node* pNode = _pNode(pPtrNode);
  
  m_pDataStack->vPushI8(pNode->iPortNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.recvSocket! -- set receive socket for node                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeRecvSocketStore(void)
{
  ENTRY(); // ENTRY("vNodeRecvSocketStore"_Stz);
  
  Object* pPtrNode;
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  Node* pNode = _pNode(pPtrNode);
  
  Object* pSocketPtr;
  if ( ! m_pDataStack->bPopPtr(pSocketPtr) )
    THROW_DSTACK();
  Socket* pSocket = _pSocket(pSocketPtr);
  
  pNode->vRecvSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.recvSocket@ -- get receive socket for node                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeRecvSocketFetch(void)
{
  ENTRY(); // ENTRY("vNodeRecvSocketFetch"_Stz);
  
  Object* pPtrNode;
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  Node* pNode = _pNode(pPtrNode);
  
  Socket* pSocket = pNode->pRecvSocket();
  
  m_pDataStack->vPushPtr(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket! -- set send socket for node                               */
/*----------------------------------------------------------------------------*/
void Step::vNodeSendSocketStore(void)
{
  ENTRY(); // ENTRY("vNodeSendSocketStore"_Stz);
  
  Object* pPtrNode;
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  Node* pNode = _pNode(pPtrNode);

  Object* pSocketPtr;
  if ( ! m_pDataStack->bPopPtr(pSocketPtr) )
    THROW_DSTACK();
  Socket* pSocket = _pSocket(pSocketPtr);
    
  pNode->vSendSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket@ -- get send socket for node                               */
/*----------------------------------------------------------------------------*/
void Step::vNodeSendSocketFetch(void)
{
  ENTRY(); // ENTRY("vNodeSendSocketFetch"_Stz);
  
  Object* pPtrNode;
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  Node* pNode = _pNode(pPtrNode);
  
  Socket* pSocket = pNode->pSendSocket();
  
  m_pDataStack->vPushPtr(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.size -- get size of this node (of a grid)                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeSize(void)
{
  ENTRY(); // ENTRY("vNodeSize"_Stz);
  
  m_pDataStack->vPushI8(AM::iRegionSize());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.start -- start node                                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeStartNode(void)
{
  ENTRY(); // ENTRY("vNodeStartNode"_Stz);
  
  Object* pPtrNode;
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  Node* pNode = _pNode(pPtrNode);
  
  m_pDataStack->vPushBool(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.state! -- set state of node                                           */
/*----------------------------------------------------------------------------*/
void Step::vNodeStateStore(void)
{
  ENTRY(); // ENTRY("vNodeStateStore"_Stz);
  
  Object* pPtrNode;
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  Node* pNode = _pNode(pPtrNode);
  
  i8 iState;
  if ( ! m_pDataStack->bPopI8(iState) )
    THROW_DSTACK();
  
  pNode->vState((eNS)iState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.state@ -- get state of node                                           */
/*----------------------------------------------------------------------------*/
void Step::vNodeStateFetch(void)
{
  ENTRY(); // ENTRY("vNodeStateFetch"_Stz);
  
  Object* pPtrNode;
  if ( ! m_pDataStack->bPopPtr(pPtrNode) )
    THROW_DSTACK();
  Node* pNode = _pNode(pPtrNode);
  
  eNS eState = pNode->eState();
  
  m_pDataStack->vPushI8(eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.status -- get status of this node (of a grid)                         */
/*----------------------------------------------------------------------------*/
void Step::vNodeStatus(void)
{
  ENTRY(); // ENTRY("vNodeStatus"_Stz);

  s256 s;
  
  Grid* pGrid = AM::pGrid();
  
  s32 sRegionNm(AM::szRegionName());
  
  if (Region::bTest(sRegionNm))
    s.vSprint("row: ('%s', '%s', 'running')"_Stz,
            pGrid->pThisNode()->szNodeNm(),
            sRegionNm.azChar());
  else
    s.vSprint("row: ('%s', '%s', 'not running')"_Stz,
            pGrid->pThisNode()->szNodeNm(),
            sRegionNm.azChar());
  
  m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
