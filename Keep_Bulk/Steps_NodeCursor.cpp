/*============================================================================*/
/* Steps_NodeCursor.cpp - node cursor step group                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// nodecursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_NodeCursor(void)
{
  vAddPrimitive("nodeCursor.begin"_Stz,  &vNodeCursorBegin,  false);
  vAddPrimitive("nodeCursor.delete"_Stz, &vNodeCursorDelete, false);
  vAddPrimitive("nodeCursor.first"_Stz,  &vNodeCursorFirst,  false);
  vAddPrimitive("nodeCursor.get"_Stz,    &vNodeCursorGet,    false);
  vAddPrimitive("nodeCursor.init"_Stz,   &vNodeCursorInit,   false);
  vAddPrimitive("nodeCursor::new"_Stz,    &vNodeCursorNew,    false);
  vAddPrimitive("nodeCursor.next"_Stz,   &vNodeCursorNext,   false);
}

/*----------------------------------------------------------------------------*/
/* nodecursor.begin -- begin using node (of grid) cursor                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorBegin(MP_Task mpTask)
{
ENTRY("vNodeCursorBegin"_Stz);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.begin"_Stz);
  
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
ENTRY("vNodeCursorDelete"_Stz);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.delete"_Stz);
  
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
ENTRY("vNodeCursorEnd"_Stz);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodecursor.end"_Stz);
  
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
ENTRY("vNodeCursorFirst"_Stz);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.first"_Stz);
  
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
ENTRY("vNodeCursorGet"_Stz);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.get"_Stz);
  
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
ENTRY("vNodeCursorInit"_Stz);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.init"_Stz);
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  new(pNodeCursor) NodeCursor(gpAttachMap->m_pGrid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor::new -- new a node (of grid) cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNew(MP_Task mpTask)
{
ENTRY("vNodeCursorNew"_Stz);
  
  NodeCursor* pNodeCursor = new NodeCursor(gpAttachMap->m_pGrid);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  mpTask->m_pDataStack->vPushObject(pNodeCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.next -- advance to next and get node (of grid)                  */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNext(MP_Task mpTask)
{
ENTRY("vNodeCursorNext"_Stz);
  
  Object* pPtrNodeCursor;
  if ( ! mpTask->m_pDataStack->bPop(pPtrNodeCursor) )
    THROW_INVALID_DSTACK("nodeCursor.next"_Stz);
  
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
