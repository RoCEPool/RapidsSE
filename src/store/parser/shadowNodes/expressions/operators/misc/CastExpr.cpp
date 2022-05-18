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

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "CastExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/misc/cast"_Stz;

static Stz* C_TARGETTYPE_PROP = "targetType"_Stz;

// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the current expression (the 'from' type)
// * The top header row represents the target type of the cast operation (the 'to' type).
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with eCT::CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[eCT::CT_ENUMS][eCT::CT_ENUMS] = {

                 //   NONE     BOOLEAN     CHAR     DATE     DECIMAL         FLOAT     INTEGER         INTERVAL_DT      INTERVAL_YM      TIME     TIMESTAMP     VARCHAR     UNKNOWN
/*  0 NONE       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_NONE, eCT::CT_NONE, eCT::CT_NONE,        eCT::CT_NONE,  eCT::CT_NONE,        eCT::CT_NONE,         eCT::CT_NONE,         eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_NONE,    eCT::CT_NONE    },
/*  1 BOOLEAN    */ { eCT::CT_NONE, eCT::CT_BOOLEAN, eCT::CT_CHAR, eCT::CT_NONE, eCT::CT_NONE,        eCT::CT_NONE,  eCT::CT_NONE,        eCT::CT_NONE,         eCT::CT_NONE,         eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/*  2 CHAR       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_DATE, eCT::CT_DECIMAL,     eCT::CT_FLOAT, eCT::CT_INTEGER,     eCT::CT_INTERVAL_DT,  eCT::CT_INTERVAL_YM,  eCT::CT_TIME, eCT::eCT::CT_TIMESTAMP, eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/*  3 DATE       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_DATE, eCT::CT_NONE,        eCT::CT_NONE,  eCT::CT_NONE,        eCT::CT_NONE,         eCT::CT_NONE,         eCT::CT_NONE, eCT::eCT::CT_TIMESTAMP, eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/*  4 DECIMAL    */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_NONE, eCT::CT_DECIMAL,     eCT::CT_FLOAT, eCT::CT_INTEGER,     eCT::CT_NONE,/*?1*/   eCT::CT_NONE,/*?1*/   eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/*  5 FLOAT      */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_NONE, eCT::CT_DECIMAL,     eCT::CT_FLOAT, eCT::CT_INTEGER,     eCT::CT_NONE,         eCT::CT_NONE,         eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/*  6 INTEGER    */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_NONE, eCT::CT_DECIMAL,     eCT::CT_FLOAT, eCT::CT_INTEGER,     eCT::CT_NONE,/*?1*/   eCT::CT_NONE,/*?1*/   eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/*  7 INTERVAL_DT*/ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_NONE, eCT::CT_NONE/*?2*/,  eCT::CT_NONE,  eCT::CT_NONE/*?2*/,  eCT::CT_INTERVAL_DT,  eCT::CT_NONE,         eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/*  8 INTERVAL_YM*/ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_NONE, eCT::CT_NONE/*?2*/,  eCT::CT_NONE,  eCT::CT_NONE/*?2*/,  eCT::CT_NONE,         eCT::CT_INTERVAL_YM,  eCT::CT_NONE, eCT::CT_NONE,      eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/*  9 TIME       */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_NONE, eCT::CT_NONE,        eCT::CT_NONE,  eCT::CT_NONE,        eCT::CT_NONE,         eCT::CT_NONE,         eCT::CT_TIME, eCT::eCT::CT_TIMESTAMP, eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/* 10 TIMESTAMP  */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_DATE, eCT::CT_NONE,        eCT::CT_NONE,  eCT::CT_NONE,        eCT::CT_NONE,         eCT::CT_NONE,         eCT::CT_TIME, eCT::eCT::CT_TIMESTAMP, eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/* 11 VARCHAR    */ { eCT::CT_NONE, eCT::CT_NONE,    eCT::CT_CHAR, eCT::CT_DATE, eCT::CT_DECIMAL,     eCT::CT_FLOAT, eCT::CT_INTEGER,     eCT::CT_INTERVAL_DT,  eCT::CT_INTERVAL_YM,  eCT::CT_TIME, eCT::eCT::CT_TIMESTAMP, eCT::CT_VARCHAR, eCT::CT_UNKNOWN },
/* 12 UNKNOWN    */ { eCT::CT_NONE, eCT::CT_BOOLEAN, eCT::CT_CHAR, eCT::CT_DATE, eCT::CT_DECIMAL,     eCT::CT_FLOAT, eCT::CT_INTEGER,     eCT::CT_INTERVAL_DT,  eCT::CT_INTERVAL_YM,  eCT::CT_TIME, eCT::eCT::CT_TIMESTAMP, eCT::CT_VARCHAR, eCT::CT_UNKNOWN }
};

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

QueryNode* CastExpr::pCreate(QueryNode* pTargetExpr, i8 iTypeId)
{
  _static_
  ASSERT(pTargetExpr);
  ASSERT(Expr::bIsOfType(pTargetExpr));
  ASSERT(TypeSystem::bIsValid(iTypeId));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pCastExpr = new QueryNode(sName, C_DEFN);
  pCastExpr->vAppendChild(pTargetExpr);

  QueryNode* pTargetType = SqlType::pCreate(iTypeId);
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
i8 CastExpr::iType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Get the types of the two operands
  QueryNode* pTargetExprNode = CastExpr::pTargetExpr(pNode);
  QueryNode* pSqlTypeNode    = CastExpr::pSqlTypeNode(pNode);

  i8 iTargetExprTypeId = Expr::iType(pTargetExprNode, pRootNode);
  eCT eTargetExprType  = TypeSystem::eCTVal(iTargetExprTypeId);

  eCT eSqlType  = TypeSystem::eCTVal(SqlType::iTypeId(pSqlTypeNode));

  eCT eResultSrcType = aeTypeCalculus[eTargetExprType][eSqlType];

  // TODO: Handle intervals
  return TypeSystem::iIdOf(eResultSrcType);
}

/*----------------------------------------------------------------------------*/
/* Returns true if an expr can be coerced to a given data type via a cast     */
/*----------------------------------------------------------------------------*/
b1 CastExpr::bIsImplicitlyCoercibleTo(i8 iTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));

  i8 iCurrentType = Expr::iType(pExpr, pRootNode);
  eCT eCurrentType = TypeSystem::eCTVal(iCurrentType);

  eCT eTargetTypeId = TypeSystem::eCTVal(iTargetTypeId);

  // We can always cast the current type to itself
  if (iCurrentType == iTargetTypeId) {
    ASSERT(aeTypeCalculus[eCurrentType][eTargetTypeId]);
    return true;
  }

  // Note: We don't implicitly coerce strings to other data types because we
  //       aren't casting a literal that we know the format of. So it could
  //       introduce runtime errors if one row happens to have a value that
  //       cannot be converted. So we leave these conversions to be done
  //       explicitly by the user, not implicitly by the planner.

  switch (eCurrentType) {
    case eCT::CT_INTEGER:
      if (eTargetTypeId == eCT::CT_DECIMAL || eTargetTypeId == eCT::CT_FLOAT)   {
        ASSERT(aeTypeCalculus[eCurrentType][eTargetTypeId]);
        return true;
      }
      break;

    case eCT::CT_DECIMAL:
      if (eTargetTypeId == eCT::CT_FLOAT)
      {
        ASSERT(aeTypeCalculus[eCurrentType][eTargetTypeId]);
        return true;
      }
      break;

    case eCT::CT_DATE:
      if (eTargetTypeId == eCT::eCT::CT_TIMESTAMP)
      {
        ASSERT(aeTypeCalculus[eCurrentType][eTargetTypeId]);
        return true;
      }
      break;

    case eCT::CT_BOOLEAN:
      if (eTargetTypeId == eCT::CT_VARCHAR || eTargetTypeId == eCT::CT_CHAR)
      {
        ASSERT(aeTypeCalculus[eCurrentType][eTargetTypeId]);
        return true;
      }
      break;

    default:
      break;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns a new expression object which represents pExpr coerced to iTypeId  */
/*----------------------------------------------------------------------------*/
QueryNode* CastExpr::pCoerceTo(i8 iTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode)
{
  ASSERT(TypeSystem::bIsValid(iTargetTypeId));
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));
  ASSERT(TypeSystem::bIsValid(Expr::iType(pExpr, pRootNode)));

  // TODO: Can we do this in terms of iTypeId?
  eCT eTargetType = TypeSystem::eCTVal(iTargetTypeId);
  ASSERT(aeTypeCalculus[Expr::iType(pExpr, pRootNode)][eTargetType]);

  QueryNode* pExprCopy = pExpr->pDeepCopy(true);
  QueryNode* pCastExpr = CastExpr::pCreate(pExprCopy, iTargetTypeId);
  return pCastExpr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
