/*============================================================================*/
/* StatementMarker.hpp - identifies the type of query being planned           */
/*                                                                            */
/* Only intending to create basic subclasses of this so that it is easier to  */
/* assert that a node is of a given type. Most other functionality falls back */
/* to this base class.                                                        */
/*============================================================================*/

#include "BASE.h"
#include "StatementMarker.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_QUERYTEXT_PROP = "queryText"_Stz;
static Stz* C_STATEMENT_DEFN = "/__root__/definitions/object/planObject/statementMarker"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create statement nodes. These are the top level         */
/* "headings" in a plan graph that identify what comes below them.            */
/*----------------------------------------------------------------------------*/
QueryNode* StatementMarker::pCreate(eST eStmtType, Stz* szDefn, Stz* szQueryText)
{
  ASSERT(eStmtType > ST_NONE);
  ASSERT(eStmtType < ST_NENUMS);
  ASSERT(szDefn);
  ASSERT(szQueryText);

  // Store the enum value in the value field of the statement node
  Value xValue((i8) eStmtType);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(szDefn, sName);

  // Create the object with its queryText property
  QueryNode* pStmt = new QueryNode(sName, szDefn, static_cast<Value&&>(xValue));
  Value xQueryTextVal(szQueryText);
  QueryNode* pQueryText = new QueryNode(C_QUERYTEXT_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN_NONNULL), static_cast<Value&&>(xQueryTextVal));
  pStmt->vAppendProperty(pQueryText);

  return pStmt;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void StatementMarker::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pStmt = _pQueryNode(pNode);

  buf.vConcat(GraphNav::szLeafType(pStmt->szType()));
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void StatementMarker::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (sBuf.nLength() > 0) {
    sBuf.vConcat("\n\n"_Stz);
  }

  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vCatSprint("StatementMarker: %s"_Stz, GraphNav::szLeafType(pNode->szType()));

  PlanNode* pQueryTextProp = GraphNav::pFindProperty(const_cast<Stz*>(C_QUERYTEXT_PROP), pNode);
  ASSERT(pQueryTextProp);

  Stz* szQueryText = pQueryTextProp->szValue();
  ASSERT(szQueryText);

  // Remove new lines from the query text as it messes with the EXPLAIN output
  while (StrLib::bReplace(szQueryText->azChar(), "\n", " "));

  // Add the queryText property
  sBuf.vConcat("\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_PLAIN);
  sBuf.vCatSprint("queryText: \"%s\""_Stz, szQueryText);

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
b1 StatementMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STATEMENT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StatementMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STATEMENT_DEFN);
}


/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* StatementMarker::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Statements don't change the columns being returned. So ask its descendant.
  ASSERT(pNode->iNumChildren() == 1);
  return PlanObject::pColumnExprs(_pQueryNode(pNode->pFirstChild()), pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the statement enum type from a given node                          */
/*----------------------------------------------------------------------------*/
StatementMarker::eST StatementMarker::eStatementType(PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  i8 iType = pNode->iValue();
  ASSERT(iType > ST_NONE);
  ASSERT(iType < ST_NENUMS);

  return (eST) iType;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
