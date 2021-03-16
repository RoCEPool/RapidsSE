/*============================================================================*/
/* FilterOp.hpp - operator that removes rows that don't match the condition   */
/*============================================================================*/

#include "BASE.h"
#include "FilterOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "store/parser/shadowNodes/expressions/Expr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_FILTEROP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/filterOp"_Stz;

static Stz* C_FILTEREXPR_PROP = "filterExpr"_Stz;
static Stz* C_FILTEREXPR_DEFN = "/__root__/definitions/object/list/filterExpr"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create FilterOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* FilterOp::pCreate(QueryNode* pFilterExpr)
{
  ASSERT(pFilterExpr);

  // Create a unique name for this FilterOp object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_FILTEROP_DEFN), sName);

  // Create the object and add its filterExpr property
  QueryNode* pFilterOp = new QueryNode(sName, C_FILTEROP_DEFN);

  pFilterExpr->vSetName(C_FILTEREXPR_PROP);
  pFilterOp->vAppendProperty(pFilterExpr);

  return pFilterOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void FilterOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pFilterOp = static_cast<QueryNode*>(pNode);

  sBuf.vConcat("FilterOp :\n  "_Stz);

  QueryNode* pFilterExpr = static_cast<QueryNode*>(GraphNav::pFindProperty(C_FILTEREXPR_PROP, pFilterOp));
  ASSERT(pFilterExpr);

  Expr::vDescribe(pFilterExpr, sBuf);
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void FilterOp::vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

  // Get the filter expression
  PlanNode* pFilterExpr = GraphNav::pFindProperty(const_cast<Stz*>(C_FILTEREXPR_PROP), pNode);
  ASSERT(pFilterExpr);

  s2K sExprBuf;
  Expr::vDescribe(pFilterExpr, sExprBuf);

  sBuf.vConcat("\n"_Stz);
  vAppendIndent(sBuf, iIndent, IF_PLAIN);
  sBuf.vCatSprint("expr: (%s)"_Stz, sExprBuf.szStz());

  // Recurse any children of pNode
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, sBuf, iIndent+1);
    }
    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 FilterOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_FILTEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the filterExpr node of the filterOp                                */
/*----------------------------------------------------------------------------*/
QueryNode* FilterOp::pFilterExpr(QueryNode* pNode)
{
  QueryNode *pExprNode =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_FILTEREXPR_PROP), pNode));

  ASSERT(pExprNode);
  return pExprNode;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedExprs that represent the column expressions      */
/* that this PlanObject will return. All nodes returned here must be freed.   */
/*----------------------------------------------------------------------------*/
PlanNodeList* FilterOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  // The FilterOp doesn't change the projected expressions, so ask its child.
  QueryNode* pChild = static_cast<QueryNode*>(pNode->pFirstChild());
  ASSERT(pChild);

  return PlanObject::pColumnExprs(pChild, pRootNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
