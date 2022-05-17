/*============================================================================*/
/* OrExpr.hpp - a logical OR expression operator                              */
/*============================================================================*/
#pragma once

#include "LogicExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class OrExpr : public LogicExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    OrExpr();

  public:
    static QueryNode* pCreate(QueryNode* pExpr1, QueryNode* pExpr2);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    static QueryNode* pChild1(QueryNode* pNode);
    static QueryNode* pChild2(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
