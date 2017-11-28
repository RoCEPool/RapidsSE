/*============================================================================*/
/* Steps_DicPath.cpp - dic path step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;

// dicstack: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_DicStack(void)
{
  vAddPrimitive("dicPath.depth"_Stz,    &vDicPathDepth,    false);
  vAddPrimitive("dicPath.drop"_Stz,     &vDicPathDrop,     false);
  vAddPrimitive("dicPath.dump"_Stz,     &vDicPathDump,     false);
  vAddPrimitive("dicPath.pick"_Stz,     &vDicPathPick,     false);
  vAddPrimitive("dicPath.push"_Stz,     &vDicPathPush,     false);
}

/*----------------------------------------------------------------------------*/
/* dicPath.depth -- get number of dictionaries on dictionary stack            */
/*----------------------------------------------------------------------------*/
void Task::vDicPathDepth(MP_Task mpTask)
{
  ENTRY("vDicPathDepth"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.depth"_Stz);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDepth = pDicPath->iCount();
  
  mpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.drop -- discard top of dictionary stack                            */
/*----------------------------------------------------------------------------*/
void Task::vDicPathDrop(MP_Task mpTask)
{
  ENTRY("vDicPathDrop"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.drop"_Stz);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  pDicPath->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.dump -- dump dictionary stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicPathDump(MP_Task mpTask)
{
  ENTRY("vDicPathDump"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.dump"_Stz);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  pDicPath->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pick -- pick a dic from dic stack and push onto data stack         */
/*----------------------------------------------------------------------------*/
void Task::vDicPathPick(MP_Task mpTask)
{
  ENTRY("vDicPathPick"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.pick"_Stz);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDic;
  if ( ! mpTask->m_pDataStack->bPop(iDic) )
    THROW_INVALID_DSTACK("dicPath.pick"_Stz);
  
  Object* pPtrDic = pDicPath->pPick(iDic);

  mpTask->m_pDataStack->vPushObject(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.push -- push a dictionary onto given dictionary stack              */
/*----------------------------------------------------------------------------*/
void Task::vDicPathPush(MP_Task mpTask)
{
  ENTRY("vDicPathPush"_Stz);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.push"_Stz);
  
  DicPath* pDicPath = DicPath::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicPath.push"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicPath->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
