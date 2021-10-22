/*============================================================================*/
/* StringObject.hpp - a simple object that contains a string value            */
/*============================================================================*/

#include "BASE.h"
#include "StringObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_STRINGOBJECT_NAME   = "stringObject"_Stz;
static Stz* C_STRINGOBJECT_DEFN   = "/__root__/definitions/object/valueObject/stringObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create stringObject nodes.                              */
/*----------------------------------------------------------------------------*/
QueryNode* StringObject::pCreate(Text* pText)
{
  _static_

  s256 sName;
  NodeNamer::vGenerateName(C_STRINGOBJECT_DEFN, sName);

  Value xVal(pText);
  QueryNode* pStringObject = new QueryNode(sName.szStz(), C_STRINGOBJECT_DEFN, static_cast<Value&&>(xVal));

  return pStringObject;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create stringObject nodes.                              */
/*----------------------------------------------------------------------------*/
QueryNode* StringObject::pCreate(Stz* szText)
{
  _static_

  s256 sName;
  NodeNamer::vGenerateName(C_STRINGOBJECT_DEFN, sName);

  Value xVal(szText);
  QueryNode* pStringObject = new QueryNode(sName.szStz(), C_STRINGOBJECT_DEFN, static_cast<Value&&>(xVal));

  return pStringObject;
}

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void StringObject::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);

  } else {
    sBuf.vConcat(pNode->szValue());
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the stringObject definition    */
/*----------------------------------------------------------------------------*/
b1 StringObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STRINGOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StringObject::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STRINGOBJECT_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
