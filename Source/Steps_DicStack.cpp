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
void Task::vAddSteps_DicStack(void)
{
  vAddStep("dicStack.depth"_Stz,    &vDicStackDepth,    false);
  vAddStep("dicStack.drop"_Stz,     &vDicStackDrop,     false);
  vAddStep("dicStack.dump"_Stz,     &vDicStackDump,     false);
  vAddStep("dicStack.pick"_Stz,     &vDicStackPick,     false);
  vAddStep("dicStack.pop"_Stz,      &vDicStackPop,      false);
  vAddStep("dicStack.push"_Stz,     &vDicStackPush,     false);
}

/*----------------------------------------------------------------------------*/
/* dicStack.depth -- get number of dictionaries on dictionary stack           */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDepth(MP_Task mpTask)
{
  ENTRY("vDicStackDepth"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.depth"_Stz);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDepth = pDicStack->iCount();
  
  mpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.drop -- discard top of dictionary stack                           */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDrop(MP_Task mpTask)
{
  ENTRY("vDicStackDrop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.drop"_Stz);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.dump -- dump dictionary stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDump(MP_Task mpTask)
{
  ENTRY("vDicStackDump"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.dump"_Stz);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pick -- pick a dic from dic stack and push onto data stack        */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPick(MP_Task mpTask)
{
  ENTRY("vDicStackPick"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.pick"_Stz);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDic;
  if ( ! mpTask->m_pDataStack->bPop(iDic) )
    THROW_INVALID_DSTACK("dicStack.pick"_Stz);
  
  Object* pPtrDic = pDicStack->pPick(iDic);

  mpTask->m_pDataStack->vPushObject(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pop -- pop top of dictionary stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPop(MP_Task mpTask)
{
  ENTRY("vDicStackPop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.pop"_Stz);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vPop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.push -- push a dictionary onto given dictionary stack             */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPush(MP_Task mpTask)
{
  ENTRY("vDicStackPush"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.push"_Stz);
  
  DicStack* pDicStack = DicStack::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicStack.push"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicStack->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
