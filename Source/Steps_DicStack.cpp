/*============================================================================*/
/* Steps_DicStack.cpp - dic stack step group                                  */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// dicstack: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_DicStack(void)
{
  vAddPrimitive("dicPath.depth"_Stz,    &vDicStackDepth,    false);
  vAddPrimitive("dicPath.drop"_Stz,     &vDicStackDrop,     false);
  vAddPrimitive("dicPath.dump"_Stz,     &vDicStackDump,     false);
  vAddPrimitive("dicPath.pick"_Stz,     &vDicStackPick,     false);
  vAddPrimitive("dicPath.pop"_Stz,      &vDicStackPop,      false);
  vAddPrimitive("dicPath.push"_Stz,     &vDicStackPush,     false);
}

/*----------------------------------------------------------------------------*/
/* dicPath.depth -- get number of dictionaries on dictionary stack            */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDepth(MP_Task mpTask)
{
  ENTRY("vDicStackDepth"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.depth"_Stz);
  
  DicStack* pDicPath = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicPath->eClass());
  
  i8 iDepth = pDicPath->iCount();
  
  mpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.drop -- discard top of dictionary stack                            */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDrop(MP_Task mpTask)
{
  ENTRY("vDicStackDrop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.drop"_Stz);
  
  DicStack* pDicPath = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicPath->eClass());
  
  pDicPath->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.dump -- dump dictionary stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDump(MP_Task mpTask)
{
  ENTRY("vDicStackDump"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.dump"_Stz);
  
  DicStack* pDicPath = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicPath->eClass());
  
  pDicPath->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pick -- pick a dic from dic stack and push onto data stack         */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPick(MP_Task mpTask)
{
  ENTRY("vDicStackPick"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.pick"_Stz);
  
  DicStack* pDicPath = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicPath->eClass());
  
  i8 iDic;
  if ( ! mpTask->m_pDataStack->bPop(iDic) )
    THROW_INVALID_DSTACK("dicPath.pick"_Stz);
  
  Object* pPtrDic = pDicPath->pPick(iDic);

  mpTask->m_pDataStack->vPushObject(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pop -- pop top of dictionary stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPop(MP_Task mpTask)
{
  ENTRY("vDicStackPop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.pop"_Stz);
  
  DicStack* pDicPath = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicPath->eClass());
  
  pDicPath->vPop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.push -- push a dictionary onto given dictionary stack              */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPush(MP_Task mpTask)
{
  ENTRY("vDicStackPush"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.push"_Stz);
  
  DicStack* pDicPath = DicStack::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicPath->eClass());
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicPath.push"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicPath->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
