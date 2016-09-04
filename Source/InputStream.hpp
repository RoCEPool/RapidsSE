/*============================================================================*/
/* InputStream.hpp - Input Stream Handler                                     */
/*============================================================================*/

#pragma once

#include "Tasks.h"

namespace Tasks
{
  class InputStream : public ObjVal
  {
  private:
    i8           m_iDepth;        // depth of this input
    bool         m_bFromFile;     // input source is from a file?
    File         m_xInputFile;    // the input file
    char*        m_wInputLine;    // input line buffer
    char*        m_yInputCursor;  // input char cursor
    char         m_cCurrChar;     // current char
    bool         m_bFirstChar;    // first char of line has been parsed
    eLT          m_eLiteralType;  // type of literal just recognized
    const char*  m_aPrompt;       // the prompt
    bool         m_bPrompt;       // print prompt
    bool         m_bLineOriented; // line oriented?
    bool         m_bEndInput;     // end of input?
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    InputStream(void);
    ~InputStream(void);
    
    void vCloseFile(void);
    i8   iDepth(void);
    void vDepth(i8 iDepth);
    bool bFirstChar(void);
    bool bGetLine(s& sInputLine);
    void vLineOriented(bool bLineOriented);
    void vNextLine(void);
    eLT  eNextLiteral(char* wWorkspace);
    bool bOpenFile(s& sInputLine, const char* aFilePath);
    void vPrintPrompt(void);
    void vPrompt(char* aPrompt);
    void vReset(void);
    bool bStdin(void);
    void vUseFile(s& sInputText);
    void vUseStdin(s& sInputLine);
    void vUseText(s& sInputText);
    
  private:
    void  vGetChar(void);
    void  vUngetChar(void);
    
    bool  bParseComment(char* wWorkspace);
    bool  bParseInteger(char* wWorkspace);
    bool  bParseName(char* wWorkspace);
    bool  bParseString(char* wWorkspace);
    
    void  vSkipWhitespace(void);
    
    bool  bEndInput(void);
    bool  bMoreInput(void);
    bool  bStartComment(void);
    bool  bStartInteger(void);
    bool  bStartName(void);
    bool  bStartString(void);
    bool  bEndLine(void);
    bool  bEndString(void);
    bool  bIsDigit(void);
    bool  bIsHex(void);
    bool  bIsNull(void);
    bool  bIsSlash(void);
    bool  bIsWhite(void);
    bool  bIsX(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
