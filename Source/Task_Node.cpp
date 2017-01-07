/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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

// node: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Node(void)
{
  vAddPrimitive("node.init",       &vNodeInit,         false);
  vAddPrimitive("node.new",        &vNodeNew,          false);
  vAddPrimitive("node.no",         &vNodeNo,           false);
  vAddPrimitive("node.status",     &vNodeStatus,       false);
}

/*----------------------------------------------------------------------------*/
/* node.init -- initialise a node (of a grid)                                 */
/*----------------------------------------------------------------------------*/
void Task::vNodeInit(NNP_Task nnpTask)
{
ENTRY("vNodeInit");
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.init");
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  new(pNode) Node;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.new -- new a node (of a grid)                                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeNew(NNP_Task nnpTask)
{
ENTRY("vNodeNew");
  
  i8 iNodeNo;
  
  if ( ! nnpTask->m_xDataStack.bPop(iNodeNo) )
    THROW_INVALID_DSTACK("node.new");
  
  Node* pNode = new Node;
  ASSERT(CN_NODE == pNode->eClass());

  pNode->vNodeNo(iNodeNo);
  
  nnpTask->m_xDataStack.vPush(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Task::vNodeNo(NNP_Task nnpTask)
{
ENTRY("vNodeNo");
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.no");
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  nnpTask->m_xDataStack.vPush(pNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.status -- get status of this node (of a grid)                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeStatus(NNP_Task nnpTask)
{
  ENTRY("vNodeStatus");

  s256 s;
  
  if (Region::bTest(gxArguments.m_sRegionName))
    String::vSprint(s.a(),"RapidsSE instance %s running", gxArguments.m_sRegionName.a());
  else
    String::vSprint(s.a(),"RapidsSE instance %s not running",gxArguments.m_sRegionName.a());
  
  nnpTask->m_xDataStack.vPush(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
