/*============================================================================*/
/* DecimalLiteral.cpp - an decimal literal expression                         */
/*============================================================================*/

#include "BASE.h"
#include "DecimalLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"
#include "SqlException.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DECIMAL_LITERAL_NAME_PREFIX = "DecimalLiteral"_Stz;
static Stz* C_DECIMAL_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/decimal"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* DecimalLiteral::pCreate(Bcd xBcd)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DECIMAL_LITERAL_DEFN, sName);

  Value xVal(xBcd);

  QueryNode* pExpr = new QueryNode(sName, C_DECIMAL_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void DecimalLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_DECIMAL);
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    s2K sBcdBuf;
    pNode->xValue().vDescribe(sBcdBuf);
    sBuf.vConcat(sBcdBuf.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the decimal of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
Bcd DecimalLiteral::xValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_DECIMAL);

  if (pNode->bIsNullValue()) {
    Bcd xBcd;
    xBcd.vSetNull();
    return xBcd;

  } else {
    return pNode->xDecimalVal();
  }
}

/*----------------------------------------------------------------------------*/
/* Sets the decimal of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
void DecimalLiteral::vSetValue(PlanNode* pNode, Bcd xVal)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_DECIMAL);
  pNode->pValue()->vSet(xVal);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 DecimalLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DECIMAL_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 DecimalLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DECIMAL_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
i8 DecimalLiteral::iType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return eCT::CT_DECIMAL;
}

/*----------------------------------------------------------------------------*/
/* Parse a string into a Bcd, setting bResult to true if successful           */
/*----------------------------------------------------------------------------*/
Bcd DecimalLiteral::xParseStr(Stz* szStr, b1& bResult)
{
  ASSERT(szStr);

  i8 nLen = szStr->nLength();
  ASSERT(szStr->nLength() > 0);

  i8 iSign = (szStr->azChar()[0] == '-' || szStr->azChar()[0] == '+') ? 1 : 0;

  i8 iPointPos = StrLib::iFind(szStr->azChar(), '.');

  if (iPointPos == -1) {
    // We've got an integer that we are parsing into a decimal.
    // This can come about through implicit coercion. e.g. we are
    // coercing the string '2' to a decimal.
    i8 iMantissa = StrLib::iToI8(szStr->azChar());
    Bcd xBcd;
    xBcd.vSet(iMantissa, 0);
    bResult = true;
    return xBcd;
  }

  ASSERT(iPointPos >= 0);  // decimal literals must always contain one decimal point

  i8 iScale = nLen - iPointPos - iSign - 1;
  i8 iPrecision = nLen - iSign - 1;

  i8 iMantissa;
  char* pcCrs = szStr->azChar();
  bResult = StrLib::bExtractDecimal(&pcCrs, iMantissa, iPrecision + iScale, iScale);

  if (!bResult) {
    SemanticException::vThrow(EC_INVALID_VALUE,
                              "%s: Unable to convert string literal to decimal literal "_Stz);
  }

  // Check for overflows. This is dependent on the size of components of the Bcd_T structure
  if (iScale > 127 || iScale < -128) {
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE,
                              "%s: Max/min decimal scale supported is [-128, 127]"_Stz);
  }

  if (iMantissa > 140737488355327 || iMantissa < -140737488355328) {
    // iDecimal > (2^47 - 1) || iDecimal < (-2^47)
    SemanticException::vThrow(
                             EC_VALUE_OUT_OF_RANGE,
                             "%s: Max/min decimal precision supported is [2^47, (2^47) - 1]"_Stz);
  }

  ASSERT(sizeof(Bcd) == C_BYTES_PER_WORD);

  Bcd xBcd;
  xBcd.vSet(iMantissa, iScale);
  return xBcd;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
