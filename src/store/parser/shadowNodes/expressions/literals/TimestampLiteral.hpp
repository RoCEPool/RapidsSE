/*============================================================================*/
/* TimestampLiteral.hpp - a timestamp literal expression                      */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"
#include "TypeId.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class TimestampLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    TimestampLiteral();

  public:
    static QueryNode* pCreate(Tsp xTsp);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static Tsp        xValue(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if the given string validly represents a timestamp
    static b1 bIsTimestampStr(Stz* szStr);

    // Parse a string into a Tsp, setting bResult to true if successful
    static Tsp xParseStr(Stz* szStr, b1& bResult);

  private:
    static b1 bHasMicros(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
