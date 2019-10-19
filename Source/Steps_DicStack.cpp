/*============================================================================*/
/* Steps_DicStack.cpp - dic stack step group                                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DicStack(void)
{
  vAddStep("dicStack.depth"_Stz,    &Step::vDicStackDepth);
  vAddStep("dicStack.drop"_Stz,     &Step::vDicStackDrop);
  vAddStep("dicStack.dump"_Stz,     &Step::vDicStackDump);
  vAddStep("dicStack.pick"_Stz,     &Step::vDicStackPick);
  vAddStep("dicStack.pop"_Stz,      &Step::vDicStackPop);
  vAddStep("dicStack.push"_Stz,     &Step::vDicStackPush);
}

/*----------------------------------------------------------------------------*/
/* dicStack.depth -- get number of dictionaries on dictionary stack           */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDepth(Forth* pForth)
{
  ENTRY("vDicStackDepth"_Stz);
  
  Object* pPtrDicStack;
  if ( ! pForth->m_pDataStack->bPop(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDepth = pDicStack->iCount();
  
  pForth->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.drop -- discard top of dictionary stack                           */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDrop(Forth* pForth)
{
  ENTRY("vDicStackDrop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! pForth->m_pDataStack->bPop(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.dump -- dump dictionary stack                                     */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDump(Forth* pForth)
{
  ENTRY("vDicStackDump"_Stz);
  
  Object* pPtrDicStack;
  if ( ! pForth->m_pDataStack->bPop(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pick -- pick a dic from dic stack and push onto data stack        */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPick(Forth* pForth)
{
  ENTRY("vDicStackPick"_Stz);
  
  Object* pPtrDicStack;
  if ( ! pForth->m_pDataStack->bPop(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDic;
  if ( ! pForth->m_pDataStack->bPop(iDic) )
    THROW_DSTACK();
  
  Object* pPtrDic = pDicStack->pPick(iDic);

  pForth->m_pDataStack->vPushPointer(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pop -- pop top of dictionary stack                                */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPop(Forth* pForth)
{
  ENTRY("vDicStackPop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! pForth->m_pDataStack->bPop(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vPop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.push -- push a dictionary onto given dictionary stack             */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPush(Forth* pForth)
{
  ENTRY("vDicStackPush"_Stz);
  
  Object* pPtrDicStack;
  if ( ! pForth->m_pDataStack->bPop(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = DicStack::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  Object* pPtrDic;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDic) )
    THROW_DSTACK();
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicStack->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
