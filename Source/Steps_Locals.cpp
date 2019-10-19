/*============================================================================*/
/* Steps_Locals.cpp - locals step group                                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Locals(void)
{
  vAddStep("local.fetch"_Stz,     &Step::vLocalFetch);
  vAddStep("local.store"_Stz,     &Step::vLocalStore);
  vAddStep("locals|"_Stz,         &Step::vLocalsBar,        true);
  vAddStep("locals.pop"_Stz,      &Step::vLocalsPop);
  vAddStep("locals.push"_Stz,     &Step::vLocalsPush);
  vAddStep("to"_Stz,              &Step::vLocalsTo,         true);
  
  vAddStep("params|"_Stz,         &Step::vLocalsBar,        true);
}

/*----------------------------------------------------------------------------*/
/* local.fetch -- push locals[n] onto dstack                                  */
/*----------------------------------------------------------------------------*/
void Step::vLocalFetch(Forth* pForth)
{
  ENTRY("vLocalFetch"_Stz);
  
  if ( ! pForth->m_pTask->m_pForth->m_pLocalsFrame )
    THROW_DSTACK();
  
  i8 iLocal;
  if ( ! pForth->m_pDataStack->bPop(iLocal) )
    THROW_DSTACK();
  
  Local* pLocal = pForth->m_pTask->m_pForth->m_pLocalsFrame->pLocal(iLocal);
  
  pForth->m_pDataStack->vPushLocal(pLocal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Step::vLocalStore(Forth* pForth)
{
  ENTRY("vLocalStore"_Stz);
  
  if ( ! pForth->m_pTask->m_pForth->m_pLocalsFrame )
    THROW_DSTACK();
  
  i8 iLocal;
  if ( ! pForth->m_pDataStack->bPop(iLocal) )
    THROW_DSTACK();
  
  Local* pLocal = pForth->m_pTask->m_pForth->m_pLocalsFrame->pLocal(iLocal);
  
  pForth->m_pDataStack->bPop(pLocal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- declare set of local variables                       immediate  */
/*----------------------------------------------------------------------------*/
void Step::vLocalsBar(Forth* pForth)
{
ENTRY("vLocalsBar"_Stz);
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  Dic* pDic = pForth->m_pTask->m_oDicStack.pPick(0);

  GenVar xVariable;
  
  for (;;) {
    
    // get name of local variable
    pForth->m_pTask->m_pCompiler->m_pTokeniser->vNextToken();
    
    // end of locals declaration?
    if (pForth->m_pTask->m_pCompiler->m_pTokeniser->sToken().bEqual("|"_Stz))
      break;

    // test for first use of locals|
    if ( ! pForth->m_pTask->m_pCompiler->m_nLocals ) {
      
      // create a locals dictionary
      pDic = new Dic;
      pDic->vName("<localsDic>"_Stz);
      pForth->m_pTask->m_oDicStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncode("locals.push"_Stz);
    }
    
    // create new word for local variable in current dictionary
    Word* pWordLocal = new(pDic) Word(pForth->m_pTask->m_pCompiler->m_pTokeniser->sToken());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vKind(WK_LOCAL);
    pWordLocal->vLocalNo(pForth->m_pTask->m_pCompiler->m_nLocals);
    
    // encode the word
    xVariable = pForth->m_pTask->m_pCompiler->m_nLocals;
    pWordLocal->vEncode("pushValue"_Stz,&xVariable);
    pWordLocal->vEncode("local.fetch"_Stz);
    pWordLocal->vEncode("exit_rt"_Stz);

    // initialise the local from dstack
    pCurrentWord->vEncode("pushValue"_Stz,&xVariable);
    pCurrentWord->vEncode("local.store"_Stz);
    
    pForth->m_pTask->m_pCompiler->m_nLocals++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPop(Forth* pForth)
{
ENTRY("vLocalsPop"_Stz);
  
  LocalSet* pLocalsFrame = pForth->m_pTask->m_pForth->m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_DSTACK();
  
  pForth->m_pTask->m_pForth->m_pLocalsFrame = pForth->m_pTask->m_pForth->m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPush(Forth* pForth)
{
ENTRY("vLocalsPush"_Stz);
  
  LocalSet* pLocalsFrame = new LocalSet;
  
  pLocalsFrame->vDiscardPoint(pForth->m_pReturnStack->aPeekValue());
  
  pLocalsFrame->vNext(pForth->m_pTask->m_pForth->m_pLocalsFrame);
  
  pForth->m_pTask->m_pForth->m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* to -- compile store value into local variable                    immediate */
/*----------------------------------------------------------------------------*/
void Step::vLocalsTo(Forth* pForth)
{
ENTRY("vLocalsTo"_Stz);
  
  Word* pCurrentWord = pForth->m_pTask->m_pCompiler->m_pCurrent;
  
  // get name of local from token stream
  pForth->m_pTask->m_pCompiler->m_pTokeniser->vNextToken();

  s32 sLocalName(pForth->m_pTask->m_pCompiler->m_pTokeniser->sToken());
  
  // find the word describing the local
  Word* pLocalWord = pForth->m_pTask->m_oDicStack.pFindWord(sLocalName);
  if (! pLocalWord)
    vFatal("No such local: %s\n"_Stz,sLocalName.azChar());
  
  GenVar xVar_LocalNo;
  xVar_LocalNo = pLocalWord->iLocalNo();
  
  pCurrentWord->vEncode("pushValue"_Stz,&xVar_LocalNo);
  pCurrentWord->vEncode("local.store"_Stz);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
