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
#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"
#include "StringList.hpp"
#include "NullLiteral.hpp"

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
static Stz* C_USINGLIST_PROP = "usingList"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* JoinOp::pCreate(Stz* szDefn, eJT eJoinType, QueryNode* pPredicate, QueryNode* pUsingList)
{
  ASSERT(szDefn);
  ASSERT(JoinOp::bIsOfType(szDefn));
  ASSERT(eJoinType > JT_NONE);
  ASSERT(eJoinType < JT_NENUMS);
  ASSERT(pPredicate || pUsingList);
  ASSERT(!pPredicate || Expr::bIsOfType(pPredicate));
  ASSERT(!pUsingList || StringList::bIsOfType(pUsingList));

  // Create a unique name for this JoinOp object
  s256 sName;
  NodeNamer::vGenerateName(szDefn, sName);

  // Create the object and add the join type and predicate properties.
  // We will need to rename them first.
  QueryNode* pJoinOp = new QueryNode(sName, szDefn);

  QueryNode* pJoinType = JoinType::pCreate(eJoinType);
  pJoinType->vSetName(C_JOINTYPE_PROP);
  pJoinOp->vAppendProperty(pJoinType);

  if (!pPredicate) {
    pPredicate = NullLiteral::pCreate();  // A temporary measure until the using list is converted to a predicate
  }

  pPredicate->vSetName(C_JOINPREDICATE_PROP);
  pJoinOp->vAppendProperty(pPredicate);

  if (pUsingList) {
    pUsingList->vSetName(C_USINGLIST_PROP);
    pJoinOp->vAppendProperty(pUsingList);
  }

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
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object


    // Get the joinType property
    eJT eJoinType = JoinOp::eJoinType(pNode);

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("join type: %s"_Stz, aJT_pretty[eJoinType]);


    // Get the joinPredicate property
    QueryNode* pJoinPredicate = JoinOp::pJoinPredicate(pNode);
    ASSERT(pJoinPredicate);
    ASSERT(Expr::bIsOfType(pJoinPredicate));

    s2K sPredicateBuf;
    Expr::vDescribe(pJoinPredicate, sPredicateBuf);

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("predicate: (%s)"_Stz, sPredicateBuf.szStz());

    // Explaining very early plans are difficult because we cannot resolve the column expressions.
    // We don't want to retrieve any ordinal expressions below before we have been able
    // to resolve all table/column names in the system.
    // When we resolve names we also add metadata into plan. So if the metadata node has
    // no children then we probably haven't resolved any names yet (except for the case of
    // the plan only containing literals).

    if (MetadataNode::bHasMetadata(ShadowRoot::pMetadataNode(pRootNode))) {
      QueryNode* pOrdinalExpr = pOrdinalJoinPredicate(_pQueryNode(pNode), pRootNode);
      sPredicateBuf.vReset();
      Expr::vDescribe(pOrdinalExpr, sPredicateBuf);
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("ordinal predicate: (%s)"_Stz, sPredicateBuf.szStz());
      delete pOrdinalExpr;
    }

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
/* Returns a new copy of the join predicate expression relative to column     */
/* ordinals. Must be freed after use.                                         */
/*----------------------------------------------------------------------------*/
QueryNode* JoinOp::pOrdinalJoinPredicate(PlanNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  // Get the symbolic join predicate expression
  QueryNode *pSymbolicPredicate = pJoinPredicate(pNode);
  ASSERT(pSymbolicPredicate);

  // Get the list of column expressions being given to this plan from its child.
  // This is the list of expression symbols that represent each column of
  // each row that this sql operator will receive.
  //
  // NB: Normally we get the column expressions from the child operator.
  //     However with this being a join operator there are two children
  //     and the ordinals can refer to columns from either child.
  //     So let's get a *combined* column expression list and then create
  //     an ordinal representation based on that *combined* column list.
  //     e.g. if child1 has 3 columns and child2 has 4 columns and we are
  //     trying to create an ordinal representation of the 2nd column from
  //     child2 then we would represent that as $5, being 3 columns from
  //     child1 plus 2 columns from child2.

  ASSERT(pNode->nChildren() == 2);
  QueryNodeList* pColExprList = PlanObject::pInputColumnExprs(_pQueryNode(pNode), pRootNode);
  ASSERT(pColExprList);
  ASSERT(pColExprList->nItems() > 0);

  // Get the list of correlated expressions from the enclosing subquery.
  // We don't need to free this object.
  QueryNode* pCorrelatedExprs = PlanObject::pCorrelatedExprs(_pQueryNode(pNode), pRootNode);

  // Use the column expression list and the symbolic filter expression to
  // generate an ordinal filter expression (one that is relative to column
  // ordinals only).
  QueryNode* pOrdinalPredicate = Expr::pSymbolicToOrdinal(pSymbolicPredicate, pColExprList, pCorrelatedExprs, pRootNode);
  ASSERT(pOrdinalPredicate);

  pColExprList->vDeleteAll();
  delete pColExprList;

  return pOrdinalPredicate;
}

/*----------------------------------------------------------------------------*/
/* Return the internal node that represents the USING column list. May be null*/
/*----------------------------------------------------------------------------*/
QueryNode* JoinOp::pUsingListNode(PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode *pUsingListNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_USINGLIST_PROP), pNode));

  ASSERT(!pUsingListNode || StringList::bIsOfType(pUsingListNode))
  return pUsingListNode;
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
      return PlanObject::pInputColumnExprs(pNode, pRootNode);

    case JT_NATURAL_INNER:
    case JT_NATURAL_LEFT_OUTER:
    case JT_NATURAL_RIGHT_OUTER:
    case JT_NATURAL_FULL_OUTER:
      // return distinct union of left/right row columns where there is
      // only one copy of common columns
      return pDistinctUnionedColumnExprs(pNode, pRootNode);

    case JT_SEMI:
    case JT_ANTI:
      // return left row columns only
      pChild = _pQueryNode(pNode->pFirstChild());
      return PlanObject::pColumnExprs(pChild, pRootNode);

    default:
      NEVER_REACH
  }
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
  QueryNodeList* pLeftColumnExprs = PlanObject::pColumnExprs(pLeftChild, pRootNode);

  // Get the list of ProjectedColumns from the right table
  QueryNode* pRightChild = _pQueryNode(pNode->pSecondChild());
  QueryNodeList* pRightColumnExprs = PlanObject::pColumnExprs(pRightChild, pRootNode);

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


/*----------------------------------------------------------------------------*/
/* Returns the number of projected columns from the first child.              */
/* Useful for working with ordinals.                                          */
/*----------------------------------------------------------------------------*/
i8 JoinOp::iColumnsOfChild1(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  // Unfortunately we need to actually get the list of columns from the first child,
  // get the count and then deallocate it. The child could another join operator
  // where it may not be straight forward to determine how many columns are being
  // projected (e.g. see the case of a NATURAL JOIN).

  QueryNodeList* pColumnList = PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pRootNode);
  i8 iCount = pColumnList->nItems();
  pColumnList->vDeleteAll();
  delete pColumnList;

  return iCount;
}

/*----------------------------------------------------------------------------*/
/* Returns the number of projected columns from the second child.             */
/* Useful for working with ordinals.                                          */
/*----------------------------------------------------------------------------*/
i8 JoinOp::iColumnsOfChild2(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  // Unfortunately we need to actually get the list of columns from the first child,
  // get the count and then deallocate it. The child could another join operator
  // where it may not be straight forward to determine how many columns are being
  // projected (e.g. see the case of a NATURAL JOIN).

  QueryNodeList* pColumnList = PlanObject::pColumnExprs(_pQueryNode(pNode->pSecondChild()), pRootNode);
  i8 iCount = pColumnList->nItems();
  pColumnList->vDeleteAll();
  delete pColumnList;

  return iCount;
}

/*----------------------------------------------------------------------------*/
/* Replace the USING column list with a join predicate                        */
/*----------------------------------------------------------------------------*/
void JoinOp::vReplaceUsing(QueryNode* pNode, QueryNode* pJoinPredicate)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);
  ASSERT(pJoinPredicate);
  ASSERT(Expr::bIsOfType(pJoinPredicate));

  // Remove the existing USING list
  QueryNode* pUsingList = pNode->pRemoveProperty(C_USINGLIST_PROP);
  ASSERT(pUsingList);
  delete pUsingList;

  // The join predicate definition is non-optional, so a dummy null literal expr
  // was inserted as a temporary value. We need to remove and delete it.
  QueryNode* pExistingPredicate = pNode->pRemoveProperty(C_JOINPREDICATE_PROP);
  ASSERT(pExistingPredicate);
  ASSERT(NullLiteral::bIsOfType(pExistingPredicate));
  delete pExistingPredicate;

  // Add in the new join predicate
  pJoinPredicate->vSetName(C_JOINPREDICATE_PROP);
  pNode->vAppendProperty(pJoinPredicate);
}

/*----------------------------------------------------------------------------*/
/* Returns true if this operator (and any expressions it may have) should be  */
/* partitioned when the plan is partitioned. Most operators will simply ask   */
/* this question of their child until we get to a data source. Most tables    */
/* would answer true. A replicated table or data source would answer false.   */
/*----------------------------------------------------------------------------*/
b1 JoinOp::bIsPartitionable(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  // Ask both children
  b1 bChild1Partitionable = PlanObject::bIsPartitionable(_pQueryNode(pNode->pFirstChild()), pRootNode);
  b1 bChild2Partitionable = PlanObject::bIsPartitionable(_pQueryNode(pNode->pSecondChild()), pRootNode);

  // If either child is partitionable then the result is also partitionable.
  return (bChild1Partitionable || bChild2Partitionable);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
