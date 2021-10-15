/*============================================================================*/
/* LocalNode.hpp - Send data to a common stage on the same node               */
/*============================================================================*/

#include "BASE.h"
#include "LocalNode.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LOCALNODE_DEFN = "/__root__/definitions/types/routing/localNode"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::LocalNode::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_LOCALNODE_DEFN, sName);

  Value xValue;
  xValue.vSet((i8) RT_LOCALNODE);

  QueryNode* pNode = new QueryNode(sName, C_LOCALNODE_DEFN, static_cast<Value&&>(xValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::LocalNode::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat(Routing::szRoutingType(pNode));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::LocalNode::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LOCALNODE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::LocalNode::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_LOCALNODE_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
