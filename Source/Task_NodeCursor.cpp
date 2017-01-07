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

// nodecursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_NodeCursor(void)
{
  vAddPrimitive("nodeCursor.begin",  &vNodeCursorBegin,  false);
  vAddPrimitive("nodeCursor.delete", &vNodeCursorDelete, false);
  vAddPrimitive("nodeCursor.first",  &vNodeCursorFirst,  false);
  vAddPrimitive("nodeCursor.get",    &vNodeCursorGet,    false);
  vAddPrimitive("nodeCursor.init",   &vNodeCursorInit,   false);
  vAddPrimitive("nodeCursor.new",    &vNodeCursorNew,    false);
  vAddPrimitive("nodeCursor.next",   &vNodeCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* nodecursor.begin -- begin using node (of grid) cursor                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorBegin(NNP_Task nnpTask)
{
ENTRY("vNodeCursorBegin");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.begin");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodecursor.delete -- delete a node (of grid) cursor                        */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorDelete(NNP_Task nnpTask)
{
ENTRY("vNodeCursorDelete");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.delete");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  delete pNodeCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodecursor.end -- end using node (of grid) cursor                          */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorEnd(NNP_Task nnpTask)
{
ENTRY("vNodeCursorEnd");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodecursor.end");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.first -- set to first and push node (of grid)                   */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorFirst(NNP_Task nnpTask)
{
ENTRY("vNodeCursorFirst");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.first");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vFirst();
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.get -- push current node (of grid)                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorGet(NNP_Task nnpTask)
{
ENTRY("vNodeCursorGet");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.get");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.init -- init a node cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorInit(NNP_Task nnpTask)
{
ENTRY("vNodeCursorInit");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.init");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  new(pNodeCursor) NodeCursor(gpGrid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.new -- new a node (of grid) cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNew(NNP_Task nnpTask)
{
ENTRY("vNodeCursorNew");
  
  NodeCursor* pNodeCursor = new NodeCursor(gpGrid);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  nnpTask->m_xDataStack.vPush(pNodeCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.next -- advance to next and get node (of grid)                  */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNext(NNP_Task nnpTask)
{
ENTRY("vNodeCursorNext");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.next");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vNext();
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xDataStack.vPush(pObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
