/*============================================================================*/
/* StageRange.hpp - an object that identifies a range of SQL compiler stages  */
/*============================================================================*/

#include "BASE.h"
#include "StageRange.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "StringObject.hpp"
#include "IntegerObject.hpp"

#include "GraphNav.hpp"

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
  ShadowNode::vObjName(C_STAGERANGE_NAME, sName);

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
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void StageRange::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pStageRange = _pQueryNode(pNode);

  QueryNode* pBegin = _pQueryNode(GraphNav::pFindProperty(C_BEGIN_PROP, pStageRange));
  QueryNode* pEnd   = _pQueryNode(GraphNav::pFindProperty(C_END_PROP,   pStageRange));

  ASSERT(pBegin);
  // pEnd may be null

  ValueObject::vDescribe(pBegin, sBuf);

  if (pEnd) {
    sBuf.vConcat("-"_Stz);
    ValueObject::vDescribe(pEnd, sBuf);
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



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
