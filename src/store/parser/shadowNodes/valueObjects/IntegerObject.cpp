/*============================================================================*/
/* IntegerObject.hpp - a simple object that contains a string value            */
/*============================================================================*/

#include "BASE.h"
#include "IntegerObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_INTEGEROBJECT_NAME   = "integerObject"_Stz;
static Stz* C_INTEGEROBJECT_DEFN   = "/__root__/definitions/object/valueObject/integerObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create IntegerObject nodes.                             */
/*----------------------------------------------------------------------------*/
QueryNode* IntegerObject::pCreate(i8 iVal)
{
  _static_

  s256 sName;
  NodeNamer::vGenerateName(C_INTEGEROBJECT_DEFN, sName);

  Value xVal(iVal);
  QueryNode* pIntegerObject = new QueryNode(sName.szStz(), C_INTEGEROBJECT_DEFN, static_cast<Value&&>(xVal));

  return pIntegerObject;
}

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void IntegerObject::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);

  } else {
    sBuf.vCatSprint("%lld"_Stz, pNode->iValue());
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the IntegerObject definition*/
/*----------------------------------------------------------------------------*/
b1 IntegerObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_INTEGEROBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IntegerObject::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_INTEGEROBJECT_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
