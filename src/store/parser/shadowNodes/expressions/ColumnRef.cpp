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

static Stz* C_CATALOG_NAME_PROP = "catalogName"_Stz;
static Stz* C_SCHEMA_NAME_PROP  = "schemaName"_Stz;
static Stz* C_TABLE_NAME_PROP   = "tableName"_Stz;
static Stz* C_COLUMN_NAME_PROP  = "columnName"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the columnName of the projectedExpr                                */
/*----------------------------------------------------------------------------*/
Stz* ColumnRef::szColumnName(QueryNode* pNode)
{
  QueryNode *pColumnNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_COLUMN_NAME_PROP, pNode));

  return pColumnNode->szValue();
}

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
  pColRef->vAppendProperty(new QueryNode(C_CATALOG_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xCatalogName)));
  pColRef->vAppendProperty(new QueryNode(C_SCHEMA_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xSchemaName)));
  pColRef->vAppendProperty(new QueryNode(C_TABLE_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xTableName)));
  pColRef->vAppendProperty(new QueryNode(C_COLUMN_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN_NONNULL), static_cast<Value&&>(xColumnName)));

  return pColRef;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ColumnRef::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pColumnRef = static_cast<QueryNode*>(pNode);

  QueryNode* pCatalogNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOG_NAME_PROP, pColumnRef));
  QueryNode* pSchemaNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_SCHEMA_NAME_PROP,  pColumnRef));
  QueryNode* pTableNode   = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLE_NAME_PROP,   pColumnRef));
  QueryNode* pColumnNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_COLUMN_NAME_PROP,  pColumnRef));

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
