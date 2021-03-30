/*============================================================================*/
/* ColumnDefs.hpp - a list of columns definitions                             */
/*============================================================================*/

#include "BASE.h"
#include "ColumnDefs.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "ColumnDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_COLUMNDEFS_DEFN = "/__root__/definitions/object/list/columnDefs"_Stz;
static Stz* C_COLUMNDEFS_NAME = "columnDefs"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of columnDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* ColumnDefs::pCreate()
{
  _static_

  // Create an empty columnDefs object
  QueryNode* pColumnDefs = new QueryNode(C_COLUMNDEFS_NAME, const_cast<Stz*>(C_COLUMNDEFS_DEFN));

  return pColumnDefs;
}

/*----------------------------------------------------------------------------*/
/* Add a columnDef to the list                                                */
/*----------------------------------------------------------------------------*/
void ColumnDefs::vAddColumnDef(QueryNode* pNode, QueryNode* pColumnDef)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(ColumnDef::bIsOfType(pColumnDef));

  pNode->vAppendChild(pColumnDef);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the column definitions into a buffer              */
/*----------------------------------------------------------------------------*/
void ColumnDefs::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pColumnDefs = static_cast<QueryNode*>(pNode);

  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    ColumnDef::vDescribe(pChild, sBuf);
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the columnDefs definition   */
/*----------------------------------------------------------------------------*/
b1 ColumnDefs::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_COLUMNDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ColumnDefs::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_COLUMNDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of columns in this list                                     */
/*----------------------------------------------------------------------------*/
i8 ColumnDefs::nColumns(QueryNode* pColumnDefs)
{
  ASSERT(bIsOfType(pColumnDefs));

  return pColumnDefs->nChildren();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
