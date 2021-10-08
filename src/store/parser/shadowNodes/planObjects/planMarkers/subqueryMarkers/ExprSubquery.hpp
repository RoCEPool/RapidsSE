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
    static QueryNode* pCreate(Stz* szAliasName, Stz* szSqlText);
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
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
