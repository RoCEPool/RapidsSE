/*============================================================================*/
/* StageRange.hpp - an object that identifies a range of SQL compiler stages  */
/*============================================================================*/

#include "BASE.h"
#include "StageRange.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "StringObject.hpp"
#include "IntegerObject.hpp"
#include "ExplainOp.hpp"
#include "StringList.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_STAGERANGE_DEFN   = "/__root__/definitions/object/compound/stageRange"_Stz;
static Stz* C_STAGERANGE_NAME   = "stageRange"_Stz;

static Stz* C_BEGIN_PROP  = "begin"_Stz;
static Stz* C_BEGIN_DEFN  = "/__root__/definitions/object/valueObject"_Stz;

static Stz* C_END_PROP  = "end"_Stz;
static Stz* C_END_DEFN  = "/__root__/definitions/object/valueObject"_Stz;

Stz* StageRange::C_RANGE_ALL = "*"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Helper function to create indexDef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* StageRange::pCreate(QueryNode* pBegin, QueryNode* pEnd)
{
  _static_
  ASSERT(pBegin);
  ASSERT(StringObject::bIsOfType(pBegin) || IntegerObject::bIsOfType(pBegin));
  ASSERT(!pEnd || StringObject::bIsOfType(pEnd) || IntegerObject::bIsOfType(pEnd));

  s256 sName;
  NodeNamer::vGenerateName(C_STAGERANGE_DEFN, sName);

  // Create the stageRange object
  QueryNode* pStageRange = new QueryNode(sName.szStz(), C_STAGERANGE_DEFN);

  // Add the begin property
  pBegin->vSetName(C_BEGIN_PROP);
  pStageRange->vAppendProperty(pBegin);

  if (pEnd) {
    pEnd->vSetName(C_END_PROP);
    pStageRange->vAppendProperty(pEnd);
  }

  return pStageRange;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create a StageRange that represents the last stage only */
/*----------------------------------------------------------------------------*/
QueryNode* StageRange::pCreateLastOnly()
{
  _static_

  s256 sName;
  NodeNamer::vGenerateName(C_STAGERANGE_DEFN, sName);

  // Create the stageRange object
  QueryNode* pStageRange = new QueryNode(sName.szStz(), C_STAGERANGE_DEFN);

  // Add the begin property
  QueryNode* pBegin = StringObject::pCreate(new Text(ExplainOp::szLastRegularExplainStage()));
  pBegin->vSetName(C_BEGIN_PROP);
  pStageRange->vAppendProperty(pBegin);

  return pStageRange;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void StageRange::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pStageRange = _pQueryNode(pNode);

  QueryNode* pBegin = _pQueryNode(GraphNav::pFindProperty(C_BEGIN_PROP, pStageRange));
  QueryNode* pEnd   = _pQueryNode(GraphNav::pFindProperty(C_END_PROP,   pStageRange));

  ASSERT(pBegin);
  // pEnd may be null

  ValueObject::vDescribe(pBegin, sBuf, pRootNode);

  if (pEnd) {
    sBuf.vConcat("-"_Stz);
    ValueObject::vDescribe(pEnd, sBuf, pRootNode);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the stageRange definition     */
/*----------------------------------------------------------------------------*/
b1 StageRange::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STAGERANGE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StageRange::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STAGERANGE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the stage num/name is contained in this range              */
/*----------------------------------------------------------------------------*/
b1 StageRange::bInRange(QueryNode* pNode, i8 iStageNum, Stz* szStageName, QueryNode* pStageNameList)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(iStageNum > 0);
  ASSERT(szStageName);
  ASSERT(pStageNameList);
  ASSERT(StringList::bIsOfType(pStageNameList));

  // We are in range if stageNum/name is >= begin and <= end.
  // If either begin or end is '*' then this means all stages and
  // is considered inclusive.

  QueryNode* pBegin = _pQueryNode(GraphNav::pFindProperty(C_BEGIN_PROP, pNode));
  QueryNode* pEnd   = _pQueryNode(GraphNav::pFindProperty(C_END_PROP,   pNode));
  ASSERT(pBegin);  // pEnd may be null

  i8   iBeginVal;
  i8   iEndVal;
  Stz* szBeginVal;
  Stz* szEndVal;

  if (pEnd) {
    // We have a range
    // Range begin
    if (IntegerObject::bIsExactType(pBegin)) {
      iBeginVal = pBegin->iValue();

      if (iStageNum < iBeginVal) {
        return false;
      }

    } else if (StringObject::bIsExactType(pBegin)) {
      szBeginVal = pBegin->szValue();

      if (!szStageName->bSame(szBeginVal) &&
          !szBeginVal->bEqual(C_RANGE_ALL) &&
          !StringList::bContains(pStageNameList, false, szBeginVal)) {
        return false;
      }

    } else {
      NEVER_REACH
    }

    // range end
    if (IntegerObject::bIsExactType(pEnd)) {
      iEndVal = pEnd->iValue();

      if (iStageNum > iEndVal) {
        return false;
      }

    } else if (StringObject::bIsExactType(pEnd)) {
      szEndVal = pEnd->szValue();

      if (!szStageName->bSame(szEndVal) &&
          !szEndVal->bEqual(C_RANGE_ALL) &&
          StringList::bContains(pStageNameList, false, szEndVal)) {
        return false;
      }

    } else {
      NEVER_REACH
    }

    return true;

  } else {
    // We don't have a range. Instead we have a specific value
    if (IntegerObject::bIsExactType(pBegin)) {
      iBeginVal = pBegin->iValue();

      return (iStageNum == iBeginVal);

    } else if (StringObject::bIsExactType(pBegin)) {
      szBeginVal = pBegin->szValue();

      return (szStageName->bSame(szBeginVal) || szBeginVal->bEqual(C_RANGE_ALL));

    } else {
      NEVER_REACH
    }
  }

}

/*----------------------------------------------------------------------------*/
/* Is the current stage equal to or beyond the end range?                     */
/*----------------------------------------------------------------------------*/
b1 StageRange::bAtOrBeyondEnd(QueryNode* pNode, i8 iStageNum, QueryNode* pStageNameList)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(iStageNum > 0);
  ASSERT(pStageNameList);
  ASSERT(StringList::bIsOfType(pStageNameList));

  QueryNode* pBegin = _pQueryNode(GraphNav::pFindProperty(C_BEGIN_PROP, pNode));
  QueryNode* pEnd   = _pQueryNode(GraphNav::pFindProperty(C_END_PROP,   pNode));
  ASSERT(pBegin);  // pEnd may be null


  if (pEnd) {
    if (IntegerObject::bIsExactType(pEnd)) {
      i8 iEndVal = pEnd->iValue();
      return (iStageNum >= iEndVal);

    } else if (StringObject::bIsExactType(pEnd)) {
      Stz* szEndVal = pEnd->szValue();

      // if end value is '*' then we are never at or beyond it
      if (szEndVal->bEqual(C_RANGE_ALL)) {
        return false;
      }

      // We are only beyond a stage name if we can find it in the stage name list
      return StringList::bContains(pStageNameList, false, szEndVal);
      
    } else {
      NEVER_REACH
    }
  }
  
  // We don't have an end range. So we must be at or beyond the start range
  if (IntegerObject::bIsExactType(pBegin)) {
    i8 iBeginVal = pBegin->iValue();
    return (iStageNum >= iBeginVal);

  } else if (StringObject::bIsExactType(pBegin)) {
    Stz* szBeginVal = pBegin->szValue();

    // if begin value is '*' then we are never at or beyond it
    if (szBeginVal->bEqual(C_RANGE_ALL)) {
      return false;
    }

    // We are only beyond a stage name if we can find it in the stage name list
    return StringList::bContains(pStageNameList, false, szBeginVal);

  } else {
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
