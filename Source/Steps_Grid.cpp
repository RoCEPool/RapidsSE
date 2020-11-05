/*============================================================================*/
/* Steps_Grid.cpp - grid step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Grid(void)
{
  vAddStep("grid::connectedNodes"_Stz,    &Step::vGridConnectedNodes);
  vAddStep("grid::definedNodes"_Stz,      &Step::vGridDefinedNodes);
  vAddStep("grid::findNode"_Stz,          &Step::vGridFindNode);
  vAddStep("grid::incConnectedNodes"_Stz, &Step::vGridIncConnectedNodes);
  vAddStep("grid::node"_Stz,              &Step::vGridNode);
  vAddStep("grid::partitions"_Stz,        &Step::vGridPartitions);
  vAddStep("grid::show"_Stz,              &Step::vGridShow);
  vAddStep("grid::slots"_Stz,             &Step::vGridSlots);
  vAddStep("grid::started!"_Stz,          &Step::vGridStartedStore);
  vAddStep("grid::started@"_Stz,          &Step::vGridStartedFetch);
  vAddStep("grid::StartNode!"_Stz,        &Step::vGridStartNodeStore);
  vAddStep("grid::StartNode@"_Stz,        &Step::vGridStartNodeFetch);
  vAddStep("grid::thisNode@"_Stz,         &Step::vGridThisNodeFetch);
  vAddStep("grid::thisNodeName@"_Stz,     &Step::vGridThisNodeNameFetch);
  vAddStep("grid::toNode"_Stz,            &Step::vGridToNode);
  vAddStep("grid::toPartition"_Stz,       &Step::vGridToPartition);
  vAddStep("grid::toSlot"_Stz,            &Step::vGridToSlot);
}

/*----------------------------------------------------------------------------*/
/* grid::connectedNodes -- get number of connected nodes in the grid          */
/*----------------------------------------------------------------------------*/
void Step::vGridConnectedNodes(void)
{
  ENTRY();
  
  i8 nConnectedNodes = AM::pGrid()->nConnectedNodes();

  m_pDataStack->vPushInt(nConnectedNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::definedNodes -- get number of nodes in the grid                             */
/*----------------------------------------------------------------------------*/
void Step::vGridDefinedNodes(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->nDefinedNodes());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::findNode -- find node based on name                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridFindNode(void)
{
  ENTRY();
  
  s256 sNodeNm;
  m_pDataStack->vPopStr(sNodeNm,__func__);
  
  Node* pNode = AM::pGrid()->pFindNode(sNodeNm);
  
  m_pDataStack->vPushPtr(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::incConnectedNodes -- inc connected nodes                             */
/*----------------------------------------------------------------------------*/
void Step::vGridIncConnectedNodes(void)
{
  ENTRY();
  
  AM::pGrid()->vIncConnectedNodes();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::node -- get node from grid                                           */
/*----------------------------------------------------------------------------*/
void Step::vGridNode(void)
{
  ENTRY();
  
  i8 iNode = m_pDataStack->iPopInt(__func__);
  
  Node* pNode = AM::pGrid()->pGetNode(iNode);
  
  m_pDataStack->vPushPtr(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::partitions -- get number of partitions in the grid                   */
/*----------------------------------------------------------------------------*/
void Step::vGridPartitions(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->nPartitions());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::show -- show the status of the grid                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridShow(void)
{
  ENTRY();
  
  vTrace("Grid::\n"_Stz);
  vTrace("  %lld defined nodes\n"_Stz,AM::pGrid()->nDefinedNodes());
  vTrace("  %lld slots per node\n"_Stz,AM::pGrid()->nSlotsPerNode());
  vTrace("  %lld total partitions\n"_Stz,AM::pGrid()->nPartitions());
  
  for (i8 i_Node = 0; i_Node < AM::pGrid()->nDefinedNodes(); i_Node++) {
    Node* pNode = AM::pGrid()->pGetNode(i_Node);
    vTrace("  Node %lld : %s IP %s port %lld rcvSkt %p sndSkt %p [%s]\n"_Stz,
           pNode->iNodeNo(),
           pNode->szNodeNm(),
           pNode->szIPAddr(),
           pNode->iPortNo(),
           pNode->pRecvSocket(),
           pNode->pSendSocket(),

           pNode->pSendSocket() ? "connected" : "not connected");
  }
  
  vTrace("\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::slots -- get number of slots of partitions per node in the grid       */
/*----------------------------------------------------------------------------*/
void Step::vGridSlots(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->nSlotsPerNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNode@ -- get this node from the grid                              */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeFetch(void)
{
  ENTRY();
  
  Object* pPtrGrid;
  
  i8 iNodeNo = AM::pGrid()->iThisNode();
  
  m_pDataStack->vPushInt(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNodeName@ -- get this node name from the grid                     */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeNameFetch(void)
{
  ENTRY();
  
  s256 sName;
  sName = AM::pGrid()->pThisNode()->szNodeNm();
  
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridStartedStore(void)
{
  ENTRY();
  
  i8 iStarted = m_pDataStack->iPopInt(__func__);
  
  AM::pGrid()->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode@ -- get grid start node                                    */
/*----------------------------------------------------------------------------*/
void Step::vGridStartNodeFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode! -- set grid start node                                    */
/*----------------------------------------------------------------------------*/
void Step::vGridStartNodeStore(void)
{
  ENTRY();
  
  i8 iStartNode = m_pDataStack->iPopInt(__func__);
  
  AM::pGrid()->vStartNode(iStartNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::started@ -- get grid started                                         */
/*----------------------------------------------------------------------------*/
void Step::vGridStartedFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->bHasStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toNode -- convert partition id to node no                            */
/*----------------------------------------------------------------------------*/
void Step::vGridToNode(void)
{
  ENTRY();
  
  i8 iParitionId = m_pDataStack->iPopInt(__func__);
  
  // for the moment its always node 0                      //! DISCUSS WITH ALKA
  //m_pDataStack->vPush((i8)0);
  
  i8 iNodeNo = TablePtn::iPartitionToNode(iParitionId);
  
  m_pDataStack->vPushInt(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toPartition -- convert slot to partitionId                           */
/*----------------------------------------------------------------------------*/
void Step::vGridToPartition(void)
{
  ENTRY();
  
  i8 iSlot = m_pDataStack->iPopInt(__func__);
  
  i8 iPartitionId = TablePtn::iSlotToPartition(iSlot);
  
  m_pDataStack->vPushInt(iPartitionId);              //! DISCUSS WITH ALKA
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toSlot -- convert partitionId to slot                                */
/*----------------------------------------------------------------------------*/
void Step::vGridToSlot(void)
{
  ENTRY();
  
  i8 iPartitionId = m_pDataStack->iPopInt(__func__);
  
  i8 iSlot = TablePtn::iPartitionToSlot(iPartitionId);
  
  m_pDataStack->vPushInt(iSlot);                     //! DISCUSS WITH ALKA
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
