/*============================================================================*/
/* Charser.hpp -- read input as chars, removing line and block comments       */
/*============================================================================*/
#pragma once

namespace LexicalAnalysis
{
  const char C_END_OF_LINE = '\n';
  
  class Charser : public Object
  {
  private:
    i8       m_iDepth;    // depth of this parser
    Charser* m_pNext;     // next parser in stack
    
    // source definition
    eIS      m_eSource;   // what source are we parsing? given file or string?
    s256     m_sPath;     // the source path
  
    // sources
    File     m_oRawFile;  // given file
    s48K     m_sRawBuff;  // raw buffer
    char*    m_azRawBuff; // ptr into m_sRawBuff -- debug help
    char*    m_acRawChar; // next raw char in raw buffer
    i8       m_iLineRaw;  // line number of raw char
    i8       m_iColRaw;   // column number of raw char

    // uses
    bool     m_bUseBegun; // has use of this charser begun?
    bool     m_bUseClosed;// has use of this charser closed?
    bool     m_bUseEnded; // has use of this charser ended?
    bool     m_bUseOpened;// has use of this charser opened?
  
    // registers
    bool     m_bSingleLineCmd; // single line command?
    i8       m_nLoaded;   // 0: neither register has char
                          // 1: m_xRegOne has char
                          // 2: m_xRegOne and m_xRegTwo each have a char
    AnnChar  m_xRegOne;   // first raw char register
    AnnChar  m_xRegTwo;   // second raw char register
    eCS      m_eComment;  // comment style currently being processed
    bool     m_bRead;     // prompt user for more input
    bool     m_bSeenEOF;  // eof seen?
  
    // some debugging aids
    bool      d_bDebugMode;  // debug mode?
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pDirAddress);
    
    Charser(Charser* pNext);
   ~Charser(void) noexcept(false);
    
    static Charser* _pParser(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
   
    void vForkReset(void);
    void vInspect(void);
    Stz* szPath(void);
    i8   iLineRaw(void);
    i8   iLineCol(void);

    // Options
    void vSingleLineCmd(bool bLineMode);
    
    // Use What
    void vUseBegin(void);
    bool bUseBegun(void);
    void vUseClose(void);
    void vUseEnd(void);
    bool bUseFile(Stz* szPath);
    void vUseStdin(void);
    void vUseString(Stz* szString);

    // Charser Stack
    Charser* pNext(void);
    void     vNext(Charser* pNext);
    i8       iDepth(void);
    
    // Processing Functions
    void vGetAnnChar(AnnChar& rxAnnChar, bool bStripComments);
    void vUngetAnnChar(AnnChar& rxAnnChar);
    bool bSeenEOF(void);
    bool bSingleLineCmd(void);
    bool bIsStdin(void);

    void vDebugMode(bool bDebugMode);
  
  private:
    
    void vResetCharser(void);
    
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
    
    // Get next annotated char
    void vGetAnnChar(AnnChar& rxAnnChar);
    void vGetAnnCharFile(AnnChar& rxAnnChar);
    void vGetAnnCharStdin(AnnChar& rxAnnChar);
    void vGetAnnCharString(AnnChar& rxAnnChar);

    // Reading
    void vNextAnnCharFromRawBuff(AnnChar& rxAnnChar);
    void vReadFileIntoRawBuff(void);
    void vReadStdinIntoRawBuff(void);
    
    Footer* aFooter(void);
  };

  Charser* _pCharser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
