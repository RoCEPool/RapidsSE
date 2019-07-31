/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

namespace Commands
{
  class Token : public Object
  {
  private:
    eTN  m_eToken; // kind of token
    s256 m_sText;  // token text
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Token(void);
    ~Token(void) noexcept(false);
  
    static Token* _pToken(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    s256& sText(void);
    void vText(s256& sText);

    eTN eToken(void);
    void vToken(eTN eToken);
  
    void vReset(void);
  
    i8 iLength(void);
    bool bSame(Stz*);
    bool bHasText(void);
    Stz* aText(void);
    i8 iToI8(void);
    void vConcat(Stz*);
    void vCopy(Stz*);
    void vCopy(Token*);
    bool bIsNumeric(void);
  
    void vStripSingleQuotes(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
