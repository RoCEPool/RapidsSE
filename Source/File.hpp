/*============================================================================*/
/* File.hpp -- os file                                                        */
/*============================================================================*/
#pragma once

extern Stz* C_OPEN_READ;
extern Stz* C_OPEN_WRITE;
extern Stz* C_OPEN_APPEND;
extern Stz* C_OPEN_READ_WRITE_EXIST;
extern Stz* C_OPEN_READ_WRITE_CREATE;
extern Stz* C_OPEN_READ_APPEND;

namespace System
{
  class Scratch;
  
  static const i8 C_PATH_MAX = 256;
  
  class File : public Object
  {
  private:
    s256    m_sFilePath;   // the path of the file (relative or absolute)
    s32     m_sOpenMode;   // open mode
    i8      m_iTraceLevel;
    FILE*   m_pFile;
  
    SysCall m_xSysCall;
    
    char*   m_aLimit;
    char*   m_aNext;
    
    Scratch* m_pScratch;
    Scratch* m_pRecord;
    i8       m_iRecordLength;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, File* pFile);
    void operator delete(void* pFileAddress);
  
    File(void);
    File(Stz* aFilePath, i8 iTraceLevel=0);
    File(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel=0);
   ~File() noexcept(false);
    
    static bool bRemove(s256& rPath);
    
    void vBuffer(i8 nBytes);
    void vClear(void);
    void vClose(void);
    bool bExists(Stz *aFilePath);
    char cGetChar(void);
    void vFileDump(void);
    void vFlush(void);
    void vInit(void);
    bool bIsClosed(void);
    bool bIsOpen(void);
    bool bIsStdin(void);
    bool bIsStdout(void);
    bool bOpen(void);
    bool bOpen(Stz* aOpenMode, i8 iTraceLevel=0);
    bool bOpen(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel=0);
    s256& sPath(void);
    void vPath(Stz* aFilePath);
    void vPath(StrVal& sFilePath);
    void vPrint(Stz* aFormat,...);
    i8   iReadBytes(void* aDst, i8 nBytes);
    bool bReadLine(StrVal& s);
    bool bReadLine(Stz* y);
    bool bReadRecord(void);
    Scratch* pRecord(void);
    i8 iRecordLength(void);
    void vReset(void);
    void vSeek(i8 iOffset);
    void vSeekEnd(void);
    void vSet(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel=0);
    void vSet(FILE* pFilePtr, Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel=0);
    SysCall& rSysCall(void);
    i8   iTell(void);
    i8   iTraceLevel(void);
    void vTraceLevel(i8 iTraceLevel);
    void vUngetChar(char cChar);
    bool bWriteBytes(void* aSrc, i8 nSize);
    void vZero(void);
  
    Footer* aFooter(void);
  };
  
  File* _pFile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
