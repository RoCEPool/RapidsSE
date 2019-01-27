/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

namespace Commands
{
  enum eTN // token type
  {
  /*  0 */ TN_NONE=0,  // none
  /*  1 */ TN_INTEGER, // integer literal, e.g. 1, 42
  /*  2 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  3 */ TN_OTHER,   // other literal, e.g. {,~
  /*  4 */ TN_STRING,  // string literal, e.g. "the answer"
  /*  5 */ TN_ENUMS
  };
  
  static Stz* aTN[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "INTEGER"_Stz,
  /*  2 */ "NAME"_Stz,
  /*  3 */ "OTHER"_Stz,
  /*  4 */ "STRING"_Stz,
  /*  5 */ "ENUMS"_Stz,
  };
  
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
