/*============================================================================*/
/* Task_DicStack.cpp - task dic stack group                                   */
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
  vAddPrimitive("dicPath.depth",    &vDicStackDepth,    false);
  vAddPrimitive("dicPath.drop",     &vDicStackDrop,     false);
  vAddPrimitive("dicPath.dump",     &vDicStackDump,     false);
  vAddPrimitive("dicPath.pick",     &vDicStackPick,     false);
  vAddPrimitive("dicPath.push",     &vDicStackPush,     false);
}

/*----------------------------------------------------------------------------*/
/* dicPath.depth -- get number of dictionaries on dictionary stack           */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDepth(NNP_Task nnpTask)
{
  ENTRY("vDicStackDepth");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.depth");
  
  DicPath* pDicStack = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicStack->eClass());
  
  i8 iDepth = pDicStack->iCount();
  
  nnpTask->m_pDataStack->vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.drop -- discard top of dictionary stack                           */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDrop(NNP_Task nnpTask)
{
  ENTRY("vDicStackDrop");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.drop");
  
  DicPath* pDicStack = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicStack->eClass());
  
  pDicStack->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.dump -- dump dictionary stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDump(NNP_Task nnpTask)
{
  ENTRY("vDicStackDump");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.dump");
  
  DicPath* pDicStack = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicStack->eClass());
  
  pDicStack->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.pick -- pick a dic from dic stack and push onto data stack        */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPick(NNP_Task nnpTask)
{
  ENTRY("vDicStackPick");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.pick");
  
  DicPath* pDicStack = (DicPath*)(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicStack->eClass());
  
  i8 iDic;
  if ( ! nnpTask->m_pDataStack->bPop(iDic) )
    THROW_INVALID_DSTACK("dicPath.pick");
  
  Object* pPtrDic = pDicStack->pPick(iDic);

  nnpTask->m_pDataStack->vPush(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicPath.push -- push a dictionary onto given dictionary stack             */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPush(NNP_Task nnpTask)
{
  ENTRY("vDicStackPush");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicPath.push");
  
  DicPath* pDicStack = DicPath::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICPATH == pDicStack->eClass());
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicPath.push");
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicStack->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
