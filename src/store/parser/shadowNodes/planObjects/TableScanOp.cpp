/*============================================================================*/
/* TableScanOp.hpp - an operator that linearly reads all rows in a table      */
/*============================================================================*/

#include "BASE.h"
#include "TableScanOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

#include "TableRef.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TABLEREF_PROP = "tableRef"_Stz;
static Stz* C_TABLEREF_DEFN = "/__root__/definitions/object/tableRef"_Stz;

static Stz* C_TABLESCAN_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource/tableScan"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* TableScanOp::pCreate(QueryNode* pTableRef)
{
  ASSERT(pTableRef);
  ASSERT(pTableRef->szType()->bEqual(C_TABLEREF_DEFN));

  // Create a unique name for this ProjectOp object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_TABLESCAN_DEFN), sName);

  // Create the object and add the tableRef property. We will need to rename it first.
  QueryNode* pTableScanOp = new QueryNode(sName, C_TABLESCAN_DEFN);

  pTableRef->vRename(C_TABLEREF_PROP);
  pTableScanOp->vAppendProperty(pTableRef);

  return pTableScanOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void TableScanOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pTableScanOp = static_cast<QueryNode*>(pNode);

  sBuf.vConcat("TableScanOp : "_Stz);

  QueryNode* pTableRef = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLEREF_PROP, pTableScanOp));
  ASSERT(pTableRef);

  TableRef::vDescribe(pTableRef, sBuf);
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 TableScanOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TABLESCAN_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
