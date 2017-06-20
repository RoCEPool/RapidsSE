/*============================================================================*/
/* Steps_NodeCursor.cpp - node cursor step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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
  vAddPrimitive("nodeCursor.begin"_Str,  &vNodeCursorBegin,  false);
  vAddPrimitive("nodeCursor.delete"_Str, &vNodeCursorDelete, false);
  vAddPrimitive("nodeCursor.first"_Str,  &vNodeCursorFirst,  false);
  vAddPrimitive("nodeCursor.get"_Str,    &vNodeCursorGet,    false);
  vAddPrimitive("nodeCursor.init"_Str,   &vNodeCursorInit,   false);
  vAddPrimitive("nodeCursor::new"_Str,    &vNodeCursorNew,    false);
  vAddPrimitive("nodeCursor.next"_Str,   &vNodeCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* nodecursor.begin -- begin using node (of grid) cursor                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorBegin(MP_Task mpTask)
{
ENTRY("vNodeCursorBegin"_Str);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.begin"_Str);
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodecursor.delete -- delete a node (of grid) cursor                        */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorDelete(MP_Task mpTask)
{
ENTRY("vNodeCursorDelete"_Str);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.delete"_Str);
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  delete pNodeCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodecursor.end -- end using node (of grid) cursor                          */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorEnd(MP_Task mpTask)
{
ENTRY("vNodeCursorEnd"_Str);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodecursor.end"_Str);
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.first -- set to first and push node (of grid)                   */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorFirst(MP_Task mpTask)
{
ENTRY("vNodeCursorFirst"_Str);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.first"_Str);
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vFirst();
  
  Object* pObject = pNodeCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.get -- push current node (of grid)                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorGet(MP_Task mpTask)
{
ENTRY("vNodeCursorGet"_Str);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.get"_Str);
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  Object* pObject = pNodeCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.init -- init a node cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorInit(MP_Task mpTask)
{
ENTRY("vNodeCursorInit"_Str);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.init"_Str);
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  new(pNodeCursor) NodeCursor(gpGrid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor::new -- new a node (of grid) cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNew(MP_Task mpTask)
{
ENTRY("vNodeCursorNew"_Str);
  
  NodeCursor* pNodeCursor = new NodeCursor(gpGrid);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  mpTask->m_pDataStack->vPushObject(pNodeCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.next -- advance to next and get node (of grid)                  */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNext(MP_Task mpTask)
{
ENTRY("vNodeCursorNext"_Str);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.next"_Str);
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vNext();
  
  Object* pObject = pNodeCursor->pObject();
  
  mpTask->m_pDataStack->vPushObject(pObject);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
