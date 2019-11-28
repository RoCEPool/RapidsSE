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
  vAddStep("str.toStrVec"_Stz,     &Step::vStrToStrVec);
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* str.algebra -- string algebra tests                                        */
/*----------------------------------------------------------------------------*/
void Step::vStrAlgebra(void)
{
  ENTRY(); // ENTRY("vStrAlgebra"_Stz);
 
  // (1) StrVal's cannot be declared as local variables
  //StrVal xStrval;  // uncommenting this line should produce a compile error
  
  // declaring a fixed length string
  s32 xString("A test"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.cat -- concat two strings                                              */
/*----------------------------------------------------------------------------*/
void Step::vStrCat(void)
{
  ENTRY(); // ENTRY("vStrCat"_Stz);
  
  s48K sStrR;
  s48K sStrB;
  s48K sStrA;
  
  if ( ! m_pDataStack->bPopS48K(sStrB,sStrA) )
    THROW_DSTACK();
  
  sStrR = sStrA;
  
  sStrR.vConcat(sStrB);
  
  m_pDataStack->vPushS48K(sStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.clip -- returns left or right n part of a string ( length str -- str ) */
/*----------------------------------------------------------------------------*/
void Step::vStrClip(void)
{
  ENTRY(); // ENTRY("vStrClip"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! m_pDataStack->bPopS48K(sString,iLength) )
    THROW_DSTACK();
  
  // validate length
  i8 iHasLength = sString.nLength();
  
  if ( iLength < 0 || llabs(iLength) >= iHasLength)
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Invalid length for: str.clip"_Stz);
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.azChar();
  else
    pStart = sString.azChar() + iHasLength - 1 - llabs(iLength);
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,llabs(iLength));
  
  m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.equal -- test if two strings are equal                                 */
/*----------------------------------------------------------------------------*/
void Step::vStrEqual(void)
{
  ENTRY(); // ENTRY("vStrEql"_Stz);
  
  GenVar xStringA;
  GenVar xStringB;
  
  // get the arguments
  if ( ! m_pDataStack->bPopVal(xStringB,xStringA) )
    THROW_DSTACK();
  
  i8 iEqual = StrLib::bEqualTo(xStringA.azChar(),
                               xStringB.azChar(),
                               C_NO_CASE);
  
  m_pDataStack->vPushInt(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Step::vStrEscape(void)
{
  ENTRY(); // ENTRY("vStrEscape"_Stz);
  
  s48K xArg;
  s48K xEscaped;
  
  // get the arguments
  if ( ! m_pDataStack->bPopS48K(xArg) )
    THROW_DSTACK();
  
  StrLib::vEscape(xArg.azChar(),xEscaped.azChar());
  
  m_pDataStack->vPushS48K(xEscaped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.firstCsv -- get first value from csv string                            */
/*----------------------------------------------------------------------------*/
void Step::vStrFirstCsv(void)
{
  ENTRY(); // ENTRY("vStrFirstCsv"_Stz);
  
  s48K sStr;
  if ( ! m_pDataStack->bPopS48K(sStr) )
    THROW_DSTACK();
  
  s48K sFirstValue;
  
  char* aValue = sStr.azChar();
  
  if (! StrLib::bExtractItem(&aValue,sFirstValue.azChar(), ','))
    sFirstValue = ""_Stz;
  
  m_pDataStack->vPushS48K(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Step::vStrFormat(void)
{
  ENTRY(); // ENTRY("vStrFormat"_Stz);
  
  s48K sFormat;
  if ( ! m_pDataStack->bPopS48K(sFormat) )
    THROW_DSTACK();
  
  s48K sString;
  
  m_pTask->vSprint(sFormat,sString);
  
  m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.left -- returns left n part of a string ( length str -- str )          */
/*----------------------------------------------------------------------------*/
void Step::vStrLeft(void)
{
  ENTRY(); // ENTRY("vClip"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! m_pDataStack->bPopS48K(sString,iLength) )
    THROW_DSTACK();
  
  // validate length
  i8 iHasLength = sString.nLength();
  
  if ( iLength < 0)
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Invalid length for: str.left"_Stz);
  
  char* pStart = sString.azChar();
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,iLength);
  
  m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.length -- string length                                                */
/*----------------------------------------------------------------------------*/
void Step::vStrLength(void)
{
  ENTRY(); // ENTRY("vStrLen"_Stz);
  
  i8 iLength;
  s48K sString;
  
  // get the arguments
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();
  
  iLength = sString.nLength();
  
  m_pDataStack->vPushInt(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.prepend -- prepend top string before second string                     */
/*----------------------------------------------------------------------------*/
void Step::vStrPrepend(void)
{
  ENTRY(); // ENTRY("vStrPrepend"_Stz);
  
  s48K xStrR;
  s48K xStrB;
  s48K xStrA;
  
  GenVar xValA;
  GenVar xValB;
  
  if ( ! m_pDataStack->bPopVal(xValB,xValA) )
    THROW_DSTACK();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first string A
  
  // if received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    ASSERT(0);
  }
  else {
    xStrA = xValA.szStz();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // second string B

  // if received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    ASSERT(0);
  }
  else {
    xStrB = xValB.szStz();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // prepend
  
  xStrR = xStrB;
  
  xStrR.vConcat(xStrA);
  
  m_pDataStack->vPushS48K(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.random-- generate random string of given length                        */
/*----------------------------------------------------------------------------*/
void Step::vStrRandom(void)
{
  ENTRY(); // ENTRY("vStrRandom"_Stz);
  
  i8 iLength;
  if ( ! m_pDataStack->bPopInt(iLength) )
    THROW_DSTACK();
  
  s256 sRnd;
  
  Random::vString(sRnd,iLength);
  
  m_pDataStack->vPushS256(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.removePrefix -- remove prefix from string                              */
/*----------------------------------------------------------------------------*/
void Step::vStrRemovePrefix(void)
{
  ENTRY(); // ENTRY("vStrRemovePrefix"_Stz);
  
  s48K sString;
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();
  
  s32 sIntro;
  if ( ! m_pDataStack->bPopStr(sIntro) )
    THROW_DSTACK();
  
  StrLib::vRemovePrefix(sString.azChar(), * sIntro.azChar());
  
  m_pDataStack->vPushS48K(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.removeSuffix -- remove suffix from string                              */
/*----------------------------------------------------------------------------*/
void Step::vStrRemoveSuffix(void)
{
  ENTRY(); // ENTRY("vStrRemoveSuffix"_Stz);
  
  s48K sString;
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();
  
  s32 sIntro;
  if ( ! m_pDataStack->bPopStr(sIntro) )
    THROW_DSTACK();
  
  StrLib::vRemoveSuffix(sString.azChar(), * sIntro.azChar());
  
  m_pDataStack->vPushS48K(sString);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.replace-- replace this with that within string                         */
/*----------------------------------------------------------------------------*/
void Step::vStrReplace(void)
{
  ENTRY(); // ENTRY("vStrReplace"_Stz);
  
  s48K sString;
  s48K sThat;
  s48K sThis;
  
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();

  if ( ! m_pDataStack->bPopS48K(sThat) )
    THROW_DSTACK();
  
  if ( ! m_pDataStack->bPopS48K(sThis) )
    THROW_DSTACK();
  
  s48K sReplaced;
  
  StrLib::bReplace(sReplaced.azChar(),sString.azChar(),sThis.azChar(),sThat.azChar());
  
  m_pDataStack->vPushS48K(sReplaced);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.right -- Returns the right n part of a string ( length str -- str )    */
/*----------------------------------------------------------------------------*/
void Step::vStrRight(void)
{
  ENTRY(); // ENTRY("vStrRight"_Stz);
  
  i8 iLength;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! m_pDataStack->bPopS48K(sString,iLength) )
    THROW_DSTACK();
  
  // validate length
  i8 iHasLength = sString.nLength();
  
  if ( iLength < 0)
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Invalid length for: str.right"_Stz);

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.azChar() + iHasLength - iLength;
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,iLength);
  
  m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( length start str == substr )                         */
/*----------------------------------------------------------------------------*/
void Step::vStrSub(void)
{
  ENTRY(); // ENTRY("vStrSub"_Stz);
  
  i8 iLength;
  i8 iStart;
  s48K sString;
  s48K sSubStr;
  
  // get the arguments
  if ( ! m_pDataStack->bPopS48K(sString,iStart,iLength) )
    THROW_DSTACK();
  
  i8 iHasLength = sString.nLength();
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Invalid start index of %lld for: str.sub"_Stz,
           iStart);
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Invalid length of %lld for: str.sub"_Stz,
           iLength);
  
  char* pStart = sString.azChar() + iStart;
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,iLength);
  
  m_pDataStack->vPushS48K(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tailInt -- extract (possible) int at end of string                         */
/*----------------------------------------------------------------------------*/
void Step::vStrTailInt(void)
{
  ENTRY(); // ENTRY("vStrTailInt"_Stz);
  
  // get the string
  s48K sString;
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();
  
  // get last char in string
  char* aLast = StrLib::sLast(sString.azChar());
  
  // return false if not a digit
  if ( ! StrLib::bIsDigit(*aLast) ) {
    m_pDataStack->vPushInt(false);
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
  m_pDataStack->vPushInt(iInt);
  m_pDataStack->vPushInt(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.toStrVec -- convert a (path) string to a str vector                    */
/*----------------------------------------------------------------------------*/
void Step::vStrToStrVec(void)
{
  ENTRY(); // ENTRY("vStrToStrVec"_Stz);
  
  // get the path string
  s256 sPath;
  if ( ! m_pDataStack->bPopStr(sPath) )
    THROW_DSTACK();
  
  i8 nStrings = 0;
  
  s32 sDicNm;
  
  char* azPath  = sPath.azChar() + 1;
  char* azDicNm = sDicNm.azChar();
  
  for (;; nStrings++) {
    
    if (! StrLib::bExtractName(&azPath,azDicNm))
      break;
    
    m_pAuxStack->vPushS32(sDicNm);
  }
  
  for (i8 iMove = 0; iMove < nStrings; iMove++) {
    
    RASSERT(m_pAuxStack->bPopStr(sDicNm));
    m_pDataStack->vPushS32(sDicNm);
  }

  m_pDataStack->vPushInt(nStrings);

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
