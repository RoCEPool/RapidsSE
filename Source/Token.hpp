/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

namespace Parsing
{
  enum eTK // token type
  {
  /*  0 */ TK_NONE=0,  // none
  /*  1 */ TK_INTEGER, // integer literal, e.g. 1, 42
  /*  2 */ TK_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  3 */ TK_OTHER,   // other literal, e.g. {, ~
  /*  4 */ TK_STRING,  // string literal, e.g. "the answer"
  /*  5 */ TK_ENUMS
  };
  
  static Str* aTK[]
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
    eTK  m_eToken; // kind of token
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

    eTK eToken(void);
    void vToken(eTK eToken);
  
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
