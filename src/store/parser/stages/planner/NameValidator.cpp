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
#include "CteDef.hpp"
#include "CteDefs.hpp"
#include "TableScanOp.hpp"


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

  // TODO: Potential Problems:
  //       * A TableSubquery could appear underneath an exprSubquery
  //       * Could there be a correlated column inside a WITH clause???
  //       * Where does a metadata entry go for a TableSubquery underneath an ExprSubquery?


  try {
    vValidateTableAndMainSubqueries(pPlanNode, pRootNode, pMetadataNode, pSeApi);


    // Now we want to visit expression subqueries, validate them and
    // validate any child subqueries that they might contain.
    vValidateExprSubqueries(pPlanNode, pRootNode, pMetadataNode, pSeApi);

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
/* Validates the table subquery initially. We can't complete the validation   */
/* until the child MainSubquery underneath this is fully validated.           */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateTableAndMainSubqueries(QueryNode* pStartingNode, PlanNode* pRootNode,
                                                    QueryNode* pMetadataNode, STORE::SeApi* pSeApi)
{
  // The callback lambda for qualifying table and main subqueries
  auto fMainSubqueryVisitor = [pRootNode, pMetadataNode, pSeApi]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, GraphNav::eWalkEvent eWalkEvent, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!SubqueryMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pSubquery = _pQueryNode(pNode);

    // We should only hit table subqueries and main subqueries, and we should
    // hit table subqueries before we hit main subqueries. We process table
    // subqueries in two steps.

    if (TableSubquery::bIsOfType(pSubquery)) {
      switch (eWalkEvent) {
        case GraphNav::EVENT_ENTER:
          vValidateTableSubqueryEntry(pSubquery, sPath, pRootNode, pMetadataNode, pSeApi);
          break;

        case GraphNav::EVENT_EXIT:
          vValidateTableSubqueryExit(pSubquery, sPath, pRootNode, pMetadataNode, pSeApi);
          break;

        default:
          NEVER_REACH
      }

    } else if (MainSubquery::bIsOfType(pSubquery) && eWalkEvent == GraphNav::EVENT_ENTER) {
      // We ignore exit event for main subqueries as we will fully process the main
      // subquery in one go.
      vValidateMainSubquery(pSubquery, sPath, pRootNode, pMetadataNode, pSeApi);

    } else if (ExprSubquery::bIsOfType(pSubquery)) {
      // We should never reach this point because we are not walking properties of nodes.
      NEVER_REACH
    }

    return GraphNav::WALK_CONTINUE;
  };

  // We only want to walk children and not properties at this point as we're not
  // ready to visit exprSubqueries yet.
  GraphNav::bGlobalWalk(pStartingNode, false, true,
                        fMainSubqueryVisitor, fMainSubqueryVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Walk the graph and validate expr subqueries and any subqueries they contain*/
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateExprSubqueries(QueryNode* pStartingNode, PlanNode* pRootNode,
                                            QueryNode* pMetadataNode, STORE::SeApi* pSeApi)
{
  // Lambda for visiting expression subqueries.
  auto fExprSubqueryVisitor = [pRootNode, pMetadataNode, pSeApi]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!ExprSubquery::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    // Validate this expression subquery and any subqueries it contains
    QueryNode *pExprSubquery = _pQueryNode(pNode);
    vValidateExprSubquery(pExprSubquery, sPath, pRootNode, pMetadataNode, pSeApi);

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pStartingNode, true, true, fExprSubqueryVisitor, pRootNode);
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

  // Get a list of table subquery aliases (or nodes) that we could resolve to.
  QueryNodeList* pResolvableSubqueries = NameValidator::pResolvableSubqueries(pSubqueryNode, sSubqueryPath, pRootNode);

  // First we will validate all of the tables and either fully qualify them or turn
  // them into references to CTEs (as SubqueryRefOp instances).

  // Finally we can validate and qualify all of the columns. We do this last so that we
  // can correlate an unqualified column with a (now) qualified table definition. We can
  // also detect ambiguous column names too. But we need all tables and CTEs to be qualified first.

  // The callback lambda for qualifying tables
  auto fTableVisitor = [pSubqueryNode, pRootNode, pMetadataNode, pSeApi, pResolvableSubqueries]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (SubqueryMarker::bIsOfType(pNode) && pNode != pSubqueryNode) {
      // We're only interested in this subquery
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);
    vValidateTables(pRootNode, pStatementMarker, pSubqueryNode, pMetadataNode, pSeApi, pResolvableSubqueries);

    return GraphNav::WALK_CONTINUE;
  };

  // The callback lambda for qualifying columns
  auto fColumnVisitor = [pSubqueryNode, pRootNode, pMetadataNode, pSeApi]
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
    vValidateColumns(pRootNode, pStatementMarker, pSubqueryNode, pMetadataNode);

    // Validate the column names of any join USING clauses
    vValidateJoinUsingColumns(pRootNode, pStatementMarker);

    return GraphNav::WALK_CONTINUE;
  };


  // Validate the tables first, then the columns separately.
  // We use the enter/exit callback of the walk algorithm with the column visitor
  // as we need to constrain the walk to the current subquery (which we detect in
  // the enter event) but we validate/resolve the columns on a bottom-up approach
  // (using the exit event) so that we can qualify any columnRefs lower in the plan
  // before we hit any aliases higher up that then copy those expressions.
  try {
    GraphNav::bGlobalWalk(pSubqueryNode, false, true, fTableVisitor, pRootNode);
    GraphNav::bGlobalWalk(pSubqueryNode, false, true, fColumnVisitor, fColumnVisitor, pRootNode);

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
  }

  // Finally, create a CteDef for the main subquery and populate it with information.
  vCreateMainSubqueryCteDef(pSubqueryNode, sSubqueryPath, pRootNode, pMetadataNode);
}

/*----------------------------------------------------------------------------*/
/* Validates an expression subquery                                           */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateExprSubquery(QueryNode* pSubqueryNode, s2K& sSubqueryPath,
                                          PlanNode* pRootNode, QueryNode* pMetadataNode,
                                          STORE::SeApi* pSeApi)
{
  ASSERT(pSubqueryNode);
  ASSERT(ExprSubquery::bIsOfType(pSubqueryNode));
  ASSERT(sSubqueryPath.nLength() > 0);
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));
  ASSERT(MetadataNode::bIsOfType(pMetadataNode));
  ASSERT(pSeApi);

  // TODO: When visiting an ExprSubquery we need to:
  // TODO: * Ensure all outer subqueries are fully validated, including column names.
  // TODO:   (this is important for correlated columns)

  // TODO: * Create a CteDef for this node and insert it into the metadata.
  // TODO: * Populate the cteDefn property of this node and link the to above CteDef.

  // TODO: * Process child table/main subqueries to:
  // TODO:    - Validate/qualify table names
  // TODO:    - Validate/qualify column names
  vValidateTableAndMainSubqueries(pSubqueryNode, pRootNode, pMetadataNode, pSeApi);

  // TODO: * Populate the remaining CteDef fields (can copy column info from the CteDef of the MainSubquery child).
  // TODO: * Create a list of any correlated column names and populate this in the current exprSubquery node.

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
  ASSERT(!pResolvableSubqueries || pResolvableSubqueries->nItems() > 0);

  // A list of nodes to be replaced and their replacements. Can only do this after
  // the walk has finished. The node to be replaced comes first, then the node
  // that is replacing it comes second.
  QueryNodeList* pOpReplacementList = new QueryNodeList();

  // Walk the graph looking for TableRefs
  auto fVisitor = [&pMetadataNode, &pSeApi, pResolvableSubqueries, pStartNode, &pOpReplacementList]
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
    // TODO: Check if there are any table subqueries that are out of scope that could
    //       be a match. Then we can generate a better error message. If not, then
    //       simply issue the unknown table error.
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
                                     QueryNode* pEnclosingSubqueryNode, QueryNode* pMetadataNode)
{
  ASSERT(pRootNode);
  ASSERT(pEnclosingSubqueryNode);
  ASSERT(pMetadataNode);
  ASSERT(pStartNode);

  // Walk the graph looking for ColumnRefs
  auto fVisitor = [&pMetadataNode, pStartNode](PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!ColumnRef::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pColumnRef = _pQueryNode(pNode);

    Stz* szTableAliasName = ColumnRef::szTableAlias(_pQueryNode(pNode));
    Stz* szCatalogName    = ColumnRef::szCatalog(_pQueryNode(pNode));
    Stz* szSchemaName     = ColumnRef::szSchema(_pQueryNode(pNode));
    Stz* szTableName      = ColumnRef::szTable(_pQueryNode(pNode));
    Stz* szColumnName     = ColumnRef::szColumn(static_cast<QueryNode *>(pNode));

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
    s256 sSqlOpPath;
    sSqlOpPath.vCopyTill(sPath, ':');

    QueryNode* pSqlOp = _pQueryNode(GraphNav::pFind(sSqlOpPath.szStz(), pStartNode));
    ASSERT(pSqlOp);
    ASSERT(SqlOperator::bIsOfType(pSqlOp));

    QueryNodeList* pDataSourceList = pMatchingDataSourcesFor(pColumnRef, pSqlOp, pRootNode, pMetadataNode);

    // We can check for an ambiguous column name
    if (pDataSourceList->nItems() > 1) {
      // pDataSourceList will be deleted in vThrowAmbiguousColumn() below
      NameValidationException::vThrowAmbiguousColumn(szCatalogName, szSchemaName, szTableName, szColumnName, pDataSourceList);
    }

    if (szCatalogName || szSchemaName || szTableName) {
      // It must be a columnRef. Can't be an aliasRef

      if (pDataSourceList->nItems() == 0) {
        delete pDataSourceList;
        NameValidationException::vThrowUnknownColumn(szCatalogName, szSchemaName, szTableName, szColumnName);
      }

      ASSERT(pDataSourceList->nItems() == 1);

      // Qualify the ColumnRef
      QueryNode* pDataSource = _pQueryNode(pDataSourceList->pPop());
      szTableAliasName = DataSource::szAliasName(pDataSource);
      szCatalogName    = DataSource::szCatalogName(pDataSource);
      szSchemaName     = DataSource::szSchemaName(pDataSource);
      szTableName      = DataSource::szTableName(pDataSource);

      ColumnRef::vQualify(_pQueryNode(pNode), szTableAliasName, szCatalogName,
                          szSchemaName, szTableName, szColumnName);

      delete pDataSourceList;
      return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ColumnRefs
    }

    // We've potentially got a list of matching table defs above, but we still need to check
    // for aliased expressions because there might be an ambiguity in alias/column names.
    QueryNodeList* pProjectedColList = pMatchingExprAliasesFor(szColumnName, pSqlOp, pRootNode);

    // A TableRef produces a projected column for each table with an alias set
    // to that table name. So we can end up with both a TableDef and an projected
    // column alias that both refer to the same table.
    if (pProjectedColList->nItems() >0 && pDataSourceList->nItems() > 0) {

      QueryNodeIter* pDataSourceIter = pDataSourceList->pNewIter();
      while (pDataSourceIter->bHasNext()) {

        QueryNode* pDataSource = pDataSourceIter->pNext();
        ASSERT(DataSource::bIsOfType(pDataSource));

        QueryNodeIter* pProjectedColIter = pProjectedColList->pNewIter();
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

    if (pProjectedColList->nItems() == 0) {
      // No aliased expressions. Could we match the columnRef against the table metadata?
      if (pDataSourceList->nItems() == 0) {
        delete pDataSourceList;
        delete pProjectedColList;
        NameValidationException::vThrowUnknownColumn(szCatalogName, szSchemaName, szTableName, szColumnName);
      }

      ASSERT(pDataSourceList->nItems() == 1);

      // Qualify the ColumnRef
      QueryNode* pDataSource = _pQueryNode(pDataSourceList->pPop());
      szTableAliasName = DataSource::szAliasName(pDataSource);
      szCatalogName    = DataSource::szCatalogName(pDataSource);
      szSchemaName     = DataSource::szSchemaName(pDataSource);
      szTableName      = DataSource::szTableName(pDataSource);

      ColumnRef::vQualify(_pQueryNode(pNode), szTableAliasName, szCatalogName,
                          szSchemaName, szTableName, szColumnName);

    } else if (pProjectedColList->nItems() == 1) {
      // We matched against an aliased (projected) expression

      if (pDataSourceList->nItems() == 1) {
        // pDataSourceList will be deleted in vThrowAmbiguousColumn() below
        // pProjectedColList will be deleted in vThrowAmbiguousColumn() below
        NameValidationException::vThrowAmbiguousColumn(szColumnName, pDataSourceList, pProjectedColList);
      }

      ASSERT(pDataSourceList->nItems() == 0);

      // We've got a single unambiguous aliased expression.
      // We need to change the ColumnRef to an AliasRef in this spot in the graph.
      // But we are also currently walking the graph and removing the node
      // would mess things up. So we need to transmogrify the node
      // in place instead.
      //
      // Create the AliasRef that we wish to transmogrify it to
      QueryNode* pProjectedCol = pProjectedColList->pPop();
      Text* pAliasName = new Text(ProjectedColumn::szAliasName(pProjectedCol));
      QueryNode* pAliasExpr = ProjectedColumn::pExprNode(pProjectedCol)->pDeepCopy(true);

      QueryNode* pAliasRef = AliasRef::pCreate(pAliasName, pAliasExpr);
      pColumnRef->vTransmogrifyTo(pAliasRef, true);
      delete pAliasRef;
      delete pProjectedCol;

    } else {
      ASSERT(pProjectedColList->nItems() > 1);
      delete pDataSourceList;
      // pProjectedColList will be deleted in vThrowAmbiguousColumn() below
      NameValidationException::vThrowAmbiguousAlias(szColumnName, pProjectedColList);
    }

    delete pDataSourceList;
    delete pProjectedColList;
    return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ColumnRefs
  };

  // Walk the /plan node, it's properties and its children.
  // We do a bottom up walk because there may be aliases higher up the plan
  // that refer to exprs lower down and we want to qualify columnRefs in those
  // lower expressions before we hit the aliases higher up.
  GraphNav::bBoundedWalkBottomUp(pStartNode, true, true, fVisitor, pRootNode);
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
  QueryNodeList* pProjectedColumnList = new QueryNodeList();

  // Get the list of columns that are projected from pSqlOp and below.
  QueryNodeList* pProjectedCols = PlanObject::pColumnExprs(pSqlOp, pRootNode);

  QueryNode* pProjectedCol;

  while ((pProjectedCol = pProjectedCols->pPop()) != nullptr) {
    Stz* szColAlias = ProjectedColumn::szAliasName(pProjectedCol);

    if (szColAlias && szAlias->bSame(szColAlias)) {
      pProjectedColumnList->vAppend(pProjectedCol);

    } else {
      delete pProjectedCol;
    }
  }

  delete pProjectedCols;

  return pProjectedColumnList;
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
      pLeftResolvedExpr = pResolveUsingColumn(szUsingCol, pLeftProjCols);

      if (pLeftResolvedExpr) {
        pRightResolvedExpr = pResolveUsingColumn(szUsingCol, pRightProjCols);
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
QueryNode* NameValidator::pResolveUsingColumn(Stz* szUsingCol, QueryNodeList* pProjColList)
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
        Expr::vDescribe(pResolvedExpr, sBuf1);
        ColumnRef::vDescribe(pProjExpr, sBuf2);

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
        Expr::vDescribe(pResolvedExpr, sBuf1);
        ProjectedColumn::vDescribe(pProjectedColumn, sBuf2);

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
/* Given a subquery node&path, return a list of subqueries that are resolvable*/
/* The returned list contains pointers to SubqueryMarker nodes.               */
/* The returned list must be freed, but not its contents. List may be null.   */
/*----------------------------------------------------------------------------*/
QueryNodeList* NameValidator::pResolvableSubqueries(QueryNode* pSubqueryNode, s2K& sSubqueryPath, PlanNode* pRootNode)
{
  ASSERT(pSubqueryNode);
  ASSERT(SubqueryMarker::bIsOfType(pSubqueryNode));
  ASSERT(sSubqueryPath.nLength() > 0);

  // The resolvable list of subqueries (for a non-recursive subquery) is found by
  // looking at the sibling subqueries that occur before the current subquery.
  // So order is important.
  // We look at siblings by getting the parent and then walking the children.

  // Get the parent node of the subquery
  PlanNode* pParentNode = GraphNav::pGetParentOfPath(sSubqueryPath.szStz(), ShadowRoot::pPlanRoot(pRootNode));

  if (!pParentNode) {
    return nullptr;
  }

  QueryNodeList* pNodeList = nullptr;

  PlanNodeIter* pChildIter = pParentNode->pNewChildrenIter();
  while (pChildIter->bHasNext()) {
    PlanNode* pChild = pChildIter->pNext();

    if (pChild == pSubqueryNode) {
      // Got the current subquery. Not interested in anything after this.
      break;
    }

    if (!TableSubquery::bIsOfType(pChild)) {
      // This should probably not occur.
      ASSERT(0);
      continue;
    }

    if (!pNodeList) {
      pNodeList = new QueryNodeList();
    }

    pNodeList->vAppend(_pQueryNode(pChild));
  }

  delete pChildIter;
  return pNodeList;
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

    QueryNode* pTypeNode = SqlType::pCreate(Expr::eType(pColExpr, pRootNode));
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
  QueryNode* pEnclosingSubqueryDefn = pFindEnclosingSubqueryDefn(ShadowRoot::pPlanRoot(pRootNode), sSubqueryPath, pRootNode);
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
  QueryNode* pEnclosingSubqueryDefn = pFindEnclosingSubqueryDefn(ShadowRoot::pPlanRoot(pRootNode), sSubqueryPath, pRootNode);
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
/* Given a root node and a path to a subquery marker, try to find the parent  */
/* subquery marker and return that node, if one exists.                       */
/*----------------------------------------------------------------------------*/
QueryNode* NameValidator::pFindEnclosingSubqueryDefn(PlanNode* pPlanRoot, s2K& sSubqueryPath, PlanNode* pRootNode)
{
  ASSERT(pPlanRoot);
  ASSERT(ShadowPlan::bIsOfType(pPlanRoot));
  ASSERT(sSubqueryPath.nLength() > 0);

  // Walk up the parent tree
  s2K sParentPath;
  sParentPath.vCopy(sSubqueryPath.szStz());

  while (GraphNav::bParentPath(sParentPath)) {
    // Get the node for this path
    PlanNode* pNode = GraphNav::pFind(sParentPath.szStz(), pPlanRoot);
    ASSERT(pNode);

    if (!SubqueryMarker::bIsOfType(pNode)) {
      continue;
    }

    // Extract the path of this enclosing subquery's definition (if it has one)
    // and lookup the actual node at this path.
    QueryNode* pEnclosingSubquery = _pQueryNode(pNode);

    Stz* szDefnPath = SubqueryMarker::szDefnPath(pEnclosingSubquery);
    if (!szDefnPath) {
      return nullptr;
    }

    QueryNode* pSubqueryDefnNode = _pQueryNode(GraphNav::pFind(szDefnPath, pRootNode));
    ASSERT(pSubqueryDefnNode);
    ASSERT(CteDef::bIsOfType(pSubqueryDefnNode));

    return pSubqueryDefnNode;
  }

  // Not found
  return nullptr;
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

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
