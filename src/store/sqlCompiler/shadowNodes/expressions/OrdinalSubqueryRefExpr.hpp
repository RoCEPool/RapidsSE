/*============================================================================*/
/* OrdinalOrdinalSubqueryRefExpr.hpp - a temp ordinal form of SubqueryRefExpr */
/*============================================================================*/


#pragma once

#include "SubqueryRefExpr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class OrdinalSubqueryRefExpr : public SubqueryRefExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    OrdinalSubqueryRefExpr();

  public:
    static QueryNode* pCreate(QueryNode* pSymbolicSubqueryRefExpr, QueryNodeList* pOrdinalParams);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns true if the expression does not contain any column references.
    // All expressions are relative column ordinals, operators or literals only.
    static b1 bIsFullyOrdinal(QueryNode* pExpr);

    // *** Internal use only ***
    // Return a new version of this expression, converted from ordinal to symbolic notation.
    static QueryNode* pOrdinalToSymbolic(QueryNode*     pOrdinalExpr,
                                         QueryNodeList* pColumnExprList,
                                         PlanNode*      pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
