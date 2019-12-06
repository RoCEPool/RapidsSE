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
  
  vAddStep("[["_Stz,                       &Step::vCompilerPostponing,    true);
  vAddStep("]]"_Stz,                       &Step::vCompilerCompiling,     true);
  
  vAddStep("compiler::compiling"_Stz,      &Step::vCompilerCompiling);
  
  vAddStep("compiler::current@"_Stz,       &Step::vCompilerCurrentFetch);
  vAddStep("compiler::current!"_Stz,       &Step::vCompilerCurrentStore);
  
  vAddStep("compiler::interpreting"_Stz,   &Step::vCompilerInterpreting);
  
  vAddStep("compiler::isCompiling"_Stz,    &Step::vCompilerIsCompiling);
  vAddStep("compiler::isInterpreting"_Stz, &Step::vCompilerIsInterpreting);
  vAddStep("compiler::isPostponing"_Stz,   &Step::vCompilerIsPostponing);

  vAddStep("compiler::nested@"_Stz,        &Step::vCompilerNestedFetch);
  vAddStep("compiler::nested!"_Stz,        &Step::vCompilerNestedStore);
  
  vAddStep("compiler::postponing"_Stz,     &Step::vCompilerPostponing);
}

/*----------------------------------------------------------------------------*/
/* [ -- change to interpreting mode                                           */
/*----------------------------------------------------------------------------*/
void Step::vCompilerInterpreting(void)
{
  ENTRY(); // ENTRY("vCompilerInterpreting"_Stz);
  
  m_pCompiler->vMode(CM_INTERPRETING);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ] -- change to compiling mode                                              */
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
  
  i8 iIsCompiling = m_pCompiler->bCompiling();
  
  m_pDataStack->vPushInt(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::isInterpreting -- currently in interpreting mode?                */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsInterpreting(void)
{
  ENTRY(); // ENTRY("vCompilerIsInterpreting"_Stz);
  
  i8 iIsInterpreting = m_pCompiler->bInterpreting();
  
  m_pDataStack->vPushInt(iIsInterpreting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiler::isPostponing -- currently in postponing mode?                    */
/*----------------------------------------------------------------------------*/
void Step::vCompilerIsPostponing(void)
{
  ENTRY(); // ENTRY("vCompilerIsPostponing"_Stz);
  
  i8 bIsPostponing = m_pCompiler->bPostponing();
  
  m_pDataStack->vPushInt(bIsPostponing);
  
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

/*----------------------------------------------------------------------------*/
/*  compiler::postponing -- change to postponing mode                         */
/*----------------------------------------------------------------------------*/
void Step::vCompilerPostponing(void)
{
  ENTRY(); // ENTRY("vCompilerPostponing"_Stz);
  
  m_pCompiler->vMode(CM_POSTPONING);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
