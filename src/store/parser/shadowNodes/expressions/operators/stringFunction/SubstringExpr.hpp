/*============================================================================*/
/* SubstringExpr.hpp - a SUBSTRING expression operator                        */
/*============================================================================*/
#pragma once

#include "StringFunctionExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class SubstringExpr : public StringFunctionExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SubstringExpr();

  public:
    static QueryNode* pCreate(QueryNode* pSrcExpr, QueryNode* pFromExpr, QueryNode* pLengthExpr = nullptr);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    static QueryNode* pSrcExpr(QueryNode* pNode);
    static QueryNode* pFromExpr(QueryNode* pNode);
    static QueryNode* pLengthExpr(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1   bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);

    // Modifies subexpressions by promoting/casting data types so that they
    // are type compatible with this expression. Modifies the expression
    // tree in place.
    static void vCoerceSubExprs(QueryNode* pSubstringExpr, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
