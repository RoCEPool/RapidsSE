/*============================================================================*/
/* Steps_Grid.cpp - grid step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern Arguments       gxArguments;

extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// grid: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Grid(void)
{
  vAddPrimitive("grid.appendNodes"_Stz,   &vGridAppendNodes,       false);
  vAddPrimitive("grid.find"_Stz,          &vGridFindNode,          false);
  vAddPrimitive("grid.init"_Stz,          &vGridInit,              false);
  vAddPrimitive("grid.node"_Stz,          &vGridNode,              false);
  vAddPrimitive("grid.nodes"_Stz,         &vGridNodes,             false);
  vAddPrimitive("grid.partitions"_Stz,    &vGridPartitions,        false);
  vAddPrimitive("grid.slots"_Stz,         &vGridSlots,             false);
  vAddPrimitive("grid.started!"_Stz,      &vGridStartedStore,      false);
  vAddPrimitive("grid.started@"_Stz,      &vGridStartedFetch,      false);
  vAddPrimitive("grid.startNode!"_Stz,    &vGridStartNodeStore,    false);
  vAddPrimitive("grid.startNode@"_Stz,    &vGridStartNodeFetch,    false);
  vAddPrimitive("grid.thisNode!"_Stz,     &vGridThisNodeStore,     false);
  vAddPrimitive("grid.thisNode@"_Stz,     &vGridThisNodeFetch,     false);
  vAddPrimitive("grid.thisNodeName@"_Stz, &vGridThisNodeNameFetch, false);
  vAddPrimitive("grid::toNode"_Stz,       &vGridToNode,            false);
  vAddPrimitive("grid::toPartition"_Stz,  &vGridToPartition,       false);
  vAddPrimitive("grid::toSlot"_Stz,       &vGridToSlot,            false);
}

/*----------------------------------------------------------------------------*/
/* grid.appendNodes -- append grid nodes into grid                            */
/*----------------------------------------------------------------------------*/
void Task::vGridAppendNodes(MP_Task mpTask)
{
  ENTRY("vGridAppendNodes"_Stz);
  
  for (i8 iNode = 0; iNode < gpAttachMap->m_pGrid->m_nNodes; iNode++) {
    Node* pNode = new Node(iNode,
                           gxArguments.m_xNodes[iNode].m_sNodeName,
                           gxArguments.m_xNodes[iNode].m_sIPAddress,
                           gxArguments.m_xNodes[iNode].m_iPortNo);
    ASSERT(CN_NODE == pNode->eClass());
    gpAttachMap->m_pGrid->vAppend(pNode);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.find -- find node from grid                                           */
/*----------------------------------------------------------------------------*/
void Task::vGridFindNode(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  s256 sPtrNodeName;
  
  if ( ! mpTask->m_pDataStack->bPop(sPtrNodeName) )
    THROW_INVALID_DSTACK("grid.find"_Stz);
  
  Node* pNode = gpAttachMap->m_pGrid->pFindNode(sPtrNodeName);
  
  mpTask->m_pDataStack->vPushObject(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.init -- initialise a grid                                             */
/*----------------------------------------------------------------------------*/
void Task::vGridInit(MP_Task mpTask)
{
ENTRY("vGridInit"_Stz);
  
  Object* pPtrGrid;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrGrid) )
    THROW_INVALID_DSTACK("grid.init"_Stz);
  
  Grid* pGrid = _pGrid(pPtrGrid);
  ASSERT(CN_GRID == pGrid->eClass());
  
  new(pGrid) Grid(gxArguments.m_nNodes, gxArguments.m_nSlots);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.node -- get node from grid                                            */
/*----------------------------------------------------------------------------*/
void Task::vGridNode(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  i8 iNode;
  
  if ( ! mpTask->m_pDataStack->bPop(iNode) )
    THROW_INVALID_DSTACK("grid.node"_Stz);
  
  mpTask->m_pDataStack->vPushObject(gpAttachMap->m_pGrid->pFindNode(iNode));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.nodes -- get number of nodes in the grid                              */
/*----------------------------------------------------------------------------*/
void Task::vGridNodes(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);

  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->m_nNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.partitions -- get number of partitions in the grid                    */
/*----------------------------------------------------------------------------*/
void Task::vGridPartitions(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->m_nPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.slots -- get number of slots of partitions per node in the grid       */
/*----------------------------------------------------------------------------*/
void Task::vGridSlots(MP_Task mpTask)
{
  ENTRY("vGridNode"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->m_nSlots);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNode! -- set this node  in the grid                               */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeStore(MP_Task mpTask)
{
  ENTRY("vGridThisNodeStore"_Stz);
  
  s256 sPtrNodeName;
  i8 iNode;
  
  Node* pThisNode = gpAttachMap->m_pGrid->pFindNode(gxArguments.m_sNodeName);
  if ( ! pThisNode ) {
    THROW("Cannot fine Node %s in the cluster"_Stz,
          gxArguments.m_sNodeName.aChar());
  }
  
  i8 iNodeNo = pThisNode->iNodeNo();
  
  gpAttachMap->m_pGrid->vThisNode(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNode@ -- get this node from the grid                              */
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
/* grid.thisNodeName@ -- get this node name from the grid                     */
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
/* grid.started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedStore(MP_Task mpTask)
{
  ENTRY("vGridSetStarted"_Stz);
  
  i8 iStarted;
  
  if ( ! mpTask->m_pDataStack->bPop(iStarted) )
    THROW_INVALID_DSTACK("grid.started!"_Stz);
  
  gpAttachMap->m_pGrid->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.startNode@ -- get grid start node                                     */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeFetch(MP_Task mpTask)
{
  ENTRY("vGridGetStarted"_Stz);
  
  mpTask->m_pDataStack->vPushInt(gpAttachMap->m_pGrid->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.startNode! -- set grid start node                                     */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeStore(MP_Task mpTask)
{
  ENTRY("vGridSetStarted"_Stz);
  
  i8 iStartNode;
  
  if ( ! mpTask->m_pDataStack->bPop(iStartNode) )
    THROW_INVALID_DSTACK("grid.startNode!"_Stz);
  
  gpAttachMap->m_pGrid->vStartNode(iStartNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started@ -- get grid started                                          */
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
  
  i8 iNodeNo = Partition::iPartitionToNode(iParitionId);
  
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
  
  i8 iPartitionId = Partition::iSlotToPartition(iSlot);
  
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
  
  i8 iSlot = Partition::iPartitionToSlot(iPartitionId);
  
  mpTask->m_pDataStack->vPushInt(iSlot);                     //! DISCUSS WITH ALKA
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
