/*============================================================================*/
/* Steps_DicPath.cpp - dic path step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// dicstack: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_DicPath(void)
{
  vAddStep("dicPath.depth"_Stz,    &vDicPathDepth,    false);
  vAddStep("dicPath.dump"_Stz,     &vDicPathDump,     false);
  vAddStep("dicPath.pick"_Stz,     &vDicPathPick,     false);
}

/*----------------------------------------------------------------------------*/
/* dicPath.depth -- get number of dictionaries in dictionary path             */
/*----------------------------------------------------------------------------*/
void Task::vDicPathDepth(MP_Task mpTask)
{
  ENTRY("vDicStackDepth"_Stz);
  
  Object* pPtrDicPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicPath) )
    THROW_INVALID_DSTACK("dicPath.depth"_Stz);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDepth = pDicPath->m_iDicDepth;
  
  mpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.dump -- dump dictionary path                                       */
/*----------------------------------------------------------------------------*/
void Task::vDicPathDump(MP_Task mpTask)
{
  ENTRY("vDicPathDump"_Stz);
  
  Object* pPtrDicPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicPath) )
    THROW_INVALID_DSTACK("dicPath.dump"_Stz);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  pDicPath->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pick -- pick a dic from dic path and push onto data stack          */
/*----------------------------------------------------------------------------*/
void Task::vDicPathPick(MP_Task mpTask)
{
  ENTRY("vDicPathPick"_Stz);
  
  Object* pPtrDicPath;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicPath) )
    THROW_INVALID_DSTACK("dicPath.pick"_Stz);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicPath);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDic;
  if ( ! mpTask->m_pDataStack->bPop(iDic) )
    THROW_INVALID_DSTACK("dicPath.pick"_Stz);

  mpTask->m_pDataStack->vPushS256(pDicPath->m_sDicPath[iDic]);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
