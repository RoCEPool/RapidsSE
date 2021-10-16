/*============================================================================*/
/* Unicast.hpp - Send data to a single endpoint                               */
/*============================================================================*/

#include "BASE.h"
#include "Unicast.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_UNICAST_DEFN = "/__root__/definitions/types/routing/unicast"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Unicast::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_UNICAST_DEFN, sName);

  Value xValue;
  xValue.vSet((i8) RT_UNICAST);

  QueryNode* pNode = new QueryNode(sName, C_UNICAST_DEFN, static_cast<Value&&>(xValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::Unicast::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat(Routing::szRoutingType(pNode));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::Unicast::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_UNICAST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::Unicast::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_UNICAST_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
