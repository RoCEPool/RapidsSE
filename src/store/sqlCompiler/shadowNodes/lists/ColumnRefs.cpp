/*============================================================================*/
/* ColumnRefs.hpp - a list of references to columns                           */
/*============================================================================*/

#include "BASE.h"
#include "ColumnRefs.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "ColumnRef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COLUMNREFS_DEFN = "/__root__/definitions/object/list/columnRefs"_Stz;
static Stz* C_COLUMNREFS_NAME = "columnRefs"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create columnRefs nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnRefs::pCreate()
{
  _static_

  // Create an empty columnRefs object
  QueryNode* pColumnRefs = new QueryNode(C_COLUMNREFS_NAME, const_cast<Stz*>(C_COLUMNREFS_DEFN));

  return pColumnRefs;
}

/*----------------------------------------------------------------------------*/
/* Add a columnRef to the list                                                */
/*----------------------------------------------------------------------------*/
void ColumnRefs::vAddColumnRef(QueryNode* pNode, QueryNode* pColumnRef)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(ColumnRef::bIsOfType(pColumnRef));

  pNode->vAppendChild(pColumnRef);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the column reference into a buffer                */
/*----------------------------------------------------------------------------*/
void ColumnRefs::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnRefs = _pQueryNode(pNode);

  PlanNodeIter* pIter = pColumnRefs->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    ColumnRef::vDescribe(pChild, sBuf, pRootNode);
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the columnRefs definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnRefs::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COLUMNREFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnRefs::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_COLUMNREFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of columns in this list                                     */
/*----------------------------------------------------------------------------*/
i8 ColumnRefs::nColumns(QueryNode* pColumnRefs)
{
  ASSERT(bIsOfType(pColumnRefs));

  return pColumnRefs->nChildren();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
