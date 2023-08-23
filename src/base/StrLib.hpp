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
    static char* wAllocate(i8 nBytes);                                                                                  // yes
    static char* wAllocate(const char* s);                                                                              // yes
    static void  vAppendChar(char** aaz, char c);                                                                       // ✕
    static void  vAppendDecimal(char** aaz, i8 i);                                                                      // ✕
    static void  vAppendHex(char** aaz, i8 i);                                                                          // ✕
    static void  vAppendNull(char** aaz);                                                                               // ✕
    static void  vAppendPad(char** aaz, const char* s, i8 nLength);                                                     // ✕
    static void  vAppendString(char** aaz, const char* s);                                                              // ✕
    static void  vAppendString(char** aaz, i8 nBufSize, const char* s, i8* piLineNo);                                   // ✕
    static char* azAscii(i8 i_Char);                                                                                    // ✕
    
    static bool  bBackward(char** ps, char* pBegin, const char c);                                                      // ✕
    static bool  bBeginsWith(const char* s, const char* sArg);                                                          // ✕
    static i8    nBurstArgs(Stz* szaArgv[], Stz* szArgStr);                                                             // ✕

    static Stz*  szCharToStr(unsigned char cArg);                                                                       // ✕
    static void  vCheckIsNot(char cArg, char cIsNot);                                                                   // yes
    static void  vClick(char* aArg);                                                                                    // ✕
    static bool  bClip(char* aArg, i8 iLen);                                                                            // ✕
    static void  vComma(char* psDst, i8 iArg);                                                                          // ✕
    static i8    iCompare(const char* pArg1, const char* pArg2);                                                        // yes
    static i8    iCompare(const char* pArg1, const char* pArg2, i8 iCase);                                              // yes
    static i8    iCompare(const char* pArg1, const char* pArg2, i8 nBytes, i8 iCase);                                   // yes
    static void  vConcat(char* pWS, i8 iDstBufSize, const char cChar, bool* pbResult = nullptr);                        // yes
    static void  vConcat(char* psDst, i8 iDstBufSize, const char* psArg, bool* pbResult = nullptr);                     // yes
    static void  vConcat(char* psDst, const char* psArg, i8 nLen);                                                      // ✕
    static void  vConcat(char* psDst, const char* psArg1, const char* psArg2);                                          // ✕
    static char* wConcat(const char* s1, const char* s2);                                                               // ✕
    static void  vConcat(char* pWS, i8 iDstBufSize, i8 i, bool* pbResult = nullptr);                                    // yes
    static void  vConcat(Stz* szDst, Stz* szArg);                                                                       // ✕
    static bool  bContains(char* s, const char cArg);                                                                   // ✕
    static bool  bContains(char* s, const char* sArg);                                                                  // ✕
    static i8    nCopy(char* psDst, const char* psSrc);                                                                 // ✕
    static i8    nCopy_nonull(char* psDst, const char* psSrc);                                                          // ✕
    static void  vCopy(char* psDst, const char* psSrc);                                                                 // ✕
    static void  vCopyLen(char* psDst, const char* psSrc, i8 nBytes);                                                   // ✕
    static void  vCopySplit(char* w1, char* w2, const char* sSrc, char cSplit);                                         // ✕
    static void  vCopyTill(char* psDst, const char* psSrc, char ccTill);                                                // ✕
    
    static i8    iDigitToI8(char cDigit);                                                                               // ✕
    static void  vDump(const char* s, i8 nMax, const char* pSuffix);                                                    // ✕
    static char* sDupe(const char* s);                                                                                  // ✕
    static char* sDupeStripped(const char* s);                                                                          // ✕
  
    static bool  bEachLine(char** ps, char* caLine);                                                                    // ✕
    static bool  bEqualTo(const char* s1, const char* s2);                                                              // ✕
    static bool  bEqualTo(const char* s1, i8 nLen2, const char* s2);                                                    // ✕
    static bool  bEqualTo(const char* s1, const char* s2, i8 iCase);                                                    // ✕
    static bool  bEnds(const char* sArg, const char cEnd);                                                              // ✕
    static bool  bEnds(char* sArg, char* sEnd);                                                                         // ✕
    static void  vEscape(char* azDst, char* azSrc);                                                                     // ✕
    static bool  bEscapeConvert(const char cArg,
                                char cEnclosedBy_Leading, char cEnclosedBy_Trailing,
                                char cEscapeChar, char cDelimiter, char cTerminator);                                   // ✕
    static bool  bEscapeConvertDefault(const char cArg,
                                char cEscapeChar, char cDelimiter);                                                     // ✕
    static char  cEscapeConvert(const char cArg);                                                                       // ✕
    static void  vEscapeDblQuote(const char* sArg, char* sEscaped);                                                     // ✕
    static char  cEscapeFor(const char cArg);                                                                           // ✕
    static char  cEscapeInverse(const char cArg);                                                                       // ✕
    static void  vEscapeInsert(char* azStr, char cChar, char cWith);                                                    // ✕
    static char  cEscapeProcessing(char** aaSrc, char cEscapeChar);                                                     // ✕
    static bool  bEscapedSpecial(const char cArg);                                                                      // ✕
    static bool  bExtractArg(char** ps, char* pPath);                                                                   // ✕
    static bool  bExtractBoolean(char** ps, i8& iBoolean);                                                              // ✕
    static bool  bExtractDate(char** ps, i8& iYear, i8& iMonth, i8& iDay);                                              // ✕
    static bool  bExtractDate2(char** ps, i8& iYear, i8& iMonth, i8& iDay,
                               eDC* pDateComponents, char cDateSeparator);                                              // ✕
    static bool  bExtractDay(char** ps, i8& iDay);                                                                      // ✕
    static bool  bExtractDecimal(char** ps, i8& iDecimal, i8 iLength, i8 iScale);                                       // ✕
    static bool  bExtractDecLit(char*& az, i8& iValue, i8& nFractionDigits);                                            // ✕
    static bool  bExtractFloat(char** ps, d8& dFloat);                                                                  // ✕
    static bool  bExtractFraction(char** ps, u8* u, i8 iDigits);                                                        // ✕
    static bool  bExtractInteger(char** ps, i8* i, bool bWithSign=false);                                               // ✕
    static bool  bExtractInteger(char** ps, i8* i, i8& nDigits);                                                        // ✕
    static bool  bExtractInterval(char** ps, i8& iSecond, i8& iMilli);                                                  // ✕
    static bool  bExtractItem(char** ps, char* pItem);                                                                  // ✕
    static bool  bExtractItem(char** ps, char* pItem, char cDelimiter);                                                 // ✕
    static bool  bExtractMonth(char** ps, i8& iMonth);                                                                  // ✕
    static bool  bExtractName(char** aazCursor, char* azName);                                                          // ✕
    static bool  bExtractPath(char** ps, char* pPath);                                                                  // ✕
    static bool  bExtractPort(char** ps, u2* pPort);                                                                    // ✕
    static bool  bExtractString(char** ps, char* pString);                                                              // ✕
    static bool  bExtractTime(char** ps, i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro);                              // ✕
    static bool  bExtractTimestamp(char** ps, i8& iYear, i8& iMonth, i8& iDay,
                                   i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro);                                    // ✕
    static bool  bExtractTimestamp2(char** ps, i8& iYear, i8& iMonth, i8& iDay,
                                   i8& iHour, i8& iMinute, i8& iSecond, i8& iMicro,
                                    eDC* pDateComponents, char cDateSeparator);                                         // ✕
    static bool  bExtractU8(char** ps, u8* u);                                                                          // ✕
    static bool  bExtractU8(char** ps, u8* u, i8 iDigits);                                                              // ✕
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
                                 i8& nChars);                                                                           // ✕
    static bool  bExtractVarcharDefault(char** ps,
                                        char* aVarchar,
                                        i8 iMaxLength,
                                        i8& nChars);                                                                    // ✕
    static bool  bExtractWord(char** ps, char* pWord);                                                                  // ✕
    static bool  bExtractWordString(char** ps, char* pWord);                                                            // ✕
    static bool  bExtractYear(char** ps, i8& iYear);                                                                    // ✕

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

    static void  vInsertChar(char* azAt, char cArg);                                                                    // ✕
    static bool  bIsAlpha(const char c);                                                                                // ✕
    static bool  bIsAlphaDigit(const char c);                                                                           // ✕
    static bool  bIsAmpersand(const char c);                                                                            // ✕
    static bool  bIsAsciiName(char* azArg);                                                                             // ✕
    static bool  bIsAsterisk(const char c);                                                                             // ✕
    static bool  bIsAt(const char c);                                                                                   // ✕
    static bool  bIsBackQuote(const char c);                                                                            // ✕
    static bool  bIsBackSlash(const char c);                                                                            // ✕
    static bool  bIsBlank(const char c);                                                                                // ✕
    static bool  bIsBoolean(char* azArg);                                                                               // ✕
    static bool  bIsCloseBrace(const char c);                                                                           // ✕
    static bool  bIsCloseBracket(const char c);                                                                         // ✕
    static bool  bIsCloseParenthesis(const char c);                                                                     // ✕
    static bool  bIsCircumflex(const char c);                                                                           // ✕
    static bool  bIsCommentBlock(char* ps);                                                                             // ✕
    static bool  bIsCommentLine(char* ps);                                                                              // ✕
    static bool  bIsColon(const char c);                                                                                // ✕
    static bool  bIsDecimal(char* ps);                                                                                  // ✕
    static bool  bIsDigit(const char c);                                                                                // ✕
    static bool  bIsDollar(const char c);                                                                               // ✕
    static bool  bIsDot(const char c);                                                                                  // ✕
    static bool  bIsDoubleQuote(const char c);                                                                          // ✕
    static bool  bIsEmpty(char* azArg);                                                                                 // ✕
    static bool  bIsEqual(const char c);                                                                                // ✕
    static bool  bIsEscapable(const char cArg);                                                                         // ✕
    static bool  bIsExclamation(const char c);                                                                          // ✕
    static bool  bIsFloat(char* azArg);                                                                                 // ✕
    static bool  bIsGreaterThan(const char c);                                                                          // ✕
    static bool  bIsHash(const char c);                                                                                 // ✕
    static bool  bIsHex(const char c);                                                                                  // ✕
    static bool  bIsInteger(char* azArg);                                                                               // ✕
    static bool  bIsIPAddr(const char* azArg);                                                                          // ✕
    static bool  bIsLessThan(const char c);                                                                             // ✕
    static bool  bIsLowerCase(const char c);                                                                            // ✕
    static bool  bIsMinus(const char c);                                                                                // ✕
    static bool  bIsNameBodyChar(const char c);                                                                         // ✕
    static bool  bIsNameStartChar(const char c);                                                                        // ✕
    static bool  bIsNewLine(const char c);                                                                              // ✕
    static bool  bIsNull(const char c);                                                                                 // ✕
    static bool  bIsOpenBrace(const char c);                                                                            // ✕
    static bool  bIsOpenBracket(const char c);                                                                          // ✕
    static bool  bIsOpenParenthesis(const char c);                                                                      // ✕
    static bool  bIsPercent(const char c);                                                                              // ✕
    static bool  bIsPipe(const char c);                                                                                 // ✕
    static bool  bIsQuestion(const char c);                                                                             // ✕
    static bool  bIsPlus(const char c);                                                                                 // ✕
    static bool  bIsSemicolon(const char c);                                                                            // ✕
    static bool  bIsSign(const char c);                                                                                 // ✕
    static bool  bIsSingleQuote(const char c);                                                                          // ✕
    static bool  bIsSlash(const char c);                                                                                // ✕
    static bool  bIsSqlName(char* ps);                                                                                  // ✕
    static bool  bIsTilde(const char c);                                                                                // ✕
    static bool  bIsTypical(const char c);                                                                              // ✕
    static bool  bIsTypical(char* azArg);                                                                               // ✕
    static bool  bIsUnderscore(const char c);                                                                           // ✕
    static bool  bIsUpperCase(const char c);                                                                            // ✕
    static bool  bIsWhite(const char c);                                                                                // ✕
    static bool  bIsWhiteSpaces(char* ps);                                                                              // ✕
    static bool  bIsX(const char c);                                                                                    // ✕

    static char* acLast(const char* s);                                                                                 // ✕
    //static i8    nLength(const char* s);
    static i8    nLength(const char* s, const char cTerm, i8 iMax);                                                     // ✕
    static i8    nLengthNonBlank(const char* s, const char cTerm, i8 iMax);                                             // ✕

    static i8    iLike2(char* azStr, char* azRegExp);                                                                   // ✕
    static i8    iLike3(char* azStr, char* azRegExp, char cEscape);                                                     // ✕

    static bool  bMatch(char* azStr, char* azRegExp);                                                                   // ✕
    static bool  bMatchHere(char* azStr, char* azRegExp);                                                               // ✕
    static bool  bMatchStar(char* azStr, char* azRegExp, char cChr);                                                    // ✕

    static char* sNew(const char* azFormat, ...);                                                                       // ✕
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
    static void  vReplaceTail(char* azArg, char* azTail);                                                               // ✕
    static void  vReverse(char* azDst, char* azSrc);                                                                    // ✕
    static void  vRight(char* azDst, char* azSrc, i8 i_At);                                                             // ✕
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
    static void  vSprint(Stz* sDst, i8 nBufSize, Stz* sFormat, ...);                                                    // ✕
    static void  vSprint(char* sDst, i8 nBufSize, Stz* sFormat, ...);                                                   // ✕
    static void  vSprint(char* sDst, i8 nBufSize, va_list args, const char* sFormat);                                   // ✕
    static void  vSprintCat(Stz*  sDst, i8 nBufSize, Stz* sFormat, ...);                                                // ✕
    static void  vSprintCat(char* sDst, i8 nBufSize, va_list args, const char* sFormat);                                // ✕
    static void  vStrip(char* s, char cTrailing);                                                                       // ✕
    static void  vSubPath(char* azDst, char* azSrc);                                                                    // ✕

    static bool  bTailIs(const char* z, char* ss);                                                                      // ✕
    static char* aczTabLoc(char* azArg, i8 i_Tab, char cPad);                                                           // ✕
    static i8    iToBcd(const char* s);                                                                                 // ✕
    static bool  bToBool(const char* s);                                                                                // ✕
    static d8    dToFlt(const char* s);                                                                                 // ✕
    static char  cToHex(i8 i);                                                                                          // ✕
    static i8    iToI8(const char* s);                                                                                  // ✕
    static i8    iToI8(const char* s, bool* bInvalidValue, bool* bOverflowed);                                          // ✕
    static char  cToLower(char c);                                                                                      // ✕
    static void  vToLower(char* s);                                                                                     // ✕
    static char  cToUpper(char c);                                                                                      // ✕
    static void  vToUpper(char* s);                                                                                     // ✕
    static void  vTopTail(char* azArg, char cIntro);                                                                    // ✕
    static char* wToPointer(void* pArg, char* w);                                                                       // ✕
    static char* wToString(bool bArg, char* w);                                                                         // ✕
    static void  vToString(char* azDst, i8 iDstBufSize, i8 iArg, bool* pbResult = nullptr);                             // yes
    static char* wToString(d8 dArg, char* w);                                                                           // ✕
    static char* wToString(i8 iArg, char* psDst, i8 iDstBufSize, bool* pbResult = nullptr);                             // yes
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
