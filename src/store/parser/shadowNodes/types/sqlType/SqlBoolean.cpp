/*============================================================================*/
/* SqlBoolean.hpp - an SQL boolean data type used in the plan                 */
/*============================================================================*/

#include "BASE.h"
#include "SqlBoolean.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLBOOLEAN_DEFN = "/__root__/definitions/types/sqlType/boolean"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* SqlBoolean::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLBOOLEAN_DEFN, sName);
  QueryNode* pNode = new QueryNode(sName, C_SQLBOOLEAN_DEFN);

  // Add in any common properties
  SqlType::vCreateCommon(pNode, TypeSystem::iIdOf(CT_BOOLEAN));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlBoolean::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);
  UNUSED(pRootNode);

  sBuf.vConcat("BOOLEAN"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlBoolean::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLBOOLEAN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlBoolean::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLBOOLEAN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
