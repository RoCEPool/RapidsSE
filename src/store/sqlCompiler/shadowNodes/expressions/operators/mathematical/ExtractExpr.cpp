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
#include "TypeSystem.hpp"
#include "ExtractMap.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_NAME_PREFIX = "ExtractExpr"_Stz;
static Stz* C_DEFN = "/__root__/definitions/object/expr/operator/mathematical/extract"_Stz;

static Stz* C_EXTRACT_FIELD_PROP = "field"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand (extract field).
//  - The second parameter represents the type of the second operand (source expression).
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static ExtractMap* S_pTypeMap = (new ExtractMap())

    ->pAddMapping(ExtractField::EF_YEAR,    TypeId::xIdOf(CT_DATE),            TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_YEAR,    TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_YEAR,    TypeId::xIdOf(IV_YEAR),            TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_YEAR,    TypeId::xIdOf(IV_YEAR_TO_MONTH),   TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_YEAR,    TypeId::xIdOf(CT_UNKNOWN),         TypeId::xIdOf(CT_UNKNOWN))

    ->pAddMapping(ExtractField::EF_MONTH,   TypeId::xIdOf(CT_DATE),            TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MONTH,   TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MONTH,   TypeId::xIdOf(IV_MONTH),           TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MONTH,   TypeId::xIdOf(IV_YEAR_TO_MONTH),   TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MONTH,   TypeId::xIdOf(CT_UNKNOWN),         TypeId::xIdOf(CT_UNKNOWN))

    ->pAddMapping(ExtractField::EF_DAY,     TypeId::xIdOf(CT_DATE),            TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_DAY,     TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_DAY,     TypeId::xIdOf(IV_DAY),             TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_DAY,     TypeId::xIdOf(IV_DAY_TO_HOUR),     TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_DAY,     TypeId::xIdOf(IV_DAY_TO_MINUTE),   TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_DAY,     TypeId::xIdOf(IV_DAY_TO_SECOND),   TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_DAY,     TypeId::xIdOf(CT_UNKNOWN),         TypeId::xIdOf(CT_UNKNOWN))

    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(CT_TIME),            TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(IV_HOUR),            TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(IV_DAY_TO_HOUR),     TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(IV_DAY_TO_MINUTE),   TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(IV_DAY_TO_SECOND),   TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(IV_HOUR_TO_MINUTE),  TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(IV_HOUR_TO_SECOND),  TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_HOUR,    TypeId::xIdOf(CT_UNKNOWN),         TypeId::xIdOf(CT_UNKNOWN))

    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(CT_TIME),            TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(IV_MINUTE),          TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(IV_DAY_TO_MINUTE),   TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(IV_DAY_TO_SECOND),   TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(IV_HOUR_TO_MINUTE),  TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(IV_HOUR_TO_SECOND),  TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(IV_MINUTE_TO_SECOND),TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(ExtractField::EF_MINUTE,  TypeId::xIdOf(CT_UNKNOWN),         TypeId::xIdOf(CT_UNKNOWN))

    ->pAddMapping(ExtractField::EF_SECOND,  TypeId::xIdOf(CT_TIME),            TypeId::xIdOf(CT_INTEGER)) // NOTE: Integer here
    ->pAddMapping(ExtractField::EF_SECOND,  TypeId::xIdOf(CT_TIMESTAMP),       TypeId::xIdOf(CT_DECIMAL)) // NOTE: Decimal here and below
    ->pAddMapping(ExtractField::EF_SECOND,  TypeId::xIdOf(IV_SECOND),          TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(ExtractField::EF_SECOND,  TypeId::xIdOf(IV_DAY_TO_SECOND),   TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(ExtractField::EF_SECOND,  TypeId::xIdOf(IV_HOUR_TO_SECOND),  TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(ExtractField::EF_SECOND,  TypeId::xIdOf(IV_MINUTE_TO_SECOND),TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(ExtractField::EF_SECOND,  TypeId::xIdOf(CT_UNKNOWN),         TypeId::xIdOf(CT_UNKNOWN))
;

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
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void ExtractExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  ExtractField::eEF eExtractField = ExtractExpr::eExtractField(_pQueryNode(pNode));
  Stz* szExtractField = ExtractField::szExtractFieldType(eExtractField);

  QueryNode* pExpr = ExtractExpr::pSourceExpr(_pQueryNode(pNode));

  sBuf.vCatSprint("EXTRACT (%s FROM %s)"_Stz,
                  szExtractField,
                  Expr::xTypeId(pExpr, pRootNode).szTypeName());
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
TypeId ExtractExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pExpr = ExtractExpr::pSourceExpr(pExprNode);

  ExtractField::eEF eOperand1 = eExtractField(pExprNode);
  TypeId            xOperand2 = Expr::xTypeId(pExpr, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(eOperand1, xOperand2);
  return xResult;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
