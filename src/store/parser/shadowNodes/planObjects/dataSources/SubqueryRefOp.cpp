/*============================================================================*/
/* SubqueryRefOp.hpp - references a table subquery elsewhere in the plan      */
/*============================================================================*/

#include "BASE.h"
#include "SubqueryRefOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

#include "ShadowRoot.hpp"
#include "NodeNamer.hpp"
#include "StringObject.hpp"
#include "MainSubquery.hpp"
#include "TableSubquery.hpp"
#include "SubqueryMarker.hpp"
#include "CteDef.hpp"
#include "ColumnDef.hpp"
#include "ColumnRef.hpp"
#include "ProjectedColumn.hpp"
#include "StageMarker.hpp"
#include "ExprList.hpp"
#include "MetadataNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource/subqueryRefOp"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create SubqueryRefOp nodes.                             */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefOp::pCreate(Stz* szSubqueryAlias)
{
  return pCreate(new Text(szSubqueryAlias));
}

/*----------------------------------------------------------------------------*/
/* Helper function to create SubqueryRefOp nodes.                             */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefOp::pCreate(Text* pSubqueryAlias)
{
  ASSERT(pSubqueryAlias);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pSubqueryRefOp = new QueryNode(sName, C_DEFN);

  // Add any extra properties here.
  vSetAlias(pSubqueryRefOp, pSubqueryAlias);

  return pSubqueryRefOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SubqueryRefOp::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  Stz* szSubqueryAlias = szAliasName(_pQueryNode(pNode));

  sBuf.vCatSprint("SubqueryRefOp : %s"_Stz, szSubqueryAlias);
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SubqueryRefOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
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
void SubqueryRefOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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

    // Print any parameters for calling this subquery

    QueryNode* pSubqueryRefOp = _pQueryNode(pNode);
    Stz* szAlias = szAliasName(pSubqueryRefOp);

    // Get the list of subquery parameters that were given to the
    // current subquery that this SubqueryRefOp is in.
    QueryNode* pCallerSymbolicParams = PlanObject::pCorrelatedExprs(pSubqueryRefOp, pRootNode);
    ASSERT(pCallerSymbolicParams);
    ASSERT(ExprList::bIsOfType(pCallerSymbolicParams));

    // Get the node that this subqueryRefOp is a proxy for.
    QueryNode* pSubqueryNode = pSubquery(pSubqueryRefOp, pRootNode);

    // The list of correlated columns, in symbolic representation
    // for the subquery we are calling
    QueryNode* pCalleeSymbolicParams = SubqueryMarker::pCorrelatedColumnsNode(pSubqueryNode);
    ASSERT(pCalleeSymbolicParams);
    ASSERT(ExprList::bIsOfType(pCalleeSymbolicParams));

    s2K sExprBuf;
    b1 bIsFirst = true;
    PlanNodeIter* pIter = pCalleeSymbolicParams->pNewChildrenIter();
    while (pIter->bHasNext()) {
      QueryNode* pSymbolicParam = _pQueryNode(pIter->pNext());
      ASSERT(Expr::bIsOfType(pSymbolicParam));

      if (bIsFirst) {
        bIsFirst = false;
      } else {
        sExprBuf.vConcat(", "_Stz);
      }

      Expr::vDescribe(pSymbolicParam, sExprBuf, pRootNode);
    }
    delete pIter;

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("params: %s(%s)"_Stz, szAlias, sExprBuf.szStz());


    // Get the ordinal parameter list
    if (MetadataNode::bHasMetadata(ShadowRoot::pMetadataNode(pRootNode))) {
      sExprBuf.vReset();
      bIsFirst = true;

      // Iterate the symbolic params and convert them to ordinal
      PlanNodeIter* pIter = pCalleeSymbolicParams->pNewChildrenIter();
      while (pIter->bHasNext()) {
        QueryNode* pSymbolicParam = _pQueryNode(pIter->pNext());
        ASSERT(Expr::bIsOfType(pSymbolicParam));

        if (bIsFirst) {
          bIsFirst = false;
        } else {
          sExprBuf.vConcat(", "_Stz);
        }

        QueryNode* pOrdinalParam = Expr::pSymbolicToOrdinal(pSymbolicParam, nullptr, pCallerSymbolicParams, pRootNode);
        Expr::vDescribe(pOrdinalParam, sExprBuf, pRootNode);
        delete pOrdinalParam;
      }
      delete pIter;

      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("ordinal params: %s(%s)"_Stz, szAlias, sExprBuf.szStz());
    }

    return;
  }

  // Manual polymorphism here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Lookup and return the node that this SubqueryRefOp is referencing          */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefOp::pSubquery(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  Stz* szAlias = szAliasName(pNode);

  // Get the path of the current node in the graph
  s2K sNodePath;
  i8 iNodeDepth;

  b1 bFound = GraphNav::bGetPathAndDepth(pNode, pRootNode, sNodePath, iNodeDepth, true);
  ASSERT(bFound);

  // First find the main subquery that this SubqueryRefOp is a child of.
  // If we walk the graph bottom-up along the path obtained above
  // then it will be the first main subquery that we come across.
  QueryNode* pMainSubquery = nullptr;
  s2K sMainSubqueryPath;

  // Lambda to call when we walk the graph upwards from the current node back to the root.
  auto fVisitor =
      [&pMainSubquery, &sMainSubqueryPath](PlanNode* pItem, s2K& sCurPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void* pvArg) -> GraphNav::eWalkCtrl {

    if (!MainSubquery::bIsOfType(pItem)) {
      return GraphNav::WALK_CONTINUE;
    }

    pMainSubquery = _pQueryNode(pItem);
    sMainSubqueryPath.vCopy(sCurPath.szStz());
    return GraphNav::WALK_ABORT;
  };

  // Invoke the walk and check the results.
  b1 bWalkResult = GraphNav::bWalkPath(pRootNode, sNodePath, nullptr, fVisitor, nullptr);

  ASSERT(!bWalkResult); // we should have aborted the walk when we found the node
  ASSERT(pMainSubquery);
  ASSERT(sMainSubqueryPath.nLength() > 0);

  // From the main subquery we again walk the graph upwards looking for a matching TableSubquery.
  // A MainSubquery can resolve aliases belonging to another TableSubquery, TS1, iff:
  //  - the TableSubquery is a sibling of the MainSubquery (both have the same parent),
  //  - The parent of the MainSubquery (and any immediate parents of that) is itself
  //    a TableSubquery, TS2, and both TS1 and TS2 are both siblings, and TS1 is an
  //    earlier child than TS2.
  // The resolving order of subqueries (for a non-recursive subquery) is found by
  // looking at the siblings of pMainSubquery to see if any of them are TableSubqueries.
  // Thereafter, if the parent is itself a TableSubquery then check all siblings of the
  // parent that occur before the parent. The first match is returned. Aliases for
  // TableSubqueries can shadow (hide) one another.
  //
  // Also refer to NameValidator::pResolvableSubqueries()

  QueryNode* pTableSubquery = nullptr;
  QueryNode* pLastNode = nullptr;

  auto fResolver = [&pTableSubquery, &pLastNode, pMainSubquery, szAlias]
                   (PlanNode* pNode, s2K& sCurrentPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void* pvArg)
                   -> GraphNav::eWalkCtrl {
    UNUSED(sCurrentPath);
    UNUSED(iDepth);
    UNUSED(eEvent);
    UNUSED(pvArg);

    if (pNode == pMainSubquery) {
      // Nothing to be done for pMainSubquery. Instead we want to walk its siblings,
      // but for that we need its parent.
      pLastNode = _pQueryNode(pNode);
      return GraphNav::WALK_CONTINUE;
    }

    // Iterate the children of pNode up until the child equals pLastNode or until
    // we find the matching TableSubquery
    PlanNodeIter* pChildIter = pNode->pNewChildrenIter();
    while (pChildIter->bHasNext()) {
      QueryNode* pChild = _pQueryNode(pChildIter->pNext());

      if (pChild == pLastNode) {
        // We don't resolve past pLastNode otherwise it is a forward reference.
        // This is not allowed for a non-recursive subquery.
        break;
      }

      if (TableSubquery::bIsOfType(pChild)) {
        Stz* szChildAlias = TableSubquery::szTableAlias(pChild);
        ASSERT(szChildAlias);

        if (szAlias->bSame(szChildAlias)) {
          // We've got a match!
          pTableSubquery = pChild;
          delete pChildIter;
          return GraphNav::WALK_ABORT;
        }
      }
    }

    delete pChildIter;

    if ( !(SubqueryMarker::bIsOfType(pNode) || StageMarker::bIsOfType(pNode)) ) {
      return GraphNav::WALK_ABORT;
    }

    pLastNode = _pQueryNode(pNode);
    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bWalkPath(pRootNode, sMainSubqueryPath, nullptr, fResolver, nullptr);

  ASSERT(pTableSubquery);
  ASSERT(TableSubquery::bIsOfType(pTableSubquery));
  ASSERT(TableSubquery::szTableAlias(pTableSubquery)->bSame(szAlias));

  return pTableSubquery;
}

/*----------------------------------------------------------------------------*/
/* Return the CteDef node that represents this subquery's definition.         */
/* If psPathBuf is not null then the path of the CteDef is written into it.   */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefOp::pDefnNode(QueryNode* pNode, PlanNode* pRootNode, s2K* psPathBuf)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  QueryNode* pTableSubquery = pSubquery(pNode, pRootNode);
  ASSERT(pTableSubquery);
  ASSERT(TableSubquery::bIsOfType(pTableSubquery));

  Stz* szDefnPath = TableSubquery::szDefnPath(pTableSubquery);
  ASSERT(szDefnPath);

  if (psPathBuf) {
    psPathBuf->vCopy(szDefnPath);
  }

  QueryNode* pCteDef = _pQueryNode(GraphNav::pFind(szDefnPath, pRootNode));
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  return pCteDef;
}


/*----------------------------------------------------------------------------*/
/* Get the list of column definitions for this data source                    */
/* Returned list must be freed, but not its contents.                         */
/*----------------------------------------------------------------------------*/
QueryNodeList* SubqueryRefOp::pColumnDefs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Get the definition of the CTE that this node references
  QueryNode* pCteDef = pDefnNode(pNode, pRootNode);
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  // Create a list for the columnDefs to be returned
  QueryNodeList* pColumnDefList = new QueryNodeList();

  // Iterate the columns in the CTE definition and add them to the above list
  PlanNodeIter* pIter = CteDef::pColumnDefIter(pCteDef);

  while (pIter->bHasNext()) {
    QueryNode* pColumnDef = _pQueryNode(pIter->pNext());
    pColumnDefList->vAppend(pColumnDef);
  }

  delete pIter;
  return pColumnDefList;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions    */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* SubqueryRefOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // We could lookup the table subquery and ask it for its projected columns,
  // however those columns would all be qualified with respect to the base tables
  // used within that subquery. Instead we want all of the columns qualified with
  // respect to the alias that the table subquery is known as. So let's use the
  // table subquery's CteDef to help us do that.

  s2K sCteDefPath;

  QueryNode* pCteDef = pDefnNode(pNode, pRootNode, &sCteDefPath);
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));
  ASSERT(sCteDefPath.nLength() > 0);

  // The (empty) list of projected columns
  QueryNodeList* pProjectedColumns = new QueryNodeList();

  PlanNodeIter* pIter = CteDef::pColumnDefIter(pCteDef);
  while (pIter->bHasNext()) {
    QueryNode* pColumnDef = _pQueryNode(pIter->pNext());
    ASSERT(ColumnDef::bIsOfType(pColumnDef));

    QueryNode* pColRef = ColumnRef::pCreate(CteDef::szAliasName(pCteDef), nullptr, nullptr,
                                            nullptr, ColumnDef::szColumnName(pColumnDef),
                                            sCteDefPath.szStz());

    // Create a ProjectedColumn from this ColumnDef and add it to the list
    QueryNode* pProjectedCol = ProjectedColumn::pCreate(pColRef, nullptr);
    pProjectedColumns->vAppend(pProjectedCol);
  }

  delete pIter;
  return pProjectedColumns;
}

/*----------------------------------------------------------------------------*/
/* Returns true if this data source can resolve the given column reference    */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefOp::bResolvesColumnRef(QueryNode* pNode,    QueryNode* pMetadataNode,
                                     PlanNode* pRootNode, Stz* szColRefTableAliasName,
                                     Stz* szCatalogName,  Stz* szSchemaName,
                                     Stz* szTableName,    Stz* szColumnName)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Get the definition of the CTE that this node references
  QueryNode* pCteDef = pDefnNode(pNode, pRootNode);
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  // Do we have a table alias (not a subquery alias)?
  Stz* szDataSourceAliasName = szAliasName(pNode);

  if (szDataSourceAliasName) {

    // There are two conditions that we need to consider:
    // 1. The columnRef might already be fully qualified. In that case, the columnRef's
    //    table alias might be set. Let's check if it is and if it matches the data
    //    source's alias name.
    // 2. The columnRef is not fully qualified and cannot specify a catalog or schema
    //    name to use this alias, but its tableName may match the alias or it may be left unqualified.
    b1 bNameMatch = false;
    if (szColRefTableAliasName) {
      bNameMatch = szColRefTableAliasName->bSame(szDataSourceAliasName);

    } else {
      bNameMatch = (!szCatalogName &&                                                                // (2)
                    !szSchemaName &&
                    (szTableName == nullptr || szTableName->bSame(szDataSourceAliasName)) );
    }

    if (bNameMatch) {
      // ColumnRef partially or fully matches the alias of this data source. But we
      // still need to check if the DataSource exposes a column with the same name.

      // Test if the column belongs to this subquery definition
      QueryNode* pColumnDef = CteDef::pColumnDef(pCteDef, szColumnName);
      return (pColumnDef != nullptr);
    }

    return false;
  }

  // No alias in this data source, so we try to match against the cat/sch/table names.

  if (szCatalogName == nullptr &&
      szSchemaName  == nullptr &&
      (szTableName  == nullptr || szTableName->bSame(szAliasName(pNode))) ) {

    // This column partially or fully matches the name of this subquery.
    // Test if the column belongs to this subquery definition.
    QueryNode* pColumnDef = CteDef::pColumnDef(pCteDef, szColumnName);
    return (pColumnDef != nullptr);
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns true if this operator (and any expressions it may have) should be  */
/* partitioned when the plan is partitioned. Most operators will simply ask   */
/* this question of their child until we get to a data source. Most tables    */
/* would answer true. A replicated table or data source would answer false.   */
/*----------------------------------------------------------------------------*/
b1 SubqueryRefOp::bIsPartitionable(QueryNode* pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 0);

  // We don't actually know if a SubqueryRefOp is partitionable until the
  // underlying table subquery has itself been partitioned. This is because
  // if the table subquery was really simple, e.g. SELECT col FROM table,
  // then it's final location is still ALL_PARTITIONS. So any main subquery
  // that uses it is still partitionable.
  // However if the table subquery had some operator that brought all of the
  // data back to the origin (e.g. an aggregation/ordering/limit/etc) then
  // this data is no longer partitionable.

  QueryNode* pTableSubquery = pSubquery(pNode, pRootNode);
  ASSERT(pTableSubquery);
  ASSERT(TableSubquery::bIsOfType(pTableSubquery));

  Location::eLO eLocation = PlanObject::eLocation(pTableSubquery, true);
  return (eLocation == Location::LO_ALL_PARTITIONS);

  // All table subqueries are non-partitionable because they are emulating
  // a single complete table. It would be wrong to stream them again on each
  // partition as we would end up with multiple copies of the data - one for
  // each partition.

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
