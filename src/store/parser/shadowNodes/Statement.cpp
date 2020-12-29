/*============================================================================*/
/* Statement.hpp - identifies the type of query being planned                 */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_QUERYTEXT_PROP = "queryText"_Stz;

// Definition paths of the above statement types
static Stz* aST_definition[] {
  /*ST_NONE         */ ""_Stz
  /*ST_CREATETABLE  */ ""_Stz
  /*ST_DROPTABLE    */ ""_Stz
  /*ST_CREATEINDEX  */ ""_Stz
  /*ST_DROPINDEX    */ ""_Stz
  /*ST_CTAS         */ ""_Stz
  /*ST_SELECT       */ "/__root__/definitions/object/planObject/statement/dml/select"_Stz
  /*ST_INSERT       */ ""_Stz
  /*ST_UPDATE       */ ""_Stz
  /*ST_DELETE       */ ""_Stz
  /*ST_INSERTSELECT */ ""_Stz
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
  ASSERT(eStmtType < ST_ENUMS);
  ASSERT(szQueryText);

  // Get the definition of the statement we're creating
  Stz* szDef = aST_definition[eStmtType];

  Value xValue(szQueryText);

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(GraphNav::szLeafType(szDef), sName);

  // Create the object with its queryText property
  QueryNode* pStmt = new QueryNode(sName, szDef);
  QueryNode* pQueryText = new QueryNode(C_QUERYTEXT_PROP, const_cast<Stz*>(C_STRING_OBJ_DEFN_NONNULL), xValue);
  pStmt->vAppendProperty(pQueryText);

  return pStmt;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void Statement::vPrint(QueryNode* pStmt, s256& buf)
{
  _static_
  ASSERT(bValidate(pStmt));

  buf.vConcat(GraphNav::szLeafType(pStmt->szType()));
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
