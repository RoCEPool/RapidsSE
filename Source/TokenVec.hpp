/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/
#pragma once

namespace TokenStream
{
  class TokenVec : public VectorNL_t<CN_TOKENVEC,Token,C_MAX_TOKENS>
  {
  private:
  
    i8 m_iCurTkn;
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenVec(bool bNewTokens=true);
   ~TokenVec(void) noexcept(false);
  
    static TokenVec* _pTokenSet(void* aAddr);
  
    void vAddToken(Stz* aStr);
    void vAppendVector(TokenVec* pTokenVec);
    i8   iCountTokens(void);
    void vCopyVector(TokenVec* pTokenVec);
    i8   iCurrToken(void);
    Token* pCurrToken(void);
    void vCutToken(i8 i_Tkn);
    void vDumpTokens(void);
    i8   iFindToken(Stz* pLookFor);
    Token* pFindToken(Stz* aText, i8& i_Token);
    Token* pFirstToken(void);
    void vIncTokens(void);
    Token* pNextToken(void);
    Token* pPrevToken(void);
    void vPrint(void);
    void vReset(void);
    void vSetAllTokens(void);
    void vSetTokens(void);
    void vStripAmpersand(void);
    void vStripSingleQuotes(void);
    void vStripSemicolon(void);
    i8   nTokens(void);
    void vToString(Stz* szStr, i8 iFromTkn=0);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
