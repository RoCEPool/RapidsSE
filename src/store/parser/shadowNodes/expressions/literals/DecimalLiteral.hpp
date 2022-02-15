/*============================================================================*/
/* DecimalLiteral.hpp - an decimal literal expression                         */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class DecimalLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    DecimalLiteral();

  public:
    static QueryNode* pCreate(Bcd xVal);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static Bcd  xValue(PlanNode* pNode);
    static void vSetValue(PlanNode* pNode, Bcd xVal);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT eType(QueryNode* pNode, PlanNode* pRootNode);

    // Parse a string into a Bcd, setting bResult to true if successful
    static Bcd xParseStr(Stz* szStr, b1& bResult);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
