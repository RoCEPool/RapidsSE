/*============================================================================*/
/* Hashcast.hpp - Send data to a single endpoint based on some hashable expr  */
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/types/routing/Routing.hpp"


namespace Parser
{
  class PlanNode;
  class QueryNode;

  class Hashcast : public Routing
  {
  private:
    // Shalt never be instantiated. Static functions only.
    Hashcast();


  public:
    static QueryNode* pCreate();
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Adds an expression to the list of expressions to hash for destination ptn
    static void vAddHashExpr(QueryNode* pNode, QueryNode* pExpr);

    // Returns the internal node that represents a list of hashable expressions
    static QueryNode* pHashExprList(QueryNode* pNode);

    // Returns a copy of the node that represents a list of hashable expressions
    // where each expression is in ordinal form. The node must be freed after use.
    static QueryNode* pOrdinalHashExprList(QueryNode* pNode, PlanNode* pRootNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
