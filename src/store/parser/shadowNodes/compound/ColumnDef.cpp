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

static Stz* C_COLUMNNAME_PROP  = "columnName"_Stz;
static Stz* C_COLUMNNAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_COLUMNTYPE_PROP  = "columnType"_Stz;
static Stz* C_COLUMNTYPE_DEFN  = "/__root__/definitions/types/sqlType"_Stz;

static Stz* C_NOTNULL_PROP     = "notNull"_Stz;
static Stz* C_NOTNULL_DEFN     = "/__root__/definitions/object/valueObject/booleanObject"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Helper function to create columnDef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnDef::pCreate(Stz* szColumnName, SqlType::Type eColumnType, b1 bNotNull)
{
  _static_

  ASSERT(szColumnName);
  ASSERT(szColumnName->nLength() > 0);


  // Create the columnDef object
  QueryNode* pColumnDef = new QueryNode(C_COLUMNDEF_NAME, C_COLUMNDEF_DEFN);

  // Create the columnName property
  Value xNameVal(szColumnName);
  QueryNode* pName = new QueryNode(C_COLUMNNAME_PROP, C_COLUMNNAME_DEFN, static_cast<Value&&>(xNameVal));
  pColumnDef->vAppendProperty(pName);

  // Create the type property
  Value xTypeVal((i8) eColumnType);
  QueryNode* pType = new QueryNode(C_COLUMNTYPE_PROP, C_COLUMNTYPE_DEFN, static_cast<Value&&>(xTypeVal));
  pColumnDef->vAppendProperty(pType);

  // Create the notNull property
  Value xNotNullVal(bNotNull);
  QueryNode* pNotNull = new QueryNode(C_NOTNULL_PROP, C_NOTNULL_DEFN, static_cast<Value&&>(xNotNullVal));
  pColumnDef->vAppendProperty(pNotNull);

  return pColumnDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void ColumnDef::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnRef = static_cast<QueryNode*>(pNode);

  QueryNode* pName    = static_cast<QueryNode*>(GraphNav::pFindProperty(C_COLUMNNAME_PROP, pColumnRef));
  QueryNode* pType    = static_cast<QueryNode*>(GraphNav::pFindProperty(C_COLUMNTYPE_PROP, pColumnRef));
  QueryNode* pNotNull = static_cast<QueryNode*>(GraphNav::pFindProperty(C_NOTNULL_PROP, pColumnRef));

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
/* get column name from columnDef                                             */
/*----------------------------------------------------------------------------*/
Stz* ColumnDef::szColumnName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnNameNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_COLUMNNAME_PROP, pNode));
  ASSERT(pColumnNameNode);

  Stz* szColumnName = pColumnNameNode->szValue();

  return szColumnName;
}

/*----------------------------------------------------------------------------*/
/* get the SQL data type of this column                                       */
/*----------------------------------------------------------------------------*/
SqlType::Type ColumnDef::eColumnType(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pTypeNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_COLUMNTYPE_PROP, pNode));
  ASSERT(pTypeNode);

  SqlType::Type eType = SqlType::eType(pTypeNode);

  return eType;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the column is defined as NOT NULL                          */
/*----------------------------------------------------------------------------*/
b1 ColumnDef::bNotNull(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pNotNullNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_NOTNULL_PROP, pNode));

  ASSERT(pNotNullNode);
  return pNotNullNode->bValue();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
