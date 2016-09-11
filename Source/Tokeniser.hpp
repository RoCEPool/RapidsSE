/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
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
  
  static const char *pTK[]
  {
    /*  0 */ "NONE",
    /*  1 */ "INTEGER",
    /*  2 */ "NAME",
    /*  3 */ "OTHER",
    /*  4 */ "STRING",
    /*  5 */ "ENUMS",
  };
  
  class Tokeniser : public Object
  {
  private:
    Parser* m_pParser; // parser
    bool    m_bMalloc; // malloc or pool heap?
    bool    m_bNormal; // normal tokens?
    char    m_cStrippedChar; // character from parser, stripped of comments
    
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tokeniser(void);
    ~Tokeniser(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vPush(void);
    bool bPop(void);
    i8   iDepth(void);
    
    void vLineMode(bool bLineMode);
    void vMalloc(bool bMalloc);
    void vNormal(bool bNormal);
    void vPrompt(s& sPrompt);
    
    bool bUseFile(s& sPath);
    void vUseStdin(void);
    void vUseString(s& sString);
    void vUseEnd(void);

    eTK  eNextToken(char* wToken);
    void vResetTokens(char* pToken[], i8 nTokens);
    i8   iFindToken(const char* pLookFor, char* pToken[], i8 nTokens);
    i8   iCountTokens(char* pToken[], i8 nTokens);
    void vDumpTokens(const char* pTitle, char* pToken[], i8 nTokens);
    
    bool bEOF(void);
    bool bLineMode(void);
    bool bStdin(void);
    
  private:
    eTK  eNextNormal(char* wToken);
    eTK  eNextSpecial(char* wToken);
    
    eTK  eParseNormal(char* wToken);
    eTK  eParseNormalInteger(char* wToken);
    eTK  eParseNormalName(char* wToken);
    eTK  eParseNormalOther(char* wToken);
    eTK  eParseNormalString(char* wToken);
    
    eTK  eParseSpecial(char* wToken);
    eTK  eParseSpecialInteger(char* wToken);
    eTK  eParseSpecialName(char* wToken);
    eTK  eParseSpecialString(char* wToken);
    
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
    bool bIsHex(void);
    bool bIsX(void);
    bool bIsWhite(void);

    void vGetNextStripped(void);
    void vUngetStrippedChar(void);
    
    bool bEndInput(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/