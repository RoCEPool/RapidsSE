/*============================================================================*/
/* TimeLiteral.hpp - a time literal expression                                */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class TimeLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    TimeLiteral();

  public:
    static QueryNode* pCreate(Tme xTme);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static Tme        xValue(PlanNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if the given string validly represents a time
    static b1 bIsTimeStr(Stz* szStr);

    // Parse a string into a Tme, setting bResult to true if successful
    static Tme xParseStr(Stz* szStr, b1& bResult);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
