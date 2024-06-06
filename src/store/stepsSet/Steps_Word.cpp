/*============================================================================*/
/* Steps_Word.cpp - word step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "TagConstants.h"


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
  vAddStep("word.categoryString@"_Stz,&Step::vWordCategoryString);
  vAddStep("word.dump"_Stz,         &Step::vWordDump);
  vAddStep("word.embObject"_Stz,    &Step::vWordEmbObject);
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
  vAddStep("word.next"_Stz,         &Step::vWordNext);
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
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  i8 nBytes = m_oDataStack.iPopInt(__func__);
  RASSERT(nBytes);

  Object* pEmbObject = (Object*)pWord->ayAllot(nBytes);
  ASSERT(pEmbObject);
  
  pEmbObject->vSetMagic();
  pEmbObject->vSetClassNo(CN_NONE);

  m_oDataStack.vPushPtr(pEmbObject);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.breakPt@ -- fetch m_bBreakPt of word                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordBreakPtFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  bool bBreakPt = pWord->bBreakPt();
  
  m_oDataStack.vPushInt(bBreakPt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.breakPt! -- store m_bBreakPt of word                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordBreakPtStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  i8 iBreakPt = m_oDataStack.iPopInt(__func__);
  
  pWord->vBreakPt(iBreakPt);
  
  vDebugPoint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.category@ -- word category enum fetch                                 */
/*----------------------------------------------------------------------------*/
void Step::vWordCategoryFetch(void)
{
  ENTRY();

  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));

  m_oDataStack.vPushInt((pWord->eCategoryEnum()));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.category! -- word category enum store                                 */
/*----------------------------------------------------------------------------*/
void Step::vWordCategoryStore(void)
{
  ENTRY();

  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));

  eWC eCategory = (eWC) m_oDataStack.iPopInt(__func__);

  pWord->vCategoryEnum(eCategory);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.categoryString@ -- word category enum fetch str                     */
/*----------------------------------------------------------------------------*/
void Step::vWordCategoryString(void)
{
  ENTRY();

  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));

	s256 eCategory = aWC[pWord->eCategoryEnum()];

  m_oDataStack.vPushStr(eCategory);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordDump(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.embObject -- get embedded object in wsp of word                       */
/*----------------------------------------------------------------------------*/
void Step::vWordEmbObject(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  char* aWsp = pWord->aWspArea();
  
  Object* pEmbObject = reinterpret_cast<Object*>(aWsp);
  
  m_oDataStack.vPushPtr(pEmbObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.TagSet@ -- fetch ename set                                            */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  TagSet* pWrdTagSet = pWord->pWrdTagSet();
  
   m_oDataStack.vPushPtr(pWrdTagSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.TagSet! -- store ename set                                            */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  TagSet* pWrdTagSet = _pTagSet(m_oDataStack.pPopPtr(__func__));
  
  pWord->vWrdTagSet(pWrdTagSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook@ -- get exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookGet(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));

	s256 sExitHook(pWord->szExitHook());
  
  m_oDataStack.vPushStr(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook! -- set exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookSet(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));

	s256 sExitHook;
  m_oDataStack.vPopStr(sExitHook, __func__);
  
  pWord->vExitHook(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::find -- find word given its name                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordFind(void)
{
  ENTRY();
  
  s256 sWordNm;
  m_oDataStack.vPopStr(sWordNm, __func__);
    
  Word* pWordFound = m_oDicStack.pFindWord(sWordNm);
  
  m_oDataStack.vPushPtr(pWordFound);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::delete -- forget word on stack                                       */
/*----------------------------------------------------------------------------*/
void Step::vWordDelete(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  Word* pRemovedWord = m_oDicStack.pRemoveWord(pWord->szWordNm());
  ASSERT(pRemovedWord);
  
  if (AM::TRACE::bVerbose(C_VB_WORD))
    m_oReturnStack.vDump(&m_oDicStack, "ReturnStack:"_Stz);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateGet(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  i8 iImmediate = pWord->bImmediate();
  
  m_oDataStack.vPushInt(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateSet(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  i8 iImmediate = m_oDataStack.iPopBol(__func__);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isImmediate -- is word immediate?                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordIsImmediate(void)
{
  ENTRY();
  
  // we should just modify the pointers rather than actually pop
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  m_oDataStack.vPushInt(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isIntrinsic -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Step::vWordIsIntrinsic(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  i8 iIsPrimitive = WK_INTRINSIC == pWord->eWord() ? 1 : 0;
  
  m_oDataStack.vPushInt(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  eWK eWord = pWord->eWord();
  
  m_oDataStack.vPushInt(eWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  i8 iWordKind = m_oDataStack.iPopInt(__func__);
  
  pWord->vWord((eWK)iWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::locate -- locate word given a fip                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordLocate(void)
{
  ENTRY();
  
  CodeCell* aHasCell = m_oDataStack.aPopFip(__func__);
  
  Word* pLocatedWord = m_oDicStack.pLocateRoutine(aHasCell);
  
  if (pLocatedWord) {
    s256 sName;
    sName = pLocatedWord->szWordNm();
    m_oDataStack.vPushStr(sName);
  }
  else {
    i8 iZero = 0;
    m_oDataStack.vPushInt(iZero);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name@ -- fetch name of word                                           */
/*----------------------------------------------------------------------------*/
void Step::vWordNameFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  s256 sWordName(pWord->szWordNm());
  
  m_oDataStack.vPushStr(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name! -- store name of word                                           */
/*----------------------------------------------------------------------------*/
void Step::vWordNameStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));

  s256 sWordNm;
  m_oDataStack.vPopStr(sWordNm, __func__);

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
  m_oDataStack.vPopStr(sWordNm, __func__);
  
  Dic* pDic;
  if (m_oDicStack.nItems())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::DIC::pRoot();

  Word* pNewWord = new(pDic) Word(sWordNm);
    
  m_oDataStack.vPushPtr(pNewWord);
  //m_pSenCompiler->vCurrentWord(pNewWord);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.next -- push next word                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordNext(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  Word* pNextWord = pWord->pNextWord();
  
  m_oDataStack.vPushPtr(pNextWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.object@ -- fetch m_pObject of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordObjectFetch(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  Object* pObject = pWord->pObject();
  
  m_oDataStack.vPushPtr(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.object! -- store m_pObject of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordObjectStore(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  Object* pObject = m_oDataStack.pPopPtr(__func__);
  
  pWord->vObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordPrev(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  Word* pPrevWord = pWord->pPrevWord();
  
  m_oDataStack.vPushPtr(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.resetRoutine -- reset the routine contents                            */
/*----------------------------------------------------------------------------*/
void Step::vWordResetRoutine(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  pWord->vResetCodeBlock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.see -- see the encoding of a routine                                  */
/*----------------------------------------------------------------------------*/
void Step::vWordSee(void)
{
  ENTRY();
  
  Word* pWord = _pWord(m_oDataStack.pPopPtr(__func__));
  
  AM::WORD::vDisassemble(pWord,&m_oDicStack);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::xcheck -- cross check this word                                      */
/*----------------------------------------------------------------------------*/
void Step::vWordXcheckInxPtr(void)
{
  ENTRY();
  
  Word* pTestWord = _pWord(m_oDataStack.pPopPtr(__func__));
  UNUSED(pTestWord);

  Word* pExeWord  = CodeBlock::pToWord(m_aInxPtr);

  ASSERT(pExeWord == pTestWord);
  UNUSED(pExeWord);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
