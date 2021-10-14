/*============================================================================*/
/* Location.hpp - cardinality & where sql operators can execute in the cluster*/
/*============================================================================*/

#include "BASE.h"
#include "store/parser/shadowNodes/types/location/Location.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

#include "AllPartitions.hpp"
#include "AllNodes.hpp"
#include "Origin.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LOCATION_DEFN = "/__root__/definitions/types/location"_Stz;

static Stz* acLO[Parser::Location::LO_NENUMS] = {
    nullptr,
    "origin"_Stz,
    "all nodes"_Stz,
    "all partitions"_Stz
};

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates a location node based on its enum type                             */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::Location::pCreate(eLO eType)
{
  _static_

  switch (eType) {

    case LO_ORIGIN:
      return Origin::pCreate();

    case LO_ALL_NODES:
      return AllNodes::pCreate();

    case LO_ALL_PARTITIONS:
      return AllPartitions::pCreate();

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::Location::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (AllPartitions::bIsOfType(pNode))   { AllPartitions::vDescribe(pNode, sBuf, pRootNode); }
  else if (AllNodes::bIsOfType(pNode))        {      AllNodes::vDescribe(pNode, sBuf, pRootNode); }
  else if (Origin::bIsOfType(pNode))          {        Origin::vDescribe(pNode, sBuf, pRootNode); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::Location::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LOCATION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::Location::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_LOCATION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns an enum that represents the type of location                       */
/*----------------------------------------------------------------------------*/
Parser::Location::eLO Parser::Location::eLocationType(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (AllPartitions::bIsOfType(pNode))   { return LO_ALL_PARTITIONS; }
  else if (AllNodes::bIsOfType(pNode))        { return LO_ALL_NODES;      }
  else if (Origin::bIsOfType(pNode))          { return LO_ORIGIN;         }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns an enum that represents the type of location                       */
/*----------------------------------------------------------------------------*/
Stz* Parser::Location::szLocationType(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (AllPartitions::bIsOfType(pNode))   { return acLO[LO_ALL_PARTITIONS]; }
  else if (AllNodes::bIsOfType(pNode))        { return acLO[LO_ALL_NODES];      }
  else if (Origin::bIsOfType(pNode))          { return acLO[LO_ORIGIN];         }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
