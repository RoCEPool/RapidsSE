/*============================================================================*/
/* Steps_SqlCompiler.cpp - sql compiler step group                            */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "PlanNode.hpp"
#include "RootNode.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SqlCompiler(void)
{
  vAddStep("sqlCompiler::nameValidate"_Stz,   &Step::vSqlCompilerNameValidate);
  vAddStep("sqlCompiler::parser"_Stz,         &Step::vSqlCompilerParser);
  vAddStep("sqlCompiler::transliterate"_Stz,  &Step::vSqlCompilerTransliterate);
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::nameValidate -- name validation                               */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerNameValidate(void)
{
  ENTRY();
  
  vTrace("Step::vSqlCompilerNameValidate\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::parser -- sql parser                                          */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerParser(void)
{
  ENTRY();
  
  vTrace("Step::vSqlCompilerParser\n"_Stz);
  
  s4K sSqlStatement;
  m_pDataStack->vPopStr(sSqlStatement,__func__);

  vTrace("%s\n"_Stz,sSqlStatement.szStz());

  Parser::RootNode* pPlanNode_root = new Parser::RootNode(nullptr);
  
  m_pDataStack->vPushPtr(pPlanNode_root);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::transliterate -- plan to pipeline transliterate               */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerTransliterate(void)
{
  ENTRY();
  
  vTrace("Step::vSqlCompilerTransliterate\n"_Stz);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
