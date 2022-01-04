/*============================================================================*/
/* NameValidator.hpp - Validates the names of objects used in the plan.       */
/*                     e.g. tables, columns, indexes, etc                     */
/*============================================================================*/

#include "BASE.h"
#include "NameValidator.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "QueryNodeList.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "MetadataNode.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "SqlException.hpp"

#include "SeApi.hpp"
#include "InfoSet.hpp"
#include "TableMetadata.hpp"
#include "PartitionKeyMetadata.hpp"

#include "TableRef.hpp"
#include "ColumnRef.hpp"
#include "AliasRef.hpp"
#include "ColumnWildcard.hpp"
#include "TableDef.hpp"
#include "ColumnDefs.hpp"
#include "ColumnDef.hpp"
#include "IndexDefs.hpp"
#include "IndexDef.hpp"
#include "ProjectOp.hpp"
#include "ProjectedColumn.hpp"
#include "DataSource.hpp"
#include "StatementMarker.hpp"
#include "SubqueryMarker.hpp"
#include "SqlOperator.hpp"
#include "SetOp.hpp"
#include "JoinOp.hpp"
#include "StringObject.hpp"
#include "StringList.hpp"
#include "EqExpr.hpp"
#include "AndExpr.hpp"
#include "MainSubquery.hpp"
#include "TableSubquery.hpp"
#include "ExprSubquery.hpp"
#include "SubqueryRefOp.hpp"
#include "SubqueryRefExpr.hpp"
#include "CteDef.hpp"
#include "CteDefs.hpp"
#include "TableScanOp.hpp"
#include "ProjectedColumns.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Walk the graph and look up all names encountered in the system metadata.   */
/* This function will fill in any unresolved name parts as well as populate   */
/* the plan with metadata.                                                    */
/* A SqlException will be thrown if a name cannot be resolved.                */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateNames(PlanNode *pRootNode)
{
  STORE::SeApi* pSeApi = new STORE::SeApi();

  // First find the metadata node
  QueryNode* pMetadataNode = ShadowRoot::pMetadataNode(pRootNode);
  ASSERT(pMetadataNode);

  // Find the /plan node
  QueryNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);

  // The plan node should have one or more children that are SubqueryMarker nodes
  ASSERT(pPlanNode->nChildren() > 0);

  // The general approach:
  //   * Walk the children graph (not properties) looking for SubqueryMarkers.
  //   * Ignore any ExprSubqueries (shouldn't hit them anyway).
  //   * On entry of a TableSubquery, validate its name against any sibling TableSubqueries
  //     and create a metadata entry.
  //   * On entry of a MainSubquery, fully validate the tables and columns.
  //   * On exit of a TableSubquery, populate the column definitions of its metadata.
  //   * Rewalk all nodes of the graph (properties included) looking for ExprSubqueries.
  //   * An ExprSubquery contains Table/MainSubquery children. So repeat all of the above
  //     for each ExprSubquery encountered.


  try {
    // We need to validate all table names first because the validation of subqueries
    // depends on table names being fully qualified.
    vValidateAllTableNames(pPlanNode, const_cast<Stz *>(C_PLAN_NAME), pRootNode, pMetadataNode, pSeApi);

    // Now complete the validation of subqueries and their columns.
    vValidateAllSubqueries(pPlanNode, const_cast<Stz *>(C_PLAN_NAME), pRootNode, pMetadataNode, pSeApi);
    delete pSeApi;

  } catch (SqlException& e) {
    delete pSeApi;
    throw;
  }

#ifdef WITH_ASSERT_MACRO
  vAssertNamesResolved(pRootNode, pPlanNode);
#endif
}

/*----------------------------------------------------------------------------*/
/* Walks the graph and validates table names in main subqueries               */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateAllTableNames(QueryNode* pStartingNode, Stz* szStartingNodePath,
                                           PlanNode* pRootNode, QueryNode* pMetadataNode,
                                           STORE::SeApi* pSeApi)
{
  ASSERT(pStartingNode);
  ASSERT(szStartingNodePath);
  ASSERT(pRootNode);
  ASSERT(pMetadataNode);

  auto fTableVisitor = [pStartingNode, szStartingNodePath, pRootNode, pMetadataNode, pSeApi]
                       (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl{

    UNUSED(iDepth);
    UNUSED(pvArg);

    // StatementMarkers occur under MainSubquery nodes. TableSubquery and ExprSubquery
    // nodes are basically containers for other subqueries including MainSubquery nodes.
    // So we should be safe to globally walk the plan looking for StatementMarker nodes.
    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    s2K sStatementMarkerFullPath(szStartingNodePath);
    Stz* szLeafSeparator = GraphNav::szLeafSeparator(szStartingNodePath);
    GraphNav::bParentPath(sStatementMarkerFullPath);
    sStatementMarkerFullPath.vCatSprint("%s%s"_Stz, (!szLeafSeparator ? ""_Stz : szLeafSeparator), sPath.szStz());

    s2K sMainSubqueryFullPath;
    QueryNode* pMainSubquery = pFindEnclosingSubquery(sStatementMarkerFullPath, pStartingNode, &sMainSubqueryFullPath);
    ASSERT(pMainSubquery);
    ASSERT(MainSubquery::bIsOfType(pMainSubquery));
    ASSERT(sMainSubqueryFullPath.nLength() > 0);

    // Get a list of table subquery aliases (or nodes) that we could resolve to from this MainSubquery
    QueryNodeList* pResolvableSubqueries = NameValidator::pResolvableTableSubqueries(pMainSubquery,
                                                                                     sMainSubqueryFullPath,
                                                                                     pRootNode);

    try {
      vValidateTables(pRootNode, pStatementMarker, pMainSubquery, pMetadataNode, pSeApi, pResolvableSubqueries);

    } catch (SqlException& e) {
      if (pResolvableSubqueries) {
        pResolvableSubqueries->vRemoveAll();
        delete pResolvableSubqueries;
      }

      throw;
    }

    if (pResolvableSubqueries) {
      pResolvableSubqueries->vRemoveAll();
      delete pResolvableSubqueries;
      pResolvableSubqueries = nullptr;
    }

    return GraphNav::WALK_CONTINUE;
  };

  // We only want to walk children and not properties in order to visit each MainSubquery node
  GraphNav::bGlobalWalk(pStartingNode, false, true, fTableVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Walks the graph and validates table, main subqueries and expr subqueries.  */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateAllSubqueries(QueryNode* pStartingNode, Stz* szStartingNodePath,
                                           PlanNode* pRootNode, QueryNode* pMetadataNode,
                                           STORE::SeApi* pSeApi)
{
  ASSERT(pStartingNode);
  ASSERT(szStartingNodePath);
  ASSERT(pRootNode);
  ASSERT(pMetadataNode);

  // The callback lambda for qualifying table and main subqueries
  auto fMainSubqueryVisitor = [szStartingNodePath, pRootNode, pMetadataNode, pSeApi]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, GraphNav::eWalkEvent eWalkEvent, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!SubqueryMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }


    s2K sFullPath(szStartingNodePath);
    Stz* szLeafSeparator = GraphNav::szLeafSeparator(szStartingNodePath);
    GraphNav::bParentPath(sFullPath);
    sFullPath.vCatSprint("%s%s"_Stz, (!szLeafSeparator ? ""_Stz : szLeafSeparator), sPath.szStz());

    QueryNode* pSubquery = _pQueryNode(pNode);

    // We should hit table subqueries and expr subqueries before we hit main subqueries.
    // We process table subqueries in two steps.

    if (TableSubquery::bIsOfType(pSubquery)) {
      switch (eWalkEvent) {
        case GraphNav::EVENT_ENTER:
          vValidateTableSubqueryEntry(pSubquery, sFullPath, pRootNode, pMetadataNode, pSeApi);
          break;

        case GraphNav::EVENT_EXIT:
          vValidateTableSubqueryExit(pSubquery, sFullPath, pRootNode, pMetadataNode, pSeApi);
          break;

        default:
          NEVER_REACH
      }

    } else if (MainSubquery::bIsOfType(pSubquery)) {
        if (eWalkEvent == GraphNav::EVENT_ENTER) {
          // We ignore exit event for main subqueries as we will fully process the main
          // subquery in one go.
          vValidateMainSubquery(pSubquery, sFullPath, pRootNode, pMetadataNode, pSeApi);
        }

    } else if (ExprSubquery::bIsOfType(pSubquery)) {
      switch (eWalkEvent) {
        case GraphNav::EVENT_ENTER:
          vValidateExprSubqueryEntry(pSubquery, sFullPath, pRootNode, pMetadataNode, pSeApi);
          break;

        case GraphNav::EVENT_EXIT:
          vValidateExprSubqueryExit(pSubquery, sFullPath, pRootNode, pMetadataNode, pSeApi);
          break;

        default:
          NEVER_REACH
      }

    } else {
      NEVER_REACH
    }

    return GraphNav::WALK_CONTINUE;
  };

  // We only want to walk children and not properties
  GraphNav::bGlobalWalk(pStartingNode, false, true,
                        fMainSubqueryVisitor, fMainSubqueryVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validates the table subquery initially. We can't complete the validation   */
/* until the child MainSubquery underneath this is fully validated.           */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateTableSubqueryEntry(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                                PlanNode* pRootNode, QueryNode* pMetadataNode,
                                                STORE::SeApi* pSeApi)
{
  ASSERT(pSubqueryNode);
  ASSERT(TableSubquery::bIsOfType(pSubqueryNode));
  ASSERT(sSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));
  ASSERT(pSeApi);
  UNUSED(pSeApi);

  // Check that the table subquery is uniquely named amongst its siblings.
  vCheckTableSubqueryIsUniquelyNamed(pSubqueryNode, sSubqueryPath, pRootNode);

  // Create an initial metadata entry for this table subquery at the appropriate
  // level & update the cteDefn property of this node.
  vCreateTableSubqueryCteDef(pSubqueryNode, sSubqueryPath, pRootNode, pMetadataNode);
}

/*----------------------------------------------------------------------------*/
/* Completes the validation of the table subquery using main subquery col info*/
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateTableSubqueryExit(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                               PlanNode* pRootNode, QueryNode* pMetadataNode,
                                               STORE::SeApi* pSeApi)
{
  ASSERT(pSubqueryNode);
  ASSERT(TableSubquery::bIsOfType(pSubqueryNode));
  ASSERT(sSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));
  ASSERT(pSeApi);
  UNUSED(sSubqueryPath);
  UNUSED(pMetadataNode);
  UNUSED(pSeApi);

  // When we get here we have previously created the definition (CteDef) for this
  // table subquery and inserted it into the metadata in the appropriate spot.
  // However we haven't filled in the column name list, nor have we validated
  // any column names that the user provided. We will do that here.

  // Get the path of the definition we previously created.
  Stz* szDefnPath = TableSubquery::szDefnPath(pSubqueryNode);
  ASSERT(szDefnPath);
  ASSERT(szDefnPath->bBeginsWith("/"_Stz));

  // Get the node representing the definition.
  QueryNode* pCteDef = _pQueryNode(GraphNav::pFind(szDefnPath, pRootNode));
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  // The main subquery that is a child of this table subquery should now have
  // all its names validated. So we can extract out the columns it projects
  // and insert this into the existing CteDef.

  // We can cheat a bit because when the main subquery was processed we would
  // have already produced the metadata for it, and the column information
  // remains the same. So all we need to do is find the CteDef for the child
  // main subquery and copy the column information from it.

  // We can't assume that there is only one child of pCteDef, so best that we
  // find the main subquery underneath pSubqueryNode, get its definition path
  // and then lookup its CteDef that way.

  QueryNode* pChildMainSubquery = TableSubquery::pChildMainSubquery(pSubqueryNode);
  Stz* szMainSubqueryPath = MainSubquery::szDefnPath(pChildMainSubquery);
  ASSERT(szMainSubqueryPath);

  QueryNode* pMainSubqueryCteDef = _pQueryNode(GraphNav::pFind(szMainSubqueryPath, pRootNode));
  ASSERT(pMainSubqueryCteDef);
  ASSERT(CteDef::bIsOfType(pMainSubqueryCteDef));

  // Check if the number of named columns in the table subquery is greater
  // than the number of columns that the main subquery is returning.
  if (TableSubquery::nColumnNames(pSubqueryNode) > CteDef::nColumns(pMainSubqueryCteDef)) {
    SemanticException::vThrow(EC_TOO_MANY_NAMED_COLUMNS,
                              ": common table expression %s"_Stz,
                              TableSubquery::szTableAlias(pSubqueryNode));
  }

  // Copy each of the column definitions from the child mainSubquery, and if
  // column names were supplied by the user then use these to override the
  // individual column definitions.
  PlanNodeIter* pColDefIter = CteDef::pColumnDefIter(pMainSubqueryCteDef);
  PlanNodeIter* pColNameIter = TableSubquery::pColumnNames(pSubqueryNode)->pNewChildrenIter();

  while (pColDefIter->bHasNext()) {
    QueryNode* pColDef = _pQueryNode(pColDefIter->pNext());
    ASSERT(ColumnDef::bIsOfType(pColDef));

    // Duplicate this column definition
    QueryNode* pColDefDup = pColDef->pDeepCopy(true);

    // Get the next name, if there is one
    if (pColNameIter->bHasNext()) {
      PlanNode* pColNameNode = pColNameIter->pNext();
      ASSERT(StringObject::bIsOfType(pColNameNode));

      Stz* szColName = pColNameNode->szValue();
      ASSERT(szColName);

      // Set this name as an alias in the duplicated definition
      ColumnDef::vSetColumnName(pColDefDup, szColName);
    }

    // Put it in the table subquery's CteDef
    CteDef::vAddColumnDef(pCteDef, pColDefDup);
  }

  // Finally check that all column names in the definition are unique
  pColDefIter->vBeforeFirst();
  while (pColDefIter->bHasNext()) {
    QueryNode* pColDef = _pQueryNode(pColDefIter->pNext());
    ASSERT(ColumnDef::bIsOfType(pColDef));

    PlanNodeIter* pColDefIter2 = CteDef::pColumnDefIter(pMainSubqueryCteDef);
    while (pColDefIter2->bHasNext()) {
      QueryNode* pColDef2 = _pQueryNode(pColDefIter2->pNext());

      if (pColDef == pColDef2) {
        continue;
      }

      Stz* szColName1 = ColumnDef::szColumnName(pColDef);
      Stz* szColName2 = ColumnDef::szColumnName(pColDef2);

      if (szColName1->bSame(szColName2)) {
        delete pColDefIter2;
        delete pColNameIter;
        delete pColDefIter;

        SemanticException::vThrow(EC_DUPLICATE_COLUMN,
                                  ": %s, in common table expression %s"_Stz,
                                  szColName1, TableSubquery::szTableAlias(pSubqueryNode));
      }
    }
    delete pColDefIter2;
  }

  delete pColNameIter;
  delete pColDefIter;
}

/*----------------------------------------------------------------------------*/
/* Validates the main subquery                                                */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateMainSubquery(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                          PlanNode* pRootNode, QueryNode* pMetadataNode,
                                          STORE::SeApi* pSeApi)
{
  ASSERT(pSubqueryNode);
  ASSERT(MainSubquery::bIsOfType(pSubqueryNode));
  ASSERT(sSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));
  ASSERT(pSeApi);

  // The callback lambda for qualifying columns
  auto fColumnVisitor = [pSubqueryNode, &sSubqueryPath, pRootNode, pMetadataNode, pSeApi]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (eEvent == GraphNav::EVENT_ENTER && SubqueryMarker::bIsOfType(pNode) && pNode != pSubqueryNode) {
      // We're only interested in this subquery. Skip any others we encounter.
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    if (eEvent != GraphNav::EVENT_EXIT || !StatementMarker::bIsOfType(pNode)) {
      // We only want to process StatementMarker sections in a bottom up fashion.
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    // Run the validation logic on the logical subgraph of each StatementMarker encountered.
    // Convert any column wildcards to column references
    vResolveColumnWildcards(pRootNode, pStatementMarker, pSubqueryNode, pMetadataNode);

    // Now resolve the columns against the known tables in the plan.
    vValidateColumns(pRootNode, pStatementMarker, pSubqueryNode, sSubqueryPath, pMetadataNode);

    // Validate the column names of any join USING clauses
    vValidateJoinUsingColumns(pRootNode, pStatementMarker);

    // Validate that all SubqueryRefExprs can find the corresponding ExprSubquery
    vValidateSubqueryReferences(pRootNode, pStatementMarker);

    // There may be invalid expressions in the projection list that might be solved
    // by implicit coercion. We need to try this here because later in this function
    // we make a CTE definition that uses the projected column's data types.
    // This overlaps with the coercion that occurs in the next transformation stage,
    // ExprValidation (see ExprValidator::vExprTypeCoercion()). The difference is
    // that here we are only going to check the projected columns and not every
    // expression in the plan.
    vCoerceProjectedExprs(pRootNode, pStatementMarker);

    return GraphNav::WALK_CONTINUE;
  };


  // Validate the tables first, then the columns separately.
  // We use the enter/exit callback of the walk algorithm with the column visitor
  // as we need to constrain the walk to the current subquery (which we detect in
  // the enter event) but we validate/resolve the columns on a bottom-up approach
  // (using the exit event) so that we can qualify any columnRefs lower in the plan
  // before we hit any aliases higher up that then copy those expressions.
  try {
    GraphNav::bGlobalWalk(pSubqueryNode, false, true, fColumnVisitor, fColumnVisitor, pRootNode);

  } catch (SqlException& e) {
    throw;
  }

  // Finally, create a CteDef for the main subquery and populate it with information.
  vCreateMainSubqueryCteDef(pSubqueryNode, sSubqueryPath, pRootNode, pMetadataNode);
}

/*----------------------------------------------------------------------------*/
/* Validates an expression subquery on descent of the graph                   */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateExprSubqueryEntry(QueryNode* pSubqueryExprNode, s2K& sSubqueryExprPath,
                                               PlanNode* pRootNode, QueryNode* pMetadataNode,
                                               STORE::SeApi* pSeApi)
{
  ASSERT(pSubqueryExprNode);
  ASSERT(ExprSubquery::bIsOfType(pSubqueryExprNode));
  ASSERT(sSubqueryExprPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));
  ASSERT(pSeApi);
  UNUSED(pSeApi);

  // When this function is invoked we should have already validated and resolved
  // the sibling MainSubquery because the MainSubquery would occur before this node
  // in the graph. This is important because then any correlated column names in the
  // outer query will be fully qualified.

  // Create an initial CteDef for this node and insert it into the metadata.
  // Save the path to this CteDef in this current node.
  vCreateExprSubqueryCteDef(pSubqueryExprNode, sSubqueryExprPath, pRootNode, pMetadataNode);
}

/*----------------------------------------------------------------------------*/
/* Completes the validation of an expression subquery on ascent of the graph  */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateExprSubqueryExit(QueryNode* pSubqueryExprNode, s2K& sSubqueryExprPath,
                                              PlanNode* pRootNode, QueryNode* pMetadataNode,
                                              STORE::SeApi* pSeApi)
{
  ASSERT(pSubqueryExprNode);
  ASSERT(ExprSubquery::bIsOfType(pSubqueryExprNode));
  ASSERT(sSubqueryExprPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));
  ASSERT(pSeApi);
  UNUSED(pSeApi);

  // Populate the remaining CteDef fields (we can copy column info from the CteDef of the
  // MainSubquery child that was processed when we descended the graph).
  vUpdateExprSubqueryCteDef(pSubqueryExprNode, sSubqueryExprPath, pRootNode, pMetadataNode);

  // TODO: * Create a list of any correlated column names and populate this in the current exprSubquery node.
  //         Note that a subquery can be correlated because its child subquery references a column in its grandparent.
}


/*----------------------------------------------------------------------------*/
/* Validate and qualify all of the tableRefs in the graph                     */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateTables(PlanNode *pRootNode, QueryNode* pStartNode,
                                    QueryNode* pEnclosingSubqueryNode,
                                    QueryNode* pMetadataNode, STORE::SeApi* pSeApi,
                                    QueryNodeList* pResolvableSubqueries)
{
  ASSERT(pRootNode);
  ASSERT(pEnclosingSubqueryNode);
  ASSERT(pMetadataNode);
  ASSERT(pStartNode);
  ASSERT(StatementMarker::bIsOfType(pStartNode));
  ASSERT(!pResolvableSubqueries || pResolvableSubqueries->nItems() > 0);

  // A list of nodes to be replaced and their replacements. Can only do this after
  // the walk has finished. The node to be replaced comes first, then the node
  // that is replacing it comes second.
  QueryNodeList* pOpReplacementList = new QueryNodeList();

  // Walk the graph looking for TableRefs
  auto fVisitor = [&pMetadataNode, &pSeApi, pResolvableSubqueries, pStartNode, &pOpReplacementList, pEnclosingSubqueryNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
    UNUSED(pRootNode);

    if (!TableRef::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pTableRef = _pQueryNode(pNode);

    Stz* szCatalogName = TableRef::szCatalog(_pQueryNode(pNode));
    Stz* szSchemaName  = TableRef::szSchema(_pQueryNode(pNode));
    Stz* szTableName   = TableRef::szTable(_pQueryNode(pNode));

    // Is this table already in the metadata? It must have a fully qualified name if it does.
    if (TableRef::bIsFullyQualified(pTableRef)) {
      QueryNode* pTableDef = MetadataNode::pTableDef(pMetadataNode, szCatalogName, szSchemaName, szTableName);
      if (pTableDef) {
        // Table is in the metadata. Nothing to do here.
        return GraphNav::WALK_CONTINUE;   // Keep walking
      }
    }

    // Table is not fully qualified or not in the plan's metadata.

    // First we try to resolve the name to a table subquery. This takes precedence over
    // any real tables (following the approach used in Postgres here...can always be changed).
    if (bResolveTableNameToSubquery(szCatalogName, szSchemaName, szTableName,
                                    pResolvableSubqueries, sPath, pStartNode,
                                    pOpReplacementList)) {
      return GraphNav::WALK_CONTINUE;
    }


    if (bResolveTableNameToBaseTable(szCatalogName, szSchemaName, szTableName,
                                     pTableRef, pMetadataNode, pSeApi)) {
      return GraphNav::WALK_CONTINUE;
    }

    // Unknown table, throw an exception
    // Check if there are any table subqueries that are out of scope that could
    // be a match. Then we can generate a better error message. If not, then
    // simply issue the unknown table error.
    vCheckNameAgainstSubqueryNotInScope(szCatalogName, szSchemaName, szTableName, pRootNode, pEnclosingSubqueryNode);

    // Couldn't correlate the name to any out-of-scope table subqueries, so
    // it is just an unknown table.
    NameValidationException::vThrowUnknownTable(szCatalogName, szSchemaName, szTableName);
    return GraphNav::WALK_ABORT; // Unnecessary, but for the compiler's benefit only
  };

  // Walk the StatementMarker-bounded section, it's properties and its children
  try {
    GraphNav::bBoundedWalk(pStartNode, true, true, fVisitor, pRootNode);

  } catch (SqlException& e) {
    if (pOpReplacementList) {
      while (pOpReplacementList->pHead() != nullptr) {
        QueryNode* pOp;
        pOp = pOpReplacementList->pPop(); // ignore this one, it's still in the graph
        pOp = pOpReplacementList->pPop(); // delete the replacement

        delete pOp;
      }

      delete pOpReplacementList;
    }

    throw;
  }

  // Process any node replacements for table subqueries.
  if (pOpReplacementList) {
    while (pOpReplacementList->pHead() != nullptr) {
      QueryNode* pOldOp = pOpReplacementList->pPop();
      QueryNode* pNewOp = pOpReplacementList->pPop();

      GraphEditor::vReplaceNode(pStartNode, pOldOp, pNewOp);
    }

    delete pOpReplacementList;
  }
}

/*----------------------------------------------------------------------------*/
/* Tries to resolve a table name to a table subquery. If successful, returns  */
/* true and writes the changes to pOpReplacmentList.                          */
/*----------------------------------------------------------------------------*/
b1 NameValidator::bResolveTableNameToSubquery(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                                              QueryNodeList* pResolvableSubqueries, s2K& sPath,
                                              QueryNode* pStartNode, QueryNodeList* pOpReplacementList)
{
  ASSERT(szTableName);
  ASSERT(sPath.nLength() > 0);
  ASSERT(pStartNode);
  ASSERT(pOpReplacementList);

  if (pResolvableSubqueries && szTableName && !szSchemaName && !szCatalogName) {
    QueryNodeIter* pIter = pResolvableSubqueries->pNewIter();

    while (pIter->bHasNext()) {
      QueryNode* pSubquery = pIter->pNext();
      ASSERT(TableSubquery::bIsOfType(pSubquery));

      Stz* szAlias = TableSubquery::szTableAlias(pSubquery);
      if (!szAlias->bSame(szTableName)) {
        continue;
      }

      // We've got a match on the table subquery.
      // We now need to replace the operator that contains this TableRef
      // with a SubqueryRefOp. We don't want to do this inside a walk so
      // instead let's add the changes to a replacement list and we can
      // replace the affected nodes after the walk is complete.

      // Get the parent operator of the TableRef
      PlanNode* pParent = GraphNav::pGetParentOfPath(sPath, pStartNode);
      ASSERT(pParent);
      ASSERT(TableScanOp::bIsOfType(pParent));

      // Create the new SubqueryRefOp
      QueryNode* pSubqueryRefOp = SubqueryRefOp::pCreate(szAlias);

      // Add to the operator replacement list
      pOpReplacementList->vAppend(_pQueryNode(pParent));
      pOpReplacementList->vAppend(pSubqueryRefOp);

      delete pIter;
      return true;
    }

    delete pIter;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Tries to resolve a table name to a base table. Returns true if successful  */
/* and qualifies the tableRef.                                                */
/* Will throw an exception if the name is ambiguous within base tables.       */
/*----------------------------------------------------------------------------*/
b1 NameValidator::bResolveTableNameToBaseTable(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                                              QueryNode* pTableRef, QueryNode* pMetadataNode, SeApi* pSeApi)
{
  ASSERT(szTableName);
  ASSERT(pTableRef);
  ASSERT(TableRef::bIsOfType(pTableRef));
  ASSERT(pMetadataNode);
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));
  ASSERT(pSeApi);

  // Look up table in SE API
  STORE::InfoSet* pTableInfoSet = pSeApi->pSearchTables(szCatalogName, szSchemaName, szTableName);
  ASSERT(pTableInfoSet);

  if (pTableInfoSet->nRows() == 0) {
    // We couldn't resolve the table in the SE metadata
    delete pTableInfoSet;
    return false;
  }

  if (pTableInfoSet->nRows() > 1) {
    // We've resolved more than one table in the metadata. The table name is ambiguous.
    delete pTableInfoSet;
    NameValidationException::vThrowAmbiguousTable(szCatalogName, szSchemaName, szTableName, pTableInfoSet);
  }

  ASSERT(pTableInfoSet->nRows() == 1);

  Row* pTblRow = pTableInfoSet->pFirstRow();
  TableMetadata* pTableMeta = static_cast<TableMetadata*>(pTblRow);

  // Get the full name from the metadata as we may not have had all of the details before
  szCatalogName = pTableMeta->szCatalogName();
  szSchemaName = pTableMeta->szSchemaName();
  szTableName = pTableMeta->szTableName();

  // Now that the table is fully qualified, check again if this is already in the metadata
  QueryNode* pTableDef = MetadataNode::pTableDef(pMetadataNode, szCatalogName, szSchemaName, szTableName);
  if (pTableDef) {
    TableRef::vQualify(pTableRef, szCatalogName, szSchemaName, szTableName);
    delete pTableInfoSet;
    return true;
  }

  // We need to create the metadata definition for this table.

  // Retrieve the column definitions and make a node out of these
  QueryNode* pColumnDefs = ColumnDefs::pCreate();
  InfoSet* pColumnInfoSet = pSeApi->pColumnDefs(szCatalogName, szSchemaName, szTableName, nullptr);

  for (Row* pColRow = pColumnInfoSet->pFirstRow();
       pColRow;
       pColRow = pColumnInfoSet->pNextRow(pColRow)) {

    ColumnMetadata* pColMeta = static_cast<ColumnMetadata*>(pColRow);

    // Convert the metadata type string to an enum and use this to create
    // a QueryNode of the correct type.
    QueryNode* pTypeNode = SqlType::pCreate(static_cast<eCT>(pColMeta->iTypeEnum()));

    // Make the column definition node and add it to the list of column definitions
    QueryNode* pColumnDef = ColumnDef::pCreate(pColMeta->szCatalogName(),
                                               pColMeta->szSchemaName(),
                                               pColMeta->szTableName(),
                                               pColMeta->szColumnName(),
                                               pTypeNode,
                                               !pColMeta->bIsNullable(),
                                               pColMeta->iColumnNumber(),
                                               pColMeta->iColumnLength(),
                                               pColMeta->iColumnScale());
    ColumnDefs::vAddColumnDef(pColumnDefs, pColumnDef);
  }

  // TODO: Get the index definitions and make a node out of it.
  // TODO: For now this is empty
  QueryNode* pIndexDefs = IndexDefs::pCreate();

  // Get the partitioning information and make a node out of it.
  QueryNode* pPartitionDef = ColumnDefs::pCreate();
  InfoSet* pPartitionKeySet = pSeApi->pPartitionKey(szCatalogName, szSchemaName, szTableName);
  ASSERT(pPartitionKeySet);

  for (Row* pPtnKeyRow = pPartitionKeySet->pFirstRow();
       pPtnKeyRow;
       pPtnKeyRow = pPartitionKeySet->pNextRow(pPtnKeyRow)) {

    PartitionKeyMetadata* pPtnKeyMeta = static_cast<PartitionKeyMetadata*>(pPtnKeyRow);

    QueryNode* pColumnDef = ColumnDefs::pFind(pColumnDefs, pPtnKeyMeta->iColumnNumber());
    ASSERT(pColumnDef);

    // Add a copy of this node to the partition definition.
    ColumnDefs::vAddColumnDef(pPartitionDef, pColumnDef->pDeepCopy(true));
  }


  // Now form a TableDef object out of the table metadata, columnDefs and indexDefs
  pTableDef = TableDef::pCreate(szCatalogName, szSchemaName, szTableName, pColumnDefs, pIndexDefs, pPartitionDef);

  // Add to the plan metadata
  MetadataNode::vAddTable(pMetadataNode, pTableDef);

  // Qualify the TableRef
  TableRef::vQualify(pTableRef, szCatalogName, szSchemaName, szTableName);

  delete pTableInfoSet;
  delete pColumnInfoSet;
  delete pPartitionKeySet;

  return true;
}

/*----------------------------------------------------------------------------*/
/* Convert any column wildcards into column references                        */
/*----------------------------------------------------------------------------*/
void NameValidator::vResolveColumnWildcards(PlanNode *pRootNode, QueryNode* pStartNode,
                                            QueryNode* pEnclosingSubqueryNode, QueryNode* pMetadataNode)
{
  UNUSED(pMetadataNode);

  ASSERT(pRootNode);
  ASSERT(pEnclosingSubqueryNode);
  ASSERT(pMetadataNode);
  ASSERT(pStartNode);
  UNUSED(pEnclosingSubqueryNode);

  // Walk the graph looking for ColumnWildcards
  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!ProjectOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pProjectOp = _pQueryNode(pNode);

    // Get the list of projected expressions and iterate these,
    // looking for columnWildcards.
    QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pProjectOp);

    if (pProjectedColumns->nChildren() == 0) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }


    PlanNodeIter *pIter = pProjectedColumns->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode *pProjectedColumn = static_cast<QueryNode *>(pIter->pNext());

      // Get the expr out of the ProjectedColumn
      QueryNode *pExpr = ProjectedColumn::pExprNode(pProjectedColumn);

      if (!ColumnWildcard::bIsOfType(pExpr)) {
        continue;
      }

      // We've got a columnWildcard. We need to remove this projectedColumn
      // from the projectedColumns list and replace it with new projectedColumn
      // instances that have columnRefs from the resolved tables.

      // First get any catalog/schema/table scopes that may limit the wildcard
      Stz* szCatalogName = ColumnWildcard::szCatalog(_pQueryNode(pExpr));
      Stz* szSchemaName  = ColumnWildcard::szSchema(_pQueryNode(pExpr));
      Stz* szTableName   = ColumnWildcard::szTable(_pQueryNode(pExpr));

      // Now generate a list of ProjectedColumn instances, but importantly we can only
      // use tableRefs that we find underneath this part of the graph, as there can be
      // other tableRefs that are not visible to this part of the query.
      QueryNodeList* pResolvedWildcardList = pLookupWildcardColumns(_pQueryNode(pNode->pFirstChild()),
                                                                   pRootNode,
                                                                   szCatalogName,
                                                                   szSchemaName,
                                                                   szTableName);

      // Ensure that the returned list is a list of ProjectedColumn instances
#ifdef WITH_ASSERT_MACRO
      QueryNodeIter* pResolvedIter = pResolvedWildcardList->pNewIter();
      while (pResolvedIter->bHasNext()) {
        PlanNode* pResolvedNode = pResolvedIter->pNext();
        ASSERT(ProjectedColumn::bIsOfType(pResolvedNode));
      }
      delete pResolvedIter;
#endif

      if (pResolvedWildcardList->nItems() == 0) {
        delete pResolvedWildcardList;
        delete pIter;
        NameValidationException::vThrowUnresolvableColumnWildcard(szCatalogName, szSchemaName, szTableName);
        return GraphNav::WALK_ABORT; // NEVER_REACH. For the IDE's code-warning benefit
      }

      // Now replace the ProjectedColumn that contains the columnWildcard with
      // each of the resolved ProjectedColumn instances that contain columnRefs.
      QueryNode* pResolvedCol;
      while ((pResolvedCol = pResolvedWildcardList->pPop()) != nullptr) {
        pIter->vInsertBefore(pResolvedCol);
      }

      ASSERT(pResolvedWildcardList->nItems() == 0);
      delete pResolvedWildcardList;

      pIter->vRemove();
      delete pProjectedColumn;
    }

    delete pIter;

    return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ProjectOps
  };

  // Walk the /plan node and its children only. We only want to stop at ProjectOps.
  // We do this so that we can modify the properties of the ProjectOp without it
  // affecting the walking of the graph. We do not want to modify the graph that
  // we are walking as this will lead to bad conclusions.
  GraphNav::bBoundedWalk(pStartNode, false, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Look up and return a list of columnRefs that match a column wildcard       */
/* for a given location in the plan hierarchy.                                */
/*----------------------------------------------------------------------------*/
QueryNodeList* NameValidator::pLookupWildcardColumns(QueryNode* pStartNode, PlanNode* pRootNode,
                                                    Stz* szCatalog, Stz* szSchema, Stz* szTable)
{
  ASSERT(pStartNode);
  ASSERT(PlanObject::bIsOfType(pStartNode));

  QueryNodeList* pProjectedColumnList = PlanObject::pColumnExprs(pStartNode, pRootNode);

  // If there are no constraints then we use all projected columns
  if (!szCatalog && !szSchema && !szTable) {
    return pProjectedColumnList;
  }

  // There are some constraints that we need to match against
  QueryNodeIter* pIter = pProjectedColumnList->pNewIter();
  while (pIter->bHasNext()) {
    QueryNode* pProjectedColumn = pIter->pNext();
    ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

    QueryNode* pExpr = ProjectedColumn::pExprNode(pProjectedColumn);
    ASSERT(Expr::bIsOfType(pExpr));

    // We can only match cat/schm/tbl constraints against columnRefs,
    // so if it isn't a ColumnRef then it can't be matched and we won't include it.
    if (!ColumnRef::bIsOfType(pExpr)) {
      pIter->vRemove();
      delete pProjectedColumn;
      continue;
    }

    Stz* szColRefTblAlias = ColumnRef::szTableAlias(pExpr);
    Stz* szColRefCatalog  = ColumnRef::szCatalog(pExpr);
    Stz* szColRefSchema   = ColumnRef::szSchema(pExpr);
    Stz* szColRefTable    = ColumnRef::szTable(pExpr);

    b1 bConstraintsMatch;

    if (szColRefTblAlias && szTable) {
      bConstraintsMatch = (szTable->bSame(szColRefTblAlias) && !szSchema && !szCatalog);

    } else {
      bConstraintsMatch =
          (!szCatalog || (szColRefCatalog && szCatalog->bSame(szColRefCatalog))) &&
          (!szSchema  || (szColRefSchema  && szSchema->bSame(szColRefSchema))  ) &&
          (!szTable   || (szColRefTable   && szTable->bSame(szColRefTable))    );
    }

    if (!bConstraintsMatch) {
      // ColumnRef doesn't match
      pIter->vRemove();
      delete pProjectedColumn;
      continue;
    }
  }

  delete pIter;
  return pProjectedColumnList;
}

/*----------------------------------------------------------------------------*/
/* Validate and qualify all of the columnRefs using the known tables          */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateColumns(PlanNode *pRootNode, QueryNode* pStartNode,
                                     QueryNode* pEnclosingSubqueryNode, s2K& sEnclosingSubqueryPath,
                                     QueryNode* pMetadataNode, QueryNode* pKnownSqlOp)
{
  ASSERT(pRootNode);
  ASSERT(!pEnclosingSubqueryNode || sEnclosingSubqueryPath.nLength() > 0);
  ASSERT(pMetadataNode);
  ASSERT(pStartNode);
  ASSERT(StatementMarker::bIsOfType(pStartNode) || Expr::bIsOfType(pStartNode));
  ASSERT(!pKnownSqlOp || SqlOperator::bIsOfType(pKnownSqlOp));

  // Walk the graph looking for ColumnRefs
  auto fVisitor = [&pMetadataNode, pStartNode, pEnclosingSubqueryNode, &sEnclosingSubqueryPath, pKnownSqlOp]
                  (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!ColumnRef::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pColumnRef = _pQueryNode(pNode);

    // We cannot skip columns that appear to be fully qualified here. Even if a columnRef
    // has a catalog, schema and table specified AND all of this exists in the metadata
    // AND that table has a matching column name, there may be an alias on the data source
    // that means that the column cannot be resolved with that fully qualified name.
    // Therefore this name is incorrect and should cause an error.

    // We either need to match this partially (or fully) qualified column against the plan
    // graph metadata, against an alias for a projected expression or against
    // a table alias found below it in the plan.
    //
    // Let's first consider matching it against a column from a table.
    // First we need to work out which table it might be from by looking for TableRefs
    // underneath the sql operator that contains this columnRef.

    // Get the Sql operator that contains this ColumnRef expression
    // by using the current path and the starting plan node
    QueryNode* pSqlOp = pKnownSqlOp;

    if (!pSqlOp) {
      s2K sSqlOpPath;
      sSqlOpPath.vCopyTill(sPath, C_PROPERTY_SEPARATOR);

      pSqlOp = _pQueryNode(GraphNav::pFind(sSqlOpPath.szStz(), pStartNode));
      ASSERT(pSqlOp);
      ASSERT(SqlOperator::bIsOfType(pSqlOp));
    }

    b1 bConsiderCorrelation = (pEnclosingSubqueryNode != nullptr);

    bValidateColumnRef(pColumnRef, pSqlOp, bConsiderCorrelation, true, pEnclosingSubqueryNode, sEnclosingSubqueryPath, pRootNode, pMetadataNode);
    return GraphNav::WALK_CONTINUE;
  };

  // Walk the starting node (a StatementMarker), it's properties and its children.
  // We do a bottom up walk because there may be aliases higher up the plan
  // that refer to exprs lower down and we want to qualify columnRefs in those
  // lower expressions before we hit the aliases higher up.
  GraphNav::bBoundedWalkBottomUp(pStartNode, true, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Validates a single ColumnRef                                               */
/* Always throws an exception if a ColumnRef is ambiguous.                    */
/* Throws an exception if column is unknown and bThrowWhenUnresolved is true. */
/*----------------------------------------------------------------------------*/
b1 NameValidator::bValidateColumnRef(QueryNode* pColumnRef, QueryNode* pSqlOp,
                                     b1 bConsiderCorrelation, b1 bThrowWhenUnresolved,
                                     QueryNode* pEnclosingSubqueryNode, s2K& sEnclosingSubqueryPath,
                                     PlanNode* pRootNode, QueryNode* pMetadataNode)
{
  ASSERT(pColumnRef);
  ASSERT(ColumnRef::bIsOfType(pColumnRef));
  ASSERT(pSqlOp);
  ASSERT(SqlOperator::bIsOfType(pSqlOp));
  ASSERT(!bConsiderCorrelation || pEnclosingSubqueryNode);
  ASSERT(!bConsiderCorrelation || MainSubquery::bIsOfType(pEnclosingSubqueryNode));
  ASSERT(!bConsiderCorrelation || sEnclosingSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(pMetadataNode);
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));

  Stz *szTableAliasName = ColumnRef::szTableAlias(pColumnRef);
  Stz *szCatalogName = ColumnRef::szCatalog(pColumnRef);
  Stz *szSchemaName = ColumnRef::szSchema(pColumnRef);
  Stz *szTableName = ColumnRef::szTable(pColumnRef);
  Stz *szColumnName = ColumnRef::szColumn(pColumnRef);
  Stz *szDefnPath = ColumnRef::szDefnPath(pColumnRef);

  QueryNodeList* pDataSourceList = pMatchingDataSourcesFor(pColumnRef, pSqlOp, pRootNode, pMetadataNode);

  // We can check for an ambiguous column name
  if (pDataSourceList->nItems() > 1) {
    // pDataSourceList will be deleted in vThrowAmbiguousColumn() below
    NameValidationException::vThrowAmbiguousColumn(szCatalogName, szSchemaName, szTableName, szColumnName, pDataSourceList);
  }

  if (szCatalogName || szSchemaName || szTableName) {
    // pColumnRef must be a columnRef. Can't be an aliasRef

    if (pDataSourceList->nItems() == 0) {
      delete pDataSourceList;

      if (bConsiderCorrelation) {
        // Could be a correlated column
        b1 bIsCorrelated = bResolveAsCorrelatedColumn(pColumnRef, pEnclosingSubqueryNode,
                                                      sEnclosingSubqueryPath, pRootNode);

        if (bIsCorrelated) {
          return true;   // Keep walking, looking for more ColumnRefs
        }
      }

      // Not correlated and not resolved, so column name is unknown
      if (bThrowWhenUnresolved) {
        NameValidationException::vThrowUnknownColumn(szCatalogName, szSchemaName, szTableName, szColumnName);
      }

      return false;
    }

    ASSERT(pDataSourceList->nItems() == 1);

    // Qualify the ColumnRef
    QueryNode* pDataSource = _pQueryNode(pDataSourceList->pPop());
    szTableAliasName = DataSource::szAliasName(pDataSource);
    szCatalogName    = DataSource::szCatalogName(pDataSource);
    szSchemaName     = DataSource::szSchemaName(pDataSource);
    szTableName      = DataSource::szTableName(pDataSource);
    szDefnPath       = nullptr;

    if (SubqueryRefOp::bIsOfType(pDataSource)) {
      QueryNode* pSubquery = SubqueryRefOp::pSubquery(pDataSource, pRootNode);
      ASSERT(pSubquery);
      ASSERT(TableSubquery::bIsOfType(pSubquery));
      szDefnPath = TableSubquery::szDefnPath(pSubquery);
    }

    ColumnRef::vQualify(pColumnRef, szTableAliasName, szCatalogName,
                        szSchemaName, szTableName, szColumnName, szDefnPath);

    delete pDataSourceList;
    return true;   // Keep walking, looking for more ColumnRefs
  }

  // The ColumnRef only has a column name. No other qualifying names.
  // pSqlOp could be projecting expressions that have an alias that match
  // this ColumnRef's column name. Get a list of these projected columns.
  // There could still be an ambiguity in the alias/column name.
  QueryNodeList* pProjectedAliasList = pMatchingExprAliasesFor(szColumnName, pSqlOp, pRootNode);

  // Filter out the aliases whose expressions are already qualified ColumnRefs.
  // That way we don't get incorrect errors about a name being ambiguous because
  // it can resolve to a ColumnRef as well as an Alias, but that Alias resolves
  // to the same ColumnRef.
  if (pProjectedAliasList->nItems() > 0 && pDataSourceList->nItems() > 0) {

    QueryNodeIter* pDataSourceIter = pDataSourceList->pNewIter();
    while (pDataSourceIter->bHasNext()) {

      QueryNode* pDataSource = pDataSourceIter->pNext();
      ASSERT(DataSource::bIsOfType(pDataSource));

      QueryNodeIter* pProjectedColIter = pProjectedAliasList->pNewIter();
      while (pProjectedColIter->bHasNext()) {
        QueryNode* pProjectedCol = pProjectedColIter->pNext();
        ASSERT(ProjectedColumn::bIsOfType(pProjectedCol));

        QueryNode* pExpr = ProjectedColumn::pExprNode(pProjectedCol);
        ASSERT(!ColumnRef::bIsOfType(pExpr) || ColumnRef::bIsFullyQualified(pExpr));

        if (ColumnRef::bIsOfType(pExpr) &&
            ColumnRef::szCatalog(pExpr)->bSame(DataSource::szCatalogName(pDataSource)) &&
            ColumnRef::szSchema(pExpr)->bSame(DataSource::szSchemaName(pDataSource)) &&
            ColumnRef::szTable(pExpr)->bSame(DataSource::szTableName(pDataSource)) ) {

          // Remove the projected column from the list.
          pProjectedColIter->vRemove();
          delete pProjectedCol;
        }
      }
      delete pProjectedColIter;
    }

    delete pDataSourceIter;
  }

  if (pProjectedAliasList->nItems() == 0) {
    // No aliased expressions. Could we match the columnRef against the table metadata?
    if (pDataSourceList->nItems() == 0) {
      delete pDataSourceList;
      delete pProjectedAliasList;

      if (bConsiderCorrelation) {
        // Is the ColumnRef a correlated column?
        b1 bIsCorrelated = bResolveAsCorrelatedColumn(pColumnRef, pEnclosingSubqueryNode,
                                                      sEnclosingSubqueryPath, pRootNode);

        if (bIsCorrelated) {
          return true;   // Keep walking, looking for more ColumnRefs
        }
      }

      if (bThrowWhenUnresolved) {
        NameValidationException::vThrowUnknownColumn(szCatalogName, szSchemaName, szTableName, szColumnName);
      }

      return false;
    }

    ASSERT(pDataSourceList->nItems() == 1);

    // Qualify the ColumnRef
    QueryNode* pDataSource = _pQueryNode(pDataSourceList->pPop());
    szTableAliasName = DataSource::szAliasName(pDataSource);
    szCatalogName    = DataSource::szCatalogName(pDataSource);
    szSchemaName     = DataSource::szSchemaName(pDataSource);
    szTableName      = DataSource::szTableName(pDataSource);
    szDefnPath       = nullptr;

    if (SubqueryRefOp::bIsOfType(pDataSource)) {
      QueryNode* pSubquery = SubqueryRefOp::pSubquery(pDataSource, pRootNode);
      ASSERT(pSubquery);
      ASSERT(TableSubquery::bIsOfType(pSubquery));
      szDefnPath = TableSubquery::szDefnPath(pSubquery);
    }

    ColumnRef::vQualify(pColumnRef, szTableAliasName, szCatalogName,
                        szSchemaName, szTableName, szColumnName, szDefnPath);

  } else if (pProjectedAliasList->nItems() == 1) {
    // We matched against an aliased (projected) expression

    if (pDataSourceList->nItems() == 1) {
      // pDataSourceList will be deleted in vThrowAmbiguousColumn() below
      // pProjectedColList will be deleted in vThrowAmbiguousColumn() below
      NameValidationException::vThrowAmbiguousColumn(szColumnName, pDataSourceList, pProjectedAliasList, pRootNode);
    }

    // We've got a single unambiguous aliased expression.
    ASSERT(pDataSourceList->nItems() == 0);
    QueryNode* pProjectedAlias = pProjectedAliasList->pPop();
    ASSERT(ProjectedColumn::bIsOfType(pProjectedAlias));
    QueryNode* pAliasedExpr = ProjectedColumn::pExprNode(pProjectedAlias);

    // It's possible that the aliased expression is not yet fully qualified.
    // This can occur when a subquery references a correlated alias name because
    // the subquery gets processed before the main subquery where the alias is defined.
    // So we may need to check.
    // This will walk the aliased expression and update it as necessary.
    vValidateColumns(pRootNode, pAliasedExpr, pEnclosingSubqueryNode, sEnclosingSubqueryPath, pMetadataNode, pSqlOp);

    // We need to change the ColumnRef to an AliasRef in this spot in the graph.
    // But we are also currently walking the graph and removing the node
    // would mess things up. So we need to transmogrify the node
    // in place instead.
    //
    // Create the AliasRef that we wish to transmogrify it to
    Text* pAliasName = new Text(ProjectedColumn::szAliasName(pProjectedAlias));

    QueryNode* pAliasRef = AliasRef::pCreate(pAliasName, pAliasedExpr->pDeepCopy(true));
    pColumnRef->vTransmogrifyTo(pAliasRef, true);
    delete pAliasRef;
    delete pProjectedAlias;

  } else {
    ASSERT(pProjectedAliasList->nItems() > 1);
    delete pDataSourceList;
    // pProjectedColList will be deleted in vThrowAmbiguousColumn() below
    NameValidationException::vThrowAmbiguousAlias(szColumnName, pProjectedAliasList, pRootNode);
  }

  delete pDataSourceList;
  delete pProjectedAliasList;

  return true;
}


/*----------------------------------------------------------------------------*/
/* Return a list of DataSource nodes that are reachable from a given sql op   */
/* and that contain a column as referenced by pColumnRef.                     */
/*----------------------------------------------------------------------------*/
QueryNodeList* NameValidator::pMatchingDataSourcesFor(QueryNode* pColumnRef,
                                                      QueryNode* pSqlOp,
                                                      PlanNode*  pRootNode,
                                                      QueryNode* pMetadataNode)
{
  // A DataSource may reference a real table, or it could represent a subquery
  // that presents as a table (e.g. a Common Table Expression (CTE)).
  // DataSources may also have an alias. If they do then that alias is masking
  // the name of the underlying real table (if applicable) and we can't match
  // against that.

  // List of matching DataSources to be returned
  QueryNodeList* pDataSourceList = new QueryNodeList();

  // Lambda called for each node under SqlOp, to look for TableRefs.
  auto fVisitor = [pColumnRef, pRootNode, pMetadataNode, &pDataSourceList]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (SetOp::bIsOfType(pNode)) {
      // We only walk down through the first child of a SetOp to resolve data sources.
      // Normally this could lead to issues whereby an operator above the SetOp could
      // refer to a column or alias that is not being projected. But at this point we
      // are only interested in resolving names. Later on (in the project pushdown stage)
      // we will check that each SQL operator has visibility of the expressions it is
      // trying to manipulate.

      // Recursively call the same function but only for the first child
      // of the SetOp.
      QueryNodeList* pSubList = pMatchingDataSourcesFor(pColumnRef,
                                                        _pQueryNode(pNode->pFirstChild()),
                                                        pRootNode,
                                                        pMetadataNode);

      // Copy the results into the parent list
      QueryNode* pItem;
      while ( (pItem = pSubList->pPop()) != nullptr) {
        pDataSourceList->vAppend(pItem);
      }

      delete pSubList;

      // We don't wish to descend this sub-tree because we manually descended
      // the first child above.
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    if (!DataSource::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pDataSource = _pQueryNode(pNode);

    // ColumnRef properties
    Stz* szColRefTableAliasName = ColumnRef::szTableAlias(pColumnRef);
    Stz* szColRefCatalogName    = ColumnRef::szCatalog(pColumnRef);
    Stz* szColRefSchemaName     = ColumnRef::szSchema(pColumnRef);
    Stz* szColRefTableName      = ColumnRef::szTable(pColumnRef);
    Stz* szColRefColumnName     = ColumnRef::szColumn(pColumnRef);

    if (DataSource::bResolvesColumnRef(pDataSource, pMetadataNode,
                                       pRootNode, szColRefTableAliasName,
                                       szColRefCatalogName, szColRefSchemaName,
                                       szColRefTableName, szColRefColumnName)) {
      // Add the data source to the list
      pDataSourceList->vAppend(pDataSource);
    }

    // Keep looking, even if we found a matching DataSource - it might be an
    // ambiguous column.
    return GraphNav::WALK_CONTINUE;
  };

    // Walk the graph from pSqlOp looking for DataSources
  GraphNav::bGlobalWalk(pSqlOp, false, true, fVisitor, pRootNode);

  return pDataSourceList;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumn nodes that are reachable from a given sql */
/* operator and that contain a given alias name. Need to free the list and    */
/* nodes returned.                                                            */
/*----------------------------------------------------------------------------*/
QueryNodeList* NameValidator::pMatchingExprAliasesFor(Stz* szAlias, QueryNode* pSqlOp, PlanNode* pRootNode)
{
  ASSERT(szAlias);
  ASSERT(pSqlOp);
  ASSERT(SqlOperator::bIsOfType(pSqlOp));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // List of matching ProjectedColumns to be returned
  QueryNodeList* pProjectedAliasList = new QueryNodeList();

  // Get the list of columns that are projected from pSqlOp and below.
  QueryNodeList* pProjectedCols = PlanObject::pColumnExprs(pSqlOp, pRootNode);

  QueryNode* pProjectedCol;

  while ((pProjectedCol = pProjectedCols->pPop()) != nullptr) {
    Stz* szColAlias = ProjectedColumn::szAliasName(pProjectedCol);

    if (szColAlias && szAlias->bSame(szColAlias)) {
      pProjectedAliasList->vAppend(pProjectedCol);

    } else {
      delete pProjectedCol;
    }
  }

  delete pProjectedCols;

  return pProjectedAliasList;
}

/*----------------------------------------------------------------------------*/
/* Try to resolve the ColumnRef/AliasRef to a parent subquery. If it is       */
/* correlated then the expression is qualified and the subqueries are marked  */
/* as correlated.                                                             */
/*----------------------------------------------------------------------------*/
b1 NameValidator::bResolveAsCorrelatedColumn(QueryNode* pColumnRef, QueryNode* pEnclosingSubqueryNode,
                                             s2K& sEnclosingSubqueryPath, PlanNode* pRootNode)
{
  ASSERT(pColumnRef);
  ASSERT(ColumnRef::bIsOfType(pColumnRef));
  ASSERT(pEnclosingSubqueryNode);
  ASSERT(MainSubquery::bIsOfType(pEnclosingSubqueryNode));
  ASSERT(sEnclosingSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // We need to:
  //  - Find the parent subquery that defines this ColumnRef/AliasRef and make a list of
  //    all subqueries between here and it.
  //  - Qualify pNode if it is a ColumnRef, or qualify the expression if it is an AliasRef.
  //  - Add a copy of the ColumnRef/AliasRef to each subquery between here and the defining subquery (exclusive).
  //  - Return true if the column was correlated successfully


  // Qualify the column reference or turn it into an alias reference.
  // Create an empty list of subqueries that are transitively correlated.
  // This will get populated below.
  QueryNodeList* pParentSubqueries = new QueryNodeList();

  try {
    b1 bQualified = bCorrelatedColumnSearch(pColumnRef,
                                            pEnclosingSubqueryNode,
                                            sEnclosingSubqueryPath,
                                            pRootNode,
                                            pParentSubqueries);

    if (!bQualified) {
      pParentSubqueries->vRemoveAll();
      delete pParentSubqueries;
      return false;
    }

  } catch (SqlException& e) {
    pParentSubqueries->vRemoveAll();
    delete pParentSubqueries;
    throw;
  }

  // The pColumnRef has either been qualified, or it has been transmogrified
  // into an AliasRef instead.
  ASSERT(ColumnRef::bIsOfType(pColumnRef) || AliasRef::bIsOfType(pColumnRef));

  // Add a copy of the ColumnRef/AliasRef to each subquery between here and the defining subquery.
  while (pParentSubqueries->pHead()) {
    QueryNode* pSubquery = pParentSubqueries->pPop();
    ASSERT(pSubquery);
    ASSERT(SubqueryMarker::bIsOfType(pSubquery));
    SubqueryMarker::vAddCorrelatedColumn(pSubquery, pColumnRef->pDeepCopy(true));
  }


  pParentSubqueries->vRemoveAll();
  delete pParentSubqueries;

  return true;
}

/*----------------------------------------------------------------------------*/
/* Search for the source of the correlated column and qualify it.             */
/* Populate a list of parent subqueries encountered along the way.            */
/*----------------------------------------------------------------------------*/
b1 NameValidator::bCorrelatedColumnSearch(QueryNode* pColumnRef,
                                          QueryNode* pEnclosingSubqueryNode,
                                          s2K& sEnclosingSubqueryPath,
                                          PlanNode *pRootNode,
                                          QueryNodeList* pParentSubqueries)
{
  ASSERT(pColumnRef);
  ASSERT(ColumnRef::bIsOfType(pColumnRef));
  ASSERT(pEnclosingSubqueryNode);
  ASSERT(MainSubquery::bIsOfType(pEnclosingSubqueryNode));
  ASSERT(sEnclosingSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(pParentSubqueries);


  // Bottom-up, walk the path from pEnclosingSubquery back to the root.
  // If we encounter a subquery, add it to the pParentSubqueries list.
  // At each node, check if any children are a MainSubquery. If so,
  // try to resolve and qualify the columnRef.

  QueryNode* pPrevNode = nullptr;

  auto fVisitor = [pColumnRef, pEnclosingSubqueryNode, pRootNode, pParentSubqueries, &pPrevNode]
                  (PlanNode *pNode, s2K &sPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void *pvArg)
                  -> GraphNav::eWalkCtrl {

      UNUSED(sPath);
      UNUSED(iDepth);
      UNUSED(eEvent);
      UNUSED(pvArg);

      // Record which subqueries we run into because they will become transitively correlated.
      if (SubqueryMarker::bIsOfType(pNode)) {
        pParentSubqueries->vAppend(_pQueryNode(pNode));
      }

      if (pNode == pEnclosingSubqueryNode) {
        // Ignore the enclosing subquery and visit the parent nodes.
        pPrevNode = _pQueryNode(pNode);
        return GraphNav::WALK_CONTINUE;
      }

      // Check if any siblings are a MainSubquery
      if (pNode->nChildren() == 1) {
        pPrevNode = _pQueryNode(pNode);
        return GraphNav::WALK_CONTINUE;
      }

      ASSERT(pNode->nChildren() > 1);
      PlanNodeIter* pChildIter = pNode->pNewChildrenIter();
      b1 bSeenPrevNode = false;
      while (pChildIter->bHasNext()) {
        QueryNode* pChild = _pQueryNode(pChildIter->pNext());

        // Skip over siblings so that we are "resolving-forward" from pPrevNode
        if (!bSeenPrevNode) {
          if (pChild == pPrevNode) {
            bSeenPrevNode = true;
          }

          continue;
        }

        if (!MainSubquery::bIsOfType(pChild)) {
          continue;
        }

        ASSERT(pChild != pPrevNode);

        // Does this MainSubquery resolve pColumnRef?
        // If so, it must have a SubqueryRefExpr that matches the same name as pEnclosingSubqueryNode.
        try {
          b1 bQualified = bQualifyCorrelatedColumn(pColumnRef, pChild, pPrevNode, pEnclosingSubqueryNode, pRootNode);

          if (bQualified) {
            // Stop walking. We can remove the last node we added to pParentSubqueries because
            // it is defining the correlated column.
            if (pParentSubqueries->pTail() == pNode) {
              pParentSubqueries->pPopTail();
            }
            delete pChildIter;
            return GraphNav::WALK_ABORT;
          }
        } catch (SqlException& e) {
          delete pChildIter;
          throw;
        }

        // This MainSubquery doesn't correlate the column but we still need to
        // add it to the list for transitive correlation.
        pParentSubqueries->vAppend(pChild);
      }
      delete pChildIter;

      pPrevNode = _pQueryNode(pNode);
      return GraphNav::WALK_CONTINUE;
  };

  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  b1 bResult = GraphNav::bWalkPath(pPlanRoot, sEnclosingSubqueryPath, nullptr, fVisitor, nullptr);

  // We abort the walk when we've resolved the correlation successfully
  if (!bResult) {
    ASSERT((ColumnRef::bIsOfType(pColumnRef) && ColumnRef::bIsFullyQualified(pColumnRef)) || AliasRef::bIsOfType(pColumnRef));
    return true;
  }

  // Not correlated to this subquery
  return false;
}


/*----------------------------------------------------------------------------*/
/* Tries to resolve and qualify pColumnRef to pMainSubquery.                  */
/* Returns true if the column was qualified at this subquery.                 */
/* Params:                                                                    */
/*   pColumnDef - the column we are trying to resolve                         */
/*   pMainSubquery - the subquery that might resolve pColumnDef               */
/*   pSiblingSubquery - the table/expr subquery that is a sibling of          */
/*                      pMainSubquery and is an ancestor of the subquery that */
/*                      contains pColumnDef.                                  */
/*   pCorrelatedSubquery - the immediate subquery parent of pColumnDef.       */
/*   pRootNode - root of the graph.                                           */
/*----------------------------------------------------------------------------*/
b1 NameValidator::bQualifyCorrelatedColumn(QueryNode* pColumnRef,
                                           QueryNode* pMainSubquery,
                                           QueryNode* pSiblingSubquery,
                                           QueryNode* pCorrelatedSubquery,
                                           PlanNode* pRootNode)
{
  ASSERT(pColumnRef);
  ASSERT(ColumnRef::bIsOfType(pColumnRef));
  ASSERT(pMainSubquery);
  ASSERT(MainSubquery::bIsOfType(pMainSubquery));
  ASSERT(pSiblingSubquery);
  ASSERT(SubqueryMarker::bIsOfType(pSiblingSubquery) && !MainSubquery::bIsOfType(pSiblingSubquery));
  ASSERT(pCorrelatedSubquery);
  ASSERT(MainSubquery::bIsOfType(pCorrelatedSubquery));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  UNUSED(pCorrelatedSubquery);

  // Walk this subquery and find the SubqueryRefExpr that matches pCorrelatedSubquery.
  // Then find it's SqlOp parent.
  // Then try to resolve pColumnRef.

  // If pSiblingSubquery is a TableSubquery then there is nothing for us to do
  // as pMainSubquery cannot resolve pColumnRef because it wouldn't make sense,
  // and it would be a forward reference from the table subquery.
  if (TableSubquery::bIsOfType(pSiblingSubquery)) {
    return false;
  }

  // If pSiblingSubquery is an ExprSubquery then we need to find the matching
  // SubqueryRefExpr in pMainSubquery, then find its parent SqlOp and then
  // try to resolve/qualify pColumnRef from there.

  // First find the SubqueryRefExpr underneath pMainSubquery.
  // Save its path relative to pMainSubquery.

  ASSERT(ExprSubquery::bIsOfType(pSiblingSubquery));
  QueryNode* pSubqueryRefExpr = nullptr;
  s2K sSubqueryRefExprPath;

  auto fSubqueryRefExprFinder = [&pSubqueryRefExpr, &sSubqueryRefExprPath, pSiblingSubquery]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg)
      -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (SubqueryRefExpr::bIsOfType(pNode) &&
        SubqueryRefExpr::szTableAlias(_pQueryNode(pNode))->bSame(ExprSubquery::szTableAlias(pSiblingSubquery))) {

      pSubqueryRefExpr = _pQueryNode(pNode);
      sSubqueryRefExprPath.vCopy(sPath.szStz());

      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pMainSubquery, true, true, fSubqueryRefExprFinder, nullptr);

  ASSERT(pSubqueryRefExpr);  // Cannot see why it couldn't be found.
  ASSERT(sSubqueryRefExprPath.nLength() > 0);


  // Now find the SqlOp that encloses pSubqueryRefExpr.

  s2K sSqlOpPath;
  sSqlOpPath.vCopyTill(sSubqueryRefExprPath, C_PROPERTY_SEPARATOR);

  QueryNode* pSqlOp = _pQueryNode(GraphNav::pFind(sSqlOpPath.szStz(), pMainSubquery));
  ASSERT(pSqlOp);
  ASSERT(SqlOperator::bIsOfType(pSqlOp));


  // Now we can try to resolve & qualify pColumnRef
  s2K sBuf;
  QueryNode* pMetadataNode = ShadowRoot::pMetadataNode(pRootNode);

  b1 bQualified = bValidateColumnRef(pColumnRef, pSqlOp, false, false, nullptr, sBuf, pRootNode, pMetadataNode);
  return bQualified;
}


/*----------------------------------------------------------------------------*/
/* Assert that all tables and columns are fully resolved and that there are   */
/* no columnWildcards left in the plan                                        */
/*----------------------------------------------------------------------------*/
void NameValidator::vAssertNamesResolved(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  // Walk every node (property and child). If we encounter a:
  //  - ColumnWildcard,
  //  - ColumnRef that is not fully resolved, or
  //  - TableRef that is not fully resolved
  // then assert an error.

  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    ASSERT(!ColumnWildcard::bIsOfType(pNode));

    if (ColumnRef::bIsOfType(pNode)) {
      ASSERT(ColumnRef::bIsFullyQualified(_pQueryNode(pNode)));
      return GraphNav::WALK_CONTINUE;
    }

    if (TableRef::bIsOfType(pNode)) {
      ASSERT(TableRef::bIsFullyQualified(_pQueryNode(pNode)));
      return GraphNav::WALK_CONTINUE;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pPlanNode, true, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Validate the column names of any join USING clauses                        */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateJoinUsingColumns(PlanNode *pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // Walk the graph looking for JoinOps
  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!JoinOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pJoinOp = _pQueryNode(pNode);
    QueryNode* pUsingList = JoinOp::pUsingListNode(pJoinOp);

    if (!pUsingList) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    ASSERT(StringList::bIsOfType(pUsingList));
    ASSERT(pUsingList->nChildren() > 0);
    ASSERT(pJoinOp->nChildren() == 2);

    // Replace the USING list with a join predicate in this join operator
    vReplaceJoinUsingColumn(pRootNode, pJoinOp, pUsingList);

    return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ColumnRefs
  };

  // Walk the /plan node, its children but not its properties.
  GraphNav::bBoundedWalkBottomUp(pStartNode, true, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Replace the USING list with a join predicate in a JoinOp                   */
/*----------------------------------------------------------------------------*/
void NameValidator::vReplaceJoinUsingColumn(PlanNode* pRootNode, QueryNode* pJoinOp, QueryNode* pUsingList)
{
  QueryNode* pLeftTable  = _pQueryNode(pJoinOp->pFirstChild());
  QueryNode* pRightTable = _pQueryNode(pJoinOp->pSecondChild());

  ASSERT(pLeftTable);
  ASSERT(pRightTable);

  // Get the projected columns of both the left and right tables.
  QueryNodeList* pLeftProjCols  = PlanObject::pColumnExprs(pLeftTable, pRootNode);
  QueryNodeList* pRightProjCols = PlanObject::pColumnExprs(pRightTable, pRootNode);

  // The join predicate we will form
  QueryNode* pJoinPredicate = nullptr;

  // Iterate the USING columns and see if we can find the column in the left and right
  // projected column lists
  PlanNodeIter* pUsingIter = pUsingList->pNewChildrenIter();
  while (pUsingIter->bHasNext()) {
    QueryNode* pUsingCol = _pQueryNode(pUsingIter->pNext());
    ASSERT(StringObject::bIsOfType(pUsingCol));

    Stz* szUsingCol = pUsingCol->szValue();
    ASSERT(szUsingCol);

    QueryNode* pLeftResolvedExpr  = nullptr;
    QueryNode* pRightResolvedExpr = nullptr;

    // Can we find this name in each projected column list?
    // Check the left then the right projected column lists.
    try {
      pLeftResolvedExpr = pResolveUsingColumn(szUsingCol, pLeftProjCols, pRootNode);

      if (pLeftResolvedExpr) {
        pRightResolvedExpr = pResolveUsingColumn(szUsingCol, pRightProjCols, pRootNode);
      }

    } catch (SqlException& e) {
      pLeftProjCols->vDeleteAll();
      pRightProjCols->vDeleteAll();
      delete pLeftProjCols;
      delete pRightProjCols;

      delete pLeftResolvedExpr;
      delete pRightResolvedExpr;
      delete pJoinPredicate;

      delete pUsingIter;
      throw;
    }

    if (!pLeftResolvedExpr || !pRightResolvedExpr) {
      pLeftProjCols->vDeleteAll();
      pRightProjCols->vDeleteAll();
      delete pLeftProjCols;
      delete pRightProjCols;

      delete pLeftResolvedExpr;
      delete pRightResolvedExpr;
      delete pJoinPredicate;

      delete pUsingIter;

      // throw exception
      SemanticException::vThrow(EC_USING_COLUMN_NOT_FOUND, ": %s"_Stz, szUsingCol);
      return; // For avoiding wrongful code-highlighting/analysis errors
    }

    // Form a join predicate
    QueryNode* pEqualityExpr = EqExpr::pCreate(pLeftResolvedExpr, pRightResolvedExpr);

    // Set it as the join predicate, or AND it with any existing join predicate
    if (pJoinPredicate) {
      pJoinPredicate = AndExpr::pCreate(pJoinPredicate, pEqualityExpr);

    } else {
      pJoinPredicate = pEqualityExpr;
    }
  }

  // Replace the USING list in the JoinOp with the join predicate
  JoinOp::vReplaceUsing(pJoinOp, pJoinPredicate);

  pLeftProjCols->vDeleteAll();
  pRightProjCols->vDeleteAll();
  delete pLeftProjCols;
  delete pRightProjCols;
  delete pUsingIter;
}

/*----------------------------------------------------------------------------*/
/* Tries to resolve a column name in a given projected column list.           */
/* Returns a ColumnRef/AliasRef that can be used to form a join predicate.    */
/* May throw an exception if the column name is ambiguous                     */
/*----------------------------------------------------------------------------*/
QueryNode* NameValidator::pResolveUsingColumn(Stz* szUsingCol, QueryNodeList* pProjColList, PlanNode* pRootNode)
{
  QueryNode* pResolvedExpr = nullptr;

  // Let's iterate the projected column list
  QueryNodeIter* pProjColIter = pProjColList->pNewIter();

  // Iterate each projected column from the current list
  while (pProjColIter->bHasNext()) {
    QueryNode* pProjectedColumn = pProjColIter->pNext();
    ASSERT(ProjectedColumn::bIsOfType(pProjectedColumn));

    // Get the expression of this projected column
    QueryNode* pProjExpr = ProjectedColumn::pExprNode(pProjectedColumn);
    ASSERT(Expr::bIsOfType(pProjExpr));

    // The alias of this projected column, if one exists
    Stz* szAliasName = ProjectedColumn::szAliasName(pProjExpr);

    if (ColumnRef::bIsOfType(pProjExpr) && ColumnRef::szColumn(pProjExpr)->bSame(szUsingCol)) {
      // We got a match.
      // First check if we got a previous match. i.e. an ambiguous column name
      if (pResolvedExpr) {
        s2K sBuf1;
        s2K sBuf2;
        Expr::vDescribe(pResolvedExpr, sBuf1, pRootNode);
        ColumnRef::vDescribe(pProjExpr, sBuf2, pRootNode);

        delete pResolvedExpr;
        delete pProjColIter;
        NameValidationException::vThrowAmbiguousUsingColumn(szUsingCol, sBuf1.szStz(), sBuf2.szStz());
        return nullptr;  // to avoid code analysis giving false warnings
      }

      // Save a copy of the ColumnRef to use in a join predicate
      pResolvedExpr = pProjExpr->pDeepCopy(true);
      // Keep checking for ambiguous columns

    } else if (szAliasName && szAliasName->bSame(szUsingCol)) {
      // We got a match on the projected column's alias
      // First check if we got a previous match. i.e. an ambiguous column name
      if (pResolvedExpr) {
        s2K sBuf1;
        s2K sBuf2;
        Expr::vDescribe(pResolvedExpr, sBuf1, pRootNode);
        ProjectedColumn::vDescribe(pProjectedColumn, sBuf2, pRootNode);

        delete pResolvedExpr;
        delete pProjColIter;
        NameValidationException::vThrowAmbiguousUsingColumn(szUsingCol, sBuf1.szStz(), sBuf2.szStz());
        return nullptr;  // to avoid code analysis giving false warnings
      }

      pResolvedExpr = AliasRef::pCreate(new Text(szAliasName), pProjExpr->pDeepCopy(true));
      // Keep checking for ambiguous columns
    }
  }

  delete pProjColIter;
  return pResolvedExpr;
}

/*----------------------------------------------------------------------------*/
/* Validate that all SubqueryRefExprs can find the corresponding ExprSubquery */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateSubqueryReferences(PlanNode* pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);
  ASSERT(StatementMarker::bIsOfType(pStartNode));

  // Walk the graph looking for SubqueryRefExpr
  auto fVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!SubqueryRefExpr::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pSubqueryRefExpr = _pQueryNode(pNode);

    QueryNode* pExprSubquery = SubqueryRefExpr::pSubquery(pSubqueryRefExpr, pRootNode);
    ASSERT(pExprSubquery);
    ASSERT(ExprSubquery::bIsOfType(pExprSubquery));
    UNUSED(pExprSubquery);

    return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ColumnRefs
  };

  // Walk the /plan node, its children and its properties.
  GraphNav::bBoundedWalk(pStartNode, true, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Try to coerce any projected expressions that have an invalid data type     */
/* (i.e. CT_NONE)                                                             */
/*----------------------------------------------------------------------------*/
void NameValidator::vCoerceProjectedExprs(PlanNode* pRootNode, QueryNode* pStartNode)
{
  ASSERT(pRootNode);
  ASSERT(pStartNode);

  // Walk the bounded graph from the bottom up looking for all ProjectOps and
  // their expressions. That way if we change an expression then we can also
  // check for the effects of that change as we walk upwards. A downwards walk
  // wouldn't allow us to be able to do that.
  auto fExprVisitor = [](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {
    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
    QueryNode* pExpr    = _pQueryNode(pNode);

    ASSERT(Expr::bIsOfType(pNode));
    if (Expr::eType(pExpr, pRootNode) == CT_NONE) {
      // We might need type coercion here
      Expr::vCoerceSubExprs(pExpr, pRootNode);

      // If we still haven't been able to correct the problem then abort any
      // further coercions. This error will be picked up in vCreateMainSubqueryCteDef().
      if (Expr::eType(pExpr, pRootNode) == CT_NONE) {
        return GraphNav::WALK_ABORT;
      }
    }

    return GraphNav::WALK_CONTINUE;
  };

  auto fOpVisitor = [fExprVisitor](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!ProjectOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pProjectOp = _pQueryNode(pNode);
    QueryNode* pProjectedCols = ProjectOp::pProjectedColumns(pProjectOp);
    ASSERT(ProjectedColumns::bIsOfType(pProjectedCols));

    PlanNodeIter* pIter = pProjectedCols->pNewChildrenIter();

    while (pIter->bHasNext()) {
      QueryNode* pProjectedCol = _pQueryNode(pIter->pNext());
      ASSERT(ProjectedColumn::bIsOfType(pProjectedCol));

      QueryNode* pProjectedExpr = ProjectedColumn::pExprNode(pProjectedCol);
      ASSERT(Expr::bIsOfType(pProjectedExpr));

      // We now need to walk this expression tree from the bottom up
      // and check for any necessary coercions.
      b1 bResult =
          GraphNav::bBoundedWalkBottomUp(pProjectedExpr,
                                         false,
                                         true,
                                         fExprVisitor,
                                         pRootNode);


      if (!bResult) {
        // If we still haven't been able to correct the problem then abort any
        // further coercions. This error will be picked up in vCreateMainSubqueryCteDef().
        ASSERT(Expr::eType(pProjectedExpr, pRootNode) == CT_NONE);
        delete pIter;
        return GraphNav::WALK_ABORT;
      }
    }

    delete pIter;

    // Keep walking up the graph
    return GraphNav::WALK_CONTINUE;
  };

  // Walk the node and its children
  GraphNav::bBoundedWalkBottomUp(pStartNode->pFirstChild(), false, true, fOpVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Given a subquery node&path, return a list of subqueries that are resolvable*/
/* The returned list contains pointers to SubqueryMarker nodes.               */
/* The returned list must be freed, but not its contents. List may be null.   */
/*----------------------------------------------------------------------------*/
QueryNodeList* NameValidator::pResolvableTableSubqueries(QueryNode* pMainSubquery, s2K& sMainSubqueryPath, PlanNode* pRootNode)
{
  ASSERT(pMainSubquery);
  ASSERT(MainSubquery::bIsOfType(pMainSubquery));
  ASSERT(sMainSubqueryPath.nLength() > 0);

  // A MainSubquery can resolve aliases belonging to another TableSubquery, TS1, iff:
  //  - the TableSubquery is a sibling of the MainSubquery (both have the same parent),
  //  - The parent of the MainSubquery (and any immediate parents of that) is itself
  //    a TableSubquery, TS2, and both TS1 and TS2 are both siblings, and TS1 is an
  //    earlier child than TS2.
  // The resolvable list of subqueries (for a non-recursive subquery) is found by
  // looking at the siblings of pMainSubquery to see if any of them are TableSubqueries.
  // Thereafter, if the parent is itself a TableSubquery then check all siblings of the
  // parent that occur before the parent.

  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  QueryNode* pLastNode = nullptr;

  QueryNodeList* pResolvableSubqueryList = new QueryNodeList();

  auto fVisitor = [pMainSubquery, &pLastNode, pResolvableSubqueryList]
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

    // Iterate the children of pNode up until the child equals pLastNode
    PlanNodeIter* pChildIter = pNode->pNewChildrenIter();
    while (pChildIter->bHasNext()) {
      QueryNode* pChild = _pQueryNode(pChildIter->pNext());

      if (pChild == pLastNode) {
        // We don't resolve past pLastNode otherwise it is a forward reference.
        // This is not allowed for a non-recursive subquery.
        break;
      }

      if (TableSubquery::bIsOfType(pChild)) {
        // It's resolvable
        pResolvableSubqueryList->vAppend(pChild);
      }
    }

    delete pChildIter;

    if (!SubqueryMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_ABORT;
    }

    pLastNode = _pQueryNode(pNode);
    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bWalkPath(pPlanRoot, sMainSubqueryPath, nullptr, fVisitor, nullptr);

  if (pResolvableSubqueryList->nItems() == 0) {
    delete pResolvableSubqueryList;
    pResolvableSubqueryList = nullptr;
  }

  return pResolvableSubqueryList;
}

/*----------------------------------------------------------------------------*/
/* Create a CteDef that represents a main subquery and add it to the metadata.*/
/* Also updates the defnPath property of the subquery node.                   */
/*----------------------------------------------------------------------------*/
void NameValidator::vCreateMainSubqueryCteDef(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                              PlanNode* pRootNode, QueryNode* pMetadataNode)
{
  ASSERT(pSubqueryNode);
  ASSERT(MainSubquery::bIsOfType(pSubqueryNode));
  ASSERT(sSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(pMetadataNode);
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));

  // The alias name of the CteDef is not important so let's just use the current
  // node name of the main subquery with a couple of underscores prepended to it
  s256 sBuf;
  sBuf.vCatSprint("__%s"_Stz, pSubqueryNode->szName());
  QueryNode* pCteDef = CteDef::pCreate(sBuf.szStz());

  // Populate pCteDef with column information
  // Get the list of projected columns of this main subquery
  QueryNodeList* pProjectedColumns = MainSubquery::pColumnExprs(pSubqueryNode, pRootNode);

  // Turn each projected column into a column definition
  QueryNodeIter* pIter = pProjectedColumns->pNewIter();
  i8 iOrdinal = -1;
  while (pIter->bHasNext()) {
    QueryNode* pProjectedCol = pIter->pNext();
    ASSERT(ProjectedColumn::bIsOfType(pProjectedCol));

    QueryNode* pColExpr = ProjectedColumn::pExprNode(pProjectedCol);
    ASSERT(pColExpr);
    ASSERT(Expr::bIsOfType(pColExpr));

    ++iOrdinal;
    s256 sColNameBuf;
    Stz* szColName = ProjectedColumn::szAliasName(pProjectedCol);
    if (!szColName) {
      // If the projected expr is a columnRef or aliasRef then let's use
      // that column name, otherwise we have to make one up.
      if (ColumnRef::bIsOfType(pColExpr)) {
        szColName = ColumnRef::szColumn(pColExpr);

      } else if (AliasRef::bIsOfType(pColExpr)) {
        szColName = AliasRef::szAlias(pColExpr);

      } else {
        sColNameBuf.vCatSprint("COL%lld"_Stz, iOrdinal + 1);
        szColName = sColNameBuf.szStz();
      }
    }

    eCT eType = Expr::eType(pColExpr, pRootNode);

    if (eType == CT_NONE) {
      s2K sBuf;
      Expr::vDescribe(pColExpr, sBuf, pRootNode);

      delete pCteDef;
      delete pIter;
      pProjectedColumns->vDeleteAll();
      delete pProjectedColumns;

      SemanticException::vThrow(EC_EXPR_INVALID_COMBINATION,
                                ": %s"_Stz,
                                sBuf.szStz() );
    }

    QueryNode* pTypeNode = SqlType::pCreate(eType);
    b1 bNotNull = false;
    i8 iPrecision = -1;
    i8 iScale = -1;

    // Create the column definition. For a main subquery there is no qualifying
    // catalog/schema/table name. Only a column name.
    QueryNode* pColumnDef = ColumnDef::pCreate(nullptr, nullptr, nullptr,
                                               szColName, pTypeNode, bNotNull,
                                               iOrdinal, iPrecision, iScale);
    CteDef::vAddColumnDef(pCteDef, pColumnDef);
  }

  delete pIter;
  pProjectedColumns->vDeleteAll();
  delete pProjectedColumns;
  pProjectedColumns = nullptr;

  // Get the CteDef node that represents the enclosing subquery's definition,
  // if one exists.
  QueryNode* pEnclosingSubqueryDefn = pFindEnclosingSubqueryDefn(sSubqueryPath, ShadowRoot::pPlanRoot(pRootNode), pRootNode);
  ASSERT(!pEnclosingSubqueryDefn || CteDef::bIsOfType(pEnclosingSubqueryDefn));

  // Add the CteDef of this main subquery to the metadata and get its full path string.
  // If there is an enclosing subquery then we add it as a child.
  if (pEnclosingSubqueryDefn) {
    pEnclosingSubqueryDefn->vAppendChild(pCteDef);

  } else {
    MetadataNode::vAddCte(pMetadataNode, pCteDef);
  }

  s2K sCteDefPath;
  MetadataNode::bGetCteDefPath(pMetadataNode, pCteDef, sCteDefPath);
  ASSERT(sCteDefPath.nLength() > 0);

  // Update the defnPath property of this main subquery node.
  SubqueryMarker::vSetDefnPath(pSubqueryNode, sCteDefPath.szStz());
}

/*----------------------------------------------------------------------------*/
/* Create initial CteDef representing a table subquery and add to the metadata*/
/* This CteDef can be updated with column information later on.               */
/* Also updates the defnPath property of the subquery node.                   */
/*----------------------------------------------------------------------------*/
void NameValidator::vCreateTableSubqueryCteDef(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                               PlanNode* pRootNode, QueryNode* pMetadataNode)
{
  ASSERT(pSubqueryNode);
  ASSERT(TableSubquery::bIsOfType(pSubqueryNode));
  ASSERT(sSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(pMetadataNode);
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));

  // Create the CteDef using the table alias
  QueryNode* pCteDef = CteDef::pCreate(TableSubquery::szTableAlias(pSubqueryNode));

  // We can't add column information yet because we haven't yet processed the
  // MainSubquery that exists underneath this pSubqueryNode.

  // Get the CteDef node that represents the enclosing subquery's definition,
  // if one exists.
  QueryNode* pEnclosingSubqueryDefn = pFindEnclosingSubqueryDefn(sSubqueryPath, ShadowRoot::pPlanRoot(pRootNode), pRootNode);
  ASSERT(!pEnclosingSubqueryDefn || CteDef::bIsOfType(pEnclosingSubqueryDefn));

  // Add the CteDef of this table subquery to the metadata and get its full path string.
  // If there is an enclosing subquery then we add it as a child of the enclosing subquery.
  if (pEnclosingSubqueryDefn) {
    pEnclosingSubqueryDefn->vAppendChild(pCteDef);

  } else {
    MetadataNode::vAddCte(pMetadataNode, pCteDef);
  }

  s2K sCteDefPath;
  MetadataNode::bGetCteDefPath(pMetadataNode, pCteDef, sCteDefPath);
  ASSERT(sCteDefPath.nLength() > 0);

  // Update the defnPath property of this main subquery node.
  TableSubquery::vSetDefnPath(pSubqueryNode, sCteDefPath.szStz());
}

/*----------------------------------------------------------------------------*/
/* Create initial CteDef representing an expr subquery and add to the metadata*/
/* This CteDef can be updated with column information later on.               */
/* Also updates the defnPath property of the subquery node.                   */
/*----------------------------------------------------------------------------*/
void NameValidator::vCreateExprSubqueryCteDef(QueryNode* pSubqueryExprNode, s2K& sSubqueryExprPath,
                                              PlanNode* pRootNode, QueryNode* pMetadataNode)
{
  ASSERT(pSubqueryExprNode);
  ASSERT(ExprSubquery::bIsOfType(pSubqueryExprNode));
  ASSERT(sSubqueryExprPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(pMetadataNode);
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));

  // Create the CteDef.
  // The alias name of the CteDef is not important so let's just use the current
  // node name of the main subquery with a couple of underscores prepended to it.
  s256 sBuf;
  sBuf.vCatSprint("__%s"_Stz, pSubqueryExprNode->szName());
  QueryNode* pCteDef = CteDef::pCreate(sBuf.szStz());

  // We can't add column information yet because we haven't yet processed the
  // MainSubquery that exists underneath this pSubqueryExprNode.

  // Get the CteDef* node that represents the enclosing subquery's definition,
  // if one exists.
  QueryNode* pEnclosingSubqueryDefn = pFindEnclosingSubqueryDefn(sSubqueryExprPath, ShadowRoot::pPlanRoot(pRootNode), pRootNode);
  if(pEnclosingSubqueryDefn) {
    ASSERT(CteDef::bIsOfType(pEnclosingSubqueryDefn));

    // Add the CteDef of this expression subquery as a child of the enclosing definition
    pEnclosingSubqueryDefn->vAppendChild(pCteDef);

  } else {
    // Add the CteDef into the root of the CTE metadata
    MetadataNode::vAddCte(pMetadataNode, pCteDef);
  }

  // Get the full path string of this CteDef node so we can save it in the SubqueryExpr node
  s2K sCteDefPath;
  MetadataNode::bGetCteDefPath(pMetadataNode, pCteDef, sCteDefPath);
  ASSERT(sCteDefPath.nLength() > 0);

  // Update the defnPath property of this expression subquery node.
  ExprSubquery::vSetDefnPath(pSubqueryExprNode, sCteDefPath.szStz());
}


/*----------------------------------------------------------------------------*/
/* Update a previously created CteDef representing an expression subquery and */
/* add in the missing column definition information.                          */
/*----------------------------------------------------------------------------*/
void NameValidator::vUpdateExprSubqueryCteDef(QueryNode* pSubqueryExprNode, s2K& sSubqueryExprPath,
                                              PlanNode* pRootNode, QueryNode* pMetadataNode)
{
  ASSERT(pSubqueryExprNode);
  ASSERT(ExprSubquery::bIsOfType(pSubqueryExprNode));
  ASSERT(sSubqueryExprPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));
  UNUSED(sSubqueryExprPath);
  UNUSED(pMetadataNode);


  // When we get here we have previously created the definition (CteDef) for this
  // expression subquery and inserted it into the metadata in the appropriate spot.
  // However we haven't filled in the column name list. We will do that here.

  // Get the path of the definition we previously created.
  Stz* szDefnPath = ExprSubquery::szDefnPath(pSubqueryExprNode);
  ASSERT(szDefnPath);
  ASSERT(szDefnPath->bBeginsWith("/"_Stz));

  // Get the node representing the definition.
  QueryNode* pCteDef = _pQueryNode(GraphNav::pFind(szDefnPath, pRootNode));
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  // The main subquery that is a child of this expression subquery should now have
  // all its names validated. So we can extract out the columns it projects
  // and insert this into the existing CteDef.

  // We can cheat a bit because when the main subquery was processed we would
  // have already produced the metadata for it, and the column information
  // remains the same. So all we need to do is find the CteDef for the child
  // main subquery and copy the column information from it.

  // We can't assume that there is only one child of pCteDef, so best that we
  // find the main subquery underneath pSubqueryExprNode, get its definition path
  // and then lookup its CteDef that way.

  auto fCondition = [](PlanNode* pNode) -> b1 { return MainSubquery::bIsOfType(pNode); };
  QueryNode* pChildMainSubquery = _pQueryNode(GraphNav::pFindChild(fCondition, pSubqueryExprNode));
  ASSERT(pChildMainSubquery);
  ASSERT(MainSubquery::bIsOfType(pChildMainSubquery));

  Stz* szMainSubqueryDefnPath = MainSubquery::szDefnPath(pChildMainSubquery);
  ASSERT(szMainSubqueryDefnPath);

  QueryNode* pMainSubqueryCteDef = _pQueryNode(GraphNav::pFind(szMainSubqueryDefnPath, pRootNode));
  ASSERT(pMainSubqueryCteDef);
  ASSERT(CteDef::bIsOfType(pMainSubqueryCteDef));


  // Copy each of the column definitions from the child mainSubquery.
  PlanNodeIter* pColDefIter = CteDef::pColumnDefIter(pMainSubqueryCteDef);

  while (pColDefIter->bHasNext()) {
    QueryNode* pColDef = _pQueryNode(pColDefIter->pNext());
    ASSERT(ColumnDef::bIsOfType(pColDef));

    // Duplicate this column definition
    QueryNode* pColDefDup = pColDef->pDeepCopy(true);

    // Put it in the table subquery's CteDef
    CteDef::vAddColumnDef(pCteDef, pColDefDup);
  }

  delete pColDefIter;
}

/*----------------------------------------------------------------------------*/
/* Given a node and a path find and return the first parent SubqueryMarker.   */
/*----------------------------------------------------------------------------*/
QueryNode* NameValidator::pFindEnclosingSubquery(s2K& sPathToSearch, PlanNode* pPathRoot, s2K* psEnclosingSubqueryPath)
{
  ASSERT(sPathToSearch.nLength() > 0);
  ASSERT(pPathRoot);

  QueryNode* pSubqueryMarker = nullptr;

  auto fVisitor = [&psEnclosingSubqueryPath, &pSubqueryMarker]
      (PlanNode* pNode, s2K& sCurrentPath, i8 iDepth, GraphNav::eWalkEvent eEvent, void* pvArg)
      -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);
    UNUSED(eEvent);
    UNUSED(pvArg);

    ASSERT(eEvent == GraphNav::EVENT_EXIT);

    if (SubqueryMarker::bIsOfType(pNode)) {
      pSubqueryMarker = _pQueryNode(pNode);

      if (psEnclosingSubqueryPath) {
        psEnclosingSubqueryPath->vCopy(sCurrentPath.szStz());
      }

      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bWalkPath(pPathRoot, sPathToSearch, nullptr, fVisitor, nullptr);

  return pSubqueryMarker;
}


/*----------------------------------------------------------------------------*/
/* Given a root node and a path to a subquery marker, try to find the parent  */
/* subquery marker and return that node, if one exists.                       */
/*----------------------------------------------------------------------------*/
QueryNode* NameValidator::pFindEnclosingSubqueryDefn(s2K& sStartingPath, PlanNode* pPathRoot, PlanNode* pRootNode)
{
  ASSERT(pPathRoot);
  ASSERT(ShadowPlan::bIsOfType(pPathRoot));
  ASSERT(sStartingPath.nLength() > 0);

  QueryNode* pEnclosingSubquery = pFindEnclosingSubquery(sStartingPath, pPathRoot);

  if (!pEnclosingSubquery) {
    return nullptr;
  }

  ASSERT(SubqueryMarker::bIsOfType(pEnclosingSubquery));

  Stz* szDefnPath = SubqueryMarker::szDefnPath(pEnclosingSubquery);
  if (!szDefnPath) {
    return nullptr;
  }

  QueryNode* pSubqueryDefnNode = _pQueryNode(GraphNav::pFind(szDefnPath, pRootNode));
  ASSERT(pSubqueryDefnNode);
  ASSERT(CteDef::bIsOfType(pSubqueryDefnNode));

  return pSubqueryDefnNode;
}

/*----------------------------------------------------------------------------*/
/* Checks that a table subquery has a unique name amongst its siblings.       */
/* Throws an exception if it is not unique.                                   */
/*----------------------------------------------------------------------------*/
void NameValidator::vCheckTableSubqueryIsUniquelyNamed(QueryNode* pTableSubquery, s2K& sSubqueryPath, PlanNode* pRootNode)
{
  ASSERT(pTableSubquery);
  ASSERT(TableSubquery::bIsOfType(pTableSubquery));
  ASSERT(sSubqueryPath.nLength() > 0);

  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanRoot);
  ASSERT(ShadowPlan::bIsOfType(pPlanRoot));

  Stz* szAlias = TableSubquery::szTableAlias(pTableSubquery);
  ASSERT(szAlias);

  // We need to check the value of szAlias against all sibling table subqueries.
  // For that we need the parent of pTableSubquery.
  QueryNode* pParent = _pQueryNode(GraphNav::pGetParentOfPath(sSubqueryPath, pPlanRoot));
  ASSERT(pParent);

  PlanNodeIter* pIter = pParent->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());

    if (pChild == pTableSubquery) {
      continue;
    }

    if (!TableSubquery::bIsOfType(pChild)) {
      continue;
    }

    Stz* szAlias2 = TableSubquery::szTableAlias(pChild);
    if (szAlias->bSame(szAlias2)) {
      delete pIter;
      SemanticException::vThrow(EC_AMBIGUOUS_WITH_QUERY, ": %s"_Stz, szAlias);
    }
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Checks if the given table name can be resolved against any table subquery  */
/* in the plan. If so, an error is thrown that an out-of-scope table alias    */
/* has been referenced.                                                       */
/*----------------------------------------------------------------------------*/
void NameValidator::vCheckNameAgainstSubqueryNotInScope(Stz* szCatalogName, Stz* szSchemaName,
                                                        Stz* szTableName, PlanNode* pRootNode,
                                                        QueryNode* pEnclosingSubquery)
{
  ASSERT(pRootNode);
  ASSERT(szTableName);
  ASSERT(pEnclosingSubquery);
  ASSERT(MainSubquery::bIsOfType(pEnclosingSubquery));

  if (szCatalogName || szSchemaName) {
    // Can't be trying to refer to a table subquery alias if these are set.
    return;
  }

  QueryNode* pPlanRoot = ShadowRoot::pPlanRoot(pRootNode);

  // Walk all child nodes in the graph and look for TableSubquery nodes.
  // Don't care if we visit valid table subqueries - at this point we've been
  // unable to validate this name, so anything it does match could be considered
  // as out of scope.

  auto fVisitor = [szTableName, pEnclosingSubquery](PlanNode* pNode, s2K& sCurrentPath, i8 iDepth, void* pvArg)
      -> GraphNav::eWalkCtrl {

    UNUSED(sCurrentPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!TableSubquery::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    Stz* szAlias = TableSubquery::szTableAlias(_pQueryNode(pNode));

    if (szTableName->bSame(szAlias)) {
      // Make sure the table reference isn't referring to the table subquery
      // it is a part of. Can't refer to itself if it is a non-recursive subquery.
      QueryNode* pMainSubquery = TableSubquery::pChildMainSubquery(_pQueryNode(pNode));

      if (pMainSubquery == pEnclosingSubquery && !TableSubquery::bIsRecursive(_pQueryNode(pNode))) {
        SemanticException::vThrow(EC_SUBQUERY_SELF_REFERENCE, ": %s"_Stz, szTableName);
      }

      SemanticException::vThrow(EC_TABLE_SUBQUERY_OUT_OF_SCOPE, ": %s"_Stz, szTableName);
      return GraphNav::WALK_ABORT;  // not required, except for compiler pickiness
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pPlanRoot, false, true, fVisitor, nullptr);
}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
