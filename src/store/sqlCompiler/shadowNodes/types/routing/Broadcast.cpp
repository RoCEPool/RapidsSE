/*============================================================================*/
/* Broadcast.hpp - Send data to all endpoints                                 */
/*============================================================================*/

#include "BASE.h"
#include "Broadcast.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_BROADCAST_DEFN = "/__root__/definitions/types/routing/broadcast"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Broadcast::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_BROADCAST_DEFN, sName);

  Value xValue;
  xValue.vSet((i8) RT_BROADCAST);

  QueryNode* pNode = new QueryNode(sName, C_BROADCAST_DEFN, static_cast<Value&&>(xValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::Broadcast::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  sBuf.vConcat(Routing::szRoutingType(pNode));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::Broadcast::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_BROADCAST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::Broadcast::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_BROADCAST_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
