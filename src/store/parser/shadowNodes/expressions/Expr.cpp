/*============================================================================*/
/* Expr.cpp - a generic expression                                            */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_EXPR_DEFN = "/__root__/definitions/object/expr"_Stz;
static Stz* C_COLREF_DEFN = "/__root__/definitions/object/expr/columnRef"_Stz;

static Stz* aTypeDefinitions[] = {
  /* 00 ET_NONE      */ "<INVALID>"_Stz,
  /* 01 ET_NULL      */ "/__root__/definitions/object/expr/literal/null"_Stz,
  /* 02 ET_BOOLEAN   */ "/__root__/definitions/object/expr/literal/boolean"_Stz,
  /* 03 ET_STRING    */ "/__root__/definitions/object/expr/literal/string"_Stz,
  /* 04 ET_INTEGER   */ "/__root__/definitions/object/expr/literal/integer"_Stz,
  /* 05 ET_FLOAT     */ "/__root__/definitions/object/expr/literal/float"_Stz,
  /* 06 ET_DECIMAL   */ "/__root__/definitions/object/expr/literal/decimal"_Stz,
  /* 07 ET_DATE      */ "/__root__/definitions/object/expr/literal/date"_Stz,
  /* 08 ET_TIME      */ "/__root__/definitions/object/expr/literal/time"_Stz,
  /* 09 ET_TIMESTAMP */ "/__root__/definitions/object/expr/literal/timestamp"_Stz,
  /* 10 ET_INTERVAL  */ "/__root__/definitions/object/expr/literal/interval"_Stz,
  /* 11 ET_NENUMS    */ "<INVALID>"_Stz,
};

static Stz* C_LITERAL_NAME_PREFIX = "ExprLiteral"_Stz;
static Stz* C_COLUMNREF_NAME_PREFIX = "ExprColRef"_Stz;

static Stz* C_CATALOG_NAME_PROP = "catalogName"_Stz;
static Stz* C_SCHEMA_NAME_PROP  = "schemaName"_Stz;
static Stz* C_TABLE_NAME_PROP   = "tableName"_Stz;
static Stz* C_COLUMN_NAME_PROP  = "columnName"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* Expr::pCreateLiteral(eET eType, Value xVal)
{
  _static_

  ASSERT(eType > ET_NONE && eType < ET_NENUMS);

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_LITERAL_NAME_PREFIX, sName);

  QueryNode* pExpr = new QueryNode(sName, aTypeDefinitions[eType], xVal);
  return pExpr;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create expression column reference nodes.               */
/*----------------------------------------------------------------------------*/
QueryNode* Expr::pCreateColumnRef(Text* pCatalogName, Text* pSchemaName,
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

  QueryNode* pExpr = new QueryNode(sName, C_COLREF_DEFN);
  pExpr->vAppendChild(new QueryNode(C_CATALOG_NAME_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), xCatalogName));
  pExpr->vAppendChild(new QueryNode(C_SCHEMA_NAME_PROP,  const_cast<Stz*>(C_STRING_OBJ_DEFN), xSchemaName));
  pExpr->vAppendChild(new QueryNode(C_TABLE_NAME_PROP,   const_cast<Stz*>(C_STRING_OBJ_DEFN), xTableName));
  pExpr->vAppendChild(new QueryNode(C_COLUMN_NAME_PROP,  const_cast<Stz*>(C_STRING_OBJ_DEFN_NONNULL), xColumnName));

  return pExpr;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void Expr::vPrint(QueryNode* pStmt, s256& sBuf)
{
  _static_
  ASSERT(bValidate(pStmt));

  sBuf.vConcat(pStmt->szName());
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
