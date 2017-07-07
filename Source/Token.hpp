/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

namespace Parsing
{
  enum eTN // token type
  {
  /*  0 */ TN_NONE=0,  // none
  /*  1 */ TN_INTEGER, // integer literal, e.g. 1, 42
  /*  2 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  3 */ TN_OTHER,   // other literal, e.g. {, ~
  /*  4 */ TN_STRING,  // string literal, e.g. "the answer"
  /*  5 */ TN_ENUMS
  };
  
  static Str* aTN[]
  {
  /*  0 */ "NONE"_Str,
  /*  1 */ "INTEGER"_Str,
  /*  2 */ "NAME"_Str,
  /*  3 */ "OTHER"_Str,
  /*  4 */ "STRING"_Str,
  /*  5 */ "ENUMS"_Str,
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
    ~Token(void);
  
    static Token* _pToken(void* aAddr);
  
    s256& sText(void);
    void vText(s256& sText);

    eTN eToken(void);
    void vToken(eTN eToken);
  
    void vReset(void);
  
    i8 iLength(void);
    bool bSame(Str*);
    bool bHasText(void);
    Str* aText(void);
    i8 iToI8(void);
    void vConcat(Str*);
    void vCopy(Str*);
    void vCopy(Token*);
    bool bIsNumeric(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
