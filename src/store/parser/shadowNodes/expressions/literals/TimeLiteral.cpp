/*============================================================================*/
/* TimeLiteral.cpp - a time literal expression                                */
/*============================================================================*/

#include "BASE.h"
#include "TimeLiteral.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TIME_LITERAL_NAME_PREFIX = "TimeLiteral"_Stz;
static Stz* C_TIME_LITERAL_DEFN = "/__root__/definitions/object/expr/literal/time"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create expression literal nodes.                        */
/*----------------------------------------------------------------------------*/
QueryNode* TimeLiteral::pCreate(Tme xTme)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_TIME_LITERAL_DEFN, sName);

  Value xVal(xTme);

  QueryNode* pExpr = new QueryNode(sName, C_TIME_LITERAL_DEFN, static_cast<Value&&>(xVal));
  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void TimeLiteral::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIME);
  UNUSED(pRootNode);

  if (pNode->bIsNullValue()) {
    sBuf.vConcat("NULL"_Stz);
  } else {
    Tme xTme = pNode->xTimeVal();
    sBuf.vCatSprint("TIME '%02lld:%02lld:%02lld'"_Stz, xTme.iHour(), xTme.iMin(), xTme.iSec());
  }
}

/*----------------------------------------------------------------------------*/
/* Gets the time of this expression. NB: call bIsNull() to check if the    */
/* value is null first.                                                       */
/*----------------------------------------------------------------------------*/
Tme TimeLiteral::xValue(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->eValueType() == Value::VT_TIME);

  if (pNode->bIsNullValue()) {
    Tme xTme;
    xTme.vSetNull();
    return xTme;

  } else {
    return pNode->xTimeVal();
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 TimeLiteral::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TIME_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TimeLiteral::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TIME_LITERAL_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation                     */
/*----------------------------------------------------------------------------*/
eCT TimeLiteral::eType(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  UNUSED(pExprNode);
  UNUSED(pRootNode);

  return CT_TIME;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given string validly represents a time                 */
/*----------------------------------------------------------------------------*/
b1 TimeLiteral::bIsTimeStr(Stz* szStr)
{
  b1 bResult;
  xParseStr(szStr, bResult);

  return bResult;
}

/*----------------------------------------------------------------------------*/
/* Parse a string into a Tme, setting bResult to true if successful           */
/*----------------------------------------------------------------------------*/
Tme TimeLiteral::xParseStr(Stz* szStr, b1& bResult)
{
  // Must begin with the form 'HH:MM:SS'

  Tme xTme;
  xTme.vSetNull();

  if (szStr->nLength() != 8) {
    bResult = false;
    return xTme;
  }

  char* pcBuf = szStr->azChar();

  // Validate the format
  if (pcBuf[2] != ':' || pcBuf[5] != ':') {
    bResult = false;
    return xTme;
  }

  if (!StrLib::bIsDigit(pcBuf[0]) ||
      !StrLib::bIsDigit(pcBuf[1]) ||
      !StrLib::bIsDigit(pcBuf[3]) ||
      !StrLib::bIsDigit(pcBuf[4]) ||
      !StrLib::bIsDigit(pcBuf[6]) ||
      !StrLib::bIsDigit(pcBuf[7]) ) {
    bResult = false;
    return xTme;
  }

  // Parse out the fields
  i8 iHour = 0;
  i8 iMin  = 0;
  i8 iSec  = 0;

  iHour *= 10; iHour += StrLib::iDigitToI8(pcBuf[0]);
  iHour *= 10; iHour += StrLib::iDigitToI8(pcBuf[1]);

  iMin *= 10; iMin += StrLib::iDigitToI8(pcBuf[3]);
  iMin *= 10; iMin += StrLib::iDigitToI8(pcBuf[4]);

  iSec *= 10; iSec += StrLib::iDigitToI8(pcBuf[6]);
  iSec *= 10; iSec += StrLib::iDigitToI8(pcBuf[7]);

  // Now check the values
  s256 sErrorMsg;
  if (!Tme::bIsValid(iHour, iMin, iSec, sErrorMsg)) {
    bResult = false;
    return xTme;
  }

  xTme.vSet(iHour, iMin, iSec, 0);
  bResult = true;
  return xTme;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
