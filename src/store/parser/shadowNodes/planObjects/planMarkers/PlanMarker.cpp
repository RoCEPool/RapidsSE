/*============================================================================*/
/* PlanMarker.hpp - An inert graph object that separates logical concepts     */
/*============================================================================*/

#include "BASE.h"
#include "PlanMarker.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/StatementMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/SubqueryMarker.hpp"
#include "StageMarker.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PLAN_MARKER_DEFN = "/__root__/definitions/object/planObject/planMarker"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void PlanMarker::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));


  // TODO: manual polymorphism here

  if      (StatementMarker::bIsOfType(pNode))   { StatementMarker::vDescribe(pNode, sBuf, pRootNode); }
  else if (    StageMarker::bIsOfType(pNode))   {     StageMarker::vDescribe(pNode, sBuf, pRootNode); }
  else if ( SubqueryMarker::bIsOfType(pNode))   {  SubqueryMarker::vDescribe(pNode, sBuf, pRootNode); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void PlanMarker::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here

  if      (StatementMarker::bIsOfType(pNode))   { StatementMarker::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (    StageMarker::bIsOfType(pNode))   {     StageMarker::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if ( SubqueryMarker::bIsOfType(pNode))   {  SubqueryMarker::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void PlanMarker::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object
    return;
  }


  // Manual polymorphism of subtypes here

  if      (StatementMarker::bIsOfType(szType))   { StatementMarker::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (    StageMarker::bIsOfType(szType))   {     StageMarker::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if ( SubqueryMarker::bIsOfType(pNode))    {  SubqueryMarker::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 PlanMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PLAN_MARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 PlanMarker::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_PLAN_MARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 PlanMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_PLAN_MARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 PlanMarker::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_PLAN_MARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* PlanMarker::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // PlanMarkers don't change the columns being returned. So ask its descendant.
  ASSERT(pNode->iNumChildren() == 1);
  return PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pRootNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
