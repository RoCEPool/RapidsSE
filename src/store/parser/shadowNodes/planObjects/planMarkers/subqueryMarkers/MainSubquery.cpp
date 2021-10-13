/*============================================================================*/
/* MainSubquery.hpp - Identifies the primary subquery to be executed          */
/*============================================================================*/

#include "BASE.h"
#include "MainSubquery.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/planObject/planMarker/subqueryMarker/mainSubquery"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create MainSubquery nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* MainSubquery::pCreate(Stz* szSqlText, Stz* szTableAlias)
{
  // Create the object
  QueryNode* pSubquery = SubqueryMarker::pCreate(C_DEFN, szSqlText, szTableAlias);

  // Create and add any properties

  return pSubquery;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void MainSubquery::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pStmt = _pQueryNode(pNode);

  buf.vConcat(GraphNav::szLeafType(pStmt->szType()));
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain to the buf             */
/*----------------------------------------------------------------------------*/
void MainSubquery::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
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
void MainSubquery::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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


    return;
  }

  // Manual polymorphism of subtypes here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 MainSubquery::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 MainSubquery::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 MainSubquery::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 MainSubquery::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
