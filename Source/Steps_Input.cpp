/*============================================================================*/
/* Steps_Input.cpp - input step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Input(void)
{
  vAddStep("input::close"_Stz,     &Step::vInputClose);
  vAddStep("input::cppTokens"_Stz, &Step::vInputCppTokens);
  vAddStep("input::dump"_Stz,      &Step::vInputDump);
  vAddStep("input::kind@"_Stz,     &Step::vInputKind);
  vAddStep("input::next"_Stz,      &Step::vInputNext);
  vAddStep("input::open"_Stz,      &Step::vInputOpen);
  vAddStep("input::pop"_Stz,       &Step::vInputPop);
  vAddStep("input::push"_Stz,      &Step::vInputPush);
  vAddStep("input::read"_Stz,      &Step::vInputRead);
  vAddStep("input::senTokens"_Stz, &Step::vInputSenTokens);
  vAddStep("input::stdin"_Stz,     &Step::vInputStdin);
  vAddStep("input::token"_Stz,     &Step::vInputTerm);
  vAddStep("input::text!"_Stz,     &Step::vInputText);
  vAddStep("input::unknown"_Stz,   &Step::vInputUnknown);
  vAddStep("input::useBegin"_Stz,  &Step::vInputUnknown);
  vAddStep("input::useEnd"_Stz,    &Step::vInputUnknown);
}

/*----------------------------------------------------------------------------*/
/* input::close -- close current input stream                                 */
/*----------------------------------------------------------------------------*/
void Step::vInputClose(void)
{
  ENTRY(); // ENTRY("vInputClose"_Stz);
  
  m_pCompiler->vUseEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::cppTokens -- expect cpp tokens                                      */
/*----------------------------------------------------------------------------*/
void Step::vInputCppTokens(void)
{
  ENTRY(); // ENTRY("vInputCppTokens"_Stz);
  
  if ( ! m_pCompiler )
    m_pCompiler = new Compiler;
  
  m_pCompiler->vCppTokens();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::dump -- dump input                                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputDump(void)
{
  ENTRY(); // ENTRY("vInputDump"_Stz);
    
  m_pCompiler->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::kind -- push current token kind onto data stack                     */
/*----------------------------------------------------------------------------*/
void Step::vInputKind(void)
{
  ENTRY(); // ENTRY("vInputKind"_Stz);
  
  eTN eTokenType = m_pCompiler->eToken();
  
  m_pDataStack->vPushInt(eTokenType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::next -- read next token and push its term                           */
/*----------------------------------------------------------------------------*/
void Step::vInputNext(void)
{
  ENTRY(); // ENTRY("vTokenColon"_Stz);
  
  m_pCompiler->vReadToken_cpl();
  
  s48K sTkn(m_pCompiler->szToken());
  
  m_pDataStack->vPushStr(sTkn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::open -- open new input using filepath on data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vInputOpen(void)
{
  ENTRY(); // ENTRY("vInputOpen"_Stz);
  
  s256 sFilePath;
  
  m_pDataStack->vPopStr(sFilePath,__func__);
  
  m_pCompiler->vUseFile(sFilePath);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::pop -- pop to previous input stream                                 */
/*----------------------------------------------------------------------------*/
void Step::vInputPop(void)
{
  ENTRY(); // ENTRY("vInputPop"_Stz);
  
  m_pCompiler->vPop();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::push -- push new input stream                                       */
/*----------------------------------------------------------------------------*/
void Step::vInputPush(void)
{
  ENTRY(); // ENTRY("vInputPush"_Stz);
  
  if ( ! m_pCompiler )
    m_pCompiler = new Compiler;
  
  m_pCompiler->vPush();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::read -- read next token from input stream                           */
/*----------------------------------------------------------------------------*/
void Step::vInputRead(void)
{
  ENTRY(); // ENTRY("vInputRead"_Stz);
    
  m_pCompiler->vReadToken_cpl();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::senTokens -- expect sen tokens                                      */
/*----------------------------------------------------------------------------*/
void Step::vInputSenTokens(void)
{
  ENTRY(); // ENTRY("vInputStdin"_Stz);
  
  if ( ! m_pCompiler )
    m_pCompiler = new Compiler;
  
  m_pCompiler->vSenTokens();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::stdin -- use stdin as input stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputStdin(void)
{
  ENTRY(); // ENTRY("vInputStdin"_Stz);
  
  if ( ! m_pCompiler )
    m_pCompiler = new Compiler;
  
  m_pCompiler->vUseStdin();
  m_pCompiler->vLineMode(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::term -- push term of current token onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vInputTerm(void)
{
  ENTRY(); // ENTRY("vInputTerm"_Stz);
  
  s48K sTkn(m_pCompiler->szToken());
  
  m_pDataStack->vPushStr(sTkn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::text! -- use string as input stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputText(void)
{
  ENTRY(); // ENTRY("vInputText"_Stz);
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  s48K sInput;                                             //! NEED STACK CHECKS
  
  m_pDataStack->vPopStr(sInput,__func__);
    
  m_pCompiler->vUseString(sInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::unknown -- unknown current token                                    */
/*----------------------------------------------------------------------------*/
void Step::vInputUnknown(void)
{
  ENTRY(); // ENTRY("vInputUnknown"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("Unknown token: %s\n"_Stz,
         m_pCompiler->szToken()
         );
  vTrace("\n"_Stz);

  AM::vBootError(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::useBegin -- begin using input                                       */
/*----------------------------------------------------------------------------*/
void Step::vInputUseBegin(void)
{
  ENTRY(); // ENTRY("vInputUseBegin"_Stz);
  
  m_pCompiler->vUseBegin();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::useBegin -- begin using input                                       */
/*----------------------------------------------------------------------------*/
void Step::vInputUseEnd(void)
{
  ENTRY(); // ENTRY("vInputUseBegin"_Stz);
  
  m_pCompiler->vUseEnd();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
