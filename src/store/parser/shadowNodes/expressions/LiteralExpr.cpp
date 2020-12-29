/*============================================================================*/
/* LiteralExpr.cpp - a literal expression                                     */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_LITERAL_NAME_PREFIX = "ExprLiteral"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* LiteralExpr::pCreate(eET eType, Value xVal)
{
  _static_

  ASSERT(eType > ET_NONE && eType < ET_NENUMS);

  // Create a unique name for this object
  s256 sName;
  ShadowNode::vObjName(C_LITERAL_NAME_PREFIX, sName);

  QueryNode* pExpr = new QueryNode(sName, Expr::szTypeDefinition(eType), xVal);
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void LiteralExpr::vDescribe(QueryNode* pStmt, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pStmt));

  sBuf.vConcat(pStmt->szName());
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
