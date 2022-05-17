/*============================================================================*/
/* SqlIntervalYM.hpp - an SQL Year-Month interval data type used in the plan  */
/*============================================================================*/

#include "BASE.h"
#include "SqlIntervalYM.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLINTERVAL_DEFN = "/__root__/definitions/types/sqlType/intervalYM"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/*----------------------------------------------------------------------------*/
PlanNode* SqlIntervalYM::pCreate()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLINTERVAL_DEFN, sName);
  QueryNode* pNode = new QueryNode(sName, C_SQLINTERVAL_DEFN);

  // Add in any common properties
  // TODO: Set the correct typeId below
  SqlType::vCreateCommon(pNode, TypeSystem::iIdOf(IV_YEAR_TO_MONTH));

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlIntervalYM::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);

  sBuf.vConcat("INTERVAL YEAR TO MONTH"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlIntervalYM::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLINTERVAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlIntervalYM::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLINTERVAL_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
