/*============================================================================*/
/* Dir.hpp -- directory services                                              */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common {
  
  //class FileEnt;
  //class DirList;

  class Dir : public mn,
              public cn
  {
  private:
    s256    m_sDirPath;   // the path of the directory
    SysCall m_xSysCall;

  public:
    Dir(void);
   ~Dir(void);

    bool bExists(s256& rPath);
    bool bGet(s256& rPath);
    void vInit(void);
    bool bMake(s256& rPath);
    bool bRemove(s256& rPath);
    bool bSet(s256& rPath);

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

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
