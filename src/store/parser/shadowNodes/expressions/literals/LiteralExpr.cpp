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
#include "eCT.h"

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
void LiteralExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (   StringLiteral::bIsOfType(pNode))  {    StringLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (  BooleanLiteral::bIsOfType(pNode))  {   BooleanLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (  IntegerLiteral::bIsOfType(pNode))  {   IntegerLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (    FloatLiteral::bIsOfType(pNode))  {     FloatLiteral::vDescribe(pNode, sBuf, pRootNode); }
//else if (DecimalLiteral::bIsOfType(pNode))    {   DecimalLiteral::vDescribe(pNode, sBuf, pRootNode);    }
  else if (     DateLiteral::bIsOfType(pNode))  {      DateLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (     TimeLiteral::bIsOfType(pNode))  {      TimeLiteral::vDescribe(pNode, sBuf, pRootNode); }
  else if (TimestampLiteral::bIsOfType(pNode))  { TimestampLiteral::vDescribe(pNode, sBuf, pRootNode); }
//else if (IntervalLiteral::bIsOfType(pNode))   {  IntervalLiteral::vDescribe(pNode, sBuf, pRootNode);    }
  else if (     NullLiteral::bIsOfType(pNode))  {      NullLiteral::vDescribe(pNode, sBuf, pRootNode); }
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

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 LiteralExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT LiteralExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pRootNode);

  if      (      StringLiteral::bIsOfType(pExprNode))  { return CT_VARCHAR;     }
  else if (     BooleanLiteral::bIsOfType(pExprNode))  { return CT_BOOLEAN;     }
  else if (     IntegerLiteral::bIsOfType(pExprNode))  { return CT_INTEGER;     }
  else if (       FloatLiteral::bIsOfType(pExprNode))  { return CT_FLOAT;       }
//  else if    (DecimalLiteral::bIsOfType(pExprNode))  { return CT_DECIMAL;     }
  else if (        DateLiteral::bIsOfType(pExprNode))  { return CT_DATE;        }
  else if (        TimeLiteral::bIsOfType(pExprNode))  { return CT_TIME;        }
  else if (   TimestampLiteral::bIsOfType(pExprNode))  { return CT_TIMESTAMP;   }
//  else if (IntervalYMLiteral::bIsOfType(pExprNode))  { return CT_INTERVAL_YM; }
//  else if (IntervalDTLiteral::bIsOfType(pExprNode))  { return CT_INTERVAL_DT; }
  else if (      NullLiteral::bIsOfType(pExprNode))    { return CT_UNKNOWN;     }
  else NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
