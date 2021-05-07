/*============================================================================*/
/* StageMarker.hpp - indicates to the transliteration when a new pipeline     */
/* stage is required                                                          */
/*============================================================================*/

#include "BASE.h"
#include "StageMarker.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_STAGEMARKER_DEFN = "/__root__/definitions/object/planObject/planMarker/stageMarker"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create stageMarker nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* StageMarker::pCreate()
{
  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_STAGEMARKER_DEFN, sName);

  // Create the object
  QueryNode* pMarker = new QueryNode(sName, C_STAGEMARKER_DEFN);

  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void StageMarker::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pStmt = _pQueryNode(pNode);

  buf.vConcat(GraphNav::szLeafType(pStmt->szType()));
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain to the buf             */
/*----------------------------------------------------------------------------*/
void StageMarker::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (sBuf.nLength() > 0) {
    sBuf.vConcat("\n\n"_Stz);
  }

  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vCatSprint("StageMarker: "_Stz);

  // Recurse any children
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, sBuf, iIndent + 1);
    }

    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 StageMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STAGEMARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StageMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STAGEMARKER_DEFN);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
