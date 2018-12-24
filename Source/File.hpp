/*============================================================================*/
/* File.hpp -- os file                                                        */
/*============================================================================*/
#pragma once


namespace System
{
  class Scratch;
  
  static const i8 C_PATH_MAX = 256;

  static const i8 C_NO_BUFFERING = 0;

  // Opens for reading. If the file does not exist or cannot be found, the
  // fopen call fails.
  static Stz* C_OPEN_READ = "r"_Stz;
  
  // Opens an empty file for writing. If the given file exists, its contents
  // are destroyed.
  static Stz* C_OPEN_WRITE = "w"_Stz;
  
  // Opens for writing at the end of the file (appending) without removing
  // the end-of-file (EOF) marker before new data is written to the file.
  // Creates the file if it does not exist.
  static Stz* C_OPEN_APPEND = "a"_Stz;
  
  // Opens for both reading and writing. The file must exist.
  static Stz* C_OPEN_READ_WRITE_EXIST = "r+"_Stz;
  
  // Opens an empty file for both reading and writing. If the file exists, its
  // contents are destroyed.
  static Stz* C_OPEN_READ_WRITE_CREATE = "w+"_Stz;
  
  // Opens for reading and appending. The appending operation includes the
  // removal of the EOF marker before new data is written to the file. The EOF
  // marker is not restored after writing is complete. Creates the file if it
  // does not exist.
  static Stz* C_OPEN_READ_APPEND = "a+"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
    bool bReadLine(Stz* y);
    void vReset(void);
    void vSeek(i8 iOffset);
    void vSeekEnd(void);
    void vSet(Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel=0);
    void vSet(FILE* pFilePtr, Stz* aFilePath, Stz* aOpenMode, i8 iTraceLevel=0);
    void vSetBuffering(i8 nBytes);
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
