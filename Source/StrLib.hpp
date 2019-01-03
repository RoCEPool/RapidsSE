/*============================================================================*/
/* StrLib.hpp -- char* string utility                                         */
/*============================================================================*/

#pragma once

namespace Datums {
  class Formatters;
}

namespace Base
{
  enum eDC // date components
  {
  /* 0 */ DC_NONE=0,
  /* 1 */ DC_YYYY,
  /* 2 */ DC_MM,
  /* 3 */ DC_DD
  };
  
  static Stz* aDC[]
  {
  /* 0 */ "none"_Stz,
  /* 1 */ "YYYY"_Stz,
  /* 2 */ "MM"_Stz,
  /* 3 */ "DD"_Stz
  };
  
  static u8 iDaysPerMonth[12]     = { 31,28,31,30,31,30,31,31,30,31,30,31};
  static u8 iDaysPerMonthLeap[12] = { 31,29,31,30,31,30,31,31,30,31,30,31};
  
  static const int C_NO_CASE = 0;
  static const int C_CASE    = 1;
  
  static const char C_NL = '\n';    // 0a is newline
  static const char C_CR = '\r';    // 0d is carriage return
  
  static const char C_COMMA = ',';
  
  class StrLib
  {
  private:
    static const i8 C_STRING_ARRAY = 16 * C_1KB;
    
  public:
    static char* wAllocate(i8 nBytes);
    static char* wAllocate(const char* s);
    static void  vAppendChar(char** pz, char c);
    static void  vAppendDecimal(char** pz, i8 i);
    static void  vAppendHex(char** pz, i8 i);
    static void  vAppendNull(char** pz);
    static void  vAppendString(char** pz, const char* s);
    
    static bool  bBackward(const char** ps, char* pBegin, const char c);

    static i8    iCompare(const char* pArg1, const char* pArg2);
    static bool  iCompare(const char* pArg1, const char* pArg2, i8 iCase);
    static bool  iCompare(const char* pArg1, const char* pArg2, i8 nBytes, i8 iCase);
    static void  vConcat(char* pWS, char cChar);
    static void  vConcat(char* psDst, const char* psArg);
    static void  vConcat(char* psDst, const char* psArg1, const char* psArg2);
    static char* wConcat(const char* s1, const char* s2);
    static void  vConcat(char* pWS, i8 i);
    static bool  bContains(char* s, const char cArg);
    static bool  bContains(char* s, const char* sArg);
    static void  vCopy(char* psDst, const char* psSrc);
    static void  vCopyLen(char* psDst, const char* psSrc, i8 nBytes);
    static void  vCopySplit(char* w1, char* w2, const char* sSrc, char cSplit);
    static void  vCopyTill(char* psDst, const char* psSrc, char ccTill);
  
    static i8    iDigitToI8(char cDigit);
    static void  vDump(const char* s, i8 nMax, const char* pSuffix);
    static char* sDupe(const char* s);
    static char* sDupeStripped(const char* s);
    
    static bool  bEnds(const char* sArg, const char cEnd);
    static bool  bEnds(const char* sArg, const char* sEnd);
    static void  vEscape(const char* sArg, char* sEscaped);
    static bool  bEscapeConvert(const char cArg,
                                char cEnclosedBy_Leading, char cEnclosedBy_Trailing,
                                char cEscapeChar, char cDelimiter, char cTerminator);
    static bool  bEscapeConvertDefault(const char cArg,
                                char cEscapeChar, char cDelimiter);
    static char  cEscapeConvert(const char cArg);
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
                                 char* aVarchar, i8 iMaxLength,
                                 char cEnclosedBy_Leading,
                                 char cEnclosedBy_Trailing,
                                 char cEscapeChar,
                                 char cFieldSeparator,
                                 char cRecordTerminator);
    static bool  bExtractVarchar2(char** ps,
                                  char* aVarchar, i8 iMaxLength,
                                  bool bIsEnclosed,
                                  char cEnclosedBy_Leading,
                                  char cEnclosedBy_Trailing,
                                  char cEscapeChar,
                                  char cFieldSeparator,
                                  char cRecordTerminator,
                                  bool bAllowNonPrintable);
    static bool  bExtractVarcharDefault2(char** ps,
                                  char* aVarchar, i8 iMaxLength,
                                  char cEscapeChar,
                                  char cFieldSeparator,
                                  bool bAllowNonPrintable);
    static bool  bExtractWord(char** ps, char* pWord);
    static bool  bExtractWordString(char** ps, char* pWord);
    static bool  bExtractYear(char** ps, i8& iYear);
  
    static bool  bFind(char** ps, const char cArg);
    static bool  bFind(char** ps, const char cArg, i8 iOccurence);
    static bool  bFind(char** ps, const char* sArg);
    static bool  bFind(char** ps, const char* sArg, i8 iLenArg);
    static void  vFormatFraction(char* sDst, u8 uFraction, i8 iDigits);
    static void  vFree(char** ps);
    
    static i8    iHexToInw(const char c);
    
    static bool  bIsAlpha(const char c);
    static bool  bIsAlphaDigit(const char c);
    static bool  bIsAlphaDigitUnder(const char c);
    static bool  bIsAlphaUnder(const char c);
    static bool  bIsDigit(const char c);
    static bool  bIsNumeric(char* ps);
    static bool  bIsHex(const char c);
    static bool  bIsWhite(const char c);
   
    static char* sLast(const char* s);
    static i8    iLength(const char* s);
    static i8    iLength(const char* s, const char cTerm, i8 iMax);
    
    static bool  bEqualTo(const char* s1, const char* s2);
    static bool  bEqualTo(const char* s1, const char* s2, i8 iCase);
    
    static char* sNew(const char* sFormat, ...);

    static void  vPad(char* w, i8 iToLength, const char cWith);
    
    static void  vRemove(char* w, const char cLeading);
    static bool  bReplace(char* pString, const char* pThis, const char* pThat);
    static bool  bReplace(char* pDst, const char* pSrc,
                          const char* pThis, const char* pThat);

    static bool  bSame(const char* s1, const char* s2);
    static i8    iSize(const char* s);
    static void  vSkipDigits(char** ps);
    static bool  bSkipEOL(char** ps);
    static bool  bSkipLeading(char** ps, char cEnclosedBy_Leading);
    static bool  bSkipNonAlnum(char** ps);
    static bool  bSkipNonDigit(char** ps);
    static void  vSkipSpaces(char** ps);
    static bool  bSkipTrailing(char** ps, bool bSawEnclosedBy_Leading, char cEnclosedBy_Trailing);
    static void  vSkipWhite(char** ps);
    static void  vSprint(Stz* sDst, Stz* sFormat, ...);
    static void  vSprint(char* sDst, Stz* sFormat, ...);
    static void  vSprint(char* sDst, va_list args, const char* sFormat);
    static void  vSprintCat(char* sDst, const char* sFormat, ...);
    static void  vSprintCat(char* sDst, va_list args, const char* sFormat);
    static void  vStrip(char* s, char cTrailing);

    static bool  bToBool(const char* s);
    static char  cToHex(i8 i);
    static i8    iToI8(const char* s);
    static void  vToLower(char* s);
    static char* wToPointer(void* pArg, char* w);
    static char* wToString(d8 dArg, char* w);
    static char* wToString(i8 iArg, char* w);
    
    static char* zTerminator(char* s);
  
    static bool  bValidateMonth(i8 iDay, i8 iMonth, i8 iYear);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
