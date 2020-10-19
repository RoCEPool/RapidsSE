/*============================================================================*/
/* StrLib.hpp -- char* string utility                                         */
/*============================================================================*/
#pragma once

#include "eDC.h"

namespace Strings
{
  static const int C_NO_CASE = 0;
  static const int C_CASE    = 1;
  
  static const char C_NL = '\n';    // 0a is newline
  static const char C_CR = '\r';    // 0d is carriage return

  class StrLib
  {
  private:
    static const i8 C_STRING_ARRAY = 16 * C_1KB;
    
  public:
    static char* wAllocate(i8 nBytes);
    static char* wAllocate(const char* s);
    static void  vAppendChar(char** aaz, char c);
    static void  vAppendDecimal(char** aaz, i8 i);
    static void  vAppendHex(char** aaz, i8 i);
    static void  vAppendNull(char** aaz);
    static void  vAppendPad(char** aaz, const char* s, i8 nLength);
    static void  vAppendString(char** aaz, const char* s);
    static char* azAscii(i8 i_Char);
    
    static bool  bBackward(char** ps, char* pBegin, const char c);
    static i8    nBurstArgs(Stz* szaArgv[], Stz* szArgStr);

    static void  vClick(char* aArg);
    static bool  bClip(char* aArg, i8 iLen);
    static void  vComma(char* psDst, i8 iArg);
    static i8    iCompare(const char* pArg1, const char* pArg2);
    static i8    iCompare(const char* pArg1, const char* pArg2, i8 iCase);
    static i8    iCompare(const char* pArg1, const char* pArg2, i8 nBytes, i8 iCase);
    static void  vConcat(char* pWS, char cChar);
    static void  vConcat(char* psDst, const char* psArg);
    static void  vConcat(char* psDst, const char* psArg1, const char* psArg2);
    static char* wConcat(const char* s1, const char* s2);
    static void  vConcat(char* pWS, i8 i);
  
    static void  vConcat(Stz* szDst, Stz* szArg);
  
    static bool  bContains(char* s, const char cArg);
    static bool  bContains(char* s, const char* sArg);
    static i8    nCopy(char* psDst, const char* psSrc);
    static i8    nCopy_nonull(char* psDst, const char* psSrc);
    static void  vCopy(char* psDst, const char* psSrc);
    static void  vCopyLen(char* psDst, const char* psSrc, i8 nBytes);
    static void  vCopySplit(char* w1, char* w2, const char* sSrc, char cSplit);
    static void  vCopyTill(char* psDst, const char* psSrc, char ccTill);
    
    static i8    iDigitToI8(char cDigit);
    static void  vDump(const char* s, i8 nMax, const char* pSuffix);
    static char* sDupe(const char* s);
    static char* sDupeStripped(const char* s);
  
    static bool  bEachLine(char** ps, char* caLine);
    static bool  bEqualTo(const char* s1, const char* s2);
    static bool  bEqualTo(const char* s1, const char* s2, i8 iCase);
    static bool  bEnds(const char* sArg, const char cEnd);
    static bool  bEnds(char* sArg, char* sEnd);
    static void  vEscape(char* azDst, char* azSrc);
    static bool  bEscapeConvert(const char cArg,
                                char cEnclosedBy_Leading, char cEnclosedBy_Trailing,
                                char cEscapeChar, char cDelimiter, char cTerminator);
    static bool  bEscapeConvertDefault(const char cArg,
                                char cEscapeChar, char cDelimiter);
    static char  cEscapeConvert(const char cArg);
    static void  vEscapeDblQuote(const char* sArg, char* sEscaped);
    static char  cEscapeFor(const char cArg);
    static char  cEscapeInverse(const char cArg);
    static char  cEscapeProcessing(char** aaSrc, char cEscapeChar);
    static bool  bEscapedSpecial(const char cArg);
    static bool  bExtractArg(char** ps, char* pPath);
    static bool  bExtractBoolean(char** ps, i8& iBoolean);
    static bool  bExtractDate(char** ps, i8& iYear, i8& iMonth, i8& iDay);
    static bool  bExtractDate2(char** ps, i8& iYear, i8& iMonth, i8& iDay,
                               eDC* pDateComponents, char cDateSeparator);
    static bool  bExtractDay(char** ps, i8& iDay);
    static bool  bExtractDecimal(char** ps, i8& iDecimal, i8 iLength, i8 iScale);
    static bool  bExtractFloat(char** ps, d8& dFloat);
    static bool  bExtractFraction(char** ps, u8* u, i8 iDigits);
    static bool  bExtractInteger(char** ps, i8* i, bool bWithSign=false);
    static bool  bExtractInteger(char** ps, i8* i, i8& nDigits);
    static bool  bExtractInterval(char** ps, i8& iSecond, i8& iMilli);
    static bool  bExtractItem(char** ps, char* pItem);
    static bool  bExtractItem(char** ps, char* pItem, char cDelimiter);
    static bool  bExtractMonth(char** ps, i8& iMonth);
    static bool  bExtractName(char** aazCursor, char* azName);
    static bool  bExtractPath(char** ps, char* pPath);
    static bool  bExtractPort(char** ps, u2* pPort);
    static bool  bExtractString(char** ps, char* pString);
    static bool  bExtractTime(char** ps, i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro);
    static bool  bExtractTimestamp(char** ps, i8& iYear, i8& iMonth, i8& iDay,
                                   i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro);
    static bool  bExtractTimestamp2(char** ps, i8& iYear, i8& iMonth, i8& iDay,
                                   i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro,
                                    eDC* pDateComponents, char cDateSeparator);
    static bool  bExtractU8(char** ps, u8* u);
    static bool  bExtractU8(char** ps, u8* u, i8 iDigits);
    static bool  bExtractVarchar(char** ps,
                                 char* aVarchar,
                                 i8 iMaxLength,
                                 bool bIsEnclosed,
                                 char cEnclosedBy_Leading,
                                 char cEnclosedBy_Trailing,
                                 char cEscapeChar,
                                 char cFieldSeparator,
                                 char cRecordTerminator,
                                 bool bAllowNonPrintable,
                                 i8& nChars);
    static bool  bExtractVarcharDefault(char** ps,
                                        char* aVarchar,
                                        i8 iMaxLength,
                                        i8& nChars);
    static bool  bExtractWord(char** ps, char* pWord);
    static bool  bExtractWordString(char** ps, char* pWord);
    static bool  bExtractYear(char** ps, i8& iYear);
  
    static bool  bFind(char** ps, const char cArg);
    static bool  bFind(char** ps, const char cArg, i8 iOccurence);
    static bool  bFind(char** ps, const char* sArg);
    static bool  bFind(char** ps, const char* sArg, i8 iLenArg);
    static void  vFormatFraction(char* sDst, u8 uFraction, i8 iDigits);
    static void  vFree(char** ps);
    
    static i8    iGleanInt(char* azArg);
  
    static i8    iHexToInw(const char c);
    
    static bool  bIsAlpha(const char c);
    static bool  bIsAlphaDigit(const char c);
    static bool  bIsDigit(const char c);
    static bool  bIsEscapable(const char cArg);
    static bool  bIsNameBodyChar(const char c);
    static bool  bIsNameStartChar(const char c);
    static bool  bIsNumeric(char* azArg);
    static bool  bIsHex(const char c);
    static bool  bIsWhite(const char c);
   
    static char* acLast(const char* s);
    //static i8    nLength(const char* s);
    static i8    nLength(const char* s, const char cTerm, i8 iMax);
    
    static char* sNew(const char* sFormat, ...);
    static char* zNullTerminator(char* s);

    static i8    iOccurrence(char* ps, const char cArg);
    static bool  bOneOf(char** asaSet, char* sWord);
  
    static void  vPad(char* azDst, i8 iToLength, const char cWith);
    
    static void  vRemove(char* azArg, const char cLeading);
    static void  vRemoveChar(char* azDst, char* azSrc, const char cChr);
    static void  vRemovePrefix(char* azArg, char cIntro);
    static void  vRemoveSuffix(char* azArg, char cIntro);
    static void  vRemoveTailInt(char* azArg);
    static bool  bReplace(char* pString, const char* pThis, const char* pThat);
    static bool  bReplace(char* pDst, const char* pSrc,
                          const char* pThis, const char* pThat);
    static void  vReplaceTail(char* azArg, char* azTail);
    static void  vReverse(char* azDst, char* azSrc);
    static void  vRight(char* azDst, char* azSrc, i8 i_At);
    static void  vRotate(char* zArg, i8 nby);
    static void  vRuler(char* azDst, i8 iLength);

    static bool  bSame(const char* s1, const char* s2);
    static i8    nSize(const char* s);
    static void  vSkipDigits(char** ps);
    static bool  bSkipEOL(char** ps);
    static bool  bSkipLeading(char** ps, char cEnclosedBy_Leading);
    static bool  bSkipNonAlnum(char** ps);
    static bool  bSkipNonDigit(char** ps);
    static void  vSkipSpaces(char** ps);
    static bool  bSkipTrailing(char** ps,
                               bool bSawEnclosedBy_Leading,
                               char cEnclosedBy_Trailing);
    static void  vSkipWhite(char** ps);
    static void  vSplitSeries(char* azStr, char* azInt, char* sSrc);
    static void  vSprint(Stz* sDst, Stz* sFormat, ...);
    static void  vSprint(char* sDst, Stz* sFormat, ...);
    static void  vSprint(char* sDst, va_list args, const char* sFormat);
    static void  vSprintCat(Stz*  sDst, Stz* sFormat, ...);
    static void  vSprintCat(char* sDst, va_list args, const char* sFormat);
    static void  vStrip(char* s, char cTrailing);
    static void  vSubPath(char* azDst, char* azSrc);

    static char* aczTabLoc(char* azArg, i8 i_Tab, char cPad);
    static bool  bToBool(const char* s);
    static char  cToHex(i8 i);
    static i8    iToI8(const char* s);
    static void  vToLower(char* s);
    static void  vTopTail(char* azArg, char cIntro);
    static char* wToPointer(void* pArg, char* w);
    static void  vToString(char* azArg, i8 iArg);
    static char* wToString(d8 dArg, char* w);
    static char* wToString(i8 iArg, char* w);
    
    static bool  bValidateMonth(i8 iDay, i8 iMonth, i8 iYear);

    static i8    iWordFlip(char* aDst, char* aSrc);
    static i8    iWordFlipBack(char* aDst, char* aSrc);
  
  /*----------------------------------------------------------------------------*/
  /* iLength: get length in bytes of arg (not including terminating null)       */
  /*----------------------------------------------------------------------------*/
  inline static i8 nLength(const char* s)
  {
    _static_
    
    i8 iLen = (i8)strlen(s);
    
    return iLen;
  }


  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
