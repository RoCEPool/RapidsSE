/*============================================================================*/
/* Statement.hpp - identifies the type of query being planned                 */
/*                                                                            */
/* Not intending to create subclasses of this to represent each statement type*/
/* because it doesn't really add any value. Instead we can capture all        */
/* behaviour in this class.                                                   */
/*============================================================================*/

#include "BASE.h"
#include "StatementMarker.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_QUERYTEXT_PROP = "queryText"_Stz;
static Stz* C_STATEMENT_DEFN = "/__root__/definitions/object/planObject/statementMarker"_Stz;

// Definition paths of the above statement types
static Stz* aST_definition[] = {
  /*ST_NONE         */ ""_Stz,
  /*ST_EXPLAIN      */ ""_Stz,
  /*ST_CREATETABLE  */ ""_Stz,
  /*ST_DROPTABLE    */ ""_Stz,
  /*ST_CREATEINDEX  */ ""_Stz,
  /*ST_DROPINDEX    */ ""_Stz,
  /*ST_CTAS         */ ""_Stz,
  /*ST_SELECT       */ "/__root__/definitions/object/planObject/statementMarker/dml/select"_Stz,
  /*ST_INSERT       */ ""_Stz,
  /*ST_UPDATE       */ ""_Stz,
  /*ST_DELETE       */ ""_Stz,
  /*ST_INSERTSELECT */ ""_Stz,
  /*ST_ENUMS        */ ""_Stz
};


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create statement nodes. These are the top level         */
/* "headings" in a plan graph that identify what comes below them.            */
/*----------------------------------------------------------------------------*/
QueryNode* StatementMarker::pCreate(eST eStmtType, Stz* szQueryText)
{
  ASSERT(eStmtType > ST_NONE);
  ASSERT(eStmtType < ST_NENUMS);
  ASSERT(szQueryText);

  // Get the definition of the statement we're creating
  Stz* szDef = aST_definition[eStmtType];

  // Store the enum value in the value field of the statement node
  Value xValue((i8) eStmtType);

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(szDef), sName);

  // Create the object with its queryText property
  QueryNode* pStmt = new QueryNode(sName, szDef, static_cast<Value&&>(xValue));
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

  QueryNode* pStmt = static_cast<QueryNode*>(pNode);

  buf.vConcat(GraphNav::szLeafType(pNode->szType()));
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void StatementMarker::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (sBuf.nLength() > 0) {
    sBuf.vConcat("\n"_Stz);
  }

  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vCatSprint("%s statement"_Stz, GraphNav::szLeafType(pNode->szType()));

  PlanNode* pQueryTextProp = GraphNav::pFindProperty(const_cast<Stz*>(C_QUERYTEXT_PROP), pNode);
  ASSERT(pQueryTextProp);

  Stz* szQueryText = pQueryTextProp->szValue();
  ASSERT(szQueryText);

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
PlanNodeList* StatementMarker::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Statements don't change the columns being returned. So ask its descendant.
  ASSERT(pNode->iNumChildren() == 1);
  return PlanObject::pColumnExprs(static_cast<QueryNode*>(pNode->pFirstChild()), pRootNode);
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
