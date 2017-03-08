/*============================================================================*/
/* Steps_DicPath.cpp - dic path step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
void Task::vDicPathDepth(NNP_Task nnpTask)
{
  ENTRY("vDicPathDepth"_Str);
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.depth");
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDepth = pDicPath->iCount();
  
  nnpTask->m_pDataStack->vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.drop -- discard top of dictionary stack                            */
/*----------------------------------------------------------------------------*/
void Task::vDicPathDrop(NNP_Task nnpTask)
{
  ENTRY("vDicPathDrop"_Str);
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.drop");
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  pDicPath->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.dump -- dump dictionary stack                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicPathDump(NNP_Task nnpTask)
{
  ENTRY("vDicPathDump"_Str);
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.dump");
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  pDicPath->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pick -- pick a dic from dic stack and push onto data stack         */
/*----------------------------------------------------------------------------*/
void Task::vDicPathPick(NNP_Task nnpTask)
{
  ENTRY("vDicPathPick"_Str);
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.pick");
  
  DicPath* pDicPath = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  i8 iDic;
  if ( ! nnpTask->m_pDataStack->bPop(iDic) )
    THROW_INVALID_DSTACK("dicPath.pick");
  
  Object* pPtrDic = pDicPath->pPick(iDic);

  nnpTask->m_pDataStack->vPush(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.push -- push a dictionary onto given dictionary stack              */
/*----------------------------------------------------------------------------*/
void Task::vDicPathPush(NNP_Task nnpTask)
{
  ENTRY("vDicPathPush"_Str);
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.push");
  
  DicPath* pDicPath = DicPath::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicPath->eClass());
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicPath.push");
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicPath->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
