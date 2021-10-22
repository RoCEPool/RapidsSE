/*============================================================================*/
/* StringList.hpp - a list of string value nodes                              */
/*============================================================================*/

#include "BASE.h"
#include "StringList.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "StringObject.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_STRINGLIST_DEFN = "/__root__/definitions/object/list/stringList"_Stz;
static Stz* C_STRINGLIST_NAME = "stringList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of stringobject nodes.                    */
/*----------------------------------------------------------------------------*/
QueryNode* StringList::pCreate()
{
  _static_

  // Create an empty stringList object
  QueryNode* pStringList = new QueryNode(C_STRINGLIST_NAME, const_cast<Stz*>(C_STRINGLIST_DEFN));

  return pStringList;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the string list into a buffer                     */
/*----------------------------------------------------------------------------*/
void StringList::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pStringList = _pQueryNode(pNode);

  PlanNodeIter* pIter = pStringList->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    sBuf.vCatSprint("'%s'"_Stz, pChild->szValue());
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the stringList definition   */
/*----------------------------------------------------------------------------*/
b1 StringList::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STRINGLIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StringList::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STRINGLIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if this string list contains a given string value             */
/*----------------------------------------------------------------------------*/
b1 StringList::bContains(QueryNode* pNode, b1 bCaseSensitive, Stz* szVal)
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
