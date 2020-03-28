/*============================================================================*/
/* Steps_RegPath.cpp - path[] step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegPath(void)
{
  vAddStep("path[]"_Stz,             &Step::vRegPathGet);
  
  vAddStep("dicStack.depth"_Stz,     &Step::vDicStackDepth);
  vAddStep("dicStack.discard"_Stz,   &Step::vDicStackDiscard);
  vAddStep("dicStack.dump"_Stz,      &Step::vDicStackDump);
  vAddStep("dicStack.pick"_Stz,      &Step::vDicStackPick);
  vAddStep("dicStack.push"_Stz,      &Step::vDicStackPush);
  vAddStep("dicStack.toPathStr"_Stz, &Step::vDicStackToPathStr);

}

/*----------------------------------------------------------------------------*/
/* path[] -- push task dictionary onto data stack                             */
/*----------------------------------------------------------------------------*/
void Step::vRegPathGet(void)
{
  ENTRY(); // ENTRY("vRegPathGet"_Stz);
    
  Object* pPtrDicStack = &m_oDicStack;
  
  m_pDataStack->vPushPtr(pPtrDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.depth -- get number of dictionaries on dictionary stack           */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDepth(void)
{
  ENTRY(); // ENTRY("vDicStackDepth"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));

  i8 iDepth = pDicStack->iCount();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.discard -- discard top of dictionary stack                        */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDiscard(void)
{
  ENTRY(); // ENTRY("vDicStackDiscard"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));

  pDicStack->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.dump -- dump dictionary stack                                     */
/*----------------------------------------------------------------------------*/
void Step::vDicStackDump(void)
{
  ENTRY(); // ENTRY("vDicStackDump"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));

  pDicStack->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pick -- pick a dic from dic stack and push onto data stack        */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPick(void)
{
  ENTRY(); // ENTRY("vDicStackPick"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDic = m_pDataStack->iPopInt(__func__);
  
  Object* pPtrDic = pDicStack->pPick(iDic);

  m_pDataStack->vPushPtr(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.push -- push a dictionary onto given dictionary stack             */
/*----------------------------------------------------------------------------*/
void Step::vDicStackPush(void)
{
  ENTRY(); // ENTRY("vDicStackPush"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));

  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
  pDicStack->vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.toPathStr -- convert dictionary stack to path string              */
/*----------------------------------------------------------------------------*/
void Step::vDicStackToPathStr(void)
{
  ENTRY(); // ENTRY("vDicStackToPathStr"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));
  
  s256 sPath;
  
  pDicStack->vToPathStr(sPath);
  
  m_pDataStack->vPushStr(sPath);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
