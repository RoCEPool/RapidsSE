/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class Token : public Object
  {
  private:
    i8    m_i_Token; // which token
    eTN   m_eToken;  // kind of token
    Stz*  m_szChars; // the charsed characters of this token
    i8    m_iLineNo; // line number where term can be found in input
    i8    m_iColNo;  // column number where term can be found in input

    s256  m__sCharsBuff; // temp buffer for charsed characters

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Token(void);
    Token(Token* pToCopy, TokenVec* pCaller);
   ~Token(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    void  vCopy_restricted(Stz* , TokenVec* pCaller);
    void  vCopy_restricted(Token* , TokenVec* pCaller);

    char* azChars(void);
    Stz*  szChars(void);
    i8    iColNo(void);
    void  vColNo(i8 iColNo);
    bool  bHasChars(void);
    void  vInitTkn(i8 i_Token);
    bool  bIsInteger(void);
    bool  bIsName(void);
    bool  bIsNumeric(void);
    i8    iLength(void);
    i8    iLineNo(void);
    void  vLineNo(i8 iLineNo);
    void  vReset(void);
    bool  bSame(Stz*);
    void  vStripSingleQuotes(void);
    i8    iToI8(void);
    eTN   eToken(void);
    void  vToken(eTN eToken);
  
    Footer* aFooter(void);
  };

  Token* _pToken(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
