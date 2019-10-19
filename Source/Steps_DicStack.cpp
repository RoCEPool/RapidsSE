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
void Step::vDicStackDepth(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicStackDepth"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicStack) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDepth = pDicStack->iCount();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.drop -- discard top of dictionary stack                           */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDrop(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicStackDrop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicStack) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.dump -- dump dictionary stack                                     */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDump(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicStackDump"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicStack) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pick -- pick a dic from dic stack and push onto data stack        */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPick(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicStackPick"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicStack) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDic;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Object* pPtrDic = pDicStack->pPick(iDic);

  mpTask->m_pForth->m_pDataStack->vPushPointer(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pop -- pop top of dictionary stack                                */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPop(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicStackPop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicStack) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vPop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.push -- push a dictionary onto given dictionary stack             */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPush(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicStackPush"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicStack) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  DicStack* pDicStack = DicStack::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicStack->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
