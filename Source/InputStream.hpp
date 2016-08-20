/*============================================================================*/
/* InputStream.hpp - Input Stream Handler                                     */
/*============================================================================*/

#pragma once

#include "Tasking.h"

namespace Tasking
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
  class InputStream : public ObjVal
  {
  private:
    i8           m_iDepth;        // depth of this input
    bool         m_bFromFile;     // input is from a file?
    File         m_xInputFile;    // the input file
    const char*  m_yInputText;    // the input text string
    char         m_cCurrChar;     // current char; 0 indicates EOF
    eLT          m_eLiteralType;  // type of literal just recognized
    bool         m_bPrompt;       // print prompt
    const char*  m_aPrompt;       // the prompt
    bool         m_bLineOriented; // line oriented? (temporary)
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    InputStream(void);
    ~InputStream(void);
    
    void vCloseFile(void);
    i8   iDepth(void);
    void vDepth(i8 iDepth);
    void vLineOriented(bool bLineOriented);
    eLT  eNextLiteral(char* wWorkspace);
    bool bOpenFile(const char* aFilePath);
    void vPrintPrompt(void);
    void vPrompt(char* aPrompt);
    void vReset(void);
    void vUseStdin(void);
    void vUseText(s& sScanText);
    
  private:
    bool  bParseComment(char* wWorkspace);
    bool  bParseInteger(char* wWorkspace);
    bool  bParseName(char* wWorkspace);
    bool  bParseString(char* wWorkspace);
    
    void  vGetChar(void);
    void  vUngetChar(void);
    
    void  vSkipWhitespace(void);
    
    bool  bEndInput(void);
    bool  bMoreInput(void);
    bool  bStartComment(void);
    bool  bStartInteger(void);
    bool  bStartString(void);
    bool  bEndLine(void);
    bool  bEndString(void);
    bool  bIsDigit(void);
    bool  bIsHex(void);
    bool  bIsWhite(void);
    bool  bIsX(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
