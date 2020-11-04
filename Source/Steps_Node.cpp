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
  ENTRY();
  
  i8 nNodes = AM::nDefinedNodes();
  
  Node* pNode_copy = AM::pGrid()->pGetNode(nNodes - 1);

  Job* pJob = AM::pThisTLSJob();
  TagSet* pJobTags = pJob->pJobTags();
  
  //pTagSet->vPrint();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s32 sNodeNm;
  
  m_pDataStack->vPopStr(sNodeNm,__func__);

  Node* pNode = new Node(pNode_copy->iNodeNo() + 1,
                         sNodeNm,
                         pJobTags->szValue("IPADDR"_Stz),
                         pJobTags->szValue("PORTNO"_Stz)->iToI8());

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
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
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
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sIP;
  sIP = pNode->szIPAddr();
  m_pDataStack->vPushStr(sIP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.name -- push name of node                                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeName(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sName;
  sName = pNode->szNodeNm();
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeNo(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eClass());
  
  m_pDataStack->vPushInt(pNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.partitions -- get no of partitions of this node (of a grid)           */
/*----------------------------------------------------------------------------*/
void Step::vNodePartitions(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->nSlotsPerNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.port -- push port number of node (of a grid)                          */
/*----------------------------------------------------------------------------*/
void Step::vNodePort(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pNode->iPortNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.recvSocket! -- set receive socket for node                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeRecvSocketStore(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));

  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  pNode->vRecvSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.recvSocket@ -- get receive socket for node                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeRecvSocketFetch(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  Socket* pSocket = pNode->pRecvSocket();
  
  m_pDataStack->vPushPtr(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket! -- set send socket for node                               */
/*----------------------------------------------------------------------------*/
void Step::vNodeSendSocketStore(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));

  Socket* pSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
    
  pNode->vSendSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket@ -- get send socket for node                               */
/*----------------------------------------------------------------------------*/
void Step::vNodeSendSocketFetch(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  Socket* pSocket = pNode->pSendSocket();
  
  m_pDataStack->vPushPtr(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.size -- get size of this node (of a grid)                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeSize(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::iRegionSize());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.start -- start node                                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeStartNode(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.state! -- set state of node                                           */
/*----------------------------------------------------------------------------*/
void Step::vNodeStateStore(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  i8 iState = m_pDataStack->iPopInt(__func__);
  
  pNode->vState((eNS)iState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.state@ -- get state of node                                           */
/*----------------------------------------------------------------------------*/
void Step::vNodeStateFetch(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  eNS eState = pNode->eState();
  
  m_pDataStack->vPushInt(eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.status -- get status of this node (of a grid)                         */
/*----------------------------------------------------------------------------*/
void Step::vNodeStatus(void)
{
  ENTRY();

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
  
  m_pDataStack->vPushStr(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
