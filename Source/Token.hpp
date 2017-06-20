/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

namespace Parsing
{
  class Token : public Object
  {
  private:
    s256 m_sText;
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Token(void);
    ~Token(void);
  
    static Token* _pToken(void* aAddr);
  
    s256& sText(void);
    void vText(s256& sText);
  
    void vReset(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
