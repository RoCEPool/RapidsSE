/*============================================================================*/
/* AllPartitions.hpp - Runs on each partition in the cluster                  */
/*============================================================================*/

#include "BASE.h"
#include "AllPartitions.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ALLPARTITIONS_DEFN = "/__root__/definitions/types/location/allPartitions"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::AllPartitions::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_ALLPARTITIONS_DEFN, sName);

  Value xValue;
  xValue.vSet((i8) LO_ALL_PARTITIONS);

  QueryNode* pNode = new QueryNode(sName, C_ALLPARTITIONS_DEFN, static_cast<Value&&>(xValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::AllPartitions::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  UNUSED(pRootNode);
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat(Location::szLocationType(pNode));
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::AllPartitions::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ALLPARTITIONS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::AllPartitions::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ALLPARTITIONS_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
