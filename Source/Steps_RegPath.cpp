/*============================================================================*/
/* Steps_RegPath.cpp - path[] step group                                      */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegPath(void)
{
  vAddStep("path[]"_Stz,           &Step::vRegPathGet);
  
  vAddStep("path[].depth"_Stz,     &Step::vRegPathDepth);
  vAddStep("path[].discard"_Stz,   &Step::vRegPathDiscard);
  vAddStep("path[].dump"_Stz,      &Step::vRegPathDump);
  vAddStep("path[].pick"_Stz,      &Step::vRegPathPick);
  vAddStep("path[].push"_Stz,      &Step::vRegPathPush);
  vAddStep("path[].toPathStr"_Stz, &Step::vRegPathToPathStr);

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
/* path[].depth -- get number of dictionaries on dictionary stack             */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDepth(void)
{
  ENTRY(); // ENTRY("vRegPathDepth"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));

  i8 iDepth = pDicStack->iCount();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].discard -- discard top of dictionary stack                          */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDiscard(void)
{
  ENTRY(); // ENTRY("vRegPathDiscard"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));

  pDicStack->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].dump -- dump dictionary stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegPathDump(void)
{
  ENTRY(); // ENTRY("vRegPathDump"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));

  pDicStack->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].pick -- pick a dic from dic stack and push onto data stack          */
/*----------------------------------------------------------------------------*/
void Step::vRegPathPick(void)
{
  ENTRY(); // ENTRY("vRegPathPick"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDic = m_pDataStack->iPopInt(__func__);
  
  Object* pPtrDic = pDicStack->pPick(iDic);

  m_pDataStack->vPushPtr(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].push -- push a dictionary onto given dictionary stack               */
/*----------------------------------------------------------------------------*/
void Step::vRegPathPush(void)
{
  ENTRY(); // ENTRY("vRegPathPush"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));

  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
  pDicStack->vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* path[].toPathStr -- convert dictionary stack to path string                */
/*----------------------------------------------------------------------------*/
void Step::vRegPathToPathStr(void)
{
  ENTRY(); // ENTRY("vRegPathToPathStr"_Stz);
  
  DicStack* pDicStack = _pDicStack(m_pDataStack->pPopPtr(__func__));
  
  s256 sPath;
  
  pDicStack->vToPathStr(sPath);
  
  m_pDataStack->vPushStr(sPath);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
