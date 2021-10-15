/*============================================================================*/
/* TableDefs.hpp - a list of tables definitions                               */
/*============================================================================*/

#include "BASE.h"
#include "TableDefs.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "TableDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TABLEDEFS_DEFN = "/__root__/definitions/object/list/tableDefs"_Stz;
static Stz* C_TABLEDEFS_NAME = "tableDefs"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of tableDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* TableDefs::pCreate()
{
  _static_

  // Create an empty tableDefs object
  QueryNode* pTableDefs = new QueryNode(C_TABLEDEFS_NAME, const_cast<Stz*>(C_TABLEDEFS_DEFN));

  return pTableDefs;
}

/*----------------------------------------------------------------------------*/
/* Add a tableDef to the list                                                */
/*----------------------------------------------------------------------------*/
void TableDefs::vAddTableDef(QueryNode* pNode, QueryNode* pTableDef)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(TableDef::bIsOfType(pTableDef));

  pNode->vAppendChild(pTableDef);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table definitions into a buffer              */
/*----------------------------------------------------------------------------*/
void TableDefs::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pTableDefs = _pQueryNode(pNode);

  PlanNodeIter* pIter = pTableDefs->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    sBuf.vConcat(TableDef::szTableName(_pQueryNode(pChild)));
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the tableDefs definition   */
/*----------------------------------------------------------------------------*/
b1 TableDefs::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TABLEDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableDefs::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TABLEDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of tables in this list                                     */
/*----------------------------------------------------------------------------*/
i8 TableDefs::nTables(QueryNode* pTableDefs)
{
  ASSERT(bIsOfType(pTableDefs));

  return pTableDefs->nChildren();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
