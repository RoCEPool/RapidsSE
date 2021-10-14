/*============================================================================*/
/* StageRangeList.hpp - a list of StageRange nodes                            */
/*============================================================================*/

#include "BASE.h"
#include "StageRangeList.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "StageRange.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_STAGERANGELIST_DEFN = "/__root__/definitions/object/list/stageRangeList"_Stz;
static Stz* C_STAGERANGELIST_NAME = "stageRangeList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of stageRange nodes.                      */
/*----------------------------------------------------------------------------*/
QueryNode* StageRangeList::pCreate()
{
  _static_

  // Create an empty stageRangeList object
  QueryNode* pStageRangeList = new QueryNode(C_STAGERANGELIST_NAME, const_cast<Stz*>(C_STAGERANGELIST_DEFN));

  return pStageRangeList;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the string list into a buffer                     */
/*----------------------------------------------------------------------------*/
void StageRangeList::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pStageRangeList = _pQueryNode(pNode);

  PlanNodeIter* pIter = pStageRangeList->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();
    ASSERT(StageRange::bIsOfType(pChild));

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    StageRange::vDescribe(pChild, sBuf, pRootNode);
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the stageRangeList defn     */
/*----------------------------------------------------------------------------*/
b1 StageRangeList::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STAGERANGELIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StageRangeList::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STAGERANGELIST_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
