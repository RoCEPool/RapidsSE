/*============================================================================*/
/* IdList.hpp - a list of object name IDs that are represented as strings     */
/*============================================================================*/

#include "BASE.h"
#include "IdList.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "StringObject.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_IDLIST_DEFN = "/__root__/definitions/object/list/idList"_Stz;
static Stz* C_IDLIST_NAME = "idList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of string nodes as an idList.             */
/*----------------------------------------------------------------------------*/
QueryNode* IdList::pCreate()
{
  _static_

  // Create an empty IdList object
  QueryNode* pIdList = new QueryNode(C_IDLIST_NAME, const_cast<Stz*>(C_IDLIST_DEFN));

  return pIdList;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the IdList into a buffer                          */
/*----------------------------------------------------------------------------*/
void IdList::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pIdList = _pQueryNode(pNode);

  PlanNodeIter* pIter = pIdList->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    sBuf.vCatSprint("%s"_Stz, pChild->szValue());
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the IdList definition       */
/*----------------------------------------------------------------------------*/
b1 IdList::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_IDLIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IdList::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_IDLIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if this string list contains a given string value             */
/*----------------------------------------------------------------------------*/
b1 IdList::bContains(QueryNode* pNode, b1 bCaseSensitive, Stz* szVal)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(szVal);

  if (pNode->nChildren() == 0) {
    return false;
  }

  b1 bFound = false;

  PlanNodeIter* pIter = pNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();
    ASSERT(StringObject::bIsOfType(pChild));

    if (bCaseSensitive) {
      if (szVal->bEqual(pChild->szValue())) {
        bFound = true;
        break;
      }

    } else {
      // Case insensitive
      if (szVal->bSame(pChild->szValue())) {
        bFound = true;
        break;
      }
    }
  }

  delete pIter;
  return bFound;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
