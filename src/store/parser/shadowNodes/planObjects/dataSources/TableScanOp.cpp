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
static Stz* C_TABLEREF_DEFN = "/__root__/definitions/object/compound/tableRef"_Stz;

static Stz* C_TABLESCAN_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource/tableScanOp"_Stz;



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
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TableScanOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pTableScanOp = static_cast<QueryNode*>(pNode);

  sBuf.vConcat("TableScanOp : "_Stz);

  QueryNode* pTableRef = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLEREF_PROP, pTableScanOp));
  ASSERT(pTableRef);

  TableRef::vDescribe(pTableRef, sBuf);
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void TableScanOp::vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

  // Get the tableref property
  PlanNode* pTableRefNode = GraphNav::pFindProperty(const_cast<Stz*>(C_TABLEREF_PROP), pNode);
  ASSERT(pTableRefNode);
  ASSERT(TableRef::bIsOfType(pTableRefNode));

  // Print the table reference
  s2K sTblBuf;
  TableRef::vDescribe(pTableRefNode, sTblBuf);
  sBuf.vConcat("\n"_Stz);
  vAppendIndent(sBuf, iIndent, IF_PLAIN);
  sBuf.vCatSprint("table: (%s)"_Stz, sTblBuf.szStz());

  // Recurse any children
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, sBuf, iIndent+1);
    }

    delete pIter;
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 TableScanOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TABLESCAN_DEFN);
}


/*----------------------------------------------------------------------------*/
/* Returns the tableRef of the queryNode                                      */
/*----------------------------------------------------------------------------*/
QueryNode* TableScanOp::pTableRef(QueryNode* pNode)
{
  QueryNode *pTableRefNode =
      static_cast<QueryNode*>(GraphNav::pFindProperty(const_cast<Stz *>(C_TABLEREF_PROP), pNode));
  QueryNode *pTableRef = static_cast<QueryNode *>(pTableRefNode);

  return pTableRef;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
