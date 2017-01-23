/*============================================================================*/
/* Task_Grid.cpp - task grid group                                            */
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
  vAddPrimitive("grid.appendNodes",&vGridAppendNodes,  false);
  vAddPrimitive("grid.init",       &vGridInit,         false);
  vAddPrimitive("grid.node",       &vGridNode,         false);
  vAddPrimitive("grid.thisNode!",  &vGridThisNodeStore,false);
  vAddPrimitive("grid.thisNode@",  &vGridThisNodeFetch,false);
  vAddPrimitive("grid.started!",   &vGridStartedStore, false);
  vAddPrimitive("grid.started@",   &vGridStartedFetch, false);
  vAddPrimitive("grid.startNode!", &vGridStartNodeStore, false);
  vAddPrimitive("grid.startNode@", &vGridStartNodeFetch, false);
}

/*----------------------------------------------------------------------------*/
/* grid.appendNodes -- append grid nodes into grid                            */
/*----------------------------------------------------------------------------*/
void Task::vGridAppendNodes(NNP_Task nnpTask)
{
  ENTRY("vGridAppendNodes");
  
  for (i8 iNode = 0; iNode < gxArguments.m_nNodes; iNode++) {
    Node* pNode = new Node(iNode,
                           gxArguments.m_xNodes[iNode].m_sNodeName.a(),
                           gxArguments.m_xNodes[iNode].m_sIPAddress.a(),
                           gxArguments.m_xNodes[iNode].m_iPortNo);
    ASSERT(CN_NODE == pNode->eClass());
    gpGrid->vAppend(pNode);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.init -- initialise a grid                                             */
/*----------------------------------------------------------------------------*/
void Task::vGridInit(NNP_Task nnpTask)
{
ENTRY("vGridInit");
  
  Object* pPtrGrid;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrGrid) )
    THROW_INVALID_DSTACK("grid.init");
  
  Grid* pGrid = _pGrid(pPtrGrid);
  ASSERT(CN_GRID == pGrid->eClass());
  
  new(pGrid) Grid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.node -- get node from grid                                            */
/*----------------------------------------------------------------------------*/
void Task::vGridNode(NNP_Task nnpTask)
{
  ENTRY("vGridNode");
  
  i8 iNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(iNode) )
    THROW_INVALID_DSTACK("grid.node");
  
  nnpTask->m_pDataStack->vPush(gpGrid->pFindNode(iNode));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNode! -- set this node  in the grid                               */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeStore(NNP_Task nnpTask)
{
  ENTRY("vGridThisNodeStore");
  
  s256 sPtrNodeName;
  i8 iNode;
  
  Node* pThisNode = gpGrid->pFindNode(gxArguments.m_sNode.a());
  if ( ! pThisNode ) {
    THROW("Cannot fine Node %s in the cluster", gxArguments.m_sNode.a());
  }
  
  gpGrid->vThisNode(pThisNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.thisNode@ -- get this node from the grid                              */
/*----------------------------------------------------------------------------*/
void Task::vGridThisNodeFetch(NNP_Task nnpTask)
{
  ENTRY("vGridThisNodeFetch");
  
  Object* pPtrGrid;
  
  nnpTask->m_pDataStack->vPush(gpGrid->iThisNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedStore(NNP_Task nnpTask)
{
  ENTRY("vGridSetStarted");
  
  i8 iStarted;
  
  if ( ! nnpTask->m_pDataStack->bPop(iStarted) )
    THROW_INVALID_DSTACK("grid.started!");
  
  gpGrid->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.startNode@ -- get grid start node                                     */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeFetch(NNP_Task nnpTask)
{
  ENTRY("vGridGetStarted");
  
  nnpTask->m_pDataStack->vPush(gpGrid->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.startNode! -- set grid start node                                     */
/*----------------------------------------------------------------------------*/
void Task::vGridStartNodeStore(NNP_Task nnpTask)
{
  ENTRY("vGridSetStarted");
  
  i8 iStartNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(iStartNode) )
    THROW_INVALID_DSTACK("grid.startNode!");
  
  gpGrid->vStartNode(iStartNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started@ -- get grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedFetch(NNP_Task nnpTask)
{
  ENTRY("vGridGetStarted");
  
  nnpTask->m_pDataStack->vPush(gpGrid->bHasStarted());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
