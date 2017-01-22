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
  vAddPrimitive("dicStack.depthOld", &vDicStackDepthOld, false);
  vAddPrimitive("dicStack.depthNew", &vDicStackDepthNew, false);
  vAddPrimitive("dicStack.drop",     &vDicStackDrop,     false);
  vAddPrimitive("dicStack.dumpOld",  &vDicStackDumpOld,  false);
  vAddPrimitive("dicStack.dumpNew",  &vDicStackDumpNew,  false);
  vAddPrimitive("dicStack.pick",     &vDicStackPick,     false);
  vAddPrimitive("dicStack.pushOld",  &vDicStackPushOld,  false);
  vAddPrimitive("dicStack.pushNew",  &vDicStackPushNew,  false);
}

/*----------------------------------------------------------------------------*/
/* dicStack.depthOld -- get number of dictionaries on dictionary stack           */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDepthOld(NNP_Task nnpTask)
{
  ENTRY("vDicStackDepth");
  
  i8 iCount;
  
  if (nnpTask->m_pSEitf) {
    iCount = nnpTask->m_pSEitf->pDicStack()->iCount();
  }
  else {
    iCount = nnpTask->m_xDicStack.iCount();
  }
  
  nnpTask->m_pDataStack->vPush(iCount);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.depth -- get number of dictionaries on dictionary stack           */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDepthNew(NNP_Task nnpTask)
{
  ENTRY("vDicStackDepth");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.pick");
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDepth = pDicStack->iCount();
  
  nnpTask->m_pDataStack->vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.drop -- discard top of dictionary stack                           */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDrop(NNP_Task nnpTask)
{
  ENTRY("vDicStackDrop");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.pick");
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vDiscard();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.dumpOld -- dump dictionary stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDumpOld(NNP_Task nnpTask)
{
  ENTRY("vDicStackDump");
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->pDicStack()->vContentDump();
  }
  else {
    nnpTask->m_xDicStack.vContentDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.dump -- dump dictionary stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vDicStackDumpNew(NNP_Task nnpTask)
{
  ENTRY("vDicStackDump");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.pick");
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  pDicStack->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pathPick -- pick a dic from dic stack and push onto data stack        */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPick(NNP_Task nnpTask)
{
  ENTRY("vDicStackPick");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.pick");
  
  DicStack* pDicStack = (DicStack*)(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  i8 iDic;
  if ( ! nnpTask->m_pDataStack->bPop(iDic) )
    THROW_INVALID_DSTACK("dicStack.pick");
  
  Object* pPtrDic = pDicStack->pPick(iDic);

  nnpTask->m_pDataStack->vPush(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pushOld -- push a dictionary from data stack  onto dictionary stack  */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPushOld(NNP_Task nnpTask)
{
  ENTRY("vDicStackPush");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicStack.pushOld");
  
  Dic* pDic = _pDic(pPtrDic);
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->vPushDic(pDic);
  }
  else {
    nnpTask->m_xDicStack.vPush(pDic);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.pushOld -- push a dictionary onto given dictionary stack             */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPushNew(NNP_Task nnpTask)
{
  ENTRY("vDicStackPushNew");
  
  Object* pPtrDicStack;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDicStack) )
    THROW_INVALID_DSTACK("dicStack.pushNew");
  
  DicStack* pDicStack = DicStack::_pDicStack(pPtrDicStack);
  ASSERT(CN_DICSTACK == pDicStack->eClass());
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dicStack.pushNew");
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDicStack->vPush(pDic);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
