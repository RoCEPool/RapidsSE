/*============================================================================*/
/* JoinOptimisation.hpp - forcing join algorithms from query hints            */
/*============================================================================*/

#include "BASE.h"
#include "JoinOptimisation.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "StatementMarker.hpp"
#include "SqlException.hpp"


#include "JoinOp.hpp"
#include "HashJoinOp.hpp"
#include "IndexLookupJoinOp.hpp"
#include "NestedLoopJoinOp.hpp"
#include "SortMergeJoinOp.hpp"
#include "Hint.hpp"
#include "Hints.hpp"
#include "HintFinder.hpp"
#include "HintConstants.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOptimisation::vOptimiseJoins(PlanNode *pRootNode)
{
  PlanNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  vApplyJoinHint(pRootNode);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOptimisation::vApplyJoinHint(PlanNode* pRootNode)
{
  PlanNode* pPlanNode = ShadowRoot::pPlanRoot(pRootNode);
  ASSERT(pPlanNode);
  ASSERT(ShadowPlan::bIsOfType(pPlanNode));

  // Iterate each node, looking for a StatementMarker
  auto fVisitor = [pRootNode, pPlanNode]
      (PlanNode *pNode, s2K &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    // Only walk the plan subtree
    if (iDepth == 1 && pNode != pPlanNode) {
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    if (!JoinOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pJoinOp = _pQueryNode(pNode);

    QueryNode* pJoinAlgoHint = HintFinder::pFindHintMatching(C_HINT_KEY_JOIN_ALGO, pJoinOp, sPath, pRootNode);

    // Safety first
    if (!pJoinAlgoHint) {
      return GraphNav::WALK_CONTINUE;
    }

    if (pJoinAlgoHint->nChildren() != 1 ||
        Hint::pFirstValue(pJoinAlgoHint)->eValueType() != Value::VT_STRING) {

      SyntaxException::vThrow(-1, -1, EC_UNRECOGNISED_HINT_VALUE, ": Hint %s"_Stz, C_HINT_KEY_JOIN_ALGO);
    }

    Stz* szAlgoType = Hint::pFirstValue(pJoinAlgoHint)->szValue();

    // Convert the join in place
    if (szAlgoType->bSame(C_HINT_VAL_JOIN_ALGO_LOOP) && !NestedLoopJoinOp::bIsExactType(pJoinOp)) {
      NestedLoopJoinOp::vConvertJoinAlgorithm(pJoinOp);

    } else if (szAlgoType->bSame(C_HINT_VAL_JOIN_ALGO_INDEX) && !IndexLookupJoinOp::bIsExactType(pJoinOp)) {

      // TODO: Must also supply the join index to be used on the right side table

      IndexLookupJoinOp::vConvertJoinAlgorithm(pJoinOp);

    } else if (szAlgoType->bSame(C_HINT_VAL_JOIN_ALGO_HASH)  && !HashJoinOp::bIsExactType(pJoinOp)) {
      HashJoinOp::vConvertJoinAlgorithm(pJoinOp);

    } else if (szAlgoType->bSame(C_HINT_VAL_JOIN_ALGO_MERGE) && !SortMergeJoinOp::bIsExactType(pJoinOp)) {
      SortMergeJoinOp::vConvertJoinAlgorithm(pJoinOp);

    } else {
      SyntaxException::vThrow(-1, -1, EC_UNRECOGNISED_HINT_VALUE, ": Hint %s = %s"_Stz, C_HINT_KEY_JOIN_ALGO, szAlgoType);
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pRootNode, false, true, fVisitor, pRootNode);
}


/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
