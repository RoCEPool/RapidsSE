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
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* DecimalLiteral::pCreateNull()
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DECIMAL_LITERAL_DEFN, sName);

  Value xVal{};
  xVal.vSetNull(Value::VT_DECIMAL);

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
TypeId DecimalLiteral::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return TypeId::xIdOf(CT_DECIMAL);
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 DecimalLiteral::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  Bcd xBcd = xValue(pExprNode);
  if (xBcd.bIsNull()) {
    return -1;
  }

  // The precision is the number of digits in the mantissa.
  i8 iPrecision = IntLib::iLog10(IntLib::iAbs(xBcd.T.m_iMantissa)) + 1;
  return iPrecision;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 DecimalLiteral::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  Bcd xBcd = xValue(pExprNode);
  if (xBcd.bIsNull()) {
    return -1;
  }

  // This is easy, each BCD value must have a known scale.
  return (i8) xBcd.T.m_uScale;
}

/*----------------------------------------------------------------------------*/
/* Parse a string into a Bcd, throwing an exception is there is a problem     */
/*----------------------------------------------------------------------------*/
Bcd DecimalLiteral::xParseStr(Stz* szStr)
{
  ASSERT(szStr);

  i8 nLen = szStr->nLength();

  if (nLen == 0) {
    SemanticException::vThrow(EC_INVALID_VALUE,
                              ": '%s': Unable to convert string literal to decimal literal"_Stz,
                              szStr);
  }

  i8 iSign = (szStr->azChar()[0] == '-' || szStr->azChar()[0] == '+') ? 1 : 0;

  i8 iPointPos = StrLib::iFind(szStr->azChar(), '.');

  if (iPointPos == -1) {
    // We've got an integer that we are parsing into a decimal.
    // This can come about through implicit coercion. e.g. we are
    // coercing the string '2' to a decimal.
    bool bOverflow = false;
    bool bInvalid = false;
    i8 iMantissa = StrLib::iToI8(szStr->azChar(), &bInvalid, &bOverflow);

    if (bInvalid) {
      SemanticException::vThrow(EC_INVALID_VALUE,
                                ": '%s': Unable to convert string literal to decimal mantissa"_Stz,
                                szStr);
    }

    if (bOverflow || iMantissa > C_BCD_MAX || iMantissa < C_BCD_MIN) {
      SemanticException::vThrow(
          EC_VALUE_OUT_OF_RANGE,
          ": %s: Max/min decimal precision supported is %lld to %lld"_Stz,
          szStr,
          C_BCD_MIN,
          C_BCD_MAX);
    }

    Bcd xBcd;
    xBcd.vSet(iMantissa, 0);
    return xBcd;
  }

  ASSERT(iPointPos >= 0);  // decimal literals must always contain one decimal point

  i8 iScale = nLen - iPointPos - 1;
  i8 iPrecision = nLen - iSign - 1;

  i8 iMantissa;
  char* pcCrs = szStr->azChar();
  bool bResult = StrLib::bExtractDecimal(&pcCrs, iMantissa, iPrecision + iScale, iScale);

  if (!bResult) {
    SemanticException::vThrow(EC_INVALID_VALUE,
                              ": %s: Unable to convert string literal to decimal literal "_Stz,
                              szStr);
  }

  // Check for overflows. This is dependent on the size of components of the Bcd_T structure
  if (iScale > C_MAX_BCD_SCALE || iScale < C_MIN_BCD_SCALE) {
    SemanticException::vThrow(EC_VALUE_OUT_OF_RANGE,
                              ": %s: Max/min decimal scale supported is %lld to %lld"_Stz,
                              szStr,
                              C_MIN_BCD_SCALE,
                              C_MAX_BCD_SCALE);
  }

  if (iMantissa > C_BCD_MAX || iMantissa < C_BCD_MIN) {
    // iDecimal > (2^47 - 1) || iDecimal < (-2^47)
    SemanticException::vThrow(
                             EC_VALUE_OUT_OF_RANGE,
                             ": %s: Max/min decimal precision supported is %lld to %lld"_Stz,
                             szStr,
                             C_BCD_MIN,
                             C_BCD_MAX);
  }

  ASSERT(sizeof(Bcd) == C_BYTES_PER_WORD);

  Bcd xBcd;

  // REVISE: If the value is small enough then represent it with 2 decimal places
  // so it prints nicely. It should strictly have a scale of 0.
  if ( iScale == 0 &&
       (iMantissa * 100) < C_BCD_MAX &&
       (iMantissa * 100) > C_BCD_MIN ) {
    xBcd.vSet(iMantissa * 100, 2);

  } else {
    xBcd.vSet(iMantissa, iScale);
  }

  return xBcd;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
