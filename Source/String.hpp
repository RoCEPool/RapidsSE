/*============================================================================*/
/* String.hpp -- char* string utility                                         */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  static const int C_NO_CASE    = 0;
  static const int C_CASE       = 1;
  
  class String : public Static
  {
  private:
    static const i8 C_STACK_WSP = 16 * C_1KB;
    
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
    
    static void  vDump(const char* s, i8 nMax, const char* pSuffix);
    static char* sDupe(const char* s);
    static char* sDupeStripped(const char* s);
    
    static bool  bEnds(const char* sArg, const char cEnd);
    static bool  bEnds(const char* sArg, const char* sEnd);
    static void  vEscape(const char* sArg, char* sEscaped);
    static bool  bExtractArg(char** ps, char* pPath);
    static bool  bExtractI8(char** ps, i8* i);
    static bool  bExtractItem(char** ps, char* pItem);
    static bool  bExtractPath(char** ps, char* pPath);
    static bool  bExtractPort(char** ps, u2* pPort);
    static bool  bExtractString(char** ps, char* pString);
    static bool  bExtractWord(char** ps, char* pWord);
    static bool  bExtractWordString(char** ps, char* pWord);
    
    static bool  bFind(char** ps, const char cArg);
    static bool  bFind(char** ps, const char* sArg);
    static bool  bFind(char** ps, const char* sArg, i8 iLenArg);
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
   
    static const char* sLast(const char* s);
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
    static bool  bSkipEOL(char** ps);
    static bool  bSkipNonAlnum(char** ps);
    static bool  bSkipNonDigit(char** ps);
    static void  vSkipWhite(char** ps);
    static void  vSprint(char* sDst, const char* sFormat, ...);
    static void  vSprint(char* sDst, va_list args, const char* sFormat);
    static void  vSprintCat(char* sDst, const char* sFormat, ...);
    static void  vSprintCat(char* sDst, va_list args, const char* sFormat);
    static void  vStrip(char* s, char c);

    static bool  bToBool(const char* s);
    static char  cToHex(i8 i);
    static i8    iToI8(const char* s);
    static void  vToLower(char* s);
    static char* wToPointer(void* pArg, char* w);
    static char* wToString(i8 iArg, char* w);
    
    static char* zTerminator(char* s);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
