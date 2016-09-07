/*============================================================================*/
/* Parser.cpp -- input parser, removing line and block comments              */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Parser : public Object
  {
  private:
    enum eIS { IS_NONE=0, IS_FILE,  IS_STDIN, IS_STRING };
    enum eCT { CT_NONE=0, CT_BLOCK, CT_LINE };
    
  private:
    bool    m_bMalloc;   // is this object in malloc heap (or small heap)?
    Parser* m_pChain;    // next parser in push chain
    i8      m_iDepth;    // depth of this parser
    s256    m_sPrompt;   // user prompt when reading from stdin
    
    // input sources
    eIS     m_eSource;   // what source are we parsing? given file or given string?
    File    m_xFile;     // given file
    bool    m_bLineMode; // line mode?
    s8K     m_sRawLine;  // either raw line read from file or given string
    char*   m_aRawLine;  // ptr into m_sLine.a() -- debug help
    char*   m_pRawChar;  // next raw char in raw line

    // registers
    i8      m_nLoaded;   // 0: neither has char, 1: m_cOne has char, 2: m_cOne & m_cTwo
    char    m_cFirst;    // first single char
    char    m_cSecond;   // second single char
    eCT     m_eComment;  // comment type currently being processed
    bool    m_bRead;     // prompt user for more input
    bool    m_bEOF;      // eof seen?
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, bool bMalloc);
    void operator delete(void* pDirAddress);
    
    Parser(void);
    ~Parser(void);
    
    // Options
    void vPrompt(s& sPrompt);
    void vLineMode(bool bLineMode);
    
    // Use What
    bool bUseFile(s& sPath);
    void vUseStdin(void);
    void vUseString(s& sString);
    void vUseEnd(void);

    // Parsing Stack
    bool bPop(void);
    void vPush(void);
    i8   iDepth(void);
    
    // Parsing
    char cGet(void);
    void vUnget(char c);
    bool bEOF(void);
    bool bLineMode(void);
    bool bStdin(void);

  private:
    
    void vResetParser(void);
    
    // Character Processing
    void vClearRegisters(void);
    char cConsumeFirst(void);
    void vDiscardFirst(void);
    void vLoadBoth(void);
    void vLoadFirst(void);
    void vLoadRegisters(void);
    void vLoadSecond(void);
    void vRewindRegisters(char c);
    
    // Comment Processing
    bool bBlockCommentEnd(void);
    void vBlockCommentSkip(void);
    bool bBlockCommentStart(void);
    bool bPossibleComment(void);
    eCT  eCommentType(void);
    bool bLineCommentEnd(void);
    void vLineCommentSkip(void);
    bool bLineCommentStart(void);
    void vStripComment(void);
    
    // Reading
    char  cFromRawInput(void);
    char  cFromRawFile(void);
    char  cFromRawStdin(void);
    char  cFromRawString(void);
    
    void  vReadLineFile(void);
    void  vReadLineStdin(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/