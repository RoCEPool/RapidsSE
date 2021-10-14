/*============================================================================*/
/* FloatObject.hpp - a simple object that contains an floating point value    */
/*============================================================================*/

#include "BASE.h"
#include "FloatObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_FLOATOBJECT_NAME   = "floatObject"_Stz;
static Stz* C_FLOATOBJECT_DEFN   = "/__root__/definitions/object/valueObject/floatObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create FloatObject nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* FloatObject::pCreate(d8 dVal)
{
  _static_

  s256 sName;
  NodeNamer::vGenerateName(C_FLOATOBJECT_DEFN, sName);

  Value xVal(dVal);
  QueryNode* pFloatObject = new QueryNode(sName.szStz(), C_FLOATOBJECT_DEFN, static_cast<Value&&>(xVal));

  return pFloatObject;
}

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void FloatObject::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);

  } else {
    sBuf.vCatSprint("%f"_Stz, pNode->iValue());
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the FloatObject definition*/
/*----------------------------------------------------------------------------*/
b1 FloatObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_FLOATOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 FloatObject::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_FLOATOBJECT_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
