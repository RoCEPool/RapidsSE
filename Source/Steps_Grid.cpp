/*============================================================================*/
/* Steps_Grid.cpp - grid step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern Arguments  gxArguments;
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Grid(void)
{
  vAddStep("grid::findNode"_Stz,     &Step::vGridFindNode);
  vAddStep("grid::node"_Stz,         &Step::vGridNode);
  vAddStep("grid::nodes"_Stz,        &Step::vGridNodes);
  vAddStep("grid::partitions"_Stz,   &Step::vGridPartitions);
  vAddStep("grid::show"_Stz,         &Step::vGridShow);
  vAddStep("grid::slots"_Stz,        &Step::vGridSlots);
  vAddStep("grid::started!"_Stz,     &Step::vGridStartedStore);
  vAddStep("grid::started@"_Stz,     &Step::vGridStartedFetch);
  vAddStep("grid::StartNode!"_Stz,   &Step::vGridStartNodeStore);
  vAddStep("grid::StartNode@"_Stz,   &Step::vGridStartNodeFetch);
  vAddStep("grid::thisNode@"_Stz,    &Step::vGridThisNodeFetch);
  vAddStep("grid::thisNodeName@"_Stz,&Step::vGridThisNodeNameFetch);
  vAddStep("grid::toNode"_Stz,       &Step::vGridToNode);
  vAddStep("grid::toPartition"_Stz,  &Step::vGridToPartition);
  vAddStep("grid::toSlot"_Stz,       &Step::vGridToSlot);
}

/*----------------------------------------------------------------------------*/
/* grid::findNode -- find node based on name                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridFindNode(MP_Task mpTask)
{
  ENTRY("vGridFindNode"_Stz);
  
  s256 sNodeNm;
  if ( ! mpTask->m_pDataStack->bPop(sNodeNm) )
    THROW_INVALID_DSTACK("grid::findNode"_Stz);
  
  Node* pNode = gpAttachMap->m_pGrid->pFindNode(sNodeNm);
  
  mpTask->m_pDataStack->vPushObject(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::node -- get node from grid                                           */
/*----------------------------------------------------------------------------*/
void Step::vGridNode(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPop(iNode) )
    THROW_INVALID_DSTACK("grid::node"_Stz);
  
  Node* pNode = gpAttachMap->m_pGrid->pGetNode(iNode);
  
  mpTask->m_pDataStack->vPushObject(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::nodes -- get number of nodes in the grid                             */
/*----------------------------------------------------------------------------*/
void Step::vGridNodes(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);

  mpTask->m_pDataStack->vPushI8(gpAttachMap->m_pGrid->m_nNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::partitions -- get number of partitions in the grid                   */
/*----------------------------------------------------------------------------*/
void Step::vGridPartitions(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  mpTask->m_pDataStack->vPushI8(gpAttachMap->m_pGrid->m_nPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::show -- show the status of the grid                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridShow(MP_Task mpTask)
{
  ENTRY("vGridShow"_Stz);
  
  vTrace("Grid::\n"_Stz);
  vTrace("  %lld nodes\n"_Stz,gpAttachMap->m_pGrid->m_nNodes);
  vTrace("  %lld slots per node\n"_Stz,gpAttachMap->m_pGrid->m_nSlots);
  vTrace("  %lld total partitions\n"_Stz,gpAttachMap->m_pGrid->m_nPartitions);
  
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    Node* pNode = gpAttachMap->m_pGrid->m_pNodes[i_Node];
    vTrace("  Node %lld : %s IP %s port %lld rcvSkt %p sndSkt %p [%s]\n"_Stz,
           pNode->iNodeNo(),
           pNode->aNodeName(),
           pNode->aNodeIP(),
           pNode->iNodePortNo(),
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
void Step::vGridSlots(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  mpTask->m_pDataStack->vPushI8(gpAttachMap->m_pGrid->m_nSlots);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNode@ -- get this node from the grid                              */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeFetch(MP_Task mpTask)
{
  ENTRY("vGridThisNodeFetch"_Stz);
  
  Object* pPtrGrid;
  
  i8 iNodeNo = gpAttachMap->m_pGrid->iThisNode();
  
  mpTask->m_pDataStack->vPushI8(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNodeName@ -- get this node name from the grid                     */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeNameFetch(MP_Task mpTask)
{
  ENTRY("vGridThisNodeNameFetch"_Stz);
  
  s256 sName;
  sName = gpAttachMap->m_pGrid->pThisNode()->aNodeName();
  
  mpTask->m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridStartedStore(MP_Task mpTask)
{
  ENTRY("vGridSetStarted"_Stz);
  
  i8 iStarted;
  
  if ( ! mpTask->m_pDataStack->bPop(iStarted) )
    THROW_INVALID_DSTACK("grid::started!"_Stz);
  
  gpAttachMap->m_pGrid->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode@ -- get grid start node                                 */
/*----------------------------------------------------------------------------*/
void Step::vGridStartNodeFetch(MP_Task mpTask)
{
  ENTRY("vGridStartNodeFetch"_Stz);
  
  mpTask->m_pDataStack->vPushI8(gpAttachMap->m_pGrid->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode! -- set grid start node                                 */
/*----------------------------------------------------------------------------*/
void Step::vGridStartNodeStore(MP_Task mpTask)
{
  ENTRY("vGridStartNodeStore"_Stz);
  
  i8 iStartNode;
  
  if ( ! mpTask->m_pDataStack->bPop(iStartNode) )
    THROW_INVALID_DSTACK("grid::StartNode!"_Stz);
  
  gpAttachMap->m_pGrid->vStartNode(iStartNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::started@ -- get grid started                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridStartedFetch(MP_Task mpTask)
{
  ENTRY("vGridGetStarted"_Stz);
  
  mpTask->m_pDataStack->vPushI8(gpAttachMap->m_pGrid->bHasStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toNode -- convert partition id to node no                            */
/*----------------------------------------------------------------------------*/
void Step::vGridToNode(MP_Task mpTask)
{
  ENTRY("vGridToNode"_Stz);
  
  i8 iParitionId;
  if ( ! mpTask->m_pDataStack->bPop(iParitionId) )
    THROW_INVALID_DSTACK("grid::toNode"_Stz);
  
  // for the moment its always node 0                      //! DISCUSS WITH ALKA
  //mpTask->m_pDataStack->vPush((i8)0);
  
  i8 iNodeNo = TablePtn::iPartitionToNode(iParitionId);
  
  mpTask->m_pDataStack->vPushI8(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toPartition -- convert slot to partitionId                           */
/*----------------------------------------------------------------------------*/
void Step::vGridToPartition(MP_Task mpTask)
{
  ENTRY("vGridToPartition"_Stz);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("grid::toPartition"_Stz);
  
  i8 iPartitionId = TablePtn::iSlotToPartition(iSlot);
  
  mpTask->m_pDataStack->vPushI8(iPartitionId);              //! DISCUSS WITH ALKA
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toSlot -- convert partitionId to slot                                */
/*----------------------------------------------------------------------------*/
void Step::vGridToSlot(MP_Task mpTask)
{
  ENTRY("vGridToSlot"_Stz);
  
  i8 iPartitionId;
  if ( ! mpTask->m_pDataStack->bPop(iPartitionId) )
    THROW_INVALID_DSTACK("grid::toSlot"_Stz);
  
  i8 iSlot = TablePtn::iPartitionToSlot(iPartitionId);
  
  mpTask->m_pDataStack->vPushI8(iSlot);                     //! DISCUSS WITH ALKA
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
