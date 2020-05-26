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
  vAddStep("word::xcheckInxPtr"_Stz,&Step::vWordXcheckInxPtr);
  
  vAddStep("word.allot"_Stz,        &Step::vWordAllot);
  vAddStep("word.breakPt@"_Stz,     &Step::vWordBreakPtFetch);
  vAddStep("word.breakPt!"_Stz,     &Step::vWordBreakPtStore);
  vAddStep("word.category@"_Stz,    &Step::vWordCategoryFetch);
  vAddStep("word.category!"_Stz,    &Step::vWordCategoryStore);
  vAddStep("word.dump"_Stz,         &Step::vWordDump);
  vAddStep("word.embObject"_Stz,    &Step::vWordEmbObject);
  vAddStep("word.exitHook@"_Stz,    &Step::vWordExitHookGet);
  vAddStep("word.exitHook!"_Stz,    &Step::vWordExitHookSet);
  vAddStep("word.immediate@"_Stz,   &Step::vWordImmediateGet);
  vAddStep("word.immediate!"_Stz,   &Step::vWordImmediateSet);
  vAddStep("word.include@"_Stz,     &Step::vWordIncludeFetch);
  vAddStep("word.include!"_Stz,     &Step::vWordIncludeStore);
  vAddStep("word.isImmediate"_Stz,  &Step::vWordIsImmediate);
  vAddStep("word.isIntrinsic"_Stz,  &Step::vWordIsIntrinsic);
  vAddStep("word.kind@"_Stz,        &Step::vWordKindFetch);
  vAddStep("word.kind!"_Stz,        &Step::vWordKindStore);
  vAddStep("word.name@"_Stz,        &Step::vWordNameFetch);
  vAddStep("word.name!"_Stz,        &Step::vWordNameStore);
  vAddStep("word.object@"_Stz,      &Step::vWordObjectFetch);
  vAddStep("word.object!"_Stz,      &Step::vWordObjectStore);
  vAddStep("word.prev"_Stz,         &Step::vWordPrev);
  vAddStep("word.resetRoutine"_Stz, &Step::vWordResetRoutine);
  vAddStep("word.see"_Stz,          &Step::vWordSee);
  vAddStep("word.tagSet@"_Stz,      &Step::vWordEnmSetFetch);
  vAddStep("word.tagSet!"_Stz,      &Step::vWordEnmSetStore);
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Step::vWord(void)
{
  ENTRY();
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.allotObject -- allot an embedded object of n bytes in wsp             */
/*----------------------------------------------------------------------------*/
void Step::vWordAllot(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 nBytes = m_pDataStack->iPopInt(__func__);
  RASSERT(nBytes);

  Object* pEmbObject = (Object*)pWord->ayAllot(nBytes);
  ASSERT(pEmbObject);
  
  pEmbObject->vMagic();
  pEmbObject->vClass(CN_NONE);

  m_pDataStack->vPushPtr(pEmbObject);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.breakPt@ -- fetch m_bBreakPt of word                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordBreakPtFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  bool bBreakPt = pWord->bBreakPt();
  
  m_pDataStack->vPushInt(bBreakPt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.breakPt! -- store m_bBreakPt of word                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordBreakPtStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iBreakPt = m_pDataStack->iPopInt(__func__);
  
  pWord->vBreakPt(iBreakPt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.category@ -- word category fetch                                      */
/*----------------------------------------------------------------------------*/
void Step::vWordCategoryFetch(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pCategory = _pWord(m_pDataStack->pPopPtr(__func__));
  
//  if (pCategory->szWordNm()->bSame("proccatg"_Stz))
//    Correctness::vDebugPoint();
//  if (pWord->szWordNm()->bSame("proccatg"_Stz))
//    Correctness::vDebugPoint();
//
//  vTrace("Word %s : Category %s\n"_Stz,
//         pWord->szWordNm(),pCategory->szWordNm());

  pWord->vCategory(pCategory);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordDump(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.embObject -- get embedded object in wsp of word                       */
/*----------------------------------------------------------------------------*/
void Step::vWordEmbObject(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  char* aWsp = pWord->aWspArea();
  
  Object* pEmbObject = reinterpret_cast<Object*>(aWsp);
  
  m_pDataStack->vPushPtr(pEmbObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.TagSet@ -- fetch ename set                                            */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  TagSet* pWrdTagSet = pWord->pWrdTagSet();
  
   m_pDataStack->vPushPtr(pWrdTagSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.TagSet! -- store ename set                                            */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  TagSet* pWrdTagSet = _pTagSet(m_pDataStack->pPopPtr(__func__));
  
  pWord->vWrdTagSet(pWrdTagSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook@ -- get exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookGet(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  s32 sExitHook;
  m_pDataStack->vPopStr(sExitHook,__func__);
  
  pWord->vExitHook(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::find -- find word given its name                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordFind(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pRemovedWord = m_oDicStack.pRemoveWord(pWord->szWordNm());
  ASSERT(pRemovedWord);
  
  if (AM::bVerbose())
    m_pReturnStack->vDump(&m_oDicStack,"ReturnStack:"_Stz);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateGet(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iImmediate = m_pDataStack->iPopInt(__func__);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.include@ -- word include group fetch                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordIncludeFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pInclude = pWord->pInclude();
  
  m_pDataStack->vPushPtr(pInclude);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.include! -- word include group store                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordIncludeStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Word* pInclude = _pWord(m_pDataStack->pPopPtr(__func__));
  
  pWord->vInclude(pInclude);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isImmediate -- is word immediate?                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordIsImmediate(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iIsPrimitive = WK_INTRINSIC == pWord->eWord() ? 1 : 0;
  
  m_pDataStack->vPushInt(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  eWK eWord = pWord->eWord();
  
  m_pDataStack->vPushInt(eWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 iWordKind = m_pDataStack->iPopInt(__func__);
  
  pWord->vWord((eWK)iWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::locate -- locate word given a fip                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordLocate(void)
{
  ENTRY();
  
  Cell* aHasCell = m_pDataStack->aPopFip(__func__);
  
  Word* pLocatedWord = m_oDicStack.pLocateRoutineWord(aHasCell);
  
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
  ENTRY();
  
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
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));

  s256 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);

  pWord->vWordNm(sWordNm);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::new -- new of word                                                   */
/*----------------------------------------------------------------------------*/
void Step::vWordNew(void)
{
  ENTRY();
  
  s256 sWordNm;
  m_pDataStack->vPopStr(sWordNm,__func__);
  
  Dic* pDic;
  if (m_oDicStack.iCount())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();

  Word* pNewWord = new(pDic) Word(sWordNm);
  
  if (m_pInput) {
    Word* pInclude = m_pInput->pUseInclude();
    pNewWord->vInclude(pInclude);
  }
  
  m_pDataStack->vPushPtr(pNewWord);
  //m_pCompiler->vCurrentWord(pNewWord);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.object@ -- fetch m_pObject of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordObjectFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Object* pObject = pWord->pObject();
  
  m_pDataStack->vPushPtr(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.object! -- store m_pObject of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordObjectStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  Object* pObject = m_pDataStack->pPopPtr(__func__);
  
  pWord->vObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordPrev(void)
{
  ENTRY();
  
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
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  pWord->vResetRoutine();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.see -- see the encoding of a routine                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordSee(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  AM::vWordSee(pWord,&m_oDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::xcheck -- cross check this word                                      */
/*----------------------------------------------------------------------------*/
void Step::vWordXcheckInxPtr(void)
{
  ENTRY();
  
  Word* pTestWord = _pWord(m_pDataStack->pPopPtr(__func__));
    
  Word* pExeWord  = Routine::pToWord(m_aInxPtr);

  ASSERT(pExeWord == pTestWord);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
