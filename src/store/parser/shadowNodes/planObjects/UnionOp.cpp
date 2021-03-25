/*============================================================================*/
/* UnionOp.hpp - a union of two SELECT queries                                */
/*============================================================================*/

#include "BASE.h"
#include "UnionOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "Expr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_UNIONOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/setOp/unionOp"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create UnionOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* UnionOp::pCreate(b1 bIsDistinct, QueryNode* pCorrelatedByList)
{
  // Create the base SetOp first
  QueryNode* pUnionOp = SetOp::pCreate(bIsDistinct, pCorrelatedByList);

  // Now change its name and type
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_UNIONOP_DEFN), sName);

  pUnionOp->vSetName(sName.szStz());
  pUnionOp->vSetType(C_UNIONOP_DEFN);

  return pUnionOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void UnionOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);


}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void UnionOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);


}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 UnionOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_UNIONOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely same as its definition         */
/*----------------------------------------------------------------------------*/
b1 UnionOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_UNIONOP_DEFN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
