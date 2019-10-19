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
void Step::vDicPathDepth(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicStackDepth"_Stz);
  
  Object* pPtrDicPath;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicPath) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDepth = pDicPath->nDicNames();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.dump -- dump dictionary path                                       */
/*----------------------------------------------------------------------------*/
void Step::vDicPathDump(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicPathDump"_Stz);
  
  Object* pPtrDicPath;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicPath) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  pDicPath->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pick -- pick a dic from dic path and push onto data stack          */
/*----------------------------------------------------------------------------*/
void Step::vDicPathPick(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicPathPick"_Stz);
  
  Object* pPtrDicPath;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDicPath) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDic;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK);

  mpTask->m_pForth->m_pDataStack->vPushS32(pDicPath->sDicName(iDic));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
