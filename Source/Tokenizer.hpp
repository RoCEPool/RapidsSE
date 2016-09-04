/*============================================================================*/
/* Tokenizer.hpp -- Tokenizer : divide input stream into tokens               */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  enum eLT // literal type
  {
    /*  0 */ LT_NONE=0,  // none
    /*  1 */ LT_NAMELIT, // name literal, e.g. WASHER or ++ etc
    /*  2 */ LT_INTLIT,  // int literal, e.g. 1, 42
    /*  3 */ LT_STRLIT,  // str litera, e.g. "the answer"
    /*  4 */ LT_ENUMS
  };
  
  static const char *pLT[]
  {
    /*  0 */ "NONE",
    /*  1 */ "NAMELIT",
    /*  2 */ "INTLIT",
    /*  3 */ "STRLIT",
    /*  4 */ "ENUMS",
  };
  class Tokenizer : public mn,
                    public cn
  {
  private:
    Parser* m_pParser;   // parser
    bool    m_bMalloc;   // malloc or pool heap?
    bool    m_bNormal;   // normal tokens?
    
    s32     m_sPrompt;   // prompt
    char    m_cCurrChar; // current char
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tokenizer(void);
    ~Tokenizer(void);
    
    void vPush(void);
    bool bPop(void);
    i8   iDepth(void);
    
    void vLineMode(bool bLineMode);
    void vMalloc(bool bMalloc);
    void vNormal(bool bNormal);
    void vPrompt(const char* pPrompt);
    void vBegin(void);
    void vEnd(void);
    
    bool bUseFile(s& sPath);
    void vUseStdin(void);
    void vUseString(s& sString);

    eLT  eNextLiteral(char* wLiteral);
    
    bool bEOF(void);
    
  private:
    eLT  eNextNormal(char* wLiteral);
    eLT  eNextSpecial(char* wLiteral);
    
    eLT  eParseNormal(char* wLiteral);
    eLT  eParseNormalInteger(char* wLiteral);
    eLT  eParseNormalName(char* wLiteral);
    eLT  eParseNormalString(char* wLiteral);
    
    eLT  eParseSpecial(char* wLiteral);
    eLT  eParseSpecialInteger(char* wLiteral);
    eLT  eParseSpecialName(char* wLiteral);
    eLT  eParseSpecialString(char* wLiteral);
    
    void vSkipWhitespace(void);
    
    bool bStartNormalInteger(void);
    bool bStartNormalName(void);
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

    void vGetChar(void);
    void vUngetChar(void);
    
    bool bEndInput(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/