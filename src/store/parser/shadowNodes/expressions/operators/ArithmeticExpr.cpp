/*============================================================================*/
/* ArithmeticExpr.hpp - an arithmetic expression operator                     */
/*============================================================================*/

#include "BASE.h"
#include "ArithmeticExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "AdditionExpr.hpp"
#include "SubtractionExpr.hpp"
#include "MultiplicationExpr.hpp"
#include "DivisionExpr.hpp"
#include "ModulusExpr.hpp"
#include "NegationExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ARITHMETIC_NAME_PREFIX = "ArithmeticExpr"_Stz;
static Stz* C_ARITHMETIC_DEFN = "/__root__/definitions/object/expr/operator/arithmetic"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ArithmeticExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (AdditionExpr::bIsOfType(pNode))        { AdditionExpr::vDescribe(pNode, sBuf);       }
  else if (SubtractionExpr::bIsOfType(pNode))     { SubtractionExpr::vDescribe(pNode, sBuf);    }
  else if (MultiplicationExpr::bIsOfType(pNode))  { MultiplicationExpr::vDescribe(pNode, sBuf); }
  else if (DivisionExpr::bIsOfType(pNode))        { DivisionExpr::vDescribe(pNode, sBuf);       }
  else if (ModulusExpr::bIsOfType(pNode))         { ModulusExpr::vDescribe(pNode, sBuf);        }
  else if (NegationExpr::bIsOfType(pNode))        { NegationExpr::vDescribe(pNode, sBuf);       }
//  else if (PowerExpr::bIsOfType(pNode))           { PowerExpr::vDescribe(pNode, sBuf);          }
//  else if (LogExpr::bIsOfType(pNode))             { LogExpr::vDescribe(pNode, sBuf);            }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ArithmeticExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ARITHMETIC_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
