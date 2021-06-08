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


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::Location::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (AllPartitions::bIsOfType(pNode))   { AllPartitions::vDescribe(pNode, sBuf); }
  else if (AllNodes::bIsOfType(pNode))        {      AllNodes::vDescribe(pNode, sBuf); }
  else if (Origin::bIsOfType(pNode))          {        Origin::vDescribe(pNode, sBuf); }

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
/* Returns the name of a location enumeration given a node                    */
/*----------------------------------------------------------------------------*/
Stz* Parser::Location::szLocationName(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  return GraphNav::szLeafType(pNode->szType());
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
