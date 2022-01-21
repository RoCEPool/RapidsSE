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
#include "TableSource.hpp"
#include "MetadataNode.hpp"
#include "IndexRef.hpp"
#include "IndexDefn.hpp"


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

      SyntaxException::vThrow(-1, -1, EC_HINT_VALUE_UNRECOGNISED, ": Hint %s"_Stz, C_HINT_KEY_JOIN_ALGO);
    }

    Stz* szAlgoType = Hint::pFirstValue(pJoinAlgoHint)->szValue();

    // Convert the join in place.

    // Conversion to Nested Loop Join
    if (szAlgoType->bSame(C_HINT_VAL_JOIN_ALGO_LOOP) && !NestedLoopJoinOp::bIsExactType(pJoinOp)) {
      NestedLoopJoinOp::vConvertJoinAlgorithm(pJoinOp, pRootNode);

    // Conversion to Index Join
    } else if (szAlgoType->bSame(C_HINT_VAL_JOIN_ALGO_INDEX) && !IndexLookupJoinOp::bIsExactType(pJoinOp)) {
      vConvertJoinToIndexJoin(pJoinOp, pRootNode, sPath);

    // Conversion to Hash Join
    } else if (szAlgoType->bSame(C_HINT_VAL_JOIN_ALGO_HASH)  && !HashJoinOp::bIsExactType(pJoinOp)) {
      HashJoinOp::vConvertJoinAlgorithm(pJoinOp, pRootNode);

    // Conversion to Sort Merge Join
    } else if (szAlgoType->bSame(C_HINT_VAL_JOIN_ALGO_MERGE) && !SortMergeJoinOp::bIsExactType(pJoinOp)) {
      SortMergeJoinOp::vConvertJoinAlgorithm(pJoinOp, pRootNode);

    } else {
      SyntaxException::vThrow(-1, -1, EC_HINT_VALUE_UNRECOGNISED, ": Hint %s = %s"_Stz, C_HINT_KEY_JOIN_ALGO, szAlgoType);
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pRootNode, false, true, fVisitor, pRootNode);
}



/*----------------------------------------------------------------------------*/
/* Converts a given join to an index join based on a join hint directive      */
/*----------------------------------------------------------------------------*/
void JoinOptimisation::vConvertJoinToIndexJoin(QueryNode* pJoinOp, PlanNode* pRootNode, s2K& sPath)
{
  // Conversion to an index join requires that the join index also be specified.
  // This index must be resolvable to the right side table.
  QueryNode* pJoinIndexHint = HintFinder::pFindHintMatching(C_HINT_KEY_JOIN_INDEX, pJoinOp, sPath, pRootNode);

  if (!pJoinIndexHint) {
    SemanticException::vThrow(EC_HINT_VALUE_MISSING,
                              ": %s %s"_Stz,
                              C_HINT_KEY_JOIN_INDEX,
                              "(required for conversion to an index join)"_Stz);
  }

  if (pJoinIndexHint->nChildren() != 1 ||
      Hint::pFirstValue(pJoinIndexHint)->eValueType() != Value::VT_STRING ||
      Hint::pFirstValue(pJoinIndexHint)->szValue()->bIsEmpty()) {
    SyntaxException::vThrow(-1, -1, EC_HINT_VALUE_UNRECOGNISED, ": Hint %s"_Stz, C_HINT_KEY_JOIN_INDEX);
  }

  Stz* szIndexName = pJoinIndexHint->pFirstChild()->szValue();

  // Check that the index exists for the right side table.
  // First get the right side table.
  ASSERT(pJoinOp->iNumChildren() == 2);

  QueryNode* pTableSource = _pQueryNode(pJoinOp->pSecondChild());
  ASSERT(TableSource::bIsOfType(pTableSource));

  QueryNode* pMetadataNode = ShadowRoot::pMetadataNode(pRootNode);
  QueryNode* pIndexDef =
      MetadataNode::pIndexDef(pMetadataNode,
                              TableSource::szCatalogName(pTableSource),
                              TableSource::szSchemaName(pTableSource),
                              TableSource::szTableName(pTableSource),
                              szIndexName);

  if (!pIndexDef) {
    SemanticException::vThrow(EC_NO_SUCH_INDEX, ": %s"_Stz, szIndexName);
  }

  // Create an indexRef
  QueryNode* pIndexRef = IndexRef::pCreate(IndexDefn::szCatalogName(pIndexDef),
                                           IndexDefn::szSchemaName(pIndexDef),
                                           IndexDefn::szTableName(pIndexDef),
                                           IndexDefn::szIndexName(pIndexDef));

  // Now we can convert to the index join
  IndexLookupJoinOp::vConvertJoinAlgorithm(pJoinOp, pIndexRef, pRootNode);
}

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
