/*============================================================================*/
/* IntervalLiteral.hpp - a general interval literal expression                */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"
#include "TypeId.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class IntervalLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    IntervalLiteral();

  public:
    static QueryNode* pCreate(Ivl xVal);
    static QueryNode* pCreateNull();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static Ivl  xValue(PlanNode* pNode);
    static void vSetValue(PlanNode* pNode, Ivl xVal);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);

    // Returns the specific type of interval subtype
    static eIV eIvlType(QueryNode* pNode);

    // Convenience method to get the broad sub-category of interval.
    // Should be either a year-month or day-time column type.
    static eCT eIvlCategory(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
