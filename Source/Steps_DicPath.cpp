/*============================================================================*/
/* Steps_DicPath.cpp - dic path step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// dicstack: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_DicStack(void)
{
  vAddPrimitive("dicPath.depth"_Str,    &vDicPathDepth,    false);
  vAddPrimitive("dicPath.drop"_Str,     &vDicPathDrop,     false);
  vAddPrimitive("dicPath.dump"_Str,     &vDicPathDump,     false);
  vAddPrimitive("dicPath.pick"_Str,     &vDicPathPick,     false);
  vAddPrimitive("dicPath.push"_Str,     &vDicPathPush,     false);
}

/*----------------------------------------------------------------------------*/
/* dicPath.depth -- get number of dictionaries on dictionary stack            */
/*----------------------------------------------------------------------------*/
void Task::vDicPathDepth(MP_Task mpTask)
{
  ENTRY("vDicPathDepth"_Str);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.depth"_Str);
  
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
  ENTRY("vDicPathDrop"_Str);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.drop"_Str);
  
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
  ENTRY("vDicPathDump"_Str);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.dump"_Str);
  
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
  ENTRY("vDicPathPick"_Str);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.pick"_Str);
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDic;
  if ( ! mpTask->m_pDataStack->bPop(iDic) )
    THROW_INVALID_DSTACK("dicPath.pick"_Str);
  
  Object* pPtrDic = pDicPath->pPick(iDic);

  mpTask->m_pDataStack->vPushObject(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.push -- push a dictionary onto given dictionary stack              */
/*----------------------------------------------------------------------------*/
void Task::vDicPathPush(MP_Task mpTask)
{
  ENTRY("vDicPathPush"_Str);
  
  Object* pPtrDicStack;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.push"_Str);
  
  DicPath* pDicPath = DicPath::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicPath.push"_Str);
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicPath->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
