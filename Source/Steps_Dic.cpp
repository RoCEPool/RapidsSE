/*============================================================================*/
/* Steps_Dic.cpp - dic step group                                             */
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
  vAddPrimitive("dic.frozen@"_Str,     &vDicFrozenGet,     false);
  vAddPrimitive("dic.frozen!"_Str,     &vDicFrozenSet,     false);
  vAddPrimitive("dic.init"_Str,        &vDicInit,          false);
  vAddPrimitive("dic.isCompiled"_Str,  &vDicIsCompiled,    false);
  vAddPrimitive("dic.kind@"_Str,       &vDicKindGet,       false);
  vAddPrimitive("dic.kind!"_Str,       &vDicKindSet,       false);
  vAddPrimitive("dic.lastWord"_Str,    &vDicLastWord,      false);
  vAddPrimitive("dic.main"_Str,        &vDicMain,          false);
  vAddPrimitive("dic.name"_Str,        &vDicName,          false);
  vAddPrimitive("dic.ref"_Str,         &vDicRef,           false);
  vAddPrimitive("dic.wordCount"_Str,   &vDicWordCount,     false);
}

/*----------------------------------------------------------------------------*/
/* dic.frozen@ -- get dictionary frozen?                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicFrozenGet(NNP_Task nnpTask)
{
ENTRY("vDicFrozenGet");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.frozen@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  bool bFrozen = pDic->bFrozen();
  
  nnpTask->m_pDataStack->vPush(bFrozen);
  
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
  
  if ( ! nnpTask->m_pDataStack->bPop(iFrozen,pPtrDic) )
    THROW_INVALID_DSTACK("dic.frozen@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDic->vFrozen(iFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.init -- initialise a dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vDicInit(NNP_Task nnpTask)
{
ENTRY("vDicInit");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
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
  
  Word* pWord = gpMainDic->pFindWord("__compiled__"_Str);
  
  i8 bIsCompiled = pWord ? true : false;
  
  nnpTask->m_pDataStack->vPush(bIsCompiled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Task::vDicKindGet(NNP_Task nnpTask)
{
  ENTRY("vDicFrozenGet");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.kind@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Object* pPtrKind;
  pPtrKind = pDic->pDicKind();
  
  nnpTask->m_pDataStack->vPush(pPtrKind);
  
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
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic,pPtrKind) )
    THROW_INVALID_DSTACK("dic.kind@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pKind = _pWord(pPtrKind);
  
  pDic->vDicKind(pKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.lastWord -- get last word of dictionary                                */
/*----------------------------------------------------------------------------*/
void Task::vDicLastWord(NNP_Task nnpTask)
{
ENTRY("vDicLastWord");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.lastWord");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pLastWord = pDic->pLastWord();
  
  nnpTask->m_pDataStack->vPush(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.main -- push main dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicMain(NNP_Task nnpTask)
{
ENTRY("vDicMain");
  
  nnpTask->m_pDataStack->vPush(gpMainDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDicName(NNP_Task nnpTask)
{
ENTRY("vDicName");
 
  Object* pPtrDic;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.name");
  
  Dic* pDic = _pDic(pPtrDic);

  ValWsp sName;
  sName = pDic->sName().aStr();
  
  nnpTask->m_pDataStack->vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Task::vDicRef(NNP_Task nnpTask)
{
  ENTRY("vDicRef");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.ref");
  
  Dic* pDic = _pDic(pPtrDic);

  bool bRef = pDic->bRefOut();
  
  nnpTask->m_pDataStack->vPush(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.wordCount -- get number of words in dictionary                         */
/*----------------------------------------------------------------------------*/
void Task::vDicWordCount(NNP_Task nnpTask)
{
  ENTRY("vDicWordCount");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.ref");
  
  Dic* pDic = _pDic(pPtrDic);
  
  i8 nWords = pDic->nWords();
  
  nnpTask->m_pDataStack->vPush(nWords);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
