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
void Step::vLocalFetch(void)
{
  ENTRY("vLocalFetch"_Stz);
  
  if ( ! m_pLocalsFrame )
    THROW_DSTACK();
  
  i8 iLocal;
  if ( ! m_pDataStack->bPopI8(iLocal) )
    THROW_DSTACK();
  
  Local* pLocal = m_pLocalsFrame->pLocal(iLocal);
  
  m_pDataStack->vPushLocal(pLocal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Step::vLocalStore(void)
{
  ENTRY("vLocalStore"_Stz);
  
  if ( ! m_pLocalsFrame )
    THROW_DSTACK();
  
  i8 iLocal;
  if ( ! m_pDataStack->bPopI8(iLocal) )
    THROW_DSTACK();
  
  Local* pLocal = m_pLocalsFrame->pLocal(iLocal);
  
  m_pDataStack->bPopLocal(pLocal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- declare set of local variables                       immediate  */
/*----------------------------------------------------------------------------*/
void Step::vLocalsBar(void)
{
ENTRY("vLocalsBar"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  Dic* pDic = m_pTask->m_oDicStack.pPick(0);

  GenVar xVariable;
  
  for (;;) {
    
    // get name of local variable
    m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();
    
    // end of locals declaration?
    if (m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken().bEqual("|"_Stz))
      break;

    // test for first use of locals|
    if ( ! m_pTask->m_pForth->m_pCompiler->m_nLocals ) {
      
      // create a locals dictionary
      pDic = new Dic;
      pDic->vName("<localsDic>"_Stz);
      m_pTask->m_oDicStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncode("locals.push"_Stz);
    }
    
    // create new word for local variable in current dictionary
    Word* pWordLocal = new(pDic) Word(m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vKind(WK_LOCAL);
    pWordLocal->vLocalNo(m_pTask->m_pForth->m_pCompiler->m_nLocals);
    
    // encode the word
    xVariable = m_pTask->m_pForth->m_pCompiler->m_nLocals;
    pWordLocal->vEncode("pushValue"_Stz,&xVariable);
    pWordLocal->vEncode("local.fetch"_Stz);
    pWordLocal->vEncode("exit_rt"_Stz);

    // initialise the local from dstack
    pCurrentWord->vEncode("pushValue"_Stz,&xVariable);
    pCurrentWord->vEncode("local.store"_Stz);
    
    m_pTask->m_pForth->m_pCompiler->m_nLocals++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPop(void)
{
ENTRY("vLocalsPop"_Stz);
  
  LocalSet* pLocalsFrame = m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_DSTACK();
  
  m_pLocalsFrame = m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPush(void)
{
ENTRY("vLocalsPush"_Stz);
  
  LocalSet* pLocalsFrame = new LocalSet;
  
  pLocalsFrame->vDiscardPoint(m_pReturnStack->aPeekValue());
  
  pLocalsFrame->vNext(m_pLocalsFrame);
  
  m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* to -- compile store value into local variable                    immediate */
/*----------------------------------------------------------------------------*/
void Step::vLocalsTo(void)
{
ENTRY("vLocalsTo"_Stz);
  
  Word* pCurrentWord = m_pTask->m_pForth->m_pCompiler->m_pCurrent;
  
  // get name of local from token stream
  m_pTask->m_pForth->m_pCompiler->m_pTokeniser->vNextToken();

  s32 sLocalName(m_pTask->m_pForth->m_pCompiler->m_pTokeniser->sToken());
  
  // find the word describing the local
  Word* pLocalWord = m_pTask->m_oDicStack.pFindWord(sLocalName);
  if (! pLocalWord)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "No such local: %s\n"_Stz,sLocalName.azChar());
  
  GenVar xVar_LocalNo;
  xVar_LocalNo = pLocalWord->iLocalNo();
  
  pCurrentWord->vEncode("pushValue"_Stz,&xVar_LocalNo);
  pCurrentWord->vEncode("local.store"_Stz);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
