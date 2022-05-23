/*============================================================================*/
/* IntegerLiteral.hpp - an integer literal expression                         */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"
#include "TypeId.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class IntegerLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    IntegerLiteral();

  public:
    static QueryNode* pCreate(i8 iVal);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static i8   iValue(PlanNode* pNode);
    static void vSetValue(PlanNode* pNode, i8 iVal);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
