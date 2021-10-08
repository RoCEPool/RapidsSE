/*============================================================================*/
/* SubqueryMarker.hpp - Marks the start of some type of subquery              */
/*============================================================================*/

#include "BASE.h"
#include "SubqueryMarker.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

#include "MainSubquery.hpp"
#include "TableSubquery.hpp"
#include "ExprSubquery.hpp"
#include "StringObject.hpp"
#include "CteDef.hpp"
#include "ExprList.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

static Stz* C_DEFN = "/__root__/definitions/object/planObject/planMarker/subqueryMarker"_Stz;

static Stz* C_DEFNPATH_PROP = "defnPath"_Stz;
static Stz* C_DEFNPATH_DEFN = "/__root__/definitions/object/valueObject/stringObject"_Stz;

static Stz* C_SQLTEXT_PROP = "sqlText"_Stz;
static Stz* C_SQLTEXT_DEFN = "/__root__/definitions/object/valueObject/stringObject"_Stz;

static Stz* C_TABLEALIAS_PROP = "tableAlias"_Stz;
static Stz* C_TABLEALIAS_DEFN = "/__root__/definitions/object/valueObject/stringObject"_Stz;

static Stz* C_CORRELATEDEXPRS_PROP = "correlatedExprs"_Stz;
static Stz* C_CORRELATEDEXPRS_DEFN = "/__root__/definitions/object/list/exprList"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Protected----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryMarker::pCreate(Stz* szDefn, Stz* szSqlText, Stz* szAliasName)
{
  ASSERT(szDefn);
  ASSERT(szSqlText);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(szDefn, sName);

  // Create the subqueryMarker
  QueryNode* pSubqueryNode = new QueryNode(sName, szDefn);

  // add the defnPath property, empty for now
  Value xDefnPathVal((Stz*) nullptr);
  QueryNode* pDefnPath = new QueryNode(C_DEFNPATH_PROP, C_DEFNPATH_DEFN, static_cast<Value&&>(xDefnPathVal));
  pSubqueryNode->vAppendProperty(pDefnPath);

  // Add the SQL text
  QueryNode* pSqlText = StringObject::pCreate(szSqlText);
  pSqlText->vSetName(C_SQLTEXT_PROP);
  pSubqueryNode->vAppendProperty(pSqlText);

  // Table alias
  Value xAliasValue(szAliasName);
  QueryNode* pTableAliasNode = new QueryNode(C_TABLEALIAS_PROP, C_TABLEALIAS_DEFN, static_cast<Value&&>(xAliasValue));
  pSubqueryNode->vAppendProperty(pTableAliasNode);

  // Create an empty correlated columns list. This can be added to later
  // once column names have been resolved.
  QueryNode* pCorrelatedColumns = ExprList::pCreate(C_CORRELATEDEXPRS_PROP);
  pSubqueryNode->vAppendProperty(pCorrelatedColumns);

  return pSubqueryNode;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SubqueryMarker::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism of subtypes here
  if      (  MainSubquery::bIsOfType(pNode)) {  MainSubquery::vDescribe(pNode, sBuf); }
  else if ( TableSubquery::bIsOfType(pNode)) { TableSubquery::vDescribe(pNode, sBuf); }
  else if (  ExprSubquery::bIsOfType(pNode)) {  ExprSubquery::vDescribe(pNode, sBuf); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SubqueryMarker::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism of subtypes here
  if      (  MainSubquery::bIsOfType(pNode)) {  MainSubquery::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if ( TableSubquery::bIsOfType(pNode)) { TableSubquery::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (  ExprSubquery::bIsOfType(pNode)) {  ExprSubquery::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SubqueryMarker::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
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
    
    // Table alias
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("table alias: %s"_Stz, szTableAlias(_pQueryNode(pNode)));

    // Get the list of correlated columns
    QueryNode* pCorrColumnsNode = pCorrelatedColumns(_pQueryNode(pNode));
    ASSERT(pCorrColumnsNode);
    ASSERT(ExprList::bIsOfType(pCorrColumnsNode));

    // Describe the list of expressions
    s2K sColBuf;
    if (pCorrColumnsNode->nChildren() > 0) {
      ExprList::vDescribe(pCorrColumnsNode, sColBuf);

    } else {
      sColBuf = "none"_Stz;
    }

    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("correlated columns: (%s)"_Stz, sColBuf.szStz());

    return;
  }


  // Manual polymorphism of subtypes here
  if      (  MainSubquery::bIsOfType(szType)) {  MainSubquery::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);    }
  else if ( TableSubquery::bIsOfType(szType)) { TableSubquery::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);    }
  else if (  ExprSubquery::bIsOfType(szType)) {  ExprSubquery::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);    }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the path of the definition node of this table subquery. May be null*/
/*----------------------------------------------------------------------------*/
Stz* SubqueryMarker::szDefnPath(QueryNode *pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pDefnPathNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_DEFNPATH_PROP), pNode));

  ASSERT(pDefnPathNode);
  ASSERT(StringObject::bIsOfType(pDefnPathNode));

  return pDefnPathNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* The textual representation of this subquery                                */
/*----------------------------------------------------------------------------*/
Stz* SubqueryMarker::szSqlText(QueryNode *pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  QueryNode* pSqlTextNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_SQLTEXT_PROP), pNode));

  ASSERT(pSqlTextNode);
  ASSERT(StringObject::bIsOfType(pSqlTextNode));

  return pSqlTextNode->szValue();
}

/*----------------------------------------------------------------------------*/
/* Returns the path of the definition node of this table subquery (a CteDef). */
/* Path may be null early in the plan.                                        */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryMarker::pCteDefn(QueryNode *pNode, PlanNode* pRootNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  Stz* szPath = szDefnPath(pNode);

  if (!szPath) {
    return nullptr;
  }

  QueryNode* pCteDef = _pQueryNode(GraphNav::pFind(szPath, pRootNode));
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  return pCteDef;
}

/*----------------------------------------------------------------------------*/
/* Sets the path of the node that represents the metadata of this CTE         */
/*----------------------------------------------------------------------------*/
void SubqueryMarker::vSetDefnPath(QueryNode *pNode, Stz *szPath)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(szPath);

  QueryNode* pDefnPathNode = _pQueryNode(GraphNav::pFindProperty(const_cast<Stz*>(C_DEFNPATH_PROP), pNode));
  ASSERT(pDefnPathNode);
  ASSERT(StringObject::bIsOfType(pDefnPathNode));
  ASSERT(pDefnPathNode->xValue().m_bIsNull);

  Value xVal(szPath);
  pDefnPathNode->vSetValue(static_cast<Value&&>(szPath));
}

/*----------------------------------------------------------------------------*/
/* Returns the name of the virtual table that this subquery is known as       */
/*----------------------------------------------------------------------------*/
Stz* SubqueryMarker::szTableAlias(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  PlanNode* pTableAliasNode = GraphNav::pFindProperty(const_cast<Stz*>(C_TABLEALIAS_PROP), pNode);
  ASSERT(pTableAliasNode);

  Stz* szTableAlias = pTableAliasNode->szValue();
  ASSERT(szTableAlias);

  return szTableAlias;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the subquery is correlated with respect to its parent      */
/*----------------------------------------------------------------------------*/
b1 SubqueryMarker::bIsCorrelated(QueryNode* pNode)
{
  UNUSED(pNode);
  ASSERT(bIsOfType(pNode));

  // All table subqueries are non-correlated
  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns the internal list of correlated columns. May be an empty list.     */
/*----------------------------------------------------------------------------*/
QueryNode* SubqueryMarker::pCorrelatedColumns(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  PlanNode* pCorrColumnsNode = GraphNav::pFindProperty(const_cast<Stz*>(C_CORRELATEDEXPRS_PROP), pNode);
  ASSERT(pCorrColumnsNode);
  ASSERT(ExprList::bIsOfType(pCorrColumnsNode));

  return _pQueryNode(pCorrColumnsNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
