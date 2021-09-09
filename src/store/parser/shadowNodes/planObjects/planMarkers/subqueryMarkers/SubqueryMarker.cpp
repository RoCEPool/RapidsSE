/*============================================================================*/
/* SubqueryMarker.hpp - Marks the start of some type of subquery              */
/*============================================================================*/

#include "BASE.h"
#include "SubqueryMarker.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

#include "MainSubquery.hpp"
#include "TableSubquery.hpp"
#include "ExprSubquery.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_DEFN = "/__root__/definitions/object/planObject/planMarker/subqueryMarker"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SubqueryMarker::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism of subtypes here
  if      (  MainSubquery::bIsOfType(pNode)) {  MainSubquery::vDescribe(pNode, sBuf); }
  else if ( TableSubquery::bIsOfType(pNode)) { TableSubquery::vDescribe(pNode, sBuf); }
  else if (  ExprSubquery::bIsOfType(pNode)) {  ExprSubquery::vDescribe(pNode, sBuf); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SubqueryMarker::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism of subtypes here
  if      (  MainSubquery::bIsOfType(pNode)) {  MainSubquery::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if ( TableSubquery::bIsOfType(pNode)) { TableSubquery::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (  ExprSubquery::bIsOfType(pNode)) {  ExprSubquery::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SubqueryMarker::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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
  if      (  MainSubquery::bIsOfType(szType)) {  MainSubquery::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);    }
  else if ( TableSubquery::bIsOfType(szType)) { TableSubquery::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);    }
  else if (  ExprSubquery::bIsOfType(szType)) {  ExprSubquery::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);    }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the subquery is correlated with respect to its parent      */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsCorrelated(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism of subtypes here
  if      (  MainSubquery::bIsOfType(pNode)) { return  MainSubquery::bIsCorrelated(pNode); }
  else if ( TableSubquery::bIsOfType(pNode)) { return TableSubquery::bIsCorrelated(pNode); }
  else if (  ExprSubquery::bIsOfType(pNode)) { return  ExprSubquery::bIsCorrelated(pNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
