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
    static QueryNode* pCreate(Stz* szDefn, eJT eJoinType, QueryNode* pPredicate, QueryNode* pUsingList);

  public:
    static void vDescribe(PlanNode *pNode, s2K &sBuf, PlanNode *pRootNode);
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
    static QueryNode* pOrdinalJoinPredicate(PlanNode* pNode, PlanNode* pRootNode);

    // Return the internal node that represents the USING column list. May be null.
    static QueryNode* pUsingListNode(PlanNode* pNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Returns the number of projected columns from each child.
    // Useful for working with ordinals.
    static i8 iColumnsOfChild1(QueryNode* pNode, PlanNode* pRootNode);
    static i8 iColumnsOfChild2(QueryNode* pNode, PlanNode* pRootNode);

    // Replace the USING column list with a join predicate
    static void vReplaceUsing(QueryNode* pNode, QueryNode* pJoinPredicate);

    // Returns true if this operator (and any expressions it may have) should be
    // partitioned when the plan is partitioned. Most operators will simply ask
    // this question of their child until we get to a data source. Most tables
    // would answer true. A replicated table or data source would answer false.
    static b1 bIsPartitionable(QueryNode* pNode, PlanNode* pRootNode);

  protected:
    // Delete all of the non-common properties for this join
    static void vDeleteNonCommonProperties(QueryNode* pNode, PlanNode* pRootNode);

  private:
    static QueryNodeList* pDistinctUnionedColumnExprs(QueryNode* pNode, PlanNode* pRootNode);
    static b1 bHasSameNamedColumn(QueryNodeList* pProjColList, QueryNode* pProjectedCol);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
