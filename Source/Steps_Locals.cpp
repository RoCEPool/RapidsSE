/*============================================================================*/
/* Steps_Locals.cpp - locals step group                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
void Step::vLocalFetch(MP_Task mpTask)
{
  ENTRY("vLocalFetch"_Stz);
  
  if ( ! mpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("local.fetch"_Stz);
  
  i8 iLocal;
  if ( ! mpTask->m_pDataStack->bPop(iLocal) )
    THROW_INVALID_DSTACK("local.fetch"_Stz);
  
  Register* pRegister = mpTask->m_pLocalsFrame->pRegister(iLocal);
  
  mpTask->m_pDataStack->vPushValue(pRegister);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop dstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Step::vLocalStore(MP_Task mpTask)
{
  ENTRY("vLocalStore"_Stz);
  
  if ( ! mpTask->m_pLocalsFrame )
    THROW_INVALID_DSTACK("localStore"_Stz);
  
  i8 iLocal;
  if ( ! mpTask->m_pDataStack->bPop(iLocal) )
    THROW_INVALID_DSTACK("localStore"_Stz);
  
  Register* pRegister = mpTask->m_pLocalsFrame->pRegister(iLocal);
  
  mpTask->m_pDataStack->bPop(pRegister);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- declare set of local variables                       immediate  */
/*----------------------------------------------------------------------------*/
void Step::vLocalsBar(MP_Task mpTask)
{
ENTRY("vLocalsBar"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  Dic* pDic = mpTask->m_oPathStack.pPick(0);

  GenVar xVariable;
  
  for (;;) {
    
    // get name of local variable
    mpTask->m_pCompiler->m_pTokeniser->vNextToken();
    
    // end of locals declaration?
    if (mpTask->m_pCompiler->m_pTokeniser->sToken().bEqual("|"_Stz))
      break;

    // test for first use of locals|
    if ( ! mpTask->m_pCompiler->m_nVariables ) {
      
      // create a locals dictionary
      pDic = new Dic;
      pDic->vName("<localsDic>"_Stz);
      mpTask->m_oPathStack.vPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncode("locals.push"_Stz);
    }
    
    // create new word for local variable in current dictionary
    Word* pWordLocal = new(pDic) Word(mpTask->m_pCompiler->m_pTokeniser->sToken());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vWordType(WT_VARIABLE);
    pWordLocal->vVariable(mpTask->m_pCompiler->m_nVariables);
    
    // encode the word
    xVariable = mpTask->m_pCompiler->m_nVariables;
    pWordLocal->vEncode("pushValue"_Stz,&xVariable);
    pWordLocal->vEncode("local.fetch"_Stz);
    pWordLocal->vEncode("exit"_Stz);

    // initialise the local from dstack
    pCurrentWord->vEncode("pushValue"_Stz,&xVariable);
    pCurrentWord->vEncode("local.store"_Stz);
    
    mpTask->m_pCompiler->m_nVariables++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPop(MP_Task mpTask)
{
ENTRY("vLocalsPop"_Stz);
  
  RegFrame* pLocalsFrame = mpTask->m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_INVALID_DSTACK("locals.pop"_Stz);
  
  mpTask->m_pLocalsFrame = mpTask->m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Step::vLocalsPush(MP_Task mpTask)
{
ENTRY("vLocalsPush"_Stz);
  
  RegFrame* pLocalsFrame = new RegFrame;
  
  pLocalsFrame->vStackPtr(mpTask->m_pReturnStack->aPeekValue());
  
  pLocalsFrame->vNext(mpTask->m_pLocalsFrame);
  
  mpTask->m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* to -- compile store value into local variable                    immediate */
/*----------------------------------------------------------------------------*/
void Step::vLocalsTo(MP_Task mpTask)
{
ENTRY("vLocalsTo"_Stz);
  
  Word* pCurrentWord = mpTask->m_pCompiler->m_pCurrentWord;
  
  // get name of local
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();

  Word* pWordVariable =
    mpTask->m_oPathStack.pFindWord(mpTask->m_pCompiler->m_pTokeniser->sToken());
  ASSERT(pWordVariable);
 
  GenVar xVariable;
  xVariable = pWordVariable->iVariable();
  
  pCurrentWord->vEncode("pushValue"_Stz,&xVariable);
  pCurrentWord->vEncode("local.store"_Stz);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
