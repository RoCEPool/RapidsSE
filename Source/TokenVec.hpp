/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/
#pragma once

namespace Commands
{
  class TokenVec : public VectorNL_t<CN_TOKENVEC,Token,CN_TOKEN,C_TOKENS,u8>
  {
  private:
  
    bool m_bDetachedRequest; // is this a detached request
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenVec(void);
    ~TokenVec(void) noexcept(false);
  
    static TokenVec* _pTokenSet(void* aAddr);
  
    void vAddToken(Stz* aStr);
    void vAppendVector(TokenVec* pTokenVec);
    i8   iCountTokens(void);
    void vCutToken(i8 i_Tkn);
    bool bDetachedRequest(void);
    void vDetachedRequest(bool bDetachedRequest);
    void vDumpTokens(Stz* pTitle, i8 nTokens);
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
    void vToString(Stz* aBuff);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
