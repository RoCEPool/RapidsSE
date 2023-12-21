/*============================================================================*/
/* DecimalLiteral.hpp - an decimal literal expression                         */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"
#include "TypeId.hpp"

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
    static QueryNode* pCreateNull();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static Bcd  xValue(PlanNode* pNode);
    static void vSetValue(PlanNode* pNode, Bcd xVal);

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

    // Parse a string into a Bcd, throwing an exception if there is a problem
    static Bcd xParseStr(Stz* szStr);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
