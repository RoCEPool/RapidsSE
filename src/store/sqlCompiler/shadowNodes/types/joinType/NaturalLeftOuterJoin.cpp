/*============================================================================*/
/* NaturalLeftOuterJoin.hpp - represents a SQL natural left outer join        */
/*============================================================================*/

#include "BASE.h"
#include "NaturalLeftOuterJoin.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NATURALLEFTOUTERJOIN_DEFN = "/__root__/definitions/types/joinType/naturalLeftOuterJoin"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* NaturalLeftOuterJoin::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_NATURALLEFTOUTERJOIN_DEFN, sName);
  Value xTypeValue((i8) JT_NATURAL_LEFT_OUTER);
  PlanNode* pNode = new QueryNode(sName, C_NATURALLEFTOUTERJOIN_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 NaturalLeftOuterJoin::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_NATURALLEFTOUTERJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 NaturalLeftOuterJoin::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_NATURALLEFTOUTERJOIN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
