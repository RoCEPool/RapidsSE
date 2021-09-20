/*============================================================================*/
/* ExprSubquery.hpp - Identifies a subquery emanating from an expression      */
/*============================================================================*/

#include "BASE.h"
#include "ExprSubquery.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "ColumnRefs.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/planObject/planMarker/subqueryMarker/exprSubquery"_Stz;

static Stz* C_CORRELATEDCOLUMNS_PROP = "correlatedColumns"_Stz;
static Stz* C_CORRELATEDCOLUMNS_DEFN = "/__root__/definitions/object/list/columnRefs"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ExprSubquery nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* ExprSubquery::pCreate()
{
  // Create the object
  QueryNode* pSubqueryNode = SubqueryMarker::pCreate(C_DEFN);

  // Create and add any properties

  // Create an empty correlated columns list. This can be added to later
  // once column names have been resolved.
  QueryNode* pCorrelatedColumns = new QueryNode(C_CORRELATEDCOLUMNS_PROP, C_CORRELATEDCOLUMNS_DEFN);
  pSubqueryNode->vAppendProperty(pCorrelatedColumns);

  return pSubqueryNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ExprSubquery::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pStmt = _pQueryNode(pNode);

  buf.vConcat(GraphNav::szLeafType(pStmt->szType()));
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain to the buf             */
/*----------------------------------------------------------------------------*/
void ExprSubquery::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children
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
void ExprSubquery::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Location is explained in the common attributes above
    // Explain any local attributes of this object

    // Get the list of correlated columns
    QueryNode* pCorrColumnsNode = pCorrelatedColumns(_pQueryNode(pNode));
    ASSERT(pCorrColumnsNode);

    // Describe the list of columnRefs
    s2K sColBuf;
    if (ColumnRefs::nColumns(pCorrColumnsNode) > 0) {
      ColumnRefs::vDescribe(pCorrColumnsNode, sColBuf);

    } else {
      sColBuf = "none"_Stz;
    }

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("correlated columns: (%s)"_Stz, sColBuf.szStz());

    return;
  }

  // Manual polymorphism of subtypes here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 ExprSubquery::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 ExprSubquery::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExprSubquery::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExprSubquery::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the subquery is correlated with respect to its parent      */
/*----------------------------------------------------------------------------*/
b1 ExprSubquery::bIsCorrelated(QueryNode* pNode)
{
  UNUSED(pNode);
  ASSERT(bIsOfType(pNode));

  // All table subqueries are non-correlated
  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns the internal list of correlated columns. May be an empty list.     */
/*----------------------------------------------------------------------------*/
QueryNode* ExprSubquery::pCorrelatedColumns(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  PlanNode* pCorrColumnsNode = GraphNav::pFindProperty(const_cast<Stz*>(C_CORRELATEDCOLUMNS_PROP), pNode);
  ASSERT(pCorrColumnsNode);
  ASSERT(ColumnRefs::bIsOfType(pCorrColumnsNode));

  return _pQueryNode(pCorrColumnsNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
