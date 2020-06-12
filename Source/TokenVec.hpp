/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class TokenVec : public ArrayNL_t<CN_TOKENVEC,Token,C_MAX_TOKENS>
  {
  private:
     
    i8 m_iCurTkn;        // the current token in the vector
  
    b256K m_xyByteSpace; // buffer for all token chars
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenVec(bool bNewTokens=true);
   ~TokenVec(void) noexcept(false);
  
    static TokenVec* pGetSubVec(TokenVec* pTknVec, i8 i_From, i8 i_To);
    static TokenVec* pGetSubVec(TokenVec* pTknVec, Stz* szStart, Stz* szEnd);
    static void      vReplace(TokenVec* pTknVec,
                              TokenVec* pFindVec, TokenVec* pWithVec);
    static TokenVec* pTokenise(Stz* szString);

    void vAdvanceCurr(bool bIncItems);
    void vAppendToken(Stz* szArgChars, eTN eToken);
    void vAppendToken(Token* pToken);
    void vAppendVector(TokenVec* pTokenVec);
    bool bBlankLine(void);
    void vCollectBegin(Token* pToken);
    void vCollectEnd(Token* pToken);
    i8   iCountTokens(void);
    void vCopyVector(TokenVec* pTokenVec);
    i8   iCurrToken(void);
    void vCurrToken(i8 i_Tkn);
    Token* pCurrToken(void);
    void vDbgStr(Stz* szDstStr);
    void vDumpVec(i8 nPlusMore);
    i8   iFindToken(Stz* pLookFor);
    i8   iFindToken(Stz* pLookFor, eTN eToken);
    Token* pFindToken(Stz* szArgChars, i8& i_Token);
    Token* pFirstToken(void);
    bool bHasSemicolon(void);
    void vIncTokens(void);
    void vInsertToken(i8 i_Tkn, Stz* szArgChars, eTN eToken);
    void vIntegrityCheck(void);
    void vListTokens(void);
    Token* pNextToken(void);
    void vNextToken(void);
    Token* pPrevToken(void);
    void vPrevToken(void);
    void vRemoveSemicolon(void);
    void vRemoveToken(i8 i_Tkn);
    void vRewindCurr(void);
    void vReplaceToken(Token* pDstToken, Token* pSrcToken);
    void vSetAllTokens(void);
    void vSoftReset(void);
    void vStripAmpersand(void);
    void vStripSingleQuotes(void);
    void vStripSemicolon(void);
    void vTrimTokens(i8 i_Tkn);
    void vTokenise(Stz* szString);
    i8   nTokens(void);
    void vUpdateToken(Token* pDstToken, Stz* szArgChars);
    void vToTknString(Stz* szDstStr);
    void vVecToString(Stz* szDstStr, i8 iFromTkn, bool bAddSemicolon);
  
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
  
    void _vTokenRemoveInterior(Token* pCutToken);
    void _vTokenRemoveLast(Token* pCutToken);

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
