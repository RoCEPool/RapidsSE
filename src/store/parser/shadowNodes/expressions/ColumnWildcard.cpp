/*============================================================================*/
/* ColumnWildcard.hpp - an expression representing one or more columns        */
/* This object will only exist temporarily until all tables in the plan have  */
/* been resolved. Then this will be replaced by a set of columnWildcards instead.  */
/*============================================================================*/

#include "BASE.h"
#include "ColumnWildcard.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "NodeNamer.hpp"

#include "ShadowRoot.hpp"
#include "MetadataNode.hpp"
#include "ColumnDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COLWILDCARD_DEFN = "/__root__/definitions/object/expr/columnWildcard"_Stz;

static Stz* C_COLUMNWILDCARD_NAME_PREFIX = "ExprColWildcard"_Stz;

static Stz* C_CATALOG_PROP = "catalogName"_Stz;
static Stz* C_SCHEMA_PROP  = "schemaName"_Stz;
static Stz* C_TABLE_PROP   = "tableName"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create expression column reference nodes.               */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnWildcard::pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_COLWILDCARD_DEFN, sName);

  // These Value instances will take ownership of the Text*s.
  Value xCatalogName(pCatalogName);
  Value xSchemaName(pSchemaName);
  Value xTableName(pTableName);

  QueryNode* pColWCard = new QueryNode(sName, C_COLWILDCARD_DEFN);
  pColWCard->vAppendProperty(new QueryNode(C_CATALOG_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xCatalogName)));
  pColWCard->vAppendProperty(new QueryNode(C_SCHEMA_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xSchemaName)));
  pColWCard->vAppendProperty(new QueryNode(C_TABLE_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xTableName)));

  return pColWCard;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ColumnWildcard::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pColumnWildcard = _pQueryNode(pNode);

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pColumnWildcard));
  QueryNode* pSchemaNode  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP, pColumnWildcard));
  QueryNode* pTableNode   = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP, pColumnWildcard));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);

  Stz* szCatalogName = pCatalogNode->szValue();
  Stz* szSchemaName  = pSchemaNode->szValue();
  Stz* szTableName   = pTableNode->szValue();

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

  sBuf.vAppend('*');
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ColumnWildcard::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COLWILDCARD_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnWildcard::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_COLWILDCARD_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId ColumnWildcard::xTypeId(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);
  UNUSED(pRootNode);

  // This should never be called. The ColumnWildcard should instead be replaced
  // with specific ColumnRefs once the tables in the plan have been resolved.
  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns the catalogName of the columnWildcard. May be null.                */
/*----------------------------------------------------------------------------*/
Stz* ColumnWildcard::szCatalog(QueryNode* pNode)
{
  QueryNode *pCatalogNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_CATALOG_PROP, pNode));

  return pCatalogNode->szValue();
}


/*----------------------------------------------------------------------------*/
/* Returns the schemaName of the columnWildcard. May be null.                 */
/*----------------------------------------------------------------------------*/
Stz* ColumnWildcard::szSchema(QueryNode* pNode)
{
  QueryNode *pSchemaNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_SCHEMA_PROP, pNode));

  return pSchemaNode->szValue();
}


/*----------------------------------------------------------------------------*/
/* Returns the tableName of the columnWildcard. May be null.                  */
/*----------------------------------------------------------------------------*/
Stz* ColumnWildcard::szTable(QueryNode* pNode)
{
  QueryNode *pTableNode = static_cast<QueryNode *>(
      GraphNav::pFindProperty(C_TABLE_PROP, pNode));

  return pTableNode->szValue();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
