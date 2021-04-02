/*============================================================================*/
/* ExplainOp.hpp - operator that explains the underlying plan                 */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ExplainOp : public SqlOperator
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ExplainOp();

  public:
    static QueryNode* pCreate(QueryNode* pStageRangeList, Stz* szFormat);
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);


    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
