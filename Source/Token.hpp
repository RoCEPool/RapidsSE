/*============================================================================*/
/* Token.hpp -- Token                                                         */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  class Token : public Object
  {
  private:
    eTN   m_eToken;  // kind of token
    Stz*  m_szChars; // the charsed characters of this token
    i8    m_iLineNo; // line number where term can be found in input
    i8    m_iColNo;  // column number where term can be found in input

    TokenVec* m_p_TokenVec; // which token vector
    i8        m_i_Token;    // which token
    
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

    void  vCopyChars_restricted(Stz* szArg, TokenVec* pCaller);
    void  vCopyToken_restricted(Token* pToken_src, TokenVec* pCaller);
    void  vInit_restricted(i8 i_Token, TokenVec* pCaller);

    void  vAdjustChars(i8 nBytes);
    i8    nChars(void);
    char* azChars(void);
    Stz*  szChars(void);
    void  vChars(Stz* szChars);
    i8    iColNo(void);
    void  vColNo(i8 iColNo);
    bool  bHasChars(void);
    bool  bIsInteger(void);
    bool  bIsName(void);
    bool  bIsNumeric(void);
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
