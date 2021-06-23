/*============================================================================*/
/* Routing.hpp - how row data should be sent to the parent node               */
/*============================================================================*/

#include "BASE.h"
#include "store/parser/shadowNodes/types/routing/Routing.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

#include "Unicast.hpp"
#include "Broadcast.hpp"
#include "Hashcast.hpp"
#include "LocalPartition.hpp"
#include "LocalNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ROUTING_DEFN = "/__root__/definitions/types/routing"_Stz;

static Stz* acRT[Parser::Routing::RT_NENUMS] = {
    nullptr,
    "unicast"_Stz,
    "broadcast"_Stz,
    "hashcast"_Stz,
    "local partition"_Stz,
    "local node"_Stz
};

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates a routing node based on its enum type                             */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Routing::pCreate(eRT eType)
{
  _static_

  switch (eType) {

    case RT_UNICAST:
      return Unicast::pCreate();

    case RT_BROADCAST:
      return Broadcast::pCreate();

    case RT_HASHCAST:
      return Hashcast::pCreate();

    case RT_LOCALPARTITION:
      return LocalPartition::pCreate();

    case RT_LOCALNODE:
      return LocalNode::pCreate();

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::Routing::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (Unicast::bIsOfType(pNode))         {        Unicast::vDescribe(pNode, sBuf); }
  else if (Broadcast::bIsOfType(pNode))       {      Broadcast::vDescribe(pNode, sBuf); }
  else if (Hashcast::bIsOfType(pNode))        {       Hashcast::vDescribe(pNode, sBuf); }
  else if (LocalPartition::bIsOfType(pNode))  { LocalPartition::vDescribe(pNode, sBuf); }
  else if (LocalNode::bIsOfType(pNode))       {      LocalNode::vDescribe(pNode, sBuf); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::Routing::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ROUTING_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::Routing::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ROUTING_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns an enum that represents the type of routing                       */
/*----------------------------------------------------------------------------*/
Parser::Routing::eRT Parser::Routing::eRoutingType(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (Unicast::bIsOfType(pNode))         { return RT_UNICAST;        }
  else if (Broadcast::bIsOfType(pNode))       { return RT_BROADCAST;      }
  else if (Hashcast::bIsOfType(pNode))        { return RT_HASHCAST;       }
  else if (LocalPartition::bIsOfType(pNode))  { return RT_LOCALPARTITION; }
  else if (LocalNode::bIsOfType(pNode))       { return RT_LOCALNODE;      }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns an enum that represents the type of routing                       */
/*----------------------------------------------------------------------------*/
Stz* Parser::Routing::szRoutingType(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (Unicast::bIsOfType(pNode))         { return acRT[RT_UNICAST];        }
  else if (Broadcast::bIsOfType(pNode))       { return acRT[RT_BROADCAST];      }
  else if (Hashcast::bIsOfType(pNode))        { return acRT[RT_HASHCAST];       }
  else if (LocalPartition::bIsOfType(pNode))  { return acRT[RT_LOCALPARTITION]; }
  else if (LocalNode::bIsOfType(pNode))       { return acRT[RT_LOCALNODE];      }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
