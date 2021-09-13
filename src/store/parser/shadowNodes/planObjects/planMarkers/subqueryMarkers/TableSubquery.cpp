/*============================================================================*/
/* TableSubquery.hpp - Identifies a subquery coming from a WITH or FROM clause*/
/*============================================================================*/

#include "BASE.h"
#include "TableSubquery.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/planObject/planMarker/subqueryMarker/tableSubquery"_Stz;

static Stz* C_TABLEALIAS_PROP = "tableAlias"_Stz;
static Stz* C_TABLEALIAS_DEFN = "/__root__/definitions/object/valueObject/stringObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create TableSubquery nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* TableSubquery::pCreate(Stz* szTableAlias)
{
  ASSERT(szTableAlias);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  // Create the object
  QueryNode* pSubqueryNode = new QueryNode(sName, C_DEFN);

  // Create and add any properties
  Value xAliasValue(szTableAlias);
  QueryNode* pTableAliasNode = new QueryNode(C_TABLEALIAS_PROP, C_TABLEALIAS_DEFN, static_cast<Value&&>(xAliasValue));
  pSubqueryNode->vAppendProperty(pTableAliasNode);

  return pSubqueryNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TableSubquery::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_
  ASSERT(bIsOfType(pNode));

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

    // Get the table alias
    PlanNode* pTableAliasNode = GraphNav::pFindProperty(const_cast<Stz*>(C_TABLEALIAS_PROP), pNode);
    ASSERT(pTableAliasNode);
    ASSERT(pTableAliasNode->szValue());

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("table alias: %s"_Stz, pTableAliasNode->szValue());

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
/* Returns true if the subquery is correlated with respect to its parent      */
/*----------------------------------------------------------------------------*/
b1 TableSubquery::bIsCorrelated(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  // All table subqueries are non-correlated
  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns the name of the virtual table that this subquery is known as       */
/*----------------------------------------------------------------------------*/
Stz* TableSubquery::szTableAlias(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  PlanNode* pTableAliasNode = GraphNav::pFindProperty(const_cast<Stz*>(C_TABLEALIAS_PROP), pNode);
  ASSERT(pTableAliasNode);

  Stz* szTableAlias = pTableAliasNode->szValue();
  ASSERT(szTableAlias);

  return szTableAlias;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
