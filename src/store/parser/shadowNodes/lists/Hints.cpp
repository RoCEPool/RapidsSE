/*============================================================================*/
/* Hints.hpp - a list of query hints                                          */
/*============================================================================*/

#include "BASE.h"
#include "Hints.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "Hint.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/list/hints"_Stz;
static Stz* C_NAME = "hints"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of tableDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* Hints::pCreate()
{
  _static_

  // Create an empty hints object
  QueryNode* pHints = new QueryNode(C_NAME, const_cast<Stz*>(C_DEFN));

  return pHints;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the hint into a buffer                            */
/*----------------------------------------------------------------------------*/
void Hints::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pHints = _pQueryNode(pNode);

  PlanNodeIter* pIter = pHints->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(Hint::bIsOfType(pChild));

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    s2K sHintBuf;
    Hint::vDescribe(pChild, sHintBuf, pRootNode);
    sBuf.vConcat(sHintBuf.szStz());
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the hints definition   */
/*----------------------------------------------------------------------------*/
b1 Hints::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Hints::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Add a hint to the list                                                     */
/*----------------------------------------------------------------------------*/
void Hints::vAddHint(QueryNode* pNode, QueryNode* pHint)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(Hint::bIsOfType(pHint));

  pNode->vAppendChild(pHint);
}

/*----------------------------------------------------------------------------*/
/* get the number of hints in this list                                     */
/*----------------------------------------------------------------------------*/
i8 Hints::nHints(QueryNode* pHints)
{
  ASSERT(bIsOfType(pHints));

  return pHints->nChildren();
}

/*----------------------------------------------------------------------------*/
/* Looks up and returns the hint *from this node* that matches the key.       */
/* Will return nullptr if no hints match.                                     */
/*----------------------------------------------------------------------------*/
QueryNode* Hints::pGetHint(QueryNode* pNode, Stz* szHintKey)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(szHintKey);

  QueryNode* pHint = nullptr;

  if (pNode->nChildren() == 0) {
    return pHint;
  }

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(Hint::bIsOfType(pChild));

    if (Hint::szHintKey(pChild)->bSame(szHintKey)) {
      pHint = pChild;
      break;
    }
  }

  delete pIter;
  return pHint;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
