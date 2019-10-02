/*============================================================================*/
/* Steps_Word.cpp - word step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Word(void)
{
  vAddStep("word::this"_Stz,       &Step::vWordThis);
  
  vAddStep("word.addEnmValStr"_Stz,&Step::vWordAddEnmValStr);
  vAddStep("word.dump"_Stz,        &Step::vWordDump);
  vAddStep("word.enmSet@"_Stz,     &Step::vWordEnmSetFetch);
  vAddStep("word.enmSet!"_Stz,     &Step::vWordEnmSetStore);
  vAddStep("word.exitHook@"_Stz,   &Step::vWordExitHookGet);
  vAddStep("word.exitHook!"_Stz,   &Step::vWordExitHookSet);
  vAddStep("word.forget"_Stz,      &Step::vWordForget);
  vAddStep("word.immediate@"_Stz,  &Step::vWordImmediateGet);
  vAddStep("word.immediate!"_Stz,  &Step::vWordImmediateSet);
  vAddStep("word.isImmediate"_Stz, &Step::vWordIsImmediate);
  vAddStep("word.isIntrinsic"_Stz, &Step::vWordIsIntrinsic);
  vAddStep("word.kind@"_Stz,       &Step::vWordKindGet);
  vAddStep("word.kind!"_Stz,       &Step::vWordKindSet);
  vAddStep("word.name"_Stz,        &Step::vWordName);
  vAddStep("word.prev"_Stz,        &Step::vWordPrev);
  vAddStep("word.type@"_Stz,       &Step::vWordTypeGet);
  vAddStep("word.type!"_Stz,       &Step::vWordTypeSet);
}

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Step::vWord(MP_Task mpTask)
{
  ENTRY("vWord"_Stz);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.addEnmValStr -- attach an ename set                                   */
/*----------------------------------------------------------------------------*/
void Step::vWordAddEnmValStr(MP_Task mpTask)
{
  ENTRY("vWordAddEnmValStr"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordAddEnmValStr"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  s32 sValue;
  if ( ! mpTask->m_pDataStack->bPop(sValue) )
    THROW_INVALID_DSTACK("word.addEnmValStr"_Stz);

  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("word.addEnmValStr"_Stz);
  
  EnmSet* pEnmSet = pWord->pEnmSet();
  
  pEnmSet->vAdd(sName,sValue,TN_STRING);
  pEnmSet->vPrint();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Step::vWordDump(MP_Task mpTask)
{
ENTRY("vWordDump"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordDump"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  pWord->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.enmSet@ -- fetch ename set                                            */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetFetch(MP_Task mpTask)
{
  ENTRY("vWordEnmSetFetch"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordEnmSetFetch"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  EnmSet* pEnmSet = pWord->pEnmSet();
  
   mpTask->m_pDataStack->vPushPointer(pEnmSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.enmSet! -- store ename set                                            */
/*----------------------------------------------------------------------------*/
void Step::vWordEnmSetStore(MP_Task mpTask)
{
  ENTRY("vWordEnmSetStore"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordEnmSetStore"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  Object* pEnmSet_obj;
  if ( ! mpTask->m_pDataStack->bPop(pEnmSet_obj) )
    THROW_INVALID_DSTACK("vWordEnmSetStore"_Stz);
  EnmSet* pEnmSet = _pEnmSet(pEnmSet_obj);
  
  pWord->vEnmSet(pEnmSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook@ -- get exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookGet(MP_Task mpTask)
{
  ENTRY("vWordExitHookGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.exitHook@"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  s32 sExitHook(pWord->szExitHook());
  
  mpTask->m_pDataStack->vPushS32(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.exitHook! -- set exit hook of word                                    */
/*----------------------------------------------------------------------------*/
void Step::vWordExitHookSet(MP_Task mpTask)
{
  ENTRY("vWordExitHookSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.exitHook!"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  s32 sExitHook;
  if ( ! mpTask->m_pDataStack->bPop(sExitHook) )
    THROW_INVALID_DSTACK("word.exitHook!"_Stz);
  
  pWord->vExitHook(sExitHook);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.forget -- forget word on stack                                        */
/*----------------------------------------------------------------------------*/
void Step::vWordForget(MP_Task mpTask)
{
  ENTRY("vWordForget"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordForget"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  ForthWord* pRemovedWord = mpTask->m_oDicStack.pRemoveWord(pWord->sName());
  ASSERT(pRemovedWord);
  
  delete pRemovedWord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate@ -- get immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateGet(MP_Task mpTask)
{
  ENTRY("vWordImmediateGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate@"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  i8 iImmediate = pWord->bImmediate();
  
  mpTask->m_pDataStack->vPushI8(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.immediate! -- set immediate flag of word                              */
/*----------------------------------------------------------------------------*/
void Step::vWordImmediateSet(MP_Task mpTask)
{
  ENTRY("vWordImmediateSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.immediate!"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  i8 iImmediate;
  if ( ! mpTask->m_pDataStack->bPop(iImmediate) )
    THROW_INVALID_DSTACK("word.immediate!"_Stz);
  
  pWord->vImmediate(iImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Step::vWordIsImmediate(MP_Task mpTask)
{
  ENTRY("vWordIsImmediate"_Stz);
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("isImmediate"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.isIntrinsic -- is word a primitive?                                   */
/*----------------------------------------------------------------------------*/
void Step::vWordIsIntrinsic(MP_Task mpTask)
{
  ENTRY("vWordIsIntrinsic"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.isIntrinsic"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  i8 iIsPrimitive = WT_INTRINSIC == pWord->eWordType() ? 1 : 0;
  
  mpTask->m_pDataStack->vPushI8(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindGet(MP_Task mpTask)
{
ENTRY("vWordKindGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindGet"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  ForthWord* pWordKind = pWord->pKind();
  
  mpTask->m_pDataStack->vPushPointer(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordKindSet(MP_Task mpTask)
{
ENTRY("vWordKindSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordKindSet"_Stz);
  ForthWord* pWordKind = _pWord(pWordObject);
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Step::vWordName(MP_Task mpTask)
{
ENTRY("vWordName"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("word.name"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  s32 sWordName(pWord->sName());
  
  mpTask->m_pDataStack->vPushS32(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.prev -- push prev word                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordPrev(MP_Task mpTask)
{
  ENTRY("vWordPrev"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("wordPrev"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  ForthWord* pPrevWord = pWord->pPrevWord();
  
  mpTask->m_pDataStack->vPushPointer(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word::this -- push current word                                            */
/*----------------------------------------------------------------------------*/
void Step::vWordThis(MP_Task mpTask)
{
ENTRY("vWordThis"_Stz);
  
  ForthWord* pWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  mpTask->m_pDataStack->vPushPointer(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type@ -- word type get                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordTypeGet(MP_Task mpTask)
{
  ENTRY("vWordTypeGet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeGet"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  eWT eWordType = pWord->eWordType();
  
  mpTask->m_pDataStack->vPushI8(eWordType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.type! -- word type set                                                */
/*----------------------------------------------------------------------------*/
void Step::vWordTypeSet(MP_Task mpTask)
{
  ENTRY("vWordTypeSet"_Stz);
  
  Object* pWordObject;
  if ( ! mpTask->m_pDataStack->bPop(pWordObject) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Stz);
  ForthWord* pWord = _pWord(pWordObject);
  
  i8 iWordType;
  
  if ( ! mpTask->m_pDataStack->bPop(iWordType) )
    THROW_INVALID_DSTACK("vWordTypeSet"_Stz);
  ForthWord* pWordKind = _pWord(pWordObject);
  
  pWord->vWordType((eWT)iWordType);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
