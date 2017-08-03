/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/
#pragma once

namespace Parsing
{
  class Tokeniser : public Object
  {
  private:
    Parser* m_pParser; // parser
    bool    m_bMalloc; // malloc or pool heap?
    bool    m_bNormal; // normal tokens?
    char    m_cParsedChar;   // character from parser
    
    s48K    m_sToken;  // current token
    eTN     m_eToken;  // token type
    
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tokeniser(void);
    ~Tokeniser(void);
    
    static Tokeniser* _pTokeniser(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    static TokenVec* pTokenise(StrVal& sString);
  
    void vPush(void);
    bool bPop(void);
    i8   iDepth(void);
    
    void vLineMode(bool bLineMode);
    void vMalloc(bool bMalloc);
    void vNormal(bool bNormal);
    void vPrompt(StrVal& sPrompt);
    
    bool bUseFile(StrVal& sPath);
    void vUseStdin(void);
    void vUseString(StrVal& sString);
    void vUseEnd(void);

    eTN  eNextToken(char* aToken);
    void vNextToken(void);
    
    i8   iFindToken(Stz* pLookFor, TokenVec& xTokenVec, i8 nTokens);
    i8   iCountTokens(TokenVec& xTokenVec, i8 nTokens);
    void vDumpTokens(Stz* pTitle, TokenVec& xTokenVec, i8 nTokens);
    
    bool bEOF(void);
    bool bLineMode(void);
    bool bStdin(void);
    
    s48K& sToken(void);
    eTN  eToken(void);
    
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
    bool bIsHex(void);
    bool bIsX(void);
    bool bIsWhite(void);

    void vGetNext(void);
    void vGetNextStripped(void);
    void vUngetStrippedChar(void);
    
    bool bEndInput(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
