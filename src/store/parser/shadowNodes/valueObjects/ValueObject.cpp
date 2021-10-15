/*============================================================================*/
/* ValueObject.hpp - a base class simple object that contains a primitive value*/
/*============================================================================*/

#include "BASE.h"
#include "ValueObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "IntegerObject.hpp"
#include "FloatObject.hpp"
#include "StringObject.hpp"
#include "BooleanObject.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_VALUEOBJECT_NAME   = "ValueObject"_Stz;
static Stz* C_VALUEOBJECT_DEFN   = "/__root__/definitions/object/valueObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Describe the object and write it into the buffer                           */
/*----------------------------------------------------------------------------*/
void ValueObject::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  // manual polymorphism here
  if      (IntegerObject::bIsOfType(pNode))   { IntegerObject::vDescribe(pNode, sBuf, pRootNode); }
  else if ( StringObject::bIsOfType(pNode))   {  StringObject::vDescribe(pNode, sBuf, pRootNode); }
  else if (  FloatObject::bIsOfType(pNode))   {   FloatObject::vDescribe(pNode, sBuf, pRootNode); }
  else if (BooleanObject::bIsOfType(pNode))   { BooleanObject::vDescribe(pNode, sBuf, pRootNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the ValueObject definition  */
/*----------------------------------------------------------------------------*/
b1 ValueObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_VALUEOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ValueObject::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_VALUEOBJECT_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
