/*============================================================================*/
/* SqlVarchar.hpp - an SQL exact-numeric varchar data type used in the plan   */
/*============================================================================*/

#include "BASE.h"
#include "SqlVarchar.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "TypeSystem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLVARCHAR_DEFN     = "/__root__/definitions/types/sqlType/varchar"_Stz;

static Stz* C_LENGTH_PROP_NAME = "length"_Stz;
static Stz* C_LENGTH_DEFN      = "/__root__/definitions/object/valueObject/integerObject"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type with the default length                   */
/*----------------------------------------------------------------------------*/
PlanNode* SqlVarchar::pCreate()
{
  return pCreate(-1);
}

/*----------------------------------------------------------------------------*/
/* Creates an instance of this type                                           */
/* Use -1 for length if not applicable.                                       */
/*----------------------------------------------------------------------------*/
PlanNode* SqlVarchar::pCreate(i8 iLength)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_SQLVARCHAR_DEFN, sName);
  QueryNode* pNode = new QueryNode(sName, C_SQLVARCHAR_DEFN);

  // Add in any common properties
  SqlType::vCreateCommon(pNode, TypeSystem::iIdOf(CT_VARCHAR));

  if (iLength != -1) {
    ASSERT(iLength > 0);

    Value xValue(iLength);
    QueryNode* pLengthNode = new QueryNode(C_LENGTH_PROP_NAME, C_LENGTH_DEFN, static_cast<Value&&>(xValue));
    pNode->vAppendProperty(pLengthNode);
  }

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlVarchar::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  sBuf.vConcat("VARCHAR"_Stz);

  PlanNode* pLengthNode = GraphNav::pFindProperty(C_LENGTH_PROP_NAME, pNode);

  if (pLengthNode) {
    sBuf.vCatSprint("(%lld)"_Stz, pLengthNode->iValue());
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlVarchar::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLVARCHAR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlVarchar::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLVARCHAR_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
