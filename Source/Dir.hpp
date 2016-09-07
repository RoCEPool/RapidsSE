/*============================================================================*/
/* Dir.hpp -- directory services                                              */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Dir : public Header
  {
  private:
    s256    m_sDirPath;   // the path of the directory
   
    DIR*    m_pDirPtr;    // dir pointer/descriptor
    s256    m_sEntryName; // name of entry just read
    i8      m_iEntryType; // type of entry just read
 
    SysCall m_xSysCall;   // system call handler
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Dir* pDir);
    void operator delete(void* pDirAddress);
    
    Dir(void);
   ~Dir(void);

    bool bExists(s256& rPath);
    bool bGetCurrent(s256& rPath);
    void vInit(void);
    bool bMake(s256& rPath);
    bool bRemove(s256& rPath);
    bool bSetCurrent(s256& rPath);
    
    //--
    
    void vClose(void);
    s&   sEntryName(void);
    i8   iEntryType(void);
    //bool bExists(void);
    //bool bGetCurrent(void);
    bool bIsClosed(void);
    bool bIsOpen(void);
    //bool bMake(void);
    bool bOpen(void);
    s&   sPath(void);
    void vPath(s256& sPath);
    bool bRead(void);
    //bool bRemove(void);
    void vRewind(void);
    void vSeek(i8 iPosition);
    //bool bSetCurrent(void);
    i8   iTell(void);
    
    

//class FileEnt;
//class DirList;
    
//    static bool   bChgCwd(const char* sDirPath);
//    static void   vDeleteFiles(const char* sDirPath, inw iDepth, Canon& canon);
//    static void   vGetCwd(char* sDirPath, inw nBytes);
//    static DirList* pGetDirList(const char* sDirPath, bool bRecursive);
//    static void   vGetDirList(DirList* pDirList, const char* sDirPath, bool bRecursive);
//    static FileList* pGetFileList(const char* sDirPath, bool bRecursive);
//    static void   vGetFileList(FileList* pList, const char* sDirPath, bool bRecursive);
//    static bool   bIsDir(const char* sDirPath);
//    static bool   bIsReg(const char* sFilePath);
//    static FileEnt* pFindFirst(Skip* pList, const char* sSuffix);
//    static void   vRemoveDir(const char* sDirPath, Canon& canon);
//    static void   vRemoveRecursive(const char* sDirPath, Canon& canon);
//    
//    static void   vDumpDirList(Skip* pDirList, Canon& canon);
//    static void   vDumpFileList(Skip* pFileList, Canon& canon);
  };

  Dir* _pDir(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
