/*============================================================================*/
/* Hashcast.hpp - Send data to a single endpoint based on some hashable expr  */
/*============================================================================*/

#include "BASE.h"
#include "Hashcast.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_HASHCAST_DEFN = "/__root__/definitions/types/routing/hashcast"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Hashcast::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_HASHCAST_DEFN, sName);

  Value xValue;
  xValue.vSet((i8) RT_HASHCAST);

  QueryNode* pNode = new QueryNode(sName, C_HASHCAST_DEFN, static_cast<Value&&>(xValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::Hashcast::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  sBuf.vConcat(Routing::szRoutingType(pNode));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::Hashcast::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_HASHCAST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::Hashcast::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_HASHCAST_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
