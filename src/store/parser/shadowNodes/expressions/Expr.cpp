/*============================================================================*/
/* Expr.cpp - a generic expression                                            */
/*============================================================================*/

#include "BASE.h"
#include "Expr.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "store/parser/shadowNodes/expressions/literals/LiteralExpr.hpp"
#include "store/parser/shadowNodes/expressions/operators/aggregates/AggregateExpr.hpp"
#include "ColumnRef.hpp"
#include "OperatorExpr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_CATALOG_PROP  = "catalogName"_Stz;
static Stz* C_SCHEMA_PROP   = "schemaName"_Stz;
static Stz* C_TABLE_PROP    = "tableName"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_EXPR_DEFN = "/__root__/definitions/object/expr"_Stz;

// The path of the definition within the graph for each SQL expression type.
static Stz* aExprTypeDefns[] = {
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
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Expr::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // TODO: manual expression polymorphism here

  // Literals
  if      (LiteralExpr::bIsOfType(pNode))   { LiteralExpr::vDescribe(pNode, sBuf); }

  // ColumnRefs
  else if (ColumnRef::bIsOfType(pNode))     { ColumnRef::vDescribe(pNode, sBuf);   }

  // Operators of all kinds
  else if (OperatorExpr::bIsOfType(pNode))  { OperatorExpr::vDescribe(pNode, sBuf);   }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Return the definition path for a given expression type                     */
/*----------------------------------------------------------------------------*/
Stz* Expr::szTypeDefinition(eET eExprType)
{
  ASSERT(eExprType > ET_NONE);
  ASSERT(eExprType < ET_NENUMS);

  return aExprTypeDefns[eExprType];
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_EXPR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if this expression (and therefore any subexpression elements  */
/* it contains) is an aggregate expression                                    */
/*----------------------------------------------------------------------------*/
b1 Expr::bIsAnAggregate(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  b1 bIsAgg = false;

  // Walk the graph, looking at the children only and return if any of them
  // are aggregate expressions.
  auto fVisitor = [&bIsAgg](PlanNode* pNode, s256& sPath, i8 iDepth, void* pvArg) -> GraphNav::eWalkCtrl {
    if (AggregateExpr::bIsOfType(pNode)) {
      bIsAgg = true;
      return GraphNav::WALK_ABORT;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bWalk(pNode, false, true, fVisitor, nullptr);

  return bIsAgg;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
