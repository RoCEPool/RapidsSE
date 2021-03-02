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


static Stz* C_TYPE_NAMES[SqlType::ST_NENUMS] = {
    /* ST_NONE      */ "<invalid>"_Stz,
    /* ST_BOOLEAN   */ "BOOLEAN"_Stz,
    /* ST_INTEGER   */ "INTEGER"_Stz,
    /* ST_FLOAT     */ "FLOAT"_Stz,
    /* ST_DECIMAL   */ "DECIMAL"_Stz,
    /* ST_VARCHAR   */ "VARCHAR"_Stz,
    /* ST_DATE      */ "DATE"_Stz,
    /* ST_TIME      */ "TIME"_Stz,
    /* ST_TIMESTAMP */ "TIMESTAMP"_Stz,
    /* ST_INTERVAL  */ "INTERVAL YEAR TO MONTH"_Stz,
    /* ST_INTERVAL  */ "INTERVAL DAY TO SECOND"_Stz,
    /* ST_UNKNOWN   */ "NULL"_Stz
};

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create a type from enum with default properties                            */
/*----------------------------------------------------------------------------*/
QueryNode* SqlType::pCreate(Type eType)
{
  _static_

  // Manual Polymorphism
  switch (eType) {
    case ST_BOOLEAN:
      return static_cast<QueryNode*>(SqlBoolean::pCreate());

    case ST_INTEGER:
      return static_cast<QueryNode*>(SqlInteger::pCreate());

    case ST_FLOAT:
      return static_cast<QueryNode*>(SqlFloat::pCreate());

    case ST_DECIMAL:
      return static_cast<QueryNode*>(SqlDecimal::pCreate());

    case ST_VARCHAR:
      return static_cast<QueryNode*>(SqlVarchar::pCreate());

    case ST_DATE:
      return static_cast<QueryNode*>(SqlDate::pCreate());

    case ST_TIME:
      return static_cast<QueryNode*>(SqlTime::pCreate());

    case ST_TIMESTAMP:
      return static_cast<QueryNode*>(SqlTimestamp::pCreate());

    case ST_INTERVAL_YM:
      return static_cast<QueryNode*>(SqlIntervalYM::pCreate());

    case ST_INTERVAL_DT:
      return static_cast<QueryNode*>(SqlIntervalDT::pCreate());

    case ST_UNKNOWN:
      return static_cast<QueryNode*>(SqlUnknown::pCreate());

    case ST_NONE:
    case ST_NENUMS:
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

/*----------------------------------------------------------------------------*/
/* Returns the name of a SQL data type given a node                           */
/*----------------------------------------------------------------------------*/
Stz* SqlType::szTypeName(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  i8 iType = pNode->iValue();
  if (iType > ST_NONE && iType < ST_NENUMS) {
    return C_TYPE_NAMES[iType];
  }

  Type eType = SqlType::eType(pNode);
  ASSERT(eType > ST_NONE);
  ASSERT(eType < ST_NENUMS);
  return C_TYPE_NAMES[eType];
}

/*----------------------------------------------------------------------------*/
/* Returns the enum of a SQL data type given a node                           */
/*----------------------------------------------------------------------------*/
SqlType::Type SqlType::eType(PlanNode* pNode)
{
  // Manual Polymorphism
  if      (SqlBoolean::bIsOfType(pNode))    { return ST_BOOLEAN;     }
  else if (SqlInteger::bIsOfType(pNode))    { return ST_INTEGER;     }
  else if (SqlFloat::bIsOfType(pNode))      { return ST_FLOAT;       }
  else if (SqlDecimal::bIsOfType(pNode))    { return ST_DECIMAL;     }
  else if (SqlVarchar::bIsOfType(pNode))    { return ST_VARCHAR;     }
  else if (SqlDate::bIsOfType(pNode))       { return ST_DATE;        }
  else if (SqlTime::bIsOfType(pNode))       { return ST_TIME;        }
  else if (SqlTimestamp::bIsOfType(pNode))  { return ST_TIMESTAMP;   }
  else if (SqlIntervalYM::bIsOfType(pNode)) { return ST_INTERVAL_YM; }
  else if (SqlIntervalDT::bIsOfType(pNode)) { return ST_INTERVAL_DT; }
  else if (SqlUnknown::bIsOfType(pNode))    { return ST_UNKNOWN;     }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns the enum of a SQL data type given string returned from the         */
/* SE Metadata API.                                                           */
/*----------------------------------------------------------------------------*/
SqlType::Type SqlType::eType(Stz* szMetadataType)
{
  ASSERT(szMetadataType);

  if      (szMetadataType->bEqual("BOOLEAN"_Stz))         { return ST_BOOLEAN;     }
  else if (szMetadataType->bEqual("INTEGER"_Stz))         { return ST_INTEGER;     }
  else if (szMetadataType->bEqual("FLOAT"_Stz))           { return ST_FLOAT;       }
  else if (szMetadataType->bEqual("DECIMAL"_Stz))         { return ST_DECIMAL;     }
  else if (szMetadataType->bEqual("VARCHAR"_Stz))         { return ST_VARCHAR;     }
  else if (szMetadataType->bEqual("DATE"_Stz))            { return ST_DATE;        }
  else if (szMetadataType->bEqual("TIME"_Stz))            { return ST_TIME;        }
  else if (szMetadataType->bEqual("TIMESTAMP"_Stz))       { return ST_TIMESTAMP;   }
  else if (szMetadataType->bEqual("INTERVAL"_Stz))        { return ST_INTERVAL_YM; }  // TODO: EE only has one interval type
  else if (szMetadataType->bEqual("xxxINTERVAL_DT"_Stz))  { return ST_INTERVAL_DT; }  // TODO: EE only has one interval type
  else if (szMetadataType->bEqual("xxxUNKNOWN"_Stz))      { return ST_UNKNOWN;     }  // TODO: Not yet supported in the EE
  else {
    NEVER_REACH
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
