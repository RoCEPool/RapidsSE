/*============================================================================*/
/* Statement.hpp - identifies the type of query being planned                 */
/*                                                                            */
/* Not intending to create subclasses of this to represent each statement type*/
/* because it doesn't really add any value. Instead we can capture all        */
/* behaviour in this class.                                                   */
/*============================================================================*/

#include "BASE.h"
#include "Statement.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_QUERYTEXT_PROP = "queryText"_Stz;
static Stz* C_STATEMENT_DEFN = "/__root__/definitions/object/planObject/statement"_Stz;

// Definition paths of the above statement types
static Stz* aST_definition[] = {
  /*ST_NONE         */ ""_Stz,
  /*ST_CREATETABLE  */ ""_Stz,
  /*ST_DROPTABLE    */ ""_Stz,
  /*ST_CREATEINDEX  */ ""_Stz,
  /*ST_DROPINDEX    */ ""_Stz,
  /*ST_CTAS         */ ""_Stz,
  /*ST_SELECT       */ "/__root__/definitions/object/planObject/statement/dml/select"_Stz,
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
QueryNode* Statement::pCreate(eST eStmtType, Stz* szQueryText)
{
  ASSERT(eStmtType > ST_NONE);
  ASSERT(eStmtType < ST_NENUMS);
  ASSERT(szQueryText);

  // Get the definition of the statement we're creating
  Stz* szDef = aST_definition[eStmtType];

  // Store the enum value in the value field of the statement node
  Value xValue;
  xValue.m_iInt = eStmtType;

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(szDef), sName);

  // Create the object with its queryText property
  QueryNode* pStmt = new QueryNode(sName, szDef, xValue);
  Value xQueryTextVal(szQueryText);
  QueryNode* pQueryText = new QueryNode(C_QUERYTEXT_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN_NONNULL), xQueryTextVal);
  pStmt->vAppendProperty(pQueryText);

  return pStmt;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Statement::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pStmt = static_cast<QueryNode*>(pNode);

  buf.vConcat(GraphNav::szLeafType(pNode->szType()));
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void Statement::vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent)
{
  _static_
  ASSERT(bValidate(pNode));

  if (sBuf.nLength() > 0) {
    sBuf.vConcat("\n"_Stz);
  }

  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);
  sBuf.vCatSprint("%s statement"_Stz, GraphNav::szLeafType(pNode->szType()));

  PlanNode* pQueryTextProp = GraphNav::pFindProperty(const_cast<Stz*>(C_QUERYTEXT_PROP), pNode);
  ASSERT(pQueryTextProp);

  Stz* szQueryText = pQueryTextProp->getStringValue();
  ASSERT(szQueryText);

  // Add the queryText property
  sBuf.vConcat("\n"_Stz);
  vAppendIndent(sBuf, iIndent, eIndentFormat::IF_PLAIN);
  sBuf.vCatSprint("queryText: \"%s\""_Stz, szQueryText);

  // Recurse any children
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, sBuf, iIndent+1);
    }

    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 Statement::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STATEMENT_DEFN);
}


/*----------------------------------------------------------------------------*/
/* Returns the statement enum type from a given node                          */
/*----------------------------------------------------------------------------*/
Statement::eST Statement::eStatementType(PlanNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  i8 iType = pNode->getIntValue();
  ASSERT(iType > ST_NONE);
  ASSERT(iType < ST_NENUMS);

  return (eST) iType;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
