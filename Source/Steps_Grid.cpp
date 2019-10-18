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
void Step::vGridConnectedNodes(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridConnectedNodes"_Stz);
  
  i8 nConnectedNodes = AM::pGrid()->nConnectedNodes();

  mpTask->m_pForth->m_pDataStack->vPushI8(nConnectedNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::definedNodes -- get number of nodes in the grid                             */
/*----------------------------------------------------------------------------*/
void Step::vGridDefinedNodes(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridDefinedNodes"_Stz);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(AM::pGrid()->nDefinedNodes());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::findNode -- find node based on name                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridFindNode(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridFindNode"_Stz);
  
  s256 sNodeNm;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sNodeNm) )
    THROW_INVALID_DSTACK("grid::findNode"_Stz);
  
  Node* pNode = AM::pGrid()->pFindNode(sNodeNm);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::incConnectedNodes -- inc connected nodes                             */
/*----------------------------------------------------------------------------*/
void Step::vGridIncConnectedNodes(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridIncConnectedNodes"_Stz);
  
  AM::pGrid()->vIncConnectedNodes();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::node -- get node from grid                                           */
/*----------------------------------------------------------------------------*/
void Step::vGridNode(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridNode"_Stz);
  
  i8 iNode;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iNode) )
    THROW_INVALID_DSTACK("grid::node"_Stz);
  
  Node* pNode = AM::pGrid()->pGetNode(iNode);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::partitions -- get number of partitions in the grid                   */
/*----------------------------------------------------------------------------*/
void Step::vGridPartitions(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridNode"_Stz);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(AM::pGrid()->nPartitions());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::show -- show the status of the grid                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridShow(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridShow"_Stz);
  
  vTrace("Grid::\n"_Stz);
  vTrace("  %lld defined nodes\n"_Stz,AM::pGrid()->nDefinedNodes());
  vTrace("  %lld slots per node\n"_Stz,AM::pGrid()->nSlotsPerNode());
  vTrace("  %lld total partitions\n"_Stz,AM::pGrid()->nPartitions());
  
  for (i8 i_Node = 0; i_Node < AM::pGrid()->nDefinedNodes(); i_Node++) {
    Node* pNode = AM::pGrid()->m_pNodes[i_Node];
    vTrace("  Node %lld : %s IP %s port %lld rcvSkt %p sndSkt %p [%s]\n"_Stz,
           pNode->iNodeNo(),
           pNode->szNodeName(),
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
void Step::vGridSlots(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridNode"_Stz);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(AM::pGrid()->nSlotsPerNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNode@ -- get this node from the grid                              */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridThisNodeFetch"_Stz);
  
  Object* pPtrGrid;
  
  i8 iNodeNo = AM::pGrid()->iThisNode();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNodeName@ -- get this node name from the grid                     */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeNameFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridThisNodeNameFetch"_Stz);
  
  s256 sName;
  sName = AM::pGrid()->pThisNode()->szNodeName();
  
  mpTask->m_pForth->m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridStartedStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridSetStarted"_Stz);
  
  i8 iStarted;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iStarted) )
    THROW_INVALID_DSTACK("grid::started!"_Stz);
  
  AM::pGrid()->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode@ -- get grid start node                                 */
/*----------------------------------------------------------------------------*/
void Step::vGridStartNodeFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridStartNodeFetch"_Stz);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(AM::pGrid()->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode! -- set grid start node                                 */
/*----------------------------------------------------------------------------*/
void Step::vGridStartNodeStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridStartNodeStore"_Stz);
  
  i8 iStartNode;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iStartNode) )
    THROW_INVALID_DSTACK("grid::StartNode!"_Stz);
  
  AM::pGrid()->vStartNode(iStartNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::started@ -- get grid started                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridStartedFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridGetStarted"_Stz);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(AM::pGrid()->bHasStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toNode -- convert partition id to node no                            */
/*----------------------------------------------------------------------------*/
void Step::vGridToNode(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridToNode"_Stz);
  
  i8 iParitionId;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iParitionId) )
    THROW_INVALID_DSTACK("grid::toNode"_Stz);
  
  // for the moment its always node 0                      //! DISCUSS WITH ALKA
  //mpTask->m_pForth->m_pDataStack->vPush((i8)0);
  
  i8 iNodeNo = TablePtn::iPartitionToNode(iParitionId);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toPartition -- convert slot to partitionId                           */
/*----------------------------------------------------------------------------*/
void Step::vGridToPartition(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridToPartition"_Stz);
  
  i8 iSlot;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("grid::toPartition"_Stz);
  
  i8 iPartitionId = TablePtn::iSlotToPartition(iSlot);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iPartitionId);              //! DISCUSS WITH ALKA
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toSlot -- convert partitionId to slot                                */
/*----------------------------------------------------------------------------*/
void Step::vGridToSlot(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vGridToSlot"_Stz);
  
  i8 iPartitionId;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iPartitionId) )
    THROW_INVALID_DSTACK("grid::toSlot"_Stz);
  
  i8 iSlot = TablePtn::iPartitionToSlot(iPartitionId);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iSlot);                     //! DISCUSS WITH ALKA
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
