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
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
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
  Parser::RootNode* pPlanRoot = Parser::_pRootNode(pPlanOperand->pPlanNode());

  // TODO: Walk the graph and validate that each name can be resolved uniquely.
  //       i.e. each table is known and unambiguous, each column refers to tables
  //       that are being used, etc.
  // TODO: Fill in the catalog/schema/table fields of the columnRef and tableRefs
  //       where applicable.


  // TODO: eventually we want the received plan graph to be immutable,
  //       so we would send a different version. But for now we will just
  //       manipulate and send the same plan graph
  m_pDataStack->vPushPtr(pPlanOperand);

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

  // TODO: Walk the graph and produce a pipeline statement.
  // Here's an example
  vTrace("\n- - - - - - - - - - - - - - - - - - \n"_Stz);

  // Lambda visitor function
  auto fVisitor = [](Parser::PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> Parser::GraphNav::eWalkCtrl {

    // We are only interested in the /__root__/plan subtree. Skip the others.
    if (iDepth > 0 && !sPath.bBeginsWith(const_cast<Stz*>(Parser::C_PLAN_PATH))) {
      return Parser::GraphNav::WALK_SKIP_SUBTREE;
    }

    // The root node can be useful for various graph navigation and manipulation.
    // e.g. finding other nodes.
    Parser::RootNode* pPlanRoot = Parser::_pRootNode(pvArg);

    vTrace("(%02lld) %s\t\t(%s)\n"_Stz, iDepth, sPath.szStz(), aCN[pNode->eGetClassNo()]);

    // Keep walking
    return Parser::GraphNav::WALK_CONTINUE;
  };

  // Walk the plan graph from the root, skipping the properties but descending
  // the children. We don't have to walk from the root, but if we don't then
  // the path string that is passed into our visitor function will be a relative
  // path and not an absolute one.
  // Let's also pass in the root node as an argument as it can be useful within
  // the visitor function.
  Parser::GraphNav::bWalk(pPlanRoot, false, true, fVisitor, pPlanRoot);

  delete pPlanRoot;
  delete pPlanOperand;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
