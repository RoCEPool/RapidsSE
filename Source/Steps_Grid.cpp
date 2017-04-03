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
void Task::vGridAppendNodes(NNP_Task nnpTask)
{
  ENTRY("vGridAppendNodes"_Str);
  
  for (i8 iNode = 0; iNode < gxArguments.m_nNodes; iNode++) {
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
void Task::vGridFindNode(NNP_Task nnpTask)
{
  ENTRY("vGridNode"_Str);
  
  s256 sPtrNodeName;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPtrNodeName) )
    THROW_INVALID_DSTACK("grid.find"_Str);
  
  Node* pNode = gpGrid->pFindNode(sPtrNodeName);
  
  nnpTask->m_pDataStack->vPush(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.init -- initialise a grid                                             */
/*----------------------------------------------------------------------------*/
void Task::vGridInit(NNP_Task nnpTask)
{
ENTRY("vGridInit"_Str);
  
  Object* pPtrGrid;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrGrid) )
    THROW_INVALID_DSTACK("grid.init"_Str);
  
  Grid* pGrid = _pGrid(pPtrGrid);
  ASSERT(CN_GRID == pGrid->eClass());
  
  new(pGrid) Grid(gxArguments.m_nNodes, gxArguments.m_nSlots);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.node -- get node from grid                                            */
/*----------------------------------------------------------------------------*/
void Task::vGridNode(NNP_Task nnpTask)
{
  ENTRY("vGridNode"_Str);
  
  i8 iNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(iNode) )
    THROW_INVALID_DSTACK("grid.node"_Str);
  
  nnpTask->m_pDataStack->vPush(gpGrid->pFindNode(iNode));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.nodes -- get number of nodes in the grid                              */
/*----------------------------------------------------------------------------*/
void Task::vGridNodes(NNP_Task nnpTask)
{
  ENTRY("vGridNode"_Str);

  nnpTask->m_pDataStack->vPush(gpGrid->m_nNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.partitions -- get number of partitions in the grid                    */
/*----------------------------------------------------------------------------*/
void Task::vGridPartitions(NNP_Task nnpTask)
{
  ENTRY("vGridNode"_Str);
  
  nnpTask->m_pDataStack->vPush(gpGrid->m_nPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.slots -- get number of slots of partitions per node in the grid       */
/*----------------------------------------------------------------------------*/
void Task::vGridSlots(NNP_Task nnpTask)
{
  ENTRY("vGridNode"_Str);
  
  nnpTask->m_pDataStack->vPush(gpGrid->m_nSlots);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNode! -- set this node  in the grid                               */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeStore(NNP_Task nnpTask)
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
void Task::vGridThisNodeFetch(NNP_Task nnpTask)
{
  ENTRY("vGridThisNodeFetch"_Str);
  
  Object* pPtrGrid;
  
  i8 iNodeNo = gpGrid->iThisNode();
  
  nnpTask->m_pDataStack->vPush(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNodeName@ -- get this node name from the grid                     */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeNameFetch(NNP_Task nnpTask)
{
  ENTRY("vGridThisNodeNameFetch"_Str);
  
  s256 sName;
  sName = gpGrid->pThisNode()->aNodeName();
  
  nnpTask->m_pDataStack->vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedStore(NNP_Task nnpTask)
{
  ENTRY("vGridSetStarted"_Str);
  
  i8 iStarted;
  
  if ( ! nnpTask->m_pDataStack->bPop(iStarted) )
    THROW_INVALID_DSTACK("grid.started!"_Str);
  
  gpGrid->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.startNode@ -- get grid start node                                     */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeFetch(NNP_Task nnpTask)
{
  ENTRY("vGridGetStarted"_Str);
  
  nnpTask->m_pDataStack->vPush(gpGrid->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.startNode! -- set grid start node                                     */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeStore(NNP_Task nnpTask)
{
  ENTRY("vGridSetStarted"_Str);
  
  i8 iStartNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(iStartNode) )
    THROW_INVALID_DSTACK("grid.startNode!"_Str);
  
  gpGrid->vStartNode(iStartNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started@ -- get grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedFetch(NNP_Task nnpTask)
{
  ENTRY("vGridGetStarted"_Str);
  
  nnpTask->m_pDataStack->vPush(gpGrid->bHasStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toNode -- convert partition id to node no                            */
/*----------------------------------------------------------------------------*/
void Task::vGridToNode(NNP_Task nnpTask)
{
  ENTRY("vGridToNode"_Str);
  
  i8 iParitionId;
  if ( ! nnpTask->m_pDataStack->bPop(iParitionId) )
    THROW_INVALID_DSTACK("grid::toNode"_Str);
  
  // for the moment its always node 0                      //! DISCUSS WITH ALKA
  //nnpTask->m_pDataStack->vPush((i8)0);
  
  i8 iNodeNo = Partition::iPartitionToNode(iParitionId);
  
  nnpTask->m_pDataStack->vPush(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toPartition -- convert slot to partitionId                           */
/*----------------------------------------------------------------------------*/
void Task::vGridToPartition(NNP_Task nnpTask)
{
  ENTRY("vGridToPartition"_Str);
  
  i8 iSlot;
  if ( ! nnpTask->m_pDataStack->bPop(iSlot) )
    THROW_INVALID_DSTACK("grid::toPartition"_Str);
  
  i8 iPartitionId = Partition::iSlotToPartition(iSlot);
  
  nnpTask->m_pDataStack->vPush(iPartitionId);              //! DISCUSS WITH ALKA
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toSlot -- convert partitionId to slot                                */
/*----------------------------------------------------------------------------*/
void Task::vGridToSlot(NNP_Task nnpTask)
{
  ENTRY("vGridToSlot"_Str);
  
  i8 iPartitionId;
  if ( ! nnpTask->m_pDataStack->bPop(iPartitionId) )
    THROW_INVALID_DSTACK("grid::toSlot"_Str);
  
  i8 iSlot = Partition::iPartitionToSlot(iPartitionId);
  
  nnpTask->m_pDataStack->vPush(iSlot);                     //! DISCUSS WITH ALKA
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
