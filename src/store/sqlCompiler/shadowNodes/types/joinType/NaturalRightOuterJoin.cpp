/*============================================================================*/
/* NaturalRightOuterJoin.hpp - represents a SQL natural right outer join      */
/*============================================================================*/

#include "BASE.h"
#include "NaturalRightOuterJoin.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NATURALRIGHTOUTERJOIN_DEFN = "/__root__/definitions/types/joinType/naturalRightOuterJoin"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* NaturalRightOuterJoin::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_NATURALRIGHTOUTERJOIN_DEFN, sName);
  Value xTypeValue((i8) JT_NATURAL_RIGHT_OUTER);
  PlanNode* pNode = new QueryNode(sName, C_NATURALRIGHTOUTERJOIN_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 NaturalRightOuterJoin::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_NATURALRIGHTOUTERJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 NaturalRightOuterJoin::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_NATURALRIGHTOUTERJOIN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
