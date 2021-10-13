/*============================================================================*/
/* SubqueryMarker.hpp - Marks the start of some type of subquery              */
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/planObjects/planMarkers/PlanMarker.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class SubqueryMarker : public PlanMarker
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SubqueryMarker();

  protected:
    static QueryNode* pCreate(Stz* szDefn, Stz* szSqlText, Stz* szAliasName);

  public:
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

    // Returns the path of the definition node of this table subquery (a CteDef object).
    // Path may be null early in the plan.
    static Stz* szDefnPath(QueryNode* pNode);

    // The textual representation of this subquery
    static Stz* szSqlText(QueryNode* pNode);

    // Returns the node that represents the CteDef of this object.
    // May be null early in the plan.
    static QueryNode* pCteDefn(QueryNode* pNode, PlanNode* pRootNode);

    // Sets the path of the node that represents the metadata of this CTE.
    static void vSetDefnPath(QueryNode* pNode, Stz* szPath);

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
