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
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void       vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns true if any of the projected expressions are aggregate expressions
    static b1 bHasAnAggregate(QueryNode* pNode);

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

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
