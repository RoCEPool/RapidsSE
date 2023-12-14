/*============================================================================*/
/* IndexDefn.hpp - a definition of a index used in the plan                   */
/* Note: There is already an IndexDef.hpp file in the project. Needed to      */
/* give this file a unique name.                                              */
/*============================================================================*/

#include "BASE.h"
#include "IndexDefn.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "ColumnRefs.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_INDEXDEF_DEFN   = "/__root__/definitions/object/compound/indexDef"_Stz;
static Stz* C_INDEXDEF_NAME   = "indexDef"_Stz;

static Stz* C_CATALOGNAME_PROP = "catalogName"_Stz;
static Stz* C_CATALOGNAME_DEFN = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_SCHEMANAME_PROP  = "schemaName"_Stz;
static Stz* C_SCHEMANAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_TABLENAME_PROP   = "tableName"_Stz;
static Stz* C_TABLENAME_DEFN   = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_INDEXNAME_PROP   = "indexName"_Stz;
static Stz* C_INDEXNAME_DEFN   = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_COLUMNS_PROP  = "columns"_Stz;
static Stz* C_COLUMNS_DEFN  = "/__root__/definitions/object/list/columnRefs"_Stz;

static Stz* C_ISUNIQUE_PROP     = "notNull"_Stz;
static Stz* C_ISUNIQUE_DEFN     = "/__root__/definitions/object/valueObject/booleanObject"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Helper function to create indexDef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* IndexDefn::pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                              Stz* szIndexName, QueryNode* pColumnRefs, b1 bIsUnique)
{
  _static_

  ASSERT(szIndexName);
  ASSERT(szIndexName->nLength() > 0);
  // Create the indexDef object
  QueryNode* pIndexDef = new QueryNode(C_INDEXDEF_NAME, C_INDEXDEF_DEFN);

  // Create the catalogName property
  Value xCatalogNameVal(szCatalogName);
  QueryNode* pCatalogName = new QueryNode(C_CATALOGNAME_PROP, C_CATALOGNAME_DEFN, static_cast<Value&&>(xCatalogNameVal));
  pIndexDef->vAppendProperty(pCatalogName);

  // Create the schemaName property
  Value xSchemaNameVal(szSchemaName);
  QueryNode* pSchemaName = new QueryNode(C_SCHEMANAME_PROP, C_SCHEMANAME_DEFN, static_cast<Value&&>(xSchemaNameVal));
  pIndexDef->vAppendProperty(pSchemaName);

  // Create the tableName property
  Value xTableNameVal(szTableName);
  QueryNode* pTableName = new QueryNode(C_TABLENAME_PROP, C_TABLENAME_DEFN, static_cast<Value&&>(xTableNameVal));
  pIndexDef->vAppendProperty(pTableName);
  
  // Create the indexName property
  Value xIndexNameVal(szIndexName);
  QueryNode* pIndexName = new QueryNode(C_INDEXNAME_PROP, C_INDEXNAME_DEFN, static_cast<Value&&>(xIndexNameVal));
  pIndexDef->vAppendProperty(pIndexName);

  // Create the columns property
  ASSERT(pColumnRefs);
  ASSERT(ColumnRefs::bIsOfType(pColumnRefs));
  pIndexDef->vAppendProperty(pColumnRefs);

  // Create the isUnique property
  Value xIsUniqueVal(bIsUnique);
  QueryNode* pIsUnique = new QueryNode(C_ISUNIQUE_PROP, C_ISUNIQUE_DEFN, static_cast<Value&&>(xIsUniqueVal));
  pIndexDef->vAppendProperty(pIsUnique);

  return pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void IndexDefn::vDescribe(PlanNode* pNode, s2K& buf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pIndexRef = _pQueryNode(pNode);

  QueryNode* pName     = _pQueryNode(GraphNav::pFindProperty(C_INDEXNAME_PROP, pIndexRef));
  QueryNode* pColumns  = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pIndexRef));
  QueryNode* pIsUnique = _pQueryNode(GraphNav::pFindProperty(C_ISUNIQUE_PROP, pIndexRef));

  ASSERT(pName);
  ASSERT(pColumns);

  Stz* szName     = pName->szValue();
  b1 bIsUnique     = (pIsUnique == nullptr ? false : pIsUnique->bValue());

  ASSERT(szName);
  ASSERT(pColumns);
  ASSERT(pColumns->nChildren() > 0);

  // Get a description of the columnRefs
  s2K sColumnDesc;
  ColumnRefs::vDescribe(pColumns, sColumnDesc, pRootNode);


  buf.vCatSprint("%s INDEX %s (%s)"_Stz,
                 (bIsUnique ? "UNIQUE"_Stz : ""_Stz),
                 szName,
                 sColumnDesc.szStz());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the indexDef definition     */
/*----------------------------------------------------------------------------*/
b1 IndexDefn::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_INDEXDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IndexDefn::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_INDEXDEF_DEFN);
}


/*----------------------------------------------------------------------------*/
/* get catalog name from indexDef                                             */
/*----------------------------------------------------------------------------*/
Stz* IndexDefn::szCatalogName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogNameNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pNode));
  ASSERT(pCatalogNameNode);

  Stz* szCatalogName = pCatalogNameNode->szValue();

  return szCatalogName;
}

/*----------------------------------------------------------------------------*/
/* get schema name from indexDef                                              */
/*----------------------------------------------------------------------------*/
Stz* IndexDefn::szSchemaName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pSchemaNameNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMANAME_PROP, pNode));
  ASSERT(pSchemaNameNode);

  Stz* szSchemaName = pSchemaNameNode->szValue();

  return szSchemaName;
}

/*----------------------------------------------------------------------------*/
/* get table name from indexDef                                               */
/*----------------------------------------------------------------------------*/
Stz* IndexDefn::szTableName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pTableNameNode = _pQueryNode(GraphNav::pFindProperty(C_TABLENAME_PROP, pNode));
  ASSERT(pTableNameNode);

  Stz* szTableName = pTableNameNode->szValue();

  return szTableName;
}

/*----------------------------------------------------------------------------*/
/* get index name from indexDef                                             */
/*----------------------------------------------------------------------------*/
Stz* IndexDefn::szIndexName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pIndexNameNode = _pQueryNode(GraphNav::pFindProperty(C_INDEXNAME_PROP, pNode));
  ASSERT(pIndexNameNode);

  Stz* szIndexName = pIndexNameNode->szValue();

  return szIndexName;
}

/*----------------------------------------------------------------------------*/
/* get the number of columns used in this index                               */
/*----------------------------------------------------------------------------*/
i8 IndexDefn::iNumColumns(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnListNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));
  ASSERT(pColumnListNode);

  return pColumnListNode->iNumChildren();
}

/*----------------------------------------------------------------------------*/
/* Return an iterator of the columns used in this index                       */
/*----------------------------------------------------------------------------*/
PlanNodeIter* IndexDefn::pColumnIter(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnListNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNS_PROP, pNode));
  ASSERT(pColumnListNode);

  return pColumnListNode->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the index is a unique index                                */
/*----------------------------------------------------------------------------*/
b1 IndexDefn::bIsUnique(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pIsUniqueNode = _pQueryNode(GraphNav::pFindProperty(C_ISUNIQUE_PROP, pNode));

  ASSERT(pIsUniqueNode);
  return pIsUniqueNode->bValue();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
