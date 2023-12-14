/*============================================================================*/
/* DecimalObject.hpp - a simple object that contains an decimaling point value    */
/*============================================================================*/

#include "BASE.h"
#include "DecimalObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DECIMALOBJECT_NAME   = "decimalObject"_Stz;
static Stz* C_DECIMALOBJECT_DEFN   = "/__root__/definitions/object/valueObject/decimalObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create DecimalObject nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* DecimalObject::pCreate(Bcd xBcd)
{
  _static_

  s256 sName;
  NodeNamer::vGenerateName(C_DECIMALOBJECT_DEFN, sName);

  Value xVal(xBcd);
  QueryNode* pDecimalObject = new QueryNode(sName.szStz(), C_DECIMALOBJECT_DEFN, static_cast<Value&&>(xVal));

  return pDecimalObject;
}

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void DecimalObject::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);

  } else {
    s2K sBcdBuf;
    pNode->xValue().vDescribe(sBcdBuf);
    sBuf.vConcat(sBcdBuf.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the DecimalObject definition*/
/*----------------------------------------------------------------------------*/
b1 DecimalObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DECIMALOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 DecimalObject::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DECIMALOBJECT_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
