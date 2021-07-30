/*============================================================================*/
/* FullOuterJoin.hpp - represents a SQL full outer join                       */
/*============================================================================*/

#include "BASE.h"
#include "FullOuterJoin.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_FULLOUTERJOIN_DEFN = "/__root__/definitions/types/joinType/fullOuterJoin"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* FullOuterJoin::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_FULLOUTERJOIN_DEFN, sName);
  Value xTypeValue((i8) JT_FULL_OUTER);
  PlanNode* pNode = new QueryNode(sName, C_FULLOUTERJOIN_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 FullOuterJoin::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_FULLOUTERJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 FullOuterJoin::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_FULLOUTERJOIN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
