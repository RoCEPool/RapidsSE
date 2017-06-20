/*============================================================================*/
/* TokenSet.hpp -- set of tokens                                              */
/*============================================================================*/
#pragma once

namespace Parsing
{
  class TokenSet : public Vector<CN_TOKENSET,Token,CN_TOKEN,256,u8>
  {
  private:
    
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenSet(void);
    ~TokenSet(void);
  
    static TokenSet* _pTokenSet(void* aAddr);
  
    void vAddToken(Str* aToken);
    i8   nTokens(void);
    void vIncTokens(void);
    void vReset(void);
    void vSetTokens(void);
    void vStripSemicolon(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
