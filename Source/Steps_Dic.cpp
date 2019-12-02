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
void Step::vDicInit(void)
{
  ENTRY(); // ENTRY("vDicInit"_Stz);
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));

  new (pDic) Dic;
  
  // this is because of GlobalDictionary (ugh)
  pDic->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicKindGet(void)
{
  ENTRY(); // ENTRY("vDicFrozenGet"_Stz);
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
  Object* pPtrKind;
  pPtrKind = pDic->pDicKind();
  
  m_pDataStack->vPushPtr(pPtrKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind! -- set dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicKindSet(void)
{
  ENTRY(); // ENTRY("vDicKindSet"_Stz);
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));

  Word* pKind = _pWord(m_pDataStack->pPopPtr(__func__));
  
  pDic->vDicKind(pKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.lastWord -- get last word of dictionary                                */
/*----------------------------------------------------------------------------*/
void Step::vDicLastWord(void)
{
  ENTRY(); // ENTRY("vDicLastWord"_Stz);
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
  Word* pLastWord = pDic->pLastWord();
  
  m_pDataStack->vPushPtr(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDicName(void)
{
  ENTRY(); // ENTRY("vDicName"_Stz);
 
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));

  s32 sName(pDic->szName());
  
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Step::vDicRef(void)
{
  ENTRY(); // ENTRY("vDicRef"_Stz);
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));

  bool bRef = pDic->bRefOut();
  
  m_pDataStack->vPushInt(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.root -- push root dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Step::vDicRoot(void)
{
  ENTRY(); // ENTRY("vDicRoot"_Stz);
  
  Dic* pRootDic = AM::pRootDic();
  m_pDataStack->vPushPtr(pRootDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.wordCount -- get number of words in dictionary                         */
/*----------------------------------------------------------------------------*/
void Step::vDicWordCount(void)
{
  ENTRY(); // ENTRY("vDicWordCount"_Stz);
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
  i8 nWords = pDic->nWords();
  
  m_pDataStack->vPushInt(nWords);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
