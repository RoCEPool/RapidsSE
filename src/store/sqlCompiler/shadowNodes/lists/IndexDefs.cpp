/*============================================================================*/
/* IndexDefs.hpp - a list of indexs definitions                             */
/*============================================================================*/

#include "BASE.h"
#include "IndexDefs.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "IndexDefn.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_INDEXDEFS_DEFN = "/__root__/definitions/object/list/indexDefs"_Stz;
static Stz* C_INDEXDEFS_NAME = "indexDefs"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of indexDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* IndexDefs::pCreate()
{
  _static_

  // Create an empty indexDefs object
  QueryNode* pIndexDefs = new QueryNode(C_INDEXDEFS_NAME, const_cast<Stz*>(C_INDEXDEFS_DEFN));

  return pIndexDefs;
}

/*----------------------------------------------------------------------------*/
/* Add a indexDef to the list                                                */
/*----------------------------------------------------------------------------*/
void IndexDefs::vAddIndexDef(QueryNode* pNode, QueryNode* pIndexDef)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(IndexDefn::bIsOfType(pIndexDef));

  pNode->vAppendChild(pIndexDef);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the index definitions into a buffer              */
/*----------------------------------------------------------------------------*/
void IndexDefs::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pIndexDefs = _pQueryNode(pNode);

  PlanNodeIter* pIter = pIndexDefs->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    IndexDefn::vDescribe(pChild, sBuf, pRootNode);
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the indexDefs definition   */
/*----------------------------------------------------------------------------*/
b1 IndexDefs::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_INDEXDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IndexDefs::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_INDEXDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of indexs in this list                                     */
/*----------------------------------------------------------------------------*/
i8 IndexDefs::nIndexes(QueryNode* pIndexDefs)
{
  ASSERT(bIsOfType(pIndexDefs));

  return pIndexDefs->nChildren();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
