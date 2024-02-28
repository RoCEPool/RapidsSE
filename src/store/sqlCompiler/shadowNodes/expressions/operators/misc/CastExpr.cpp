/*============================================================================*/
/* CastExpr.hpp - an expression operator for converting the returning type    */
/*============================================================================*/

#include "BASE.h"
#include "CastExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "eCT.h"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "SqlType.hpp"
#include "GraphNav.hpp"
#include "TypeSystem.hpp"
#include "TypeMap2D.hpp"
#include "ComparisonExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "CastExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/misc/cast"_Stz;

static Stz* C_TARGETTYPE_PROP = "targetType"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand (the 'from' type)
//  - The second parameter represents the type of the second operand (the 'to' type).
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static TypeMap2D* S_pTypeMap = (new TypeMap2D())
    // BOOLEAN
    ->pAddSelfMapping(TypeId::xIdOf(CT_BOOLEAN))
    ->pAddMapping(TypeId::xIdOf(CT_BOOLEAN), TypeId::xIdOf(CT_CHAR),            TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_BOOLEAN), TypeId::xIdOf(CT_VARCHAR),         TypeId::xIdOf(CT_VARCHAR))

    // CHAR
    ->pAddSelfMapping(TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),    TypeId::xIdOf(CT_DATE),            TypeId::xIdOf(CT_DATE))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),    TypeId::xIdOf(CT_DECIMAL),         TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),    TypeId::xIdOf(CT_FLOAT),           TypeId::xIdOf(CT_FLOAT))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),    TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),    TypeId::xIdOf(CT_TIME),            TypeId::xIdOf(CT_TIME))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),    TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_TIMESTAMP))
    ->pAddMapping(TypeId::xIdOf(CT_CHAR),    TypeId::xIdOf(CT_VARCHAR),         TypeId::xIdOf(CT_VARCHAR))
    // TODO: We could possibly cast strings to intervals

    // DATE
    ->pAddSelfMapping(TypeId::xIdOf(CT_DATE))
    ->pAddMapping(TypeId::xIdOf(CT_DATE),    TypeId::xIdOf(CT_CHAR),            TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_DATE),    TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_TIMESTAMP))
    ->pAddMapping(TypeId::xIdOf(CT_DATE),    TypeId::xIdOf(CT_VARCHAR),         TypeId::xIdOf(CT_VARCHAR))

    // DECIMAL
    // Note: You can convert decimal to single field intervals.
    //       Apart from seconds (which have a fractional component), other values
    //       ought to use the non-fractional part of the decimal (integer rounding).
    ->pAddSelfMapping(TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(CT_CHAR),            TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(CT_FLOAT),           TypeId::xIdOf(CT_FLOAT))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(CT_VARCHAR),         TypeId::xIdOf(CT_VARCHAR))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(IV_YEAR),            TypeId::xIdOf(IV_YEAR))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(IV_MONTH),           TypeId::xIdOf(IV_MONTH))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(IV_DAY),             TypeId::xIdOf(IV_DAY))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(IV_HOUR),            TypeId::xIdOf(IV_HOUR))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(IV_MINUTE),          TypeId::xIdOf(IV_MINUTE))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL), TypeId::xIdOf(IV_SECOND),          TypeId::xIdOf(IV_SECOND))

    // FLOAT
    // Note: You can convert floats to single field intervals.
    //       Apart from seconds (which have a fractional component), other values
    //       ought to use the non-fractional part of the float (integer rounding).
    ->pAddSelfMapping(TypeId::xIdOf(CT_FLOAT))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(CT_CHAR),            TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(CT_DECIMAL),         TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(CT_VARCHAR),         TypeId::xIdOf(CT_VARCHAR))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(IV_YEAR),            TypeId::xIdOf(IV_YEAR))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(IV_MONTH),           TypeId::xIdOf(IV_MONTH))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(IV_DAY),             TypeId::xIdOf(IV_DAY))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(IV_HOUR),            TypeId::xIdOf(IV_HOUR))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(IV_MINUTE),          TypeId::xIdOf(IV_MINUTE))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),   TypeId::xIdOf(IV_SECOND),          TypeId::xIdOf(IV_SECOND))

    // INTEGER
    // Note: You can convert integers to single field intervals.
    ->pAddSelfMapping(TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(CT_CHAR),            TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(CT_DECIMAL),         TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(CT_FLOAT),           TypeId::xIdOf(CT_FLOAT))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(CT_VARCHAR),         TypeId::xIdOf(CT_VARCHAR))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(IV_YEAR),            TypeId::xIdOf(IV_YEAR))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(IV_MONTH),           TypeId::xIdOf(IV_MONTH))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(IV_DAY),             TypeId::xIdOf(IV_DAY))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(IV_HOUR),            TypeId::xIdOf(IV_HOUR))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(IV_MINUTE),          TypeId::xIdOf(IV_MINUTE))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),   TypeId::xIdOf(IV_SECOND),          TypeId::xIdOf(IV_SECOND))

    // TIME
    ->pAddSelfMapping(TypeId::xIdOf(CT_TIME))
    ->pAddMapping(TypeId::xIdOf(CT_TIME),    TypeId::xIdOf(CT_CHAR),            TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_TIME),    TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_TIMESTAMP))
    ->pAddMapping(TypeId::xIdOf(CT_TIME),    TypeId::xIdOf(CT_VARCHAR),         TypeId::xIdOf(CT_VARCHAR))

    // TIMESTAMP
    ->pAddSelfMapping(TypeId::xIdOf(CT_TIMESTAMP))
    ->pAddMapping(TypeId::xIdOf(CT_TIMESTAMP),TypeId::xIdOf(CT_CHAR),            TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_TIMESTAMP),TypeId::xIdOf(CT_DATE),            TypeId::xIdOf(CT_DATE))
    ->pAddMapping(TypeId::xIdOf(CT_TIMESTAMP),TypeId::xIdOf(CT_TIME),            TypeId::xIdOf(CT_TIME))
    ->pAddMapping(TypeId::xIdOf(CT_TIMESTAMP),TypeId::xIdOf(CT_VARCHAR),         TypeId::xIdOf(CT_VARCHAR))

    // VARCHAR
    ->pAddSelfMapping(TypeId::xIdOf(CT_VARCHAR))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),    TypeId::xIdOf(CT_CHAR),            TypeId::xIdOf(CT_CHAR))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),    TypeId::xIdOf(CT_DATE),            TypeId::xIdOf(CT_DATE))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),    TypeId::xIdOf(CT_DECIMAL),         TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),    TypeId::xIdOf(CT_FLOAT),           TypeId::xIdOf(CT_FLOAT))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),    TypeId::xIdOf(CT_INTEGER),         TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),    TypeId::xIdOf(CT_TIME),            TypeId::xIdOf(CT_TIME))
    ->pAddMapping(TypeId::xIdOf(CT_VARCHAR),    TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_TIMESTAMP))
        // TODO: We could possibly cast strings to intervals

    // INTERVALS
    // You can cast any DT interval to any other DT interval, or YM interval to any other YM interval.
    // But you cannot cast a DT interval to a YM interval or vice versa.
    ->pAddMapping(TypeId::xIdOf(IV_YEAR),            TM_ALL_YM_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_MONTH),           TM_ALL_YM_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_YEAR_TO_MONTH),   TM_ALL_YM_INTERVALS,           TypeMap2D::SAME_AS_ARG2)

    ->pAddMapping(TypeId::xIdOf(IV_DAY),             TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),            TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),          TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),          TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),     TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),   TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),   TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),  TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),  TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND),TM_ALL_DT_INTERVALS,           TypeMap2D::SAME_AS_ARG2)

    // Single field intervals can also be cast to integers, decimals and floats.
    ->pAddMapping(TypeId::xIdOf(IV_YEAR),            TM_ALL_NUMERICS,               TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_MONTH),           TM_ALL_NUMERICS,               TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_DAY),             TM_ALL_NUMERICS,               TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),            TM_ALL_NUMERICS,               TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),          TM_ALL_NUMERICS,               TypeMap2D::SAME_AS_ARG2)
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),          TM_ALL_NUMERICS,               TypeMap2D::SAME_AS_ARG2)

    // All intervals can be cast to a string. They could print something like:
    // "1 day, 2 hours, 3 minutes, 4.5 seconds"
//    ->pAddMapping(TM_ALL_DT_INTERVALS,            TypeId::xIdOf(CT_VARCHAR),        TypeMap2D::SAME_AS_ARG2)
//    ->pAddMapping(TM_ALL_DT_INTERVALS,            TypeId::xIdOf(CT_CHAR),           TypeMap2D::SAME_AS_ARG2)
//    ->pAddMapping(TM_ALL_YM_INTERVALS,            TypeId::xIdOf(CT_VARCHAR),        TypeMap2D::SAME_AS_ARG2)
//    ->pAddMapping(TM_ALL_YM_INTERVALS,            TypeId::xIdOf(CT_CHAR),           TypeMap2D::SAME_AS_ARG2)

    // NULL
    ->pAddMapping(TM_ALL_INC_UNKNOWN,             TypeId::xIdOf(CT_UNKNOWN),        TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN), TM_ALL_NON_UNKNOWN,                    TypeMap2D::SAME_AS_ARG2)
;

// References:
//
// Refer to section 6.12 of the SQL spec (<cast specification>).
// Partially summarised here:
//   /*?1*/ : When converting a decimal or integer to an interval then the interval must only contain a single field.
//   /*?2*/ : When converting an interval to a decimal or integer, the interval must only contain a single field.
//
//  - When casting to a number, if there is a loss of leading significant digits after any rounding/truncating
//    necessary to fit the value in the number then an "out of range" exception must be emitted.
//  - When casting a character type to a numeric, if the string does not contain a valid number representation
//    then an exception is raised.
//  - If casting a date to a timestamp then the time component of the timestamp is set to all zeros.
//  - If casting a time to a timestamp then the date component of the timestamp is set to the value of CURRENT_DATE.

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* CastExpr::pCreate(QueryNode* pTargetExpr, TypeId xTypeId, i8 iPrecision, i8 iScale)
{
  _static_
  ASSERT(pTargetExpr);
  ASSERT(Expr::bIsOfType(pTargetExpr));
  ASSERT(xTypeId.bIsValid());

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pCastExpr = new QueryNode(sName, C_DEFN);
  pCastExpr->vAppendChild(pTargetExpr);

  QueryNode* pTargetType = SqlType::pCreate(xTypeId, iPrecision, iScale);
  pTargetType->vSetName(C_TARGETTYPE_PROP);
  pCastExpr->vAppendProperty(pTargetType);

  return pCastExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void CastExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pTargetExpr = CastExpr::pTargetExpr(_pQueryNode(pNode));
  QueryNode* pTargetType = CastExpr::pSqlTypeNode(_pQueryNode(pNode));
  ASSERT(   Expr::bIsOfType(pTargetExpr));
  ASSERT(SqlType::bIsOfType(pTargetType));

  s2K sExprBuf;
  s2K sTypeBuf;

  Expr::vDescribe(pTargetExpr, sExprBuf, pRootNode);
  SqlType::vDescribe(pTargetType, sTypeBuf, pRootNode);

  sBuf.vCatSprint("CAST(%s AS %s)"_Stz, sExprBuf.szStz(), sTypeBuf.szStz());
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void CastExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pTargetExpr = CastExpr::pTargetExpr(_pQueryNode(pNode));
  QueryNode* pTargetType = CastExpr::pSqlTypeNode(_pQueryNode(pNode));
  ASSERT(   Expr::bIsOfType(pTargetExpr));
  ASSERT(SqlType::bIsOfType(pTargetType));

  s2K sTypeBuf;
  SqlType::vDescribe(pTargetType, sTypeBuf, pRootNode);

  sBuf.vCatSprint("CAST(%s AS %s)"_Stz,
                  Expr::xTypeId(pTargetExpr, pRootNode).szTypeName(),
                  sTypeBuf.szStz());
}

/*----------------------------------------------------------------------------*/
/* Return the property node containing the target type enumeration            */
/*----------------------------------------------------------------------------*/
QueryNode* CastExpr::pSqlTypeNode(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pTargetTypeNode = _pQueryNode(GraphNav::pFindProperty(C_TARGETTYPE_PROP, pNode));
  ASSERT(SqlType::bIsOfType(pTargetTypeNode));
  
  return pTargetTypeNode;
}

/*----------------------------------------------------------------------------*/
/* Return the expression node being cast                                      */
/*----------------------------------------------------------------------------*/
QueryNode* CastExpr::pTargetExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  QueryNode* pTargetExprNode = _pQueryNode(pNode->pFirstChild());
  ASSERT(Expr::bIsOfType(pTargetExprNode));

  return pTargetExprNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 CastExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 CastExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of -1 means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId CastExpr::xTypeId(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Get the types of the two operands
  QueryNode* pTargetExprNode = CastExpr::pTargetExpr(pNode);
  QueryNode* pSqlTypeNode    = CastExpr::pSqlTypeNode(pNode);

  TypeId xTargetExprTypeId = Expr::xTypeId(pTargetExprNode, pRootNode);

  TypeId xSqlType = SqlType::xTypeId(pSqlTypeNode);

  TypeId xResult = S_pTypeMap->xLookup(xTargetExprTypeId, xSqlType);

  return xResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 CastExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  QueryNode* pSqlTypeNode = CastExpr::pSqlTypeNode(pExprNode);
  ASSERT(SqlType::bIsOfType(pSqlTypeNode));

  i8 iVal = SqlType::iPrecision(pSqlTypeNode, pRootNode);
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 CastExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  QueryNode* pSqlTypeNode = CastExpr::pSqlTypeNode(pExprNode);
  ASSERT(SqlType::bIsOfType(pSqlTypeNode));

  i8 iVal = SqlType::iScale(pSqlTypeNode, pRootNode);
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* Returns true if an expr can be coerced to a given data type via a cast     */
/*----------------------------------------------------------------------------*/
b1 CastExpr::bIsImplicitlyCoercibleTo(TypeId xTargetTypeId, QueryNode* pExpr,
                                      QueryNode* pParentExpr, PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));

  TypeId xCurrentType = Expr::xTypeId(pExpr, pRootNode);
  eCT eTargetTypeId = xTargetTypeId.eCTVal();

  // We can always cast the current type to itself
  if (xCurrentType == xTargetTypeId) {
    ASSERT(S_pTypeMap->xLookup(xCurrentType, xTargetTypeId).bIsValid());
    return true;
  }

  // Note: We don't implicitly coerce strings to other data types because we
  //       aren't casting a literal that we know the format of. So it could
  //       introduce runtime errors if one row happens to have a value that
  //       cannot be converted. So we leave these conversions to be done
  //       explicitly by the user, not implicitly by the planner.

  switch (xCurrentType.eCTVal()) {
    case CT_INTEGER:
      if (eTargetTypeId == CT_DECIMAL || eTargetTypeId == CT_FLOAT)   {
        ASSERT(S_pTypeMap->xLookup(xCurrentType, xTargetTypeId).bIsValid());
        return true;
      }
      break;

    case CT_DECIMAL:
      if (eTargetTypeId == CT_FLOAT)
      {
        ASSERT(S_pTypeMap->xLookup(xCurrentType, xTargetTypeId).bIsValid());
        return true;
      }
      break;

    case CT_DATE:
      if (eTargetTypeId == CT_TIMESTAMP)
      {
        ASSERT(S_pTypeMap->xLookup(xCurrentType, xTargetTypeId).bIsValid());
        return true;
      }
      break;

    // Note: We don't implicitly cast a time to a timestamp because
    //       it may not be clear to the user what date should be chosen.
    //       Should it be TODAY, or 1900-01-01, or something else?
    //       An explicit cast by the user allows this, and uses the current date.

    case CT_BOOLEAN:
      if (eTargetTypeId == CT_VARCHAR || eTargetTypeId == CT_CHAR)
      {
        ASSERT(S_pTypeMap->xLookup(xCurrentType, xTargetTypeId).bIsValid());
        return true;
      }
      break;

    case CT_INTERVAL_YM:
    case CT_INTERVAL_DT:
      // We support the casting of a single field interval to an integer.
      switch (xCurrentType.eIVVal()) {
        case IV_YEAR:
        case IV_MONTH:
        case IV_DAY:
        case IV_HOUR:
        case IV_MINUTE:
        case IV_SECOND:
          if (eTargetTypeId == CT_INTEGER && pParentExpr && ComparisonExpr::bIsOfType(pParentExpr)) {

            // There must be at least one integer child and any interval children must all be
            // of the same type. Remember that a BETWEEN expr has more than 2 children.
            bool bFoundInteger = false;
            eIV eIvlType = IV_NONE;

            PlanNodeIter* pChildIter = pParentExpr->pNewChildrenIter();
            while (pChildIter->bHasNext()) {
              PlanNode* pChild = pChildIter->pNext();
              TypeId xChildType = Expr::xTypeId(_pQueryNode(pChild), pRootNode);

              if (xChildType.eCTVal() == CT_INTEGER) {
                bFoundInteger = true;
                continue;
              }
              if (xChildType.eCTVal() == CT_INTERVAL_YM || xChildType.eCTVal() == CT_INTERVAL_DT) {
                if (eIvlType == IV_NONE) {
                  eIvlType = xChildType.eIVVal();
                  continue;
                }

                if (eIvlType != xChildType.eIVVal()) {
                  // Found more than 1 interval type. This can occur in a BETWEEN expression.
                  delete pChildIter;
                  return false;
                }
              }

            }
            delete pChildIter;
            return bFoundInteger;
          }
          // TODO: Support casting an INTERVAL SECOND to a DECIMAL.
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns a new expression object which represents pExpr coerced to xTypeId  */
/*----------------------------------------------------------------------------*/
QueryNode* CastExpr::pCoerceTo(TypeId xTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode,
                               i8 iPrecision, i8 iScale)
{
  ASSERT(xTargetTypeId.bIsValid());
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));
  ASSERT(Expr::xTypeId(pExpr, pRootNode).bIsValid());
  ASSERT(S_pTypeMap->xLookup(Expr::xTypeId(pExpr, pRootNode), xTargetTypeId).bIsValid());
  UNUSED(pRootNode);

  QueryNode* pExprCopy = pExpr->pDeepCopy(true);
  QueryNode* pCastExpr = CastExpr::pCreate(pExprCopy, xTargetTypeId, iPrecision, iScale);
  return pCastExpr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
