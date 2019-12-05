/*============================================================================*/
/* Steps_Compiler.cpp - compiler step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Compiler(void)
{
  vAddStep("compiler::current@"_Stz, &Step::vCompilerCurrentFetch);
  vAddStep("compiler::current!"_Stz, &Step::vCompilerCurrentStore);
  
  vAddStep("compiler::nested@"_Stz,  &Step::vCompilerNestedFetch);
  vAddStep("compiler::nested!"_Stz,  &Step::vCompilerNestedStore);
}

/*----------------------------------------------------------------------------*/
/* compiler::current@ -- get current word                                     */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCurrentFetch(void)
{
  ENTRY(); // ENTRY("vCompilerCurrentFetch"_Stz);
  
  if (! m_pCompiler)
    m_pCompiler = new Compiler;
  
  Word* pCurrWord = m_pCompiler->pCurrentWord();
  
  m_pDataStack->vPushPtr(pCurrWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::current! -- put current word                                     */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCurrentStore(void)
{
  ENTRY(); // ENTRY("vCompilerCurrentStore"_Stz);
  
  Word* pCurrWord = _pWord(m_pDataStack->pPopPtr(__func__));
  
  if (! m_pCompiler)
    m_pCompiler = new Compiler;
  
  m_pCompiler->vCurrentWord(pCurrWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::nested@ -- get nested word                                       */
/*----------------------------------------------------------------------------*/
void Step::vCompilerNestedFetch(void)
{
  ENTRY(); // ENTRY("vCompilerNestedFetch"_Stz);
  
  if (! m_pCompiler)
    m_pCompiler = new Compiler;
  
  i8 i_Nested = m_pDataStack->iPopInt(__func__);
  
  Word* pNested = m_pCompiler->pNestedWord(i_Nested);
  
  m_pDataStack->vPushPtr(pNested);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::nested! -- set nested word                                       */
/*----------------------------------------------------------------------------*/
void Step::vCompilerNestedStore(void)
{
  ENTRY(); // ENTRY("vCompilerNestedStore"_Stz);
  
  i8 i_Nested = m_pDataStack->iPopInt(__func__);
  
  Word* pNested = _pWord(m_pDataStack->pPopPtr(__func__));
  
  if (! m_pCompiler)
    m_pCompiler = new Compiler;
  
  m_pCompiler->vNestedWord(i_Nested,pNested);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
