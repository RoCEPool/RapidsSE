/*============================================================================*/
/* SqlType.hpp - an abstract base class of SQL data sqlType used in the plan    */
/*============================================================================*/

#include "BASE.h"
#include "SqlType.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "SqlBoolean.hpp"
#include "SqlInteger.hpp"
#include "SqlFloat.hpp"
#include "SqlDecimal.hpp"
#include "SqlVarchar.hpp"
#include "SqlDate.hpp"
#include "SqlTime.hpp"
#include "SqlTimestamp.hpp"
#include "SqlIntervalYM.hpp"
#include "SqlIntervalDT.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLTYPE_DEFN = "/__root__/definitions/types/sqlType"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlType::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // Manual Polymorphism
  if      (SqlBoolean::bIsOfType(pNode))    { SqlBoolean::vDescribe(pNode, sBuf);    }
  else if (SqlInteger::bIsOfType(pNode))    { SqlInteger::vDescribe(pNode, sBuf);    }
  else if (SqlFloat::bIsOfType(pNode))      { SqlFloat::vDescribe(pNode, sBuf);      }
  else if (SqlDecimal::bIsOfType(pNode))    { SqlDecimal::vDescribe(pNode, sBuf);    }
  else if (SqlVarchar::bIsOfType(pNode))    { SqlVarchar::vDescribe(pNode, sBuf);    }
  else if (SqlDate::bIsOfType(pNode))       { SqlDate::vDescribe(pNode, sBuf);       }
  else if (SqlTime::bIsOfType(pNode))       { SqlTime::vDescribe(pNode, sBuf);       }
  else if (SqlTimestamp::bIsOfType(pNode))  { SqlTimestamp::vDescribe(pNode, sBuf);  }
  else if (SqlIntervalYM::bIsOfType(pNode)) { SqlIntervalYM::vDescribe(pNode, sBuf); }
  else if (SqlIntervalDT::bIsOfType(pNode)) { SqlIntervalDT::vDescribe(pNode, sBuf); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlType::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLTYPE_DEFN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
