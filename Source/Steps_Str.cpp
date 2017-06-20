/*============================================================================*/
/* Steps_Str.cpp - str step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// string: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Str(void)
{
  vAddPrimitive("s256.init"_Str,    &vS256Init,      false);
  
  vAddPrimitive("str::algebra"_Str, &vStrAlgebra,    false);
  vAddPrimitive("strAlgebra"_Str,   &vStrAlgebra,    false);
  
  vAddPrimitive("str.cat"_Str,      &vStrCat,        false);
  vAddPrimitive("str.clip"_Str,     &vStrClip,       false);
  vAddPrimitive("str.equal"_Str,    &vStrEqual,      false);
  vAddPrimitive("str.escape"_Str,   &vStrEscape,     false);
  vAddPrimitive("str.firstCsv"_Str, &vStrFirstCsv,   false);
  vAddPrimitive("str.format"_Str,   &vStrFormat,     false);
  vAddPrimitive("str.left"_Str,     &vStrLeft,       false);
  vAddPrimitive("str.length"_Str,   &vStrLength,     false);
  vAddPrimitive("str.pad"_Str,      &vStrPad,        false);
  vAddPrimitive("str.prepend"_Str,  &vStrPrepend,    false);
  vAddPrimitive("str.random"_Str,   &vStrRandom,     false);
  vAddPrimitive("str.replace"_Str,  &vStrReplace,    false);
  vAddPrimitive("str.right"_Str,    &vStrRight,      false);
  vAddPrimitive("str.sub"_Str,      &vStrSub,        false);
  vAddPrimitive("str.tailInt"_Str,  &vStrTailInt,    false);
}

/*----------------------------------------------------------------------------*/
/* s256.init -- initialise an s256                                            */
/*----------------------------------------------------------------------------*/
void Task::vS256Init(MP_Task mpTask)
{
  ENTRY("vS256Init"_Str);
  
  Object* pPtrStrVal;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrStrVal) )
    THROW_INVALID_DSTACK("s256.init"_Str);
  
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
  ENTRY("vStrAlgebra"_Str);
 
  // (1) StrVal's cannot be declared as local variables
  //StrVal xStrval;  // uncommenting this line should produce a compile error
  
  // declaring a fixed length string
  s32 xString("A test"_Str);
  
  ValWrk xValWrk;
  StrVal* pStrVal = _pStrVal(&xValWrk);
  
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.cat -- concat two strings                                              */
/*----------------------------------------------------------------------------*/
void Task::vStrCat(MP_Task mpTask)
{
  ENTRY("vStrCat"_Str);
  
  s48K sStrR;
  s48K sStrB;
  s48K sStrA;
  
  if ( ! mpTask->m_pDataStack->bPop(sStrB,sStrA) )
    THROW_INVALID_DSTACK("str.cat"_Str);
  
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
  ENTRY("vStrClip"_Str);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.clip"_Str);
  
  // validate length
  i8 iHasLength = sString.iLength();
  
  if ( iLength < 0 || llabs(iLength) >= iHasLength)
    THROW("Invalid length for: str.clip"_Str);
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.s();
  else
    pStart = sString.s() + iHasLength - 1 - llabs(iLength);
  
  StrLib::vCopyLen(sSubStr.s(),pStart,llabs(iLength));
  
  mpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.equal -- test if two strings are equal                                 */
/*----------------------------------------------------------------------------*/
void Task::vStrEqual(MP_Task mpTask)
{
  ENTRY("vStrEql"_Str);
  
  ValWrk xStringA;
  ValWrk xStringB;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(xStringB,xStringA) )
    THROW_INVALID_DSTACK("str.equal"_Str);
  
  i8 iEqual = StrLib::bEqualTo(_pStrVal(xStringA.pNestedValueSelf())->s(),
                               _pStrVal(xStringB.pNestedValueSelf())->s(),
                               C_NO_CASE);
  
  mpTask->m_pDataStack->vPushInt(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Task::vStrEscape(MP_Task mpTask)
{
  ENTRY("vStrEscape"_Str);
  
  s48K xArg;
  s48K xEscaped;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(xArg) )
    THROW_INVALID_DSTACK("str.escape"_Str);
  
  StrLib::vEscape(xArg.s(),xEscaped.s());
  
  mpTask->m_pDataStack->vPushS48K(xEscaped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.firstCsv -- get first value from csv string                            */
/*----------------------------------------------------------------------------*/
void Task::vStrFirstCsv(MP_Task mpTask)
{
  ENTRY("vStrFirstCsv"_Str);
  
  s48K sStr;
  if ( ! mpTask->m_pDataStack->bPop(sStr) )
    THROW_INVALID_DSTACK("str.firstCsv"_Str);
  
  s48K sFirstValue;
  
  char* aValue = sStr.s();
  
  if (! StrLib::bExtractItem(&aValue,sFirstValue.s(), '|'))
    sFirstValue = ""_Str;
  
  mpTask->m_pDataStack->vPushS48K(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Task::vStrFormat(MP_Task mpTask)
{
  ENTRY("vStrFormat"_Str);
  
  s48K sFormat;
  if ( ! mpTask->m_pDataStack->bPop(sFormat) )
    THROW_INVALID_DSTACK("str.format"_Str);
  
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
  ENTRY("vClip"_Str);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.left"_Str);
  
  // validate length
  i8 iHasLength = sString.iLength();
  
  if ( iLength < 0)
    THROW("Invalid length for: str.left"_Str);
  
  char* pStart = sString.s();
  
  StrLib::vCopyLen(sSubStr.s(),pStart,iLength);
  
  mpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.length -- string length                                                    */
/*----------------------------------------------------------------------------*/
void Task::vStrLength(MP_Task mpTask)
{
  ENTRY("vStrLen"_Str);
  
  i8 iLength;
  s48K sString;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.length"_Str);
  
  iLength = sString.iLength();
  
  mpTask->m_pDataStack->vPushInt(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.pad -- pad string to given length                                      */
/*----------------------------------------------------------------------------*/
void Task::vStrPad(MP_Task mpTask)
{
  ENTRY("vStringPad"_Str);
  
  i8 iLength;
  
  ValWrk xStringA;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(iLength,xStringA) )
    THROW_INVALID_DSTACK("str.pad"_Str);
  
  StrLib::vPad(_pStrVal(xStringA.pNestedValueSelf())->s(),iLength,' ');
  
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
  ENTRY("vStrPrepend"_Str);
  
  s48K xStrR;
  s48K xStrB;
  s48K xStrA;
  
  ValWrk xValA;
  ValWrk xValB;
  
  if ( ! mpTask->m_pDataStack->bPop(xValB,xValA) )
    THROW_INVALID_DSTACK("str.prepend"_Str);
  
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
  ENTRY("vStrRandom"_Str);
  
  i8 iLength;
  if ( ! mpTask->m_pDataStack->bPop(iLength) )
    THROW_INVALID_DSTACK("str.random"_Str);
  
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
  ENTRY("vStrReplace"_Str);
  
  s48K sString;
  s48K sThat;
  s48K sThis;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.replace"_Str);

  if ( ! mpTask->m_pDataStack->bPop(sThat) )
    THROW_INVALID_DSTACK("str.replace"_Str);
  
  if ( ! mpTask->m_pDataStack->bPop(sThis) )
    THROW_INVALID_DSTACK("str.replace"_Str);
  
  s48K sReplaced;
  
  StrLib::bReplace(sReplaced.s(),sString.s(),sThis.s(),sThat.s());
  
  mpTask->m_pDataStack->vPushS48K(sReplaced);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.right -- Returns the right n part of a string ( length str -- str )    */
/*----------------------------------------------------------------------------*/
void Task::vStrRight(MP_Task mpTask)
{
  ENTRY("vStrRight"_Str);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.right"_Str);
  
  // validate length
  i8 iHasLength = sString.iLength();
  
  if ( iLength < 0)
    THROW("Invalid length for: str.right"_Str);

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.s() + iHasLength - iLength;
  
  StrLib::vCopyLen(sSubStr.s(),pStart,iLength);
  
  mpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( length start str == substr )                         */
/*----------------------------------------------------------------------------*/
void Task::vStrSub(MP_Task mpTask)
{
  ENTRY("vStrSub"_Str);
  
  i8 iLength;
  i8 iStart;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! mpTask->m_pDataStack->bPop(sString,iStart,iLength) )
    THROW_INVALID_DSTACK("str.sub"_Str);
  
  i8 iHasLength = sString.iLength();
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    THROW("Invalid start index of %lld for: str.sub"_Str,iStart);
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    THROW("Invalid length of %lld for: str.sub"_Str,iLength);
  
  char* pStart = sString.s() + iStart;
  
  StrLib::vCopyLen(sSubStr.s(),pStart,iLength);
  
  mpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tailInt -- extract (possible) int at end of string                         */
/*----------------------------------------------------------------------------*/
void Task::vStrTailInt(MP_Task mpTask)
{
  ENTRY("vStrTailInt"_Str);
  
  // get the string
  s48K sString;
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.tailInt"_Str);
  
  // get last char in string
  char* aLast = StrLib::sLast(sString.s());
  
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
  
  char* pf = sFormat.s();
  char* ps = sString.s();
  
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
      
      for (char* pv = sValue.s(); *pv; )
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
