/*============================================================================*/
/* IndexScanOp.hpp - an operator that linearly reads all rows from an index   */
/*============================================================================*/

#include "BASE.h"
#include "IndexScanOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

#include "ShadowRoot.hpp"
#include "store/parser/shadowNodes/compound/TableRef.hpp"
#include "store/parser/shadowNodes/compound/IndexRef.hpp"
#include "NodeNamer.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource/tableSource/indexScanOp"_Stz;

static Stz* C_INDEXREF_PROP = "indexRef"_Stz;
static Stz* C_INDEXREF_DEFN = "/__root__/definitions/object/compound/tableRef/indexRef"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create IndexScanOp nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* IndexScanOp::pCreate(QueryNode* pIndexRef)
{
  ASSERT(pIndexRef);
  ASSERT(IndexRef::bIsExactType(pIndexRef));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  // Create the object and add the pIndexRef property. We will need to rename it first.
  QueryNode* pIndexScanOp = new QueryNode(sName, C_DEFN);

  pIndexRef->vSetName(C_INDEXREF_PROP);
  pIndexScanOp->vAppendProperty(pIndexRef);

  return pIndexScanOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void IndexScanOp::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pIndexScanOp = _pQueryNode(pNode);

  sBuf.vConcat("IndexScanOp : "_Stz);

  QueryNode* pIndexRef = IndexScanOp::pIndexRef(pIndexScanOp);
  ASSERT(pIndexRef);

  IndexRef::vDescribe(pIndexRef, sBuf, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void IndexScanOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
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
void IndexScanOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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

    // Get the indexref property
    PlanNode* pIndexRefNode = GraphNav::pFindProperty(const_cast<Stz*>(C_INDEXREF_PROP), pNode);
    ASSERT(pIndexRefNode);
    ASSERT(IndexRef::bIsOfType(pIndexRefNode));

    // Print the index reference as a table reference
    s2K sTblBuf;
    TableRef::vDescribe(pIndexRefNode, sTblBuf, pRootNode);
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("table: (%s)"_Stz, sTblBuf.szStz());

    // And the index reference
    Stz* szIndex = szIndexName(_pQueryNode(pNode));
    if (!szIndex) {
      szIndex = "TBD"_Stz;  // Temporary, for drop 4
    }
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("index: (%s)"_Stz, szIndexName(_pQueryNode(pNode)));

    return;
  }

  // Manual polymorphism here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 IndexScanOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 IndexScanOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IndexScanOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IndexScanOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the tableRef of the queryNode                                      */
/*----------------------------------------------------------------------------*/
QueryNode* IndexScanOp::pTableRef(QueryNode* pNode)
{
  // An indexRef is a subclass of a tableRef.
  QueryNode *pTableRefNode = _pQueryNode(GraphNav::pFindProperty(C_INDEXREF_PROP, pNode));

  ASSERT(pTableRefNode);
  ASSERT(TableRef::bIsOfType(pTableRefNode));

  return pTableRefNode;
}

/*----------------------------------------------------------------------------*/
/* Returns the indexRef of the queryNode                                      */
/*----------------------------------------------------------------------------*/
QueryNode* IndexScanOp::pIndexRef(QueryNode* pNode)
{
  QueryNode *pIndexRefNode = _pQueryNode(GraphNav::pFindProperty(C_INDEXREF_PROP, pNode));

  ASSERT(pIndexRefNode);
  ASSERT(IndexRef::bIsOfType(pIndexRefNode));

  return pIndexRefNode;
}

/*----------------------------------------------------------------------------*/
/* Returns the index name of the index reference                              */
/*----------------------------------------------------------------------------*/
Stz* IndexScanOp::szIndexName(QueryNode *pNode)
{
  QueryNode* pIndexRefNode = pIndexRef(pNode);
  ASSERT(pIndexRefNode);
  ASSERT(IndexRef::bIsOfType(pIndexRefNode));

  return IndexRef::szIndex(pIndexRefNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
