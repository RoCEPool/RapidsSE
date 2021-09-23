/*============================================================================*/
/* ColumnRef.cpp - an expression representing a reference to a column         */
/*============================================================================*/

#include "BASE.h"
#include "ColumnRef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "NodeNamer.hpp"

#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"
#include "ColumnDef.hpp"
#include "StringObject.hpp"
#include "CteDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COLREF_DEFN = "/__root__/definitions/object/expr/columnRef"_Stz;

static Stz* C_COLUMNREF_NAME_PREFIX = "ExprColRef"_Stz;

static Stz* C_TABLEALIAS_PROP = "tableAlias"_Stz;
static Stz* C_CATALOG_PROP    = "catalogName"_Stz;
static Stz* C_SCHEMA_PROP     = "schemaName"_Stz;
static Stz* C_TABLE_PROP      = "tableName"_Stz;
static Stz* C_COLUMN_PROP     = "columnName"_Stz;
static Stz* C_DEFNPATH_PROP   = "defnPath"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create expression column reference nodes.               */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnRef::pCreate(Text* pTableAliasName,
                              Text* pCatalogName, Text* pSchemaName,
                              Text* pTableName, Text* pColumnName,
                              Text* pDefnPath)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_COLREF_DEFN, sName);

  // These Value instances will take ownership of the Text*s.
  Value xTableAliasName(pTableAliasName);
  Value xCatalogName(pCatalogName);
  Value xSchemaName(pSchemaName);
  Value xTableName(pTableName);
  Value xColumnName(pColumnName);
  Value xDefnPath(pDefnPath);

  QueryNode* pColRef = new QueryNode(sName, C_COLREF_DEFN);
  pColRef->vAppendProperty(new QueryNode(C_TABLEALIAS_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xTableAliasName)));
  pColRef->vAppendProperty(new QueryNode(C_CATALOG_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xCatalogName)));
  pColRef->vAppendProperty(new QueryNode(C_SCHEMA_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xSchemaName)));
  pColRef->vAppendProperty(new QueryNode(C_TABLE_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xTableName)));
  pColRef->vAppendProperty(new QueryNode(C_COLUMN_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN_NONNULL), static_cast<Value&&>(xColumnName)));
  pColRef->vAppendProperty(new QueryNode(C_DEFNPATH_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xDefnPath)));

  return pColRef;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create expression column reference nodes.               */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnRef::pCreate(Stz* szTableAliasName,
                              Stz* szCatalogName, Stz* szSchemaName,
                              Stz* szTableName,   Stz* szColumnName,
                              Stz* szDefnPath)
{
  _static_

  Text* pTableAliasName = (szTableAliasName ? new Text(szTableAliasName) : nullptr);
  Text* pCatalogName    = (szCatalogName    ? new Text(szCatalogName)    : nullptr);
  Text* pSchemaName     = (szSchemaName     ? new Text(szSchemaName)     : nullptr);
  Text* pTableName      = (szTableName      ? new Text(szTableName)      : nullptr);
  Text* pColumnName     = (szColumnName     ? new Text(szColumnName)     : nullptr);
  Text* pDefnPath       = (szDefnPath       ? new Text(szDefnPath)       : nullptr);

  return pCreate(pTableAliasName, pCatalogName, pSchemaName, pTableName, pColumnName, pDefnPath);
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ColumnRef::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnRef = _pQueryNode(pNode);

  QueryNode* pTableAliasNode = _pQueryNode(GraphNav::pFindProperty(C_TABLEALIAS_PROP, pColumnRef));
  QueryNode* pCatalogNode    = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP,    pColumnRef));
  QueryNode* pSchemaNode     = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP,     pColumnRef));
  QueryNode* pTableNode      = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP,      pColumnRef));
  QueryNode* pColumnNode     = _pQueryNode(GraphNav::pFindProperty(C_COLUMN_PROP,     pColumnRef));

  ASSERT(pTableAliasNode || (pCatalogNode && pSchemaNode && pTableNode));
  ASSERT(pColumnNode);

  Stz* szTableAliasName = pTableAliasNode->szValue();
  Stz* szCatalogName    = pCatalogNode->szValue();
  Stz* szSchemaName     = pSchemaNode->szValue();
  Stz* szTableName      = pTableNode->szValue();
  Stz* szColumnName     = pColumnNode->szValue();

  if (szTableAliasName) {
    sBuf.vConcat(szTableAliasName);
    sBuf.vAppend('.');

  } else {
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
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnRef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_COLREF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT ColumnRef::eType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(bIsFullyQualified(pNode));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Get the metadata node
  QueryNode* pMetadata = ShadowRoot::pMetadataNode(pRootNode);
  ASSERT(pMetadata);

  Stz* szCatalog  = ColumnRef::szCatalog(pNode);
  Stz* szSchema   = ColumnRef::szSchema(pNode);
  Stz* szTable    = ColumnRef::szTable(pNode);
  Stz* szColumn   = ColumnRef::szColumn(pNode);
  Stz* szDefnPath = ColumnRef::szDefnPath(pNode);

  ASSERT(szColumn);

  if (!szCatalog || !szSchema || !szTable) {
    // This can occur when we have columnRefs for named subqueries, such as
    // Common Table Expressions (CTEs), which have a (table) name but no
    // catalog or schema name.
    ASSERT(szDefnPath);

    // Get the node for this path
    QueryNode* pCteDef = _pQueryNode(GraphNav::pFind(szDefnPath, pRootNode));
    ASSERT(pCteDef);
    ASSERT(CteDef::bIsOfType(pCteDef));

    QueryNode* pColumnDef = CteDef::pColumnDef(pCteDef, szColumn);
    ASSERT(pColumnDef);
    ASSERT(ColumnDef::bIsOfType(pColumnDef));

    eCT eType = ColumnDef::eColumnType(pColumnDef);
    ASSERT(eType > CT_NONE);
    ASSERT(eType < CT_ENUMS);

    return eType;
  }

  QueryNode* pColumnDef = MetadataNode::pColumnDef(pMetadata,
                                                   szCatalog,
                                                   szSchema,
                                                   szTable,
                                                   szColumn);
  ASSERT(pColumnDef);

  eCT eType = ColumnDef::eColumnType(pColumnDef);
  ASSERT(eType > CT_NONE);
  ASSERT(eType < CT_ENUMS);

  return eType;
}

/*----------------------------------------------------------------------------*/
/* Returns the tableAliasName of the columnRef. May be null.                  */
/*----------------------------------------------------------------------------*/
Stz* ColumnRef::szTableAlias(QueryNode* pNode)
{
  QueryNode *pTableAliasNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_TABLEALIAS_PROP, pNode));

  return pTableAliasNode->szValue();
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
/* Returns the columnName of the ColumnRef                                    */
/*----------------------------------------------------------------------------*/
Stz* ColumnRef::szColumn(QueryNode* pNode)
{
  QueryNode *pColumnNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_COLUMN_PROP, pNode));

  return pColumnNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the path of the definition of this ColumnRef. Used by CTEs where   */
/* it cannot be determined (easily) from just the tablename above.            */
/*----------------------------------------------------------------------------*/
Stz* ColumnRef::szDefnPath(QueryNode* pNode)
{
  QueryNode *pDefnNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_DEFNPATH_PROP, pNode));

  ASSERT(pDefnNode);
  ASSERT(StringObject::bIsOfType(pDefnNode));

  return pDefnNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the columnRef is fully qualified (has cat/sch/tbl names    */
/* or an alias name)                                                          */
/*----------------------------------------------------------------------------*/
b1 ColumnRef::bIsFullyQualified(QueryNode* pNode)
{
  QueryNode *pTableAliasNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_TABLEALIAS_PROP, pNode));

  QueryNode *pCatalogNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_CATALOG_PROP, pNode));

  QueryNode *pSchemaNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_SCHEMA_PROP, pNode));

  QueryNode *pTableNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_TABLE_PROP, pNode));

  QueryNode *pColumnNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_COLUMN_PROP, pNode));

  ASSERT(pTableAliasNode);
  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);
  ASSERT(pColumnNode);

  Stz* szTableAliasName = pTableAliasNode->szValue();
  Stz* szCatalogName    = pCatalogNode->szValue();
  Stz* szSchemaName     = pSchemaNode->szValue();
  Stz* szTableName      = pTableNode->szValue();
  Stz* szColumnName     = pColumnNode->szValue();

  return (szTableAliasName || (szCatalogName && szSchemaName && szTableName)) && szColumnName;
}

/*----------------------------------------------------------------------------*/
/* Update the ColumnRef so that it is fully qualified                         */
/*----------------------------------------------------------------------------*/
void ColumnRef::vQualify(QueryNode* pColumnRef, Stz* szTableAliasName,
                         Stz* szCatalogName, Stz* szSchemaName,
                         Stz* szTableName, Stz* szColumnName)
{
  ASSERT(bIsOfType(pColumnRef));
  ASSERT(szTableAliasName || (szCatalogName && szSchemaName && szTableName));
  ASSERT(szColumnName);

  QueryNode* pTableAliasNode = _pQueryNode(GraphNav::pFindProperty(C_TABLEALIAS_PROP, pColumnRef));
  QueryNode* pCatalogNode    = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP,    pColumnRef));
  QueryNode* pSchemaNode     = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP,     pColumnRef));
  QueryNode* pTableNode      = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP,      pColumnRef));
  QueryNode* pColumnNode     = _pQueryNode(GraphNav::pFindProperty(C_COLUMN_PROP,     pColumnRef));

  ASSERT(pTableAliasNode);
  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);
  ASSERT(pColumnNode);

  Value xTableAliasValue;
  xTableAliasValue.vSet(szTableAliasName);
  pTableAliasNode->vSetValue(static_cast<Value&&>(xTableAliasValue));

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


/*----------------------------------------------------------------------------*/
/* Compare two expression trees and return true if they are equal with        */
/* respect to the operations and symbols used.                                */
/*----------------------------------------------------------------------------*/
b1 ColumnRef::bIsSymbolicallyEqual(QueryNode* pColRef1, QueryNode* pColRef2)
{
  // This is already validated
//  if (pExpr1 == pExpr2) return true;
//
//  // Is one of them not null and the other null?
//  if (pExpr1 && !pExpr2) return false;
//  if (!pExpr1 && pExpr2) return false;
//
//  // We don't compare names as they are allowed to be different
//  // (except in the case of properties, but see below).
//
//  // Types must match
//  if (!pExpr1->szType()->bEqual(pExpr2->szType())) return false;

  ASSERT(bIsOfType(pColRef1));
  ASSERT(bIsOfType(pColRef2));


  // Values must match. Note that this does a case sensitive comparison.
  if (pColRef1->xValue() != pColRef2->xValue()) return false;

  // All properties must match, but catalog/schema/table/column names
  // must be compared case insensitively.
  if (pColRef1->nProperties() != pColRef2->nProperties()) return false;
  ASSERT(pColRef1->nProperties() == 6);

  Stz* szStr1;
  Stz* szStr2;

  szStr1 = ColumnRef::szTableAlias(pColRef1);
  szStr2 = ColumnRef::szTableAlias(pColRef2);
  if (szStr1 || szStr2) {
    if (!szStr1 || !szStr2)  return false;
    if (!szStr1->bSame(szStr2))   return false;
  }

  szStr1 = ColumnRef::szCatalog(pColRef1);
  szStr2 = ColumnRef::szCatalog(pColRef2);
  if (szStr1 || szStr2) {
    if (!szStr1 || !szStr2)  return false;
    if (!szStr1->bSame(szStr2))   return false;
  }

  szStr1 = ColumnRef::szSchema(pColRef1);
  szStr2 = ColumnRef::szSchema(pColRef2);
  if (szStr1 || szStr2) {
    if (!szStr1 || !szStr2)  return false;
    if (!szStr1->bSame(szStr2))   return false;
  }

  szStr1 = ColumnRef::szTable(pColRef1);
  szStr2 = ColumnRef::szTable(pColRef2);
  if (szStr1 || szStr2) {
    if (!szStr1 || !szStr2)  return false;
    if (!szStr1->bSame(szStr2))   return false;
  }

  szStr1 = ColumnRef::szColumn(pColRef1);
  szStr2 = ColumnRef::szColumn(pColRef2);
  if (szStr1 || szStr2) {
    if (!szStr1 || !szStr2)  return false;
    if (!szStr1->bSame(szStr2))   return false;
  }

  // All children must match and must be in the same order.
  if (pColRef1->nChildren() != pColRef2->nChildren()) return false;
  if (pColRef1->nChildren() > 0) {
    PlanNodeIter* pIter1 = pColRef1->pNewChildrenIter();
    PlanNodeIter* pIter2 = pColRef2->pNewChildrenIter();

    while (pIter1->bHasNext()) {
      QueryNode* pChild1 = _pQueryNode(pIter1->pNext());
      QueryNode* pChild2 = _pQueryNode(pIter2->pNext());

      // Compare the children
      if (!Expr::bIsSymbolicallyEqual(pChild1, pChild2)) return false;
    }

    delete pIter1;
    delete pIter2;
  }

  // The two expressions must be symbolically equal
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
