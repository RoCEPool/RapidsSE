/*============================================================================*/
/* File.hpp -- os file                                                        */
/*============================================================================*/
#pragma once

extern Str* C_OPEN_READ;
extern Str* C_OPEN_WRITE;
extern Str* C_OPEN_APPEND;
extern Str* C_OPEN_READ_WRITE_EXIST;
extern Str* C_OPEN_READ_WRITE_CREATE;
extern Str* C_OPEN_READ_APPEND;

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
    FILE*   m_aFilePtr;
  
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
   ~File();
    
    static bool bRemove(s256& rPath);
    
    void vBuffer(i8 nBytes);
    void vClear(void);
    void vClose(void);
    bool bExists(Str *aFilePath);
    char cGetChar(void);
    void vFileDump(void);
    void vFlush(void);
    void vInit(void);
    bool bIsClosed(void);
    bool bIsOpen(void);
    bool bIsStdin(void);
    bool bIsStdout(void);
    bool bOpen(void);
    bool bOpen(Str* aOpenMode, i8 iTraceLevel=0);
    bool bOpen(Str* aFilePath, Str* aOpenMode, i8 iTraceLevel=0);
    s256& sPath(void);
    void vPath(Str* aFilePath);
    void vPath(StrVal& sFilePath);
    void vPrint(Str* aFormat,...);
    i8   iReadChunk(void* pChunk, i8 nBytes);
    bool bReadLine(StrVal& s);
    bool bReadLine(Str* y);
    bool bReadRecord(void);
    Scratch* pRecord(void);
    i8 iRecordLength(void);
    void vReset(void);
    void vSeek(i8 iOffset);
    void vSeekEnd(void);
    void vSet(Str* aFilePath, Str* aOpenMode, i8 iTraceLevel=0);
    void vSet(FILE* pFilePtr, Str* aFilePath, Str* aOpenMode, i8 iTraceLevel=0);
    SysCall& rSysCall(void);
    i8   iTell(void);
    i8   iTraceLevel(void);
    void vTraceLevel(i8 iTraceLevel);
    void vUngetChar(char cChar);
    void vZero(void);
  
    Footer* pFooter(void);
  };
  
  File* _pFile(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
