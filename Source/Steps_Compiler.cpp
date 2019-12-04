/*============================================================================*/
/* Steps_Compiler.cpp - compiler step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Compiler(void)
{
  vAddStep("compiler::current"_Stz,      &Step::vCompilerCurrent);
  
  vAddStep("compiler::currentWord@"_Stz, &Step::vCompilerCurrentWordFetch);
  vAddStep("compiler::currentWord!"_Stz, &Step::vCompilerCurrentWordStore);
}

/*----------------------------------------------------------------------------*/
/* compiler::current -- push word being compiled                              */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCurrent(void)
{
  ENTRY(); // ENTRY("vCompilerCurrent"_Stz);
  
  Word* pWord = m_pCompiler->pCurrentWord();
  
  m_pDataStack->vPushPtr(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::currentWord@ -- get current word                                 */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCurrentWordFetch(void)
{
  ENTRY(); // ENTRY("vCompilerCurrentWordFetch"_Stz);
  
  if (! m_pCompiler)
    m_pCompiler = new Compiler;
  
  Word* pCurrWord = m_pCompiler->pCurrentWord();
  
  m_pDataStack->vPushPtr(pCurrWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::currentWord! -- put current word                                 */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCurrentWordStore(void)
{
  ENTRY(); // ENTRY("vCompilerCurrentWordStore"_Stz);
  
  Word* pCurrWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  if (! m_pCompiler)
    m_pCompiler = new Compiler;
  
  m_pCompiler->vCurrentWord(pCurrWord);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
