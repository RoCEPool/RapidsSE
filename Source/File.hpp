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
    FILE*   m_aFile;       // the os/lib file pointer
  
    SysCall m_xSysCall;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, File* pFile);
    void operator delete(void* pFileAddress);
  
    File(void);
   ~File() noexcept(false);
    
    static bool bExists(Stz *aFilePath);
    static bool bRemove(Stz* aFilePath);

    void vClose(void);
    char cGetChar(void);
    void vFileDump(void);
    void vFlush(void);
    bool bIsClosed(void);
    bool bIsOpen(void);
    bool bIsStdin(void);
    bool bIsStdout(void);
    bool bOpen(void);
    bool bOpen(Stz* aFilePath, Stz* aOpenMode);
    s256& sPath(void);
    void vPath(Stz* aFilePath);
    void vPrint(Stz* aFormat,...);
    i8   iReadBytes(void* aDst, i8 nBytes);
    bool bReadLine(Stz* y);
    void vReset(void);
    void vSeek(i8 iOffset);
    void vSeekEnd(void);
    void vSetBuffering(i8 nBytes);
    SysCall& rSysCall(void);
    i8   iTell(void);
    void vUngetChar(char cChar);
    void vUseStdin(void);
    bool bWriteBytes(void* aSrc, i8 nBytes);
  
    Footer* aFooter(void);
  };
  
  File* _pFile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
