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
  vAddStep("input::dump"_Stz,      &Step::vInputDump);
  vAddStep("input::next"_Stz,      &Step::vInputNext);
  vAddStep("input::open"_Stz,      &Step::vInputOpen);
  vAddStep("input::pop"_Stz,       &Step::vInputPop);
  vAddStep("input::push"_Stz,      &Step::vInputPush);
  vAddStep("input::stdin"_Stz,     &Step::vInputStdin);
  vAddStep("input::text"_Stz,      &Step::vInputText);
  vAddStep("input::token"_Stz,     &Step::vInputToken);
  vAddStep("input::type"_Stz,      &Step::vInputType);
  vAddStep("input::unknown"_Stz,   &Step::vInputUnknown);

  vAddStep("next-token"_Stz,       &Step::vTokenColon);
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
/* input::dump -- dump input                                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputDump(void)
{
  ENTRY(); // ENTRY("vInputDump"_Stz);
    
  m_pCompiler->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::next -- get next input token from current input stream              */
/*----------------------------------------------------------------------------*/
void Step::vInputNext(void)
{
  ENTRY(); // ENTRY("vInputNext"_Stz);
    
  m_pCompiler->vReadToken_cpl();

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
/* input::push -- push input stream, allowing for new input stream            */
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
/* input::stdin -- use stdin as new input stream                              */
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
/* input::text -- use string as new input stream                              */
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
/* input::token -- push current token of input stream onto data stack         */
/*----------------------------------------------------------------------------*/
void Step::vInputToken(void)
{
  ENTRY(); // ENTRY("vInputToken"_Stz);
  
  s48K sTkn(m_pCompiler->szToken());
  
  m_pDataStack->vPushStr(sTkn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::type -- push current token type onto data stack                     */
/*----------------------------------------------------------------------------*/
void Step::vInputType(void)
{
  ENTRY(); // ENTRY("vInputType"_Stz);
  
  eTN eTokenType = m_pCompiler->eToken();
  
  m_pDataStack->vPushInt(eTokenType);
  
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
/* next-token -- token colon                                                  */
/*----------------------------------------------------------------------------*/
void Step::vTokenColon(void)
{
  ENTRY(); // ENTRY("vTokenColon"_Stz);
  
  m_pCompiler->vReadToken_cpl();
  
  s48K sTkn(m_pCompiler->szToken());
  
  m_pDataStack->vPushStr(sTkn);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
