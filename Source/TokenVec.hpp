/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/
#pragma once

namespace Commands
{
  class TokenVec : public VectorNL_t<CN_TOKENVEC,Token,C_MAX_TOKENS>
  {
  private:
  
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
    void vCutToken(i8 i_Tkn);
    void vDumpTokens(void);
    i8   iFindToken(Stz* pLookFor);
    Token* pFindToken(Stz* aText, i8& i_Token);
    i8   nTokens(void);
    void vIncTokens(void);
    void vPrint(void);
    void vReset(void);
    void vSetAllTokens(void);
    void vSetTokens(void);
    void vStripAmpersand(void);
    void vStripSingleQuotes(void);
    void vStripSemicolon(void);
    void vToString(Stz* szStr, i8 iFromTkn=0);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
