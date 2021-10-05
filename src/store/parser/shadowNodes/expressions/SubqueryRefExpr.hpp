/*============================================================================*/
/* SubqueryRefExpr.hpp - an expression node that references a subquery        */
/*============================================================================*/

// Note: This class is named SubqueryRefExpr to reflect the fact that while it
//       is a reference to a subquery that exists elsewhere in the plan, it is
//       still an expression object.

#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class SubqueryRefExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SubqueryRefExpr();

  public:
    static QueryNode* pCreate(Stz* szSubqueryAlias);

    static void  vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1    bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT   eType(QueryNode* pNode, PlanNode* pRootNode);

    // Return the alias that this subquery is known as
    static Stz* szTableAlias(QueryNode* pNode);

    // Return the node that this SubqueryRefExpr is referencing
    static QueryNode* pSubquery(QueryNode* pNode, PlanNode* pRootNode);

  private:
    // Return the CteDef node that represents this subquery's definition.
    // If psPathBuf is not null then the path of the CteDef is written into it.
    static QueryNode* pDefnNode(QueryNode* pNode, PlanNode* pRootNode, s2K* psPathBuf = nullptr);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
