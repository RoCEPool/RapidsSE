/*============================================================================*/
/* ExprSubquery.hpp - Identifies a subquery emanating from an expression      */
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/SubqueryMarker.hpp"

namespace Parser
{
  class QueryNode;

  class ExprSubquery : public SubqueryMarker
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ExprSubquery();

  public:
    static QueryNode* pCreate(Stz* szAliasName);
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Returns the name of the virtual table that this subquery is known as
    static Stz* szTableAlias(QueryNode* pNode);

    // Returns true if the subquery is correlated with respect to its parent subquery
    static b1 bIsCorrelated(QueryNode* pNode);

    // Returns the internal list of correlated columns. May be an empty list.
    static QueryNode* pCorrelatedColumns(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
