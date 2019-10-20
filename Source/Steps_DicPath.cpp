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
  ENTRY("vDicStackDepth"_Stz);
  
  Object* pPtrDicPath;
  if ( ! m_pDataStack->bPopObject(pPtrDicPath) )
    THROW_DSTACK();
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDepth = pDicPath->nDicNames();
  
  m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.dump -- dump dictionary path                                       */
/*----------------------------------------------------------------------------*/
void Step::vDicPathDump(void)
{
  ENTRY("vDicPathDump"_Stz);
  
  Object* pPtrDicPath;
  if ( ! m_pDataStack->bPopObject(pPtrDicPath) )
    THROW_DSTACK();
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  pDicPath->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pick -- pick a dic from dic path and push onto data stack          */
/*----------------------------------------------------------------------------*/
void Step::vDicPathPick(void)
{
  ENTRY("vDicPathPick"_Stz);
  
  Object* pPtrDicPath;
  if ( ! m_pDataStack->bPopObject(pPtrDicPath) )
    THROW_DSTACK();
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDic;
  if ( ! m_pDataStack->bPopI8(iDic) )
    THROW_DSTACK();

  m_pDataStack->vPushS32(pDicPath->sDicName(iDic));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
