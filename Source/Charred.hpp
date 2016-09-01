/*============================================================================*/
/* Charred.hpp -- char input processor which removes line abd block comments  */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Charred : public mn,
                  public cn
  {
  private:
    enum eCW { CW_NONE=0, CW_FILE,  CW_STDIN, CW_STRING };
    enum eCT { CT_NONE=0, CT_BLOCK, CT_LINE };
    
  private:
    bool   m_bMalloc;  // is this object in malloc heap (or small heap)?
    eCW    m_eWhich;   // what are we charring? given file or given string?
    File   m_xFile;    // given file
    s256   m_sString;  // either string read from file or given string being charred
    char*  m_pChar;    // ptr into string
    
    s256   m_sPrompt;  // user prompt when reading from stdin
    bool   m_bPrompt;  // prompt user for input

    // regs
    i8     m_nChars;   // 0: neither has char, 1: m_cOne has char, 2: m_cOne & m_cTwo
    char   m_cOne;     // first single char buffer
    char   m_cTwo;     // second single char buffer
    eCT    m_eComment; // comment type currently being processed
    
    Charred* m_pChain; // next chard in chain
    
  public:
    void* operator new(size_t, bool bMalloc);
    void operator delete(void* pDirAddress);
    
    Charred(void);
    ~Charred(void);
    
    void vBeginUse(void);
    void vEndUse(void);
    void vPrompt(s& sPrompt);
    bool bUseFile(s& sPath);
    void vUseStdin(void);
    void vUseString(s& sString);

    char cGet(void);
    void vUnget(char c);
    
  private:
    
    // Character Processing
    void vAdvanceOne(void);
    void vConsumeOne(void);
    void vDiscardOne(void);
    void vRewindOne(char c);
    
    // Register Management
    void vClearRegs(void);
    void vGetMore(void);
    void vReset(void);
    
    // Comment Processing
    bool bBlockCommentEnd(void);
    bool bBlockCommentStart(void);
    bool bCommentStart(void);
    eCT  eCommentType(void);
    bool bLineCommentEnd(void);
    bool bLineCommentStart(void);
    void vSkipBlockComment(void);
    void vSkipLineComment(void);
    
    // File Reading
    bool bEndLine(void);
    void vReadLineFile(void);
    void vReadLineStdin(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/