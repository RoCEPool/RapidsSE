/*============================================================================*/
/* Types.hpp - an abstract base class of custom types used in the plan        */
/*============================================================================*/

#include "BASE.h"
#include "Types.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "SqlType.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TYPES_DEFN = "/__root__/definitions/types"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Types::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  // Statement
  if      (SqlType::bIsOfType(pNode))     { SqlType::vDescribe(pNode, sBuf, pRootNode);    }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Types::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TYPES_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Types::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TYPES_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
