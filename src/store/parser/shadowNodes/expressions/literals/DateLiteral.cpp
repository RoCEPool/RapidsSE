/*============================================================================*/
/* DateLiteral.cpp - an date literal expression                               */
/*============================================================================*/

#include "BASE.h"
#include "DateLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DATE_LITERAL_NAME_PREFIX = "DateLiteral"_Stz;
static Stz* C_DATE_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/date"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* DateLiteral::pCreate(Dte xDte)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_DATE_LITERAL_DEFN, sName);

  Value xVal(xDte);

  QueryNode* pExpr = new QueryNode(sName, C_DATE_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void DateLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_DATE);
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    Dte xDte = pNode->xDateVal();
    sBuf.vCatSprint("DATE '%04lld-%02lld-%02lld'"_Stz, xDte.iYear(), xDte.iMonth(), xDte.iDay());
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the date of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
Dte DateLiteral::xValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_DATE);

  if (pNode->bIsNullValue()) {
    Dte xDte;
    xDte.vSetNull();
    return xDte;

  } else {
    return pNode->xDateVal();
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 DateLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DATE_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 DateLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DATE_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT DateLiteral::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return CT_DATE;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given string validly represents a date                 */
/*----------------------------------------------------------------------------*/
b1 DateLiteral::bIsDateStr(Stz* szStr)
{
  b1 bResult;
  xParseStr(szStr, bResult);

  return bResult;
}


/*----------------------------------------------------------------------------*/
/* Parse a string into a Dte, setting bResult to true if successful           */
/*----------------------------------------------------------------------------*/
Dte DateLiteral::xParseStr(Stz* szStr, b1& bResult)
{
  // Must begin with the form 'YYYY-MM-DD'.
  // That way we can also parse timestamps as dates too.

  Dte xDte;
  xDte.vSetNull();

  i8 nLen = szStr->nLength();
  if (nLen != 10) {
    bResult = false;
    return xDte;
  }

  char* pcBuf = szStr->azChar();
  if (pcBuf[4] != '-' || pcBuf[7] != '-') {
    bResult = false;
    return xDte;
  }

  if (!StrLib::bIsDigit(pcBuf[0]) ||
      !StrLib::bIsDigit(pcBuf[1]) ||
      !StrLib::bIsDigit(pcBuf[2]) ||
      !StrLib::bIsDigit(pcBuf[3]) ||
      !StrLib::bIsDigit(pcBuf[5]) ||
      !StrLib::bIsDigit(pcBuf[6]) ||
      !StrLib::bIsDigit(pcBuf[8]) ||
      !StrLib::bIsDigit(pcBuf[9]) ) {
    bResult = false;
    return xDte;
  }

  // Parse out the fields
  i8 iYear  = 0;
  i8 iMonth = 0;
  i8 iDay   = 0;

  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[0]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[1]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[2]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[3]);

  iMonth *= 10; iMonth += StrLib::iDigitToI8(pcBuf[5]);
  iMonth *= 10; iMonth += StrLib::iDigitToI8(pcBuf[6]);

  iDay *= 10; iDay += StrLib::iDigitToI8(pcBuf[8]);
  iDay *= 10; iDay += StrLib::iDigitToI8(pcBuf[9]);

  s256 sErrorMsg;
  if (!Dte::bIsValid(iYear, iMonth, iDay, sErrorMsg)) {
    bResult = false;
    return xDte;
  }

  bResult = true;
  xDte.vSet(iYear, iMonth, iDay);
  return xDte;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
