/*============================================================================*/
/* HintFinder.hpp - a utility for finding hints that are in scope in the plan */
/*============================================================================*/
#pragma once

#include <functional> // for std::function
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  // Note: This class is for searching for hints within the plan hierarchy.
  //       For finding hints that are specific to a certain node, see PlanObject::pGetHint()
  class QueryNode;

  class HintFinder
  {
  private:
    HintFinder();

  public:

    // Looks up and returns the first hint that matches the hint key from
    // pSqlOp upwards to the root node. This can be a slow operation as we
    // don't know the path of pSqlOp within the graph & have to find this first.
    // Returned node* is internal and should not be modified/freed.
    static QueryNode* pFindHintMatching(Stz* szHintKey, QueryNode* pSqlOp, PlanNode* pRootNode);

    // Looks up and returns the first hint that matches the hint key from
    // pSqlOp upwards to the root node. We must know the absolute path
    // of pSqlOp within the graph so it can be walked.
    // Returned node* is internal and should not be modified/freed.
    static QueryNode* pFindHintMatching(Stz* szHintKey, QueryNode* pSqlOp, s2K& sSqlOpPath, PlanNode* pRootNode);


    // Looks up and returns all hints that match the hint key from pSqlOp
    // upwards to the root node. This can be a slow operation as we
    // don't know the path of pSqlOp within the graph & have to find this first.
    // Returned QueryNodeList* must be freed but not its contents.
    static QueryNodeList* pFindAllHintsMatching(Stz* szHintKey, QueryNode* pSqlOp, PlanNode* pRootNode);

    // Looks up and returns all hints that match the hint key from pSqlOp
    // upwards to the root node. We must know the absolute path
    // of pSqlOp within the graph so it can be walked.
    // Returned QueryNodeList* must be freed but not its contents.
    static QueryNodeList* pFindAllHintsMatching(Stz* szHintKey, QueryNode* pSqlOp, s2K& sSqlOpPath, PlanNode* pRootNode);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
