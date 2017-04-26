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
void Task::vS256Init(NNP_Task nnpTask)
{
  ENTRY("vS256Init"_Str);
  
  Object* pPtrStrVal;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrStrVal) )
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
void Task::vStrAlgebra(NNP_Task nnpTask)
{
  ENTRY("vStrAlgebra"_Str);
 
  // (1) StrVal's cannot be declared as local variables
  //StrVal xStrval;  // uncommenting this line should produce a compile error
  
  // declaring a fixed length string
  s32 xString("A test"_Str);
  
  ValWsp xValWsp;
  StrVal* pStrVal = _pStrVal(&xValWsp);
  
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.cat -- concat two strings                                              */
/*----------------------------------------------------------------------------*/
void Task::vStrCat(NNP_Task nnpTask)
{
  ENTRY("vStrCat"_Str);
  
  s48K sStrR;
  s48K sStrB;
  s48K sStrA;
  
  if ( ! nnpTask->m_pDataStack->bPop(sStrB,sStrA) )
    THROW_INVALID_DSTACK("str.cat"_Str);
  
  sStrR = sStrA;
  
  sStrR.vConcat(sStrB);
  
  nnpTask->m_pDataStack->vPushS48K(sStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.clip -- returns left or right n part of a string ( length str -- str ) */
/*----------------------------------------------------------------------------*/
void Task::vStrClip(NNP_Task nnpTask)
{
  ENTRY("vStrClip"_Str);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
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
  
  nnpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.equal -- test if two strings are equal                                 */
/*----------------------------------------------------------------------------*/
void Task::vStrEqual(NNP_Task nnpTask)
{
  ENTRY("vStrEql"_Str);
  
  ValWsp xStringA;
  ValWsp xStringB;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(xStringB,xStringA) )
    THROW_INVALID_DSTACK("str.equal"_Str);
  
  i8 iEqual = StrLib::bEqualTo(_pStrVal(&xStringA)->s(),
                               _pStrVal(&xStringB)->s(),
                               C_NO_CASE);
  
  nnpTask->m_pDataStack->vPushInt(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Task::vStrEscape(NNP_Task nnpTask)
{
  ENTRY("vStrEscape"_Str);
  
  s48K xArg;
  s48K xEscaped;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(xArg) )
    THROW_INVALID_DSTACK("str.escape"_Str);
  
  StrLib::vEscape(xArg.s(),xEscaped.s());
  
  nnpTask->m_pDataStack->vPushS48K(xEscaped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.firstCsv -- get first value from csv string                            */
/*----------------------------------------------------------------------------*/
void Task::vStrFirstCsv(NNP_Task nnpTask)
{
  ENTRY("vStrFirstCsv"_Str);
  
  ValWsp xStr;
  if ( ! nnpTask->m_pDataStack->bPop(xStr) )
    THROW_INVALID_DSTACK("str.firstCsv"_Str);
  
  s256 sFirstValue;
  
  char* z = _pStrVal(&xStr)->s();
  
  if (! StrLib::bExtractItem(&z,sFirstValue.s(), '|'))
    sFirstValue = ""_Str;
  
  nnpTask->m_pDataStack->vPushS256(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Task::vStrFormat(NNP_Task nnpTask)
{
  ENTRY("vStrFormat"_Str);
  
  s48K sFormat;
  if ( ! nnpTask->m_pDataStack->bPop(sFormat) )
    THROW_INVALID_DSTACK("str.format"_Str);
  
  s48K sString;
  
  nnpTask->vSprint(sFormat,sString);
  
  nnpTask->m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.left -- returns left n part of a string ( length str -- str )          */
/*----------------------------------------------------------------------------*/
void Task::vStrLeft(NNP_Task nnpTask)
{
  ENTRY("vClip"_Str);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.left"_Str);
  
  // validate length
  i8 iHasLength = sString.iLength();
  
  if ( iLength < 0)
    THROW("Invalid length for: str.left"_Str);
  
  char* pStart = sString.s();
  
  StrLib::vCopyLen(sSubStr.s(),pStart,iLength);
  
  nnpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.length -- string length                                                    */
/*----------------------------------------------------------------------------*/
void Task::vStrLength(NNP_Task nnpTask)
{
  ENTRY("vStrLen"_Str);
  
  i8 iLength;
  s48K sString;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.length"_Str);
  
  iLength = sString.iLength();
  
  nnpTask->m_pDataStack->vPushInt(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.pad -- pad string to given length                                      */
/*----------------------------------------------------------------------------*/
void Task::vStrPad(NNP_Task nnpTask)
{
  ENTRY("vStringPad"_Str);
  
  i8 iLength;
  
  ValWsp xStringA;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(iLength,xStringA) )
    THROW_INVALID_DSTACK("str.pad"_Str);
  
  StrLib::vPad(_pStrVal(&xStringA)->s(),iLength,' ');
  
  // adjust area size
  _pStrVal(&xStringA)->vAreaSize();
  
  nnpTask->m_pDataStack->vPushValWsp(xStringA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.prepend -- prepend top string before second string                     */
/*----------------------------------------------------------------------------*/
void Task::vStrPrepend(NNP_Task nnpTask)
{
  ENTRY("vStrPrepend"_Str);
  
  s48K xStrR;
  s48K xStrB;
  s48K xStrA;
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValB,xValA) )
    THROW_INVALID_DSTACK("str.prepend"_Str);
  
  // If received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    s48K xString;
    ValLib::vToString(&xValA,xString);
    xStrA = xString;
  }
  else {
    xStrA = _pStrVal(&xValA);                          //! need to simplify this
  }
  
  // If received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    s48K xString;
    ValLib::vToString(&xValB,xString);
    xStrB = xString;
  }
  else {
    xStrB = _pStrVal(&xValB);
  }
  
  xStrR = xStrB;
  
  xStrR.vConcat(xStrA);
  
  nnpTask->m_pDataStack->vPushS48K(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.random-- generate random string of given length                        */
/*----------------------------------------------------------------------------*/
void Task::vStrRandom(NNP_Task nnpTask)
{
  ENTRY("vStrRandom"_Str);
  
  i8 iLength;
  if ( ! nnpTask->m_pDataStack->bPop(iLength) )
    THROW_INVALID_DSTACK("str.random"_Str);
  
  s48K sRnd;
  
  sRnd = Random::sRandomString(iLength).aStr();
  
  nnpTask->m_pDataStack->vPushS48K(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.replace-- replace this with that within string                         */
/*----------------------------------------------------------------------------*/
void Task::vStrReplace(NNP_Task nnpTask)
{
  ENTRY("vStrReplace"_Str);
  
  s48K sString;
  s48K sThat;
  s48K sThis;
  
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.replace"_Str);

  if ( ! nnpTask->m_pDataStack->bPop(sThat) )
    THROW_INVALID_DSTACK("str.replace"_Str);
  
  if ( ! nnpTask->m_pDataStack->bPop(sThis) )
    THROW_INVALID_DSTACK("str.replace"_Str);
  
  s48K sReplaced;
  
  StrLib::bReplace(sReplaced.s(),sString.s(),sThis.s(),sThat.s());
  
  nnpTask->m_pDataStack->vPushS48K(sReplaced);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.right -- Returns the right n part of a string ( length str -- str )    */
/*----------------------------------------------------------------------------*/
void Task::vStrRight(NNP_Task nnpTask)
{
  ENTRY("vStrRight"_Str);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.right"_Str);
  
  // validate length
  i8 iHasLength = sString.iLength();
  
  if ( iLength < 0)
    THROW("Invalid length for: str.right"_Str);

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.s() + iHasLength - iLength;
  
  StrLib::vCopyLen(sSubStr.s(),pStart,iLength);
  
  nnpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( start length str == substr )                         */
/*----------------------------------------------------------------------------*/
void Task::vStrSub(NNP_Task nnpTask)
{
  ENTRY("vStrSub"_Str);
  
  i8 iLength;
  i8 iStart;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iStart,iLength) )
    THROW_INVALID_DSTACK("str.sub"_Str);
  
  i8 iHasLength = sString.iLength();
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    THROW("Invalid start index for: str.sub"_Str);
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    THROW("Invalid length for: str.sub"_Str);
  
  char* pStart = sString.s() + iStart;
  
  StrLib::vCopyLen(sSubStr.s(),pStart,iLength);
  
  nnpTask->m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tailInt -- extract (possible) int at end of string                         */
/*----------------------------------------------------------------------------*/
void Task::vStrTailInt(NNP_Task nnpTask)
{
  ENTRY("vStrTailInt"_Str);
  
  // get the string
  s48K sString;
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.tailInt"_Str);
  
  // get last char in string
  char* aLast = StrLib::sLast(sString.s());
  
  // return false if not a digit
  if ( ! StrLib::bIsDigit(*aLast) ) {
    nnpTask->m_pDataStack->vPushInt((i8)0);
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
  nnpTask->m_pDataStack->vPushInt(iInt);
  nnpTask->m_pDataStack->vPushInt((i8)1);
  
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
