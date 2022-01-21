/*============================================================================*/
/* SortMergeJoinOp.hpp - sorted merge join algorithm                          */
/*============================================================================*/

#include "BASE.h"
#include "SortMergeJoinOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

#include "ShadowRoot.hpp"
#include "Expr.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_DEFN = "/__root__/definitions/object/planObject/sqlOperator/joinOp/sortMergeJoinOp"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create this join operator.                              */
/*----------------------------------------------------------------------------*/
QueryNode* SortMergeJoinOp::pCreate(eJT eJoinType, QueryNode* pJoinPredicate, QueryNode* pUsingList)
{
  QueryNode* pSortMergeJoinOp = JoinOp::pCreate(C_DEFN, eJoinType, pJoinPredicate, pUsingList);

  // Add any extra properties here.

  return pSortMergeJoinOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SortMergeJoinOp::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  s2K sPredicateBuf;
  QueryNode* pPredicate = pJoinPredicate(pNode);
  ASSERT(pPredicate);
  ASSERT(Expr::bIsOfType(pPredicate));
  Expr::vDescribe(pPredicate, sPredicateBuf, pRootNode);

  sBuf.vCatSprint("SortMergeJoinOp : %s ; %s"_Stz,
                  aJT_pretty[JoinOp::eJoinType(pNode)],
                  sPredicateBuf.szStz());
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SortMergeJoinOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, pExplainNode, sBuf, iIndent + 1);
    }

    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SortMergeJoinOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object

    return;
  }

  // Manual polymorphism here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 SortMergeJoinOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 SortMergeJoinOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SortMergeJoinOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SortMergeJoinOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Converts pJoinOp to a sort merge join                                      */
/*----------------------------------------------------------------------------*/
void SortMergeJoinOp::vConvertJoinAlgorithm(QueryNode* pJoinOp, PlanNode* pRootNode)
{
  ASSERT(JoinOp::bIsOfType(pJoinOp));

  JoinOp::vDeleteNonCommonProperties(pJoinOp, pRootNode);

  pJoinOp->vSetType(C_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
