/*============================================================================*/
/* Characterise.hpp -- read input as chars, removing line and block comments  */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  const char C_END_OF_LINE = '\n';
  

  class Characterise : public Object
  {
  private:
    i8      m_iViaMalloc; // is this object in malloc heap (or small heap)?
    i8      m_iDepth;     // depth of this parser
    Characterise* m_pNext;     // next parser in stack
    
    // source definition
    eIS     m_eSource;   // what source are we parsing? given file or string?
    s256    m_sSource;   // source
    bool    m_bLineMode; // line mode?
  
    // sources
    File    m_oRawFile;  // given file
    s48K    m_sRawLine;  // either raw line read from file or given string
    char*   m_azRawLine; // ptr into m_sRawLine -- debug help
    char*   m_acRawChar; // next raw char in raw line
    i8      m_iLineRaw;
    i8      m_iColRaw;

    // registers
    i8      m_nLoaded;   // 0: neither has char
                         // 1: m_cFirst has char
                         // 2: m_cFirst and m_cSecond each have a char
    AnnChar m_xAnnOne;   //
    AnnChar m_xAnnTwo;   //
    eCS     m_eComment;  // comment style currently being processed
    bool    m_bRead;     // prompt user for more input
    bool    m_bEOF;      // eof seen?
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, i8 iViaMalloc);
    void operator delete(void* pDirAddress);
    
    Characterise(Characterise* pNext);
   ~Characterise(void) noexcept(false);
    
    static Characterise* _pParser(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
   
    Stz* szSource(void);
  
    // Options
    void vLineMode(bool bLineMode);
    
    // Use What
    bool bUseFile(Stz* szPath);
    void vUseStdin(void);
    void vUseString(Stz* szString);
    void vUseEnd(void);

    // Characterise Stack
    Characterise* pNext(void);
    void    vNext(Characterise* pNext);
    i8      iDepth(void);
    
    // Processing Functions
    void vGet(AnnChar& rxAnnChar, bool bStripComments);
    void vUnget(AnnChar& rxAnnChar);
    bool bEOF(void);
    bool bLineMode(void);
    bool bStdin(void);

  private:
    
    void vResetParser(void);
    
    // Character Processing
    void vClearRegs(void);
    void vConsumeFirst(AnnChar& rxAnnChar);
    void vDiscardFirst(void);
    void vLoadBoth(void);
    void vLoadFirst(void);
    void vLoadRegs(bool bStripComments);
    void vLoadSecond(void);
    void vRewindRegs(AnnChar& rxAnnChar);
    
    // Comment Processing
    bool bBlockCommentEnd(void);
    void vBlockCommentSkip(void);
    bool bBlockCommentStart(void);
    bool bPossibleComment(void);
    eCS  eCommentStyle(void);
    bool bLineCommentEnd(void);
    void vLineCommentSkip(void);
    bool bLineCommentStart(void);
    void vStripComment(void);
    
    // Reading
    void  vRawCharFromInput(AnnChar& rxAnnChar);
    void  vRawCharFromFile(AnnChar& rxAnnChar);
    void  vRawCharFromStdin(AnnChar& rxAnnChar);
    void  vRawCharFromString(AnnChar& rxAnnChar);
    void  vNextCharFromRawLine(AnnChar& rxAnnChar);

    void  vReadRawLineFile(void);
    void  vReadRawLineStdin(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
