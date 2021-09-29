/*============================================================================*/
/* SubqueryExpr.hpp - an expression node that wraps a subquery                */
/*============================================================================*/
#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class SubqueryExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SubqueryExpr();

  public:
    static QueryNode* pCreate();

    static void  vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1    bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT   eType(QueryNode* pNode, PlanNode* pRootNode);

    // Returns the path where the definition of this subquery is defined
    static Stz* szDefnPath(QueryNode* pNode);

    // Returns true if this subquery is correlated or false if it is not.
    static b1 bIsCorrelated(QueryNode* pNode);

    // Returns an iterator of the correlated expressions in this subquery.
    // Iterator must be freed after use.
    static PlanNodeIter* pCorrelatedExprsIter(QueryNode* pNode);

  private:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
