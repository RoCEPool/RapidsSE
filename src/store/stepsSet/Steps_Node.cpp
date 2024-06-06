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
#include "JobList.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Node(void)
{
  vAddStep("node::new"_Stz,        &Step::vNodeNew);
  
  vAddStep("Node[]"_Stz,           &Step::vNodeGet);
  vAddStep("Node[].get"_Stz,       &Step::vNodeGet);
  vAddStep("Node[].set"_Stz,       &Step::vNodeSet);

  vAddStep("node.cleanup"_Stz,      &Step::vNodeCleanup);
  vAddStep("node.connectRetry"_Stz, &Step::vNodeConnectRetry);
  vAddStep("node.forget"_Stz,      &Step::vNodeForget);
  vAddStep("node.IP"_Stz,          &Step::vNodeIP);
  vAddStep("node.isThisNode"_Stz,  &Step::vNodeIsThisNode);
  vAddStep("node.name"_Stz,        &Step::vNodeName);
  vAddStep("node.id"_Stz,          &Step::vNodeId);
  vAddStep("node.partitions"_Stz,  &Step::vNodePartitions);
  vAddStep("node.port"_Stz,        &Step::vNodePort);
  vAddStep("node.portWL"_Stz,      &Step::vNodePortWL);
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
  m_oDataStack.vPopStr(sNodeNm, __func__);
  
  s32 sIpAddr;
  m_oDataStack.vPopStr(sIpAddr, __func__);
  
  i8 iPortNo   = m_oDataStack.iPopInt(__func__);
  i8 iPortNoWL = m_oDataStack.iPopInt(__func__);

  i8 nKnownNodes = AM::pGrid()->nKnownNodes();
  i8 nPtnsPerNode = AM::SYSARGS::nPtnsPerNode();

  Node* pNode = new Node(nKnownNodes,
                         sNodeNm,
                         nPtnsPerNode,
                         sIpAddr,
                         iPortNo,
                         iPortNoWL,
                         AM::szRegionName());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::pGrid()->vAddNode(pNode);

  
  m_oDataStack.vPushPtr(pNode);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* node.cleanup -- set status to failed and cleanup sockets, set grid to unhealthy*/
/*----------------------------------------------------------------------------*/
void Step::vNodeCleanup(void)
{
  ENTRY();

  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  Grid* pGrid = AM::pGrid();

  pGrid->vGrabWrite();

  pGrid->vHealthy(false);

  // latch the job Originating list and Job Participant list
  JobList* pPtpJobList = AM::pParticipatingJobs();
  pPtpJobList->vLatch(true);

  JobList* pOriginatingJobList = AM::pOriginatingJobs();
  pOriginatingJobList->vLatch(true);

  pNode->vState(NS_INACTIVE);

  Socket* pRecvSocket = pNode->pRecvSocket();
  if(pRecvSocket)
  {
    if(pRecvSocket->bIsOpen())
    {
      SysCall oSysCall;
      bool bResult = pRecvSocket->bClose(oSysCall);

      if ( ! bResult )
        vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
               "Unable to close recv socket: Errno %d!"_Stz, oSysCall.iError());
    }
    delete pRecvSocket;
    pNode->vRecvSocket(nullptr);
  }

  Socket* pSendSocket = pNode->pSendSocket();
  if(pSendSocket)
  {
    if(pSendSocket->bIsOpen())
    {
      SysCall oSysCall;
      bool bResult = pSendSocket->bClose(oSysCall);

      if ( ! bResult )
        vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
               "Unable to close send socket: Errno %d!"_Stz, oSysCall.iError());
    }
    delete pSendSocket;
    pNode->vSendSocket(nullptr);
  }

  pGrid->vDropWrite();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.connectRetry -- Number of connect retry attempts                      */
/*----------------------------------------------------------------------------*/
void Step::vNodeConnectRetry(void)
{
  ENTRY();

  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));

  pNode->vRetryDecrement();

  m_oDataStack.vPushInt(pNode->iRetry());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.forget -- remove node from node list and delete                       */
/*----------------------------------------------------------------------------*/
void Step::vNodeForget(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
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
  
  m_oDataStack.vPushPtr(*m_pLogicUnit->m_xpNode);

  EXIT();
}
/*----------------------------------------------------------------------------*/
/* node.IP -- push IP of node                                                 */
/*----------------------------------------------------------------------------*/
void Step::vNodeIP(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eGetClassNo());
  
  s256 sIP;
  sIP = pNode->szIPAddr();
  m_oDataStack.vPushStr(sIP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.isThisNode -- is given node this node                                 */
/*----------------------------------------------------------------------------*/
void Step::vNodeIsThisNode(void)
{
  ENTRY();

  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));

  Grid* pGrid = AM::pGrid();
  
  if (pGrid->pThisNode() == pNode)
    m_oDataStack.vPushInt(true);
  else
    m_oDataStack.vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.name -- push name of node                                             */
/*----------------------------------------------------------------------------*/
void Step::vNodeName(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eGetClassNo());
  
  s256 sName;
  sName = pNode->szNodeNm();
  m_oDataStack.vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.id -- push node id of of given node                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeId(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  ASSERT(CN_NODE == pNode->eGetClassNo());
  
  m_oDataStack.vPushInt(pNode->iNodeId());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.partitions -- get no of partitions of this node (of a grid)           */
/*----------------------------------------------------------------------------*/
void Step::vNodePartitions(void)
{
  ENTRY();
  
  m_oDataStack.vPushInt(AM::PTNMAP::nPtnsPerNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.port -- push port number of node (of a grid)                          */
/*----------------------------------------------------------------------------*/
void Step::vNodePort(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  
  m_oDataStack.vPushInt(pNode->iPortNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.portWL -- push wireline port number of node (of a grid)               */
/*----------------------------------------------------------------------------*/
void Step::vNodePortWL(void)
{
  ENTRY();

  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));

  m_oDataStack.vPushInt(pNode->iPortNoWL());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.recvSocket! -- set receive socket for node                            */
/*----------------------------------------------------------------------------*/
void Step::vNodeRecvSocketStore(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));

  Socket* pSocket = _pSocket(m_oDataStack.pPopPtr(__func__));

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
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  
  Socket* pSocket = pNode->pRecvSocket();
  
  m_oDataStack.vPushPtr(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket! -- set send socket for node                               */
/*----------------------------------------------------------------------------*/
void Step::vNodeSendSocketStore(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));

  Socket* pSocket = _pSocket(m_oDataStack.pPopPtr(__func__));
    
  pNode->vSendSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.sendSocket@ -- get send socket for node                               */
/*----------------------------------------------------------------------------*/
void Step::vNodeSendSocketFetch(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  
  Socket* pSocket = pNode->pSendSocket();
  
  m_oDataStack.vPushPtr(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Node[].set -- set Node[]                                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeSet(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  
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
  
  m_oDataStack.vPushInt(AM::iRegionSize());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.start -- start node                                                   */
/*----------------------------------------------------------------------------*/
void Step::vNodeStartNode(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  UNUSED(pNode);

  m_oDataStack.vPushInt(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.state! -- set state of node                                           */
/*----------------------------------------------------------------------------*/
void Step::vNodeStateStore(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  
  i8 iState = m_oDataStack.iPopInt(__func__);
  
  pNode->vState((eNS)iState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.state@ -- get state of node                                           */
/*----------------------------------------------------------------------------*/
void Step::vNodeStateFetch(void)
{
  ENTRY();
  
  Node* pNode = _pNode(m_oDataStack.pPopPtr(__func__));
  
  eNS eState = pNode->eState();
  
  m_oDataStack.vPushInt(eState);
  
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
  
  m_oDataStack.vPushStr(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
