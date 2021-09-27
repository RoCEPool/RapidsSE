/*============================================================================*/
/* TableSubquery.hpp - Identifies a subquery coming from a WITH or FROM clause*/
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/planObjects/planMarkers/subqueryMarkers/SubqueryMarker.hpp"

namespace Parser
{
  class QueryNode;

  class TableSubquery : public SubqueryMarker
  {
  private:
    // Shalt never be instantiated. Static functions only.
    TableSubquery();

  public:
    static QueryNode* pCreate(Text* pTableAlias, QueryNode* pColumnNames, b1 bIsRecursive);
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

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Returns true if the subquery is correlated with respect to its parent subquery
    static b1 bIsCorrelated(QueryNode* pNode);

    // Returns the name of the virtual table that this subquery is known as
    static Stz* szTableAlias(QueryNode* pNode);

    // Returns the internal node that contains the list of column names
    static QueryNode* pColumnNames(QueryNode* pNode);

    // Returns the number of named column names
    static i8 nColumnNames(QueryNode* pNode);

    // Returns true if this subquery has been defined to be recursive
    static b1 bIsRecursive(QueryNode* pNode);

    // Find and return the main subquery node that exists underneath this table subquery node.
    static QueryNode* pChildMainSubquery(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
