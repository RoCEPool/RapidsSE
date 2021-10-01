/*============================================================================*/
/* SubqueryExpr.hpp - an expression node that wraps a subquery                */
/*============================================================================*/

// Note: This class is named SubqueryExpr, not ExprSubquery, to recognise that
//       it is fundamentally an Expr element first and a wrapper for a subquery
//       second. Similar to how we name expression operators (e.g. AdditionExpr).
//       We may talk about "expression subqueries", but the definition and shadow
//       node that implement them are named SubqueryExpr.

#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class SubqueryExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SubqueryExpr();

  public:
    static QueryNode* pCreate(QueryNode* pSubqueryContainer);

    static void  vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1    bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT   eType(QueryNode* pNode, PlanNode* pRootNode);

    // Returns the path where the definition of this subquery is defined
    static Stz* szDefnPath(QueryNode* pNode);

    // Sets the path of the node that represents the metadata of this CTE.
    static void vSetDefnPath(QueryNode* pNode, Stz* szPath);

    // Returns true if this subquery is correlated or false if it is not.
    static b1 bIsCorrelated(QueryNode* pNode);

    // Returns an iterator of the correlated expressions in this subquery.
    // Iterator must be freed after use.
    static PlanNodeIter* pCorrelatedExprsIter(QueryNode* pNode);

    // Return the node that is the root of the subquery plan graph
    static QueryNode* pSubqueryContainer(QueryNode* pNode);

    // Return the name of the property that is the root of the subquery plan graph
    static Stz* szSubqueryContainerProp(void);

    // Find and return the main subquery node that exists underneath this expression subquery node.
    static QueryNode* pChildMainSubquery(QueryNode* pNode);

  private:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
