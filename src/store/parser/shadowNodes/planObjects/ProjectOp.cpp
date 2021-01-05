/*============================================================================*/
/* ProjectOp.hpp - operator that changes the defn of rows that it processes   */
/*============================================================================*/

#include "BASE.h"
#include "ProjectOp.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "ProjectedExpr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PROJECTEDEXPRS_PROP = "projectedExprs"_Stz;
static Stz* C_PROJECTEDEXPRS_DEFN = "/__root__/definitions/object/list/projectedExprs"_Stz;
static Stz* C_PROJECTOP_DEFN = "/__root__/definitions/object/planObject/sqlOperator/projectOp"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create ProjectOp nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* ProjectOp::pCreate(QueryNodeList* pSelectItems)
{
  ASSERT(pSelectItems);
  ASSERT(pSelectItems->nItems() > 0);

  // Create the projectedExprs property of the ProjectOp
  Value xValue;
  QueryNode* pProjectedExprs = new QueryNode(C_PROJECTEDEXPRS_PROP, const_cast<Stz*>(C_PROJECTEDEXPRS_DEFN), xValue);

  QueryNode* pItem;
  while ((pItem = pSelectItems->pPop()) != nullptr) {
    pProjectedExprs->vAppendChild(pItem);
  }

  // Create a unique name for this ProjectOp object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(C_PROJECTOP_DEFN), sName);

  // Create the object and add  its projectedExprs property
  QueryNode* pProjectOp = new QueryNode(sName, C_PROJECTOP_DEFN);
  pProjectOp->vAppendProperty(pProjectedExprs);

  return pProjectOp;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ProjectOp::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pProjectOp = static_cast<QueryNode*>(pNode);

  sBuf.vConcat("ProjectOp :"_Stz);

  QueryNode* pProjectedExprs = static_cast<QueryNode*>(GraphNav::pFindProperty(C_PROJECTEDEXPRS_PROP, pProjectOp));
  ASSERT(pProjectedExprs);

  i8 nCols = pProjectedExprs->nChildren();
  for (i8 i=0; i<nCols; i++) {
    QueryNode* pProjectedExpr = static_cast<QueryNode*>(pProjectedExprs->pNthChild(i));
    ASSERT(ProjectedExpr::bIsOfType(pProjectedExpr));

    sBuf.vConcat("\n  "_Stz);
    ProjectedExpr::vDescribe(pProjectedExpr, sBuf);
  }
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void ProjectOp::vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent)
{
  _static_
  ASSERT(bValidate(pNode));

  sBuf.vConcat("\n\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));

  // Get the list of projected expressions
  PlanNode* pProjExprs = GraphNav::pFindProperty(const_cast<Stz*>(C_PROJECTEDEXPRS_PROP), pNode);
  ASSERT(pProjExprs);

  // Iterate each projected expression
  i8 nCols = pProjExprs->nChildren();
  ASSERT(nCols > 0);

  for (i8 i=0; i<nCols; i++) {
    PlanNode* pProjExpr = pProjExprs->pNthChild(i);
    ASSERT(pProjExpr);
    ASSERT(ProjectedExpr::bIsOfType(pProjExpr));

    // Get the description of the projected expression
    s2K sColBuf;
    ProjectedExpr::vDescribe(pProjExpr, sColBuf);
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("column: (%s)"_Stz, sColBuf.szStz());
  }

  // Recurse any children
  i8 nChildren = pNode->nChildren();
  for (i8 i=0; i<nChildren; i++) {
    PlanNode* pChild = pNode->pNthChild(i);
    ASSERT(PlanObject::bIsOfType(pChild));

    PlanObject::vExplain(pChild, sBuf, iIndent+1);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 ProjectOp::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PROJECTOP_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
