/*============================================================================*/
/* SqlTimestamp.hpp - an SQL timestamp data type used in the plan             */
/*============================================================================*/

#include "BASE.h"
#include "SqlTimestamp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLTIMESTAMP_DEFN = "/__root__/definitions/types/sqlType/timestamp"_Stz;
static Stz* C_TZOFFSET_PROP = "tzOffset"_Stz;
static Stz* C_TZOFFSET_DEFN = "/__root__/definitions/object/valueObject/integerObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* SqlTimestamp::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLTIMESTAMP_DEFN, sName);
  Value xTypeValue((i8) CT_TIMESTAMP);
  PlanNode* pNode = new QueryNode(sName, C_SQLTIMESTAMP_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type with timezone offset information          */
/*----------------------------------------------------------------------------*/
PlanNode* SqlTimestamp::pCreate(i8 iMinOffset)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLTIMESTAMP_DEFN, sName);
  Value xTypeValue((i8) CT_TIMESTAMP);
  QueryNode* pNode = new QueryNode(sName, C_SQLTIMESTAMP_DEFN, static_cast<Value&&>(xTypeValue));

  // Create the offset property
  Value xValue(iMinOffset);
  QueryNode* pOffset = new QueryNode(C_TZOFFSET_PROP, C_TZOFFSET_DEFN, static_cast<Value&&>(xValue));
  pNode->vAppendProperty(pOffset);

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlTimestamp::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);
  UNUSED(pRootNode);

  sBuf.vConcat("TIMESTAMP"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlTimestamp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLTIMESTAMP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlTimestamp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLTIMESTAMP_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
