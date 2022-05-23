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
    static b1   bIsNull(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1   bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if an expression can be coerced to a given data type
    static b1 bIsImplicitlyCoercibleTo(TypeId xTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode);

    // Returns a new expression object which represents pExpr coerced to eTarget
    static QueryNode* pCoerceTo(TypeId xTargetTypeId, QueryNode* pExpr, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
