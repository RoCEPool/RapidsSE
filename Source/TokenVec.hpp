/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/
#pragma once

//#define __WITH_TOKEN_ARY

namespace LexicalAnalysis
{
  #ifdef __WITH_TOKEN_ARY
  class TokenVec : public ArrayNL_t<CN_TOKENVEC,Token,C_MAX_TOKENS>
  #else
  class TokenVec : public VectorNL_t<CN_TOKENVEC,Token,C_MAX_TOKENS>
  #endif
  {
  private:
   
    s256 m_sSource;  // where did the tokens come from
  
    i8 m_iCurTkn;
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenVec(bool bNewTokens=true);
   ~TokenVec(void) noexcept(false);
  
    static TokenVec* _pTokenSet(void* aAddr);
  
    void vAdvanceCurr(void);
    void vAppendToken(Stz* szTerm);
    void vAppendVector(TokenVec* pTokenVec);
    i8   iCountTokens(void);
    void vCopyVector(TokenVec* pTokenVec);
    i8   iCurrToken(void);
    Token* pCurrToken(void);
    void vCutToken(i8 i_Tkn);
    void vDumpTokens(void);
    i8   iFindToken(Stz* pLookFor);
    Token* pFindToken(Stz* szTerm, i8& i_Token);
    Token* pFirstToken(void);
    void vIncTokens(void);
    void vInsertToken(i8 i_Tkn, Stz* szTerm, eTN eToken);
    Token* pNextToken(void);
    Token* pPrevToken(void);
    void vPrint(void);
    void vReset(void);
    void vSetAllTokens(void);
    void vSetTokens(void);
    Stz* szSource(void);
    void vSource(Stz* szSource);
    void vStripAmpersand(void);
    void vStripSingleQuotes(void);
    void vStripSemicolon(void);
    i8   nTokens(void);
    void vVecToString(Stz* szDstStr, i8 iFromTkn, bool bAddSemicolon);

    Footer* aFooter(void);
  };

  TokenVec* _pTokenVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
