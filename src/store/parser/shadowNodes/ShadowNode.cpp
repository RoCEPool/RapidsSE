/*============================================================================*/
/* ShadowNode.cpp - a base class of all logical graph nodes                   */
/*============================================================================*/

#include "BASE.h"
#include "ShadowNode.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/StatementMarker.hpp"
#include "store/parser/shadowNodes/compound/TableRef.hpp"
#include "Expr.hpp"
#include "store/parser/shadowNodes/compound/ProjectedColumn.hpp"
#include "ShadowRoot.hpp"

#include "GraphValidator.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// Static
Atomic* ShadowNode::ms_pObjId = new Atomic();

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Validates that a node is correctly built                                   */
/* The fTypeValidator param is expected to be something like the              */
/* bIsOfType() function.                                                      */
/*----------------------------------------------------------------------------*/
b1 ShadowNode::bValidate(QueryNode* pQueryNode, PlanNode* pPlanRoot,
                         std::function<b1(PlanNode*)> fTypeValidator)
{
  ASSERT(pQueryNode);
  ASSERT(pPlanRoot);
  UNUSED(pPlanRoot);

  // Validate that this node is of the expected type
  fTypeValidator(pQueryNode);

#ifdef WITH_ASSERT_MACRO
  GraphValidator::vValidatePlanNode(pQueryNode, pPlanRoot);
#endif

  return true;
}

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void ShadowNode::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  // manual polymorphism here

  if      (PlanObject::bIsOfType(pNode))      {      PlanObject::vDescribe(pNode, sBuf, pRootNode); }
  else if (Expr::bIsOfType(pNode))            {            Expr::vDescribe(pNode, sBuf, pRootNode); }
  else if (TableRef::bIsOfType(pNode))        {        TableRef::vDescribe(pNode, sBuf, pRootNode); }
  else if (ProjectedColumn::bIsOfType(pNode)) { ProjectedColumn::vDescribe(pNode, sBuf, pRootNode); }
  else if (ShadowRoot::bIsOfType(pNode))      {      ShadowRoot::vDescribe(pNode, sBuf, pRootNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }

}

/*----------------------------------------------------------------------------*/
/* Returns true if a given node is a given type or is derived from that type. */
/* e.g. a node with the following type:                                       */
/*    /__root__/definitions/object/expr/literal/string                        */
/* will return true when compared to the following type path:                 */
/*    /__root__/definitions/object/expr                                       */
/*----------------------------------------------------------------------------*/
b1 ShadowNode::bIsOfType(PlanNode* pNode, Stz* szTypePath)
{
  ASSERT(pNode);
  ASSERT(pNode->szType());

  return bIsOfType(pNode->szType(), szTypePath);
}

/*----------------------------------------------------------------------------*/
/* Returns true if a given node is a given type or is derived from that type. */
/* e.g. a node with the following type:                                       */
/*    /__root__/definitions/object/expr/literal/string                        */
/* will return true when compared to the following type path:                 */
/*    /__root__/definitions/object/expr                                       */
/*----------------------------------------------------------------------------*/
b1 ShadowNode::bIsOfType(Stz* szNodeType, Stz* szTypePath)
{
  ASSERT(szNodeType);
  ASSERT(szNodeType->nLength() > 0);
  ASSERT(szTypePath);
  ASSERT(szTypePath->nLength() > 0);

  // Check that the type field of the given node is either equal to the
  // type path given, or the type field of the given node begins with
  // the type path given followed by a '/'. This indicates that it is a
  // child of that type. It also avoids false comparisons. e.g. a "timestamp"
  // type can be confused with a "time" type because "timestamp" begins
  // with "time".

  b1 bResult = (
      szNodeType->bEqual(szTypePath) ||
      (
          szNodeType->bBeginsWith(szTypePath)            &&
          szNodeType->nLength() > szTypePath->nLength()  &&
          szNodeType->azChar()[szTypePath->nLength()] == '/'
      )
  );

  return bResult;
}

/*----------------------------------------------------------------------------*/
/* Returns true if a given node is of an exact given type only.               */
/* e.g. a node with the following type:                                       */
/*    /__root__/definitions/object/expr/literal/string                        */
/* will return false when compared to the following type path:                */
/*    /__root__/definitions/object/expr                                       */
/*----------------------------------------------------------------------------*/
b1 ShadowNode::bIsExactType(PlanNode* pNode, Stz* szTypePath)
{
  ASSERT(pNode);
  ASSERT(szTypePath);
  ASSERT(szTypePath->nLength() > 0);

  return pNode->szType()->bEqual(szTypePath);
}

/*----------------------------------------------------------------------------*/
/* Returns true if a given node is of an exact given type only.               */
/* e.g. a node with the following type:                                       */
/*    /__root__/definitions/object/expr/literal/string                        */
/* will return false when compared to the following type path:                */
/*    /__root__/definitions/object/expr                                       */
/*----------------------------------------------------------------------------*/
b1 ShadowNode::bIsExactType(Stz* szNodeType, Stz* szTypePath)
{
  ASSERT(szNodeType);
  ASSERT(szTypePath);
  ASSERT(szTypePath->nLength() > 0);

  return szNodeType->bEqual(szTypePath);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
