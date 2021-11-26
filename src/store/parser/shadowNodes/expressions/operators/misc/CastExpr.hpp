/*============================================================================*/
/* CastExpr.hpp - an expression operator for converting the returning type    */
/*============================================================================*/
#pragma once

#include "MiscExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class CastExpr : public MiscExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    CastExpr();

  public:
    static QueryNode* pCreate(QueryNode* pTargetExpr, eCT eTargetType);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    static QueryNode* pSqlTypeNode(QueryNode* pNode);
    static QueryNode* pTargetExpr(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1   bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static eCT  eType(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if an expression can be coerced to a given data type via a cast
    static b1 bIsCoercibleTo(eCT eTarget, QueryNode* pExpr, PlanNode* pRootNode);

    // Returns a new expression object which represents pExpr coerced to eTarget
    static QueryNode* pCoerceTo(eCT eTarget, QueryNode* pExpr, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
