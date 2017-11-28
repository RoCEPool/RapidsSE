/*============================================================================*/
/* Steps_Str.cpp - str step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern NP_Task         gnpBootTask;

// string: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Str(void)
{
  vAddPrimitive("s256.init"_Stz,    &vS256Init,      false);
  
  vAddPrimitive("str::algebra"_Stz, &vStrAlgebra,    false);
  vAddPrimitive("strAlgebra"_Stz,   &vStrAlgebra,    false);
  
  vAddPrimitive("str.cat"_Stz,      &vStrCat,        false);
  vAddPrimitive("str.clip"_Stz,     &vStrClip,       false);
  vAddPrimitive("str.equal"_Stz,    &vStrEqual,      false);
  vAddPrimitive("str.escape"_Stz,   &vStrEscape,     false);
  vAddPrimitive("str.firstCsv"_Stz, &vStrFirstCsv,   false);
  vAddPrimitive("str.format"_Stz,   &vStrFormat,     false);
  vAddPrimitive("str.left"_Stz,     &vStrLeft,       false);
  vAddPrimitive("str.length"_Stz,   &vStrLength,     false);
  vAddPrimitive("str.pad"_Stz,      &vStrPad,        false);
  vAddPrimitive("str.prepend"_Stz,  &vStrPrepend,    false);
  vAddPrimitive("str.random"_Stz,   &vStrRandom,     false);
  vAddPrimitive("str.replace"_Stz,  &vStrReplace,    false);
  vAddPrimitive("str.right"_Stz,    &vStrRight,      false);
  vAddPrimitive("str.sub"_Stz,      &vStrSub,        false);
  vAddPrimitive("str.tailInt"_Stz,  &vStrTailInt,    false);
}

/*----------------------------------------------------------------------------*/
/* s256.init -- initialise an s256                                            */
/*----------------------------------------------------------------------------*/
void Task::vS256Init(MP_Task mpTask)
{
  ENTRY("vS256Init"_Stz);
  
  Object* pPtrStrVal;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrStrVal) )
    THROW_INVALID_DSTACK("s256.init"_Stz);
  
  s256* ps256 = (s256*)_pStrVar(pPtrStrVal);
  
  ASSERT(CN_STRVAR == ps256->eClass());
  
  new(ps256) s256(ps256);
  
  EXIT();
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* str.algebra -- string algebra tests                                        */
/*----------------------------------------------------------------------------*/
void Task::vStrAlgebra(MP_Task mpTask)
{
  ENTRY("vStrAlgebra"_Stz);
 
  // (1) StrVal's cannot be declared as local variables
  //StrVal xStrval;  // uncommenting this line should produce a compile error
  
  // declaring a fixed length string
  s32 xString("A test"_Stz);
  
  ValWrk xValWrk;
  StrVal* pStrVal = _pStrVal(&xValWrk);
  
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.cat -- concat two strings                                              */
/*----------------------------------------------------------------------------*/
void Task::vStrCat(MP_Task mpTask)
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
void Task::vStrClip(MP_Task mpTask)
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
void Task::vStrEqual(MP_Task mpTask)
{
  ENTRY("vStrEql"_Stz);
  
  ValWrk xStringA;
  ValWrk xStringB;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(xStringB,xStringA) )
    THROW_INVALID_DSTACK("str.equal"_Stz);
  
  i8 iEqual = StrLib::bEqualTo(_pStrVal(xStringA.pNestedValueSelf())->aChar(),
                               _pStrVal(xStringB.pNestedValueSelf())->aChar(),
                               C_NO_CASE);
  
  mpTask->m_pDataStack->vPushInt(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Task::vStrEscape(MP_Task mpTask)
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
void Task::vStrFirstCsv(MP_Task mpTask)
{
  ENTRY("vStrFirstCsv"_Stz);
  
  s48K sStr;
  if ( ! mpTask->m_pDataStack->bPop(sStr) )
    THROW_INVALID_DSTACK("str.firstCsv"_Stz);
  
  s48K sFirstValue;
  
  char* aValue = sStr.aChar();
  
  if (! StrLib::bExtractItem(&aValue,sFirstValue.aChar(), C_FS))
    sFirstValue = ""_Stz;
  
  mpTask->m_pDataStack->vPushS48K(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Task::vStrFormat(MP_Task mpTask)
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
void Task::vStrLeft(MP_Task mpTask)
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
void Task::vStrLength(MP_Task mpTask)
{
  ENTRY("vStrLen"_Stz);
  
  i8 iLength;
  s48K sString;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.length"_Stz);
  
  iLength = sString.iLength();
  
  mpTask->m_pDataStack->vPushInt(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.pad -- pad string to given length                                      */
/*----------------------------------------------------------------------------*/
void Task::vStrPad(MP_Task mpTask)
{
  ENTRY("vStringPad"_Stz);
  
  i8 iLength;
  
  ValWrk xStringA;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(iLength,xStringA) )
    THROW_INVALID_DSTACK("str.pad"_Stz);
  
  StrLib::vPad(_pStrVal(xStringA.pNestedValueSelf())->aChar(),iLength,' ');
  
  // adjust area size
  _pStrVal(xStringA.pNestedValueSelf())->vAreaSize();
  
  mpTask->m_pDataStack->vPushValWrk(xStringA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.prepend -- prepend top string before second string                     */
/*----------------------------------------------------------------------------*/
void Task::vStrPrepend(MP_Task mpTask)
{
  ENTRY("vStrPrepend"_Stz);
  
  s48K xStrR;
  s48K xStrB;
  s48K xStrA;
  
  ValWrk xValA;
  ValWrk xValB;
  
  if ( ! mpTask->m_pDataStack->bPop(xValB,xValA) )
    THROW_INVALID_DSTACK("str.prepend"_Stz);
  
  // If received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    s48K xString;
    ValLib::vToString(&xValA,xString);
    xStrA = xString;
  }
  else {
    xStrA = _pStrVal(xValA.pNestedValueSelf());                          //! need to simplify this
  }
  
  // If received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    s48K xString;
    ValLib::vToString(&xValB,xString);
    xStrB = xString;
  }
  else {
    xStrB = _pStrVal(xValB.pNestedValueSelf());
  }
  
  xStrR = xStrB;
  
  xStrR.vConcat(xStrA);
  
  mpTask->m_pDataStack->vPushS48K(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.random-- generate random string of given length                        */
/*----------------------------------------------------------------------------*/
void Task::vStrRandom(MP_Task mpTask)
{
  ENTRY("vStrRandom"_Stz);
  
  i8 iLength;
  if ( ! mpTask->m_pDataStack->bPop(iLength) )
    THROW_INVALID_DSTACK("str.random"_Stz);
  
  s48K sRnd;
  
  sRnd = Random::sRandomString(iLength).aStr();
  
  mpTask->m_pDataStack->vPushS48K(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.replace-- replace this with that within string                         */
/*----------------------------------------------------------------------------*/
void Task::vStrReplace(MP_Task mpTask)
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
void Task::vStrRight(MP_Task mpTask)
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
void Task::vStrSub(MP_Task mpTask)
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
void Task::vStrTailInt(MP_Task mpTask)
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
    mpTask->m_pDataStack->vPushInt((i8)0);
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
  mpTask->m_pDataStack->vPushInt(iInt);
  mpTask->m_pDataStack->vPushInt((i8)1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint of stack using given format string                                  */
/*----------------------------------------------------------------------------*/
void Task::vSprint(s48K& sFormat, s48K& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  char* pf = sFormat.aChar();
  char* ps = sString.aChar();
  
  while (*pf) {
    
    // test for ordinary character
    if ('%' != *pf) {
      *ps++ = *pf++;
      continue;
    }
    
    // we have a '%'
    pf++;
    
    // test for %%
    if ('%' == *pf) {
      *ps++ = *pf++;
      continue;
    }
    
    // should be %v or %V
    ASSERT('v' == *pf || 'V' == *pf);
    
    Value* pValue0 = m_pDataStack->pPeekValue(0);
    
    // test for StrVal
    if (CN_STRVAL == pValue0->eClass()) {
      
      //*pf = 's';                                                    //! Defect??
      
      s48K sValue;
      m_pDataStack->bPop(sValue);
      
      if ('V' == *pf)
        *ps++ = '"';
      
      for (char* pv = sValue.aChar(); *pv; )
      *ps++ = *pv++;
      
      if ('V' == *pf)
        *ps++ = '"';
      
      pf++;
      
      continue;
    }
    
    // test for intval
    if (CN_INTVAL == pValue0->eClass()) {
      
      //*pf = 'i';                                                    //! Defect??
      
      i8 iValue;
      m_pDataStack->bPop(iValue);
      
      char aValue[32];
      StrLib::wToString(iValue,aValue);
      
      for (char* pv = aValue; *pv; )
      *ps++ = *pv++;
      
      pf++;
      
      continue;
    }
    
    // just copy %?
    *ps++ = '%';
    *ps++ = *pf++;
  }
  
  //*pf = '\0';                                                       //! Defect??
  *ps = '\0';
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
