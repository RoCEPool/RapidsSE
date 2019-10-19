/*============================================================================*/
/* Steps_Input.cpp - input step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Input(void)
{
  vAddStep("input.close"_Stz,     &Step::vInputClose);
  vAddStep("input.depth"_Stz,     &Step::vInputDepth);
  vAddStep("input.next"_Stz,      &Step::vInputNext);
  vAddStep("input.open"_Stz,      &Step::vInputOpen);
  vAddStep("input.pop"_Stz,       &Step::vInputPop);
  vAddStep("input.push"_Stz,      &Step::vInputPush);
  vAddStep("input.stdin"_Stz,     &Step::vInputStdin);
  vAddStep("input.text"_Stz,      &Step::vInputText);
  vAddStep("input.token"_Stz,     &Step::vInputToken);
  vAddStep("input.type"_Stz,      &Step::vInputType);
  vAddStep("input.unknown"_Stz,   &Step::vInputUnknown);

  vAddStep("next-token"_Stz,          &Step::vTokenColon);
}

/*----------------------------------------------------------------------------*/
/* input.close -- close current input stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputClose(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputClose"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vUseEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.depth -- push current depth of input stream onto data stack         */
/*----------------------------------------------------------------------------*/
void Step::vInputDepth(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputDepth"_Stz);
  
  i8 iDepth = mpTask->m_pCompiler->m_pTokeniser->iDepth();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.next -- get next input token from current input stream               */
/*----------------------------------------------------------------------------*/
void Step::vInputNext(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputNext"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }
  
  char* aToken = mpTask->m_pCompiler->m_pTokeniser->sToken().azChar();
  
  eTN eToken = mpTask->m_pCompiler->m_pTokeniser->eNextToken(aToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.open -- open new input using filepath on data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vInputOpen(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputOpen"_Stz);
  
  s256 sFilePath;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sFilePath) )
    THROW_DSTACK();
  
  bool bOpened = mpTask->m_pCompiler->m_pTokeniser->bUseFile(sFilePath);
  
  if ( ! bOpened )
    vThrow(__func__,__FILE__,__LINE__,"No such file: %s"_Stz,sFilePath.azChar());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.pop -- pop to previous input stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputPop(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputPop"_Stz);
  
  bool bPopped = mpTask->m_pCompiler->m_pTokeniser->bPop();
  ASSERT(bPopped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.push -- push input stream, allowing for new input stream             */
/*----------------------------------------------------------------------------*/
void Step::vInputPush(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputPush"_Stz);
  
  if ( ! mpTask->m_pCompiler )
    mpTask->m_pCompiler = new Compiler;
  
  mpTask->m_pCompiler->m_pTokeniser->vPush();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.stdin -- use stdin as new input stream                               */
/*----------------------------------------------------------------------------*/
void Step::vInputStdin(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputStdin"_Stz);
  
  if ( ! mpTask->m_pCompiler )
    mpTask->m_pCompiler = new Compiler;
  
  mpTask->m_pCompiler->m_pTokeniser->vUseStdin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.text -- use string as new input stream                               */
/*----------------------------------------------------------------------------*/
void Step::vInputText(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputText"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }
  
  s48K sInput;                                             //! NEED STACK CHECKS
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sInput) )
    THROW_DSTACK();
    
  mpTask->m_pCompiler->m_pTokeniser->vUseString(sInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.token -- push current token of input stream onto data stack          */
/*----------------------------------------------------------------------------*/
void Step::vInputToken(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputToken"_Stz);
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.type -- push current token type onto data stack                      */
/*----------------------------------------------------------------------------*/
void Step::vInputType(MP_Task mpTask, Forth* pForth)
{
ENTRY("vInputType"_Stz);
  
  eTN eTokenType = mpTask->m_pCompiler->m_pTokeniser->eToken();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(eTokenType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.unknown -- unknown current token                                     */
/*----------------------------------------------------------------------------*/
void Step::vInputUnknown(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vInputUnknown"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("Unknown token: %s\n"_Stz,
         mpTask->m_pCompiler->m_pTokeniser->sToken().azChar()
         );
  vTrace("\n"_Stz);

  AM::vBootError(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next-token -- token colon                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTokenColon(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vTokenColon"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
