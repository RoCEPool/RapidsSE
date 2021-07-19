/*============================================================================*/
/* ExplainMarker.hpp - identifies the subgraph that follows is a SELECT stmt   */
/*============================================================================*/

#include "BASE.h"
#include "ExplainMarker.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/StatementMarker.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/planObject/planMarker/statementMarker/dml/explain"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create statement nodes. These are the top level         */
/* "headings" in a plan graph that identify what comes below them.            */
/*----------------------------------------------------------------------------*/
QueryNode* ExplainMarker::pCreate(Stz* szQueryText)
{
  // Create it in the base class
  QueryNode* pMarker = StatementMarker::pCreate(ST_EXPLAIN, C_DEFN, szQueryText);
  return pMarker;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 ExplainMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 ExplainMarker::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExplainMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExplainMarker::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void ExplainMarker::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s256 sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object
    return;
  }


  // Manual polymorphism of subtypes here
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
