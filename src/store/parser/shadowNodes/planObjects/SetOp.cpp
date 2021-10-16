/*============================================================================*/
/* SetOp.hpp - base class of all set operators                                */
/*============================================================================*/

#include "BASE.h"
#include "SetOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "UnionOp.hpp"
#include "StringList.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SETOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/setOp"_Stz;

static Stz* C_ISDISTINCT_PROP = "isDistinct"_Stz;
static Stz* C_ISDISTINCT_DEFN = "/__root__/definitions/object/valueObject/booleanObject"_Stz;

static Stz* C_CORRESPONDING_PROP = "correspondingByList"_Stz;
static Stz* C_CORRESPONDING_DEFN = "/__root__/definitions/object/list/stringList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create SetOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* SetOp::pCreate(b1 bIsDistinct, QueryNode* pCorrelatedByList)
{
  // Create the isDistinct property of the SetOp
  Value xIsDistinctVal(bIsDistinct);
  QueryNode* pIsDistinct = new QueryNode(C_ISDISTINCT_PROP,
                                         C_ISDISTINCT_DEFN,
                                         static_cast<Value&&>(xIsDistinctVal));


  // Create a unique name for this SetOp object
  s256 sName;
  NodeNamer::vGenerateName(C_SETOP_DEFN, sName);

  // Create the object and add its properties
  QueryNode* pSetOp = new QueryNode(sName, C_SETOP_DEFN);

  pSetOp->vAppendProperty(pIsDistinct);
  if (pCorrelatedByList) {
    pCorrelatedByList->vSetName(C_CORRESPONDING_PROP);
    ASSERT(pCorrelatedByList->szType()->bEqual(C_CORRESPONDING_DEFN));

    pSetOp->vAppendProperty(pCorrelatedByList);
  }

  return pSetOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SetOp::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (    UnionOp::bIsOfType(pNode)) {     UnionOp::vDescribe(pNode, sBuf, pRootNode);  }
//else if (IntersectOp::bIsOfType(pNode)) { IntersectOp::vDescribe(pNode, sBuf, pRootNode);   }
//else if (   ExceptOp::bIsOfType(pNode)) {    ExceptOp::vDescribe(pNode, sBuf, pRootNode);   }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SetOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (    UnionOp::bIsOfType(pNode)) {     UnionOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent);  }
//else if (IntersectOp::bIsOfType(pNode)) { IntersectOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent);   }
//else if (   ExceptOp::bIsOfType(pNode)) {    ExceptOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent);   }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SetOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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

    QueryNode* pSetOp = _pQueryNode(pNode);

    // Get the isDistinct property
    b1 bIsDistinct = SetOp::bIsDistinct(pSetOp);

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("isDistinct: (%s)"_Stz, bIsDistinct ? "true"_Stz : "false"_Stz);

    // Get the corresponding by list
    QueryNode* pCorrespondingByList = SetOp::pCorrespondingByList(pSetOp);

    if (pCorrespondingByList) {
      s2K sColumnBuf;
      StringList::vDescribe(pCorrespondingByList, sColumnBuf, pRootNode);

      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, IF_PLAIN);
      sBuf.vCatSprint("corresponding by: (%s)"_Stz, sColumnBuf.szStz());
    }

    return;
  }

  // Manual polymorphism here
  if      (    UnionOp::bIsOfType(szType)) {     UnionOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);  }
//else if (IntersectOp::bIsOfType(szType)) { IntersectOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);   }
//else if (   ExceptOp::bIsOfType(szType)) {    ExceptOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);   }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 SetOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SETOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 SetOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_SETOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely same as its definition         */
/*----------------------------------------------------------------------------*/
b1 SetOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SETOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SetOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_SETOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if non-unique rows are to be filtered out                     */
/*----------------------------------------------------------------------------*/
b1 SetOp::bIsDistinct(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pIsDistinctNode =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_ISDISTINCT_PROP), pNode));

  return pIsDistinctNode->bValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the node representing the list of columns that are to used         */
/*----------------------------------------------------------------------------*/
QueryNode* SetOp::pCorrespondingByList(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pCorrespondingByList =
      _pQueryNode(GraphNav::pFindProperty(const_cast<Stz *>(C_CORRESPONDING_PROP), pNode));

  return pCorrespondingByList;
}

/*----------------------------------------------------------------------------*/
/* Remove and null the CORRESPONDING BY list                                  */
/*----------------------------------------------------------------------------*/
void SetOp::vRemoveCorrespondingByList(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pProp = pNode->pRemoveProperty(C_CORRESPONDING_PROP);

  if (pProp) {
    delete pProp;
  }
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions    */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* SetOp::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);
  ASSERT(pNode->nChildren() == 2);

  // Eventually, any CORRELATED BY list will be pushed down and nulled in this op.
  // Therefore the list of column expressions is the same in either child and
  // we will simply select the first child in this case.

  QueryNode* pChild = _pQueryNode(pNode->pFirstChild());

  return PlanObject::pColumnExprs(pChild, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the SQL keyword that this operator is known by. e.g UNION          */
/*----------------------------------------------------------------------------*/
Stz* SetOp::szSqlKeyword(QueryNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (    UnionOp::bIsOfType(pNode)) { return     UnionOp::szSqlKeyword(pNode);  }
//else if (IntersectOp::bIsOfType(pNode)) { return IntersectOp::szSqlKeyword(pNode);   }
//else if (   ExceptOp::bIsOfType(pNode)) { return    ExceptOp::szSqlKeyword(pNode);   }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
