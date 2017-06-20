/*============================================================================*/
/* TokenSet.hpp -- set of tokens                                              */
/*============================================================================*/
#pragma once

namespace Parsing
{
  class TokenSet : public Object
  {
  private:
    TokenAry m_xTokenAry;  // array of tokens
    WspFxd<C_TOKEN_AREA> m_xTokenArea; // token work space
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TokenSet(void);
    ~TokenSet(void);
  
    static TokenSet* _pTokenSet(void* aAddr);
  
    Str*& operator [](i8 i_TokenNo);

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
