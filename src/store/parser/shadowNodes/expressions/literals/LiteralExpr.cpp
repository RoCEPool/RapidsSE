/*============================================================================*/
/* LiteralExpr.cpp - a literal expression                                     */
/*============================================================================*/

#include "BASE.h"
#include "LiteralExpr.hpp"
#include "StringLiteral.hpp"
#include "IntegerLiteral.hpp"
#include "FloatLiteral.hpp"
#include "BooleanLiteral.hpp"
#include "DateLiteral.hpp"
#include "TimeLiteral.hpp"
#include "TimestampLiteral.hpp"
#include "NullLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LITERAL_NAME_PREFIX = "ExprLiteral"_Stz;
static Stz* C_LITERAL_DEFN = "/__root__/definitions/object/expr/literal"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void LiteralExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (StringLiteral::bIsOfType(pNode))     { StringLiteral::vDescribe(pNode, sBuf);    }
  else if (BooleanLiteral::bIsOfType(pNode))    { BooleanLiteral::vDescribe(pNode, sBuf);   }
  else if (IntegerLiteral::bIsOfType(pNode))    { IntegerLiteral::vDescribe(pNode, sBuf);   }
  else if (FloatLiteral::bIsOfType(pNode))      { FloatLiteral::vDescribe(pNode, sBuf);     }
//  else if (DecimalLiteral::bIsOfType(pNode))    { DecimalLiteral::vDescribe(pNode, sBuf);     }
  else if (DateLiteral::bIsOfType(pNode))       { DateLiteral::vDescribe(pNode, sBuf);      }
  else if (TimeLiteral::bIsOfType(pNode))       { TimeLiteral::vDescribe(pNode, sBuf);      }
  else if (TimestampLiteral::bIsOfType(pNode))  { TimestampLiteral::vDescribe(pNode, sBuf); }
//  else if (IntervalLiteral::bIsOfType(pNode))   { IntervalLiteral::vDescribe(pNode, sBuf);    }
  else if (NullLiteral::bIsOfType(pNode))       { NullLiteral::vDescribe(pNode, sBuf);      }
  else NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Returns true if the node has a null value. NB: this is disinct from        */
/* the node representing a NULL literal.                                      */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsNull(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  return pNode->bIsNullValue();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LITERAL_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
