/*============================================================================*/
/* TableScanOp.hpp - an operator that linearly reads all rows in a table      */
/*============================================================================*/

#include "BASE.h"
#include "TableScanOp.hpp"

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

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TABLEREF_PROP = "tableRef"_Stz;
static Stz* C_TABLEREF_DEFN = "/__root__/definitions/object/compound/tableRef"_Stz;

static Stz* C_TABLESCAN_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource/tableScanOp"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* TableScanOp::pCreate(QueryNode* pTableRef)
{
  ASSERT(pTableRef);
  ASSERT(pTableRef->szType()->bEqual(C_TABLEREF_DEFN));

  // Create a unique name for this ProjectOp object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_TABLESCAN_DEFN), sName);

  // Create the object and add the tableRef property. We will need to rename it first.
  QueryNode* pTableScanOp = new QueryNode(sName, C_TABLESCAN_DEFN);

  pTableRef->vSetName(C_TABLEREF_PROP);
  pTableScanOp->vAppendProperty(pTableRef);

  return pTableScanOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TableScanOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pTableScanOp = static_cast<QueryNode*>(pNode);

  sBuf.vConcat("TableScanOp : "_Stz);

  QueryNode* pTableRef = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLEREF_PROP, pTableScanOp));
  ASSERT(pTableRef);

  TableRef::vDescribe(pTableRef, sBuf);
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void TableScanOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

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

  // Recurse any children
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, sBuf, iIndent + 1);
    }

    delete pIter;
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 TableScanOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TABLESCAN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableScanOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TABLESCAN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the tableRef of the queryNode                                      */
/*----------------------------------------------------------------------------*/
QueryNode* TableScanOp::pTableRef(QueryNode* pNode)
{
  QueryNode *pTableRefNode =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_TABLEREF_PROP), pNode));
  QueryNode *pTableRef = static_cast<QueryNode *>(pTableRefNode);

  return pTableRef;
}

/*----------------------------------------------------------------------------*/
/* Returns the catalog name of the table reference                            */
/*----------------------------------------------------------------------------*/
Stz* TableScanOp::szCatalogName(QueryNode *pNode)
{
  QueryNode* pTableRef = TableScanOp::pTableRef(pNode);

  return TableRef::szCatalog(pTableRef);
}

/*----------------------------------------------------------------------------*/
/* Returns the schema name of the table reference                             */
/*----------------------------------------------------------------------------*/
Stz* TableScanOp::szSchemaName(QueryNode *pNode)
{
  QueryNode* pTableRef = TableScanOp::pTableRef(pNode);

  return TableRef::szSchema(pTableRef);
}

/*----------------------------------------------------------------------------*/
/* Returns the table name of the table reference                              */
/*----------------------------------------------------------------------------*/
Stz* TableScanOp::szTableName(QueryNode *pNode)
{
  QueryNode* pTableRef = TableScanOp::pTableRef(pNode);

  return TableRef::szTable(pTableRef);
}

/*----------------------------------------------------------------------------*/
/* Get the list of column definitions for this data source                    */
/* Returned list must be freed, but not its contents                          */
/*----------------------------------------------------------------------------*/
PlanNodeList* TableScanOp::pColumnDefs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  QueryNode* pMetadataNode = ShadowRoot::pMetadataNode(pRootNode);
  ASSERT(pMetadataNode);

  // Create a list for the columnDefs to be returned
  PlanNodeList* pColumnDefList = new PlanNodeList();

  // Iterate the columns in the metadata and add them to the above list
  PlanNodeIter* pIter = MetadataNode::pColumnDefIter(pMetadataNode,
                                                     szCatalogName(pNode),
                                                     szSchemaName(pNode),
                                                     szTableName(pNode));

  while (pIter->bHasNext()) {
    QueryNode* pColumnDef = static_cast<QueryNode*>(pIter->pNext());
    pColumnDefList->vAppend(pColumnDef);
  }

  delete pIter;
  return pColumnDefList;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedExprs that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
PlanNodeList* TableScanOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  QueryNode* pMetadataNode = ShadowRoot::pMetadataNode(pRootNode);
  ASSERT(pMetadataNode);

  // Create a list for the columnRefs to be returned  (note: columnREFS not columnDEFS)
  PlanNodeList* pColumnRefList = new PlanNodeList();

  // Iterate the columns in the metadata and add them to the above list
  PlanNodeIter* pIter = MetadataNode::pColumnDefIter(pMetadataNode,
                                                     szCatalogName(pNode),
                                                     szSchemaName(pNode),
                                                     szTableName(pNode));

  while (pIter->bHasNext()) {
    QueryNode* pColumnDef = static_cast<QueryNode*>(pIter->pNext());

    // Create a columnRef expression object from this column definition
    QueryNode* pColumnRef = ColumnRef::pCreate(ColumnDef::szCatalogName(pColumnDef),
                                               ColumnDef::szSchemaName(pColumnDef),
                                               ColumnDef::szTableName(pColumnDef),
                                               ColumnDef::szColumnName(pColumnDef));

    // Create a ProjectedExpr for this expression
    QueryNode* pProjectedColumn = ProjectedColumn::pCreate(pColumnRef,
                                                           new Text(ColumnDef::szColumnName(pColumnDef)));

    // Append it to the list
    pColumnRefList->vAppend(pProjectedColumn);
  }

  delete pIter;
  return pColumnRefList;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
