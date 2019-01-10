/*============================================================================*/
/* Steps_Str.cpp - str step group                                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Str(void)
{
  vAddStep("str::algebra"_Stz, &Step::vStrAlgebra);
  vAddStep("strAlgebra"_Stz,   &Step::vStrAlgebra);
  
  vAddStep("str.cat"_Stz,      &Step::vStrCat);
  vAddStep("str.clip"_Stz,     &Step::vStrClip);
  vAddStep("str.equal"_Stz,    &Step::vStrEqual);
  vAddStep("str.escape"_Stz,   &Step::vStrEscape);
  vAddStep("str.firstCsv"_Stz, &Step::vStrFirstCsv);
  vAddStep("str.format"_Stz,   &Step::vStrFormat);
  vAddStep("str.left"_Stz,     &Step::vStrLeft);
  vAddStep("str.length"_Stz,   &Step::vStrLength);
  vAddStep("str.pad"_Stz,      &Step::vStrPad);
  vAddStep("str.prepend"_Stz,  &Step::vStrPrepend);
  vAddStep("str.random"_Stz,   &Step::vStrRandom);
  vAddStep("str.replace"_Stz,  &Step::vStrReplace);
  vAddStep("str.right"_Stz,    &Step::vStrRight);
  vAddStep("str.sub"_Stz,      &Step::vStrSub);
  vAddStep("str.tailInt"_Stz,  &Step::vStrTailInt);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* str.algebra -- string algebra tests                                        */
/*----------------------------------------------------------------------------*/
void Step::vStrAlgebra(MP_Task mpTask)
{
  ENTRY("vStrAlgebra"_Stz);
 
  // (1) StrVal's cannot be declared as local variables
  //StrVal xStrval;  // uncommenting this line should produce a compile error
  
  // declaring a fixed length string
  s32 xString("A test"_Stz);
  
  GenVar xVariable;
  StrVal* pStrVal = _pStrVal(&xVariable);
  
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.cat -- concat two strings                                              */
/*----------------------------------------------------------------------------*/
void Step::vStrCat(MP_Task mpTask)
{
  ENTRY("vStrCat"_Stz);
  
  s48K sStrR;
  s48K sStrB;
  s48K sStrA;
  
  if ( ! mpTask->m_pDataStack->bPop(sStrB,sStrA) )
    THROW_INVALID_DSTACK("str.cat"_Stz);
  
  sStrR = sStrA;
  
  sStrR.vConcat(sStrB);
  
  mpTask->m_pDataStack->vPushS48K(sStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.clip -- returns left or right n part of a string ( length str -- str ) */
/*----------------------------------------------------------------------------*/
void Step::vStrClip(MP_Task mpTask)
{
  ENTRY("vStrClip"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.clip"_Stz);
  
  // validate length
  i8 iHasLength = sString.iLength();
  
  if ( iLength < 0 || llabs(iLength) >= iHasLength)
    THROW("Invalid length for: str.clip"_Stz);
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.aChar();
  else
    pStart = sString.aChar() + iHasLength - 1 - llabs(iLength);
  
  StrLib::vCopyLen(sSubStr.aChar(),pStart,llabs(iLength));
  
  mpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.equal -- test if two strings are equal                                 */
/*----------------------------------------------------------------------------*/
void Step::vStrEqual(MP_Task mpTask)
{
  ENTRY("vStrEql"_Stz);
  
  GenVar xStringA;
  GenVar xStringB;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(xStringB,xStringA) )
    THROW_INVALID_DSTACK("str.equal"_Stz);
  
  i8 iEqual = StrLib::bEqualTo(_pStrVal(xStringA.pNestedValueSelf())->aChar(),
                               _pStrVal(xStringB.pNestedValueSelf())->aChar(),
                               C_NO_CASE);
  
  mpTask->m_pDataStack->vPushI8(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Step::vStrEscape(MP_Task mpTask)
{
  ENTRY("vStrEscape"_Stz);
  
  s48K xArg;
  s48K xEscaped;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(xArg) )
    THROW_INVALID_DSTACK("str.escape"_Stz);
  
  StrLib::vEscape(xArg.aChar(),xEscaped.aChar());
  
  mpTask->m_pDataStack->vPushS48K(xEscaped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.firstCsv -- get first value from csv string                            */
/*----------------------------------------------------------------------------*/
void Step::vStrFirstCsv(MP_Task mpTask)
{
  ENTRY("vStrFirstCsv"_Stz);
  
  s48K sStr;
  if ( ! mpTask->m_pDataStack->bPop(sStr) )
    THROW_INVALID_DSTACK("str.firstCsv"_Stz);
  
  s48K sFirstValue;
  
  char* aValue = sStr.aChar();
  
  if (! StrLib::bExtractItem(&aValue,sFirstValue.aChar(), ','))
    sFirstValue = ""_Stz;
  
  mpTask->m_pDataStack->vPushS48K(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Step::vStrFormat(MP_Task mpTask)
{
  ENTRY("vStrFormat"_Stz);
  
  s48K sFormat;
  if ( ! mpTask->m_pDataStack->bPop(sFormat) )
    THROW_INVALID_DSTACK("str.format"_Stz);
  
  s48K sString;
  
  mpTask->vSprint(sFormat,sString);
  
  mpTask->m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.left -- returns left n part of a string ( length str -- str )          */
/*----------------------------------------------------------------------------*/
void Step::vStrLeft(MP_Task mpTask)
{
  ENTRY("vClip"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.left"_Stz);
  
  // validate length
  i8 iHasLength = sString.iLength();
  
  if ( iLength < 0)
    THROW("Invalid length for: str.left"_Stz);
  
  char* pStart = sString.aChar();
  
  StrLib::vCopyLen(sSubStr.aChar(),pStart,iLength);
  
  mpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.length -- string length                                                */
/*----------------------------------------------------------------------------*/
void Step::vStrLength(MP_Task mpTask)
{
  ENTRY("vStrLen"_Stz);
  
  i8 iLength;
  s48K sString;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.length"_Stz);
  
  iLength = sString.iLength();
  
  mpTask->m_pDataStack->vPushI8(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.pad -- pad string to given length                                      */
/*----------------------------------------------------------------------------*/
void Step::vStrPad(MP_Task mpTask)
{
  ENTRY("vStringPad"_Stz);
  
  i8 iLength;
  
  GenVar xStringA;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(iLength,xStringA) )
    THROW_INVALID_DSTACK("str.pad"_Stz);
  
  StrLib::vPad(_pStrVal(xStringA.pNestedValueSelf())->aChar(),iLength,' ');
  
  // adjust area size
  _pStrVal(xStringA.pNestedValueSelf())->vAreaSize();
  
  mpTask->m_pDataStack->vPushGenVar(xStringA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.prepend -- prepend top string before second string                     */
/*----------------------------------------------------------------------------*/
void Step::vStrPrepend(MP_Task mpTask)
{
  ENTRY("vStrPrepend"_Stz);
  
  s48K xStrR;
  s48K xStrB;
  s48K xStrA;
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pDataStack->bPop(xValB,xValA) )
    THROW_INVALID_DSTACK("str.prepend"_Stz);
  
  // If received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    s48K xString;
    xValA.vToString(xString);
    xStrA = xString;
  }
  else {
    xStrA = _pStrVal(xValA.pNestedValueSelf())->aStz();                          //! need to simplify this
  }
  
  // If received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    s48K xString;
    xValB.vToString(xString);
    xStrB = xString;
  }
  else {
    xStrB = _pStrVal(xValB.pNestedValueSelf())->aStz();
  }
  
  xStrR = xStrB;
  
  xStrR.vConcat(xStrA);
  
  mpTask->m_pDataStack->vPushS48K(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.random-- generate random string of given length                        */
/*----------------------------------------------------------------------------*/
void Step::vStrRandom(MP_Task mpTask)
{
  ENTRY("vStrRandom"_Stz);
  
  i8 iLength;
  if ( ! mpTask->m_pDataStack->bPop(iLength) )
    THROW_INVALID_DSTACK("str.random"_Stz);
  
  s256 sRnd;
  
  Random::vString(sRnd,iLength);
  
  mpTask->m_pDataStack->vPushS256(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.replace-- replace this with that within string                         */
/*----------------------------------------------------------------------------*/
void Step::vStrReplace(MP_Task mpTask)
{
  ENTRY("vStrReplace"_Stz);
  
  s48K sString;
  s48K sThat;
  s48K sThis;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.replace"_Stz);

  if ( ! mpTask->m_pDataStack->bPop(sThat) )
    THROW_INVALID_DSTACK("str.replace"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(sThis) )
    THROW_INVALID_DSTACK("str.replace"_Stz);
  
  s48K sReplaced;
  
  StrLib::bReplace(sReplaced.aChar(),sString.aChar(),sThis.aChar(),sThat.aChar());
  
  mpTask->m_pDataStack->vPushS48K(sReplaced);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.right -- Returns the right n part of a string ( length str -- str )    */
/*----------------------------------------------------------------------------*/
void Step::vStrRight(MP_Task mpTask)
{
  ENTRY("vStrRight"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.right"_Stz);
  
  // validate length
  i8 iHasLength = sString.iLength();
  
  if ( iLength < 0)
    THROW("Invalid length for: str.right"_Stz);

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.aChar() + iHasLength - iLength;
  
  StrLib::vCopyLen(sSubStr.aChar(),pStart,iLength);
  
  mpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( length start str == substr )                         */
/*----------------------------------------------------------------------------*/
void Step::vStrSub(MP_Task mpTask)
{
  ENTRY("vStrSub"_Stz);
  
  i8 iLength;
  i8 iStart;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString,iStart,iLength) )
    THROW_INVALID_DSTACK("str.sub"_Stz);
  
  i8 iHasLength = sString.iLength();
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    THROW("Invalid start index of %lld for: str.sub"_Stz,iStart);
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    THROW("Invalid length of %lld for: str.sub"_Stz,iLength);
  
  char* pStart = sString.aChar() + iStart;
  
  StrLib::vCopyLen(sSubStr.aChar(),pStart,iLength);
  
  mpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tailInt -- extract (possible) int at end of string                         */
/*----------------------------------------------------------------------------*/
void Step::vStrTailInt(MP_Task mpTask)
{
  ENTRY("vStrTailInt"_Stz);
  
  // get the string
  s48K sString;
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.tailInt"_Stz);
  
  // get last char in string
  char* aLast = StrLib::sLast(sString.aChar());
  
  // return false if not a digit
  if ( ! StrLib::bIsDigit(*aLast) ) {
    mpTask->m_pDataStack->vPushBool(0);
    return;
  }
  
  // move backward to start of digit sequence
  char* aDigit = aLast;
  while (StrLib::bIsDigit(*aDigit))
    aDigit--;
  aDigit++;
  
  // step forward through each digit, computing int
  i8 iInt = 0;
  while (StrLib::bIsDigit(*aDigit))
    iInt = (iInt * 10) + (*aDigit++ - '0');
  
  // return ( int true )
  mpTask->m_pDataStack->vPushI8(iInt);
  mpTask->m_pDataStack->vPushBool(1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
