/*============================================================================*/
/* ColumnRef.cpp - an expression representing a reference to a column         */
/*============================================================================*/

#include "BASE.h"
#include "ColumnRef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

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
  pColRef->vAppendChild(new QueryNode(C_CATALOG_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), xCatalogName));
  pColRef->vAppendChild(new QueryNode(C_SCHEMA_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), xSchemaName));
  pColRef->vAppendChild(new QueryNode(C_TABLE_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), xTableName));
  pColRef->vAppendChild(new QueryNode(C_COLUMN_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN_NONNULL), xColumnName));

  return pColRef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void ColumnRef::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  sBuf.vConcat(pNode->szName());
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
