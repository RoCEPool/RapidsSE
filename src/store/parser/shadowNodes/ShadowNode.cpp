/*============================================================================*/
/* ShadowNode.cpp - a base class of all logical graph nodes                   */
/*============================================================================*/

#include "BASE.h"
#include "ShadowNode.hpp"

#include "PlanNode.hpp"

#include "store/parser/shadowNodes/planObjects/Statement.hpp"
#include "TableRef.hpp"
#include "Expr.hpp"
#include "ProjectedExpr.hpp"
#include "ShadowRoot.hpp"

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
/*----------------------------------------------------------------------------*/
b1 ShadowNode::bValidate(PlanNode* pPlanNode)
{
  ASSERT(pPlanNode);

  Stz* szDefnPath = pPlanNode->szType();
  ASSERT(szDefnPath);
  ASSERT(szDefnPath->nLength() > 0);

  // TODO: Get the root of the graph
//  PlanNode* pDefn = pFind(szDefnPath, pRoot);

  return true;
}

/*----------------------------------------------------------------------------*/
/* Creates a unique ID for an object name so that the names of siblings       */
/* do not conflict and are unique.                                            */
/*----------------------------------------------------------------------------*/
void ShadowNode::vObjName(Stz *szPrefix, s256 &sBuf)
{
  ASSERT(szPrefix);

  sBuf.vSprint("%s%04lld"_Stz, szPrefix, ms_pObjId->iInc());
}

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void ShadowNode::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  // manual polymorphism here

  // Statements
  if      (PlanObject::bIsOfType(pNode))      { PlanObject::vDescribe(pNode, sBuf); }

  // Expressions
  else if (Expr::bIsOfType(pNode))            { Expr::vDescribe(pNode, sBuf);      }

  // TableRefs
  else if (TableRef::bIsOfType(pNode))        { TableRef::vDescribe(pNode, sBuf);  }

  // ProjectedExpr
  else if (ProjectedExpr::bIsOfType(pNode))   { ProjectedExpr::vDescribe(pNode, sBuf);  }

  // Root node
  else if (ShadowRoot::bIsOfType(pNode))        { ShadowRoot::vDescribe(pNode, sBuf); }

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
  ASSERT(szTypePath);
  ASSERT(szTypePath->nLength() > 0);

  return pNode->szType()->bBeginsWith(szTypePath);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
