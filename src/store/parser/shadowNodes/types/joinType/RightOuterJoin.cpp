/*============================================================================*/
/* RightOuterJoin.hpp - represents a SQL right outer join                     */
/*============================================================================*/

#include "BASE.h"
#include "RightOuterJoin.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_RIGHTOUTERJOIN_DEFN = "/__root__/definitions/types/joinType/rightOuterJoin"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* RightOuterJoin::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_RIGHTOUTERJOIN_DEFN, sName);
  Value xTypeValue((i8) JT_RIGHT_OUTER);
  PlanNode* pNode = new QueryNode(sName, C_RIGHTOUTERJOIN_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 RightOuterJoin::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_RIGHTOUTERJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 RightOuterJoin::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_RIGHTOUTERJOIN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
