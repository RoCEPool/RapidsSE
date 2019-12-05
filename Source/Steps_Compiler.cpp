/*============================================================================*/
/* Steps_Compiler.cpp - compiler step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Compiler(void)
{
  vAddStep("["_Stz,                        &Step::vCompilerInterpreting,  true);
  vAddStep("]"_Stz,                        &Step::vCompilerCompiling);
  
  vAddStep("compiler::current@"_Stz,       &Step::vCompilerCurrentFetch);
  vAddStep("compiler::current!"_Stz,       &Step::vCompilerCurrentStore);
  
  vAddStep("compiler::isCompiling"_Stz,    &Step::vCompilerIsCompiling);
  vAddStep("compiler::isInterpreting"_Stz, &Step::vCompilerIsInterpreting);
  
  vAddStep("compiler::nested@"_Stz,        &Step::vCompilerNestedFetch);
  vAddStep("compiler::nested!"_Stz,        &Step::vCompilerNestedStore);
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Step::vCompilerInterpreting(void)
{
  ENTRY(); // ENTRY("vCompilerInterpreting"_Stz);
  
  m_pCompiler->vMode(CM_INTERPRETING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Step::vCompilerCompiling(void)
{
  ENTRY(); // ENTRY("vCompilerCompiling"_Stz);
  
  m_pCompiler->vMode(CM_COMPILING);
  
  EXIT();
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
/* compiler::isCompiling -- currently in compiling mode?                      */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsCompiling(void)
{
  ENTRY(); // ENTRY("vCompilerIsCompiling"_Stz);
  
  i8 iIsCompiling = CM_COMPILING == m_pCompiler->eMode();
  
  m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::isInterpreting -- currently in interpreting mode?                */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsInterpreting(void)
{
  ENTRY(); // ENTRY("vDocolIsInterpreting"_Stz);
  
  i8 iIsInterpreting = CM_INTERPRETING == m_pCompiler->eMode();
  
  m_pDataStack->vPushInt(iIsInterpreting);
  
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
  
  Word* pNested = _pWord(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Nested = m_pDataStack->iPopInt(__func__);
  
  if (! m_pCompiler)
    m_pCompiler = new Compiler;
  
  m_pCompiler->vNestedWord(i_Nested,pNested);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
