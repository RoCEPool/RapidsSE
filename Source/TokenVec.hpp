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
  
    static void vReplace(TokenVec* pTknVec, TokenVec* pFindVec, TokenVec* pWithVec);
    static TokenVec* pSubVec(TokenVec* pTknVec, i8 i_From, i8 i_To);
    static TokenVec* pSubVec(TokenVec* pTknVec, Stz* szStart, Stz* szEnd);
    static TokenVec* pTokenise(Stz* szString);
    static TokenVec* _pTokenSet(void* aAddr);

    void vAdvanceCurr(void);
    void vAppendToken(Stz* szArgChars);
    void vAppendVector(TokenVec* pTokenVec);
    bool bBlankLine(void);
    void vCollectBegin(Token* pToken);
    void vCollectEnd(Token* pToken);
    i8   iCountTokens(void);
    void vCopyVector(TokenVec* pTokenVec);
    i8   iCurrToken(void);
    Token* pCurrToken(void);
    i8   iFindToken(Stz* pLookFor);
    Token* pFindToken(Stz* szArgChars, i8& i_Token);
    Token* pFirstToken(void);
    void vIncTokens(void);
    void vInsertToken(i8 i_Tkn, Stz* szArgChars, eTN eToken);
    Stz* szLaterChars(void);
    void vListTokens(void);
    Token* pNextToken(void);
    Token* pPrevToken(void);
    void vRemoveToken(i8 i_Tkn);
    void vReplaceToken(Token* pDstToken, Token* pSrcToken);
    void vSetAllTokens(void);
    void vSetTokens(void);
    void vSoftReset(void);
    void vStripAmpersand(void);
    void vStripSingleQuotes(void);
    void vStripSemicolon(void);
    void vTakeCharsBytes(i8 nBytes);
    void vTokenise(Stz* szString);
    i8   nTokens(void);
    void vUpdateToken(Token* pDstToken, Stz* szArgChars);
    void vVecToString(Stz* szDstStr, i8 iFromTkn, bool bAddSemicolon);
  
  private:
    Stz* _szByteSpaceExtend(i8 nBytes);
    void _vByteSpaceInsert(Stz* szAtChars, i8 nBytess);
    void _vByteSpaceRemove(Stz* szAtChars, i8 nBytes);
    void _vByteSpaceTrim(i8 nBytes);
  
    void _vCharsCopyInterior(Token* pDstToken, Stz* szArgChars);
    void _vCharsCopyLast(Token* pDstToken, Stz* szArgChars);

    void _vHardReset(void);
    
    bool _bIsLastToken(Token* pToken);

    void _vTokenCutInterior(Token* pCutToken);
    void _vTokenCutLast(Token* pCutToken);

    void _vVecAdjustChars(i8 i_Tkn, i8 nByBytes);
    void _vVecCheckChars(void);
    void _vVecRippleLeft(i8 i_Tkn);
    void _vVecRippleRight(i8 i_Tkn);

    Footer* aFooter(void);
  };

  TokenVec* _pTokenVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
