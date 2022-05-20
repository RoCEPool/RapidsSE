/*============================================================================*/
/* SqlType.hpp - an abstract base class of SQL data sqlType used in the plan    */
/*============================================================================*/

#include "BASE.h"
#include "SqlType.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "IntegerObject.hpp"
#include "TypeSystem.hpp"
#include "GraphNav.hpp"

#include "SqlBoolean.hpp"
#include "SqlInteger.hpp"
#include "SqlFloat.hpp"
#include "SqlDecimal.hpp"
#include "SqlVarchar.hpp"
#include "SqlChar.hpp"
#include "SqlDate.hpp"
#include "SqlTime.hpp"
#include "SqlTimestamp.hpp"
#include "SqlIntervalYM.hpp"
#include "SqlIntervalDT.hpp"
#include "SqlUnknown.hpp"

using namespace Parser;

/*
 * Types are hard. On one hand we want the type system to be fully dynamic, but
 * we also want to optimise for the built-in types. And these built-in types
 * are...quirky.
 *
 * When we ask for the type of a column we expect something like INTEGER, DATE,
 * etc. But it can also be things like VARCHAR(10) or DECIMAL(5,2). So the
 * length/precision/scale is also a property of the type.
 *
 * But at the same time if there were no built-in types we wouldn't expect to
 * create a new type of DECIMAL(5,2) which is a different type from DECIMAL(9)6,2).
 * So how do we think of types when some of them can have different properties?
 *
 * The TypeSystem class tries to represent the raw type (e.g. DECIMAL). It does
 * not try to encapsulate the properties of that type (e.g. DECIMAL(5,2)).
 *
 * This SqlType class tries to represent the types with their properties. This is
 * important because we need to do things like describe the type with their properties
 * and this is not as straight forward as just printing the raw type and then its
 * properties in parentheses. e.g. consider the following examples:
 *   VARCHAR(10)
 *   DECIMAL(5,2)
 *   INTERVAL HOUR (2) TO SECOND (6)
 *
 * Hence we still represent types in the graph with properties. We try to minimise
 * the use of eCT and prefer typeId where possible, however eCT is useful for
 * optimising the built-in types.
 */

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLTYPE_DEFN = "/__root__/definitions/types/sqlType"_Stz;

static Stz* C_TYPEID_PROP  = "typeId"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create a type from enum with default properties                            */
/*----------------------------------------------------------------------------*/
QueryNode* SqlType::pCreate(TypeId xTypeId)
{
  _static_
  ASSERT(xTypeId.bIsValid());

  eCT eType = xTypeId.eCTVal();
  QueryNode* pNode;

  // Manual Polymorphism
  switch (eType) {
    case CT_BOOLEAN:
      pNode = _pQueryNode(SqlBoolean::pCreate());
      break;

    case CT_CHAR:
      pNode = _pQueryNode(SqlChar::pCreate());
      break;

    case CT_DATE:
      pNode = _pQueryNode(SqlDate::pCreate());
      break;

    case CT_DECIMAL:
      pNode = _pQueryNode(SqlDecimal::pCreate());
      break;

    case CT_FLOAT:
      pNode = _pQueryNode(SqlFloat::pCreate());
      break;

    case CT_INTEGER:
      pNode = _pQueryNode(SqlInteger::pCreate());
      break;

    case CT_INTERVAL_DT:
      pNode = _pQueryNode(SqlIntervalDT::pCreate());
      break;

    case CT_INTERVAL_YM:
      pNode = _pQueryNode(SqlIntervalYM::pCreate());
      break;

    case CT_TIME:
      pNode = _pQueryNode(SqlTime::pCreate());
      break;

    case CT_TIMESTAMP:
      pNode = _pQueryNode(SqlTimestamp::pCreate());
      break;

    case CT_VARCHAR:
      pNode = _pQueryNode(SqlVarchar::pCreate());
      break;

    case CT_UNKNOWN:
      pNode = _pQueryNode(SqlUnknown::pCreate());
      break;

    case CT_NONE:
    case CT_ENUMS:
    default:
      NEVER_REACH
  }

  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Create common properties in the given node                                 */
/*----------------------------------------------------------------------------*/
void SqlType::vCreateCommon(QueryNode* pNode, TypeId xTypeId)
{
  _static_
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(xTypeId.bIsValid());

  // Add in the typeId property
  QueryNode* pTypeIdNode = IntegerObject::pCreate(xTypeId.iIdVal());
  pTypeIdNode->vSetName(C_TYPEID_PROP);
  pNode->vAppendProperty(pTypeIdNode);
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlType::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (   SqlBoolean::bIsOfType(pNode))     {    SqlBoolean::vDescribe(pNode, sBuf, pRootNode); }
  else if (   SqlInteger::bIsOfType(pNode))     {    SqlInteger::vDescribe(pNode, sBuf, pRootNode); }
  else if (     SqlFloat::bIsOfType(pNode))     {      SqlFloat::vDescribe(pNode, sBuf, pRootNode); }
  else if (   SqlDecimal::bIsOfType(pNode))     {    SqlDecimal::vDescribe(pNode, sBuf, pRootNode); }
  else if (   SqlVarchar::bIsOfType(pNode))     {    SqlVarchar::vDescribe(pNode, sBuf, pRootNode); }
  else if (      SqlChar::bIsOfType(pNode))     {       SqlChar::vDescribe(pNode, sBuf, pRootNode);    }
  else if (      SqlDate::bIsOfType(pNode))     {       SqlDate::vDescribe(pNode, sBuf, pRootNode); }
  else if (      SqlTime::bIsOfType(pNode))     {       SqlTime::vDescribe(pNode, sBuf, pRootNode); }
  else if ( SqlTimestamp::bIsOfType(pNode))     {  SqlTimestamp::vDescribe(pNode, sBuf, pRootNode); }
  else if (SqlIntervalDT::bIsOfType(pNode))     { SqlIntervalDT::vDescribe(pNode, sBuf, pRootNode); }
  else if (SqlIntervalYM::bIsOfType(pNode))     { SqlIntervalYM::vDescribe(pNode, sBuf, pRootNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlType::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLTYPE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlType::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLTYPE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the name of a SQL data type given a node                           */
/*----------------------------------------------------------------------------*/
Stz* SqlType::szTypeName(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  TypeId xTypeId = SqlType::xTypeId(pNode);
  ASSERT(xTypeId.bIsValid());

  return xTypeId.szTypeName();
}

/*----------------------------------------------------------------------------*/
/* Returns the typeId of a SQL data type given a node                         */
/*----------------------------------------------------------------------------*/
TypeId SqlType::xTypeId(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pTypeIdNode = _pQueryNode(GraphNav::pFindProperty(C_TYPEID_PROP, pNode));
  ASSERT(IntegerObject::bIsOfType(pTypeIdNode));

  TypeId xTypeId{pTypeIdNode->iValue()};
  return xTypeId;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
