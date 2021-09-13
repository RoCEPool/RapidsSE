/*============================================================================*/
/* UnionOp.hpp - a union of two SELECT queries                                */
/*============================================================================*/

#include "BASE.h"
#include "UnionOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "Expr.hpp"
#include "StringList.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_UNIONOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/setOp/unionOp"_Stz;

static Stz* C_SQL_KEYWORD = "UNION"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create UnionOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* UnionOp::pCreate(b1 bIsDistinct, QueryNode* pCorrelatedByList)
{
  // Create the base SetOp first
  QueryNode* pUnionOp = SetOp::pCreate(bIsDistinct, pCorrelatedByList);

  // Now change its name and type
  s256 sName;
  NodeNamer::vGenerateName(C_UNIONOP_DEFN, sName);

  pUnionOp->vSetName(sName.szStz());
  pUnionOp->vSetType(C_UNIONOP_DEFN);

  return pUnionOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void UnionOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pUnionOp = _pQueryNode(pNode);
  UNUSED(pUnionOp);

  sBuf.vConcat("UnionOp :\n  "_Stz);

  // TODO: ???
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void UnionOp::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
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
void UnionOp::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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
    return;
  }

  // Manual polymorphism here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 UnionOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_UNIONOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 UnionOp::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_UNIONOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely same as its definition         */
/*----------------------------------------------------------------------------*/
b1 UnionOp::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_UNIONOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 UnionOp::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_UNIONOP_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the SQL keyword that this operator is known by. e.g UNION          */
/*----------------------------------------------------------------------------*/
Stz* UnionOp::szSqlKeyword(QueryNode* pNode)
{
  _static_
  UNUSED(pNode); // in release
  ASSERT(bIsOfType(pNode));

  return C_SQL_KEYWORD;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
