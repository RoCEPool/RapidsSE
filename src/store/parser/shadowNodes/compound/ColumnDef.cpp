/*============================================================================*/
/* ColumnDef.hpp - a definition of a column used in the plan                  */
/*============================================================================*/

#include "BASE.h"
#include "ColumnDef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COLUMNDEF_DEFN   = "/__root__/definitions/object/compound/columnDef"_Stz;
static Stz* C_COLUMNDEF_NAME   = "columnDef"_Stz;

static Stz* C_CATALOGNAME_PROP = "catalogName"_Stz;
static Stz* C_CATALOGNAME_DEFN = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_SCHEMANAME_PROP  = "schemaName"_Stz;
static Stz* C_SCHEMANAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_TABLENAME_PROP   = "tableName"_Stz;
static Stz* C_TABLENAME_DEFN   = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_COLUMNNAME_PROP  = "columnName"_Stz;
static Stz* C_COLUMNNAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_COLUMNTYPE_PROP  = "columnType"_Stz;
static Stz* C_COLUMNTYPE_DEFN  = "/__root__/definitions/types/sqlType"_Stz;

static Stz* C_NOTNULL_PROP     = "notNull"_Stz;
static Stz* C_NOTNULL_DEFN     = "/__root__/definitions/object/valueObject/booleanObject"_Stz;

static Stz* C_ORDINAL_PROP     = "ordinal"_Stz;
static Stz* C_ORDINAL_DEFN     = "/__root__/definitions/object/valueObject/integerObject"_Stz;

static Stz* C_PRECISION_PROP   = "precision"_Stz;
static Stz* C_PRECISION_DEFN   = "/__root__/definitions/object/valueObject/integerObject"_Stz;

static Stz* C_SCALE_PROP       = "scale"_Stz;
static Stz* C_SCALE_DEFN       = "/__root__/definitions/object/valueObject/integerObject"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Helper function to create columnDef nodes.                                 */
/* Ordinal is 0 based. Use -1 for precision and scale if not set or n/a.      */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnDef::pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName,
                              Stz* szColumnName, QueryNode* pTypeNode, b1 bNotNull,
                              i8 iOrdinal, i8 iPrecision, i8 iScale)
{
  _static_

  ASSERT(szColumnName);
  ASSERT(szColumnName->nLength() > 0);
  ASSERT(iOrdinal >= 0);
  ASSERT(iPrecision >= -1);
  ASSERT(iScale >= -1);
  ASSERT(iOrdinal >= 0);
  ASSERT(iPrecision >= -1);
  ASSERT(iScale >= -1);

  // Create the columnDef object
  QueryNode* pColumnDef = new QueryNode(szColumnName, C_COLUMNDEF_DEFN);

  // Create the catalogName property
  Value xCatalogNameVal(szCatalogName);
  QueryNode* pCatalogName = new QueryNode(C_CATALOGNAME_PROP, C_CATALOGNAME_DEFN, static_cast<Value&&>(xCatalogNameVal));
  pColumnDef->vAppendProperty(pCatalogName);

  // Create the schemaName property
  Value xSchemaNameVal(szSchemaName);
  QueryNode* pSchemaName = new QueryNode(C_SCHEMANAME_PROP, C_SCHEMANAME_DEFN, static_cast<Value&&>(xSchemaNameVal));
  pColumnDef->vAppendProperty(pSchemaName);

  // Create the tableName property
  Value xTableNameVal(szTableName);
  QueryNode* pTableName = new QueryNode(C_TABLENAME_PROP, C_TABLENAME_DEFN, static_cast<Value&&>(xTableNameVal));
  pColumnDef->vAppendProperty(pTableName);

  // Create the columnName property
  Value xColumnNameVal(szColumnName);
  QueryNode* pColumnName = new QueryNode(C_COLUMNNAME_PROP, C_COLUMNNAME_DEFN, static_cast<Value&&>(xColumnNameVal));
  pColumnDef->vAppendProperty(pColumnName);

  // Add the type property
  pTypeNode->vSetName(C_COLUMNTYPE_PROP);
  pColumnDef->vAppendProperty(pTypeNode);

  // Create the notNull property
  Value xNotNullVal(bNotNull);
  QueryNode* pNotNull = new QueryNode(C_NOTNULL_PROP, C_NOTNULL_DEFN, static_cast<Value&&>(xNotNullVal));
  pColumnDef->vAppendProperty(pNotNull);

  // Create the ordinal property
  Value xOrdinalVal(iOrdinal);
  QueryNode* pOrdinal = new QueryNode(C_ORDINAL_PROP, C_ORDINAL_DEFN, static_cast<Value&&>(xOrdinalVal));
  pColumnDef->vAppendProperty(pOrdinal);

  // Create the precision property
  Value xPrecisionVal(iPrecision);
  QueryNode* pPrecision = new QueryNode(C_PRECISION_PROP, C_PRECISION_DEFN, static_cast<Value&&>(xPrecisionVal));
  pColumnDef->vAppendProperty(pPrecision);

  // Create the scale property
  Value xScaleVal(iScale);
  QueryNode* pScale = new QueryNode(C_SCALE_PROP, C_SCALE_DEFN, static_cast<Value&&>(xScaleVal));
  pColumnDef->vAppendProperty(pScale);

  return pColumnDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void ColumnDef::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnRef = _pQueryNode(pNode);

  QueryNode* pName    = _pQueryNode(GraphNav::pFindProperty(C_COLUMNNAME_PROP, pColumnRef));
  QueryNode* pType    = _pQueryNode(GraphNav::pFindProperty(C_COLUMNTYPE_PROP, pColumnRef));
  QueryNode* pNotNull = _pQueryNode(GraphNav::pFindProperty(C_NOTNULL_PROP, pColumnRef));

  ASSERT(pName);
  ASSERT(pType);

  Stz* szName     = pName->szValue();
  b1 bNotNull     = (pNotNull == nullptr ? false : pNotNull->bValue());

  ASSERT(szName);

  buf.vConcat(szName);
  buf.vAppend(' ');
  buf.vConcat(SqlType::szTypeName(pType));

  if (bNotNull) {
    buf.vAppend(' ');
    buf.vConcat("NOT NULL"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the columnDef definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnDef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COLUMNDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnDef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_COLUMNDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get catalog name from columnDef                                            */
/*----------------------------------------------------------------------------*/
Stz* ColumnDef::szCatalogName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogNameNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pNode));
  ASSERT(pCatalogNameNode);

  Stz* szCatalogName = pCatalogNameNode->szValue();

  return szCatalogName;
}

/*----------------------------------------------------------------------------*/
/* get schema name from columnDef                                            */
/*----------------------------------------------------------------------------*/
Stz* ColumnDef::szSchemaName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pSchemaNameNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMANAME_PROP, pNode));
  ASSERT(pSchemaNameNode);

  Stz* szSchemaName = pSchemaNameNode->szValue();

  return szSchemaName;
}

/*----------------------------------------------------------------------------*/
/* get table name from columnDef                                            */
/*----------------------------------------------------------------------------*/
Stz* ColumnDef::szTableName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pTableNameNode = _pQueryNode(GraphNav::pFindProperty(C_TABLENAME_PROP, pNode));
  ASSERT(pTableNameNode);

  Stz* szTableName = pTableNameNode->szValue();

  return szTableName;
}

/*----------------------------------------------------------------------------*/
/* get column name from columnDef                                             */
/*----------------------------------------------------------------------------*/
Stz* ColumnDef::szColumnName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnNameNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNNAME_PROP, pNode));
  ASSERT(pColumnNameNode);

  Stz* szColumnName = pColumnNameNode->szValue();

  return szColumnName;
}

/*----------------------------------------------------------------------------*/
/* get the SQL data type of this column                                       */
/*----------------------------------------------------------------------------*/
eCT ColumnDef::eColumnType(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pTypeNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNTYPE_PROP, pNode));
  ASSERT(pTypeNode);

  eCT eType = SqlType::eType(pTypeNode);

  return eType;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the column is defined as NOT NULL                          */
/*----------------------------------------------------------------------------*/
b1 ColumnDef::bNotNull(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pNotNullNode = _pQueryNode(GraphNav::pFindProperty(C_NOTNULL_PROP, pNode));

  ASSERT(pNotNullNode);
  return pNotNullNode->bValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the 0-based index of the column number                             */
/*----------------------------------------------------------------------------*/
i8 ColumnDef::iOrdinal(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pOrdinalNode = _pQueryNode(GraphNav::pFindProperty(C_ORDINAL_PROP, pNode));

  ASSERT(pOrdinalNode);
  return pOrdinalNode->iValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the length/precision of the column, or -1 if not set/applicable    */
/*----------------------------------------------------------------------------*/
i8 ColumnDef::iPrecision(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pPrecisionNode = _pQueryNode(GraphNav::pFindProperty(C_PRECISION_PROP, pNode));

  ASSERT(pPrecisionNode);
  return pPrecisionNode->iValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the scale (num digits to the right of the decimal point), or -1    */
/*----------------------------------------------------------------------------*/
i8 ColumnDef::iScale(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pScaleNode = _pQueryNode(GraphNav::pFindProperty(C_SCALE_PROP, pNode));

  ASSERT(pScaleNode);
  return pScaleNode->iValue();
}

/*----------------------------------------------------------------------------*/
/* Updates the column name of this column definition                          */
/*----------------------------------------------------------------------------*/
void ColumnDef::vSetColumnName(QueryNode* pNode, Stz* szNewName)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(szNewName);

  QueryNode* pColumnNameNode = _pQueryNode(GraphNav::pFindProperty(C_COLUMNNAME_PROP, pNode));

  ASSERT(pColumnNameNode);
  Value xValue(szNewName);
  pColumnNameNode->vSetValue(static_cast<Value&&>(xValue));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
