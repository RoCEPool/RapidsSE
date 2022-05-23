/*============================================================================*/
/* TimestampLiteral.cpp - a timestamp literal expression                                */
/*============================================================================*/

#include "BASE.h"
#include "TimestampLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TIMESTAMP_LITERAL_NAME_PREFIX = "TimestampLiteral"_Stz;
static Stz* C_TIMESTAMP_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/timestamp"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* TimestampLiteral::pCreate(Tsp xTsp)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_TIMESTAMP_LITERAL_DEFN, sName);

  Value xVal(xTsp);

  QueryNode* pExpr = new QueryNode(sName, C_TIMESTAMP_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TimestampLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIMESTAMP);
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    Tsp xTsp = pNode->xTspVal();

    if (bHasMicros(pNode)) {
      sBuf.vCatSprint("TIMESTAMP '%04lld-%02lld-%02lld %02lld:%02lld:%02lld.%06lld'"_Stz,
                      xTsp.iYear(), xTsp.iMonth(), xTsp.iDay(), xTsp.iHour(), xTsp.iMin(), xTsp.iSec(), xTsp.iMicro());
    } else {
      sBuf.vCatSprint("TIMESTAMP '%04lld-%02lld-%02lld %02lld:%02lld:%02lld'"_Stz,
                      xTsp.iYear(), xTsp.iMonth(), xTsp.iDay(), xTsp.iHour(), xTsp.iMin(), xTsp.iSec());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the timestamp of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
Tsp TimestampLiteral::xValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIMESTAMP);

  if (pNode->bIsNullValue()) {
    Tsp xTsp;
    xTsp.vSetNull();
    return xTsp;

  } else {
    return pNode->xTspVal();
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the timestamp of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
b1 TimestampLiteral::bHasMicros(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIMESTAMP);

  if (pNode->bIsNullValue()) {
    return false;

  } else {
    return pNode->xTspVal().iMicro() != 0;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 TimestampLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TIMESTAMP_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TimestampLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TIMESTAMP_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
TypeId TimestampLiteral::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return TypeId::xIdOf(CT_TIMESTAMP);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given string validly represents a timestamp            */
/*----------------------------------------------------------------------------*/
b1 TimestampLiteral::bIsTimestampStr(Stz* szStr)
{
  b1 bResult;
  xParseStr(szStr, bResult);

  return bResult;
}


/*----------------------------------------------------------------------------*/
/* Parse a string into a Tsp, setting bResult to true if successful           */
/*----------------------------------------------------------------------------*/
Tsp TimestampLiteral::xParseStr(Stz* szStr, b1& bResult)
{
  // Must begin with the form 'YYYY-MM-DD'.
  // That way we can also parse timestamps as dates too.

  Tsp xTsp;
  xTsp.vSetNull();

  i8 iLen = szStr->nLength();
  if (iLen < 19) {
    bResult = false;
    return xTsp;
  }

  char* pcBuf = szStr->azChar();
  if (pcBuf[4] != '-' || pcBuf[7] != '-' || pcBuf[10] != ' ' || pcBuf[13] != ':' || pcBuf[16] != ':') {
    bResult = false;
    return xTsp;
  }

  if (!StrLib::bIsDigit(pcBuf[0])  ||
      !StrLib::bIsDigit(pcBuf[1])  ||
      !StrLib::bIsDigit(pcBuf[2])  ||
      !StrLib::bIsDigit(pcBuf[3])  ||
      !StrLib::bIsDigit(pcBuf[5])  ||
      !StrLib::bIsDigit(pcBuf[6])  ||
      !StrLib::bIsDigit(pcBuf[8])  ||
      !StrLib::bIsDigit(pcBuf[9])  ||
      !StrLib::bIsDigit(pcBuf[11]) ||
      !StrLib::bIsDigit(pcBuf[12]) ||
      !StrLib::bIsDigit(pcBuf[14]) ||
      !StrLib::bIsDigit(pcBuf[15]) ||
      !StrLib::bIsDigit(pcBuf[17]) ||
      !StrLib::bIsDigit(pcBuf[18]) ) {
    bResult = false;
    return xTsp;
  }

  // check for optional micros
  b1 bHasMicros = false;

  if (iLen > 19) {
    if (pcBuf[19] != '.') {
      bResult = false;
      return xTsp;
    }

    for (i8 iIdx = 20; iIdx < iLen; iIdx++) {
      if (iIdx > 25) {
        // can only have 6 digits of microseconds
        bResult = false;
        return xTsp;
      }

      if (!StrLib::bIsDigit(pcBuf[iIdx])) {
        bResult = false;
        return xTsp;
      }
    }

    bHasMicros = true;
  }

  // Parse out the fields
  i8 iYear  = 0;
  i8 iMonth = 0;
  i8 iDay   = 0;

  i8 iHour = 0;
  i8 iMin  = 0;
  i8 iSec  = 0;
  i8 iMicro = 0;

  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[0]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[1]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[2]);
  iYear *= 10; iYear += StrLib::iDigitToI8(pcBuf[3]);

  iMonth *= 10; iMonth += StrLib::iDigitToI8(pcBuf[5]);
  iMonth *= 10; iMonth += StrLib::iDigitToI8(pcBuf[6]);

  iDay *= 10; iDay += StrLib::iDigitToI8(pcBuf[8]);
  iDay *= 10; iDay += StrLib::iDigitToI8(pcBuf[9]);

  iHour *= 10; iHour += StrLib::iDigitToI8(pcBuf[11]);
  iHour *= 10; iHour += StrLib::iDigitToI8(pcBuf[12]);

  iMin *= 10; iMin += StrLib::iDigitToI8(pcBuf[14]);
  iMin *= 10; iMin += StrLib::iDigitToI8(pcBuf[15]);

  iSec *= 10; iSec += StrLib::iDigitToI8(pcBuf[17]);
  iSec *= 10; iSec += StrLib::iDigitToI8(pcBuf[18]);

  if (bHasMicros) {
    for (i8 iIdx = 20; iIdx < 26; iIdx++) {
      iMicro *= 10;
      if (iIdx < iLen) {
        iMicro += StrLib::iDigitToI8(pcBuf[iIdx]);
      }
    }
  }

  s256 sErrorMsg;
  if (!Tsp::bIsValid(iYear, iMonth, iDay, iHour, iMin, iSec, iMicro, sErrorMsg)) {
    bResult = false;
    return xTsp;
  }

  bResult = true;
  xTsp.vSet(iYear, iMonth, iDay, iHour, iMin, iSec, iMicro);
  return xTsp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
