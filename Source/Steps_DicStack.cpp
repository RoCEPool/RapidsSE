/*============================================================================*/
/* Steps_DicStack.cpp - dic stack step group                                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DicStack(void)
{
  vAddStep("dicStack.depth"_Stz,     &Step::vDicStackDepth);
  vAddStep("dicStack.drop"_Stz,      &Step::vDicStackDrop);
  vAddStep("dicStack.dump"_Stz,      &Step::vDicStackDump);
  vAddStep("dicStack.pick"_Stz,      &Step::vDicStackPick);
  vAddStep("dicStack.pop"_Stz,       &Step::vDicStackPop);
  vAddStep("dicStack.push"_Stz,      &Step::vDicStackPush);
  vAddStep("dicStack.toPathStr"_Stz, &Step::vDicStackToPathStr);
}

/*----------------------------------------------------------------------------*/
/* dicStack.depth -- get number of dictionaries on dictionary stack           */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDepth(void)
{
  ENTRY("vDicStackDepth"_Stz);
  
  Object* pPtrDicStack;
  if ( ! m_pDataStack->bPopObject(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = _pDicStack(pPtrDicStack);

  i8 iDepth = pDicStack->iCount();
  
  m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.drop -- discard top of dictionary stack                           */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDrop(void)
{
  ENTRY("vDicStackDrop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! m_pDataStack->bPopObject(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = _pDicStack(pPtrDicStack);

  pDicStack->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.dump -- dump dictionary stack                                     */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDump(void)
{
  ENTRY("vDicStackDump"_Stz);
  
  Object* pPtrDicStack;
  if ( ! m_pDataStack->bPopObject(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = _pDicStack(pPtrDicStack);

  pDicStack->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pick -- pick a dic from dic stack and push onto data stack        */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPick(void)
{
  ENTRY("vDicStackPick"_Stz);
  
  Object* pPtrDicStack;
  if ( ! m_pDataStack->bPopObject(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDic;
  if ( ! m_pDataStack->bPopI8(iDic) )
    THROW_DSTACK();
  
  Object* pPtrDic = pDicStack->pPick(iDic);

  m_pDataStack->vPushPointer(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pop -- pop top of dictionary stack                                */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPop(void)
{
  ENTRY("vDicStackPop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! m_pDataStack->bPopObject(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = _pDicStack(pPtrDicStack);

  pDicStack->vPop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.push -- push a dictionary onto given dictionary stack             */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPush(void)
{
  ENTRY("vDicStackPush"_Stz);
  
  Object* pPtrDicStack;
  if ( ! m_pDataStack->bPopObject(pPtrDicStack) )
    THROW_DSTACK();
  
  DicStack* pDicStack = _pDicStack(pPtrDicStack);

  Object* pPtrDic;
  
  if ( ! m_pDataStack->bPopObject(pPtrDic) )
    THROW_DSTACK();
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicStack->vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.toPathStr -- convert dictionary stack to path string              */
/*----------------------------------------------------------------------------*/
void Step::vDicStackToPathStr(void)
{
  ENTRY("vDicStackToPathStr"_Stz);
  
  Object* pPtrDicStack;
  if ( ! m_pDataStack->bPopObject(pPtrDicStack) )
    THROW_DSTACK();
  DicStack* pDicStack = _pDicStack(pPtrDicStack);
  
  s256 sPath;
  
  pDicStack->vToPathStr(sPath);
  
  m_pDataStack->vPushS256(sPath);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
