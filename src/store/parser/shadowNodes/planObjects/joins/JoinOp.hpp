/*============================================================================*/
/* JoinOp.hpp - a base definition of a join operation                         */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"
#include "eJT.h"

namespace Parser
{
  class JoinOp : public SqlOperator
  {

  private:
    // Shalt never be instantiated. Static functions only.
    JoinOp();

  protected:
    static QueryNode* pCreate(Stz* szDefn, eJT eJoinType, QueryNode* pPredicate);

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

    // Return the join type enumeration (inner, left outer, etc)
    static eJT eJoinType(PlanNode* pNode);

    // Return the node that represents the join type
    static QueryNode* pJoinType(PlanNode* pNode);

    // Return the node representing the join predicate expression
    static QueryNode* pJoinPredicate(PlanNode* pNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

  private:
    static QueryNodeList* pUnionedColumnExprs(QueryNode* pNode, PlanNode* pRootNode);
    static QueryNodeList* pDistinctUnionedColumnExprs(QueryNode* pNode, PlanNode* pRootNode);
    static b1 bHasSameNamedColumn(QueryNodeList* pProjColList, QueryNode* pProjectedCol);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
