/*============================================================================*/
/* ExplainOp.hpp - operator that explains the underlying plan                 */
/*============================================================================*/

#include "BASE.h"
#include "ExplainOp.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "StageRangeList.hpp"
#include "StageRange.hpp"
#include "StringObject.hpp"

#include "GraphNav.hpp"
#include "SqlException.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_EXPLAINOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/explainOp"_Stz;

static Stz* C_STAGERANGELIST_PROP = "stageRangeList"_Stz;
static Stz* C_STAGERANGELIST_DEFN = "/__root__/definitions/object/list/stageRangeList"_Stz;

static Stz* C_FORMAT_PROP = "format"_Stz;
static Stz* C_FORMAT_DEFN = "/__root__/definitions/object/valueObject/stringObject"_Stz;

static Stz* C_STAGECOUNT_PROP = "stageCount"_Stz;
static Stz* C_STAGECOUNT_DEFN = "/__root__/definitions/object/valueObject/integerObject"_Stz;

static Stz* C_STAGESPAST_PROP = "stagesPast"_Stz;
static Stz* C_STAGESPAST_DEFN = "/__root__/definitions/object/list/stringList"_Stz;

static Stz* C_FORMAT_PRETTY = "PRETTY"_Stz;
static Stz* C_FORMAT_ROW    = "ROW"_Stz;

// Name of the last stage prior to transliteration and conversion to
// a physical pipeline statement
static Stz* C_LAST_REGULAR_EXPLAIN_STAGE = "explainPlan"_Stz;

// Name of the last stage that can be explained
static Stz* C_LAST_EXPLAINABLE_STAGE = "transliterate"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ExplainOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* ExplainOp::pCreate(QueryNode* pStageRangeList, Stz* szFormat)
{
  ASSERT(pStageRangeList);

  // Create a unique name for this ExplainOp object
  s256 sName;
  NodeNamer::vGenerateName(C_EXPLAINOP_DEFN, sName);

  // Create the explain object
  QueryNode* pExplainOp = new QueryNode(sName, C_EXPLAINOP_DEFN);

  // Add the stageRangeList property
  pStageRangeList->vSetName(C_STAGERANGELIST_PROP);
  pExplainOp->vAppendProperty(pStageRangeList);

  // Create and add the format property
  Value xFmtVal;
  if (szFormat) {
    if (szFormat->bSame(C_FORMAT_PRETTY)) {
      xFmtVal.vSet(new Text(C_FORMAT_PRETTY));

    } else if (szFormat->bSame(C_FORMAT_ROW)) {
      xFmtVal.vSet(new Text(C_FORMAT_ROW));

    } else {
      SemanticException::vThrow(EC_EXPLAIN_INVALID_FORMAT, ": %s"_Stz, szFormat);
    }
  } else {
    // Use the pretty format as default
    xFmtVal.vSet(new Text(C_FORMAT_PRETTY));
  }

  QueryNode* pFormat = new QueryNode(C_FORMAT_PROP, C_FORMAT_DEFN, static_cast<Value&&>(xFmtVal));
  pExplainOp->vAppendProperty(pFormat);

  // Create and add the stageCount property
  Value xStageCountVal((i8) 0);
  QueryNode* pStageCount = new QueryNode(C_STAGECOUNT_PROP, C_STAGECOUNT_DEFN, static_cast<Value&&>(xStageCountVal));
  pExplainOp->vAppendProperty(pStageCount);

  // Create and add the stagesPast property
  QueryNode* pStagesPast = new QueryNode(C_STAGESPAST_PROP, C_STAGESPAST_DEFN);
  pExplainOp->vAppendProperty(pStagesPast);

  return pExplainOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ExplainOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExplainOp = _pQueryNode(pNode);
  UNUSED(pExplainOp);

  sBuf.vConcat("ExplainOp :\n  "_Stz);
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void ExplainOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

  // Add the local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children of pNode
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
void ExplainOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s256 sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object

    // Get the stageRangeList
    PlanNode* pStageRangeList = GraphNav::pFindProperty(const_cast<Stz*>(C_STAGERANGELIST_PROP), pNode);
    ASSERT(pStageRangeList);

    if (pStageRangeList->nChildren() != 0) {
      s2K sStageBuf;
      StageRangeList::vDescribe(pStageRangeList, sStageBuf);
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("stageRangeList: %s"_Stz, sStageBuf.szStz());
    }

    // Print the format
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    PlanNode* pFormat = GraphNav::pFindProperty(const_cast<Stz*>(C_FORMAT_PROP), pNode);
    ASSERT(StringObject::bIsOfType(pFormat));
    sBuf.vCatSprint("format: %s"_Stz, pFormat->szValue());

    return;
  }

  // Manual polymorphism here

}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 ExplainOp::bIsOfType(PlanNode *pNode)
{
  _static_
  return ShadowNode::bIsOfType(pNode, C_EXPLAINOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 ExplainOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_EXPLAINOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExplainOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_EXPLAINOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExplainOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_EXPLAINOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions    */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* ExplainOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  UNUSED(pNode);
  UNUSED(pRootNode);

  // There should never be a need to call this function on an ExplainOp,
  // even if we were explaining an explained query.
  NEVER_REACH

//  _static_
//  ASSERT(bIsOfType(pNode));
//  ASSERT(pRootNode);
//
//  // The ExplainOp doesn't change the projected expressions, so ask its child.
//  QueryNode* pChild = _pQueryNode(pNode->pFirstChild());
//  ASSERT(pChild);
//
//  return PlanObject::pColumnExprs(pChild, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Increments the number of stages (operators in the pipeline) encountered    */
/*----------------------------------------------------------------------------*/
void ExplainOp::vIncStageCount(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  // Get the stageCount property
  QueryNode* pStageCountNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGECOUNT_PROP), pNode));
  ASSERT(pStageCountNode);
  ASSERT(ShadowNode::bIsExactType(pStageCountNode, C_STAGECOUNT_DEFN));

  i8 iCount = pStageCountNode->iValue();
  ++iCount;
  Value xValue(iCount);
  pStageCountNode->vSetValue(static_cast<Value&&>(xValue));
}

/*----------------------------------------------------------------------------*/
/* Get the current stage count                                                */
/*----------------------------------------------------------------------------*/
i8 ExplainOp::iStageCount(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  // Get the stageCount property
  QueryNode* pStageCountNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGECOUNT_PROP), pNode));
  ASSERT(pStageCountNode);
  ASSERT(ShadowNode::bIsExactType(pStageCountNode, C_STAGECOUNT_DEFN));

  i8 iCount = pStageCountNode->iValue();
  return iCount;
}

/*----------------------------------------------------------------------------*/
/* Record the name of a compiler stage encountered by appending the stage name*/
/* to the stagesPast list.                                                    */
/*----------------------------------------------------------------------------*/
void ExplainOp::vAddCompilerStageName(QueryNode* pNode, Stz* szStageName)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(szStageName->nLength() > 0);

  // Get the stagesPast property
  QueryNode* pStagesPastNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGESPAST_PROP), pNode));
  ASSERT(pStagesPastNode);
  ASSERT(ShadowNode::bIsExactType(pStagesPastNode, C_STAGESPAST_DEFN));

  // Create a new StringObject to add to this list
  QueryNode* pChild = StringObject::pCreate(new Text(szStageName));
  pStagesPastNode->vAppendChild(pChild);
}

/*----------------------------------------------------------------------------*/
/* Get the last compiler stage name encountered                               */
/*----------------------------------------------------------------------------*/
Stz* ExplainOp::szLastStageName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  // Get the stagesPast property
  QueryNode* pStagesPastNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGESPAST_PROP), pNode));
  ASSERT(pStagesPastNode);
  ASSERT(ShadowNode::bIsExactType(pStagesPastNode, C_STAGESPAST_DEFN));

  QueryNode* pLastChild = _pQueryNode(pStagesPastNode->pLastChild());

  if (!pLastChild) {
    return nullptr;
  }

  ASSERT(StringObject::bIsOfType(pLastChild));
  return pLastChild->szValue();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the current stage needs to be explained.                   */
/* Uses the # stages encountered and the list of stage names past to          */
/* determine this.                                                            */
/*----------------------------------------------------------------------------*/
b1 ExplainOp::bExplainStage(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  b1 bInRange = false;

  // Get the current stage number and name
  i8 iStageNum;
  Stz* szStageName;

  QueryNode* pStageCountNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGECOUNT_PROP), pNode));
  ASSERT(pStageCountNode);
  ASSERT(ShadowNode::bIsExactType(pStageCountNode, C_STAGECOUNT_DEFN));

  iStageNum = pStageCountNode->iValue();
  ASSERT(iStageNum > 0);

  // Get the stagesPast property
  QueryNode* pStagesPastNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGESPAST_PROP), pNode));
  ASSERT(pStagesPastNode);
  ASSERT(ShadowNode::bIsExactType(pStagesPastNode, C_STAGESPAST_DEFN));

  QueryNode* pLastStagesPastChild = _pQueryNode(pStagesPastNode->pLastChild());
  ASSERT(pLastStagesPastChild);
  ASSERT(StringObject::bIsExactType(pLastStagesPastChild));

  szStageName = pLastStagesPastChild->szValue();
  ASSERT(szStageName);

  // Now use the stage number and name to determine if it fits in any of the stage ranges

  // Get the stageRangeList property and iterate the ranges
  QueryNode* pStageRangeList = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGERANGELIST_PROP), pNode));
  ASSERT(pStageRangeList);
  ASSERT(StageRangeList::bIsExactType(pStageRangeList));
  ASSERT(pStageRangeList->nChildren() > 0);

  PlanNodeIter* pIter = pStageRangeList->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pStageRange = _pQueryNode(pIter->pNext());

    if (StageRange::bInRange(pStageRange, iStageNum, szStageName, pStagesPastNode)) {
      bInRange = true;
      break;
    }
  }

  delete pIter;
  return bInRange;
}

/*----------------------------------------------------------------------------*/
/* Returns true if we have reached the last stage to be explained             */
/*----------------------------------------------------------------------------*/
b1 ExplainOp::bExitEarly(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  // Get the name of the last stage encountered and compare it to the last
  // explainable stage.
  Stz* szLastStageName = ExplainOp::szLastStageName(pNode);
  if (szLastStageName->bSame(C_LAST_EXPLAINABLE_STAGE)) {
    return true;
  }

  // Get the stage count
  QueryNode* pStageCountNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGECOUNT_PROP), pNode));
  ASSERT(pStageCountNode);
  ASSERT(ShadowNode::bIsExactType(pStageCountNode, C_STAGECOUNT_DEFN));

  i8 iStageNum = pStageCountNode->iValue();
  ASSERT(iStageNum > 0);

  // Get the stagesPast property
  QueryNode* pStagesPastNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGESPAST_PROP), pNode));
  ASSERT(pStagesPastNode);
  ASSERT(ShadowNode::bIsExactType(pStagesPastNode, C_STAGESPAST_DEFN));

  // Get the stageRangeList property and iterate the ranges
  QueryNode* pStageRangeList = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_STAGERANGELIST_PROP), pNode));
  ASSERT(pStageRangeList);
  ASSERT(StageRangeList::bIsExactType(pStageRangeList));
  ASSERT(pStageRangeList->nChildren() > 0);

  b1 bEarlyExit = true;

  PlanNodeIter* pIter = pStageRangeList->pNewChildrenIter();
  while (pIter->bHasNext()) {
    QueryNode* pStageRange = _pQueryNode(pIter->pNext());

    // We can only exit early if we are at or beyond all end ranges
    if (!StageRange::bAtOrBeyondEnd(pStageRange, iStageNum, pStagesPastNode)) {
      bEarlyExit = false;
      break;
    }
  }

  delete pIter;
  return bEarlyExit;
}

/*----------------------------------------------------------------------------*/
/* Return the name of the last stage before transliteration stages            */
/*----------------------------------------------------------------------------*/
Stz* ExplainOp::szLastRegularExplainStage()
{
  return C_LAST_REGULAR_EXPLAIN_STAGE;
}

/*----------------------------------------------------------------------------*/
/* Return the tabledef name for the definition of the explain table           */
/* for the current format. Must return one of the C_TDEF_* constants.         */
/*----------------------------------------------------------------------------*/
Stz* ExplainOp::szTableDef(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  // Get the format property, if it exists
  QueryNode* pFormatNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_FORMAT_PROP), pNode));
  ASSERT(!pFormatNode || ShadowNode::bIsExactType(pFormatNode, C_FORMAT_DEFN));

  if (!pFormatNode) {
    return C_TDEF_EXPLAIN;
  }

  Stz* szFormat = pFormatNode->szValue();

  if (szFormat->bSame(C_FORMAT_PRETTY)) {
    return C_TDEF_EXPLAIN;

  } else if (szFormat->bSame(C_FORMAT_ROW)) {
    NOT_IMPLEMENTED_YET

  } else {
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
