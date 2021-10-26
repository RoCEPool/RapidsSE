/*============================================================================*/
/* HintFinder.hpp - a utility for finding hints that are in scope in the plan */
/*============================================================================*/

#include "BASE.h"
#include "HintFinder.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "RootNode.hpp"
#include "GraphNav.hpp"

#include "ShadowRoot.hpp"
#include "ShadowPlan.hpp"
#include "PlanObject.hpp"
#include "Hint.hpp"
#include "Hints.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Looks up and returns the first hint that matches the hint key from         */
/* pSqlOp upwards to the root node. This can be a slow operation as we        */
/* don't know the path of pSqlOp within the graph & have to find this first.  */
/* Returned node* is internal and should not be modified/freed.               */
/*----------------------------------------------------------------------------*/
QueryNode* HintFinder::pFindHintMatching(Stz* szHintKey,
                                         QueryNode* pSqlOp,
                                         QueryNode* pRootNode)
{
  ASSERT(szHintKey);
  ASSERT(pSqlOp);
  ASSERT(PlanObject::bIsOfType(pSqlOp));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // First find the absolute path of pSqlOp in the plan
  s2K sSqlOpPath;
  i8 iDepth;
  b1 bResult = GraphNav::bGetPathAndDepth(pSqlOp, pRootNode, sSqlOpPath, iDepth, true, false);

  ASSERT(bResult);
  ASSERT(sSqlOpPath.nLength() > 0);
  ASSERT(sSqlOpPath.bBeginsWith("/"_Stz));

  return pFindHintMatching(szHintKey, pSqlOp, sSqlOpPath, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Looks up and returns the first hint that matches the hint key from         */
/* pSqlOp upwards to the root node. We must know the absolute path            */
/* of pSqlOp within the graph so it can be walked.                            */
/* Returned node* is internal and should not be modified/freed.               */
/*----------------------------------------------------------------------------*/
QueryNode* HintFinder::pFindHintMatching(Stz* szHintKey, QueryNode* pSqlOp,
                                         s2K& sSqlOpPath, QueryNode* pRootNode)
{
  ASSERT(szHintKey);
  ASSERT(pSqlOp);
  ASSERT(PlanObject::bIsOfType(pSqlOp));
  ASSERT(sSqlOpPath.nLength() > 0);
  ASSERT(sSqlOpPath.bBeginsWith("/"_Stz));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Walk the path in bottom up order, looking for the hint at each node.
  QueryNode* pHint = nullptr;
  auto fVisitor = [&pHint, szHintKey, pRootNode]
                  (PlanNode* pNode, s2K& sCurrentPath, i8 iDepth, GraphNav::eWalkEvent eWalkEvent, void* pvArg)
                  -> GraphNav::eWalkCtrl {

    ASSERT(PlanObject::bIsOfType(pNode));
    pHint = PlanObject::pGetHint(_pQueryNode(pNode), szHintKey);
    if (pHint) {
      ASSERT(Hint::bIsOfType(pHint));
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bWalkPath(pRootNode, sSqlOpPath, nullptr, fVisitor, nullptr);
  ASSERT(!pHint || (Hint::bIsOfType(pHint) && Hint::szHintKey(pHint)->bSame(szHintKey)));

  return pHint;
}

/*----------------------------------------------------------------------------*/
/* Looks up and returns all hints that match the hint key from pSqlOp         */
/* upwards to the root node. This can be a slow operation as we               */
/* don't know the path of pSqlOp within the graph & have to find this first.  */
/* Returned QueryNodeList* must be freed but not its contents.                */
/*----------------------------------------------------------------------------*/
QueryNodeList* HintFinder::pFindAllHintsMatching(Stz* szHintKey,
                                                 QueryNode* pSqlOp,
                                                 QueryNode* pRootNode)
{
  ASSERT(szHintKey);
  ASSERT(pSqlOp);
  ASSERT(PlanObject::bIsOfType(pSqlOp));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // First find the absolute path of pSqlOp in the plan
  s2K sSqlOpPath;
  i8 iDepth;
  b1 bResult = GraphNav::bGetPathAndDepth(pSqlOp, pRootNode, sSqlOpPath, iDepth, true, false);

  ASSERT(bResult);
  ASSERT(sSqlOpPath.nLength() > 0);
  ASSERT(sSqlOpPath.bBeginsWith("/"_Stz));

  return pFindAllHintsMatching(szHintKey, pSqlOp, sSqlOpPath, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Looks up and returns all hints that match the hint key from pSqlOp         */
/* upwards to the root node. We must know the absolute path                   */
/* of pSqlOp within the graph so it can be walked.                            */
/* Returned QueryNodeList* must be freed but not its contents.                */
/*----------------------------------------------------------------------------*/
QueryNodeList* HintFinder::pFindAllHintsMatching(Stz* szHintKey,
                                                 QueryNode* pSqlOp,
                                                 s2K& sSqlOpPath,
                                                 QueryNode* pRootNode)
{
  ASSERT(szHintKey);
  ASSERT(pSqlOp);
  ASSERT(PlanObject::bIsOfType(pSqlOp));
  ASSERT(sSqlOpPath.nLength() > 0);
  ASSERT(sSqlOpPath.bBeginsWith("/"_Stz));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // Walk the path in bottom up order, looking for the hint at each node.
  QueryNodeList* pHintList = new QueryNodeList();

  auto fVisitor = [&pHintList, szHintKey, pRootNode]
      (PlanNode* pNode, s2K& sCurrentPath, i8 iDepth, GraphNav::eWalkEvent eWalkEvent, void* pvArg)
      -> GraphNav::eWalkCtrl {

    ASSERT(PlanObject::bIsOfType(pNode));
    QueryNode* pHint = PlanObject::pGetHint(_pQueryNode(pNode), szHintKey);
    if (pHint) {
      ASSERT(Hint::bIsOfType(pHint));
      pHintList->vAppend(pHint);
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bWalkPath(pRootNode, sSqlOpPath, nullptr, fVisitor, nullptr);

  return pHintList;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
