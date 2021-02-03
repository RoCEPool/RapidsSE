/*============================================================================*/
/* ComparisonExpr.hpp - an abstract expression operator for comparing exprs   */
/*============================================================================*/

#include "BASE.h"
#include "ComparisonExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "EqExpr.hpp"
#include "NeExpr.hpp"
#include "GtExpr.hpp"
#include "GeExpr.hpp"
#include "LtExpr.hpp"
#include "LeExpr.hpp"
//#include "BetweenExpr.hpp"
//#include "InExpr.hpp"
//#include "AnyExpr.hpp"
//#include "ExistsExpr.hpp"
//#include "LikeExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COMPARISON_NAME_PREFIX = "ComparisonExpr"_Stz;
static Stz* C_COMPARISON_DEFN = "/__root__/definitions/object/expr/operator/comparison"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ComparisonExpr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if      (EqExpr::bIsOfType(pNode))        { EqExpr::vDescribe(pNode, sBuf);       }
  else if (NeExpr::bIsOfType(pNode))        { NeExpr::vDescribe(pNode, sBuf);       }
  else if (GtExpr::bIsOfType(pNode))        { GtExpr::vDescribe(pNode, sBuf);       }
  else if (GeExpr::bIsOfType(pNode))        { GeExpr::vDescribe(pNode, sBuf);       }
  else if (LtExpr::bIsOfType(pNode))        { LtExpr::vDescribe(pNode, sBuf);       }
  else if (LeExpr::bIsOfType(pNode))        { LeExpr::vDescribe(pNode, sBuf);       }
//  else if (BetweenExpr::bIsOfType(pNode))   { BetweenExpr::vDescribe(pNode, sBuf);  }
//  else if (InExpr::bIsOfType(pNode))        { InExpr::vDescribe(pNode, sBuf);       }
//  else if (AnyExpr::bIsOfType(pNode))       { AnyExpr::vDescribe(pNode, sBuf);      }
//  else if (ExistsExpr::bIsOfType(pNode))    { ExistsExpr::vDescribe(pNode, sBuf);   }
//  else if (LikeExpr::bIsOfType(pNode))      { LikeExpr::vDescribe(pNode, sBuf);     }
  else NEVER_REACH
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ComparisonExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COMPARISON_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
