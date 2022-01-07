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
    "n/a"_Stz,
    "unicast"_Stz,
    "broadcast"_Stz,
    "hashcast"_Stz,
    "local partition"_Stz,
    "local node"_Stz
};


// What is the default routing, given two locations values?
// This is intended to be ok for most sql operator combinations.
// Exceptional cases (such as joins) can be handled separately.
//
// The left header column represents the "from" location
// The top header row represents the "to" location
// Invalid combinations are represented with LO_NONE

static Routing::eRT aeDefaultRoutes[Parser::Location::LO_NENUMS][Parser::Location::LO_NENUMS] = {
//                                  LO_NONE             LO_ORIGIN            LO_ALL_NODES           LO_ALL_PARTITIONS
/* 0 LO_NONE           */ { Routing::RT_NONE,  Routing::RT_NONE,     Routing::RT_NONE,       Routing::RT_NONE           },
/* 1 LO_ORIGIN         */ { Routing::RT_NONE,  Routing::RT_UNICAST,  Routing::RT_BROADCAST,  Routing::RT_BROADCAST      },
/* 2 LO_ALL_NODES      */ { Routing::RT_NONE,  Routing::RT_UNICAST,  Routing::RT_BROADCAST,  Routing::RT_BROADCAST      },
/* 3 LO_ALL_PARTITIONS */ { Routing::RT_NONE,  Routing::RT_UNICAST,  Routing::RT_LOCALNODE,  Routing::RT_LOCALPARTITION }
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
void Parser::Routing::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (       Unicast::bIsOfType(pNode))  {        Unicast::vDescribe(pNode, sBuf, pRootNode); }
  else if (     Broadcast::bIsOfType(pNode))  {      Broadcast::vDescribe(pNode, sBuf, pRootNode); }
  else if (      Hashcast::bIsOfType(pNode))  {       Hashcast::vDescribe(pNode, sBuf, pRootNode); }
  else if (LocalPartition::bIsOfType(pNode))  { LocalPartition::vDescribe(pNode, sBuf, pRootNode); }
  else if (     LocalNode::bIsOfType(pNode))  {      LocalNode::vDescribe(pNode, sBuf, pRootNode); }

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
  if      (       Unicast::bIsOfType(pNode))  { return acRT[RT_UNICAST];        }
  else if (     Broadcast::bIsOfType(pNode))  { return acRT[RT_BROADCAST];      }
  else if (      Hashcast::bIsOfType(pNode))  { return acRT[RT_HASHCAST];       }
  else if (LocalPartition::bIsOfType(pNode))  { return acRT[RT_LOCALPARTITION]; }
  else if (     LocalNode::bIsOfType(pNode))  { return acRT[RT_LOCALNODE];      }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns an enum that represents the type of routing                        */
/*----------------------------------------------------------------------------*/
Stz* Parser::Routing::szRoutingType(eRT eRouteType)
{
  _static_
  ASSERT(eRouteType > RT_NONE);
  ASSERT(eRouteType < RT_NENUMS);

  return acRT[eRouteType];
}

/*----------------------------------------------------------------------------*/
/* Return an enum of the routing type from its string value (case insensitive)*/
/*----------------------------------------------------------------------------*/
Parser::Routing::eRT Parser::Routing::eRoutingType(Stz* szRoutingType)
{
  ASSERT(szRoutingType);
  eRT eRouteType = RT_NONE;

  while (eRouteType != RT_NENUMS) {
    eRouteType = (eRT) ((i8) eRouteType + 1);

    if (acRT[eRouteType]->bSame(szRoutingType)) {
      return eRouteType;
    }
  }

  return RT_NONE;
}


/*----------------------------------------------------------------------------*/
/* Returns an enum that represents the type of routing                       */
/*----------------------------------------------------------------------------*/
Parser::Routing::eRT Parser::Routing::eDefaultRouting(Parser::Location::eLO eFromLocation,
                                                      Parser::Location::eLO eToLocation)
{
  _static_
  ASSERT(eFromLocation > Location::LO_NONE  );
  ASSERT(eFromLocation < Location::LO_NENUMS);
  ASSERT(eToLocation   > Location::LO_NONE  );
  ASSERT(eToLocation   < Location::LO_NENUMS);

  eRT eRouting = aeDefaultRoutes[eFromLocation][eToLocation];
  return eRouting;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
