/*============================================================================*/
/* LiteralExpr.hpp - a literal expression                                     */
/*============================================================================*/
#pragma once

#include "Expr.hpp"
#include "TypeId.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class LiteralExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    LiteralExpr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Represent the expression as types instead of names (e.g. INTEGER + INTEGER)
    static void vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    static b1   bIsNull(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1   bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if an expression can be coerced to a given data type
    static b1 bIsImplicitlyCoercibleTo(TypeId xTargetTypeId, QueryNode* pExpr,
                                       QueryNode* pParentExpr, PlanNode* pRootNode);
    static b1 bIsImplicitlyCoercibleTo(eIV eCurrentIvl, TypeId xTargetTypeId,
                                       QueryNode* pParentExpr, PlanNode* pRootNode);

    // Returns a new expression object which represents pExpr coerced to eTarget
    static QueryNode* pCoerceTo(TypeId xTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode);

  private:
    // Helper functions when converting literals
    static QueryNode* pLiteralToVarchar(QueryNode* pSrc, PlanNode* pRootNode);

    static QueryNode* pDateToTimestamp(QueryNode* pSrc, PlanNode* pRootNode);

    static QueryNode* pDecimalToFloat(QueryNode* pSrc, PlanNode* pRootNode);
    static QueryNode* pDecimalToInteger(QueryNode* pSrc, PlanNode* pRootNode);

    static QueryNode* pFloatToDecimal(QueryNode* pSrc, PlanNode* pRootNode);

    static QueryNode* pIntegerToDecimal(QueryNode* pSrc, PlanNode* pRootNode);

    static QueryNode* pIntervalToDecimal(QueryNode* pSrc, PlanNode* pRootNode, eIV eCurrentIVType);
    static QueryNode* pIntervalToFloat(QueryNode* pSrc, PlanNode* pRootNode, eIV eCurrentIVType);
    static QueryNode* pIntervalToInteger(QueryNode* pSrc, PlanNode* pRootNode, eIV eCurrentIVType);
    static QueryNode* pIntervalToInterval(QueryNode* pSrc, PlanNode* pRootNode, eIV eTargetIVType);

    static QueryNode* pTimestampToDate(QueryNode* pSrc, PlanNode* pRootNode);
    static QueryNode* pTimestampToTime(QueryNode* pSrc, PlanNode* pRootNode);

    static QueryNode* pVarcharToDate(QueryNode* pSrc, PlanNode* pRootNode);
    static QueryNode* pVarcharToDecimal(QueryNode* pSrc, PlanNode* pRootNode);
    static QueryNode* pVarcharToFloat(QueryNode* pSrc, PlanNode* pRootNode);
    static QueryNode* pVarcharToInteger(QueryNode* pSrc, PlanNode* pRootNode);
    static QueryNode* pVarcharToTime(QueryNode* pSrc, PlanNode* pRootNode);
    static QueryNode* pVarcharToTimestamp(QueryNode* pSrc, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
