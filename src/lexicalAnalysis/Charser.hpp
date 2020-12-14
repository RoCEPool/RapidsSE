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
    s32      m_sDivision; // division of prompt command file
    s32      m_sSection;  // section of prompt command file
    Word*    m_pInclude;  // the include file/group

    // source
    File*    m_pSrcFile;  // given file
    i8       m_iSrcLine;  // line number of raw char in source
    i8       m_iSrcCol;   // column number of raw char in source
  
    // raw buffer
    s48K     m_sRawBuff;  // raw buffer
    char*    m_azRawBuff; // ptr into m_sRawBuff -- debug help
    char*    m_acRawChar; // next raw char in raw buffer

    // use state
    bool     m_bUseBegun; // has use of this charser begun?
    bool     m_bUseClosed;// has use of this charser closed?
    bool     m_bUseEnded; // has use of this charser ended?
    bool     m_bUseOpened;// has use of this charser opened?

    // mode
    bool     m_bSingleLineCmd; // single line command?

    // registers
    i8       m_nLoaded;   // 0: neither register has char
                          // 1: m_xRegOne has char
                          // 2: m_xRegOne and m_xRegTwo each have a char
    AnnChar  m_xRegOne;   // first raw char register
    AnnChar  m_xRegTwo;   // second raw char register
    eCS      m_eComment;  // comment style currently being processed
    bool     m_bRead;     // prompt user for more input
    bool     m_bSeenEOF;  // eof seen?

    // some debugging aids
    bool      d_iDebugMode;  // debug mode?
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pDirAddress);
    
    Charser(Charser* pNext);
   ~Charser(void) noexcept(false);
    
    static Charser* _pParser(void* aAddr);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
   
    void vGetAnnChar(AnnChar& rxAnnChar, bool bStrip);
    void vUngetAnnChar(AnnChar& rxAnnChar);

    // Source
    bool bIsStdin(void);
    Stz* szPath(void);
    bool bSeenEOF(void);
    i8   iSrcCol(void);
    i8   iSrcLine(void);
  
    Stz* szDivision(void);
    void vDivision(Stz* szDivision);
    Stz* szSection(void);
    void vSection(Stz* szSection);

    // Control
    void vDebugMode(i8 iDebugMode);
    void vForkReset(void);
    void vInspect(void);
 
    // Mode
    bool bSingleLineCmd(void);
    void vSingleLineCmd(bool bLineMode);
    
    // Usage
    void vUseBegin(void);
    bool bUseBegun(void);
    void vUseClose(void);
    void vUseEnd(void);
    bool bUseFile(Stz* szPath);
    Word* pUseInclude(void);
    void vUseInclude(Word* pInclude);
    void vUseStdin(void);
    void vUseString(Stz* szString);

    // Charser Stack
    Charser* pNext(void);
    void     vNext(Charser* pNext);
    i8       iDepth(void);
    
    // debug
    i8       nLoaded(void);
    AnnChar* aRegOne(void);
    AnnChar* aRegTwo(void);
    char*    azRawBuff(void);
    char*    acRawChar(void);

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
    eCS  eCommentStyle(void);
    bool bLineCommentEnd(void);
    void vLineCommentSkip(void);
    bool bLineCommentStart(void);
    bool bPossibleComment(void);
    void vStripComment(void);
    
    // Loading Register With Next Raw char
    void vLoadRegWithRawChar(AnnChar& rxReg);
    void vLoadRegWithRawCharFromFile(AnnChar& rxReg);
    void vLoadRegWithRawCharFromStdin(AnnChar& rxReg);
    void vLoadRegWithRawCharFromString(AnnChar& rxReg);

    // Reading Next Raw Char
    void vReadRawCharFromRawBuff(AnnChar& rxReg);
    void vReadFileIntoRawBuff(void);
    void vReadStdinIntoRawBuff(void);
    
    Footer* aFooter(void);
  };

  Charser* _pCharser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
