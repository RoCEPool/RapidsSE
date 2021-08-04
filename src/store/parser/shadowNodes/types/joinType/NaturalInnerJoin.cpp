/*============================================================================*/
/* NaturalInnerJoin.hpp - represents a SQL natural inner join                 */
/*============================================================================*/

#include "BASE.h"
#include "NaturalInnerJoin.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NATURALINNERJOIN_DEFN = "/__root__/definitions/types/joinType/naturalInnerJoin"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* NaturalInnerJoin::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_NATURALINNERJOIN_DEFN, sName);
  Value xTypeValue((i8) JT_NATURAL_INNER);
  PlanNode* pNode = new QueryNode(sName, C_NATURALINNERJOIN_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 NaturalInnerJoin::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_NATURALINNERJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 NaturalInnerJoin::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_NATURALINNERJOIN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
