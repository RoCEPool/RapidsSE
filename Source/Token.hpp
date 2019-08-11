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
    Token(Token* pToCopy);
   ~Token(void) noexcept(false);
  
    static Token* _pToken(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void  vConcat(Stz*);
    void  vCopy(Stz*);
    void  vCopy(Token*);
    bool  bHasText(void);
    bool  bIsNumeric(void);
    i8    iLength(void);
    void  vReset(void);
    bool  bSame(Stz*);
    void  vStripSingleQuotes(void);
    Stz*  aText(void);
    s256& sText(void);
    void  vText(s256& sText);
    i8    iToI8(void);
    eTN   eToken(void);
    void  vToken(eTN eToken);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
