/*============================================================================*/
/* ProjectOp.hpp - operator that changes the defn of rows that it processes   */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ProjectOp : public SqlOperator
  {
  protected:
    static Stz* C_PROJECTEDCOLUMNS_PROP;
    static Stz* C_PROJECTEDCOLUMNS_DEFN;

  private:
    // Shalt never be instantiated. Static functions only.
    ProjectOp();

  public:
    static QueryNode* pCreate(QueryNodeList* pSelectItems);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static void       vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Returns true if any of the projected expressions are aggregate expressions
    static b1 bHasAnAggregate(QueryNode* pNode);

    // Returns true if all of the projected expressions are aggregate expressions
    static b1 bHasOnlyAggregates(QueryNode* pNode, b1 bAllowLiterals);

    // Return the internal node that represents the list of projected expressions.
    // Individual projected expressions are children of this node.
    static QueryNode* pProjectedColumns(QueryNode* pNode);

    // Returns a new copy of the projected expressions rewritten relative to column ordinals.
    // Must be freed after use.
    static QueryNode* pOrdinalProjectedColumns(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    // This is conceptually the same as pProjectedColumns() above, except this
    // function is implemented by all PlanObjects.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Merges the ProjectOp, pOtherOp, into this ProjectOp
    static void vSquash(QueryNode* pNode, QueryNode* pOtherOp, PlanNode* pRootNode);

    // Adds an expression to be projected, with an optional alias
    static void vPushdownExpr(QueryNode* pNode, QueryNode* pExpr, Text* pAlias);

    // Returns a new ProjectOp node with projected columns modified for
    // partitioned aggregate expressions
    static QueryNode* pCreatePartitioned(QueryNode* pNode);

    // Returns a new ProjectOp node with projected columns super-aggregating aggregated
    // expressions from each partition or node
    static QueryNode* pCreateSuperAggregate(QueryNode* pSinglePartitionNode);

    // Returns a new ProjectOp node with projected columns doubly super-aggregating aggregated
    // expressions from each partition or node. Used to factor in node-level aggregation.
    static QueryNode* pCreateDoubleSuperAggregate(QueryNode* pSinglePartitionNode);

    // Returns the number of columns being projected
    static i8 nColumns(QueryNode* pNode);

    // Returns a ptr to the internal ProjectedColumn node (not Expr) that contains
    // a symbolically equal expression as pExpr, or nullptr
    static QueryNode* pColumnWithExpr(QueryNode* pNode, QueryNode* pExpr);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
