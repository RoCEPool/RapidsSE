/*============================================================================*/
/* TableScanOp.hpp - an operator that linearly reads all rows in a table      */
/*============================================================================*/

#include "BASE.h"
#include "TableScanOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

#include "ShadowRoot.hpp"
#include "store/parser/shadowNodes/compound/TableRef.hpp"
#include "NodeNamer.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource/tableSource/tableScanOp"_Stz;

static Stz* C_TABLEREF_PROP = "tableRef"_Stz;
static Stz* C_TABLEREF_DEFN = "/__root__/definitions/object/compound/tableRef"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create TableScanOp nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* TableScanOp::pCreate(QueryNode* pTableRef)
{
  ASSERT(pTableRef);
  ASSERT(TableRef::bIsExactType(pTableRef));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  // Create the object and add the tableRef property. We will need to rename it first.
  QueryNode* pTableScanOp = new QueryNode(sName, C_DEFN);

  pTableRef->vSetName(C_TABLEREF_PROP);
  pTableScanOp->vAppendProperty(pTableRef);

  return pTableScanOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TableScanOp::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pTableScanOp = _pQueryNode(pNode);

  sBuf.vConcat("TableScanOp : "_Stz);

  QueryNode* pTableRef = TableScanOp::pTableRef(pTableScanOp);
  ASSERT(pTableRef);

  TableRef::vDescribe(pTableRef, sBuf, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void TableScanOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, pExplainNode, sBuf, iIndent + 1);
    }

    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void TableScanOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object

    // Get the tableref property
    PlanNode* pTableRefNode = GraphNav::pFindProperty(const_cast<Stz*>(C_TABLEREF_PROP), pNode);
    ASSERT(pTableRefNode);
    ASSERT(TableRef::bIsOfType(pTableRefNode));

    // Print the table reference
    s2K sTblBuf;
    TableRef::vDescribe(pTableRefNode, sTblBuf, pRootNode);
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("table: (%s)"_Stz, sTblBuf.szStz());


    return;
  }

  // Manual polymorphism here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 TableScanOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 TableScanOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableScanOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableScanOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the tableRef of the queryNode                                      */
/*----------------------------------------------------------------------------*/
QueryNode* TableScanOp::pTableRef(QueryNode* pNode)
{
  QueryNode *pTableRefNode = _pQueryNode(GraphNav::pFindProperty(C_TABLEREF_PROP, pNode));

  ASSERT(pTableRefNode);
  ASSERT(TableRef::bIsOfType(pTableRefNode));

  return pTableRefNode;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
