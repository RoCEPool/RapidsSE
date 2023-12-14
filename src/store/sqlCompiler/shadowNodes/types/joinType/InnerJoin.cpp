/*============================================================================*/
/* InnerJoin.hpp - represents a SQL inner join                                */
/*============================================================================*/

#include "BASE.h"
#include "InnerJoin.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_INNERJOIN_DEFN = "/__root__/definitions/types/joinType/innerJoin"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* InnerJoin::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_INNERJOIN_DEFN, sName);
  Value xTypeValue((i8) JT_INNER);
  PlanNode* pNode = new QueryNode(sName, C_INNERJOIN_DEFN, static_cast<Value&&>(xTypeValue));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 InnerJoin::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_INNERJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 InnerJoin::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_INNERJOIN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
