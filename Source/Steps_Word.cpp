/*============================================================================*/
/* Steps_Word.cpp - word step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Word(void)
{
  vAddStep("word::delete"_Stz,      &Step::vWordDelete);
  vAddStep("word::find"_Stz,        &Step::vWordFind);
  vAddStep("word::locate"_Stz,      &Step::vWordLocate);
  vAddStep("word::new"_Stz,         &Step::vWordNew);
  
  vAddStep("word.category@"_Stz,    &Step::vWordCategoryFetch);
  vAddStep("word.category!"_Stz,    &Step::vWordCategoryStore);
  vAddStep("word.dump"_Stz,         &Step::vWordDump);
  vAddStep("word.EnameSet@"_Stz,    &Step::vWordEnmSetFetch);
  vAddStep("word.EnameSet!"_Stz,    &Step::vWordEnmSetStore);
  vAddStep("word.exitHook@"_Stz,    &Step::vWordExitHookGet);
  vAddStep("word.exitHook!"_Stz,    &Step::vWordExitHookSet);
  vAddStep("word.immediate@"_Stz,   &Step::vWordImmediateGet);
  vAddStep("word.immediate!"_Stz,   &Step::vWordImmediateSet);
  vAddStep("word.isImmediate"_Stz,  &Step::vWordIsImmediate);
  vAddStep("word.isIntrinsic"_Stz,  &Step::vWordIsIntrinsic);
  vAddStep("word.kind@"_Stz,        &Step::vWordKindFetch);
  vAddStep("word.kind!"_Stz,        &Step::vWordKindStore);
  vAddStep("word.name@"_Stz,        &Step::vWordNameFetch);
  vAddStep("word.name!"_Stz,        &Step::vWordNameStore);
  vAddStep("word.prev"_Stz,         &Step::vWordPrev);
  vAddStep("word.resetRoutine"_Stz, &Step::vWordResetRoutine);
  vAddStep("word.see"_Stz,          &Step::vWordSee);
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Step::vWord(void)
{
  ENTRY(); // ENTRY("vWord"_Stz);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.category@ -- word category fetch                                      */
/*----------------------------------------------------------------------------*/
void Step::vWordCategoryFetch(void)
{
  ENTRY(); // ENTRY("vWordKindGet"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pCategory = pWord->pCategory();
  
  m_pDataStack->vPushPtr(pCategory);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.category! -- word category store                                      */
/*----------------------------------------------------------------------------*/
void Step::vWordCategoryStore(void)
{
  ENTRY(); // ENTRY("vWordKindSet"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pCategory = _pWord(m_pDataStack->pPopPtr(__func__));
  
  pWord->vCategory(pCategory);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordDump(void)
{
  ENTRY(); // ENTRY("vWordDump"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.EnameSet@ -- fetch ename set                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetFetch(void)
{
  ENTRY(); // ENTRY("vWordEnmSetFetch"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  EnameSet* pEnameSet = pWord->pEnameSet();
  
   m_pDataStack->vPushPtr(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.EnameSet! -- store ename set                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetStore(void)
{
  ENTRY(); // ENTRY("vWordEnmSetStore"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  EnameSet* pEnameSet = _pEnameSet(m_pDataStack->pPopPtr(__func__));
  
  pWord->vEnameSet(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook@ -- get exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookGet(void)
{
  ENTRY(); // ENTRY("vWordExitHookGet"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  s32 sExitHook(pWord->szExitHook());
  
  m_pDataStack->vPushStr(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook! -- set exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookSet(void)
{
  ENTRY(); // ENTRY("vWordExitHookSet"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  s32 sExitHook;
  m_pDataStack->vPopStr(sExitHook,__func__);
  
  pWord->vExitHook(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.find -- find word given its name                                      */
/*----------------------------------------------------------------------------*/
void Step::vWordFind(void)
{
  ENTRY(); // ENTRY("vWordNew"_Stz);
  
  s32 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);
    
  Word* pWordFound = m_oDicStack.pFindWord(sWordNm);
  
  m_pDataStack->vPushPtr(pWordFound);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::delete -- forget word on stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vWordDelete(void)
{
  ENTRY(); // ENTRY("vWordForget"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pRemovedWord = m_oDicStack.pRemoveWord(pWord->szWordNm());
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateGet(void)
{
  ENTRY(); // ENTRY("vWordImmediateGet"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iImmediate = pWord->bImmediate();
  
  m_pDataStack->vPushInt(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateSet(void)
{
  ENTRY(); // ENTRY("vWordImmediateSet"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iImmediate = m_pDataStack->iPopInt(__func__);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isImmediate -- is word immediate?                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordIsImmediate(void)
{
  ENTRY(); // ENTRY("vWordIsImmediate"_Stz);
  
  // we should just modify the pointers rather than actually pop
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  m_pDataStack->vPushInt(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isIntrinsic -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Step::vWordIsIntrinsic(void)
{
  ENTRY(); // ENTRY("vWordIsIntrinsic"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iIsPrimitive = WK_INTRINSIC == pWord->eKind() ? 1 : 0;
  
  m_pDataStack->vPushInt(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindFetch(void)
{
  ENTRY(); // ENTRY("vWordKindFetch"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  eWK eKind = pWord->eKind();
  
  m_pDataStack->vPushInt(eKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindStore(void)
{
  ENTRY(); // ENTRY("vWordKindStore"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iKind = m_pDataStack->iPopInt(__func__);
  
  pWord->vKind((eWK)iKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::locate -- locate word given a fip                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordLocate(void)
{
  ENTRY(); // ENTRY("vWordKindStore"_Stz);
  
  Cell* aHasCell = m_pDataStack->aPopFip(__func__);
  
  Word* pLocatedWord = m_oDicStack.pLocateWord(aHasCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->szWordNm();
    m_pDataStack->vPushStr(sName);
  }
  else {
    i8 iZero = 0;
    m_pDataStack->vPushInt(iZero);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name@ -- fetch name of word                                           */
/*----------------------------------------------------------------------------*/
void Step::vWordNameFetch(void)
{
  ENTRY(); // ENTRY("vWordName"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  s32 sWordName(pWord->szWordNm());
  
  m_pDataStack->vPushStr(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name! -- store name of word                                           */
/*----------------------------------------------------------------------------*/
void Step::vWordNameStore(void)
{
  ENTRY(); // ENTRY("vWordName"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));

  s32 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);

  pWord->vWordNm(sWordNm);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.new -- new of word                                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordNew(void)
{
  ENTRY(); // ENTRY("vWordNew"_Stz);
  
  s32 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);
  
  Dic* pDic;
  if (m_oDicStack.iCount())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();

  Word* pNewWord = new(pDic) Word(sWordNm);

  m_pDataStack->vPushPtr(pNewWord);
  //m_pCompiler->vCurrentWord(pNewWord);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordPrev(void)
{
  ENTRY(); // ENTRY("vWordPrev"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pPrevWord = pWord->pPrevWord();
  
  m_pDataStack->vPushPtr(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.resetRoutine -- reset the routine contents                            */
/*----------------------------------------------------------------------------*/
void Step::vWordResetRoutine(void)
{
  ENTRY(); // ENTRY("vWordResetRoutine"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  pWord->vResetRoutine();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.see -- see the encoding of a routine                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordSee(void)
{
  ENTRY(); // ENTRY("vWordSee"_Stz);
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  AM::vWordSee(pWord,&m_oDicStack);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
