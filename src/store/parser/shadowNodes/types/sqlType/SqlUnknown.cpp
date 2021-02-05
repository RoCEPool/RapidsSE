/*============================================================================*/
/* SqlUnknown.hpp - an SQL unknown data type (NULL) used in the plan          */
/*============================================================================*/

#include "BASE.h"
#include "SqlUnknown.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLUNKNOWN_DEFN = "/__root__/definitions/types/sqlType/unknown"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* SqlUnknown::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_SQLUNKNOWN_DEFN), sName);

  PlanNode* pNode = new QueryNode(sName, C_SQLUNKNOWN_DEFN);

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlUnknown::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("NULL"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlUnknown::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLUNKNOWN_DEFN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
