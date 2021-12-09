/*============================================================================*/
/* ExtractExpr.hpp - an EXTRACT expression operator                           */
/*============================================================================*/

#include "BASE.h"
#include "ExtractExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "eCT.h"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "ExtractExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/mathematical/extract"_Stz;

static Stz* C_EXTRACT_FIELD_PROP = "field"_Stz;

// What is the resulting type when we combine these inputs?
// * The left header column represents the type of the first operand (extract field).
// * The top header row represents the type of the second operand (source expression).
// * The corresponding cell in the matrix represents the type of the resultant from this operation.
// * Invalid/unsupported combinations are represented with CT_NONE.
// * Fields marked with /*?*/ are not currently supported but could be.  Refer to
//   the documentation below marked with this symbol for more information.
// * Fields marked with /*X*/ are not currently supported but should be.
static eCT aeTypeCalculus[ExtractField::EF_NENUMS][CT_ENUMS] = {

                 //   NONE     BOOLEAN  CHAR     DATE          DECIMAL     FLOAT       INTEGER       INTERVAL_DT     INTERVAL_YM     TIME          TIMESTAMP     VARCHAR  UNKNOWN
/*  0 EF_NONE    */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,        CT_NONE,        CT_NONE,      CT_NONE,      CT_NONE, CT_NONE    },
/*  1 EF_YEAR    */ { CT_NONE, CT_NONE, CT_NONE, CT_INTEGER,   CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,        CT_INTEGER,     CT_NONE,      CT_INTEGER,   CT_NONE, CT_UNKNOWN },
/*  2 EF_MONTH   */ { CT_NONE, CT_NONE, CT_NONE, CT_INTEGER,   CT_NONE,    CT_NONE,    CT_NONE,      CT_NONE,        CT_INTEGER,     CT_NONE,      CT_INTEGER,   CT_NONE, CT_UNKNOWN },
/*  3 EF_DAY     */ { CT_NONE, CT_NONE, CT_NONE, CT_INTEGER,   CT_NONE,    CT_NONE,    CT_NONE,      CT_INTEGER,     CT_DATE,        CT_NONE,      CT_INTEGER,   CT_NONE, CT_UNKNOWN },
/*  4 EF_HOUR    */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_INTEGER,     CT_NONE,        CT_INTEGER,   CT_INTEGER,   CT_NONE, CT_UNKNOWN },
/*  5 EF_MINUTE  */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_INTEGER,     CT_NONE,        CT_INTEGER,   CT_INTEGER,   CT_NONE, CT_UNKNOWN },
/*  6 EF_SECOND  */ { CT_NONE, CT_NONE, CT_NONE, CT_NONE,      CT_NONE,    CT_NONE,    CT_NONE,      CT_INTEGER,     CT_NONE,        CT_INTEGER,   CT_DECIMAL,   CT_NONE, CT_UNKNOWN },
};

// References:
// Extracting year-min returns an integer.
// Extracting a second returns a decimal with a scale not less than the precision of the fractional seconds field, if applicable.
// Ref 6.27 (<numeric value function>), Syntax Rules #7
//
// In the case of extracting a SECOND from a TIME or INTERVAL_DT value I think it makes sense to
// return an INTEGER since a TIME or INTERVAL_DT value cannot contain fractional parts,
// whereas a TIMESTAMP can.

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* ExtractExpr::pCreate(QueryNode* pExtractField, QueryNode* pExpr)
{
  _static_
  ASSERT(pExtractField);
  ASSERT(ExtractField::bIsOfType(pExtractField));
  ASSERT(pExpr);
  ASSERT(Expr::bIsOfType(pExpr));

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DEFN, sName);

  QueryNode* pExtractNode = new QueryNode(sName, C_DEFN);

  pExtractField->vSetName(C_EXTRACT_FIELD_PROP);
  pExtractNode->vAppendProperty(pExtractField);

  pExtractNode->vAppendChild(pExpr);

  return pExtractNode;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void ExtractExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  ExtractField::eEF eExtractField = ExtractExpr::eExtractField(_pQueryNode(pNode));
  Stz* szExtractField = ExtractField::szExtractFieldType(eExtractField);

  QueryNode* pExpr = ExtractExpr::pSourceExpr(_pQueryNode(pNode));

  sBuf.vCatSprint("EXTRACT (%s FROM "_Stz, szExtractField);

  s2K sExprBuf;
  if (pExpr->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pExpr)) {
    Expr::vDescribe(pExpr, sExprBuf, pRootNode);

  } else {
    sExprBuf.vConcat("("_Stz);
    Expr::vDescribe(pExpr, sExprBuf, pRootNode);
    sExprBuf.vConcat(")"_Stz);
  }

  sBuf.vCatSprint("%s)"_Stz, sExprBuf.szStz());
}

/*----------------------------------------------------------------------------*/
/* The enum field to be extracted                                             */
/*----------------------------------------------------------------------------*/
ExtractField::eEF ExtractExpr::eExtractField(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pExtractFieldNode = _pQueryNode(GraphNav::pFindProperty(C_EXTRACT_FIELD_PROP, pNode));
  ASSERT(pExtractFieldNode);
  ASSERT(ExtractField::bIsOfType(pExtractFieldNode));

  return ExtractField::eExtractFieldType(pExtractFieldNode);
}

/*----------------------------------------------------------------------------*/
/* Return the expression that we are extracting the field from                */
/*----------------------------------------------------------------------------*/
QueryNode* ExtractExpr::pSourceExpr(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 1);

  return _pQueryNode(pNode->pFirstChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 ExtractExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExtractExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
eCT ExtractExpr::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pExpr = ExtractExpr::pSourceExpr(pExprNode);

  ExtractField::eEF eOperand1 = eExtractField(pExprNode);
  eCT               eOperand2 = Expr::eType(pExpr, pRootNode);

  // Lookup the type of the result
  eCT eResult = aeTypeCalculus[eOperand1][eOperand2];

  return eResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
