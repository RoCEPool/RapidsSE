/*============================================================================*/
/* Charred.cpp -- input parser, removing line and block comments              */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Parser : public mn,
                 public cn
  {
  private:
    enum eIS { IS_NONE=0, IS_FILE,  IS_STDIN, IS_STRING };
    enum eCT { CT_NONE=0, CT_BLOCK, CT_LINE };
    
  private:
    bool    m_bMalloc;   // is this object in malloc heap (or small heap)?
    i8      m_iDepth;    // depth of this parser
    Parser* m_pChain;    // next parser in push chain
    s256    m_sPrompt;   // user prompt when reading from stdin
    
    // input sources
    eIS     m_eSource;   // what source are we parsing? given file or given string?
    File    m_xFile;     // given file
    bool    m_bEOF;      // eof hit?
    s256    m_sRawLine;  // either line read from file or the given string being parsed
    char*   m_aRawLine;  // ptr into m_sLine.a() -- debug help
    char*   m_pRawInput; // next input char (ptr into line)
    bool    m_bLineMode; // line mode
    bool    m_bPrompt;   // prompt user for more input

    // registers
    i8      m_nLoaded;   // 0: neither has char, 1: m_cOne has char, 2: m_cOne & m_cTwo
    char    m_cFirst;    // first single char
    char    m_cSecond;   // second single char
    eCT     m_eComment;  // comment type currently being processed
    
    
  public:
    void* operator new(size_t, bool bMalloc);
    void operator delete(void* pDirAddress);
    
    Parser(void);
    ~Parser(void);
    
    void vBeginParsing(void);
    void vEndParsing(void);
    void vPrompt(s& sPrompt);
    void vLineMode(bool bLineMode);
    bool bUseFile(s& sPath);
    void vUseStdin(void);
    void vUseString(s& sString);

    char cGet(void);
    void vUnget(char c);

    bool bPop(void);
    void vPush(void);
    i8   iDepth(void);
    bool bEOF(void);
    
    void vZero(void);
    
  private:
    
    void vDepth(i8 iDepth);
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
    void  vReadLineFile(void);
    void  vReadLineStdin(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/