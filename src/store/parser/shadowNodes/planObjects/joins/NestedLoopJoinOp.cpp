/*============================================================================*/
/* NestedLoopJoinOp.hpp - nested loop join algorithm                          */
/*============================================================================*/

#include "BASE.h"
#include "NestedLoopJoinOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

#include "ShadowRoot.hpp"
#include "Expr.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_NESTEDLOOPJOIN_DEFN = "/__root__/definitions/object/planObject/sqlOperator/joinOp/nestedLoopJoinOp"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create this join operator.                              */
/*----------------------------------------------------------------------------*/
QueryNode* NestedLoopJoinOp::pCreate(eJT eJoinType, QueryNode* pJoinPredicate, QueryNode* pUsingList)
{
  QueryNode* pNestedLoopJoinOp = JoinOp::pCreate(C_NESTEDLOOPJOIN_DEFN, eJoinType, pJoinPredicate, pUsingList);

  // Add any extra properties here.

  return pNestedLoopJoinOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void NestedLoopJoinOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  s2K sPredicateBuf;
  QueryNode* pPredicate = pJoinPredicate(pNode);
  ASSERT(pPredicate);
  ASSERT(Expr::bIsOfType(pPredicate));
  Expr::vDescribe(pPredicate, sPredicateBuf);

  sBuf.vCatSprint("NestedLoopJoinOp : %s ; %s"_Stz,
                  aJT_pretty[JoinOp::eJoinType(pNode)],
                  sPredicateBuf.szStz());
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void NestedLoopJoinOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
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
void NestedLoopJoinOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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
b1 NestedLoopJoinOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_NESTEDLOOPJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 NestedLoopJoinOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_NESTEDLOOPJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 NestedLoopJoinOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_NESTEDLOOPJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 NestedLoopJoinOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_NESTEDLOOPJOIN_DEFN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
