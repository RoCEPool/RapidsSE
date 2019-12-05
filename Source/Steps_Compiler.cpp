/*============================================================================*/
/* Steps_Compiler.cpp - compiler step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Compiler(void)
{
  vAddStep("["_Stz,                        &Step::vDocolInterpreting,   true);
  vAddStep("]"_Stz,                        &Step::vDocolCompiling);
  
  vAddStep("compiler::current@"_Stz,       &Step::vCompilerCurrentFetch);
  vAddStep("compiler::current!"_Stz,       &Step::vCompilerCurrentStore);
  
  vAddStep("compiler::isCompiling"_Stz,    &Step::vCompilerIsCompiling);
  vAddStep("compiler::isInterpreting"_Stz, &Step::vCompilerIsInterpreting);
  
  vAddStep("compiler::nested@"_Stz,        &Step::vCompilerNestedFetch);
  vAddStep("compiler::nested!"_Stz,        &Step::vCompilerNestedStore);
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



/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Step::vDocolCompiling(void)
{
  ENTRY(); // ENTRY("vDocolCompiling"_Stz);
  
  m_pCompiler->vCompiling(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Step::vDocolInterpreting(void)
{
  ENTRY(); // ENTRY("vDocolInterpreting"_Stz);
  
  m_pCompiler->vCompiling(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::isCompiling -- currently in compiling mode?                         */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsCompiling(void)
{
  ENTRY(); // ENTRY("vCompilerIsCompiling"_Stz);
  
  i8 iIsCompiling = m_pCompiler->bCompiling() ? 1 : 0;
  
  m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::isInterpreting -- currently in interpreting mode?                   */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsInterpreting(void)
{
  ENTRY(); // ENTRY("vDocolIsInterpreting"_Stz);
  
  i8 iIsInterpreting = m_pCompiler->bCompiling() ? 0 : 1;
  
  m_pDataStack->vPushInt(iIsInterpreting);
  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
