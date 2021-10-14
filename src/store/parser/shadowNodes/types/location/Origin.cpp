/*============================================================================*/
/* Origin.hpp - Runs once on the origin node in the cluster                   */
/*============================================================================*/

#include "BASE.h"
#include "Origin.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ORIGIN_DEFN = "/__root__/definitions/types/location/origin"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Origin::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_ORIGIN_DEFN, sName);

  Value xValue;
  xValue.vSet((i8) LO_ORIGIN);

  QueryNode* pNode = new QueryNode(sName, C_ORIGIN_DEFN, static_cast<Value&&>(xValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::Origin::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat(Location::szLocationType(pNode));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::Origin::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ORIGIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::Origin::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ORIGIN_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
