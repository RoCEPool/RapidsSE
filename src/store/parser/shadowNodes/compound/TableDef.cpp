/*============================================================================*/
/* TableDef.hpp - a definition of a table used in the plan                    */
/*============================================================================*/

#include "BASE.h"
#include "TableDef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "ColumnDefs.hpp"
#include "ColumnDef.hpp"
#include "IndexDefs.hpp"
#include "IndexDefn.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TABLEDEF_DEFN   = "/__root__/definitions/object/compound/tableDef"_Stz;
static Stz* C_TABLEDEF_NAME   = "tableDef"_Stz;

static Stz* C_CATALOGNAME_PROP  = "catalogName"_Stz;
static Stz* C_CATALOGNAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_SCHEMANAME_PROP  = "schemaName"_Stz;
static Stz* C_SCHEMANAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_TABLENAME_PROP  = "tableName"_Stz;
static Stz* C_TABLENAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_COLUMNS_PROP  = "columns"_Stz;
static Stz* C_COLUMNS_DEFN  = "/__root__/definitions/object/list/columnDefs"_Stz;

static Stz* C_INDEXES_PROP  = "indexes"_Stz;
static Stz* C_INDEXES_DEFN  = "/__root__/definitions/object/list/indexDefs"_Stz;

static Stz* C_PARTITIONING_PROP  = "partitioning"_Stz;
static Stz* C_PARTITIONING_DEFN  = "/__root__/definitions/object/list/columnDefs"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Helper function to create tableDef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* TableDef::pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                             QueryNode* pColumnDefs, QueryNode* pIndexDefs, QueryNode* pPartitionDef)
{
  _static_

  ASSERT(szCatalogName);
  ASSERT(szCatalogName->nLength() > 0);
  ASSERT(szSchemaName);
  ASSERT(szSchemaName->nLength() > 0);
  ASSERT(szTableName);
  ASSERT(szTableName->nLength() > 0);
  ASSERT(pColumnDefs);
  ASSERT(ColumnDefs::bIsOfType(pColumnDefs));
  ASSERT(pColumnDefs->nChildren() > 0);
  ASSERT(pIndexDefs);
  ASSERT(IndexDefs::bIsOfType(pIndexDefs));
  ASSERT(pPartitionDef);
  ASSERT(ColumnDefs::bIsOfType(pPartitionDef));


  // Create the tableDef object
  QueryNode* pTableDef = new QueryNode(szTableName, C_TABLEDEF_DEFN);

  // Create the catalogName property
  Value xCatalogNameVal(szCatalogName);
  QueryNode* pCatalogName = new QueryNode(C_CATALOGNAME_PROP, C_CATALOGNAME_DEFN, static_cast<Value&&>(xCatalogNameVal));
  pTableDef->vAppendProperty(pCatalogName);

  // Create the schemaName property
  Value xSchemaNameVal(szSchemaName);
  QueryNode* pSchemaName = new QueryNode(C_SCHEMANAME_PROP, C_SCHEMANAME_DEFN, static_cast<Value&&>(xSchemaNameVal));
  pTableDef->vAppendProperty(pSchemaName);

  // Create the tableName property
  Value xTableNameVal(szTableName);
  QueryNode* pTableName = new QueryNode(C_TABLENAME_PROP, C_TABLENAME_DEFN, static_cast<Value&&>(xTableNameVal));
  pTableDef->vAppendProperty(pTableName);

  // Create the columns property
  pColumnDefs->vSetName(C_COLUMNS_PROP);
  pTableDef->vAppendProperty(pColumnDefs);

  // Create the indexes property
  pIndexDefs->vSetName(C_INDEXES_PROP);
  pTableDef->vAppendProperty(pIndexDefs);

  // Create the partitioning property
  pPartitionDef->vSetName(C_PARTITIONING_PROP);
  pTableDef->vAppendProperty(pPartitionDef);

  return pTableDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void TableDef::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pTableDef = _pQueryNode(pNode);

  QueryNode* pCatalogName = _pQueryNode(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pTableDef));
  QueryNode* pSchemaName  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMANAME_PROP, pTableDef));
  QueryNode* pTableName   = _pQueryNode(GraphNav::pFindProperty(C_TABLENAME_PROP, pTableDef));
  QueryNode* pColumns     = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pTableDef));
  QueryNode* pIndexes     = _pQueryNode(GraphNav::pFindProperty(C_INDEXES_PROP, pTableDef));
  UNUSED(pIndexes);
  
  ASSERT(pCatalogName);
  ASSERT(pSchemaName);
  ASSERT(pTableName);
  ASSERT(pTableName->szValue());
  ASSERT(pColumns);
  ASSERT(pColumns->nChildren() > 0);
  ASSERT(pIndexes);

 
  
  sBuf.vConcat("TABLE "_Stz);
  if (pCatalogName->szValue()) {
    sBuf.vConcat(pCatalogName->szValue());
    sBuf.vConcat("."_Stz);
  }

  if (pSchemaName->szValue()) {
    sBuf.vConcat(pSchemaName->szValue());
    sBuf.vConcat("."_Stz);
  }

  sBuf.vConcat(pTableName->szValue());

  s2K sColumnDesc;
  ColumnDefs::vDescribe(pColumns, sColumnDesc, pRootNode);
  sBuf.vCatSprint("(%s)"_Stz, sColumnDesc.szStz());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the tableDef definition     */
/*----------------------------------------------------------------------------*/
b1 TableDef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TABLEDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableDef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TABLEDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the catalog name from tableDef                                         */
/*----------------------------------------------------------------------------*/
Stz* TableDef::szCatalogName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogNameNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pNode));
  ASSERT(pCatalogNameNode);

  Stz* szCatalogName = pCatalogNameNode->szValue();

  return szCatalogName;
}

/*----------------------------------------------------------------------------*/
/* get the schema name from tableDef                                          */
/*----------------------------------------------------------------------------*/
Stz* TableDef::szSchemaName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pSchemaNameNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMANAME_PROP, pNode));
  ASSERT(pSchemaNameNode);

  Stz* szSchemaName = pSchemaNameNode->szValue();

  return szSchemaName;
}

/*----------------------------------------------------------------------------*/
/* get the table name from tableDef                                           */
/*----------------------------------------------------------------------------*/
Stz* TableDef::szTableName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pTableNameNode = _pQueryNode(GraphNav::pFindProperty(C_TABLENAME_PROP, pNode));
  ASSERT(pTableNameNode);

  Stz* szTableName = pTableNameNode->szValue();

  return szTableName;
}

/*----------------------------------------------------------------------------*/
/* Returns a QueryNode that represents a given ColumnDef in this table        */
/*----------------------------------------------------------------------------*/
QueryNode* TableDef::pColumnDef(QueryNode* pNode, Stz* szColumnName)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(szColumnName);

  QueryNode* pColumnDef = nullptr;

  // Get the columns property
  QueryNode* pColumnsNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));

  if (!pColumnsNode) {
    return pColumnDef;
  }

  PlanNodeIter* pIter = pColumnsNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(ColumnDef::bIsOfType(pChild));

    if (ColumnDef::szColumnName(pChild)->bSame(szColumnName)) {
      pColumnDef = pChild;
      break;
    }
  }

  delete pIter;
  return pColumnDef;
}

/*----------------------------------------------------------------------------*/
/* get an iterator of PlanNodes that represent ColumnDefs                     */
/*----------------------------------------------------------------------------*/
PlanNodeIter* TableDef::pColumnDefIter(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnsNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));
  ASSERT(pColumnsNode);

  return pColumnsNode->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Returns a QueryNode that represents a given IndexDef in this table         */
/*----------------------------------------------------------------------------*/
QueryNode* TableDef::pIndexDef(QueryNode* pNode, Stz* szIndexName)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(szIndexName);

  QueryNode* pIndexDef = nullptr;

  // Get the indexs property
  QueryNode* pIndexsNode = _pQueryNode(GraphNav::pFindProperty(C_INDEXES_PROP, pNode));

  if (!pIndexsNode) {
    return pIndexDef;
  }

  PlanNodeIter* pIter = pIndexsNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(IndexDefn::bIsOfType(pChild));

    if (IndexDefn::szIndexName(pChild)->bSame(szIndexName)) {
      pIndexDef = pChild;
      break;
    }
  }

  delete pIter;
  return pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* get an iterator of PlanNodes that represent IndexDefs                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* TableDef::pIndexDefIter(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pIndexesNode = _pQueryNode(GraphNav::pFindProperty(C_INDEXES_PROP, pNode));
  ASSERT(pIndexesNode);

  return pIndexesNode->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Returns a QueryNode that represents the partition information of this table*/
/*----------------------------------------------------------------------------*/
QueryNode* TableDef::pPartitionDef(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  // Get the indexs property
  QueryNode* pPartitioningNode = _pQueryNode(GraphNav::pFindProperty(C_PARTITIONING_PROP, pNode));
  ASSERT(pPartitioningNode);

  return pPartitioningNode;
}

/*----------------------------------------------------------------------------*/
/* get an iterator of PlanNodes that represent the partitioning ColumnDefs    */
/*----------------------------------------------------------------------------*/
PlanNodeIter* TableDef::pPartitionDefIter(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pPartitioningNode = pPartitionDef(pNode);
  ASSERT(pPartitioningNode);

  return pPartitioningNode->pNewChildrenIter();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
