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

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "CastExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/misc/cast"_Stz;

static Stz* C_TARGETTYPE_PROP = "targetType"_Stz;

// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the target expression.
// * The top header row represents the target type of the cast operation.
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[CT_ENUMS][CT_ENUMS] = {

                 //   NONE     BOOLEAN     CHAR     DATE     DECIMAL         FLOAT     INTEGER         INTERVAL_DT      INTERVAL_YM      TIME     TIMESTAMP     VARCHAR     UNKNOWN
/*  0 NONE       */ { CT_NONE, CT_NONE,    CT_NONE, CT_NONE, CT_NONE,        CT_NONE,  CT_NONE,        CT_NONE,         CT_NONE,         CT_NONE, CT_NONE,      CT_NONE,    CT_NONE    },
/*  1 BOOLEAN    */ { CT_NONE, CT_BOOLEAN, CT_CHAR, CT_NONE, CT_NONE,        CT_NONE,  CT_NONE,        CT_NONE,         CT_NONE,         CT_NONE, CT_NONE,      CT_VARCHAR, CT_UNKNOWN },
/*  2 CHAR       */ { CT_NONE, CT_NONE,    CT_CHAR, CT_DATE, CT_DECIMAL,     CT_FLOAT, CT_INTEGER,     CT_INTERVAL_DT,  CT_INTERVAL_YM,  CT_TIME, CT_TIMESTAMP, CT_VARCHAR, CT_UNKNOWN },
/*  3 DATE       */ { CT_NONE, CT_NONE,    CT_CHAR, CT_DATE, CT_NONE,        CT_NONE,  CT_NONE,        CT_NONE,         CT_NONE,         CT_NONE, CT_TIMESTAMP, CT_VARCHAR, CT_UNKNOWN },
/*  4 DECIMAL    */ { CT_NONE, CT_NONE,    CT_CHAR, CT_NONE, CT_DECIMAL,     CT_FLOAT, CT_INTEGER,     CT_NONE,/*?1*/   CT_NONE,/*?1*/   CT_NONE, CT_NONE,      CT_VARCHAR, CT_UNKNOWN },
/*  5 FLOAT      */ { CT_NONE, CT_NONE,    CT_CHAR, CT_NONE, CT_DECIMAL,     CT_FLOAT, CT_INTEGER,     CT_NONE,         CT_NONE,         CT_NONE, CT_NONE,      CT_VARCHAR, CT_UNKNOWN },
/*  6 INTEGER    */ { CT_NONE, CT_NONE,    CT_CHAR, CT_NONE, CT_DECIMAL,     CT_FLOAT, CT_INTEGER,     CT_NONE,/*?1*/   CT_NONE,/*?1*/   CT_NONE, CT_NONE,      CT_VARCHAR, CT_UNKNOWN },
/*  7 INTERVAL_DT*/ { CT_NONE, CT_NONE,    CT_CHAR, CT_NONE, CT_NONE/*?2*/,  CT_NONE,  CT_NONE/*?2*/,  CT_INTERVAL_DT,  CT_NONE,         CT_NONE, CT_NONE,      CT_VARCHAR, CT_UNKNOWN },
/*  8 INTERVAL_YM*/ { CT_NONE, CT_NONE,    CT_CHAR, CT_NONE, CT_NONE/*?2*/,  CT_NONE,  CT_NONE/*?2*/,  CT_NONE,         CT_INTERVAL_YM,  CT_NONE, CT_NONE,      CT_VARCHAR, CT_UNKNOWN },
/*  9 TIME       */ { CT_NONE, CT_NONE,    CT_CHAR, CT_NONE, CT_NONE,        CT_NONE,  CT_NONE,        CT_NONE,         CT_NONE,         CT_TIME, CT_TIMESTAMP, CT_VARCHAR, CT_UNKNOWN },
/* 10 TIMESTAMP  */ { CT_NONE, CT_NONE,    CT_CHAR, CT_DATE, CT_NONE,        CT_NONE,  CT_NONE,        CT_NONE,         CT_NONE,         CT_TIME, CT_TIMESTAMP, CT_VARCHAR, CT_UNKNOWN },
/* 11 VARCHAR    */ { CT_NONE, CT_NONE,    CT_CHAR, CT_DATE, CT_DECIMAL,     CT_FLOAT, CT_INTEGER,     CT_INTERVAL_DT,  CT_INTERVAL_YM,  CT_TIME, CT_TIMESTAMP, CT_VARCHAR, CT_UNKNOWN },
/* 12 UNKNOWN    */ { CT_NONE, CT_BOOLEAN, CT_CHAR, CT_DATE, CT_DECIMAL,     CT_FLOAT, CT_INTEGER,     CT_INTERVAL_DT,  CT_INTERVAL_YM,  CT_TIME, CT_TIMESTAMP, CT_VARCHAR, CT_UNKNOWN }
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

QueryNode* CastExpr::pCreate(QueryNode* pTargetExpr, eCT eTargetType)
{
  _static_
  ASSERT(pTargetExpr);
  ASSERT(Expr::bIsOfType(pTargetExpr));
  ASSERT(eTargetType > CT_NONE);
  ASSERT(eTargetType < CT_ENUMS);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pCastExpr = new QueryNode(sName, C_DEFN);
  pCastExpr->vAppendChild(pTargetExpr);

  QueryNode* pTargetType = SqlType::pCreate(eTargetType);
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
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
eCT CastExpr::eType(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Get the types of the two operands
  QueryNode* pTargetExprNode = CastExpr::pTargetExpr(pNode);
  QueryNode* pSqlTypeNode    = CastExpr::pSqlTypeNode(pNode);

  eCT eExprType = Expr::eType(pTargetExprNode, pRootNode);
  eCT eSqlType  = SqlType::eType(pSqlTypeNode);

  eCT eResultSrcType = aeTypeCalculus[eExprType][eSqlType];
  return eResultSrcType;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
