/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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

// dic: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Dic(void)
{
  vAddPrimitive("dic.count",       &vDicCount,         false);
  vAddPrimitive("dic.drop",        &vDicDrop,          false);
  vAddPrimitive("}}",              &vDicDrop,          false);
  vAddPrimitive("dic.frozen@",     &vDicFrozenGet,     false);
  vAddPrimitive("dic.frozen!",     &vDicFrozenSet,     false);
  vAddPrimitive("dic.init",        &vDicInit,          false);
  vAddPrimitive("dic.isCompiled",  &vDicIsCompiled,    false);
  vAddPrimitive("dic.kind@",       &vDicKindGet,       false);
  vAddPrimitive("dic.kind!",       &vDicKindSet,       false);
  vAddPrimitive("dic.last",        &vDicLast,          false);
  vAddPrimitive("dic.main",        &vDicMain,          false);
  vAddPrimitive("dic.name",        &vDicName,          false);
  vAddPrimitive("dic.pick",        &vDicPick,          false);
  vAddPrimitive("dic.push",        &vDicPush,          false);
  vAddPrimitive("{{",              &vDicPush,          false);
  vAddPrimitive("dic.ref",         &vDicRef,           false);
  vAddPrimitive("dic.stack",       &vDicStack,         false);
}

/*----------------------------------------------------------------------------*/
/* dic.count -- get number of dictionaries on dictionary stack                */
/*----------------------------------------------------------------------------*/
void Task::vDicCount(NNP_Task nnpTask)
{
ENTRY("vDicCount");
  
  i8 iCount;
  
  if (nnpTask->m_pSEitf) {
    iCount = nnpTask->m_pSEitf->pDicStack()->iCount();
  }
  else {
    iCount = nnpTask->m_xDicStack.iCount();
  }
  
  nnpTask->m_xDataStack.vPush(iCount);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.drop -- discard top of dictionary stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicDrop(NNP_Task nnpTask)
{
ENTRY("vDicDrop");
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->vPopDic();
  }
  else {
    nnpTask->m_xDicStack.vDiscard();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.frozen@ -- get dictionary frozen?                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicFrozenGet(NNP_Task nnpTask)
{
ENTRY("vDicFrozenGet");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.frozen@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  bool bFrozen = pDic->bFrozen();
  
  nnpTask->m_xDataStack.vPush(bFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.frozen! -- set dictionary frozen?                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicFrozenSet(NNP_Task nnpTask)
{
ENTRY("vDicFrozenSet");

  i8 iFrozen;
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xDataStack.bPop(iFrozen,pPtrDic) )
    THROW_INVALID_DSTACK("dic.frozen@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDic->vFrozen(iFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.dump -- dump dictionary stack                                          */
/*----------------------------------------------------------------------------*/
void Task::vDicStack(NNP_Task nnpTask)
{
ENTRY("vDicDump");
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->pDicStack()->vDump();
  }
  else {
    nnpTask->m_xDicStack.vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.init -- initialise a dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vDicInit(NNP_Task nnpTask)
{
ENTRY("vDicInit");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.init");
  
  Dic* pDic = _pDic(pPtrDic);

  new (pDic) Dic;
  
  // this is because of GlobalDictionary (ugh)
  pDic->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.isCompiled -- has main dictionary been fully compiled?                 */
/*----------------------------------------------------------------------------*/
void Task::vDicIsCompiled(NNP_Task nnpTask)
{
  ENTRY("vDicIsCompiled");
  
  Word* pWord = nnpTask->pFindWord("__compiled__");
  
  i8 bIsCompiled = pWord ? true : false;
  
  nnpTask->m_xDataStack.vPush(bIsCompiled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Task::vDicKindGet(NNP_Task nnpTask)
{
  ENTRY("vDicFrozenGet");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.kind@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Object* pPtrKind;
  pPtrKind = pDic->pDicKind();
  
  nnpTask->m_xDataStack.vPush(pPtrKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind! -- set dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Task::vDicKindSet(NNP_Task nnpTask)
{
  ENTRY("vDicKindSet");
  
  Object* pPtrKind;
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic,pPtrKind) )
    THROW_INVALID_DSTACK("dic.kind@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pKind = _pWord(pPtrKind);
  
  pDic->vDicKind(pKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.last -- get last word of dictionary                                    */
/*----------------------------------------------------------------------------*/
void Task::vDicLast(NNP_Task nnpTask)
{
ENTRY("vDicLast");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.last");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pLastWord = pDic->pCurrentWord();
  
  nnpTask->m_xDataStack.vPush(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.main -- push main dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicMain(NNP_Task nnpTask)
{
ENTRY("vDicMain");
  
  nnpTask->m_xDataStack.vPush(gpMainDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDicName(NNP_Task nnpTask)
{
ENTRY("vDicName");
 
  Object* pPtrDic;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.name");
  
  Dic* pDic = _pDic(pPtrDic);

  ValWsp sName;
  sName = pDic->sName().a();
  
  nnpTask->m_xDataStack.vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.pick -- put a dictionary from dictionary stack onto param stack        */
/*----------------------------------------------------------------------------*/
void Task::vDicPick(NNP_Task nnpTask)
{
ENTRY("vDicPick");
  
  i8 iDic;
  if ( ! nnpTask->m_xDataStack.bPop(iDic) )
    THROW_INVALID_DSTACK("dic.pick");
  
  Object* pPtrDic;

  if (nnpTask->m_pSEitf) {
    pPtrDic = nnpTask->m_pSEitf->pDicStack()->pPick(iDic);
  }
  else {
    pPtrDic = nnpTask->m_xDicStack.pPick(iDic);
  }
  
  nnpTask->m_xDataStack.vPush(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.push -- push a dictionary from param stack onto dictionary stack       */
/*----------------------------------------------------------------------------*/
void Task::vDicPush(NNP_Task nnpTask)
{
ENTRY("vDicPush");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.push");

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
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Task::vDicRef(NNP_Task nnpTask)
{
  ENTRY("vDicRef");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.ref");
  
  Dic* pDic = _pDic(pPtrDic);

  bool bRef = pDic->bRefOut();
  
  nnpTask->m_xDataStack.vPush(bRef);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
