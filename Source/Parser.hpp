/*============================================================================*/
/* Parser.cpp -- input parser, removing line and block comments              */
/*============================================================================*/
#pragma once

namespace Parsing
{
  const char C_END_OF_LINE = '\n';
  
  class Parser : public Object
  {
  private:
    enum eIS { IS_NONE=0, IS_FILE,  IS_STDIN, IS_STRING };
    enum eCT { CT_NONE=0, CT_BLOCK, CT_LINE };
    
  private:
    bool    m_bMalloc;   // is this object in malloc heap (or small heap)?
    i8      m_iDepth;    // depth of this parser
    Parser* m_pNext;     // next parser in stack
    
    // input sources
    eIS     m_eSource;   // what source are we parsing? given file or given string?
    s256    m_sPrompt;   // user prompt when reading from stdin
    File    m_oFile;     // given file
    bool    m_bLineMode; // line mode?
    s48K    m_sRawLine;  // either raw line read from file or given string
    char*   m_aRawLine;  // ptr into m_sLine.s() -- debug help
    char*   m_pRawChar;  // next raw char in raw line

    // registers
    i8      m_nLoaded;   // 0: neither has char
                         // 1: m_cFirst has char
                         // 2: m_cFirst and m_cSecond each have a char
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
    
    Parser(Parser* pNext);
    ~Parser(void) noexcept(false);
    
    static Parser* _pParser(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
   
    
    // Options
    void vPrompt(s256& sPrompt);
    void vLineMode(bool bLineMode);
    
    // Use What
    bool bUseFile(s256& sPath);
    void vUseStdin(void);
    void vUseString(s4K& sString);
    void vUseString(s48K& sString);
    void vUseEnd(void);

    // Parsing Stack
    Parser* pNext(void);
    void    vNext(Parser* pNext);
    i8      iDepth(void);
    
    // Parsing
    char cGet(bool bStripComments);
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
    void vLoadRegisters(bool bStripComments);
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
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
