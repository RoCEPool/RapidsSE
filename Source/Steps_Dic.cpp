/*============================================================================*/
/* Steps_Dic.cpp - dic step group                                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dic(void)
{
  vAddStep("dic.frozen@"_Stz,     &Step::vDicFrozenGet);
  vAddStep("dic.frozen!"_Stz,     &Step::vDicFrozenSet);
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
/* dic.frozen@ -- get dictionary frozen?                                      */
/*----------------------------------------------------------------------------*/
void Step::vDicFrozenGet(MP_Task mpTask)
{
ENTRY("vDicFrozenGet"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.frozen@"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  bool bFrozen = pDic->bFrozen();
  
  mpTask->m_pDataStack->vPushI8(bFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.frozen! -- set dictionary frozen?                                      */
/*----------------------------------------------------------------------------*/
void Step::vDicFrozenSet(MP_Task mpTask)
{
ENTRY("vDicFrozenSet"_Stz);

  i8 iFrozen;
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(iFrozen,pPtrDic) )
    THROW_INVALID_DSTACK("dic.frozen@"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDic->vFrozen(iFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.init -- initialise a dictionary                                        */
/*----------------------------------------------------------------------------*/
void Step::vDicInit(MP_Task mpTask)
{
ENTRY("vDicInit"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.init"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);

  new (pDic) Dic;
  
  // this is because of GlobalDictionary (ugh)
  pDic->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicKindGet(MP_Task mpTask)
{
  ENTRY("vDicFrozenGet"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.kind@"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  Object* pPtrKind;
  pPtrKind = pDic->pDicKind();
  
  mpTask->m_pDataStack->vPushPointer(pPtrKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind! -- set dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Step::vDicKindSet(MP_Task mpTask)
{
  ENTRY("vDicKindSet"_Stz);
  
  Object* pPtrKind;
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic,pPtrKind) )
    THROW_INVALID_DSTACK("dic.kind@"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  ForthWord* pKind = _pWord(pPtrKind);
  
  pDic->vDicKind(pKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.lastWord -- get last word of dictionary                                */
/*----------------------------------------------------------------------------*/
void Step::vDicLastWord(MP_Task mpTask)
{
ENTRY("vDicLastWord"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.lastWord"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  ForthWord* pLastWord = pDic->pLastWord();
  
  mpTask->m_pDataStack->vPushPointer(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Step::vDicName(MP_Task mpTask)
{
ENTRY("vDicName"_Stz);
 
  Object* pPtrDic;
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.name"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);

  s32 sName;
  sName = pDic->sName();
  
  mpTask->m_pDataStack->vPushS32(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Step::vDicRef(MP_Task mpTask)
{
  ENTRY("vDicRef"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.ref"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);

  bool bRef = pDic->bRefOut();
  
  mpTask->m_pDataStack->vPushI8(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.root -- push root dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Step::vDicRoot(MP_Task mpTask)
{
  ENTRY("vDicRoot"_Stz);
  
  mpTask->m_pDataStack->vPushPointer(&gpAttachMap->m_oRootDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.wordCount -- get number of words in dictionary                         */
/*----------------------------------------------------------------------------*/
void Step::vDicWordCount(MP_Task mpTask)
{
  ENTRY("vDicWordCount"_Stz);
  
  Object* pPtrDic;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDic) )
    THROW_INVALID_DSTACK("dic.ref"_Stz);
  
  Dic* pDic = _pDic(pPtrDic);
  
  i8 nWords = pDic->nWords();
  
  mpTask->m_pDataStack->vPushI8(nWords);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
