/*============================================================================*/
/* ShadowPlan.hpp - a shadow node of the main /__root__/plan node             */
/*============================================================================*/

#include "BASE.h"
#include "ShadowPlan.hpp"

#include "QueryNode.hpp"
#include "MainSubquery.hpp"
#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PLAN_NAME = "plan"_Stz;
static Stz* C_PLAN_DEFN = "/__root__/definitions/object/planNode"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Creates a node to represent the /__root__/plan node in the graph           */
/*----------------------------------------------------------------------------*/
QueryNode* ShadowPlan::pCreate()
{
  _static_

  QueryNode* pPlan = new QueryNode(C_PLAN_NAME, const_cast<Stz*>(C_PLAN_DEFN));
  return pPlan;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the plan node defn    */
/*----------------------------------------------------------------------------*/
b1 ShadowPlan::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, const_cast<Stz*>(C_PLAN_DEFN));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ShadowPlan::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, const_cast<Stz*>(C_PLAN_DEFN));
}

/*----------------------------------------------------------------------------*/
/* Returns the one and only main subquery child node                          */
/*----------------------------------------------------------------------------*/
QueryNode* ShadowPlan::pMainSubquery(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  auto fCondition = [](PlanNode* pChild) { return MainSubquery::bIsOfType(pChild); };

  QueryNode* pMainSubquery = _pQueryNode(GraphNav::pFindChild(fCondition, pNode));
  ASSERT(pMainSubquery);

  return pMainSubquery;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
