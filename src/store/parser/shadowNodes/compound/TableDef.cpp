/*============================================================================*/
/* TableDef.hpp - a definition of a table used in the plan                    */
/*============================================================================*/

#include "BASE.h"
#include "TableDef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "ColumnDefs.hpp"
#include "IndexDefs.hpp"

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



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Helper function to create tableDef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* TableDef::pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                             QueryNode* pColumnDefs, QueryNode* pIndexDefs)
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


  // Create the tableDef object
  QueryNode* pTableDef = new QueryNode(C_TABLEDEF_NAME, C_TABLEDEF_DEFN);

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
  pColumnDefs->vRename(C_COLUMNS_PROP);
  pTableDef->vAppendProperty(pColumnDefs);

  // Create the indexes property
  pIndexDefs->vRename(C_INDEXES_PROP);
  pTableDef->vAppendProperty(pIndexDefs);

  return pTableDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void TableDef::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pTableDef = static_cast<QueryNode*>(pNode);

  QueryNode* pCatalogName = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pTableDef));
  QueryNode* pSchemaName  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_SCHEMANAME_PROP, pTableDef));
  QueryNode* pTableName   = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLENAME_PROP, pTableDef));
  QueryNode* pColumns     = static_cast<QueryNode*>(GraphNav::pFindProperty(C_COLUMNS_PROP, pTableDef));
  QueryNode* pIndexes     = static_cast<QueryNode*>(GraphNav::pFindProperty(C_INDEXES_PROP, pTableDef));
  
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
  ColumnDefs::vDescribe(pColumns, sColumnDesc);
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
/* get the catalog name from tableDef                                         */
/*----------------------------------------------------------------------------*/
Stz* TableDef::szCatalogName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogNameNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pNode));
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

  QueryNode* pSchemaNameNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_SCHEMANAME_PROP, pNode));
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

  QueryNode* pTableNameNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLENAME_PROP, pNode));
  ASSERT(pTableNameNode);

  Stz* szTableName = pTableNameNode->szValue();

  return szTableName;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
