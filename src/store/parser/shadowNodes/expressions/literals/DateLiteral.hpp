/*============================================================================*/
/* DateLiteral.hpp - an date literal expression                               */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class DateLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    DateLiteral();

  public:
    static QueryNode* pCreate(Dte xVal);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static Dte        xValue(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if the given string validly represents a date
    static b1 bIsDateStr(Stz* szStr);

    // Parse a string into a Dte, setting bResult to true if successful
    static Dte xParseStr(Stz* szStr, b1& bResult);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
