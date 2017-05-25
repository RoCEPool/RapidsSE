/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
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
  
  class Tokeniser : public Object
  {
  private:
    Parser* m_pParser; // parser
    bool    m_bMalloc; // malloc or pool heap?
    bool    m_bNormal; // normal tokens?
    char    m_cParsedChar;   // character from parser
    
    s48K    m_sToken;  // current token
    eTK     m_eToken;  // token type
    
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tokeniser(void);
    ~Tokeniser(void);
    
    static Tokeniser* _pTokeniser(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
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

    eTK  eNextToken(char* aToken);
    void vNextToken(void);
    
    void vResetTokens(TokenAry* pTokenAry, Str* aToken[], i8 nTokens);
    i8   iFindToken(Str* pLookFor, TokenAry* pTokenAry, Str* aToken[], i8 nTokens);
    i8   iCountTokens(TokenAry* pTokenAry, Str* pToken[], i8 nTokens);
    void vDumpTokens(Str* pTitle, TokenAry* pTokenAry, Str* aToken[], i8 nTokens);
    
    bool bEOF(void);
    bool bLineMode(void);
    bool bStdin(void);
    
    s48K& sToken(void);
    eTK  eToken(void);
    
  private:
    eTK  eNextNormal(char* aToken);
    eTK  eNextSpecial(char* aToken);
    
    eTK  eParseNormal(char* aToken);
    eTK  eParseNormalInteger(char* aToken);
    eTK  eParseNormalName(char* aToken);
    eTK  eParseNormalOther(char* aToken);
    eTK  eParseNormalString(char* aToken);
    
    eTK  eParseSpecial(char* aToken);
    eTK  eParseSpecialInteger(char* aToken);
    eTK  eParseSpecialName(char* aToken);
    eTK  eParseSpecialString(char* aToken);
    
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
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
