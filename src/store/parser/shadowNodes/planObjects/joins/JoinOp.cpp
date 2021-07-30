/*============================================================================*/
/* JoinOp.hpp - a base definition of a join operation                         */
/*============================================================================*/

#include "BASE.h"
#include "JoinOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

#include "TableScanOp.hpp"
#include "StringObject.hpp"
#include "Expr.hpp"
#include "ProjectedColumn.hpp"
#include "ColumnRef.hpp"
#include "NodeNamer.hpp"

#include "NestedLoopJoinOp.hpp"
#include "HashJoinOp.hpp"
#include "SortMergeJoinOp.hpp"
#include "IndexLookupJoinOp.hpp"
#include "JoinType.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_JOINOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/joinOp"_Stz;

static Stz* C_JOINTYPE_PROP = "joinType"_Stz;
static Stz* C_JOINPREDICATE_PROP = "joinPredicate"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* JoinOp::pCreate(Stz* szDefn, eJT eJoinType, QueryNode* pPredicate)
{
  ASSERT(szDefn);
  ASSERT(JoinOp::bIsOfType(szDefn));
  ASSERT(eJoinType > JT_NONE);
  ASSERT(eJoinType < JT_NENUMS);
  ASSERT(pPredicate);
  ASSERT(Expr::bIsExactType(pPredicate));

  // Create a unique name for this JoinOp object
  s256 sName;
  NodeNamer::vGenerateName(szDefn, sName);

  // Create the object and add the join type and predicate properties.
  // We will need to rename them first.
  QueryNode* pJoinOp = new QueryNode(sName, szDefn);

  QueryNode* pJoinType = JoinType::pCreate(eJoinType);
  pJoinType->vSetName(C_JOINTYPE_PROP);
  pJoinOp->vAppendProperty(pJoinType);

  pPredicate->vSetName(C_JOINPREDICATE_PROP);
  pJoinOp->vAppendProperty(pPredicate);

  return pJoinOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void JoinOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here
  if      ( NestedLoopJoinOp::bIsOfType(pNode))   {  NestedLoopJoinOp::vDescribe(pNode, sBuf);  }
  else if (       HashJoinOp::bIsOfType(pNode))   {        HashJoinOp::vDescribe(pNode, sBuf);  }
  else if (  SortMergeJoinOp::bIsOfType(pNode))   {   SortMergeJoinOp::vDescribe(pNode, sBuf);  }
  else if (IndexLookupJoinOp::bIsOfType(pNode))   { IndexLookupJoinOp::vDescribe(pNode, sBuf);  }
  else {
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void JoinOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here
  if      ( NestedLoopJoinOp::bIsOfType(pNode))   {  NestedLoopJoinOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent);  }
  else if (       HashJoinOp::bIsOfType(pNode))   {        HashJoinOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent);  }
  else if (  SortMergeJoinOp::bIsOfType(pNode))   {   SortMergeJoinOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent);  }
  else if (IndexLookupJoinOp::bIsOfType(pNode))   { IndexLookupJoinOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent);  }
  else {
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void JoinOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s256 sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object


    // Get the joinType property
    eJT eJoinType = JoinOp::eJoinType(pNode);

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("join type: (%s)"_Stz, aJT_pretty[eJoinType]);


    // Get the joinPredicate property
    QueryNode* pJoinPredicate = JoinOp::pJoinPredicate(pNode);
    ASSERT(pJoinPredicate);
    ASSERT(Expr::bIsOfType(pJoinPredicate));

    s2K sPredicateBuf;
    Expr::vDescribe(pJoinPredicate, sPredicateBuf);

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("predicate: (%s)"_Stz, sPredicateBuf.szStz());

    return;
  }

  // Manual polymorphism here
  if      ( NestedLoopJoinOp::bIsOfType(pNode))   {  NestedLoopJoinOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);  }
  else if (       HashJoinOp::bIsOfType(pNode))   {        HashJoinOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);  }
  else if (  SortMergeJoinOp::bIsOfType(pNode))   {   SortMergeJoinOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);  }
  else if (IndexLookupJoinOp::bIsOfType(pNode))   { IndexLookupJoinOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);  }
  else {
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 JoinOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_JOINOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 JoinOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_JOINOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 JoinOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_JOINOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 JoinOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_JOINOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Return the join type (inner, left outer, etc)                              */
/*----------------------------------------------------------------------------*/
eJT JoinOp::eJoinType(PlanNode* pNode)
{
  QueryNode *pJoinType = JoinOp::pJoinType(pNode);
  return JoinType::eJoinType(pJoinType);
}

/*----------------------------------------------------------------------------*/
/* Return the node that represents the join type (inner, left outer, etc)     */
/*----------------------------------------------------------------------------*/
QueryNode* JoinOp::pJoinType(PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode *pJoinTypeNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_JOINTYPE_PROP), pNode));

  ASSERT(pJoinTypeNode);
  ASSERT(JoinType::bIsOfType(pJoinTypeNode))
  return pJoinTypeNode;
}

/*----------------------------------------------------------------------------*/
/* Return the node representing the join predicate expression                 */
/*----------------------------------------------------------------------------*/
QueryNode* JoinOp::pJoinPredicate(PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode *pPredicate =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_JOINPREDICATE_PROP), pNode));

  ASSERT(pPredicate);
  ASSERT(Expr::bIsOfType(pPredicate))
  return pPredicate;
}


/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions    */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* JoinOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(pNode->nChildren() == 2);

  eJT eJoinType = JoinOp::eJoinType(pNode);

  QueryNode* pChild;

  switch (eJoinType) {

    case JT_INNER:
    case JT_CROSS:
    case JT_LEFT_OUTER:
    case JT_RIGHT_OUTER:
    case JT_FULL_OUTER:
      // return a union of left/right row columns
      return pUnionedColumnExprs(pNode, pRootNode);

    case JT_NATURAL:
      // return distinct union of left/right row columns where there is
      // only one copy of common columns
      return pDistinctUnionedColumnExprs(pNode, pRootNode);

    case JT_SEMI:
    case JT_ANTI:
      // return left row columns only
      pChild = _pQueryNode(pNode->pFirstChild());
      return SqlOperator::pColumnExprs(pChild, pRootNode);

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that are a union of the left and right   */
/* table's projected columns.                                                 */
/*----------------------------------------------------------------------------*/
QueryNodeList* JoinOp::pUnionedColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(pNode->nChildren() == 2);

  // Get the list of ProjectedColumns from the left table
  QueryNode* pLeftChild = _pQueryNode(pNode->pFirstChild());
  QueryNodeList* pLeftColumnExprs = SqlOperator::pColumnExprs(pLeftChild, pRootNode);

  // Get the list of ProjectedColumns from the right table
  QueryNode* pRightChild = _pQueryNode(pNode->pSecondChild());
  QueryNodeList* pRightColumnExprs = SqlOperator::pColumnExprs(pRightChild, pRootNode);

  // Remove all of the columnExprs from the right list and append them to the left list.
  QueryNode* pItem;
  while ((pItem = pRightColumnExprs->pPop()) != nullptr) {
    ASSERT(ProjectedColumn::bIsOfType(pItem));
    pLeftColumnExprs->vAppend(pItem);
  }

  ASSERT(pRightColumnExprs->nItems() == 0);
  delete pRightColumnExprs;

  return pLeftColumnExprs;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that are a distinct union of the left and*/
/*  right table's projected columns. Only one copy of common column names will*/
/* be included.                                                               */
/*----------------------------------------------------------------------------*/
QueryNodeList* JoinOp::pDistinctUnionedColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(pNode->nChildren() == 2);

  // Get the list of ProjectedColumns from the left table
  QueryNode* pLeftChild = _pQueryNode(pNode->pFirstChild());
  QueryNodeList* pLeftColumnExprs = SqlOperator::pColumnExprs(pLeftChild, pRootNode);

  // Get the list of ProjectedColumns from the right table
  QueryNode* pRightChild = _pQueryNode(pNode->pSecondChild());
  QueryNodeList* pRightColumnExprs = SqlOperator::pColumnExprs(pRightChild, pRootNode);

  // Remove all of the columnExprs from the right list and append them to the left list
  // only if they are uniquely named.
  QueryNode* pItem;
  while ((pItem = pRightColumnExprs->pPop()) != nullptr) {

    ASSERT(ProjectedColumn::bIsOfType(pItem));
    if (bHasSameNamedColumn(pLeftColumnExprs, pItem)) {
      // skip it
      continue;
    }

    pLeftColumnExprs->vAppend(pItem);
  }

  pRightColumnExprs->vDeleteAll();
  delete pRightColumnExprs;

  return pLeftColumnExprs;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the list contains a projected column with the same column  */
/* name as the projected column parameter                                     */
/*----------------------------------------------------------------------------*/
b1 JoinOp::bHasSameNamedColumn(QueryNodeList* pProjColList, QueryNode* pProjectedCol)
{
  b1 bResult = false;
  QueryNodeIter* pIter = pProjColList->pNewIter();

  while (pIter->bHasNext()) {
    QueryNode* pListItem = pIter->pNext();
    ASSERT(ProjectedColumn::bIsOfType(pListItem));

    // Check if they have the same alias
    Stz* szItemAlias    = ProjectedColumn::szAliasName(pListItem);
    Stz* szProjColAlias = ProjectedColumn::szAliasName(pProjectedCol);

    if (szItemAlias && szProjColAlias && szItemAlias->bEqual(szProjColAlias)) {
      bResult = true;
      break;
    }

    // Check if they are a columnRef with the same column name
    QueryNode* pItemExpr    = ProjectedColumn::pExprNode(pListItem);
    QueryNode* pProjColExpr = ProjectedColumn::pExprNode(pProjectedCol);

    ASSERT(Expr::bIsOfType(pItemExpr));
    ASSERT(Expr::bIsOfType(pProjColExpr));

    if (ColumnRef::bIsOfType(pItemExpr) && ColumnRef::bIsOfType(pProjColExpr)) {
      Stz* szItemColumnName = ColumnRef::szColumn(pItemExpr);
      Stz* szProjColColumnName = ColumnRef::szColumn(pProjectedCol);

      ASSERT(szItemColumnName);
      ASSERT(szProjColColumnName);

      if (szItemColumnName->bEqual(szProjColColumnName)) {
        bResult = true;
        break;
      }
    }
  }

  delete pIter;
  return bResult;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
