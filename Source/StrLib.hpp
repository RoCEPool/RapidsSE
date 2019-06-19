/*============================================================================*/
/* StrLib.hpp -- char* string utility                                         */
/*============================================================================*/

#pragma once

namespace Chunks { class Formatters; }

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
  
  static i8 iDaysPerMonth[12]     = { 31,28,31,30,31,30,31,31,30,31,30,31};
  static i8 iDaysPerMonthLeap[12] = { 31,29,31,30,31,30,31,31,30,31,30,31};
  
  static const int C_NO_CASE = 0;
  static const int C_CASE    = 1;
  
  static const char C_NL = '\n';    // 0a is newline
  static const char C_CR = '\r';    // 0d is carriage return
  
  static const char C_COMMA = ',';
  
  static Stz* szaAscii[128] = {
    "NUL"_Stz,"SOH"_Stz,"STX"_Stz,"ETX"_Stz,"EOT"_Stz,"ENQ"_Stz,"ACK"_Stz,"BEL"_Stz,
     "BS"_Stz, "HT"_Stz, "LF"_Stz, "VT"_Stz, "FF"_Stz, "CR"_Stz, "SO"_Stz, "SI"_Stz,
    "DLE"_Stz,"DC1"_Stz,"DC2"_Stz,"DC3"_Stz,"DC4"_Stz,"NAK"_Stz,"SYN"_Stz,"ETB"_Stz,
    "CAN"_Stz, "EM"_Stz,"SUB"_Stz,"ESC"_Stz, "FS"_Stz, "GS"_Stz, "RS"_Stz, "US"_Stz,
      " "_Stz,  "!"_Stz, "\""_Stz,  "#"_Stz,  "$"_Stz,  "%"_Stz,  "&"_Stz,  "'"_Stz,
      "("_Stz,  ")"_Stz,  "*"_Stz,  "+"_Stz,  ","_Stz,  "-"_Stz,  "."_Stz,  "/"_Stz,
      "0"_Stz,  "1"_Stz,  "2"_Stz,  "3"_Stz,  "4"_Stz,  "5"_Stz,  "6"_Stz,  "7"_Stz,
      "8"_Stz,  "9"_Stz,  ":"_Stz,  ";"_Stz,  "<"_Stz,  "="_Stz,  ">"_Stz,  "?"_Stz,
      "@"_Stz,  "A"_Stz,  "B"_Stz,  "C"_Stz,  "D"_Stz,  "E"_Stz,  "F"_Stz,  "G"_Stz,
      "H"_Stz,  "I"_Stz,  "J"_Stz,  "K"_Stz,  "L"_Stz,  "M"_Stz,  "N"_Stz,  "O"_Stz,
      "P"_Stz,  "Q"_Stz,  "R"_Stz,  "S"_Stz,  "T"_Stz,  "U"_Stz,  "V"_Stz,  "W"_Stz,
      "X"_Stz,  "Y"_Stz,  "Z"_Stz,  "["_Stz,  "\\"_Stz, "]"_Stz,  "^"_Stz,  "_"_Stz,
      "`"_Stz,  "a"_Stz,  "b"_Stz,  "c"_Stz,  "d"_Stz,  "e"_Stz,  "f"_Stz,  "g"_Stz,
      "h"_Stz,  "i"_Stz,  "j"_Stz,  "k"_Stz,  "l"_Stz,  "m"_Stz,  "n"_Stz,  "o"_Stz,
      "p"_Stz,  "q"_Stz,  "r"_Stz,  "s"_Stz,  "t"_Stz,  "u"_Stz,  "v"_Stz,  "w"_Stz,
      "x"_Stz,  "y"_Stz,  "z"_Stz,  "{"_Stz,  "|"_Stz,  "}"_Stz,  "~"_Stz,"DEL"_Stz
  };
  
  enum eOC // other characters
  {
  /*  0 */ OC_NONE=0,
  /*    */ OC_1,
  /*    */ TD_ENUMS,
  };
  
  static Stz* aOC[]
  {
  /*  0 */ "NONE"_Stz,
  /*    */ "⬛️"_Stz,
  /*    */ "⬜️"_Stz,
  /*    */ "🛑"_Stz,
  /*    */ "🅰️"_Stz,
  /*    */ "🅱️"_Stz,
  /*    */ "🔵"_Stz,
  /*    */ "⚫️"_Stz,
  /*    */ "⚪️"_Stz,
  /*    */ "🔴"_Stz,
  /*    */ "⭕️"_Stz,
  /*    */ "❌"_Stz,
  /*    */ "💢"_Stz,
  /*    */ "↙️"_Stz,
  /*    */ "↘️"_Stz,
  /*    */ "🔷"_Stz,
  /*    */ "🔶"_Stz,
  /*    */ "♠️"_Stz,
  /*    */ "♣️"_Stz,
  /*    */ "♥️"_Stz,
  /*    */ "♦️"_Stz,
  /*    */ "🆗"_Stz,
  /*    */ "0️⃣"_Stz,
  /*    */ "9️⃣"_Stz,
  /*    */ "#️⃣"_Stz,
  /*    */ "*️⃣"_Stz,
  /*    */ "📈"_Stz,
  /*    */ "📉"_Stz,
  /*    */ "✅"_Stz,
  /*    */ "✅"_Stz,
  /*    */ "❎"_Stz,
  /*    */ "✳️"_Stz,
  /*    */ "🚹"_Stz,
  /*    */ "🚺"_Stz,
  /*    */ "⚛️"_Stz,
  /*    */ "❗️"_Stz,
  /*    */ "‼️"_Stz,
  /*    */ "❓"_Stz,
  /*    */ "⁉️"_Stz,
  /*    */ "➕"_Stz,
  /*    */ "➖"_Stz,
  /*    */ "➗"_Stz,
  /*    */ "✖️"_Stz,
  /*    */ "♾"_Stz,
  /*    */ "💲"_Stz,
  /*    */ "ENUMS"_Stz,
  };

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
    static void  vAppendPad(char** pz, const char* s, i8 nLength);
    static void  vAppendString(char** pz, const char* s);
    static char* azAscii(i8 i_Char);
    
    static bool  bBackward(const char** ps, char* pBegin, const char c);

    static void  vClick(char* aArg);
    static void  vComma(char* psDst, i8 iArg);
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
    static i8    nCopy(char* psDst, const char* psSrc);
    static void  vCopy(char* psDst, const char* psSrc);
    static void  vCopyLen(char* psDst, const char* psSrc, i8 nBytes);
    static void  vCopySplit(char* w1, char* w2, const char* sSrc, char cSplit);
    static void  vCopyTill(char* psDst, const char* psSrc, char ccTill);
  
    static i8    iDigitToI8(char cDigit);
    static void  vDump(const char* s, i8 nMax, const char* pSuffix);
    static char* sDupe(const char* s);
    static char* sDupeStripped(const char* s);
  
    static bool  bEachLine(char** ps, char* caLine);
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
    static void  vReverse(char* azDst, char* azSrc);
    static void  vRotate(char* zArg, i8 nby);

    static bool  bSame(const char* s1, const char* s2);
    static i8    iSize(const char* s);
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
    static void  vToString(char* psDst, i8 iArg);
    static char* wToString(d8 dArg, char* w);
    static char* wToString(i8 iArg, char* w);
    
    static char* zTerminator(char* s);
  
    static bool  bValidateMonth(i8 iDay, i8 iMonth, i8 iYear);

    static i8    iWordFlip(char* aDst, char* aSrc);
    static i8    iWordFlipBack(char* aDst, char* aSrc);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
