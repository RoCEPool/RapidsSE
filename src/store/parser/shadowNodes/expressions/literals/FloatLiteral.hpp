/*============================================================================*/
/* FloatLiteral.hpp - a float literal expression                              */
/*============================================================================*/
#pragma once

#include "LiteralExpr.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class FloatLiteral : public LiteralExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    FloatLiteral();

  public:
    static QueryNode* pCreate(d8 dVal);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static d8   dValue(PlanNode* pNode);
    static void vSetValue(PlanNode* pNode, d8 dVal);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT eType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
