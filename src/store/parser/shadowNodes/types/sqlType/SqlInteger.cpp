/*============================================================================*/
/* SqlFloat.hpp - an SQL approximate-numeric float data type used in the plan */
/*============================================================================*/

#include "BASE.h"
#include "SqlInteger.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLINTEGER_DEFN = "/__root__/definitions/types/sqlType/integer"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* SqlInteger::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_SQLINTEGER_DEFN), sName);
  Value xTypeValue((i8) ST_INTEGER);
  PlanNode* pNode = new QueryNode(sName, C_SQLINTEGER_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlInteger::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("INTEGER"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlInteger::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLINTEGER_DEFN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
