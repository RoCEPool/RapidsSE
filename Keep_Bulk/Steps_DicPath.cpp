/*============================================================================*/
/* Steps_DicPath.cpp - dic path step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_DicPath(void)
{
  vAddStep("dicPath.depth"_Stz,    &Step::vDicPathDepth);
  vAddStep("dicPath.dump"_Stz,     &Step::vDicPathDump);
  vAddStep("dicPath.pick"_Stz,     &Step::vDicPathPick);
}

/*----------------------------------------------------------------------------*/
/* dicPath.depth -- get number of dictionaries in dictionary path             */
/*----------------------------------------------------------------------------*/
void Step::vDicPathDepth(void)
{
  ENTRY();
  
  DicPath* pDicPath = _pDicPath(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDepth = pDicPath->nDicNames();
  
  m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.dump -- dump dictionary path                                       */
/*----------------------------------------------------------------------------*/
void Step::vDicPathDump(void)
{
  ENTRY();
  
  DicPath* pDicPath = _pDicPath(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  pDicPath->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pick -- pick a dic from dic path and push onto data stack          */
/*----------------------------------------------------------------------------*/
void Step::vDicPathPick(void)
{
  ENTRY();
  
  DicPath* pDicPath = _pDicPath(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDic = m_pDataStack->iPopInt(__func__);

  s32 sDicNm(pDicPath->sDicName(iDic));
  
  m_pDataStack->vPushStr(sDicNm);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
