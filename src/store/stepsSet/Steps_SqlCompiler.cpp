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
#include "OtherOperand.hpp"

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
  
  OtherOperand* pOtherOperand = _pOtherOperand(m_pDataStack->pPopPtr(__func__));
  Parser::RootNode* pPlanNode_root = (Parser::RootNode*)pOtherOperand->pObject();

  
  Parser::RootNode* pPlanNode_root2 = new Parser::RootNode(nullptr);
  OtherOperand* pOtherOperand2 = new OtherOperand(pPlanNode_root2);

  
  //vTrace("  NameValidate(RootNode %p)-> RootNode %p\n"_Stz,
  //       pPlanNode_root,pPlanNode_root2);

  delete pOtherOperand;
  delete pPlanNode_root;

  
  m_pDataStack->vPushPtr(pOtherOperand2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sqlCompiler::parser -- sql parser                                          */
/*----------------------------------------------------------------------------*/
void Step::vSqlCompilerParser(void)
{
  ENTRY();
  
  //vTrace("\nStep::vSqlCompilerParser\n"_Stz);
  
  s4K sSqlStatement;
  m_pDataStack->vPopStr(sSqlStatement,__func__);

  Parser::RootNode* pPlanNode_root = new Parser::RootNode(nullptr);
  
  OtherOperand* pOtherOperand = new OtherOperand(pPlanNode_root);
  
  m_pDataStack->vPushPtr(pOtherOperand);

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

  OtherOperand* pOtherOperand = _pOtherOperand(m_pDataStack->pPopPtr(__func__));

  Parser::RootNode* pPlanNode_root = (Parser::RootNode*)pOtherOperand->pObject();

  //vTrace("  Transliterate(RootNode %p) -> nil\n"_Stz,pPlanNode_root);

  delete pOtherOperand;
  delete pPlanNode_root;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
