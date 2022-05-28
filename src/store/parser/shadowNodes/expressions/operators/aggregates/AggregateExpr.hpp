/*============================================================================*/
/* AggregateExpr.hpp - a aggregate expression                                     */
/*============================================================================*/
#pragma once

#include "Expr.hpp"
#include "TypeId.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class AggregateExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    AggregateExpr();

  protected:
    static const Stz* C_DISTINCT_PROP_NAME;
    static const Stz* C_DISTINCT_PROP_TYPE;
    static QueryNode* pChild1(QueryNode* pNode);

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static b1   bIsDistinct(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
