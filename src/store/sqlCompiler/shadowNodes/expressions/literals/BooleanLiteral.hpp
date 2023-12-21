/*============================================================================*/
/* BooleanLiteral.hpp - an boolean literal expression                         */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"
#include "TypeId.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class BooleanLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    BooleanLiteral();

  public:
    static QueryNode* pCreate(b1 bVal);
    static QueryNode* pCreateNull();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static b1         bValue(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
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
