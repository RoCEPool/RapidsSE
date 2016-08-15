/*============================================================================*/
/* Input.hpp - input stream handler                                            */
/*============================================================================*/

#pragma once

#include "Tasking.h"

namespace Tasking
{
  class InputStream : public ObjVal
  {
  private:
    i8           m_iDepth;
    bool         m_bFromFile;
    const char*  m_yScanText;
    eLT          m_eLiteralType;
    char         m_cChar;
    bool         m_bPrompt;
    File         m_xInputFile;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    InputStream(void);
    ~InputStream(void);
    
    i8   iDepth(void);
    void vDepth(i8 iDepth);
    
    eLT  eLiteralType(void);
    void vCloseFile(void);
    void vDiscardLine(void);
    void vNextLiteral(char* aWorkspace);
    bool bOpenFile(const char* aFilePath);
    void vPrompt(void);
    void vReset(void);
    void vScanText(void);
    void vScanText(s& sScanText);
    void vSetStdin(void);
    
  private:
    bool  bParseBlock(char* aWorkspace);
    bool  bParseComment(char* aWorkspace);
    bool  bParseInteger(char* aWorkspace);
    bool  bParseLine(char* aWorkspace);
    bool  bParseName(char* aWorkspace);
    bool  bParseString(char* aWorkspace);
    
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
