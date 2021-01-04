/*============================================================================*/
/* ProjectOp.hpp - operator that changes the defn of rows that it processes   */
/*============================================================================*/

#include "BASE.h"
#include "ProjectOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "ProjectedExpr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PROJECTEDEXPRS_PROP = "projectedExprs"_Stz;
static Stz* C_PROJECTEDEXPRS_DEFN = "/__root__/definitions/object/list/projectedExprs"_Stz;
static Stz* C_PROJECTION_DEFN = "/__root__/definitions/object/planObject/sqlOperator/projection"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pCreate(QueryNodeList* pSelectItems)
{
  ASSERT(pSelectItems);
  ASSERT(pSelectItems->nItems() > 0);

  // Create the projectedExprs property of the ProjectOp
  Value xValue;
  QueryNode* pProjectedExprs = new QueryNode(C_PROJECTEDEXPRS_PROP, const_cast<Stz*>(C_PROJECTEDEXPRS_DEFN), xValue);

  QueryNode* pItem;
  while ((pItem = pSelectItems->pPop()) != nullptr) {
    pProjectedExprs->vAppendChild(pItem);
  }

  // Create a unique name for this ProjectOp object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_PROJECTION_DEFN), sName);

  // Create the object and add  its projectedExprs property
  QueryNode* pProjectOp = new QueryNode(sName, C_PROJECTION_DEFN);
  pProjectOp->vAppendProperty(pProjectedExprs);

  return pProjectOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void ProjectOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pProjectOp = static_cast<QueryNode*>(pNode);

  sBuf.vConcat("ProjectOp :"_Stz);

  QueryNode* pProjectedExprs = static_cast<QueryNode*>(GraphNav::pFindProperty(C_PROJECTEDEXPRS_PROP, pProjectOp));
  ASSERT(pProjectedExprs);

  i8 nCols = pProjectedExprs->nChildren();
  for (i8 i=0; i<nCols; i++) {
    QueryNode* pProjectedExpr = static_cast<QueryNode*>(pProjectedExprs->pNthChild(i));
    ASSERT(ProjectedExpr::bIsOfType(pProjectedExpr));

    sBuf.vConcat("\n  "_Stz);
    ProjectedExpr::vDescribe(pProjectedExpr, sBuf);
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTION_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
