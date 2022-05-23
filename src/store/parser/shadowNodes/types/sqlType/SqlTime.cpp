/*============================================================================*/
/* SqlTime.hpp - an SQL time data type used in the plan                 */
/*============================================================================*/

#include "BASE.h"
#include "SqlTime.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLTIME_DEFN = "/__root__/definitions/types/sqlType/time"_Stz;
static Stz* C_TZOFFSET_PROP = "tzOffset"_Stz;
static Stz* C_TZOFFSET_DEFN = "/__root__/definitions/object/valueObject/integerObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* SqlTime::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLTIME_DEFN, sName);
  QueryNode* pNode = new QueryNode(sName, C_SQLTIME_DEFN);

  // Add in any common properties
  SqlType::vCreateCommon(pNode, TypeId::xIdOf(CT_TIME));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type with timezone offset information          */
/*----------------------------------------------------------------------------*/
PlanNode* SqlTime::pCreate(i8 iMinOffset)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLTIME_DEFN, sName);
  QueryNode* pNode = new QueryNode(sName, C_SQLTIME_DEFN);

  // Add in any common properties
  SqlType::vCreateCommon(pNode, TypeId::xIdOf(CT_TIME));

  // Create the offset property
  Value xValue(iMinOffset);
  QueryNode* pOffset = new QueryNode(C_TZOFFSET_PROP, C_TZOFFSET_DEFN, static_cast<Value&&>(xValue));
  pNode->vAppendProperty(pOffset);

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlTime::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);
  UNUSED(pRootNode);

  sBuf.vConcat("TIME"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlTime::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLTIME_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlTime::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLTIME_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
