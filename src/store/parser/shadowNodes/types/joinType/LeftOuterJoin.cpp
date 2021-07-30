/*============================================================================*/
/* LeftOuterJoin.hpp - represents a SQL left outer join                       */
/*============================================================================*/

#include "BASE.h"
#include "LeftOuterJoin.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LEFTOUTERJOIN_DEFN = "/__root__/definitions/types/joinType/leftOuterJoin"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* LeftOuterJoin::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_LEFTOUTERJOIN_DEFN, sName);
  Value xTypeValue((i8) JT_LEFT_OUTER);
  PlanNode* pNode = new QueryNode(sName, C_LEFTOUTERJOIN_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 LeftOuterJoin::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LEFTOUTERJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 LeftOuterJoin::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_LEFTOUTERJOIN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
