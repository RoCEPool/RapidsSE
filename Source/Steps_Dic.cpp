/*============================================================================*/
/* Steps_Dic.cpp - dic step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dic(void)
{
  vAddStep("dic.init"_Stz,        &Step::vDicInit);
  vAddStep("dic.kind@"_Stz,       &Step::vDicKindGet);
  vAddStep("dic.kind!"_Stz,       &Step::vDicKindSet);
  vAddStep("dic.lastWord"_Stz,    &Step::vDicLastWord);
  vAddStep("dic.name"_Stz,        &Step::vDicName);
  vAddStep("dic.ref"_Stz,         &Step::vDicRef);
  vAddStep("dic.root"_Stz,        &Step::vDicRoot);
  vAddStep("dic.wordCount"_Stz,   &Step::vDicWordCount);
}

/*----------------------------------------------------------------------------*/
/* dic.init -- initialise a dictionary                                        */
/*----------------------------------------------------------------------------*/
void Step::vDicInit(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDicInit"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Dic* pDic = _pDic(pPtrDic);

  new (pDic) Dic;
  
  // this is because of GlobalDictionary (ugh)
  pDic->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicKindGet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicFrozenGet"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Dic* pDic = _pDic(pPtrDic);
  
  Object* pPtrKind;
  pPtrKind = pDic->pDicKind();
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pPtrKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind! -- set dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicKindSet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicKindSet"_Stz);
  
  Object* pPtrKind;
  Object* pPtrDic;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDic,pPtrKind) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pKind = _pWord(pPtrKind);
  
  pDic->vDicKind(pKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.lastWord -- get last word of dictionary                                */
/*----------------------------------------------------------------------------*/
void Step::vDicLastWord(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDicLastWord"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pLastWord = pDic->pLastWord();
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDicName(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDicName"_Stz);
 
  Object* pPtrDic;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Dic* pDic = _pDic(pPtrDic);

  s32 sName(pDic->szName());
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Step::vDicRef(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicRef"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Dic* pDic = _pDic(pPtrDic);

  bool bRef = pDic->bRefOut();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.root -- push root dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Step::vDicRoot(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicRoot"_Stz);
  
  Dic* pRootDic = AM::pRootDic();
  mpTask->m_pForth->m_pDataStack->vPushPointer(pRootDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.wordCount -- get number of words in dictionary                         */
/*----------------------------------------------------------------------------*/
void Step::vDicWordCount(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDicWordCount"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrDic) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Dic* pDic = _pDic(pPtrDic);
  
  i8 nWords = pDic->nWords();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(nWords);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
