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
  vAddPrimitive("s256.init",    &vS256Init,      false);
  
  vAddPrimitive("str.cat",      &vStrCat,        false);
  vAddPrimitive("str.clip",     &vStrClip,       false);
  vAddPrimitive("str.equal",    &vStrEqual,      false);
  vAddPrimitive("str.escape",   &vStrEscape,     false);
  vAddPrimitive("str.firstCsv", &vStrFirstCsv,   false);
  vAddPrimitive("str.format",   &vStrFormat,     false);
  vAddPrimitive("str.left",     &vStrLeft,       false);
  vAddPrimitive("str.length",   &vStrLength,     false);
  vAddPrimitive("str.pad",      &vStrPad,        false);
  vAddPrimitive("str.prepend",  &vStrPrepend,    false);
  vAddPrimitive("str.random",   &vStrRandom,     false);
  vAddPrimitive("str.right",    &vStrRight,      false);
  vAddPrimitive("str.sub",      &vStrSub,        false);
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
  
  StrVal* pStrVal = _pStrVal(pPtrStrVal);
  
  ASSERT(CN_STRVAL == pStrVal->eClass());
  
  new(pStrVal) StrVal(256);
  
  EXIT();
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* str.cat -- str.cat two strings                                              */
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
  
  String::vConcat(xStrR.sStr(),xStrB.sStr());
  xStrR.pAsStrVal()->vSize();
  
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
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.clip");
  
  // validate length
  i8 iHasLength = String::iLength(sString.a());
  
  if ( iLength < 0 || llabs(iLength) >= iHasLength)
    THROW("Invalid length for: str.clip");
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.a();
  else
    pStart = sString.a() + iHasLength - 1 - llabs(iLength);
  
  String::vCopyLen(sSubStr.a(),pStart,llabs(iLength));
  
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
  
  i8 iEqual = String::bEqualTo(xStringA.sStr(),xStringB.sStr());
  
  nnpTask->m_pDataStack->vPush(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Task::vStrEscape(NNP_Task nnpTask)
{
  ENTRY("vStrEscape");
  
  ValWsp xArg;
  ValWsp xEscaped;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(xArg) )
    THROW_INVALID_DSTACK("str.escape");
  
  xEscaped = "";
  String::vEscape(xArg.sStr(),xEscaped.sStr());
  
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
  
  char* z = xStr.sStr();
  
  if (! String::bExtractItem(&z,sFirstValue.a()))
    sFirstValue = "";
  
  nnpTask->m_pDataStack->vPush(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Task::vStrFormat(NNP_Task nnpTask)
{
  ENTRY("vStrFormat");
  
  s8K sFormat;
  if ( ! nnpTask->m_pDataStack->bPop(sFormat) )
    THROW_INVALID_DSTACK("str.format");
  
  s8K sString;
  
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
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.left");
  
  // validate length
  i8 iHasLength = String::iLength(sString.a());
  
  if ( iLength < 0)
    THROW("Invalid length for: str.left");
  
  char* pStart = sString.a();
  
  String::vCopyLen(sSubStr.a(),pStart,iLength);
  
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
  s8K sString;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("str.length");
  
  iLength = String::iLength(sString.a());
  
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
  
  String::vPad(xStringA.sStr(),iLength,' ');
  xStringA.pAsStrVal()->vSize();
  
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
  
  s8K sRnd;
  
  sRnd = Random::sRandomString(iLength).a();
  
  nnpTask->m_pDataStack->vPush(sRnd);
  
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
  if ( ! nnpTask->m_pDataStack->bPop(sString,iLength) )
    THROW_INVALID_DSTACK("str.right");
  
  // validate length
  i8 iHasLength = String::iLength(sString.a());
  
  if ( iLength < 0)
    THROW("Invalid length for: str.right");

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.a() + iHasLength - iLength;
  
  String::vCopyLen(sSubStr.a(),pStart,iLength);
  
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
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_pDataStack->bPop(sString,iStart,iLength) )
    THROW_INVALID_DSTACK("str.sub");
  
  i8 iHasLength = String::iLength(sString.a());
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    THROW("Invalid start index for: str.sub");
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    THROW("Invalid length for: str.sub");
  
  char* pStart = sString.a() + iStart;
  
  String::vCopyLen(sSubStr.a(),pStart,iLength);
  
  nnpTask->m_pDataStack->vPush(sSubStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
