/*============================================================================*/
/* Steps_Node.cpp - node step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node.hpp"
#include "Grid.hpp"
#include "Job.hpp"

#include "Socket.hpp" //?
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Node(void)
{
  vAddStep("node::new"_Stz,        &Step::vNodeNew);
  
  vAddStep("Node[]"_Stz,           &Step::vNodeGet);
  vAddStep("Node[].get"_Stz,       &Step::vNodeGet);
  vAddStep("Node[].set"_Stz,       &Step::vNodeSet);

  vAddStep("node.forget"_Stz,      &Step::vNodeForget);
  vAddStep("node.IP"_Stz,          &Step::vNodeIP);
  vAddStep("node.isThisNode"_Stz,  &Step::vNodeIsThisNode);
  vAddStep("node.name"_Stz,        &Step::vNodeName);
  vAddStep("node.id"_Stz,          &Step::vNodeId);
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
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s32 sNodeNm;
  m_pDataStack->vPopStr(sNodeNm,__func__);
  
  s32 sIpAddr;
  m_pDataStack->vPopStr(sIpAddr,__func__);
  
  i8 iPortNo = m_pDataStack->iPopInt(__func__);

  i8 nKnownNodes = AM::pGrid()->nKnownNodes();
  i8 nPtnsPerNode = AM::SYSARGS::nPtnsPerNode();

  Node* pNode = new Node(nKnownNodes,
                         sNodeNm,
                         nPtnsPerNode,
                         sIpAddr,
                         iPortNo,
                         AM::szRegionName());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::pGrid()->vAddNode(pNode);

  
  m_pDataStack->vPushPtr(pNode);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
  ASSERT(CN_NODE == pNode->eGetClassNo());
  
  Grid* pGrid = AM::pGrid();

  pGrid->vRemoveNode(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Node[].get -- get Node[]                                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpNode);

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* node.IP -- push IP of node                                                 */
/*----------------------------------------------------------------------------*/
void Step::vNodeIP(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eGetClassNo());
  
  s256 sIP;
  sIP = pNode->szIPAddr();
  m_pDataStack->vPushStr(sIP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.isThisNode -- is given node this node                                 */
/*----------------------------------------------------------------------------*/
void Step::vNodeIsThisNode(void)
{
  ENTRY();

  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));

  Grid* pGrid = AM::pGrid();
  
  if (pGrid->pThisNode() == pNode)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.name -- push name of node                                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeName(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eGetClassNo());
  
  s256 sName;
  sName = pNode->szNodeNm();
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.id -- push node id of of given node                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeId(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eGetClassNo());
  
  m_pDataStack->vPushInt(pNode->iNodeId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.partitions -- get no of partitions of this node (of a grid)           */
/*----------------------------------------------------------------------------*/
void Step::vNodePartitions(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::PTNMAP::nPtnsPerNode());
  
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

  if (pNode && pSocket) {
    pNode->vRecvSocket(pSocket);
  }
  
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
/* Node[].set -- set Node[]                                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeSet(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pNode || m_pLogicUnit->m_xpNode.bIsNull());
  
  m_pLogicUnit->m_xpNode = pNode;

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
  UNUSED(pNode);

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
