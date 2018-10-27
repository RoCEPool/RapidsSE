/*============================================================================*/
/* Steps_Input.cpp - input step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// input: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Input(void)
{
  vAddStep("input.close"_Stz,     &Task::vInputClose);
  vAddStep("input.depth"_Stz,     &Task::vInputDepth);
  vAddStep("input.next"_Stz,      &Task::vInputNext);
  vAddStep("input.open"_Stz,      &Task::vInputOpen);
  vAddStep("input.pop"_Stz,       &Task::vInputPop);
  vAddStep("input.push"_Stz,      &Task::vInputPush);
  vAddStep("input.stdin"_Stz,     &Task::vInputStdin);
  vAddStep("input.text"_Stz,      &Task::vInputText);
  vAddStep("input.token"_Stz,     &Task::vInputToken);
  vAddStep("input.type"_Stz,      &Task::vInputType);
  vAddStep("input.unknown"_Stz,   &Task::vInputUnknown);

  vAddStep("token:"_Stz,          &Task::vTokenColon);
}

/*----------------------------------------------------------------------------*/
/* input.close -- close current input stream                                  */
/*----------------------------------------------------------------------------*/
void Task::vInputClose(MP_Task mpTask)
{
ENTRY("vInputClose"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vUseEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.depth -- push current depth of input stream onto data stack         */
/*----------------------------------------------------------------------------*/
void Task::vInputDepth(MP_Task mpTask)
{
ENTRY("vInputDepth"_Stz);
  
  i8 iDepth = mpTask->m_pCompiler->m_pTokeniser->iDepth();
  
  mpTask->m_pDataStack->vPushInt(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.next -- get next input token from current input stream               */
/*----------------------------------------------------------------------------*/
void Task::vInputNext(MP_Task mpTask)
{
ENTRY("vInputNext"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }
  
  char* aToken = mpTask->m_pCompiler->m_pTokeniser->sToken().aChar();
  
  eTN eToken = mpTask->m_pCompiler->m_pTokeniser->eNextToken(aToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.open -- open new input using filepath on data stack                  */
/*----------------------------------------------------------------------------*/
void Task::vInputOpen(MP_Task mpTask)
{
ENTRY("vInputOpen"_Stz);
  
  s256 sFilePath;
  
  if ( ! mpTask->m_pDataStack->bPop(sFilePath) )
    THROW_INVALID_DSTACK("input.open"_Stz);
  
  bool bOpened = mpTask->m_pCompiler->m_pTokeniser->bUseFile(sFilePath);
  
  if ( ! bOpened )
    THROW("No such file: %s"_Stz,sFilePath.aChar());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.pop -- pop to previous input stream                                  */
/*----------------------------------------------------------------------------*/
void Task::vInputPop(MP_Task mpTask)
{
ENTRY("vInputPop"_Stz);
  
  bool bPopped = mpTask->m_pCompiler->m_pTokeniser->bPop();
  ASSERT(bPopped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.push -- push input stream, allowing for new input stream             */
/*----------------------------------------------------------------------------*/
void Task::vInputPush(MP_Task mpTask)
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
void Task::vInputStdin(MP_Task mpTask)
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
void Task::vInputText(MP_Task mpTask)
{
ENTRY("vInputText"_Stz);
  
  if (mpTask->m_iTracing) {
    ASSERT(mpTask->m_iTracing);
  }
  
  s48K sInput;                                             //! NEED STACK CHECKS
  
  if ( ! mpTask->m_pDataStack->bPop(sInput) )
    THROW_INVALID_DSTACK("input.text"_Stz);
    
  mpTask->m_pCompiler->m_pTokeniser->vUseString(sInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.token -- push current token of input stream onto data stack          */
/*----------------------------------------------------------------------------*/
void Task::vInputToken(MP_Task mpTask)
{
ENTRY("vInputToken"_Stz);
  
  mpTask->m_pDataStack->vPushS48K(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.type -- push current token type onto data stack                      */
/*----------------------------------------------------------------------------*/
void Task::vInputType(MP_Task mpTask)
{
ENTRY("vInputType"_Stz);
  
  eTN eTokenType = mpTask->m_pCompiler->m_pTokeniser->eToken();
  
  mpTask->m_pDataStack->vPushInt(eTokenType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.unknown -- unknown current token                                     */
/*----------------------------------------------------------------------------*/
void Task::vInputUnknown(MP_Task mpTask)
{
  ENTRY("vInputUnknown"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("Unknown token: %s\n"_Stz,
         mpTask->m_pCompiler->m_pTokeniser->sToken().aChar()
         );
  vTrace("\n"_Stz);

  gpAttachMap->m_bBootError = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* token: -- token colon                                                      */
/*----------------------------------------------------------------------------*/
void Task::vTokenColon(MP_Task mpTask)
{
  ENTRY("vTokenColon"_Stz);
  
  mpTask->m_pCompiler->m_pTokeniser->vNextToken();
  
  mpTask->m_pDataStack->vPushS48K(mpTask->m_pCompiler->m_pTokeniser->sToken());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
