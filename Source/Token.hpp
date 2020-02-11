/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

namespace TokenStream
{
  class Token : public Object
  {
  private:
    eTN   m_eToken;  // kind of token
    s256  m_sTerm;   // term (aka text)
    i8    m_iLineNo; // line number where term can be found in input
    i8    m_iColNo;  // column number where term can be found in input
  
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
  
    i8    iColNo(void);
    void  vColNo(i8 iColNo);
    void  vConcat(Stz*);
    void  vCopy(Stz*);
    void  vCopy(Token*);
    bool  bHasText(void);
    bool  bIsInteger(void);
    bool  bIsName(void);
    bool  bIsNumeric(void);
    i8    iLength(void);
    i8    iLineNo(void);
    void  vLineNo(i8 iLineNo);
    void  vReset(void);
    bool  bSame(Stz*);
    void  vStripSingleQuotes(void);
    char* azTerm(void);
    Stz*  szTerm(void);
    i8    iToI8(void);
    eTN   eToken(void);
    void  vToken(eTN eToken);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
