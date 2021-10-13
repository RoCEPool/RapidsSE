/*============================================================================*/
/* ColumnParameter.hpp - A reference to a column by its parameter num (1-based)*/
/*============================================================================*/
#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ColumnParameter : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ColumnParameter();

  public:
    static QueryNode* pCreate(i8 iOrdinal, QueryNode* pSymExpr);

    static void  vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1    bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT   eType(QueryNode* pNode, PlanNode* pRootNode);

    // Returns the 1-based parameter number of this column
    static i8 iParameter(QueryNode* pNode);

    // Returns the internal columnExpr node that is represented by this parameter
    static QueryNode* pColumnExpr(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
