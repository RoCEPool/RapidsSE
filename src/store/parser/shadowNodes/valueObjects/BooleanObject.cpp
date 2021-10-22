/*============================================================================*/
/* BooleanObject.hpp - a simple object that contains a string value            */
/*============================================================================*/

#include "BASE.h"
#include "BooleanObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_BOOLEANOBJECT_NAME   = "booleanObject"_Stz;
static Stz* C_BOOLEANOBJECT_DEFN   = "/__root__/definitions/object/valueObject/booleanObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create BooleanObject nodes.                             */
/*----------------------------------------------------------------------------*/
QueryNode* BooleanObject::pCreate(b1 bVal)
{
  _static_

  s256 sName;
  NodeNamer::vGenerateName(C_BOOLEANOBJECT_DEFN, sName);

  Value xVal(bVal);
  QueryNode* pBooleanObject = new QueryNode(sName.szStz(), C_BOOLEANOBJECT_DEFN, static_cast<Value&&>(xVal));

  return pBooleanObject;
}

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void BooleanObject::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);

  } else {
    sBuf.vCatSprint("%s"_Stz, pNode->bValue() ? "true"_Stz : "false"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the BooleanObject definition*/
/*----------------------------------------------------------------------------*/
b1 BooleanObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_BOOLEANOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 BooleanObject::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_BOOLEANOBJECT_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
