/*============================================================================*/
/* SqlFloat.hpp - an SQL approximate-numeric float data type used in the plan */
/*============================================================================*/

#include "BASE.h"
#include "SqlFloat.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLFLOAT_DEFN = "/__root__/definitions/types/sqlType/float"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* SqlFloat::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLFLOAT_DEFN, sName);
  QueryNode* pNode = new QueryNode(sName, C_SQLFLOAT_DEFN);

  // Add in any common properties
  SqlType::vCreateCommon(pNode, TypeSystem::iIdOf(CT_FLOAT));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlFloat::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);
  UNUSED(pRootNode);

  sBuf.vConcat("FLOAT"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlFloat::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLFLOAT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlFloat::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLFLOAT_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
