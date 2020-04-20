/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class TokenVec : public ArrayNL_t<CN_TOKENVEC,Token,C_MAX_TOKENS>
  {
  private:
     
    i8 m_iCurTkn;       // the current token in the vector
  
    b256K m_xyCharsSet; // buffer for all token chars 
  
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

    void vCopyCharsTV_restricted(Token* pDstToken, Stz* szArgChars);
    void vCopyTokenTV_restricted(Token* pDstToken, Token* pSrcToken);
  
    void vAdvanceCurr(void);
    void vAppendToken(Stz* szArgChars);
    void vAppendVector(TokenVec* pTokenVec);
    bool bBlankLine(void);
    i8   iCountTokens(void);
    void vCopyVector(TokenVec* pTokenVec);
    i8   iCurrToken(void);
    Token* pCurrToken(void);
    void vCutToken(i8 i_Tkn);
    void vDumpTokens(void);
    i8   iFindToken(Stz* pLookFor);
    Token* pFindToken(Stz* szArgChars, i8& i_Token);
    Token* pFirstToken(void);
    void vIncTokens(void);
    void vInsertToken_restricted(i8 i_Tkn, Stz* szArgChars, eTN eToken);
    Stz* szLaterChars(void);
    Token* pNextToken(void);
    Token* pPrevToken(void);
    void vPrint(void);
    void vReset(void);
    void vSetAllTokens(void);
    void vSetTokens(void);
    void vStripAmpersand(void);
    void vStripSingleQuotes(void);
    void vStripSemicolon(void);
    void vTakeCharsBytes(i8 nBytes);
    void vTokenise(Stz* szString);
    i8   nTokens(void);
    void vVecToString(Stz* szDstStr, i8 iFromTkn, bool bAddSemicolon);
  
  private:
    void _vByteSpaceInsert(i8 i_Tkn, i8 nBytess);
    void _vByteSpaceRemove(i8 i_Tkn, i8 nBytes);
    void _vByteSpaceTrim(i8 nBytes);
  
    void _vMoveCharsLeft(Stz* szAtChars, i8 nBytes);
    void _vMoveCharsRight(Stz* szAtChars, i8 nBytes);

    Footer* aFooter(void);
  };

  TokenVec* _pTokenVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
