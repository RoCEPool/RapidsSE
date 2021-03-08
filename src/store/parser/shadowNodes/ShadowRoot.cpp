/*============================================================================*/
/* ShadowRoot.hpp - a shadow node of the root of the plan graph               */
/*============================================================================*/

#include "BASE.h"
#include "ShadowRoot.hpp"
#include "Statement.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a description of the ProjectedExpr into a buffer                   */
/*----------------------------------------------------------------------------*/
void ShadowRoot::vDescribe(PlanNode* pRoot, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pRoot));

  sBuf.vConcat(GraphNav::szLeafType(pRoot->szType()));
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void ShadowRoot::vExplain(PlanNode* pRoot, s48K& sBuf, i8 iIndent)
{
  _static_

  ASSERT(bIsOfType(pRoot));

  // Get the first child of /__root__/plan
  PlanNode* pPlan = GraphNav::pFindChild(const_cast<Stz*>(C_PLAN_NAME), pRoot);
  ASSERT(pPlan);

  PlanNode* pStatement = pPlan->pFirstChild();
  ASSERT(pStatement);
  ASSERT(Statement::bIsOfType(pStatement));

  Statement::vExplain(pStatement, sBuf, iIndent);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the root node definition    */
/*----------------------------------------------------------------------------*/
b1 ShadowRoot::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, const_cast<Stz*>(C_ROOT_DEFN_PATH));
}

/*----------------------------------------------------------------------------*/
/* Returns the node representing the metadata node in the plan graph          */
/*----------------------------------------------------------------------------*/
QueryNode* ShadowRoot::pMetadataNode(PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pMetadata = static_cast<QueryNode*>(GraphNav::pFindChild(const_cast<Stz*>(C_METADATA_NAME), pNode));
  ASSERT(pMetadata);

  return pMetadata;
}

/*----------------------------------------------------------------------------*/
/* Returns the node representing the /__root__/plan node in the plan graph    */
/*----------------------------------------------------------------------------*/
QueryNode* ShadowRoot::pPlanNode(PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pPlan = static_cast<QueryNode*>(GraphNav::pFindChild(const_cast<Stz*>(C_PLAN_NAME), pNode));
  ASSERT(pPlan);

  return pPlan;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
