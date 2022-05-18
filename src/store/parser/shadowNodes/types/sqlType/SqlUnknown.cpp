/*============================================================================*/
/* SqlUnknown.hpp - an SQL unknown data type (NULL) used in the plan          */
/*============================================================================*/

#include "BASE.h"
#include "SqlUnknown.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"

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
  NodeNamer::vGenerateName(C_SQLUNKNOWN_DEFN, sName);
  QueryNode* pNode = new QueryNode(sName, C_SQLUNKNOWN_DEFN);

  // Add in any common properties
  SqlType::vCreateCommon(pNode, TypeSystem::iIdOf(eCT::CT_UNKNOWN));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlUnknown::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);
  UNUSED(pRootNode);

  sBuf.vConcat("NULL"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlUnknown::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLUNKNOWN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlUnknown::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLUNKNOWN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
