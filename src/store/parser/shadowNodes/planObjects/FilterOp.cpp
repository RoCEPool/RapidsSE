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
#include "NodeNamer.hpp"
#include "MetadataNode.hpp"
#include "ShadowRoot.hpp"

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
  NodeNamer::vGenerateName(C_FILTEROP_DEFN, sName);

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

  QueryNode* pFilterOp = _pQueryNode(pNode);

  sBuf.vConcat("FilterOp :\n  "_Stz);

  QueryNode* pFilterExpr = _pQueryNode(GraphNav::pFindProperty(C_FILTEREXPR_PROP, pFilterOp));
  ASSERT(pFilterExpr);

  Expr::vDescribe(pFilterExpr, sBuf);
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void FilterOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children of pNode
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, pExplainNode, sBuf, iIndent + 1);
    }
    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void FilterOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object

    // Get the filter expression
    PlanNode* pFilterExpr = GraphNav::pFindProperty(const_cast<Stz*>(C_FILTEREXPR_PROP), pNode);
    ASSERT(pFilterExpr);

    s2K sExprBuf;
    Expr::vDescribe(pFilterExpr, sExprBuf);

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("expr: (%s)"_Stz, sExprBuf.szStz());


    // Explaining very early plans are difficult because we cannot resolve the column expressions.
    // We don't want to retrieve any ordinal expressions below before we have been able
    // to resolve all table/column names in the system.
    // When we resolve names we also add metadata into plan. So if the metadata node has
    // no children then we probably haven't resolved any names yet (except for the case of
    // the plan only containing literals).

    if (MetadataNode::bHasMetadata(ShadowRoot::pMetadataNode(pRootNode))) {
      QueryNode* pOrdinalExpr = pOrdinalFilterExpr(_pQueryNode(pNode), pRootNode);
      sExprBuf.vReset();
      Expr::vDescribe(pOrdinalExpr, sExprBuf);
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("ordinal expr: (%s)"_Stz, sExprBuf.szStz());
      delete pOrdinalExpr;
    }

    return;
  }

  // Manual polymorphism here

}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 FilterOp::bIsOfType(PlanNode *pNode)
{
  _static_
  return ShadowNode::bIsOfType(pNode, C_FILTEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 FilterOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_FILTEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 FilterOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_FILTEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 FilterOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_FILTEROP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the filterExpr node of the filterOp                                */
/*----------------------------------------------------------------------------*/
QueryNode* FilterOp::pFilterExpr(QueryNode* pNode)
{
  _static_
  QueryNode *pExprNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_FILTEREXPR_PROP), pNode));

  ASSERT(pExprNode);
  return pExprNode;
}

/*----------------------------------------------------------------------------*/
/* Returns a new copy of the filter expression relative to column ordinals.   */
/* Must be freed after use.                                                   */
/*----------------------------------------------------------------------------*/
QueryNode* FilterOp::pOrdinalFilterExpr(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Get the symbolic filter expression
  QueryNode *pSymbolicExpr = pFilterExpr(pNode);
  ASSERT(pSymbolicExpr);

  // Get the list of column expressions being given to this plan from its child.
  // This is the list of expression symbols that represent each column of
  // each row that this sql operator will receive.
  ASSERT(pNode->nChildren() == 1);
  QueryNodeList* pColExprList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(pNode, pRootNode);

  // Use the column expression list and the symbolic filter expression to
  // generate an ordinal filter expression (one that is relative to column
  // ordinals only).
  QueryNode* pOrdinalExpr = Expr::pSymbolicToOrdinal(pSymbolicExpr, pColExprList, pCorrelatedExprs, pRootNode);
  ASSERT(pOrdinalExpr);

  pColExprList->vDeleteAll();
  delete pColExprList;

  return pOrdinalExpr;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions    */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* FilterOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  // The FilterOp doesn't change the projected expressions, so ask its child.
  QueryNode* pChild = _pQueryNode(pNode->pFirstChild());
  ASSERT(pChild);

  return PlanObject::pColumnExprs(pChild, pRootNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
