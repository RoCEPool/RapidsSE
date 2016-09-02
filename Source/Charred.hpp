/*============================================================================*/
/* Charred.cpp -- input processor, removing line and block comments           */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Charred : public mn,
                  public cn
  {
  private:
    enum eIS { IS_NONE=0, IS_FILE,  IS_STDIN, IS_STRING };
    enum eCT { CT_NONE=0, CT_BLOCK, CT_LINE };
    
  private:
    bool   m_bMalloc;  // is this object in malloc heap (or small heap)?
    Charred* m_pChain; // next chard in chain
    
    eIS    m_eSource;  // what source are we charring? given file or given string?
    File   m_xFile;    // given file
    s256   m_sString;  // either string read from file or given string being charred
    char*  m_pChar;    // ptr into string
    char*  m_pEnd;     // m_pChar must be < m_pEnd (note its not <=)
    
    s256   m_sPrompt;  // user prompt when reading from stdin
    bool   m_bPrompt;  // prompt user for input

    // regs
    i8     m_nChars;   // 0: neither has char, 1: m_cOne has char, 2: m_cOne & m_cTwo
    char   m_cOne;     // first single char buffer
    char   m_cTwo;     // second single char buffer
    eCT    m_eComment; // comment type currently being processed
    
    
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
    void vClearRegs(void);
    char cConsumeOne(void);
    void vDiscardOne(void);
    void vGetFirst(void);
    void vGetMore(void);
    void vGetSecond(void);
    void vGetTwo(void);
    void vResetAll(void);
    void vRewindOne(char c);
    
    // Comment Processing
    bool bBlockCommentEnd(void);
    void vBlockCommentSkip(void);
    bool bBlockCommentStart(void);
    bool bPossibleComment(void);
    eCT  eCommentType(void);
    bool bLineCommentEnd(void);
    void vLineCommentSkip(void);
    bool bLineCommentStart(void);
    
    // Reading
    char  cNextChar(void);
    void  vReadLineFile(void);
    void  vReadLineStdin(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/