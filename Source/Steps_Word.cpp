/*============================================================================*/
/* Steps_Word.cpp - word step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Word(void)
{
  vAddStep("word.category@"_Stz,   &Step::vWordCategoryFetch);
  vAddStep("word.category!"_Stz,   &Step::vWordCategoryStore);
  vAddStep("word.dump"_Stz,        &Step::vWordDump);
  vAddStep("word.EnameSet@"_Stz,   &Step::vWordEnmSetFetch);
  vAddStep("word.EnameSet!"_Stz,   &Step::vWordEnmSetStore);
  vAddStep("word.exitHook@"_Stz,   &Step::vWordExitHookGet);
  vAddStep("word.exitHook!"_Stz,   &Step::vWordExitHookSet);
  vAddStep("word.forget"_Stz,      &Step::vWordForget);
  vAddStep("word.immediate@"_Stz,  &Step::vWordImmediateGet);
  vAddStep("word.immediate!"_Stz,  &Step::vWordImmediateSet);
  vAddStep("word.isImmediate"_Stz, &Step::vWordIsImmediate);
  vAddStep("word.isIntrinsic"_Stz, &Step::vWordIsIntrinsic);
  vAddStep("word.kind@"_Stz,       &Step::vWordKindFetch);
  vAddStep("word.kind!"_Stz,       &Step::vWordKindStore);
  vAddStep("word.name@"_Stz,       &Step::vWordNameFetch);
  vAddStep("word.name!"_Stz,       &Step::vWordNameStore);
  vAddStep("word.prev"_Stz,        &Step::vWordPrev);
  vAddStep("word.see"_Stz,         &Step::vWordSee);
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
  Object* pCategoryObject = m_pDataStack->pPopPtr(__func__);
  Word* pCategory = _pWord(pCategoryObject);
  
  pWord->vCategory(pCategory);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordDump(void)
{
  ENTRY(); // ENTRY("vWordDump"_Stz);
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.EnameSet@ -- fetch ename set                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetFetch(void)
{
  ENTRY(); // ENTRY("vWordEnmSetFetch"_Stz);
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
  Object* pEnameSet_obj = m_pDataStack->pPopPtr(__func__);
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  pWord->vEnameSet(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook@ -- get exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookGet(void)
{
  ENTRY(); // ENTRY("vWordExitHookGet"_Stz);
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
  s32 sExitHook;
  m_pDataStack->vPopStr(sExitHook,__func__);
  
  pWord->vExitHook(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.forget -- forget word on stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vWordForget(void)
{
  ENTRY(); // ENTRY("vWordForget"_Stz);
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate = m_pDataStack->iPopInt(__func__);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordIsImmediate(void)
{
  ENTRY(); // ENTRY("vWordIsImmediate"_Stz);
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
  i8 iKind = m_pDataStack->iPopInt(__func__);
  
  pWord->vKind((eWK)iKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name@ -- fetch name of word                                           */
/*----------------------------------------------------------------------------*/
void Step::vWordNameFetch(void)
{
  ENTRY(); // ENTRY("vWordName"_Stz);
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
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
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);

  s32 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);

  pWord->vWordNm(sWordNm);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordPrev(void)
{
  ENTRY(); // ENTRY("vWordPrev"_Stz);
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
  Word* pPrevWord = pWord->pPrevWord();
  
  m_pDataStack->vPushPtr(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.see -- see the encoding of a routine                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordSee(void)
{
  ENTRY(); // ENTRY("vWordSee"_Stz);
  
  Object* pWordObject = m_pDataStack->pPopPtr(__func__);
  Word* pWord = _pWord(pWordObject);
  
  AM::vWordSee(pWord,&m_oDicStack);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
