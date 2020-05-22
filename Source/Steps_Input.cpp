/*============================================================================*/
/* Steps_Input.cpp - input step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Input(void)
{
  vAddStep("input::chars"_Stz,     &Step::vInputChars);
  vAddStep("input::close"_Stz,     &Step::vInputClose);
  vAddStep("input::cppTokens"_Stz, &Step::vInputCppTokens);
  vAddStep("input::dump"_Stz,      &Step::vInputDump);
  vAddStep("input::inspect"_Stz,   &Step::vInputInspect);
  vAddStep("input::kind@"_Stz,     &Step::vInputKind);
  vAddStep("input::next"_Stz,      &Step::vInputNext);
  vAddStep("input::open"_Stz,      &Step::vInputOpen);
  vAddStep("input::pop"_Stz,       &Step::vInputPop);
  vAddStep("input::push"_Stz,      &Step::vInputPush);
  vAddStep("input::read"_Stz,      &Step::vInputRead);
  vAddStep("input::senTokens"_Stz, &Step::vInputSenTokens);
  vAddStep("input::stdin"_Stz,     &Step::vInputStdin);
  vAddStep("input::unknown"_Stz,   &Step::vInputUnknown);
  vAddStep("input::useBegin"_Stz,  &Step::vInputUseBegin);
  vAddStep("input::useEnd"_Stz,    &Step::vInputUseEnd);
  vAddStep("input::useInclude"_Stz,&Step::vInputUseInclude);
  vAddStep("input::useString"_Stz, &Step::vInputUseString);
}

/*----------------------------------------------------------------------------*/
/* input::term -- push term of current token onto data stack                  */
/*----------------------------------------------------------------------------*/
void Step::vInputChars(void)
{
  ENTRY();
  
  s48K sTkn(m_pInput->szChars());
  
  m_pDataStack->vPushStr(sTkn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::close -- close current input stream                                 */
/*----------------------------------------------------------------------------*/
void Step::vInputClose(void)
{
  ENTRY();
  
  m_pInput->vUseClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::cppTokens -- expect cpp tokens                                      */
/*----------------------------------------------------------------------------*/
void Step::vInputCppTokens(void)
{
  ENTRY();
    
  m_pInput->vFormCppTokens();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::dump -- dump input                                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputDump(void)
{
  ENTRY();
    
  m_pInput->vInspect();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::inpsect -- inspect input                                            */
/*----------------------------------------------------------------------------*/
void Step::vInputInspect(void)
{
  ENTRY();
    
  //m_pInput->vInspect();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::kind -- push current token kind onto data stack                     */
/*----------------------------------------------------------------------------*/
void Step::vInputKind(void)
{
  ENTRY();
  
  eTN eTokenType = m_pInput->eToken();
  
  m_pDataStack->vPushInt(eTokenType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::next -- read next token and push its chars                          */
/*----------------------------------------------------------------------------*/
void Step::vInputNext(void)
{
  ENTRY();
  
  // read next token
  m_pInput->vReadToken();
  
  // push the token
  s48K sTkn(m_pInput->szChars());
  m_pDataStack->vPushStr(sTkn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::open -- open new input using filepath on data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vInputOpen(void)
{
  ENTRY();
  
  s256 sFilePath;
  
  m_pDataStack->vPopStr(sFilePath,__func__);
  
  bool bOpened = m_pInput->bUseFile(sFilePath);
  
  if ( ! bOpened )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "No such file: %s"_Stz,sFilePath.szStz());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::pop -- pop to previous input stream                                 */
/*----------------------------------------------------------------------------*/
void Step::vInputPop(void)
{
  ENTRY();
  
  bool bPopped = m_pInput->bPopCharser();
  ASSERT(bPopped);
  
  if (-1 == m_pInput->iDepth())
    vDeleteInput();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::push -- push new input stream                                       */
/*----------------------------------------------------------------------------*/
void Step::vInputPush(void)
{
  ENTRY();
  
  if (! m_pInput) {
    vNewInput();
    ASSERT(0 == m_pInput->iDepth());
  }
  
  m_pInput->vPushCharser();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::read -- read next token from input stream                           */
/*----------------------------------------------------------------------------*/
void Step::vInputRead(void)
{
  ENTRY();
    
  m_pInput->vReadToken();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::senTokens -- expect sen tokens                                      */
/*----------------------------------------------------------------------------*/
void Step::vInputSenTokens(void)
{
  ENTRY();
  
  m_pInput->vFormSenTokens();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::stdin -- use stdin as input stream                                  */
/*----------------------------------------------------------------------------*/
void Step::vInputStdin(void)
{
  ENTRY();
    
  m_pInput->vUseStdin();
  m_pInput->vSingleLineCmd(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::unknown -- unknown current token                                    */
/*----------------------------------------------------------------------------*/
void Step::vInputUnknown(void)
{
  ENTRY();
  
  vTrace("\n"_Stz);
  vTrace("Unknown token: %s\n"_Stz,
         m_pInput->szChars()
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
  ENTRY();
  
  m_pInput->vUseBegin();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::useBegin -- begin using input                                       */
/*----------------------------------------------------------------------------*/
void Step::vInputUseEnd(void)
{
  ENTRY();
  
  m_pInput->vUseEnd();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::useInclude --                                                       */
/*----------------------------------------------------------------------------*/
void Step::vInputUseInclude(void)
{
  ENTRY();
  
  Word* pInclude = _pWord(m_pDataStack->pPopPtr(__func__));

  m_pInput->vUseInclude(pInclude);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input::useString -- use string as input stream                             */
/*----------------------------------------------------------------------------*/
void Step::vInputUseString(void)
{
  ENTRY();
  
  if (m_pTask->m_iTraceMask) {
    ASSERT(m_pTask->m_iTraceMask);
  }
  
  s48K sInput;                                             //! NEED STACK CHECKS
  
  m_pDataStack->vPopStr(sInput,__func__);
    
  m_pInput->vUseString(sInput);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
