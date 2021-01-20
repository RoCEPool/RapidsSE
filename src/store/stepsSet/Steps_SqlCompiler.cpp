/*============================================================================*/
/* Steps_SqlCompiler.cpp - sql compiler step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "PlanNode.hpp"
#include "RootNode.hpp"
#include "PlanDefinitionParser.hpp"
#include "SqlPlanner.hpp"
#include "ShadowNode.hpp"
#include "ShadowRoot.hpp"
#include "PlanOperand.hpp"

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
  
  //vTrace("\nStep::vSqlCompilerNameValidate\n"_Stz);
  
  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanNode_root = (Parser::RootNode*)pPlanOperand->pPlanNode();

  
  Parser::RootNode* pPlanNode_root2 = new Parser::RootNode(nullptr);
  PlanOperand* pPlanOperand2 = new PlanOperand(pPlanNode_root2);

  
  //vTrace("  NameValidate(RootNode %p)-> RootNode %p\n"_Stz,
  //       pPlanNode_root,pPlanNode_root2);

  delete pPlanOperand;
  delete pPlanNode_root;

  
  m_pDataStack->vPushPtr(pPlanOperand2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::parser -- sql parser                                          */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerParser(void)
{
  ENTRY();
  
  //vTrace("\nStep::vSqlCompilerParser\n"_Stz);

  // Get the original SQL statement entered
  s4K sSqlStatement;
  m_pDataStack->vPopStr(sSqlStatement,__func__);

  // Read in and parse the plan graph definitions
  const Parser::PlanNode* pGraphTemplate = Parser::PlanDefinitionParser::pGraphTemplate();

  if (pGraphTemplate == nullptr) {
    // TODO: handle error
//    return false;
  }

  // Parse the SQL command.
  Parser::SqlPlanner* pPlanner = new Parser::SqlPlanner();
  Parser::PlanNode* pPlanRoot = pPlanner->plan(sSqlStatement.szStz(), pGraphTemplate);
  delete pPlanner;

  if (!pPlanRoot) {
    // TODO: handle error
  }

  s48K sExplainBuf;
  Parser::ShadowRoot::vExplain(pPlanRoot, sExplainBuf, 0);
  vTrace("%s\n"_Stz, sExplainBuf.szStz());

  PlanOperand* pPlanOperand = new PlanOperand(pPlanRoot);
  m_pDataStack->vPushPtr(pPlanOperand);

  //vTrace("  antlr(%s) -> RootNode %p\n"_Stz,
  //       sSqlStatement.szStz(),pPlanNode_root);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::transliterate -- plan to pipeline transliterate               */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerTransliterate(void)
{
  ENTRY();
  
  //vTrace("\nStep::vSqlCompilerTransliterate\n"_Stz);

  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());

  //vTrace("  Transliterate(RootNode %p) -> nil\n"_Stz,pPlanNode_root);

  delete pPlanRoot;
  delete pPlanOperand;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
