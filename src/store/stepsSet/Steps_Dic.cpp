/*============================================================================*/
/* Steps_Dic.cpp - dic step group                                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dic(void)
{
  vAddStep("dic::new"_Stz,        &Step::vDicNew);
  vAddStep("dic::root"_Stz,       &Step::vDicRoot);
  
  vAddStep("dic.delete"_Stz,      &Step::vDicDelete);
  vAddStep("dic.firstWord"_Stz,   &Step::vDicFirstWord);
  vAddStep("dic.kind@"_Stz,       &Step::vDicKindGet);
  vAddStep("dic.kindString@"_Stz, &Step::vDicKindGetString);
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
  
  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));

  delete pDic;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.firstWord -- get first word of dictionary                               */
/*----------------------------------------------------------------------------*/
void Step::vDicFirstWord(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));
  
  Word* pFirstWord = pDic->pFirstWord();
  
  m_oDataStack.vPushPtr(pFirstWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind (enum)                                    */
/*----------------------------------------------------------------------------*/
void Step::vDicKindGet(void)
{
  ENTRY();

  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));

  eDK eDicKind = pDic->eDicKind();

  m_oDataStack.vPushInt((i8) eDicKind);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kindString@ -- get dictionary kind string                              */
/*----------------------------------------------------------------------------*/
void Step::vDicKindGetString(void)
{
  ENTRY();

  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));

  s32 sDicKind = aDK[pDic->eDicKind()];

  m_oDataStack.vPushStr(sDicKind);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind! -- set dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicKindSet(void)
{
  ENTRY();

  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));

  eDK eDicKind = (eDK)m_oDataStack.iPopInt(__func__);

  pDic->vDicKind(eDicKind);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.lastWord -- get last word of dictionary                                */
/*----------------------------------------------------------------------------*/
void Step::vDicLastWord(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));
  
  Word* pLastWord = pDic->pLastWord();
  
  m_oDataStack.vPushPtr(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDicNameFetch(void)
{
  ENTRY();
 
  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));

  s256 sName(pDic->szDicNm());
  
  m_oDataStack.vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name! -- set dictionary name                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicNameStore(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));

  s256 sName;
  m_oDataStack.vPopStr(sName, __func__);
  
  pDic->vDicNm(sName);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* dic::new -- create new dictionary                                          */
/*----------------------------------------------------------------------------*/
void Step::vDicNew(void)
{
  ENTRY();
    
  s256 szDicNm;
  m_oDataStack.vPopStr(szDicNm, __func__);
  
  Dic* pDic = new Dic(szDicNm);
  
  m_oDataStack.vPushPtr(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Step::vDicRef(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));

  bool bRef = pDic->bRefOut();
  
  m_oDataStack.vPushInt(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic::root -- push root dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Step::vDicRoot(void)
{
  ENTRY();
  
  Dic* pDicRoot = AM::DIC::pRoot();
  m_oDataStack.vPushPtr(pDicRoot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.wordCount -- get number of words in dictionary                         */
/*----------------------------------------------------------------------------*/
void Step::vDicWordCount(void)
{
  ENTRY();
  
  Dic* pDic = _pDic(m_oDataStack.pPopPtr(__func__));
  
  i8 nWords = pDic->nWords();
  
  m_oDataStack.vPushInt(nWords);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
