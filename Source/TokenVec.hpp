/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/
#pragma once

namespace Parsing
{
  class TokenVec : public Vector<CN_TOKENVEC,Token,CN_TOKEN,C_TOKENS,u8>
  {
  private:
  
    bool m_bDetachedRequest; // is this a detached request
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenVec(void);
    ~TokenVec(void);
  
    static TokenVec* _pTokenSet(void* aAddr);
  
    void vAddToken(Stz* aStr);
    void vAppendVector(TokenVec* pTokenVec);
    bool bDetachedRequest(void);
    void vDetachedRequest(bool bDetachedRequest);
    i8   iFindToken(Stz* pLookFor);
    Token* pFindToken(Stz* aText, i8& i_Token);
    i8   nTokens(void);
    void vIncTokens(void);
    void vReset(void);
    void vSetAllTokens(void);
    void vSetTokens(void);
    void vStripAmpersand(void);
    void vStripSingleQuotes(void);
    void vStripSemicolon(void);


    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
