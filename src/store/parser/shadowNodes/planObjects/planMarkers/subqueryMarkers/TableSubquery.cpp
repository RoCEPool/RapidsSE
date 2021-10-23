/*============================================================================*/
/* TableSubquery.hpp - Identifies a subquery coming from a WITH or FROM clause*/
/*============================================================================*/

#include "BASE.h"
#include "TableSubquery.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "IdList.hpp"
#include "StringObject.hpp"
#include "MainSubquery.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/planObject/planMarker/subqueryMarker/tableSubquery"_Stz;

static Stz* C_TABLEALIAS_PROP = "tableAlias"_Stz;
static Stz* C_TABLEALIAS_DEFN = "/__root__/definitions/object/valueObject/stringObject"_Stz;

static Stz* C_COLUMNNAMES_PROP = "columnNames"_Stz;
static Stz* C_COLUMNNAMES_DEFN = "/__root__/definitions/object/list/idList"_Stz;

static Stz* C_ISRECURSIVE_PROP = "isRecursive"_Stz;
static Stz* C_ISRECURSIVE_DEFN = "/__root__/definitions/object/valueObject/booleanObject"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create TableSubquery nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* TableSubquery::pCreate(Text* pTableAlias, Stz* szSqlText, QueryNode* pColumnNames, b1 bIsRecursive)
{
  ASSERT(pTableAlias);
  ASSERT(pColumnNames);
  ASSERT(IdList::bIsOfType(pColumnNames));

  // Create the object
  QueryNode* pSubqueryNode = SubqueryMarker::pCreate(C_DEFN, szSqlText, pTableAlias->szGet());
  delete pTableAlias;

  // Create and add any properties

  pColumnNames->vSetName(C_COLUMNNAMES_PROP);
  pSubqueryNode->vAppendProperty(pColumnNames);

  Value xIsRecursiveValue(bIsRecursive);
  QueryNode* pIsRecursiveNode = new QueryNode(C_ISRECURSIVE_PROP, C_ISRECURSIVE_DEFN, static_cast<Value&&>(xIsRecursiveValue));
  pSubqueryNode->vAppendProperty(pIsRecursiveNode);

  return pSubqueryNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TableSubquery::vDescribe(PlanNode* pNode, s2K& buf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pStmt = _pQueryNode(pNode);

  buf.vConcat(GraphNav::szLeafType(pStmt->szType()));
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain to the buf             */
/*----------------------------------------------------------------------------*/
void TableSubquery::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, pExplainNode, sBuf, iIndent + 1);
    }

    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void TableSubquery::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Location is explained in the common attributes above
    // Explain any local attributes of this object

    // Named columns.
    QueryNode* pColumnNamesNode = pColumnNames(_pQueryNode(pNode));
    ASSERT(pColumnNamesNode);
    if (pColumnNamesNode->nChildren() > 0) {
      s2K sColumnBuf;
      IdList::vDescribe(pColumnNamesNode, sColumnBuf, pRootNode);

      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("named columns: (%s)"_Stz, sColumnBuf.szStz());
    }

    // isRecursive
    b1 bIsRecursive = TableSubquery::bIsRecursive(_pQueryNode(pNode));
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("recursive: %s"_Stz, bIsRecursive ? "true"_Stz : "false"_Stz);

    return;
  }

  // Manual polymorphism of subtypes here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 TableSubquery::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 TableSubquery::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableSubquery::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableSubquery::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* TableSubquery::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TableSubqueries don't change the columns being returned. But instead of asking
  // its first child, there might be more nested table subqueries. Instead we need
  // to ask the main subquery child of this node instead.

  auto fCondition = [](PlanNode* pItem) -> b1 { return MainSubquery::bIsOfType(pItem); };

  QueryNode* pMainSubquery = _pQueryNode(GraphNav::pFindChild( fCondition, pNode));
  ASSERT(pMainSubquery);
  ASSERT(MainSubquery::bIsOfType(pMainSubquery));

  return MainSubquery::pColumnExprs(pMainSubquery, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the internal node that contains the list of column names           */
/*----------------------------------------------------------------------------*/
QueryNode* TableSubquery::pColumnNames(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnNamesNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_COLUMNNAMES_PROP), pNode));
  ASSERT(pColumnNamesNode);

  return pColumnNamesNode;
}

/*----------------------------------------------------------------------------*/
/* Returns the number of named column names                                   */
/*----------------------------------------------------------------------------*/
i8 TableSubquery::nColumnNames(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnNamesNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_COLUMNNAMES_PROP), pNode));
  ASSERT(pColumnNamesNode);

  return pColumnNamesNode->nChildren();
}

/*----------------------------------------------------------------------------*/
/* Returns true if this subquery has been defined to be recursive             */
/*----------------------------------------------------------------------------*/
b1 TableSubquery::bIsRecursive(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pRecursiveNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_ISRECURSIVE_PROP), pNode));
  ASSERT(pRecursiveNode);

  return pRecursiveNode->bValue();
}

/*----------------------------------------------------------------------------*/
/* Find and return the main subquery node that exists underneath this table   */
/* subquery node.                                                             */
/*----------------------------------------------------------------------------*/
QueryNode* TableSubquery::pChildMainSubquery(QueryNode *pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  auto fCondition = [](PlanNode* pItem) -> b1 {
    return MainSubquery::bIsOfType(pItem);
  };

  QueryNode* pMainSubquery = _pQueryNode(GraphNav::pFindChild(fCondition, pNode));
  ASSERT(pMainSubquery);
  ASSERT(MainSubquery::bIsOfType(pMainSubquery));

  return pMainSubquery;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
