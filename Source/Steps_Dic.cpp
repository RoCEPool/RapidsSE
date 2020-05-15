/*============================================================================*/
/* Steps_Dic.cpp - dic step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dic(void)
{
  vAddStep("dic::new"_Stz,        &Step::vDicNew);
  vAddStep("dic::root"_Stz,       &Step::vDicRoot);
  
  vAddStep("dic.delete"_Stz,      &Step::vDicDelete);
  vAddStep("dic.kind@"_Stz,       &Step::vDicKindGet);
  vAddStep("dic.kind!"_Stz,       &Step::vDicKindSet);
  vAddStep("dic.lastWord"_Stz,    &Step::vDicLastWord);
  vAddStep("dic.name@"_Stz,       &Step::vDicNameFetch);
  vAddStep("dic.name!"_Stz,       &Step::vDicNameStore);
  vAddStep("dic.ref"_Stz,         &Step::vDicRef);
  vAddStep("dic.wordCount"_Stz,   &Step::vDicWordCount);
}

/*----------------------------------------------------------------------------*/
/* dic.delete -- delete a dictionary                                          */
/*----------------------------------------------------------------------------*/
void Step::vDicDelete(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));

  delete pDic;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicKindGet(void)
{
  ENTRY();
  
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
  ENTRY();
  
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
  ENTRY();
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
  Word* pLastWord = pDic->pLastWord();
  
  m_pDataStack->vPushPtr(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDicNameFetch(void)
{
  ENTRY();
 
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));

  s32 sName(pDic->szDicNm());
  
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name! -- set dictionary name                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicNameStore(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));

  s32 sName;
  m_pDataStack->vPopStr(sName,__func__);
  
  pDic->vDicNm(sName);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* dic::new -- create new dictionary                                          */
/*----------------------------------------------------------------------------*/
void Step::vDicNew(void)
{
  ENTRY();
    
  s32 szDicNm;
  m_pDataStack->vPopStr(szDicNm,__func__);
  
  Dic* pDic = new Dic(szDicNm);
  
  m_pDataStack->vPushPtr(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Step::vDicRef(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));

  bool bRef = pDic->bRefOut();
  
  m_pDataStack->vPushInt(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic::root -- push root dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Step::vDicRoot(void)
{
  ENTRY();
  
  Dic* pRootDic = AM::pRootDic();
  m_pDataStack->vPushPtr(pRootDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.wordCount -- get number of words in dictionary                         */
/*----------------------------------------------------------------------------*/
void Step::vDicWordCount(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_pDataStack->pPopPtr(__func__));
  
  i8 nWords = pDic->nWords();
  
  m_pDataStack->vPushInt(nWords);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
