/*============================================================================*/
/* IndexLookupJoinOp.hpp - index lookup join algorithm                        */
/*============================================================================*/

#include "BASE.h"
#include "IndexLookupJoinOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

#include "ShadowRoot.hpp"
#include "Expr.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_INDEXLOOKUPJOIN_DEFN = "/__root__/definitions/object/planObject/sqlOperator/joinOp/indexLookupJoinOp"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create this join operator.                              */
/*----------------------------------------------------------------------------*/
QueryNode* IndexLookupJoinOp::pCreate(eJT eJoinType, QueryNode* pJoinPredicate, QueryNode* pUsingList)
{
  QueryNode* pIndexLookupJoinOp = JoinOp::pCreate(C_INDEXLOOKUPJOIN_DEFN, eJoinType, pJoinPredicate, pUsingList);

  // Add any extra properties here.

  return pIndexLookupJoinOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void IndexLookupJoinOp::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  s2K sPredicateBuf;
  QueryNode* pPredicate = pJoinPredicate(pNode);
  ASSERT(pPredicate);
  ASSERT(Expr::bIsOfType(pPredicate));
  Expr::vDescribe(pPredicate, sPredicateBuf, pRootNode);

  sBuf.vCatSprint("IndexLookupJoinOp : %s ; %s"_Stz,
                  aJT_pretty[JoinOp::eJoinType(pNode)],
                  sPredicateBuf.szStz());
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void IndexLookupJoinOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
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
void IndexLookupJoinOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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
b1 IndexLookupJoinOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_INDEXLOOKUPJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 IndexLookupJoinOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_INDEXLOOKUPJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IndexLookupJoinOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_INDEXLOOKUPJOIN_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IndexLookupJoinOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_INDEXLOOKUPJOIN_DEFN);
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
