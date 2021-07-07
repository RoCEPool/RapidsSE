/*============================================================================*/
/* DmlMarker.hpp - identifies the subgraph that follows is a DML statement    */
/*============================================================================*/

#include "BASE.h"
#include "DmlMarker.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "SelectMarker.hpp"
#include "ExplainMarker.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/planObject/planMarker/statementMarker/dml"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 DmlMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 DmlMarker::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 DmlMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 DmlMarker::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void DmlMarker::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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
  if      ( SelectMarker::bIsOfType(szType)) {  SelectMarker::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);    }
  else if (ExplainMarker::bIsOfType(szType)) { ExplainMarker::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
