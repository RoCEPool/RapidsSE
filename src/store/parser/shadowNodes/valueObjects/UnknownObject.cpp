/*============================================================================*/
/* UnknownObject.hpp - a simple object with a null value of unknown type      */
/*============================================================================*/

#include "BASE.h"
#include "UnknownObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME   = "unknownObject"_Stz;
static Stz* C_DEFN   = "/__root__/definitions/object/valueObject/unknownObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create unknownObject nodes.                              */
/*----------------------------------------------------------------------------*/
QueryNode* UnknownObject::pCreate()
{
  _static_

  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  Value xVal;
  xVal.vSetNull(Value::VT_UNKNOWN);
  QueryNode* pUnknownObject = new QueryNode(sName.szStz(), C_DEFN, static_cast<Value&&>(xVal));

  return pUnknownObject;
}

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void UnknownObject::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  sBuf.vConcat("NULL"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the unknownObject definition    */
/*----------------------------------------------------------------------------*/
b1 UnknownObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 UnknownObject::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
