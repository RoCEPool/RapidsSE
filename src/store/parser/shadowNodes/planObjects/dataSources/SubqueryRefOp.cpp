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

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource/subqueryRefOp"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefOp::pCreate(Stz* pSubqueryAlias)
{
  ASSERT(pSubqueryAlias);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pSubqueryRefOp = new QueryNode(sName, C_DEFN);

  // Add any extra properties here.
  vSetAlias(pSubqueryRefOp, new Text(pSubqueryAlias));

  return pSubqueryRefOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SubqueryRefOp::vDescribe(PlanNode* pNode, s2K& sBuf)
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

  // Find the main subquery that this SubqueryRefOp is a child of.
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

  // Get the parent node of the main subquery
  PlanNode* pParent = GraphNav::pGetParentOfPath(sMainSubqueryPath.szStz(), pRootNode);
  ASSERT(pParent);

  // Search the children of pParent for table subqueries, comparing the alias name.
  // First define a lambda to help us
  auto fChildCondition = [szAlias](PlanNode* pItem) -> b1 {
    if (!TableSubquery::bIsOfType(pItem)) {
      return false; // not interested
    }

    QueryNode* pTableSubquery = _pQueryNode(pItem);
    Stz* szAlias2 = TableSubquery::szTableAlias(pTableSubquery);
    ASSERT(szAlias2);

    return (szAlias->bSame(szAlias2));
  };

  // Now use the lambda to search for this child
  QueryNode* pTableSubquery = _pQueryNode(GraphNav::pFindChild(fChildCondition, pParent));
  ASSERT(pTableSubquery);
  ASSERT(TableSubquery::bIsOfType(pTableSubquery));

  return pTableSubquery;
}

/*----------------------------------------------------------------------------*/
/* Return the CteDef node that represents this subquery's definition.         */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryRefOp::pDefnNode(QueryNode* pNode, PlanNode* pRootNode)
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

  QueryNode* pCteDef = pDefnNode(pNode, pRootNode);
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  // The (empty) list of projected columns
  QueryNodeList* pProjectedColumns = new QueryNodeList();

  PlanNodeIter* pIter = CteDef::pColumnDefIter(pCteDef);
  while (pIter->bHasNext()) {
    QueryNode* pColumnDef = _pQueryNode(pIter->pNext());
    ASSERT(ColumnDef::bIsOfType(pColumnDef));

    QueryNode* pColRef = ColumnRef::pCreate(nullptr, nullptr, nullptr,
                                            CteDef::szAliasName(pCteDef), ColumnDef::szColumnName(pColumnDef));

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
    b1 bNameMatch =
        (szColRefTableAliasName && szColRefTableAliasName->bSame(szDataSourceAliasName))  // (1)
        ||
        (!szCatalogName &&                                                                // (2)
         !szSchemaName &&
         (szTableName == nullptr || szTableName->bSame(szDataSourceAliasName)) );

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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
