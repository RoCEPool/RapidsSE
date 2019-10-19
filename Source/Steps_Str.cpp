/*============================================================================*/
/* Steps_Str.cpp - str step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Str(void)
{
  vAddStep("str::algebra"_Stz,     &Step::vStrAlgebra);
  vAddStep("strAlgebra"_Stz,       &Step::vStrAlgebra);
  
  vAddStep("str.cat"_Stz,          &Step::vStrCat);
  vAddStep("str.clip"_Stz,         &Step::vStrClip);
  vAddStep("str.equal"_Stz,        &Step::vStrEqual);
  vAddStep("str.escape"_Stz,       &Step::vStrEscape);
  vAddStep("str.firstCsv"_Stz,     &Step::vStrFirstCsv);
  vAddStep("str.format"_Stz,       &Step::vStrFormat);
  vAddStep("str.left"_Stz,         &Step::vStrLeft);
  vAddStep("str.length"_Stz,       &Step::vStrLength);
  vAddStep("str.prepend"_Stz,      &Step::vStrPrepend);
  vAddStep("str.random"_Stz,       &Step::vStrRandom);
  vAddStep("str.removePrefix"_Stz, &Step::vStrRemovePrefix);
  vAddStep("str.removeSuffix"_Stz, &Step::vStrRemoveSuffix);
  vAddStep("str.replace"_Stz,      &Step::vStrReplace);
  vAddStep("str.right"_Stz,        &Step::vStrRight);
  vAddStep("str.sub"_Stz,          &Step::vStrSub);
  vAddStep("str.tailInt"_Stz,      &Step::vStrTailInt);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* str.algebra -- string algebra tests                                        */
/*----------------------------------------------------------------------------*/
void Step::vStrAlgebra(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrAlgebra"_Stz);
 
  // (1) StrVal's cannot be declared as local variables
  //StrVal xStrval;  // uncommenting this line should produce a compile error
  
  // declaring a fixed length string
  s32 xString("A test"_Stz);
  
  GenVar xVariable;
  StrVal* aStrVal = _aStrVal(&xVariable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.cat -- concat two strings                                              */
/*----------------------------------------------------------------------------*/
void Step::vStrCat(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrCat"_Stz);
  
  s48K sStrR;
  s48K sStrB;
  s48K sStrA;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sStrB,sStrA) )
    THROW_DSTACK();
  
  sStrR = sStrA;
  
  sStrR.vConcat(sStrB);
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.clip -- returns left or right n part of a string ( length str -- str ) */
/*----------------------------------------------------------------------------*/
void Step::vStrClip(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrClip"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString,iLength) )
    THROW_DSTACK();
  
  // validate length
  i8 iHasLength = sString.nLength();
  
  if ( iLength < 0 || llabs(iLength) >= iHasLength)
    vThrow(__func__,__FILE__,__LINE__,"Invalid length for: str.clip"_Stz);
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.azChar();
  else
    pStart = sString.azChar() + iHasLength - 1 - llabs(iLength);
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,llabs(iLength));
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.equal -- test if two strings are equal                                 */
/*----------------------------------------------------------------------------*/
void Step::vStrEqual(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrEql"_Stz);
  
  GenVar xStringA;
  GenVar xStringB;
  
  // get the arguments
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xStringB,xStringA) )
    THROW_DSTACK();
  
  i8 iEqual = StrLib::bEqualTo(_aStrVal(xStringA.aNestedValue())->azChar(),
                               _aStrVal(xStringB.aNestedValue())->azChar(),
                               C_NO_CASE);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Step::vStrEscape(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrEscape"_Stz);
  
  s48K xArg;
  s48K xEscaped;
  
  // get the arguments
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xArg) )
    THROW_DSTACK();
  
  StrLib::vEscape(xArg.azChar(),xEscaped.azChar());
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(xEscaped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.firstCsv -- get first value from csv string                            */
/*----------------------------------------------------------------------------*/
void Step::vStrFirstCsv(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrFirstCsv"_Stz);
  
  s48K sStr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sStr) )
    THROW_DSTACK();
  
  s48K sFirstValue;
  
  char* aValue = sStr.azChar();
  
  if (! StrLib::bExtractItem(&aValue,sFirstValue.azChar(), ','))
    sFirstValue = ""_Stz;
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Step::vStrFormat(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrFormat"_Stz);
  
  s48K sFormat;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sFormat) )
    THROW_DSTACK();
  
  s48K sString;
  
  mpTask->vSprint(sFormat,sString);
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.left -- returns left n part of a string ( length str -- str )          */
/*----------------------------------------------------------------------------*/
void Step::vStrLeft(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vClip"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString,iLength) )
    THROW_DSTACK();
  
  // validate length
  i8 iHasLength = sString.nLength();
  
  if ( iLength < 0)
    vThrow(__func__,__FILE__,__LINE__,"Invalid length for: str.left"_Stz);
  
  char* pStart = sString.azChar();
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,iLength);
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.length -- string length                                                */
/*----------------------------------------------------------------------------*/
void Step::vStrLength(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrLen"_Stz);
  
  i8 iLength;
  s48K sString;
  
  // get the arguments
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString) )
    THROW_DSTACK();
  
  iLength = sString.nLength();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.prepend -- prepend top string before second string                     */
/*----------------------------------------------------------------------------*/
void Step::vStrPrepend(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrPrepend"_Stz);
  
  s48K xStrR;
  s48K xStrB;
  s48K xStrA;
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(xValB,xValA) )
    THROW_DSTACK();
  
  // If received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    ASSERT(0);
//    s48K xString;
//    xValA.vGetString(xString);
//    xStrA = _aStrVal(xValA.aNestedValue())->szStz();
  }
  else {
    xStrA = _aStrVal(xValA.aNestedValue())->szStz();                          //! need to simplify this
  }
  
  // If received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    ASSERT(0);
//    s48K xString;
//    xValB.vGetString(xString);
//    xStrB = xString;
  }
  else {
    xStrB = _aStrVal(xValB.aNestedValue())->szStz();
  }
  
  xStrR = xStrB;
  
  xStrR.vConcat(xStrA);
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.random-- generate random string of given length                        */
/*----------------------------------------------------------------------------*/
void Step::vStrRandom(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrRandom"_Stz);
  
  i8 iLength;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(iLength) )
    THROW_DSTACK();
  
  s256 sRnd;
  
  Random::vString(sRnd,iLength);
  
  mpTask->m_pForth->m_pDataStack->vPushS256(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.removePrefix -- remove prefix from string                              */
/*----------------------------------------------------------------------------*/
void Step::vStrRemovePrefix(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrRemovePrefix"_Stz);
  
  s48K sString;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString) )
    THROW_DSTACK();
  
  s32 sIntro;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sIntro) )
    THROW_DSTACK();
  
  StrLib::vRemovePrefix(sString.azChar(), * sIntro.azChar());
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.removeSuffix -- remove suffix from string                              */
/*----------------------------------------------------------------------------*/
void Step::vStrRemoveSuffix(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrRemoveSuffix"_Stz);
  
  s48K sString;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString) )
    THROW_DSTACK();
  
  s32 sIntro;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sIntro) )
    THROW_DSTACK();
  
  StrLib::vRemoveSuffix(sString.azChar(), * sIntro.azChar());
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sString);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.replace-- replace this with that within string                         */
/*----------------------------------------------------------------------------*/
void Step::vStrReplace(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrReplace"_Stz);
  
  s48K sString;
  s48K sThat;
  s48K sThis;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString) )
    THROW_DSTACK();

  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sThat) )
    THROW_DSTACK();
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sThis) )
    THROW_DSTACK();
  
  s48K sReplaced;
  
  StrLib::bReplace(sReplaced.azChar(),sString.azChar(),sThis.azChar(),sThat.azChar());
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sReplaced);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.right -- Returns the right n part of a string ( length str -- str )    */
/*----------------------------------------------------------------------------*/
void Step::vStrRight(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrRight"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString,iLength) )
    THROW_DSTACK();
  
  // validate length
  i8 iHasLength = sString.nLength();
  
  if ( iLength < 0)
    vThrow(__func__,__FILE__,__LINE__,"Invalid length for: str.right"_Stz);

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.azChar() + iHasLength - iLength;
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,iLength);
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( length start str == substr )                         */
/*----------------------------------------------------------------------------*/
void Step::vStrSub(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrSub"_Stz);
  
  i8 iLength;
  i8 iStart;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString,iStart,iLength) )
    THROW_DSTACK();
  
  i8 iHasLength = sString.nLength();
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    vThrow(__func__,__FILE__,__LINE__,"Invalid start index of %lld for: str.sub"_Stz,iStart);
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    vThrow(__func__,__FILE__,__LINE__,"Invalid length of %lld for: str.sub"_Stz,iLength);
  
  char* pStart = sString.azChar() + iStart;
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,iLength);
  
  mpTask->m_pForth->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tailInt -- extract (possible) int at end of string                         */
/*----------------------------------------------------------------------------*/
void Step::vStrTailInt(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vStrTailInt"_Stz);
  
  // get the string
  s48K sString;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sString) )
    THROW_DSTACK();
  
  // get last char in string
  char* aLast = StrLib::sLast(sString.azChar());
  
  // return false if not a digit
  if ( ! StrLib::bIsDigit(*aLast) ) {
    mpTask->m_pForth->m_pDataStack->vPushBool(false);
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
  mpTask->m_pForth->m_pDataStack->vPushI8(iInt);
  mpTask->m_pForth->m_pDataStack->vPushBool(true);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
