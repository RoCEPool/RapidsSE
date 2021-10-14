/*============================================================================*/
/* LimitOp.hpp - operator that limits the maximum number of rows being returned*/
/*============================================================================*/

#include "BASE.h"
#include "LimitOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LIMITOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/limitOp"_Stz;

static Stz* C_LIMITPROP_NAME = "limit"_Stz;
static Stz* C_LIMITPROP_DEFN = "/__root__/definitions/object/valueObject/integerObject"_Stz;
static Stz* C_OFFSETPROP_NAME = "offset"_Stz;
static Stz* C_OFFSETPROP_DEFN = "/__root__/definitions/object/valueObject/integerObject"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create LimitOp nodes.                                   */
/* Set offset to -1 if it is not applicable.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* LimitOp::pCreate(i8 iLimit, i8 iOffset)
{
  ASSERT(iLimit >= 0);
  ASSERT(iOffset == -1 || iOffset >= 0);

  // Create a unique name for this LimitOp object
  s256 sName;
  NodeNamer::vGenerateName(C_LIMITOP_DEFN, sName);

  // Create the object and add its limit and offset properties
  QueryNode* pLimitOp = new QueryNode(sName, C_LIMITOP_DEFN);

  Value xLimitVal(iLimit);
  QueryNode* pLimitProp = new QueryNode(C_LIMITPROP_NAME, C_LIMITPROP_DEFN, static_cast<Value&&>(xLimitVal));

  Value xOffsetVal;
  if (iOffset == -1)  xOffsetVal.vSetNull(Value::VT_INT);
  else                xOffsetVal.vSet(iOffset);

  QueryNode* pOffsetProp = new QueryNode(C_OFFSETPROP_NAME, C_OFFSETPROP_DEFN, static_cast<Value&&>(xOffsetVal));

  pLimitOp->vAppendProperty(pLimitProp);
  pLimitOp->vAppendProperty(pOffsetProp);

  return pLimitOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void LimitOp::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
      ASSERT(bIsOfType(pNode));

  QueryNode* pLimitOp = _pQueryNode(pNode);

  QueryNode* pLimitProp = _pQueryNode(GraphNav::pFindProperty(C_LIMITPROP_NAME, pLimitOp));
  QueryNode* pOffsetProp = _pQueryNode(GraphNav::pFindProperty(C_OFFSETPROP_NAME, pLimitOp));
  ASSERT(pLimitOp);
  ASSERT(pOffsetProp);

  sBuf.vCatSprint("LimitOp : Limit = %lld"_Stz, pLimitProp->iValue());

  if (!pOffsetProp->bIsNullValue()) {
    sBuf.vCatSprint(", Offset = %lld"_Stz, pOffsetProp->iValue());
  }
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void LimitOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
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
void LimitOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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

    // Get the limit and offset properties
    PlanNode* pLimitProp = GraphNav::pFindProperty(const_cast<Stz*>(C_LIMITPROP_NAME), pNode);
    ASSERT(pLimitProp);

    PlanNode* pOffsetProp = GraphNav::pFindProperty(const_cast<Stz*>(C_OFFSETPROP_NAME), pNode);
    ASSERT(pOffsetProp);

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("limit: (%lld)"_Stz, pLimitProp->iValue());

    if (!pOffsetProp->bIsNullValue()) {
      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("offset: (%lld)"_Stz, pOffsetProp->iValue());
    }

    return;
  }

  // Manual polymorphism here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 LimitOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_LIMITOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 LimitOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_LIMITOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 LimitOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_LIMITOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 LimitOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_LIMITOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the limit value of the limitOp                                     */
/*----------------------------------------------------------------------------*/
i8 LimitOp::iLimit(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode *pLimitNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_LIMITPROP_NAME), pNode));

  ASSERT(pLimitNode);
  return pLimitNode->iValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the offset value of the limitOp, or -1 if not set                  */
/*----------------------------------------------------------------------------*/
i8 LimitOp::iOffset(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode *pOffsetNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_OFFSETPROP_NAME), pNode));

  ASSERT(pOffsetNode);

  if (pOffsetNode->bIsNullValue()) {
    return -1;
  }

  return pOffsetNode->iValue();
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumn that represent the column expressions     */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* LimitOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  // The LimitOp doesn't change the projected expressions, so ask its child.
  QueryNode* pChild = _pQueryNode(pNode->pFirstChild());
  ASSERT(pChild);

  return PlanObject::pColumnExprs(pChild, pRootNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
