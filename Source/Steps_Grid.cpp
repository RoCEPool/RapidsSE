/*============================================================================*/
/* Steps_Grid.cpp - grid step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// grid: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Grid(void)
{
  vAddPrimitive("grid.appendNodes"_Str,   &vGridAppendNodes,       false);
  vAddPrimitive("grid.find"_Str,          &vGridFindNode,          false);
  vAddPrimitive("grid.init"_Str,          &vGridInit,              false);
  vAddPrimitive("grid.node"_Str,          &vGridNode,              false);
  vAddPrimitive("grid.nodes"_Str,         &vGridNodes,             false);
  vAddPrimitive("grid.partitions"_Str,    &vGridPartitions,        false);
  vAddPrimitive("grid.slots"_Str,         &vGridSlots,             false);
  vAddPrimitive("grid.started!"_Str,      &vGridStartedStore,      false);
  vAddPrimitive("grid.started@"_Str,      &vGridStartedFetch,      false);
  vAddPrimitive("grid.startNode!"_Str,    &vGridStartNodeStore,    false);
  vAddPrimitive("grid.startNode@"_Str,    &vGridStartNodeFetch,    false);
  vAddPrimitive("grid.thisNode!"_Str,     &vGridThisNodeStore,     false);
  vAddPrimitive("grid.thisNode@"_Str,     &vGridThisNodeFetch,     false);
  vAddPrimitive("grid.thisNodeName@"_Str, &vGridThisNodeNameFetch, false);
  vAddPrimitive("grid::toNode"_Str,       &vGridToNode,            false);
  vAddPrimitive("grid::toPartition"_Str,  &vGridToPartition,       false);
  vAddPrimitive("grid::toSlot"_Str,       &vGridToSlot,            false);
}

/*----------------------------------------------------------------------------*/
/* grid.appendNodes -- append grid nodes into grid                            */
/*----------------------------------------------------------------------------*/
void Task::vGridAppendNodes(MP_Task mpTask)
{
  ENTRY("vGridAppendNodes"_Str);
  
  for (i8 iNode = 0; iNode < gpGrid->m_nNodes; iNode++) {
    Node* pNode = new Node(iNode,
                           gxArguments.m_xNodes[iNode].m_sNodeName,
                           gxArguments.m_xNodes[iNode].m_sIPAddress,
                           gxArguments.m_xNodes[iNode].m_iPortNo);
    ASSERT(CN_NODE == pNode->eClass());
    gpGrid->vAppend(pNode);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.find -- find node from grid                                           */
/*----------------------------------------------------------------------------*/
void Task::vGridFindNode(MP_Task mpTask)
{
  ENTRY("vGridNode"_Str);
  
  s256 sPtrNodeName;
  
  if ( ! mpTask->m_pDataStack->bPop(sPtrNodeName) )
    THROW_INVALID_DSTACK("grid.find"_Str);
  
  Node* pNode = gpGrid->pFindNode(sPtrNodeName);
  
  mpTask->m_pDataStack->vPushObject(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.init -- initialise a grid                                             */
/*----------------------------------------------------------------------------*/
void Task::vGridInit(MP_Task mpTask)
{
ENTRY("vGridInit"_Str);
  
  Object* pPtrGrid;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrGrid) )
    THROW_INVALID_DSTACK("grid.init"_Str);
  
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
  ENTRY("vGridNode"_Str);
  
  i8 iNode;
  
  if ( ! mpTask->m_pDataStack->bPop(iNode) )
    THROW_INVALID_DSTACK("grid.node"_Str);
  
  mpTask->m_pDataStack->vPushObject(gpGrid->pFindNode(iNode));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.nodes -- get number of nodes in the grid                              */
/*----------------------------------------------------------------------------*/
void Task::vGridNodes(MP_Task mpTask)
{
  ENTRY("vGridNode"_Str);

  mpTask->m_pDataStack->vPushInt(gpGrid->m_nNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.partitions -- get number of partitions in the grid                    */
/*----------------------------------------------------------------------------*/
void Task::vGridPartitions(MP_Task mpTask)
{
  ENTRY("vGridNode"_Str);
  
  mpTask->m_pDataStack->vPushInt(gpGrid->m_nPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.slots -- get number of slots of partitions per node in the grid       */
/*----------------------------------------------------------------------------*/
void Task::vGridSlots(MP_Task mpTask)
{
  ENTRY("vGridNode"_Str);
  
  mpTask->m_pDataStack->vPushInt(gpGrid->m_nSlots);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNode! -- set this node  in the grid                               */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeStore(MP_Task mpTask)
{
  ENTRY("vGridThisNodeStore"_Str);
  
  s256 sPtrNodeName;
  i8 iNode;
  
  Node* pThisNode = gpGrid->pFindNode(gxArguments.m_sNodeName);
  if ( ! pThisNode ) {
    THROW("Cannot fine Node %s in the cluster"_Str,
          gxArguments.m_sNodeName.s());
  }
  
  i8 iNodeNo = pThisNode->iNodeNo();
  
  gpGrid->vThisNode(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNode@ -- get this node from the grid                              */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeFetch(MP_Task mpTask)
{
  ENTRY("vGridThisNodeFetch"_Str);
  
  Object* pPtrGrid;
  
  i8 iNodeNo = gpGrid->iThisNode();
  
  mpTask->m_pDataStack->vPushInt(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNodeName@ -- get this node name from the grid                     */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeNameFetch(MP_Task mpTask)
{
  ENTRY("vGridThisNodeNameFetch"_Str);
  
  s256 sName;
  sName = gpGrid->pThisNode()->aNodeName();
  
  mpTask->m_pDataStack->vPushS256(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedStore(MP_Task mpTask)
{
  ENTRY("vGridSetStarted"_Str);
  
  i8 iStarted;
  
  if ( ! mpTask->m_pDataStack->bPop(iStarted) )
    THROW_INVALID_DSTACK("grid.started!"_Str);
  
  gpGrid->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.startNode@ -- get grid start node                                     */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeFetch(MP_Task mpTask)
{
  ENTRY("vGridGetStarted"_Str);
  
  mpTask->m_pDataStack->vPushInt(gpGrid->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.startNode! -- set grid start node                                     */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeStore(MP_Task mpTask)
{
  ENTRY("vGridSetStarted"_Str);
  
  i8 iStartNode;
  
  if ( ! mpTask->m_pDataStack->bPop(iStartNode) )
    THROW_INVALID_DSTACK("grid.startNode!"_Str);
  
  gpGrid->vStartNode(iStartNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started@ -- get grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedFetch(MP_Task mpTask)
{
  ENTRY("vGridGetStarted"_Str);
  
  mpTask->m_pDataStack->vPushInt(gpGrid->bHasStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toNode -- convert partition id to node no                            */
/*----------------------------------------------------------------------------*/
void Task::vGridToNode(MP_Task mpTask)
{
  ENTRY("vGridToNode"_Str);
  
  i8 iParitionId;
  if ( ! mpTask->m_pDataStack->bPop(iParitionId) )
    THROW_INVALID_DSTACK("grid::toNode"_Str);
  
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
  ENTRY("vGridToPartition"_Str);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("grid::toPartition"_Str);
  
  i8 iPartitionId = Partition::iSlotToPartition(iSlot);
  
  mpTask->m_pDataStack->vPushInt(iPartitionId);              //! DISCUSS WITH ALKA
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toSlot -- convert partitionId to slot                                */
/*----------------------------------------------------------------------------*/
void Task::vGridToSlot(MP_Task mpTask)
{
  ENTRY("vGridToSlot"_Str);
  
  i8 iPartitionId;
  if ( ! mpTask->m_pDataStack->bPop(iPartitionId) )
    THROW_INVALID_DSTACK("grid::toSlot"_Str);
  
  i8 iSlot = Partition::iPartitionToSlot(iPartitionId);
  
  mpTask->m_pDataStack->vPushInt(iSlot);                     //! DISCUSS WITH ALKA
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
