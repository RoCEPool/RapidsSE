/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/
#pragma once

#include "TokenArray.hpp"

namespace BASE
{
  class TokenVec : public Object
  {
  private:
    TokenArray* m_pTokenArray;
    TokenArrayIter m_xTokenIter;

    b256K m_xyByteSpace; // buffer for all token chars
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenVec(void);
   ~TokenVec(void) noexcept(false);
  
    static TokenVec* pGetSubVec(TokenVec* pTknVec, i8 i_From, i8 i_To);
    static TokenVec* pGetSubVec(TokenVec* pTknVec, Stz* szStart, Stz* szEnd);
    static bool      bReplace(TokenVec* pTknVec,
                              TokenVec* pFindVec, TokenVec* pWithVec);
    //static TokenVec* pTokeniseSen(Stz* szString);

    void vAdvanceCurr(void);
    void vAppendToken(Stz* szArgChars, eTN eToken);
    void vAppendToken(Token* pToken);
    void vAppendVector(TokenVec* pTokenVec);
    bool bBlankLine(void);
    Token* pCurrToken(void);
    void vDbgStr(String* psDstStr);
    void vDumpVec();
    i8   iFindToken(Stz* pLookFor);
    i8   iFindToken(Stz* pLookFor, eTN eToken);
    Token* pFindToken(Stz* szArgChars, i8& i_Token);
    i8   iFirstTag(void);
    i8   iFirstTag2(void);
    Token* pFirstToken(void);
    Token* pGet(i8 iIdx);
    bool bHasSemicolon(void);
    void vInsertToken(i8 i_Tkn, Stz* szArgChars, eTN eToken);
    void vIntegrityCheck(void);
    i8   nItems(void);
    i8   iLastToken(void);
    Token* pLastToken(void);
    void vListTokens(void);
    Token* pNextToken(void);
    Token* pPrevToken(void);
    void vRemoveSemicolon(void);
    void vRemoveToken(i8 i_Tkn);
    void vRewindCurr(void);
    void vSoftReset(void);
    void vStripSingleQuotes(void);
    void vStripSemicolon(void);
    void vTrimTokens(i8 i_Tkn);
    i8   nTokens(void);
    void vUpdateToken(Token* pDstToken, Stz* szArgChars);
    i8   nUsedByteSpace(void);
    void vToTknString(String* psDstStr);
    void vVecToString(String* psDstStr, i8 iFromTkn, bool bAddSemicolon);
  
  private:
    Stz* _szByteSpaceExtend(i8 nBytes);
    void _vByteSpaceInsert(Stz* szAtChars, i8 nBytesIns);
    void _vByteSpaceRemove(Stz* szAtChars, i8 nBytesRem);
    void _vByteSpaceTake(i8 nBytes);
    void _vByteSpaceTrim(i8 nBytes);
  
    void _vCharsCopyInterior(Token* pDstToken, Stz* szArgChars);
    void _vCharsCopyLast(Token* pDstToken, Stz* szArgChars);

    void _vHardReset(void);
    
    bool _bIsLastToken(Token* pToken);

    Stz* _szLaterChars(void);
  
    void _vTokenRemoveInterior(i8 iIdx);
    void _vTokenRemoveLast(i8 iIdx);

    void _vVecAdjustChars(i8 i_Tkn, i8 nByBytes);
    void _vVecRippleLeft(i8 i_Tkn, i8 iAdjCharsBytes);
    void _vVecRippleRight(i8 i_Tkn, i8 iAdjCharsBytes);

    Footer* aFooter(void);
  };

  TokenVec* _pTokenVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
