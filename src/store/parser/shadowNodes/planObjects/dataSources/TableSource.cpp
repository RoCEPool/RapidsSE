/*============================================================================*/
/* TableSource.hpp - an abstract data source that reads data from a table     */
/*============================================================================*/

#include "BASE.h"
#include "TableSource.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"
#include "store/parser/shadowNodes/compound/TableRef.hpp"
#include "ColumnRef.hpp"
#include "ColumnDef.hpp"
#include "ProjectedColumn.hpp"
#include "NodeNamer.hpp"
#include "TableDef.hpp"
#include "TableScanOp.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TABLEREF_PROP = "tableRef"_Stz;
static Stz* C_TABLEREF_DEFN = "/__root__/definitions/object/compound/tableRef"_Stz;

static Stz* C_TABLESOURCE_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource/tableSource"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* TableSource::pCreate(Stz* szDefn, QueryNode* pTableRef)
{
  ASSERT(szDefn);
  ASSERT(TableSource::bIsOfType(szDefn));
  ASSERT(pTableRef);
  ASSERT(TableRef::bIsExactType(pTableRef));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(szDefn, sName);

  // Create the object and add the tableRef property. We will need to rename it first.
  QueryNode* pTableSource = new QueryNode(sName, szDefn);

  pTableRef->vSetName(C_TABLEREF_PROP);
  pTableSource->vAppendProperty(pTableRef);

  return pTableSource;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TableSource::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism of subtypes here
  if        ( TableScanOp::bIsOfType(pNode)) {  TableScanOp::vDescribe(pNode, sBuf); }
//  else if ( IndexScanOp::bIsOfType(pNode)) {  IndexScanOp::vDescribe(pNode, sBuf); }
  else { NEVER_REACH }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void TableSource::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism of subtypes here
  if        ( TableScanOp::bIsOfType(pNode)) {  TableScanOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
//  else if ( IndexScanOp::bIsOfType(pNode)) {  IndexScanOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else { NEVER_REACH }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void TableSource::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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
    // Get the tableref property
    PlanNode* pTableRefNode = GraphNav::pFindProperty(const_cast<Stz*>(C_TABLEREF_PROP), pNode);
    ASSERT(pTableRefNode);
    ASSERT(TableRef::bIsOfType(pTableRefNode));

    // Print the table reference
    s2K sTblBuf;
    TableRef::vDescribe(pTableRefNode, sTblBuf);
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("table: (%s)"_Stz, sTblBuf.szStz());

    return;
  }

  // Manual polymorphism here
  if        (TableScanOp::bIsOfType(szType))   { TableScanOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  //else if (IndexScanOp::bIsOfType(szType))   { IndexScanOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);  }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 TableSource::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TABLESOURCE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 TableSource::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_TABLESOURCE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableSource::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TABLESOURCE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableSource::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_TABLESOURCE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the tableRef of the queryNode                                      */
/*----------------------------------------------------------------------------*/
QueryNode* TableSource::pTableRef(QueryNode* pNode)
{
  QueryNode *pTableRefNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_TABLEREF_PROP), pNode));
  QueryNode *pTableRef = static_cast<QueryNode *>(pTableRefNode);

  return pTableRef;
}

/*----------------------------------------------------------------------------*/
/* Returns the catalog name of the table reference                            */
/*----------------------------------------------------------------------------*/
Stz* TableSource::szCatalogName(QueryNode *pNode)
{
  QueryNode* pTableRef = TableSource::pTableRef(pNode);

  return TableRef::szCatalog(pTableRef);
}

/*----------------------------------------------------------------------------*/
/* Returns the schema name of the table reference                             */
/*----------------------------------------------------------------------------*/
Stz* TableSource::szSchemaName(QueryNode *pNode)
{
  QueryNode* pTableRef = TableSource::pTableRef(pNode);

  return TableRef::szSchema(pTableRef);
}

/*----------------------------------------------------------------------------*/
/* Returns the table name of the table reference                              */
/*----------------------------------------------------------------------------*/
Stz* TableSource::szTableName(QueryNode *pNode)
{
  QueryNode* pTableRef = TableSource::pTableRef(pNode);

  return TableRef::szTable(pTableRef);
}

/*----------------------------------------------------------------------------*/
/* Get the list of column definitions for this data source                    */
/* Returned list must be freed, but not its contents                          */
/*----------------------------------------------------------------------------*/
QueryNodeList* TableSource::pColumnDefs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  QueryNode* pMetadataNode = ShadowRoot::pMetadataNode(pRootNode);
  ASSERT(pMetadataNode);

  // Create a list for the columnDefs to be returned
  QueryNodeList* pColumnDefList = new QueryNodeList();

  // Iterate the columns in the metadata and add them to the above list
  PlanNodeIter* pIter = MetadataNode::pColumnDefIter(pMetadataNode,
                                                     szCatalogName(pNode),
                                                     szSchemaName(pNode),
                                                     szTableName(pNode));

  while (pIter->bHasNext()) {
    QueryNode* pColumnDef = _pQueryNode(pIter->pNext());
    pColumnDefList->vAppend(pColumnDef);
  }

  delete pIter;
  return pColumnDefList;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* TableSource::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  QueryNode* pMetadataNode = ShadowRoot::pMetadataNode(pRootNode);
  ASSERT(pMetadataNode);

  // Create a list for the columnRefs to be returned  (note: columnREFS not columnDEFS)
  QueryNodeList* pColumnRefList = new QueryNodeList();

  Stz* szDataSourceAliasName = szAliasName(pNode);

  // Iterate the columns in the metadata and add them to the above list
  PlanNodeIter* pIter = MetadataNode::pColumnDefIter(pMetadataNode,
                                                     szCatalogName(pNode),
                                                     szSchemaName(pNode),
                                                     szTableName(pNode));

  while (pIter->bHasNext()) {
    QueryNode* pColumnDef = _pQueryNode(pIter->pNext());

    // Create a columnRef expression object from this column definition
    QueryNode* pColumnRef = ColumnRef::pCreate(szDataSourceAliasName,
                                               ColumnDef::szCatalogName(pColumnDef),
                                               ColumnDef::szSchemaName(pColumnDef),
                                               ColumnDef::szTableName(pColumnDef),
                                               ColumnDef::szColumnName(pColumnDef));

    // Create a ProjectedColumn for this expression
    QueryNode* pProjectedColumn = ProjectedColumn::pCreate(pColumnRef,
                                                           new Text(ColumnDef::szColumnName(pColumnDef)));

    // Append it to the list
    pColumnRefList->vAppend(pProjectedColumn);
  }

  delete pIter;
  return pColumnRefList;
}

/*----------------------------------------------------------------------------*/
/* Returns true if this data source can resolve the given column reference    */
/*----------------------------------------------------------------------------*/
b1 TableSource::bResolvesColumnRef(QueryNode* pNode, QueryNode* pMetadataNode,
                                  Stz* szColRefTableAliasName,
                                  Stz* szCatalogName, Stz* szSchemaName,
                                  Stz* szTableName, Stz* szColumnName)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // First let's check if the data source's alias matches.
  // If we have an alias then we can't try to match against the underlying
  // table name as the alias masks that.
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

      QueryNode* pTableDef = MetadataNode::pTableDef(pMetadataNode,
                                                     TableSource::szCatalogName(pNode),
                                                     TableSource::szSchemaName(pNode),
                                                     TableSource::szTableName(pNode));
      ASSERT(pTableDef);

      // Test if the column belongs to this table definition
      QueryNode* pColumnDef = TableDef::pColumnDef(pTableDef, szColumnName);
      return (pColumnDef != nullptr);
    }

    return false;
  }

  // No alias in this data source, so we try to match against the cat/sch/table names.

  if ((szCatalogName == nullptr || szCatalogName->bSame(TableSource::szCatalogName(pNode))) &&
      (szSchemaName == nullptr  ||  szSchemaName->bSame(TableSource::szSchemaName(pNode))) &&
      (szTableName == nullptr   ||   szTableName->bSame(TableSource::szTableName(pNode))) ) {

    // This column partially or fully matches the name of this table. Get the
    // table definition of this table from the metadata
    QueryNode *pTableDef = MetadataNode::pTableDef(pMetadataNode,
                                                   TableSource::szCatalogName(pNode),
                                                   TableSource::szSchemaName(pNode),
                                                   TableSource::szTableName(pNode));
    ASSERT(pTableDef);

    // Test if the column belongs to this table definition
    QueryNode *pColumnDef = TableDef::pColumnDef(pTableDef, szColumnName);
    return (pColumnDef != nullptr);
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Prints into the buffer the identifier for the data source.                 */
/* If an alias is set then that is used. Otherwise it could be a table name.  */
/*----------------------------------------------------------------------------*/
void TableSource::vPrintName(QueryNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  Stz* szAlias = szAliasName(pNode);

  if (szAlias) {
    sBuf.vCopy(szAlias);
    return;
  }

  sBuf.vSprint("%s.%s.%s"_Stz,
               TableSource::szCatalogName(pNode),
               TableSource::szSchemaName(pNode),
               TableSource::szTableName(pNode) );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
