/*============================================================================*/
/* File.hpp -- os file                                                        */
/*============================================================================*/
#pragma once

#include "Common.h"

extern const char* C_OPEN_READ;
extern const char* C_OPEN_WRITE;
extern const char* C_OPEN_APPEND;
extern const char* C_OPEN_READ_WRITE_EXIST;
extern const char* C_OPEN_READ_WRITE_CREATE;
extern const char* C_OPEN_READ_APPEND;

namespace Common
{
  static const i8 C_PATH_MAX = 256;
  
//  static const char* C_OPEN_READ;
//  static const char* C_OPEN_WRITE;
//  static const char* C_OPEN_APPEND;
//  static const char* C_OPEN_READ_WRITE_EXIST;
//  static const char* C_OPEN_READ_WRITE_CREATE;
//  static const char* C_OPEN_READ_APPEND;

  class File : public Header
  {
  private:
    static const i8 C_WORKAREA = C_256KB;
    
    s256    m_sFilePath;   // the path of the file (relative or absolute)
    s32     m_sOpenMode;   // open mode
    i8      m_iTraceLevel;
    FILE*   m_pFilePtr;
  
    SysCall m_xSysCall;
    
    char*   m_pLimit;
    char*   m_pNext;
    char*   m_pWorkarea;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, File* pFile);
    void operator delete(void* pFileAddress);
    File(void);
   ~File();
    
    void vBuffer(i8 nBytes);
    void vClear(void);
    void vClose(void);
    bool bExists(const char *aFilePath);
    char cGetChar(void);
    void vDump(void);
    void vFlush(void);
    void vInit(void);
    bool bIsClosed(void);
    bool bIsOpen(void);
    bool bIsStdin(void);
    bool bIsStdout(void);
    bool bOpen(void);
    bool bOpen(const char* aOpenMode, i8 iTraceLevel=0);
    bool bOpen(const char* aFilePath, const char* aOpenMode, i8 iTraceLevel=0);
    s256& sPath(void);
    void vPath(const char* aFilePath);
    void vPath(s& sFilePath);
    void vPrint(const char* sFormat,...);
    i8   iReadChunk(void* pChunk, i8 nBytes);
    bool bReadLine(s& s);
    bool bReadLine(char* y);
    void vReset(void);
    void vSeek(i8 iOffset);
    void vSeekEnd(void);
    void vSet(const char* aFilePath, const char* aOpenMode, i8 iTraceLevel=0);
    void vSet(FILE* pFilePtr, const char* aFilePath, const char* aOpenMode, i8 iTraceLevel=0);
    SysCall& rSysCall(void);
    i8   iTell(void);
    i8   iTraceLevel(void);
    void vTraceLevel(i8 iTraceLevel);
    void vUngetChar(char cChar);
    void vZero(void);
  
    Footer* pFooter(void);
  //    File(const char* aFilePath, const char* aOpenMode, i8 iTraceLevel);
  //    File(FILE* pFile, const char* aFilePath, const char* saOpenMode, i8 iTraceLevel);
  
//
//    void  vChmod(SysCall& SysCall);
//    void  vClose(SysCall& SysCall);
//    const char* sFileName(void);
//    void  vFlush(SysCall& SysCall);
//    i8    iGetPos(SysCall& SysCall);
//    void  vInclude(const char* sString, SysCall& SysCall);
//    bool  bOpened(void);
//    void  vPrint(SysCall& SysCall, const char* sFormat,...);
//    void  vPrintArgs(SysCall& SysCall, const char* sFormat,va_list* pxArgs);
//    void  vRead(void* pBuffer, i8 nBytes, SysCall& SysCall);
//    i8    iReadInt(SysCall& SysCall);
//    i8    iReadLine(SysCall& SysCall);
//    void  vReadLine(char* w, SysCall& SysCall);
//    void  vSeek(i8 iOffset, SysCall& SysCall);
//    i8    iSize(SysCall& SysCall);
//    void  vSkipLine(SysCall& SysCall);
//    void  vWrite(void* pBuffer, i8 nBytes, SysCall& SysCall);
//    void  vWriteCS(void* pBuffer, i8 nBytes, SysCall& SysCall);
//    
//    static i8   iSizeFile(const char* aFilePath, SysCall& SysCall);
//    static void vDeleteFile(const char* aFilePath, SysCall& SysCall);
//    static void vExtractFileName(char* aFileName, const char* aFilePath);
//    static void vRename(const char *aOld, const char *aNew, SysCall& SysCall);
//    
//    static FILE* pOpen(const char* aFilePath, const char* aMode);
//    
//    /////////////////////////////////////////////////////////////////////////////
//    
//    bool bGet(char* w, i8 nSize);
//  
//    bool bPut(const char* w);
//    void vPut(const char* w);
//
//    void vPrint(const char* sFormat,...);
//    
  };
  
  File* _pFile(void* pAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
