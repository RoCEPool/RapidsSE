/*============================================================================*/
/* Task_Str.cpp - task str group                                              */
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
  vAddPrimitive("strpad",      &vStringPad,   false);
  vAddPrimitive("subStr",      &vSubStr,      false);
  vAddPrimitive("concat",      &vConcat,      false);
  vAddPrimitive("clip",        &vClip,        false);
  vAddPrimitive("prepend",     &vPrepend,     false);
  vAddPrimitive("toDigitName", &vToDigitName, false);
  vAddPrimitive("sprint",      &vSprint,      false);
  vAddPrimitive("s256.init",   &vS256Init,    false);
  vAddPrimitive("csv.firstValue",  &vCsvFirstValue,    false);
  
  vAddPrimitive("str.equal",   &vStrEqual,    false);
  vAddPrimitive("str.length",  &vStrLength,   false);
  vAddPrimitive("str.random",  &vStrRandom,   false);
  vAddPrimitive("str.right",   &vStrRight,    false);
}

/*----------------------------------------------------------------------------*/
/* csv.firstValue -- get first value from csv string                          */
/*----------------------------------------------------------------------------*/
void Task::vCsvFirstValue(NNP_Task nnpTask)
{
  ENTRY("vCsvFirstValue");
  
  ValWsp xStr;
  if ( ! nnpTask->m_xDataStack.bPop(xStr) )
    THROW_INVALID_DSTACK("csv.firstValue");
  
  s256 sFirstValue;
  
  char* z = xStr.sStr();
  
  if (! String::bExtractItem(&z,sFirstValue.a()))
    sFirstValue = "";
  
  nnpTask->m_xDataStack.vPush(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* strpad -- pad string to given length                                       */
/*----------------------------------------------------------------------------*/
void Task::vStringPad(NNP_Task nnpTask)
{
ENTRY("vStringPad");
  
  i8 iLength;

  ValWsp xStringA;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(iLength,xStringA) )
    THROW_INVALID_DSTACK("strpad");
  
  String::vPad(xStringA.sStr(),iLength,' ');
  xStringA.pAsStrVal()->vSize();
  
  nnpTask->m_xDataStack.vPush(xStringA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( start length str == substr )                         */
/*----------------------------------------------------------------------------*/
//void Task::vSubStr(NNP_Task nnpTask)
//{
//ENTRY("vSubStr");
//  
//  i8 iLength;
//  i8 iStart;
//  ValWsp xString;
//  ValWsp xSubStr;
//  
//  StrVal* pStrVal = xString.pAsStrVal();
//  
//  // get the arguments
//  if ( ! nnpTask->m_xDataStack.bPop(iStart,iLength,xString) )
//    THROW_INVALID_DSTACK("substr");
//  
//  // validate start index
//  if ( iStart < 0 || iStart >= pStrVal->iChars())
//    THROW("Invalid start index for: subStr");
//  
//  // handle to end of string case
//  if (-1 == iLength)
//    iLength = pStrVal->iChars() - iStart;
//  
//  if ( iLength <= 0 || iStart + iLength > pStrVal->iChars())
//    THROW("Invalid length for: substr");
//  
//  // compute substring
//  //xSubStr.vCopyString(xString.sStr() + iStartIndex,iLength);
//  
//  char* pStart = xString.sStr() + iStart;
//  
//  xSubStr.pAsStrVal()->vCopyString(pStart,iLength);
//  
//  nnpTask->m_xDataStack.vPush(&xSubStr);
//  
//  EXIT();
//}
void Task::vSubStr(NNP_Task nnpTask)
{
  ENTRY("vSubStr");
  
  i8 iLength;
  i8 iStart;
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(iStart,iLength,sString) )
    THROW_INVALID_DSTACK("substr");
  
  i8 iHasLength = String::iLength(sString.a());
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    THROW("Invalid start index for: subStr");
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    THROW("Invalid length for: substr");
  
  char* pStart = sString.a() + iStart;
  
  String::vCopyLen(sSubStr.a(),pStart,iLength);
  
  nnpTask->m_xDataStack.vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* concat -- concat two strings                                               */
/*----------------------------------------------------------------------------*/
void Task::vConcat(NNP_Task nnpTask)
{
ENTRY("vConcat");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;
  
  if ( ! nnpTask->m_xDataStack.bPop(xStrB,xStrA) )
    THROW_INVALID_DSTACK("concat");
  
  xStrR = xStrA;
  
  String::vConcat(xStrR.sStr(),xStrB.sStr());
  xStrR.pAsStrVal()->vSize();
  
  nnpTask->m_xDataStack.vPush(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* clip -- clip string ( length str -- str )                                  */
/*----------------------------------------------------------------------------*/
//void Task::vClip(NNP_Task nnpTask)
//{
//ENTRY("vClip");
//  
//  i8 iLength;
//  ValWsp xString;
//  ValWsp xSubStr;
//  
//  StrVal* pStrVal = xString.pAsStrVal();
//  
//  // get the arguments
//  if ( ! nnpTask->m_xDataStack.bPop(iLength,xString) )
//    THROW_INVALID_DSTACK("clip");
//  
//  // validate length
//  i8 iStrLength = pStrVal->iChars();
//  
//  if ( iLength == 0 || llabs(iLength) >= iStrLength)
//    THROW("Invalid length for: clip");
//  
//  char* pStart;
//  
//  if (iLength > 0)
//    pStart = xString.sStr();
//  else
//    pStart = xString.sStr() + iStrLength - 1 - llabs(iLength);
//  
//  xSubStr.pAsStrVal()->vCopyString(pStart,llabs(iLength));
//  
//  nnpTask->m_xDataStack.vPush(&xSubStr);
//  
//  EXIT();
//}
void Task::vClip(NNP_Task nnpTask)
{
  ENTRY("vClip");
  
  i8 iLength;
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(iLength,sString) )
    THROW_INVALID_DSTACK("clip");
  
  // validate length
  i8 iHasLength = String::iLength(sString.a());
  
  if ( iLength == 0 || llabs(iLength) >= iHasLength)
    THROW("Invalid length for: clip");
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.a();
  else
    pStart = sString.a() + iHasLength - 1 - llabs(iLength);
  
  String::vCopyLen(sSubStr.a(),pStart,llabs(iLength));
  
  nnpTask->m_xDataStack.vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >DigitName -- convert to digit name                                        */
/*----------------------------------------------------------------------------*/
void Task::vToDigitName(NNP_Task nnpTask)
{
ENTRY("vToDigitName");
  
  i8 iDigit;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(iDigit) )
    THROW_INVALID_DSTACK(">DigitName");
  
  s32 sName;
  
  switch (iDigit) {
    case 0: sName = "zero";    break;
    case 1: sName = "one";     break;
    case 2: sName = "two";     break;
    case 3: sName = "three";   break;
    case 4: sName = "four";    break;
    case 5: sName = "five";    break;
    case 6: sName = "six";     break;
    case 7: sName = "seven";   break;
    case 8: sName = "eight";   break;
    case 9: sName = "nine";    break;
    default:sName = "invalid"; break;
  }

  nnpTask->m_xDataStack.vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prepend -- prepend a string before the other                               */
/*----------------------------------------------------------------------------*/
void Task::vPrepend(NNP_Task nnpTask)
{
  ENTRY("vPrepend");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValB,xValA) )
  THROW_INVALID_DSTACK("prepend");
  
  // If received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    s8K sString;
    xValA.vToString(sString.a());
    xStrA = sString.a();
  }
  else {
   xStrA = xValA;
  }
  
  // If received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    s8K sString;
    xValB.vToString(sString.a());
    xStrB = sString.a();
  }
  else {
   xStrB = xValB;
  }
  
  xStrR = xStrB;
  
  String::vConcat(xStrR.sStr(),xStrA.sStr());
  xStrR.pAsStrVal()->vSize();
  
  nnpTask->m_xDataStack.vPush(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sprint -- pop format string and use it for a sprintf                       */
/*----------------------------------------------------------------------------*/
void Task::vSprint(NNP_Task nnpTask)
{
  ENTRY("vSprint");
  
  s8K sFormat;
  if ( ! nnpTask->m_xDataStack.bPop(sFormat) )
    THROW_INVALID_DSTACK("sprint");
  
  s8K sString;
  
  nnpTask->vSprint(sFormat,sString);
  
  nnpTask->m_xDataStack.vPush(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* s256.init -- initialise an s256                                            */
/*----------------------------------------------------------------------------*/
void Task::vS256Init(NNP_Task nnpTask)
{
  ENTRY("vS256Init");
  
  Object* pPtrStrVal;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrStrVal) )
    THROW_INVALID_DSTACK("s256.init");
  
  StrVal* pStrVal = _pStrVal(pPtrStrVal);
  
  ASSERT(CN_STRVAL == pStrVal->eClass());
  
  new(pStrVal) StrVal(256);
  
  EXIT();
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* str.equal -- test if two strings are equal                                 */
/*----------------------------------------------------------------------------*/
void Task::vStrEqual(NNP_Task nnpTask)
{
  ENTRY("vStrEql");
  
  ValWsp xStringA;
  ValWsp xStringB;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(xStringB,xStringA) )
    THROW_INVALID_DSTACK("str.equal");
  
  i8 iEqual = String::bEqualTo(xStringA.sStr(),xStringB.sStr());
  
  nnpTask->m_xDataStack.vPush(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.length -- string length                                                    */
/*----------------------------------------------------------------------------*/
void Task::vStrLength(NNP_Task nnpTask)
{
  ENTRY("vStrLen");
  
  i8 iLength;
  s8K sString;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(sString) )
    THROW_INVALID_DSTACK("str.length");
  
  iLength = String::iLength(sString.a());
  
  nnpTask->m_xDataStack.vPush(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.random-- generate random string of given length                        */
/*----------------------------------------------------------------------------*/
void Task::vStrRandom(NNP_Task nnpTask)
{
  ENTRY("vStrRandom");
  
  i8 iLength;
  if ( ! nnpTask->m_xDataStack.bPop(iLength) )
    THROW_INVALID_DSTACK("str.random");
  
  s8K sRnd;
  
  sRnd = Random::sRandomString(iLength).a();
  
  nnpTask->m_xDataStack.vPush(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.right -- Returns the right n part of a string ( length str -- str )    */
/*----------------------------------------------------------------------------*/
void Task::vStrRight(NNP_Task nnpTask)
{
  ENTRY("vStrRight");
  
  i8 iLength;
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_xDataStack.bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.right");
  
  // validate length
  i8 iHasLength = String::iLength(sString.a());
  
  if ( iLength < 0)
    THROW("Invalid length for: str.right");

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.a() + iHasLength - iLength;
  
  String::vCopyLen(sSubStr.a(),pStart,iLength);
  
  nnpTask->m_xDataStack.vPush(sSubStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
