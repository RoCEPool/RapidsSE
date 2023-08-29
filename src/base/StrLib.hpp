/*============================================================================*/
/* StrLib.hpp -- char* string utility                                         */
/*============================================================================*/
#pragma once

#include <iostream>
#include <string.h>

#include "eDC.h"

namespace BASE
{
  static const int C_NO_CASE = 0;
  static const int C_CASE    = 1;
  
  static const char C_NL = '\n';    // 0a is newline
  static const char C_CR = '\r';    // 0d is carriage return

  class StrLib
  {
  private:
    static const i8 C_STRING_ARRAY = 16 * C_1KB;

    // Checks if there is sufficient buffer space & optionally sets the
    // callerResult if not null. Otherwise reports the truncation in the log file.
    // Returns true if everything is ok or false if a string truncation will occur
    inline static bool bCheckBufferSpace(const char* azFuncName, i8 iReqBufSize, i8 iDstBufSize, bool* pbCallerResult);

    // Record internally that a string truncation has occurred
    inline static void vReportStringTruncation(const char* azFuncName, i8 iReqBufSize, i8 iDstBufSize);
    
  public:                                                                                                               // SAFE?
    static void  vAppendChar(char** aazDstCrs, const char* azDstLastChar, char cChar, bool* pbResult = nullptr);        // yes
    static void  vAppendDecimal(char** aazDstCrs, const char* azDstLastChar, i8 iDecVal, bool* pbResult = nullptr);     // yes
    static void  vAppendHex(char** aazDstCrs, const char* azDstLastChar, i8 iDecVal, bool* pbResult = nullptr);         // yes
    static void  vAppendNull(char** aazDstCrs, const char* azDstLastChar, bool* pbResult = nullptr);                    // yes
    static void  vAppendPad(char** aazDstCrs, const char* psDst, i8 iDstBufSize, i8 nPadLen, bool* pbResult = nullptr); // yes
    static void  vAppendString(char** aazDstCrs, const char* azDstLastChar,
                               const char* pSrc, bool* pbResult = nullptr);                                             // yes
    static void  vAppendString(char** aazDstCrs, const char* azDstLastChar,
                               const char* pSrc, i8* piLineNo, bool* pbResult = nullptr);                               // yes
    static char* azAscii(i8 i_Char);                                                                                    // yes
    
    static bool  bBackward(char** ppsSrc, const char* psBegin, const char cUntil);                                            // yes
    static bool  bBeginsWith(const char* psSrc, const char* psArg);                                                     // yes

    static Stz*  szCharToStr(unsigned char cArg);                                                                       // yes
    static void  vCheckIsNot(char cArg, char cIsNot);                                                                   // yes
    static void  vClick(char* psDst);                                                                                   // yes
    static bool  bClip(char* psDst, i8 iMaxLen);                                                                        // yes
    static void  vComma(char* psDst, i8 iDstBufSize, i8 iArg, bool* pbResult = nullptr);                                // yes
    static i8    iCompare(const char* psArg1, const char* psArg2);                                                      // yes
    static i8    iCompare(const char* psArg1, const char* psArg2, i8 iCase);                                            // yes
    static i8    iCompare(const char* psArg1, const char* psArg2, i8 nBytes, i8 iCase);                                 // yes
    static void  vConcat(char* psDst, i8 iDstBufSize, const char cChar,  bool* pbResult = nullptr);                     // yes
    static void  vConcat(char* psDst, i8 iDstBufSize, const char* psArg, bool* pbResult = nullptr);                     // yes
    static void  vConcat(char* psDst, i8 iDstBufSize, const char* psArg, i8 nCatLen, bool* pbResult=nullptr);           // yes
    static void  vConcat(char* psDst, i8 iDstBufSize, const char* psArg1, const char* psArg2, bool* pbResult=nullptr);  // yes
    static void  vConcat(char* psDst, i8 iDstBufSize, i8 i, bool* pbResult = nullptr);                                  // yes
    static bool  bContains(const char* psSrc, const char cArg);                                                         // yes
    static bool  bContains(const char* psSrc, const char* sArg);                                                        // yes
    static i8    nCopy(char* psDst, i8 iDstBufSize, const char* psSrc, bool* pbResult = nullptr);                       // yes
    static void  vCopy(char* psDst, i8 iDstBufSize, const char* psSrc, bool* pbResult = nullptr);                       // yes
    static void  vCopyLen(char* psDst, i8 iDstBufSize, const char* psSrc, i8 nBytes, bool* pbResult = nullptr);         // yes
    static void  vCopySplit(char* psDst1, i8 iDst1BufSize,
                            char* psDst2, i8 iDst2BufSize,
                            const char* psSrc, char cSplit, bool* bpResult = nullptr);                                  // yes
    static void  vCopyTill(char* psDst, i8 iDstBufSize, const char* psSrc, char ccTill, bool* pbResult = nullptr);      // yes
    
    static i8    iDigitToI8(char cDigit);                                                                               // yes
    static void  vDump(const char* psSrc, i8 nMaxChars, const char* psSuffix, bool* pbResult = nullptr);                // yes

    static bool  bEqualTo(const char* psArg1, const char* psArg2);                                                      // yes
    static bool  bEqualTo(const char* psArg1, i8 nLen, const char* psArg2);                                             // yes
    static bool  bEqualTo(const char* psArg1, const char* psArg2, i8 iCase);                                            // yes
    static bool  bEnds(const char* psArg, const char cEnd);                                                             // yes
    static bool  bEnds(const char* psArg, const char* psEnd);                                                           // yes
    static void  vEscape(char* psDst, i8 iDstBufSize, const char* psSrc, bool* bpResult = nullptr);                     // yes
    static bool  bEscapeConvert(const char cArg,
                                char cEnclosedBy_Leading, char cEnclosedBy_Trailing,
                                char cEscapeChar, char cDelimiter, char cTerminator);                                   // yes
    static bool  bEscapeConvertDefault(const char cArg,
                                char cEscapeChar, char cDelimiter);                                                     // yes
    static char  cEscapeConvert(const char cArg);                                                                       // yes
    static void  vEscapeDblQuote(char* psDst, i8 iDstBufSize, const char* psSrc, bool* pbResult = nullptr);             // yes
    static char  cEscapeFor(const char cArg);                                                                           // yes
    static char  cEscapeInverse(const char cArg);                                                                       // yes
    static void  vEscapeInsert(char* psDst, i8 iDstBufSize, char cChar, char cWith, bool* pbResult = nullptr);          // yes
    static char  cEscapeProcessing(const char** aaSrc, char cEscapeChar);                                               // yes
    static bool  bEscapedSpecial(const char cArg);                                                                      // yes
    static bool  bExtractBoolean(char** ppsSrcCrs, i8& iBoolean);                                                       // yes
    static bool  bExtractDate(char** ppsSrcCrs, i8& iYear, i8& iMonth, i8& iDay);                                       // yes
    static bool  bExtractDate2(char** ppsSrcCrs, i8& iYear, i8& iMonth, i8& iDay,
                               eDC* pDateComponents, char cDateSeparator);                                              // yes
    static bool  bExtractDay(char** ppsSrcCrs, i8& iDay);                                                               // yes
    static bool  bExtractDecimal(char** ppsSrcCrs, i8& iDecimal, i8 iLength, i8 iScale);                                // yes
    static bool  bExtractDecLit(char*& ppsSrcCrs, i8& iValue, i8& nFractionDigits);                                     // yes
    static bool  bExtractFloat(char** ppsSrcCrs, d8& dFloat);                                                           // yes
    static bool  bExtractFraction(char** ppsSrcCrs, u8* u, i8 iDigits);                                                 // yes
    static bool  bExtractInteger(char** ppsSrcCrs, i8* i, bool bWithSign=false);                                        // yes
    static bool  bExtractInteger(char** ppsSrcCrs, i8* i, i8& nDigits);                                                 // yes
    static bool  bExtractInterval(char** ppsSrcCrs, i8& iSecond, i8& iMilli);                                           // yes
    static bool  bExtractItem(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult = nullptr);                 // yes
    static bool  bExtractItem(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, char cDelimiter, bool* pbResult = nullptr);// yes
    static bool  bExtractMonth(char** ppsSrcCrs, i8& iMonth);                                                           // yes
    static bool  bExtractName(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult = nullptr);                 // yes
    static bool  bExtractPath(char** ppsSrcCrs, char* pPath);                                                           // ✕
    static bool  bExtractPort(char** ppsSrcCrs, u2* pPort);                                                             // ✕
    static bool  bExtractString(char** ppsSrcCrs, char* pString);                                                       // ✕
    static bool  bExtractTime(char** ppsSrcCrs, i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro);                       // ✕
    static bool  bExtractTimestamp(char** ppsSrcCrs, i8& iYear, i8& iMonth, i8& iDay,
                                   i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro);                                    // ✕
    static bool  bExtractTimestamp2(char** ppsSrcCrs, i8& iYear, i8& iMonth, i8& iDay,
                                    i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro,
                                    eDC* pDateComponents, char cDateSeparator);                                         // ✕
    static bool  bExtractU8(char** ppsSrcCrs, u8* u);                                                                   // ✕
    static bool  bExtractU8(char** ppsSrcCrs, u8* u, i8 iDigits);                                                       // ✕
    static bool  bExtractVarchar(char** ppsSrcCrs,
                                 char* aVarchar,
                                 i8 iMaxLength,
                                 bool bIsEnclosed,
                                 char cEnclosedBy_Leading,
                                 char cEnclosedBy_Trailing,
                                 char cEscapeChar,
                                 char cFieldSeparator,
                                 char cRecordTerminator,
                                 bool bAllowNonPrintable,
                                 i8& nChars);                                                                           // ✕
    static bool  bExtractVarcharDefault(char** ppsSrcCrs,
                                        char* aVarchar,
                                        i8 iMaxLength,
                                        i8& nChars);                                                                    // ✕
    static bool  bExtractWord(char** ppsSrcCrs, char* pWord);                                                           // ✕
    static bool  bExtractWordString(char** ppsSrcCrs, char* psDst, i8 iDstBufSize, bool* pbResult = nullptr);           // yes
    static bool  bExtractYear(char** ppsSrcCrs, i8& iYear);                                                             // ✕

    static i8    iFind(char* ps, const char cArg);                                                                      // ✕
    static bool  bFind(char** ps, const char cArg);                                                                     // ✕
    static bool  bFind(char** ps, const char cArg, i8 iOccurence);                                                      // ✕
    static bool  bFind(char** ps, const char* sArg);                                                                    // ✕
    static bool  bFind(char** ps, const char* sArg, i8 iLenArg);                                                        // ✕
    static void  vFormatFraction(char* sDst, i8 nBufSize, u8 uFraction, i8 iDigits);                                    // ✕
    static void  vFree(char** ps);                                                                                      // ✕
    
    static i8    iGleanInt(char* azArg);                                                                                // ✕
    static char* azGroupEnd(char* azArg, char cItemSep, i8 nAvailBytes,
                            i8& rnBytesInGroup, i8& rnLinesInGroup);                                                    // ✕

    static i8    iHexToInw(const char c);                                                                               // ✕

    static void  vInsertChar(char* psDst, i8 iDstBufSize, char cArg, bool* pbResult = nullptr);                         // yes
    static bool  bIsAlpha(const char c);                                                                                // yes
    static bool  bIsAlphaDigit(const char c);                                                                           // yes
    static bool  bIsAmpersand(const char c);                                                                            // yes
    static bool  bIsAsciiName(const char* psArg);                                                                       // yes
    static bool  bIsAsterisk(const char c);                                                                             // yes
    static bool  bIsAt(const char c);                                                                                   // yes
    static bool  bIsBackQuote(const char c);                                                                            // yes
    static bool  bIsBackSlash(const char c);                                                                            // yes
    static bool  bIsBlank(const char c);                                                                                // yes
    static bool  bIsBoolean(const char* psArg);                                                                         // yes
    static bool  bIsCloseBrace(const char c);                                                                           // yes
    static bool  bIsCloseBracket(const char c);                                                                         // yes
    static bool  bIsCloseParenthesis(const char c);                                                                     // yes
    static bool  bIsCircumflex(const char c);                                                                           // yes
    static bool  bIsCommentBlock(const char* psArg);                                                                    // yes
    static bool  bIsCommentLine(const char* ps);                                                                        // yes
    static bool  bIsColon(const char c);                                                                                // yes
    static bool  bIsDecimal(const char* psArg);                                                                         // yes
    static bool  bIsDigit(const char c);                                                                                // yes
    static bool  bIsDollar(const char c);                                                                               // yes
    static bool  bIsDot(const char c);                                                                                  // yes
    static bool  bIsDoubleQuote(const char c);                                                                          // yes
    static bool  bIsEmpty(const char* azArg);                                                                           // yes
    static bool  bIsEqual(const char c);                                                                                // yes
    static bool  bIsEscapable(const char cArg);                                                                         // yes
    static bool  bIsExclamation(const char c);                                                                          // yes
    static bool  bIsFloat(const char* psArg);                                                                           // yes
    static bool  bIsGreaterThan(const char c);                                                                          // yes
    static bool  bIsHash(const char c);                                                                                 // yes
    static bool  bIsHex(const char c);                                                                                  // yes
    static bool  bIsInteger(const char* psArg);                                                                         // yes
    static bool  bIsIPAddr(const char* psArg);                                                                          // yes
    static bool  bIsLessThan(const char c);                                                                             // yes
    static bool  bIsLowerCase(const char c);                                                                            // yes
    static bool  bIsMinus(const char c);                                                                                // yes
    static bool  bIsNameBodyChar(const char c);                                                                         // yes
    static bool  bIsNameStartChar(const char c);                                                                        // yes
    static bool  bIsNewLine(const char c);                                                                              // yes
    static bool  bIsNull(const char c);                                                                                 // yes
    static bool  bIsOpenBrace(const char c);                                                                            // yes
    static bool  bIsOpenBracket(const char c);                                                                          // yes
    static bool  bIsOpenParenthesis(const char c);                                                                      // yes
    static bool  bIsPercent(const char c);                                                                              // yes
    static bool  bIsPipe(const char c);                                                                                 // yes
    static bool  bIsQuestion(const char c);                                                                             // yes
    static bool  bIsPlus(const char c);                                                                                 // yes
    static bool  bIsSemicolon(const char c);                                                                            // yes
    static bool  bIsSign(const char c);                                                                                 // yes
    static bool  bIsSingleQuote(const char c);                                                                          // yes
    static bool  bIsSlash(const char c);                                                                                // yes
    static bool  bIsSqlName(char* ps);                                                                                  // yes
    static bool  bIsTilde(const char c);                                                                                // yes
    static bool  bIsTypical(const char c);                                                                              // yes
    static bool  bIsTypical(const char* psArg);                                                                         // yes
    static bool  bIsUnderscore(const char c);                                                                           // yes
    static bool  bIsUpperCase(const char c);                                                                            // yes
    static bool  bIsWhite(const char c);                                                                                // yes
    static bool  bIsWhiteSpaces(const char* psArg);                                                                     // yes
    static bool  bIsX(const char c);                                                                                    // yes

    static char* acLast(const char* s);                                                                                 // ✕
    //static i8    nLength(const char* s);
    static i8    nLength(const char* s, const char cTerm, i8 iMax);                                                     // ✕
    static i8    nLengthNonBlank(const char* s, const char cTerm, i8 iMax);                                             // ✕

    static i8    iLike2(char* azStr, char* azRegExp);                                                                   // ✕
    static i8    iLike3(char* azStr, char* azRegExp, char cEscape);                                                     // ✕

    static bool  bMatch(char* azStr, char* azRegExp);                                                                   // ✕
    static bool  bMatchHere(char* azStr, char* azRegExp);                                                               // ✕
    static bool  bMatchStar(char* azStr, char* azRegExp, char cChr);                                                    // ✕

    static char* azNullTerminator(char* s);                                                                             // ✕

    static i8    iOccurrence(char* ps, const char cArg);                                                                // ✕
    static bool  bOneOf(char** asaSet, char* sWord);                                                                    // ✕
  
    static void  vPad(char* azDst, i8 iToLength, const char cWith);                                                     // ✕
    static void  vPut(char* azDst, i8 iAt, char cChr);                                                                  // ✕
    static void  vPut(char* azDst, i8 iAt, char* azSrc);                                                                // ✕
  
    static void  vRemove(char* azArg, const char cLeading);                                                             // ✕
    static void  vRemoveChar(char* azDst, char* azSrc, const char cChr);                                                // ✕
    static void  vRemoveHead(char* azArg, const char cSep);                                                             // ✕
    static void  vRemovePrefix(char* azArg, char cIntro);                                                               // ✕
    static void  vRemoveSuffix(char* azArg, char cIntro);                                                               // ✕
    static void  vRemoveTail(char* azArg, const char* pTail);                                                           // ✕
    static void  vRemoveTailBy(char* azArg, i8 nChars);                                                                 // ✕
    static void  vRemoveTailInt(char* azArg);                                                                           // ✕
    static void  vRemoveTill(char* azArg, const char cTill, bool bRemoveTheTill=false);                                 // ✕
    static void  vRemoveToken(char* azArg);                                                                             // ✕
    static bool  bReplace(char* pString, const char* pThis, const char* pThat);                                         // ✕
    static bool  bReplace(char* pDst, const char* pSrc,
                          const char* pThis, const char* pThat);                                                        // ✕
    static void  vReplaceTail(char* azDst, const char* azTail);                                                               // ✕
    static void  vReverse(char* psDst, i8 iDstBufSize, char* psSrc, bool* pbResult = nullptr);                          // yes
    static void  vRotate(char* zArg, i8 nby);                                                                           // ✕
    static void  vRuler(char* azDst, i8 iLength);                                                                       // ✕

    static bool  bSame(const char c1, const char c2);                                                                   // ✕
    static bool  bSame(const char* s1, const char* s2);                                                                 // ✕
    static void  vShortHand(char* azDst, const char* azSrc);                                                            // ✕
    static void  vShuffleLeft(char* azDst, const char* azSrc);                                                          // ✕
    static void  vShuffleLeft(char* azDst, i8 i_Right);                                                                 // ✕
    static bool  bSimilar(const char* azArg1, const char* azArg2, const char* sSep);                                    // ✕
    static i8    nSize(const char* s);                                                                                  // ✕
    static void  vSkipCommentBlock(char** ps);                                                                          // ✕
    static void  vSkipCommentLine(char** ps);                                                                           // ✕
    static void  vSkipCommentsEtc(char** aazCrs);                                                                       // ✕
    static void  vSkipDigits(char** ps);                                                                                // ✕
    static bool  bSkipEOL(char** ps);                                                                                   // ✕
    static bool  bSkipLeading(char** ps, char cEnclosedBy_Leading);                                                     // ✕
    static bool  bSkipNonAlnum(char** ps);                                                                              // ✕
    static bool  bSkipNonDigit(char** ps);                                                                              // ✕
    static void  vSkipSpaces(char** ps);                                                                                // ✕
    static bool  bSkipTrailing(char** ps,
                               bool bSawEnclosedBy_Leading,
                               char cEnclosedBy_Trailing);                                                              // ✕
    static void  vSkipWhite(char** ps);                                                                                 // ✕
    static void  vSplitSeries(char* azStr, char* azInt, char* sSrc);                                                    // ✕
    static void  vSprint(char* psDst, i8 iDstBufSize, bool* pbResult, Stz* sFormat, ...);                               // yes
    static void  vSprint(Stz* psDst, i8 iDstBufSize, bool* pbResult, Stz* sFormat, ...);                                // yes
    static void  vSprint(char* psDst, i8 iDstBufSize, va_list args, const char* sFormat, bool* pbResult =  nullptr);    // yes
    static void  vSprintCat(Stz*  psDst, i8 iDstBufSize, bool* pbResult, Stz* sFormat, ...);                            // yes
    static void  vSprintCat(char* psDst, i8 iDstBufSize, va_list args, const char* sFormat, bool* pbResult = nullptr);  // yes
    static void  vStrip(char* s, char cTrailing);                                                                       // ✕
    static void  vSubPath(char* azDst, i8 iDstBufSize, char* azSrc, bool* pbResult = nullptr);                          // yes

    static bool  bTailIs(const char* z, char* ss);                                                                      // ✕
    static char* aczTabLoc(char* azArg, i8 i_Tab, char cPad);                                                           // ✕
    static i8    iToBcd(const char* s);                                                                                 // ✕
    static bool  bToBool(const char* s);                                                                                // ✕
    static d8    dToFlt(const char* s);                                                                                 // ✕
    static char  cToHex(i8 i);                                                                                          // ✕
    static i8    iToI8(const char* s);                                                                                  // ✕
    static i8    iToI8(const char* s, bool* bInvalidValue, bool* bOverflowed);                                          // ✕
    static char  cToLower(char c);                                                                                      // yes
    static void  vToLower(char* psDst, i8 iDstBufSize, bool* pbResult = nullptr);                                       // yes
    static char  cToUpper(char c);                                                                                      // yes
    static void  vToUpper(char* psDst, i8 iDstBufSize, bool* pbResult = nullptr);                                       // yes
    static void  vTopTail(char* azArg, char cIntro);                                                                    // ✕
    static void  vToPointer(char* psDst, i8 iDstBufSize, const void* pvArg, bool* pbResult = nullptr);                  // yes
    static void  vToString(char* psDst, i8 iDstBufSize, bool bArg, bool* pbResult = nullptr);                           // yes
    static void  vToString(char* azDst, i8 iDstBufSize, i8 iArg, bool* pbResult = nullptr);                             // yes
    static void  vToString(char* psDst, i8 iDstBufSize, d8 dArg, bool* pbResult = nullptr);                             // yes
    static void  vTrim(char* azArg, char cTrim);                                                                        // ✕

    static bool  bValidateMonth(i8 iDay, i8 iMonth, i8 iYear);                                                          // ✕

    static i8    iWordFlip(char* aDst, char* aSrc);                                                                     // ✕
    static i8    iWordFlipBack(char* aDst, char* aSrc);                                                                 // ✕
  
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

  i8 StrLib_iLike2(char* azStr, char* azRegExp);
  i8 StrLib_iLike3(char* azStr, char* azRegExp, char cEsc);
  i8 StrLib_iWordFlipBack(char* aDst, char* aSrc);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
