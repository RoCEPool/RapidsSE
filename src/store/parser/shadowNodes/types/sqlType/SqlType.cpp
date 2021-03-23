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
#include "SqlUnknown.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLTYPE_DEFN = "/__root__/definitions/types/sqlType"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create a type from enum with default properties                            */
/*----------------------------------------------------------------------------*/
QueryNode* SqlType::pCreate(eCT eType)
{
  _static_

  // Manual Polymorphism
  switch (eType) {
    case CT_BOOLEAN:
      return static_cast<QueryNode*>(SqlBoolean::pCreate());

    case CT_CHAR:
      return static_cast<QueryNode*>(SqlVarchar::pCreate());

    case CT_DATE:
      return static_cast<QueryNode*>(SqlDate::pCreate());

    case CT_DECIMAL:
      return static_cast<QueryNode*>(SqlDecimal::pCreate());

    case CT_FLOAT:
      return static_cast<QueryNode*>(SqlFloat::pCreate());

    case CT_INTEGER:
      return static_cast<QueryNode*>(SqlInteger::pCreate());

    case CT_INTERVAL_DT:
      return static_cast<QueryNode*>(SqlIntervalDT::pCreate());

    case CT_INTERVAL_YM:
      return static_cast<QueryNode*>(SqlIntervalYM::pCreate());

    case CT_TIME:
      return static_cast<QueryNode*>(SqlTime::pCreate());

    case CT_TIMESTAMP:
      return static_cast<QueryNode*>(SqlTimestamp::pCreate());

    case CT_VARCHAR:
      return static_cast<QueryNode*>(SqlVarchar::pCreate());

    case CT_UNKNOWN:
      return static_cast<QueryNode*>(SqlUnknown::pCreate());

    case CT_NONE:
    case CT_ENUMS:
    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlType::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (SqlBoolean::bIsOfType(pNode))    {    SqlBoolean::vDescribe(pNode, sBuf); }
  else if (SqlInteger::bIsOfType(pNode))    {    SqlInteger::vDescribe(pNode, sBuf); }
  else if (SqlFloat::bIsOfType(pNode))      {      SqlFloat::vDescribe(pNode, sBuf); }
  else if (SqlDecimal::bIsOfType(pNode))    {    SqlDecimal::vDescribe(pNode, sBuf); }
  else if (SqlVarchar::bIsOfType(pNode))    {    SqlVarchar::vDescribe(pNode, sBuf); }
  else if (SqlDate::bIsOfType(pNode))       {       SqlDate::vDescribe(pNode, sBuf); }
  else if (SqlTime::bIsOfType(pNode))       {       SqlTime::vDescribe(pNode, sBuf); }
  else if (SqlTimestamp::bIsOfType(pNode))  {  SqlTimestamp::vDescribe(pNode, sBuf); }
  else if (SqlIntervalDT::bIsOfType(pNode)) { SqlIntervalDT::vDescribe(pNode, sBuf); }
  else if (SqlIntervalYM::bIsOfType(pNode)) { SqlIntervalYM::vDescribe(pNode, sBuf); }
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

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlType::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLTYPE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the name of a SQL data type given a node                           */
/*----------------------------------------------------------------------------*/
Stz* SqlType::szTypeName(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  i8 iType = pNode->iValue();
  if (iType > CT_NONE && iType < CT_ENUMS) {
    return aCT[iType];
  }

  eCT eType = SqlType::eType(pNode);
  ASSERT(eType > CT_NONE);
  ASSERT(eType < CT_ENUMS);
  return aCT[eType];
}

/*----------------------------------------------------------------------------*/
/* Returns the enum of a SQL data type given a node                           */
/*----------------------------------------------------------------------------*/
eCT SqlType::eType(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  i8 iType = pNode->iValue();
  if (iType > CT_NONE && iType < CT_ENUMS) {
    return static_cast<eCT>(iType);
  }

  // Manual Polymorphism
  if      (   SqlBoolean::bIsOfType(pNode))   { return CT_BOOLEAN;     }
  else if (   SqlInteger::bIsOfType(pNode))   { return CT_INTEGER;     }
  else if (     SqlFloat::bIsOfType(pNode))   { return CT_FLOAT;       }
  else if (   SqlDecimal::bIsOfType(pNode))   { return CT_DECIMAL;     }
  else if (   SqlVarchar::bIsOfType(pNode))   { return CT_VARCHAR;     }
  else if (      SqlDate::bIsOfType(pNode))   { return CT_DATE;        }
  else if (      SqlTime::bIsOfType(pNode))   { return CT_TIME;        }
  else if ( SqlTimestamp::bIsOfType(pNode))   { return CT_TIMESTAMP;   }
  else if (SqlIntervalDT::bIsOfType(pNode))   { return CT_INTERVAL_DT; }
  else if (SqlIntervalYM::bIsOfType(pNode))   { return CT_INTERVAL_YM; }
  else if (   SqlUnknown::bIsOfType(pNode))   { return CT_UNKNOWN;     }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
