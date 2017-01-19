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
  vAddPrimitive("grid.appendNode", &vGridAppendNode,   false);
  vAddPrimitive("grid.appendNodes",&vGridAppendNodes,  false);
  vAddPrimitive("grid.node!",      &vGridNodeStore,    false);
  vAddPrimitive("grid.node@",      &vGridNodeFetch,    false);
  vAddPrimitive("grid.nodeName",   &vGridNodeName,    false);
  vAddPrimitive("grid.started!",   &vGridStartedStore, false);
  vAddPrimitive("grid.started@",   &vGridStartedFetch, false);
  vAddPrimitive("grid.init",       &vGridInit,         false);
}

/*----------------------------------------------------------------------------*/
/* grid.appendNode -- append grid node into grid                              */
/*----------------------------------------------------------------------------*/
void Task::vGridAppendNode(NNP_Task nnpTask)
{
ENTRY("vGridAppendNode");
 
  Object* pPtrNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("grid.addNode");

  Node* pNode = _pNode(pPtrNode);
  ASSERT(pNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  gpGrid->vAppend(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.appendNodes -- append grid nodes into grid                            */
/*----------------------------------------------------------------------------*/
void Task::vGridAppendNodes(NNP_Task nnpTask)
{
  ENTRY("vGridAppendNodes");
  
  for (i8 iNode = 0; iNode < gxArguments.m_nNodes; iNode++) {
    Node* pNode = new Node(iNode,
                           gxArguments.Nodes[iNode].m_sNodeName.a(),
                           gxArguments.Nodes[iNode].m_sIpAddress.a(),
                           gxArguments.Nodes[iNode].m_iPortNo);
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
/* grid.node! -- set grid node                                                */
/*----------------------------------------------------------------------------*/
void Task::vGridNodeStore(NNP_Task nnpTask)
{
  ENTRY("vGridNodeStore");
  
  s256 sPtrNodeName;
  i8 iNode;
  
  //if ( ! nnpTask->m_pDataStack->bPop(sPtrNodeName) )
  //  THROW_INVALID_DSTACK("grid.node!");
    
  gpGrid->vNode(gpGrid->pFindNode(gxArguments.m_sNode.a())->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.node@ -- get grid node                                                */
/*----------------------------------------------------------------------------*/
void Task::vGridNodeFetch(NNP_Task nnpTask)
{
  ENTRY("vGridNodeFetch");
  
  Object* pPtrGrid;
  
  //if ( ! nnpTask->m_pDataStack->bPop(pPtrGrid) )
  //  THROW_INVALID_DSTACK("grid.node@");
  
  //Grid* pGrid = _pGrid(pPtrGrid);
  //ASSERT(CN_GRID == pGrid->eClass());
  
  nnpTask->m_pDataStack->vPush(gpGrid->iNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.nodename -- get grid node name                                        */
/*----------------------------------------------------------------------------*/
void Task::vGridNodeName(NNP_Task nnpTask)
{
  ENTRY("vGridNodeName");
  
  i8 iNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(iNode) )
    THROW_INVALID_DSTACK("grid.nodename");
  
  //Grid* pGrid = _pGrid(pPtrGrid);
  //ASSERT(CN_GRID == pGrid->eClass());
  
  s256 sName;
  sName = gpGrid->pFindNode(iNode)->pName();
  
  nnpTask->m_pDataStack->vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedStore(NNP_Task nnpTask)
{
  ENTRY("vGridSetStarted");
  
  Object* pPtrGrid;
  i8 iStarted;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrGrid) )
    THROW_INVALID_DSTACK("grid.started!");
  
  Grid* pGrid = _pGrid(pPtrGrid);
  ASSERT(CN_GRID == pGrid->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(iStarted) )
    THROW_INVALID_DSTACK("grid.started!");
  
  pGrid->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started@ -- get grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedFetch(NNP_Task nnpTask)
{
  ENTRY("vGridGetStarted");
  
  Object* pPtrGrid;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrGrid) )
    THROW_INVALID_DSTACK("grid.started@");
  
  Grid* pGrid = _pGrid(pPtrGrid);
  ASSERT(CN_GRID == pGrid->eClass());
  
  nnpTask->m_pDataStack->vPush(pGrid->bHasStarted());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
