/*============================================================================*/
/* SchemaDef.hpp - a definition of a schema used in the plan                  */
/*============================================================================*/

#include "BASE.h"
#include "SchemaDef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "TableDefs.hpp"
#include "TableDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SCHEMADEF_DEFN   = "/__root__/definitions/object/compound/schemaDef"_Stz;
static Stz* C_SCHEMADEF_NAME   = "schemaDef"_Stz;

static Stz* C_CATALOGNAME_PROP  = "catalogName"_Stz;
static Stz* C_CATALOGNAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_SCHEMANAME_PROP  = "schemaName"_Stz;
static Stz* C_SCHEMANAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_TABLES_PROP  = "tables"_Stz;
static Stz* C_TABLES_DEFN  = "/__root__/definitions/object/list/tableDefs"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create schemaDef nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* SchemaDef::pCreate(Stz* szCatalogName, Stz* szSchemaName, QueryNode* pTableDefs)
{
  _static_

  ASSERT(szCatalogName);
  ASSERT(szCatalogName->nLength() > 0);
  ASSERT(szSchemaName);
  ASSERT(szSchemaName->nLength() > 0);
  ASSERT(szSchemaName);
  ASSERT(szSchemaName->nLength() > 0);
  ASSERT(pTableDefs);


  // Create the schemaDef object
  QueryNode* pSchemaDef = new QueryNode(szSchemaName, C_SCHEMADEF_DEFN);

  // Create the catalogName property
  Value xCatalogNameVal(szCatalogName);
  QueryNode* pCatalogName = new QueryNode(C_CATALOGNAME_PROP, C_CATALOGNAME_DEFN, static_cast<Value&&>(xCatalogNameVal));
  pSchemaDef->vAppendProperty(pCatalogName);

  // Create the schemaName property
  Value xSchemaNameVal(szSchemaName);
  QueryNode* pSchemaName = new QueryNode(C_SCHEMANAME_PROP, C_SCHEMANAME_DEFN, static_cast<Value&&>(xSchemaNameVal));
  pSchemaDef->vAppendProperty(pSchemaName);

  // Create the tables property
  pTableDefs->vSetName(C_TABLES_PROP);
  pSchemaDef->vAppendProperty(pTableDefs);

  return pSchemaDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the schema reference into a buffer                */
/*----------------------------------------------------------------------------*/
void SchemaDef::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pSchemaDef = _pQueryNode(pNode);

  QueryNode* pCatalogName = _pQueryNode(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pSchemaDef));
  QueryNode* pSchemaName  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMANAME_PROP, pSchemaDef));

  ASSERT(pCatalogName);
  ASSERT(pSchemaName);
  ASSERT(pSchemaName->szValue());

  sBuf.vConcat("SCHEMA "_Stz);
  if (pCatalogName->szValue()) {
    sBuf.vConcat(pCatalogName->szValue());
    sBuf.vConcat("."_Stz);
  }

  sBuf.vConcat(pSchemaName->szValue());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the schemaDef definition    */
/*----------------------------------------------------------------------------*/
b1 SchemaDef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SCHEMADEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SchemaDef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SCHEMADEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the catalog name from schemaDef                                        */
/*----------------------------------------------------------------------------*/
Stz* SchemaDef::szCatalogName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogNameNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pNode));
  ASSERT(pCatalogNameNode);

  Stz* szCatalogName = pCatalogNameNode->szValue();

  return szCatalogName;
}

/*----------------------------------------------------------------------------*/
/* get the schema name from schemaDef                                         */
/*----------------------------------------------------------------------------*/
Stz* SchemaDef::szSchemaName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pSchemaNameNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMANAME_PROP, pNode));
  ASSERT(pSchemaNameNode);

  Stz* szSchemaName = pSchemaNameNode->szValue();

  return szSchemaName;
}

/*----------------------------------------------------------------------------*/
/* Returns a QueryNode that represents a given TableDef in this schema        */
/*----------------------------------------------------------------------------*/
QueryNode* SchemaDef::pTableDef(QueryNode* pNode, Stz* szTableName)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(szTableName);

  QueryNode* pTableDef = nullptr;

  // Get the tables property
  QueryNode* pTablesNode = _pQueryNode(GraphNav::pFindProperty(C_TABLES_PROP, pNode));

  if (!pTablesNode) {
    return pTableDef;
  }

  PlanNodeIter* pIter = pTablesNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(TableDef::bIsOfType(pChild));

    if (TableDef::szTableName(pChild)->bSame(szTableName)) {
      pTableDef = pChild;
      break;
    }
  }

  delete pIter;
  return pTableDef;
}

/*----------------------------------------------------------------------------*/
/* get an iterator of PlanNodes that represent TableDefs                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* SchemaDef::pTableDefIter(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pTablesNode = _pQueryNode(GraphNav::pFindProperty(C_TABLES_PROP, pNode));
  ASSERT(pTablesNode);

  return pTablesNode->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Add a tableDef to this schemaDef                                           */
/*----------------------------------------------------------------------------*/
void SchemaDef::vAddTable(QueryNode *pNode, QueryNode *pTableDefNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pTablesNode = _pQueryNode(GraphNav::pFindProperty(C_TABLES_PROP, pNode));
  ASSERT(pTablesNode);

  pTablesNode->vAppendChild(pTableDefNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
