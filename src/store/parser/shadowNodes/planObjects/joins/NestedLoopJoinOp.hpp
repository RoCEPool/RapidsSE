/*============================================================================*/
/* NestedLoopJoinOp.hpp - nested loop join algorithm                          */
/*============================================================================*/
#pragma once

#include "JoinOp.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class NestedLoopJoinOp : public JoinOp
  {
  private:
    // Shalt never be instantiated. Static functions only.
    NestedLoopJoinOp();

  public:
    static QueryNode* pCreate(eJT eJoinType, QueryNode* pJoinPredicate, QueryNode* pUsingList);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Converts pJoinOp to a nested loop join
    static void vConvertJoinAlgorithm(QueryNode* pJoinOp, PlanNode* pRootNode);
  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
