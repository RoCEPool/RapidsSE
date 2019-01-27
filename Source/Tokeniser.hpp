/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/
#pragma once

namespace Commands
{
  class Tokeniser : public Object
  {
  private:
    TokenVec  m_xTokenVec;  // token vector
    Parser*   m_pParser; // parser
  
    bool m_bMalloc; // malloc or pool heap?
    bool m_bNormal; // normal tokens?
    char m_cParsedChar;   // character from parser
    
    s48K m_sToken;  // current token
    eTN  m_eToken;  // token type

    s256  m_sPrompt;    // normal prompt
    s256  m_sContinue;  // continuation prompt

    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tokeniser(void);
    ~Tokeniser(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static TokenVec* pTokenise(s4K& sString);
    static TokenVec* pTokenise(s48K& sString);

    void vCollectStatement(void);
    i8   iDepth(void);
    bool bEOF(void);
    void vLineMode(bool bLineMode);
    bool bLineMode(void);
    void vMalloc(bool bMalloc);
    eTN  eNextToken(char* aToken);
    void vNextToken(void);
    void vNormal(bool bNormal);
    bool bPop(void);
    void vPrompt(s256& sPrompt);
    void vPush(void);
    bool bStdin(void);
    TokenVec* pTknVec(void);
    eTN  eToken(void);
    s48K& sToken(void);
    void vUseEnd(void);
    bool bUseFile(s256& sPath);
    void vUseStdin(void);
    void vUseString(s4K& sString);
    void vUseString(s48K& sString);
 
  private:
    eTN  eNextNormal(char* aToken);
    eTN  eNextSpecial(char* aToken);
    
    eTN  eParseNormal(char* aToken);
    eTN  eParseNormalInteger(char* aToken);
    eTN  eParseNormalName(char* aToken);
    eTN  eParseNormalOther(char* aToken);
    eTN  eParseNormalString(char* aToken);
    
    eTN  eParseSpecial(char* aToken);
    eTN  eParseSpecialInteger(char* aToken);
    eTN  eParseSpecialName(char* aToken);
    eTN  eParseSpecialString(char* aToken);
    
    void vSkipWhitespace(void);
    bool bHaveWhitespace(void);
    
    bool bStartNormalInteger(void);
    bool bStartNormalName(void);
    bool bStartNormalOther(void);
    bool bStartNormalString(void);
    bool bEndNormalString(void);

    bool bStartSpecialInteger(void);
    bool bStartSpecialName(void);
    bool bStartSpecialString(void);
    bool bEndSpecialString(void);
   
    bool bIsDigit(void);
    bool bIsDot(void);
    bool bIsEqual(void);
    bool bIsHex(void);
    bool bIsLessThan(void);
    bool bIsGreaterThan(void);
    bool bIsMinus(void);
    bool bIsX(void);
    bool bIsWhite(void);

    void vGetNext(void);
    void vGetNextStripped(void);
    void vUngetStrippedChar(void);
    
    bool bEndInput(void);
    
    Footer* aFooter(void);
  };
  
  Tokeniser* _pTokeniser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
