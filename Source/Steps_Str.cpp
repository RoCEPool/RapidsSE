/*============================================================================*/
/* Steps_Str.cpp - str step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  ENTRY("vS256Init");
  
  Object* pPtrStrVal;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrStrVal) )
    THROW_INVALID_DSTACK("s256.init");
  
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
  ENTRY("vStrAlgebra");
 
  // (1) StrVal's cannot be declared as local variables
  //StrVal xStrval;  // uncommenting this line should produce a compile error
  
  // declaring a fixed length string
  s32 xString("A test");
  
  ValWsp xValWsp;
  StrVal* pStrVal = _pStrVal(&xValWsp);
  
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.cat -- concat two strings                                              */
/*----------------------------------------------------------------------------*/
void Task::vStrCat(NNP_Task nnpTask)
{
  ENTRY("vStrCat");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;
  
  if ( ! nnpTask->m_pDataStack->bPop(xStrB,xStrA) )
    THROW_INVALID_DSTACK("str.cat");
  
  xStrR = xStrA;
  
  StrLib::vConcat(_pStrVal(&xStrR)->aStr(),_pStrVal(&xStrB)->aStr());
  
  // adjust area size
  _pStrVal(&xStrR)->vAreaSize();
  
  nnpTask->m_pDataStack->vPush(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.clip -- returns left or right n part of a string ( length str -- str ) */
/*----------------------------------------------------------------------------*/
void Task::vStrClip(NNP_Task nnpTask)
{
  ENTRY("vStrClip");
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.clip");
  
  // validate length
  i8 iHasLength = StrLib::iLength(sString.aStr());
  
  if ( iLength < 0 || llabs(iLength) >= iHasLength)
    THROW("Invalid length for: str.clip"_Str);
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.aStr();
  else
    pStart = sString.aStr() + iHasLength - 1 - llabs(iLength);
  
  StrLib::vCopyLen(sSubStr.aStr(),pStart,llabs(iLength));
  
  nnpTask->m_pDataStack->vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.equal -- test if two strings are equal                                 */
/*----------------------------------------------------------------------------*/
void Task::vStrEqual(NNP_Task nnpTask)
{
  ENTRY("vStrEql");
  
  ValWsp xStringA;
  ValWsp xStringB;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(xStringB,xStringA) )
    THROW_INVALID_DSTACK("str.equal");
  
  i8 iEqual = StrLib::bEqualTo(_pStrVal(&xStringA)->aStr(),
                               _pStrVal(&xStringB)->aStr());
  
  nnpTask->m_pDataStack->vPush(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Task::vStrEscape(NNP_Task nnpTask)
{
  ENTRY("vStrEscape");
  
  s48K xArg;
  s48K xEscaped;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(xArg) )
    THROW_INVALID_DSTACK("str.escape");
  
  StrLib::vEscape(xArg.aStr(),xEscaped.aStr());
  
  nnpTask->m_pDataStack->vPush(xEscaped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.firstCsv -- get first value from csv string                            */
/*----------------------------------------------------------------------------*/
void Task::vStrFirstCsv(NNP_Task nnpTask)
{
  ENTRY("vStrFirstCsv");
  
  ValWsp xStr;
  if ( ! nnpTask->m_pDataStack->bPop(xStr) )
    THROW_INVALID_DSTACK("str.firstCsv");
  
  s256 sFirstValue;
  
  char* z = _pStrVal(&xStr)->aStr();
  
  if (! StrLib::bExtractItem(&z,sFirstValue.aStr()))
    sFirstValue = ""_Str;
  
  nnpTask->m_pDataStack->vPush(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Task::vStrFormat(NNP_Task nnpTask)
{
  ENTRY("vStrFormat");
  
  s48K sFormat;
  if ( ! nnpTask->m_pDataStack->bPop(sFormat) )
    THROW_INVALID_DSTACK("str.format");
  
  s48K sString;
  
  nnpTask->vSprint(sFormat,sString);
  
  nnpTask->m_pDataStack->vPush(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.left -- returns left n part of a string ( length str -- str )          */
/*----------------------------------------------------------------------------*/
void Task::vStrLeft(NNP_Task nnpTask)
{
  ENTRY("vClip");
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.left");
  
  // validate length
  i8 iHasLength = StrLib::iLength(sString.aStr());
  
  if ( iLength < 0)
    THROW("Invalid length for: str.left"_Str);
  
  char* pStart = sString.aStr();
  
  StrLib::vCopyLen(sSubStr.aStr(),pStart,iLength);
  
  nnpTask->m_pDataStack->vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.length -- string length                                                    */
/*----------------------------------------------------------------------------*/
void Task::vStrLength(NNP_Task nnpTask)
{
  ENTRY("vStrLen");
  
  i8 iLength;
  s48K sString;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.length");
  
  iLength = StrLib::iLength(sString.aStr());
  
  nnpTask->m_pDataStack->vPush(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.pad -- pad string to given length                                      */
/*----------------------------------------------------------------------------*/
void Task::vStrPad(NNP_Task nnpTask)
{
  ENTRY("vStringPad");
  
  i8 iLength;
  
  ValWsp xStringA;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(iLength,xStringA) )
    THROW_INVALID_DSTACK("str.pad");
  
  StrLib::vPad(_pStrVal(&xStringA)->aStr(),iLength,' ');
  
  // adjust area size
  _pStrVal(&xStringA)->vAreaSize();
  
  nnpTask->m_pDataStack->vPush(xStringA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.prepend -- prepend top string before second string                     */
/*----------------------------------------------------------------------------*/
void Task::vStrPrepend(NNP_Task nnpTask)
{
  ENTRY("vStrPrepend");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_pDataStack->bPop(xValB,xValA) )
    THROW_INVALID_DSTACK("str.prepend");
  
  // If received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    s48K xString;
    ValLib::vToString(&xValA,xString.aStr());
    xStrA = xString;
  }
  else {
    xStrA = xValA;
  }
  
  // If received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    s48K xString;
    ValLib::vToString(&xValB,xString.aStr());
    xStrB = xString;
  }
  else {
    xStrB = xValB;
  }
  
  xStrR = xStrB;
  
  StrLib::vConcat(_pStrVal(&xStrR)->aStr(),_pStrVal(&xStrA)->aStr());
  
  // adjust area size
  _pStrVal(&xStrR)->vAreaSize();
  
  nnpTask->m_pDataStack->vPush(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.random-- generate random string of given length                        */
/*----------------------------------------------------------------------------*/
void Task::vStrRandom(NNP_Task nnpTask)
{
  ENTRY("vStrRandom");
  
  i8 iLength;
  if ( ! nnpTask->m_pDataStack->bPop(iLength) )
    THROW_INVALID_DSTACK("str.random");
  
  s48K sRnd;
  
  sRnd = Random::sRandomString(iLength).aStrArea();
  
  nnpTask->m_pDataStack->vPush(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.replace-- replace this with that within string                         */
/*----------------------------------------------------------------------------*/
void Task::vStrReplace(NNP_Task nnpTask)
{
  ENTRY("vStrReplace");
  
  s48K sString;
  s48K sThat;
  s48K sThis;
  
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.replace");

  if ( ! nnpTask->m_pDataStack->bPop(sThat) )
    THROW_INVALID_DSTACK("str.replace");
  
  if ( ! nnpTask->m_pDataStack->bPop(sThis) )
    THROW_INVALID_DSTACK("str.replace");
  
  s48K sReplaced;
  
  StrLib::bReplace(sReplaced.aStr(),sString.aStr(),sThis.aStr(),sThat.aStr());
  
  nnpTask->m_pDataStack->vPush(sReplaced);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.right -- Returns the right n part of a string ( length str -- str )    */
/*----------------------------------------------------------------------------*/
void Task::vStrRight(NNP_Task nnpTask)
{
  ENTRY("vStrRight");
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.right");
  
  // validate length
  i8 iHasLength = StrLib::iLength(sString.aStr());
  
  if ( iLength < 0)
    THROW("Invalid length for: str.right"_Str);

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.aStr() + iHasLength - iLength;
  
  StrLib::vCopyLen(sSubStr.aStr(),pStart,iLength);
  
  nnpTask->m_pDataStack->vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( start length str == substr )                         */
/*----------------------------------------------------------------------------*/
void Task::vStrSub(NNP_Task nnpTask)
{
  ENTRY("vStrSub");
  
  i8 iLength;
  i8 iStart;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iStart,iLength) )
    THROW_INVALID_DSTACK("str.sub");
  
  i8 iHasLength = StrLib::iLength(sString.aStr());
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    THROW("Invalid start index for: str.sub"_Str);
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    THROW("Invalid length for: str.sub"_Str);
  
  char* pStart = sString.aStr() + iStart;
  
  StrLib::vCopyLen(sSubStr.aStr(),pStart,iLength);
  
  nnpTask->m_pDataStack->vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tailInt -- extract (possible) int at end of string                         */
/*----------------------------------------------------------------------------*/
void Task::vStrTailInt(NNP_Task nnpTask)
{
  ENTRY("vStrTailInt");
  
  // get the string
  s48K sString;
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.tailInt");
  
  // get last char in string
  char* aLast = StrLib::sLast(sString.aStr());
  
  // return false if not a digit
  if ( ! StrLib::bIsDigit(*aLast) ) {
    nnpTask->m_pDataStack->vPush((i8)0);
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
  nnpTask->m_pDataStack->vPush(iInt);
  nnpTask->m_pDataStack->vPush((i8)1);
  
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
  
  char* pf = sFormat.aStr();
  char* ps = sString.aStr();
  
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
    
    Value* pValue0 = m_pDataStack->pPeek(0);
    
    // test for StrVal
    if (CN_STRVAL == pValue0->eClass()) {
      
      //*pf = 's';                                                    //! Defect??
      
      s48K sValue;
      m_pDataStack->bPop(sValue);
      
      if ('V' == *pf)
        *ps++ = '"';
      
      for (char* pv = sValue.aStr(); *pv; )
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
