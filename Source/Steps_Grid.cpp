/*============================================================================*/
/* Steps_Grid.cpp - grid step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern Arguments       gxArguments;

// grid: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Grid(void)
{
  vAddStep("grid::node"_Stz,         &Task::vGridNode,              false);
  vAddStep("grid::nodes"_Stz,        &Task::vGridNodes,             false);
  vAddStep("grid::partitions"_Stz,   &Task::vGridPartitions,        false);
  vAddStep("grid::show"_Stz,         &Task::vGridShow,              false);
  vAddStep("grid::slots"_Stz,        &Task::vGridSlots,             false);
  vAddStep("grid::started!"_Stz,     &Task::vGridStartedStore,      false);
  vAddStep("grid::started@"_Stz,     &Task::vGridStartedFetch,      false);
  vAddStep("grid::StartNode!"_Stz,   &Task::vGridStartNodeStore,    false);
  vAddStep("grid::StartNode@"_Stz,   &Task::vGridStartNodeFetch,    false);
  vAddStep("grid::thisNode@"_Stz,    &Task::vGridThisNodeFetch,     false);
  vAddStep("grid::thisNodeName@"_Stz,&Task::vGridThisNodeNameFetch, false);
  vAddStep("grid::toNode"_Stz,       &Task::vGridToNode,            false);
  vAddStep("grid::toPartition"_Stz,  &Task::vGridToPartition,       false);
  vAddStep("grid::toSlot"_Stz,       &Task::vGridToSlot,            false);
}

/*----------------------------------------------------------------------------*/
/* grid::node -- get node from grid                                            */
/*----------------------------------------------------------------------------*/
void Task::vGridNode(MP_Task mpTask)
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
void Task::vGridNodes(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);

  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->m_nNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::partitions -- get number of partitions in the grid                   */
/*----------------------------------------------------------------------------*/
void Task::vGridPartitions(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->m_nPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::show -- show the status of the grid                                  */
/*----------------------------------------------------------------------------*/
void Task::vGridShow(MP_Task mpTask)
{
  ENTRY("vGridShow"_Stz);
  
  vTrace("Grid::\n"_Stz);
  vTrace("  %lld nodes\n"_Stz,gpAttachMap->m_pGrid->m_nNodes);
  vTrace("  %lld slots per node\n"_Stz,gpAttachMap->m_pGrid->m_nSlots);
  vTrace("  %lld total partitions\n"_Stz,gpAttachMap->m_pGrid->m_nPartitions);
  
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    Node* pNode = gpAttachMap->m_pGrid->m_pNodes[i_Node];
    vTrace("  Node %lld : %s IP %s port %lld %s\n"_Stz,
           pNode->iNodeNo(),
           pNode->aNodeName(),
           pNode->aNodeIP(),
           pNode->iNodePortNo(),
           pNode->pSocket() ? "connected" : "not connected");
  }
  
  vTrace("\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::slots -- get number of slots of partitions per node in the grid       */
/*----------------------------------------------------------------------------*/
void Task::vGridSlots(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->m_nSlots);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNode@ -- get this node from the grid                              */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeFetch(MP_Task mpTask)
{
  ENTRY("vGridThisNodeFetch"_Stz);
  
  Object* pPtrGrid;
  
  i8 iNodeNo = gpAttachMap->m_pGrid->iThisNode();
  
  mpTask->m_pDataStack->vPushInt(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNodeName@ -- get this node name from the grid                     */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeNameFetch(MP_Task mpTask)
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
void Task::vGridStartedStore(MP_Task mpTask)
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
void Task::vGridStartNodeFetch(MP_Task mpTask)
{
  ENTRY("vGridStartNodeFetch"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode! -- set grid start node                                 */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeStore(MP_Task mpTask)
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
void Task::vGridStartedFetch(MP_Task mpTask)
{
  ENTRY("vGridGetStarted"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->bHasStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toNode -- convert partition id to node no                            */
/*----------------------------------------------------------------------------*/
void Task::vGridToNode(MP_Task mpTask)
{
  ENTRY("vGridToNode"_Stz);
  
  i8 iParitionId;
  if ( ! mpTask->m_pDataStack->bPop(iParitionId) )
    THROW_INVALID_DSTACK("grid::toNode"_Stz);
  
  // for the moment its always node 0                      //! DISCUSS WITH ALKA
  //mpTask->m_pDataStack->vPush((i8)0);
  
  i8 iNodeNo = TablePtn::iPartitionToNode(iParitionId);
  
  mpTask->m_pDataStack->vPushInt(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toPartition -- convert slot to partitionId                           */
/*----------------------------------------------------------------------------*/
void Task::vGridToPartition(MP_Task mpTask)
{
  ENTRY("vGridToPartition"_Stz);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("grid::toPartition"_Stz);
  
  i8 iPartitionId = TablePtn::iSlotToPartition(iSlot);
  
  mpTask->m_pDataStack->vPushInt(iPartitionId);              //! DISCUSS WITH ALKA
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toSlot -- convert partitionId to slot                                */
/*----------------------------------------------------------------------------*/
void Task::vGridToSlot(MP_Task mpTask)
{
  ENTRY("vGridToSlot"_Stz);
  
  i8 iPartitionId;
  if ( ! mpTask->m_pDataStack->bPop(iPartitionId) )
    THROW_INVALID_DSTACK("grid::toSlot"_Stz);
  
  i8 iSlot = TablePtn::iPartitionToSlot(iPartitionId);
  
  mpTask->m_pDataStack->vPushInt(iSlot);                     //! DISCUSS WITH ALKA
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
