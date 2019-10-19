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
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Step::vWord(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWord"_Stz);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.category@ -- word category fetch                                      */
/*----------------------------------------------------------------------------*/
void Step::vWordCategoryFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordKindGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  Word* pCategory = pWord->pCategory();
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pCategory);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.category! -- word category store                                      */
/*----------------------------------------------------------------------------*/
void Step::vWordCategoryStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordKindSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  Object* pCategoryObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pCategoryObject) )
    THROW_DSTACK();
  Word* pCategory = _pWord(pCategoryObject);
  
  pWord->vCategory(pCategory);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordDump(MP_Task mpTask, Forth* pForth)
{
ENTRY("vWordDump"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.EnameSet@ -- fetch ename set                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordEnmSetFetch"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  EnameSet* pEnameSet = pWord->pEnameSet();
  
   mpTask->m_pForth->m_pDataStack->vPushPointer(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.EnameSet! -- store ename set                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordEnmSetStore"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  Object* pEnameSet_obj;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pEnameSet_obj) )
    THROW_DSTACK();
  EnameSet* pEnameSet = _pEnameSet(pEnameSet_obj);
  
  pWord->vEnameSet(pEnameSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook@ -- get exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookGet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordExitHookGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  s32 sExitHook(pWord->szExitHook());
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook! -- set exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookSet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordExitHookSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  s32 sExitHook;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sExitHook) )
    THROW_DSTACK();
  
  pWord->vExitHook(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.forget -- forget word on stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vWordForget(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordForget"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  Word* pRemovedWord = mpTask->m_oDicStack.pRemoveWord(pWord->sName());
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateGet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordImmediateGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate = pWord->bImmediate();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateSet(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordImmediateSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  i8 iImmediate;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iImmediate) )
    THROW_DSTACK();
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordIsImmediate(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordIsImmediate"_Stz);
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isIntrinsic -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Step::vWordIsIntrinsic(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordIsIntrinsic"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsPrimitive = WK_INTRINSIC == pWord->eKind() ? 1 : 0;
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindFetch(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordKindFetch"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  eWK eKind = pWord->eKind();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(eKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordKindStore"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  i8 iKind;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iKind) )
    THROW_DSTACK();
  
  pWord->vKind((eWK)iKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name@ -- fetch name of word                                           */
/*----------------------------------------------------------------------------*/
void Step::vWordNameFetch(MP_Task mpTask, Forth* pForth)
{
ENTRY("vWordName"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  s32 sWordName(pWord->sName());
  
  mpTask->m_pForth->m_pDataStack->vPushS32(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name! -- store name of word                                           */
/*----------------------------------------------------------------------------*/
void Step::vWordNameStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordName"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);

  s32 sName;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sName) )
    THROW_DSTACK();

  pWord->vName(sName);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordPrev(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vWordPrev"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pWordObject) )
    THROW_DSTACK();
  Word* pWord = _pWord(pWordObject);
  
  Word* pPrevWord = pWord->pPrevWord();
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pPrevWord);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
