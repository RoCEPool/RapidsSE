/*============================================================================*/
/* LogicExpr.hpp - an abstract expression operator for boolean logic exprs    */
/*============================================================================*/

#include "BASE.h"
#include "LogicExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "AndExpr.hpp"
#include "OrExpr.hpp"
#include "XorExpr.hpp"
#include "NotExpr.hpp"
#include "IsNullExpr.hpp"
//#include "LikeExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LOGIC_NAME_PREFIX = "LogicExpr"_Stz;
static Stz* C_LOGIC_DEFN = "/__root__/definitions/object/expr/operator/logic"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void LogicExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (AndExpr::bIsOfType(pNode))       { AndExpr::vDescribe(pNode, sBuf);    }
  else if (OrExpr::bIsOfType(pNode))        { OrExpr::vDescribe(pNode, sBuf);     }
  else if (XorExpr::bIsOfType(pNode))       { XorExpr::vDescribe(pNode, sBuf);    }
  else if (NotExpr::bIsOfType(pNode))       { NotExpr::vDescribe(pNode, sBuf);    }
  else if (IsNullExpr::bIsOfType(pNode))    { IsNullExpr::vDescribe(pNode, sBuf); }
//  else if (LikeExpr::bIsOfType(pNode))      { LikeExpr::vDescribe(pNode, sBuf);   }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 LogicExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LOGIC_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
