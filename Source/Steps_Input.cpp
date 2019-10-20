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
void Step::vInputClose(void)
{
ENTRY("vInputClose"_Stz);
  
  m_pTask->m_pCompiler->m_pTokeniser->vUseEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.depth -- push current depth of input stream onto data stack         */
/*----------------------------------------------------------------------------*/
void Step::vInputDepth(void)
{
ENTRY("vInputDepth"_Stz);
  
  i8 iDepth = m_pTask->m_pCompiler->m_pTokeniser->iDepth();
  
  m_pDataStack->vPushI8(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.next -- get next input token from current input stream               */
/*----------------------------------------------------------------------------*/
void Step::vInputNext(void)
{
ENTRY("vInputNext"_Stz);
  
  if (m_pTask->m_iTracing) {
    ASSERT(m_pTask->m_iTracing);
  }
  
  char* aToken = m_pTask->m_pCompiler->m_pTokeniser->sToken().azChar();
  
  eTN eToken = m_pTask->m_pCompiler->m_pTokeniser->eNextToken(aToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.open -- open new input using filepath on data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vInputOpen(void)
{
ENTRY("vInputOpen"_Stz);
  
  s256 sFilePath;
  
  if ( ! m_pDataStack->bPop(sFilePath) )
    THROW_DSTACK();
  
  bool bOpened = m_pTask->m_pCompiler->m_pTokeniser->bUseFile(sFilePath);
  
  if ( ! bOpened )
    vThrow(FK_FAILED,__func__,__FILE__,__LINE__,
           "No such file: %s",sFilePath.azChar());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.pop -- pop to previous input stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputPop(void)
{
ENTRY("vInputPop"_Stz);
  
  bool bPopped = m_pTask->m_pCompiler->m_pTokeniser->bPop();
  ASSERT(bPopped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.push -- push input stream, allowing for new input stream             */
/*----------------------------------------------------------------------------*/
void Step::vInputPush(void)
{
ENTRY("vInputPush"_Stz);
  
  if ( ! m_pTask->m_pCompiler )
    m_pTask->m_pCompiler = new Compiler;
  
  m_pTask->m_pCompiler->m_pTokeniser->vPush();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.stdin -- use stdin as new input stream                               */
/*----------------------------------------------------------------------------*/
void Step::vInputStdin(void)
{
ENTRY("vInputStdin"_Stz);
  
  if ( ! m_pTask->m_pCompiler )
    m_pTask->m_pCompiler = new Compiler;
  
  m_pTask->m_pCompiler->m_pTokeniser->vUseStdin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.text -- use string as new input stream                               */
/*----------------------------------------------------------------------------*/
void Step::vInputText(void)
{
ENTRY("vInputText"_Stz);
  
  if (m_pTask->m_iTracing) {
    ASSERT(m_pTask->m_iTracing);
  }
  
  s48K sInput;                                             //! NEED STACK CHECKS
  
  if ( ! m_pDataStack->bPopS48K(sInput) )
    THROW_DSTACK();
    
  m_pTask->m_pCompiler->m_pTokeniser->vUseString(sInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.token -- push current token of input stream onto data stack          */
/*----------------------------------------------------------------------------*/
void Step::vInputToken(void)
{
ENTRY("vInputToken"_Stz);
  
  m_pDataStack->vPushS48K(m_pTask->m_pCompiler->m_pTokeniser->sToken());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.type -- push current token type onto data stack                      */
/*----------------------------------------------------------------------------*/
void Step::vInputType(void)
{
ENTRY("vInputType"_Stz);
  
  eTN eTokenType = m_pTask->m_pCompiler->m_pTokeniser->eToken();
  
  m_pDataStack->vPushI8(eTokenType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.unknown -- unknown current token                                     */
/*----------------------------------------------------------------------------*/
void Step::vInputUnknown(void)
{
  ENTRY("vInputUnknown"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("Unknown token: %s\n"_Stz,
         m_pTask->m_pCompiler->m_pTokeniser->sToken().azChar()
         );
  vTrace("\n"_Stz);

  AM::vBootError(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next-token -- token colon                                                      */
/*----------------------------------------------------------------------------*/
void Step::vTokenColon(void)
{
  ENTRY("vTokenColon"_Stz);
  
  m_pTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  m_pDataStack->vPushS48K(m_pTask->m_pCompiler->m_pTokeniser->sToken());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
