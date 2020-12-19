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
  vAddStep("str.gleanInt"_Stz,     &Step::vStrGleanInt);
  vAddStep("str.left"_Stz,         &Step::vStrLeft);
  vAddStep("str.length"_Stz,       &Step::vStrLength);
  vAddStep("str.prepend"_Stz,      &Step::vStrPrepend);
  vAddStep("str.random"_Stz,       &Step::vStrRandom);
  vAddStep("str.removePrefix"_Stz, &Step::vStrRemovePrefix);
  vAddStep("str.removeSuffix"_Stz, &Step::vStrRemoveSuffix);
  vAddStep("str.removeTailInt"_Stz,&Step::vStrRemoveTailInt);
  vAddStep("str.replace"_Stz,      &Step::vStrReplace);
  vAddStep("str.right"_Stz,        &Step::vStrRight);
  vAddStep("str.same"_Stz,         &Step::vStrSame);
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
  ENTRY();
 
  // (1) StzVal's cannot be declared as local variables
  //StzVal xStrval;  // uncommenting this line should produce a compile error
  
  // declaring a fixed length string
  s32 xString("A test"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.cat -- concat two strings                                              */
/*----------------------------------------------------------------------------*/
void Step::vStrCat(void)
{
  ENTRY();
  
  s48K sStrR;
  s48K sStrB;
  s48K sStrA;
  
  m_pDataStack->vPopStr(sStrB,__func__);

  m_pDataStack->vPopStr(sStrA,__func__);

  sStrR = sStrA;
  
  sStrR.vConcat(sStrB);
  
  m_pDataStack->vPushStr(sStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.clip -- returns left or right n part of a string ( length str -- str ) */
/*----------------------------------------------------------------------------*/
void Step::vStrClip(void)
{
  ENTRY();
  
  s48K sSubStr;
  
  // get the arguments
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  
  i8 iLength = m_pDataStack->iPopInt(__func__);

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
  
  m_pDataStack->vPushStr(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.equal -- test if two strings are equal                                 */
/*----------------------------------------------------------------------------*/
void Step::vStrEqual(void)
{
  ENTRY();
  
  CellSingle xStringA;
  m_pDataStack->vPopVal(xStringA,__func__);
  
  CellSingle xStringB;
  m_pDataStack->vPopVal(xStringB,__func__);

  // get the arguments
  
  // ray : 4 jun 2020 : this was a same test. made this a strict equality test
  i8 iEqual = StrLib::bEqualTo(xStringA.azChar(),
                               xStringB.azChar());
  
  m_pDataStack->vPushInt(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.escape -- put a '\\' before and '\"'                                   */
/*----------------------------------------------------------------------------*/
void Step::vStrEscape(void)
{
  ENTRY();
  
  s48K xArg;
  s48K xEscaped;
  
  // get the arguments
  m_pDataStack->vPopStr(xArg,__func__);
  
  StrLib::vEscapeDblQuote(xArg.azChar(),xEscaped.azChar());
  
  m_pDataStack->vPushStr(xEscaped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.firstCsv -- get first value from csv string                            */
/*----------------------------------------------------------------------------*/
void Step::vStrFirstCsv(void)
{
  ENTRY();
  
  s48K sStr;
  m_pDataStack->vPopStr(sStr,__func__);
  
  s48K sFirstValue;
  
  char* aValue = sStr.azChar();
  
  if (! StrLib::bExtractItem(&aValue,sFirstValue.azChar(), ','))
    sFirstValue = ""_Stz;
  
  m_pDataStack->vPushStr(sFirstValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.format -- pop format string and use it for a sprintf                   */
/*----------------------------------------------------------------------------*/
void Step::vStrFormat(void)
{
  ENTRY();
  
  s48K sFormat;
  m_pDataStack->vPopStr(sFormat,__func__);
  
  s48K sString;
  
  m_pTask->vValSprint(sFormat,sString);
  
  m_pDataStack->vPushStr(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.gleanInt -- glean int from string                                      */
/*----------------------------------------------------------------------------*/
void Step::vStrGleanInt(void)
{
  ENTRY();
  
  s48K sArg;
  m_pDataStack->vPopStr(sArg,__func__);
  
  i8 iInt = StrLib::iGleanInt(sArg.azChar());
  
  m_pDataStack->vPushInt(iInt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.left -- returns left n part of a string ( length str -- str )          */
/*----------------------------------------------------------------------------*/
void Step::vStrLeft(void)
{
  ENTRY();
  
  s48K sSubStr;
  
  // get the arguments
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  
  i8 iLength = m_pDataStack->iPopInt(__func__);

  // validate length
  i8 iHasLength = sString.nLength();
  
  if ( iLength < 0)
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Invalid length for: str.left"_Stz);
  
  char* pStart = sString.azChar();
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,iLength);
  
  m_pDataStack->vPushStr(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.length -- string length                                                */
/*----------------------------------------------------------------------------*/
void Step::vStrLength(void)
{
  ENTRY();
  
  i8 iLength;
  s48K sString;
  
  // get the arguments
  m_pDataStack->vPopStr(sString,__func__);
  
  iLength = sString.nLength();
  
  m_pDataStack->vPushInt(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.prepend -- prepend top string before second string                     */
/*----------------------------------------------------------------------------*/
void Step::vStrPrepend(void)
{
  ENTRY();
  
  s48K xStrR;
  s48K xStrB;
  s48K xStrA;
  
  //! the use of A and B here doesnt seem intuitive
  CellSingle xValB;
  m_pDataStack->vPopVal(xValB,__func__);
 
  CellSingle xValA;
  m_pDataStack->vPopVal(xValA,__func__);

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
  
  m_pDataStack->vPushStr(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.random-- generate random string of given length                        */
/*----------------------------------------------------------------------------*/
void Step::vStrRandom(void)
{
  ENTRY();
  
  i8 iLength = m_pDataStack->iPopInt(__func__);
  
  s256 sRnd;
  
  RndLib::vString(sRnd,iLength);
  
  m_pDataStack->vPushStr(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.removePrefix -- remove prefix from string                              */
/*----------------------------------------------------------------------------*/
void Step::vStrRemovePrefix(void)
{
  ENTRY();
  
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  
  s32 sIntro;
  m_pDataStack->vPopStr(sIntro,__func__);
  
  StrLib::vRemovePrefix(sString.azChar(), * sIntro.azChar());
  
  m_pDataStack->vPushStr(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.removeSuffix -- remove suffix from string                              */
/*----------------------------------------------------------------------------*/
void Step::vStrRemoveSuffix(void)
{
  ENTRY();
  
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  
  s32 sIntro;
  m_pDataStack->vPopStr(sIntro,__func__);
  
  StrLib::vRemoveSuffix(sString.azChar(), * sIntro.azChar());
  
  m_pDataStack->vPushStr(sString);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* remove tail int from string                                                */
/*----------------------------------------------------------------------------*/
void Step::vStrRemoveTailInt(void)
{
  ENTRY();
  
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  
  StrLib::vRemoveTailInt(sString.azChar());
  
  m_pDataStack->vPushStr(sString);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.replace-- replace this with that within string                         */
/*----------------------------------------------------------------------------*/
void Step::vStrReplace(void)
{
  ENTRY();
  
  s48K sString;
  s48K sThat;
  s48K sThis;
  
  m_pDataStack->vPopStr(sString,__func__);

  m_pDataStack->vPopStr(sThat,__func__);
  
  m_pDataStack->vPopStr(sThis,__func__);
  
  s48K sReplaced;
  
  StrLib::bReplace(sReplaced.azChar(),sString.azChar(),sThis.azChar(),sThat.azChar());
  
  m_pDataStack->vPushStr(sReplaced);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.right -- Returns the right n part of a string ( length str -- str )    */
/*----------------------------------------------------------------------------*/
void Step::vStrRight(void)
{
  ENTRY();
  
  s48K sSubStr;
  
  // get the arguments
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  i8 iLength = m_pDataStack->iPopInt(__func__);

  // validate length
  i8 iHasLength = sString.nLength();
  
  if ( iLength < 0)
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Invalid length for: str.right"_Stz);

  if (iLength > iHasLength)
    iLength = iHasLength;
  
  char* pStart = sString.azChar() + iHasLength - iLength;
  
  StrLib::vCopyLen(sSubStr.azChar(),pStart,iLength);
  
  m_pDataStack->vPushStr(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str.same -- test if two strings are same                                   */
/*----------------------------------------------------------------------------*/
void Step::vStrSame(void)
{
  ENTRY();
  
  CellSingle xStringA;
  m_pDataStack->vPopVal(xStringA,__func__);
  
  CellSingle xStringB;
  m_pDataStack->vPopVal(xStringB,__func__);

  // get the arguments
  
  i8 iEqual = StrLib::bSame(xStringA.azChar(),
                            xStringB.azChar());
  
  m_pDataStack->vPushInt(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( length start str == substr )                         */
/*----------------------------------------------------------------------------*/
void Step::vStrSub(void)
{
  ENTRY();
  
  s48K sSubStr;
  
  // get the arguments
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  i8 iStart = m_pDataStack->iPopInt(__func__);
  i8 iLength = m_pDataStack->iPopInt(__func__);

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
  
  m_pDataStack->vPushStr(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tailInt -- extract (possible) int at end of string                         */
/*----------------------------------------------------------------------------*/
void Step::vStrTailInt(void)
{
  ENTRY();
  
  // get the string
  s48K sString;
  m_pDataStack->vPopStr(sString,__func__);
  
  // get last char in string
  char* acLast = StrLib::acLast(sString.azChar());
  
  // return false if not a digit
  if ( ! StrLib::bIsDigit(*acLast) ) {
    m_pDataStack->vPushInt(false);
    return;
  }
  
  // move backward to start of digit sequence
  char* acDigit = acLast;
  while (StrLib::bIsDigit(*acDigit))
    acDigit--;
  acDigit++;
  
  // step forward through each digit, computing int
  i8 iInt = 0;
  while (StrLib::bIsDigit(*acDigit))
    iInt = (iInt * 10) + (*acDigit++ - '0');
  
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
  ENTRY();
  
  // get the path string
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);
  
  i8 nStrings = 0;
  
  s32 sDicNm;
  
  char* azPath  = sPath.azChar() + 1;
  char* azDicNm = sDicNm.azChar();
  
  for (;; nStrings++) {
    
    if (! StrLib::bExtractName(&azPath,azDicNm))
      break;
    
    m_pAuxStack->vPushStr(sDicNm);
  }
  
  for (i8 iMove = 0; iMove < nStrings; iMove++) {
    
    m_pAuxStack->vPopStr(sDicNm,__func__);
    m_pDataStack->vPushStr(sDicNm);
  }

  m_pDataStack->vPushInt(nStrings);

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
