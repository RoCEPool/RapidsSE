/*============================================================================*/
/* ColumnRef.cpp - an expression representing a reference to a column         */
/*============================================================================*/

#include "BASE.h"
#include "ColumnRef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COLREF_DEFN = "/__root__/definitions/object/expr/columnRef"_Stz;

static Stz* C_COLUMNREF_NAME_PREFIX = "ExprColRef"_Stz;

static Stz* C_CATALOG_PROP = "catalogName"_Stz;
static Stz* C_SCHEMA_PROP  = "schemaName"_Stz;
static Stz* C_TABLE_PROP   = "tableName"_Stz;
static Stz* C_COLUMN_PROP  = "columnName"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create expression column reference nodes.               */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnRef::pCreate(Text* pCatalogName, Text* pSchemaName,
                              Text* pTableName, Text* pColumnName)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_COLUMNREF_NAME_PREFIX, sName);

  // These Value instances will take ownership of the Text*s.
  Value xCatalogName(pCatalogName);
  Value xSchemaName(pSchemaName);
  Value xTableName(pTableName);
  Value xColumnName(pColumnName);

  QueryNode* pColRef = new QueryNode(sName, C_COLREF_DEFN);
  pColRef->vAppendProperty(new QueryNode(C_CATALOG_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xCatalogName)));
  pColRef->vAppendProperty(new QueryNode(C_SCHEMA_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xSchemaName)));
  pColRef->vAppendProperty(new QueryNode(C_TABLE_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xTableName)));
  pColRef->vAppendProperty(new QueryNode(C_COLUMN_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN_NONNULL), static_cast<Value&&>(xColumnName)));

  return pColRef;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ColumnRef::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnRef = static_cast<QueryNode*>(pNode);

  QueryNode* pCatalogNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOG_PROP, pColumnRef));
  QueryNode* pSchemaNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_SCHEMA_PROP, pColumnRef));
  QueryNode* pTableNode   = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLE_PROP, pColumnRef));
  QueryNode* pColumnNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_COLUMN_PROP, pColumnRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);
  ASSERT(pColumnNode);

  Stz* szCatalogName = pCatalogNode->szValue();
  Stz* szSchemaName  = pSchemaNode->szValue();
  Stz* szTableName   = pTableNode->szValue();
  Stz* szColumnName  = pColumnNode->szValue();

  if (szCatalogName) {
    sBuf.vConcat(szCatalogName);
    sBuf.vAppend('.');
    ASSERT(szSchemaName);
  }

  if (szSchemaName) {
    sBuf.vConcat(szSchemaName);
    sBuf.vAppend('.');
    ASSERT(szTableName);
  }

  if (szTableName) {
    sBuf.vConcat(szTableName);
    sBuf.vAppend('.');
  }

  ASSERT(szColumnName);
  sBuf.vConcat(szColumnName);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ColumnRef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COLREF_DEFN);
}


/*----------------------------------------------------------------------------*/
/* Returns the catalogName of the columnRef. May be null.                     */
/*----------------------------------------------------------------------------*/
Stz* ColumnRef::szCatalog(QueryNode* pNode)
{
  QueryNode *pCatalogNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_CATALOG_PROP, pNode));

  return pCatalogNode->szValue();
}


/*----------------------------------------------------------------------------*/
/* Returns the schemaName of the columnRef. May be null.                      */
/*----------------------------------------------------------------------------*/
Stz* ColumnRef::szSchema(QueryNode* pNode)
{
  QueryNode *pSchemaNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_SCHEMA_PROP, pNode));

  return pSchemaNode->szValue();
}


/*----------------------------------------------------------------------------*/
/* Returns the tableName of the columnRef. May be null.                       */
/*----------------------------------------------------------------------------*/
Stz* ColumnRef::szTable(QueryNode* pNode)
{
  QueryNode *pTableNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_TABLE_PROP, pNode));

  return pTableNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the columnName of the projectedExpr                                */
/*----------------------------------------------------------------------------*/
Stz* ColumnRef::szColumn(QueryNode* pNode)
{
  QueryNode *pColumnNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_COLUMN_PROP, pNode));

  return pColumnNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the columnRef is fully qualified (has cat/sch/tbl names)   */
/*----------------------------------------------------------------------------*/
b1 ColumnRef::bIsFullyQualified(QueryNode* pNode)
{
  QueryNode *pCatalogNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_CATALOG_PROP, pNode));

  QueryNode *pSchemaNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_SCHEMA_PROP, pNode));

  QueryNode *pTableNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_TABLE_PROP, pNode));

  QueryNode *pColumnNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_COLUMN_PROP, pNode));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);
  ASSERT(pColumnNode);

  Stz* szCatalogName = pCatalogNode->szValue();
  Stz* szSchemaName  = pSchemaNode->szValue();
  Stz* szTableName   = pTableNode->szValue();
  Stz* szColumnName  = pColumnNode->szValue();

  return (szCatalogName && szSchemaName && szTableName && szColumnName);
}

/*----------------------------------------------------------------------------*/
/* Update the ColumnRef so that it is fully qualified                         */
/*----------------------------------------------------------------------------*/
void ColumnRef::vQualify(QueryNode* pColumnRef, Stz* szCatalogName, Stz* szSchemaName,
                         Stz* szTableName, Stz* szColumnName)
{
  ASSERT(bIsOfType(pColumnRef));
  ASSERT(szCatalogName);
  ASSERT(szSchemaName);
  ASSERT(szTableName);
  ASSERT(szColumnName);

  QueryNode* pCatalogNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOG_PROP, pColumnRef));
  QueryNode* pSchemaNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_SCHEMA_PROP, pColumnRef));
  QueryNode* pTableNode   = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLE_PROP, pColumnRef));
  QueryNode* pColumnNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_COLUMN_PROP, pColumnRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);
  ASSERT(pColumnNode);

  Value xCatValue;
  xCatValue.vSet(szCatalogName);
  pCatalogNode->vSetValue(static_cast<Value&&>(xCatValue));

  Value xSchValue;
  xSchValue.vSet(szSchemaName);
  pSchemaNode->vSetValue(static_cast<Value&&>(xSchValue));

  Value xTblValue;
  xTblValue.vSet(szTableName);
  pTableNode->vSetValue(static_cast<Value&&>(xTblValue));

  Value xColValue;
  xColValue.vSet(szColumnName);
  pColumnNode->vSetValue(static_cast<Value&&>(xColValue));

  ASSERT(bIsFullyQualified(pColumnRef));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
