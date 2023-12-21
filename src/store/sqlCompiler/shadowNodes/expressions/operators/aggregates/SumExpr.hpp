/*============================================================================*/
/* SumExpr.hpp - an SUM() aggregate expression                                */
/*============================================================================*/
#pragma once

#include "AggregateExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class SumExpr : public AggregateExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SumExpr();

  public:
    static QueryNode* pCreate(b1 bIsDistinct, QueryNode* pChildExpr);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Represent the expression as types instead of names (e.g. INTEGER + INTEGER)
    static void vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    static QueryNode* pChild1(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);

    // Returns the precision of the resulting expression. Returns -1 if not set or n/a.
    static i8 iPrecision(QueryNode* pExprNode, PlanNode* pRootNode);

    // Returns the scale of the resulting expression. Returns -1 if not set or n/a.
    static i8 iScale(QueryNode* pExprNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
