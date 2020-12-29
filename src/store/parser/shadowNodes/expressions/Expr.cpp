/*============================================================================*/
/* Expr.cpp - a generic expression                                            */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"


/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_EXPR_DEFN = "/__root__/definitions/object/expr"_Stz;

static Stz* aTypeDefinitions[] = {
  /* 00 ET_NONE      */ "<INVALID>"_Stz,
  /* 01 ET_NULL      */ "/__root__/definitions/object/expr/literal/null"_Stz,
  /* 02 ET_BOOLEAN   */ "/__root__/definitions/object/expr/literal/boolean"_Stz,
  /* 03 ET_STRING    */ "/__root__/definitions/object/expr/literal/string"_Stz,
  /* 04 ET_INTEGER   */ "/__root__/definitions/object/expr/literal/integer"_Stz,
  /* 05 ET_FLOAT     */ "/__root__/definitions/object/expr/literal/float"_Stz,
  /* 06 ET_DECIMAL   */ "/__root__/definitions/object/expr/literal/decimal"_Stz,
  /* 07 ET_DATE      */ "/__root__/definitions/object/expr/literal/date"_Stz,
  /* 08 ET_TIME      */ "/__root__/definitions/object/expr/literal/time"_Stz,
  /* 09 ET_TIMESTAMP */ "/__root__/definitions/object/expr/literal/timestamp"_Stz,
  /* 10 ET_INTERVAL  */ "/__root__/definitions/object/expr/literal/interval"_Stz,
  /* 11 ET_NENUMS    */ "<INVALID>"_Stz,
};

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a description of the statement into a buffer                       */
/*----------------------------------------------------------------------------*/
void Expr::vDescribe(QueryNode* pStmt, s2K& sBuf)
{
  _static_
  ASSERT(bValidate(pStmt));

  // TODO: manual polymorphism here

  sBuf.vConcat(pStmt->szName());
}

/*----------------------------------------------------------------------------*/
/* Return the definition path for a given expression type                     */
/*----------------------------------------------------------------------------*/
Stz* Expr::szTypeDefinition(eET eExprType)
{
  ASSERT(eExprType > ET_NONE);
  ASSERT(eExprType < ET_NENUMS);

  return aTypeDefinitions[eExprType];
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
