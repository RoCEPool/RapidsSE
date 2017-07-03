/*============================================================================*/
/* Steps_Dic.cpp - dic step group                                             */
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
void Task::vDicFrozenGet(MP_Task mpTask)
{
ENTRY("vDicFrozenGet"_Str);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.frozen@"_Str);
  
  Dic* pDic = _pDic(pPtrDic);
  
  bool bFrozen = pDic->bFrozen();
  
  mpTask->m_pDataStack->vPushInt(bFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.frozen! -- set dictionary frozen?                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicFrozenSet(MP_Task mpTask)
{
ENTRY("vDicFrozenSet"_Str);

  i8 iFrozen;
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(iFrozen,pPtrDic) )
    THROW_INVALID_DSTACK("dic.frozen@"_Str);
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDic->vFrozen(iFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.init -- initialise a dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vDicInit(MP_Task mpTask)
{
ENTRY("vDicInit"_Str);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.init"_Str);
  
  Dic* pDic = _pDic(pPtrDic);

  new (pDic) Dic;
  
  // this is because of GlobalDictionary (ugh)
  pDic->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.isCompiled -- has main dictionary been fully compiled?                 */
/*----------------------------------------------------------------------------*/
void Task::vDicIsCompiled(MP_Task mpTask)
{
  ENTRY("vDicIsCompiled"_Str);
  
  Word* pWord = gpMainDic->pFindWord("__compiled__"_Str);
  
  i8 bIsCompiled = pWord ? true : false;
  
  mpTask->m_pDataStack->vPushInt(bIsCompiled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Task::vDicKindGet(MP_Task mpTask)
{
  ENTRY("vDicFrozenGet"_Str);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.kind@"_Str);
  
  Dic* pDic = _pDic(pPtrDic);
  
  Object* pPtrKind;
  pPtrKind = pDic->pDicKind();
  
  mpTask->m_pDataStack->vPushObject(pPtrKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind! -- set dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Task::vDicKindSet(MP_Task mpTask)
{
  ENTRY("vDicKindSet"_Str);
  
  Object* pPtrKind;
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic,pPtrKind) )
    THROW_INVALID_DSTACK("dic.kind@"_Str);
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pKind = _pWord(pPtrKind);
  
  pDic->vDicKind(pKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.lastWord -- get last word of dictionary                                */
/*----------------------------------------------------------------------------*/
void Task::vDicLastWord(MP_Task mpTask)
{
ENTRY("vDicLastWord"_Str);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.lastWord"_Str);
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pLastWord = pDic->pLastWord();
  
  mpTask->m_pDataStack->vPushObject(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.main -- push main dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicMain(MP_Task mpTask)
{
ENTRY("vDicMain"_Str);
  
  mpTask->m_pDataStack->vPushObject(gpMainDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDicName(MP_Task mpTask)
{
ENTRY("vDicName"_Str);
 
  Object* pPtrDic;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.name"_Str);
  
  Dic* pDic = _pDic(pPtrDic);

  ValWrk sName;
  sName = pDic->sName();
  
  mpTask->m_pDataStack->vPushValWrk(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Task::vDicRef(MP_Task mpTask)
{
  ENTRY("vDicRef"_Str);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.ref"_Str);
  
  Dic* pDic = _pDic(pPtrDic);

  bool bRef = pDic->bRefOut();
  
  mpTask->m_pDataStack->vPushInt(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.wordCount -- get number of words in dictionary                         */
/*----------------------------------------------------------------------------*/
void Task::vDicWordCount(MP_Task mpTask)
{
  ENTRY("vDicWordCount"_Str);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.ref"_Str);
  
  Dic* pDic = _pDic(pPtrDic);
  
  i8 nWords = pDic->nWords();
  
  mpTask->m_pDataStack->vPushInt(nWords);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
