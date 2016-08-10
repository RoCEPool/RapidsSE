/*============================================================================*/
/* Dir.cpp -- directory services                                              */
/*============================================================================*/

#include "Dir.hpp"

/*----------------------------------------------------------------------------*/
/* Dir: constructor                                                           */
/*----------------------------------------------------------------------------*/
Dir::Dir(void)
: cn(CN_DIR)
{
};

/*----------------------------------------------------------------------------*/
/* ~Dir: destructor                                                           */
/*----------------------------------------------------------------------------*/
Dir::~Dir(void)
{
  vInit();
}

/*----------------------------------------------------------------------------*/
/* test if directory path exists                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bExists(s256& rPath)
{
  m_xSysCall.vClear();
  
  struct stat xStat;

  int iCode = stat(rPath.a(),&xStat);

  if (0 == iCode && (xStat.st_mode & S_IFDIR)) {
    m_xSysCall.vSuccess();
    return true;
  }

  if (iCode && ENOENT == errno) {
    m_xSysCall.vSuccess();
    return false;
  }

  m_xSysCall.vError(errno);
  
  vError("Dir::bExists: stat(%s) error; errno=%d",rPath.a(),errno);

  return false;
}

/*----------------------------------------------------------------------------*/
/* get current working directory                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bGet(s256& rPath)
{
  m_xSysCall.vClear();
  
  char* aDirPath = getcwd(rPath.a(),rPath.iSize());

  m_sDirPath = aDirPath;
  
  if ( ! aDirPath ) {
    m_xSysCall.vError(errno);
    vWarn("Dir::bGet: getcwd() error; errno=%d",errno);
    return false;
  }
  
  m_xSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Dir::vInit(void)
{
  m_sDirPath = "";
  m_xSysCall.vClear();
}

/*----------------------------------------------------------------------------*/
/* set current working directory                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bSet(s256& rPath)
{
  m_xSysCall.vClear();
  i8 iCode = chdir(rPath.a());
  
  if (iCode) {
    m_xSysCall.vError(errno);
    vWarn("chdir() error; errno=%d",errno);
    return false;
  }

  m_xSysCall.vSuccess();
  return true;
}


///*---------------------------------------------------------------------------*/
///* vDeleteFiles: delete contents of named directory                          */
///*---------------------------------------------------------------------------*/
//void Dir::vDeleteFiles(const char* sDirPath, inw iDepth, Canon& canon)
//{
//  char wTestPath[File::C_PATH_MAX];
//  
//  DIR *pDir = opendir(sDirPath);
//  
//  if ( ! pDir ) {
//    canon.vError(errno);
//    vReportWarning("opendir() error; errno=%d",errno);
//    return;
//  }
//  
//  struct dirent *pDirEnt;
//  while ( (pDirEnt = readdir(pDir)) ) {
//    if (0 == Str::iCompare(pDirEnt->d_name,"."))
//      continue;
//    
//    if (0 == Str::iCompare(pDirEnt->d_name,".."))
//      continue;
//    
//    Str::vSprint(wTestPath,"%s/%s",sDirPath,pDirEnt->d_name);
//    
//    //if (DT_DIR == pDirEnt->d_type)
//    if ( bIsDir(wTestPath) )
//    {
//      char wSubDirPath[File::C_PATH_MAX];
//      Str::vSprint(wSubDirPath
//                    ,"%s/%s"
//                    ,sDirPath
//                    ,pDirEnt->d_name
//                    );
//      
//      vRemoveRecursive(wSubDirPath,canon);
//      
//      if ( ! canon.bIsSuccess() ) {
//        inw rCode = closedir(pDir);
//        if (-1 == rCode) {
//          vReportWarning("closedir() error; errno=%d",errno);
//          return;
//        }
//        return;
//      }
//      continue;
//    }
//    
//    char wFilePath[File::C_PATH_MAX];
//    Str::vSprint(wFilePath
//                  ,"%s/%s"
//                  ,sDirPath
//                  ,pDirEnt->d_name
//                  );
//    
//    Canon canon;
//    File::vDeleteFile(wFilePath,canon);
//    
//    if (canon.bIsError()) {
//      inw rCode = closedir(pDir);
//      if (-1 == rCode) {
//        vReportWarning("closedir() error; errno=%d",errno);
//        return;
//      }
//      return;
//    }
//  }
//  
//  inw rCode = closedir(pDir);
//  if (-1 == rCode) {
//    canon.vError(errno);
//    vReportWarning("closedir() error; errno=%d",errno);
//    return;
//  }
//  
//  if (iDepth) {
//    Dir::vRemoveDir(sDirPath,canon);
//    
//    if (canon.bIsError()) {
//      vReportWarning("Error while trying remove dir: %s",sDirPath);
//      return;
//    }
//    
//    if (canon.bIsFailure()) {
//      vReportWarning("Unable to remove dir: %s",sDirPath);
//      return;
//    }
//  }
//
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* vGetCwd: get current working directory                                    */
///*---------------------------------------------------------------------------*/
//void Dir::vGetCwd(char* sDirPath, inw nBytes)
//{
//  char* psDirPath = getcwd(sDirPath,nBytes);
//  
//  if ( ! psDirPath )
//    vReportWarning("getcwd() error; errno=%d",errno);
//}
//
///*---------------------------------------------------------------------------*/
///* pGetDirList: get dir list from directory (optionally recursive)           */
///*---------------------------------------------------------------------------*/
//DirList* Dir::pGetDirList(const char* sDirPath, bool bRecursive)
//{
//  DirList* pDirList = new DirList;
//  
//  vGetDirList(pDirList,sDirPath,bRecursive);
//  
//  return pDirList;
//}
//
///*---------------------------------------------------------------------------*/
///* pGetDirList: get dir list from directory (optionally recursive)           */
///*---------------------------------------------------------------------------*/
//void Dir::vGetDirList(DirList* pDirList, const char* sDirPath, bool bRecursive)
//{
//  char wTestPath[File::C_PATH_MAX];
//  
//  DIR *pDir = opendir(sDirPath);
//  
//  if ( ! pDir ) {
//    vReportWarning("opendir() error; errno=%d",errno);
//    return;
//  }
//  
//  struct dirent *pDirEnt;
//  while ( (pDirEnt = readdir(pDir)) ) {
//    if (0 == Str::iCompare(pDirEnt->d_name,"."))
//      continue;
//    
//    if (0 == Str::iCompare(pDirEnt->d_name,".."))
//      continue;
//    
//    Str::vSprint(wTestPath,"%s/%s",sDirPath,pDirEnt->d_name);
//    
//    if ( ! bIsDir(wTestPath)) {
//      vReportInfo("GetDirList: testpath not a directory %s",wTestPath);
//      continue;
//    }
//    
//    char wSubPath[File::C_PATH_MAX];
//    Str::vSprint(wSubPath,"%s/%s",sDirPath,pDirEnt->d_name);
//    
//    DirEntry* pDirEnt = new DirEntry(wSubPath);
//    
//    bool b = pDirList->bInsert(pDirEnt);
//    if ( ! b )
//      vReportError("Directory exists twice? : %s",wSubPath);
//    
//    if (!bRecursive)
//      continue;
//    
//    vGetDirList(pDirList,wSubPath,bRecursive);
//  }
//  
//  inw rCode = closedir(pDir);
//  
//  if (-1 == rCode)
//    vReportWarning("closedir() error; errno=%d",errno);
//}
//
///*---------------------------------------------------------------------------*/
///* pGetFileList: get file list from directory (optionally recursive)         */
///*---------------------------------------------------------------------------*/
//FileList* Dir::pGetFileList(const char* sDirPath, bool bRecursive)
//{
//  FileList* pFileList = new FileList;
//  
//  vGetFileList(pFileList,sDirPath,bRecursive);
//  
//  return pFileList;
//}
//
///*---------------------------------------------------------------------------*/
///* vGetFileList: get file list from directory (optionally recursive)         */
///*---------------------------------------------------------------------------*/
//void Dir::vGetFileList(FileList* pFileList, const char* sDirPath, bool bRecursive)
//{
//  char wTestPath[File::C_PATH_MAX];
//  
//  DIR *pDir = opendir(sDirPath);
//  
//  if ( ! pDir ) {
//    vReportWarning("opendir() error; errno=%d",errno);
//    return;
//  }
//  
//  struct dirent *pDirEnt;
//  while ( (pDirEnt = readdir(pDir)) ) {
//    if (0 == Str::iCompare(pDirEnt->d_name,"."))
//      continue;
//    
//    if (0 == Str::iCompare(pDirEnt->d_name,".."))
//      continue;
//    
//    Str::vSprint(wTestPath,"%s/%s",sDirPath,pDirEnt->d_name);
//    
//    //if (DT_DIR == pDirEnt->d_type)
//    if ( bIsDir(wTestPath) )
//    {
//      if ( ! bRecursive )
//        continue;
//      
//      char wSubPath[File::C_PATH_MAX];
//      Str::vSprint(wSubPath
//                    ,"%s/%s"
//                    ,sDirPath
//                    ,pDirEnt->d_name
//                    );
//      
//      vGetFileList(pFileList,wSubPath,bRecursive);
//      
//      continue;
//    }
//    
//    if ('.' == pDirEnt->d_name[0])
//      continue;
//    
//    //if (DT_REG != pDirEnt->d_type)
//    //  continue;
//  
//    if ( ! bIsReg(wTestPath) )
//      continue;
//    
//    // the d_name is the name of the file; not the dir name
//    FileEnt* pFileEnt = new FileEnt(sDirPath,pDirEnt->d_name);
//    
//    bool b = pFileList->bInsert(pFileEnt);
//    if ( ! b )
//      vReportError("GetFileList: file exists twice? : %s",pFileEnt->m_oFileName);
//  }
//  
//  inw rCode = closedir(pDir);
//  
//  if (-1 == rCode)
//    vReportWarning("closedir() error; errno=%d",errno);
//}
//
///*---------------------------------------------------------------------------*/
///* bIsDir: is given path a directory?                                        */
///*---------------------------------------------------------------------------*/
//bool Dir::bIsDir(const char* sDirPath)
//{
//  struct stat xStat;
//  
//  int rCode = lstat(sDirPath,&xStat);
//  
//  if (-1 == rCode) {
//    vReportWarning("lstat() error; errno=%d",errno);
//    return false;
//  }
//  
//  if (S_ISDIR(xStat.st_mode))
//    return true;
//  else
//    return false;
//}
//
///*---------------------------------------------------------------------------*/
///* bIsReg: is given path a regular file?                                     */
///*---------------------------------------------------------------------------*/
//bool Dir::bIsReg(const char* sFilePath)
//{
//  struct stat xStat;
//  
//  int rCode = lstat(sFilePath,&xStat);
//  
//  if (-1 == rCode) {
//    vReportWarning("lstat() error; errno=%d",errno);
//    return false;
//  }
//  
//  if (S_ISREG(xStat.st_mode))
//    return true;
//  else
//    return false;
//}
//
///*---------------------------------------------------------------------------*/
///* bMakeDir: make specified directory                                        */
///*---------------------------------------------------------------------------*/
//bool Dir::bMakeDir(const char* sDirPath)
//{
//  //! this needs reworking
//  
//  if (mkdir(sDirPath,0777) != 0 && EEXIST != errno) {
//    vReportWarning("mkdir() error; errno=%d",errno);
//    return false;
//  }
//  
//  return true;
//}
//
///*---------------------------------------------------------------------------*/
///* pFindFirst: find first file in list matching suffix                       */
///*---------------------------------------------------------------------------*/
//FileEnt* Dir::pFindFirst(Skip* pFileList, const char* sSuffix)
//{
//  bool bWasRemoved;
//  
//  FileEnt* pFileEnt = nullptr;
//  
//  while ( (pFileEnt = (FileEnt*)pFileList->pPop(&bWasRemoved,1)) ) {
//    
//    vReportInfo("%s  %s",pFileEnt->m_oDirName,pFileEnt->m_oFileName);
//    
//    if (Str::bEnds(pFileEnt->m_oFileName,sSuffix)) {
//      vReportInfo("found %s",pFileEnt->m_oFileName);
//      return pFileEnt;
//    }
//    
//  }
//  
//  return nullptr;
//}
//
///*---------------------------------------------------------------------------*/
///* vRemoveDir: remove named directory                                        */
///*---------------------------------------------------------------------------*/
//void Dir::vRemoveDir(const char* sDirPath, Canon& canon)
//{
//  int rCode = rmdir(sDirPath);
//  
//  if (-1 == rCode) {
//    canon.vError(errno);
//    vReportError("rmdir() error; errno=%d",errno);
//    return;
//  }
//  
//  if (0 == rCode) {
//    canon.vSuccess();
//    return;
//  }
//  
//  canon.vFailure();
//}
//
///*---------------------------------------------------------------------------*/
///* vRemoveRecursive: recursively remove named directory                      */
///*---------------------------------------------------------------------------*/
//void Dir::vRemoveRecursive(const char* sDirPath, Canon& canon)
//{
//  char wTestPath[File::C_PATH_MAX];
//  
//  DIR *pDir = opendir(sDirPath);
//  if ( ! pDir ) {
//    canon.vError(errno);
//    vReportWarning("opendir() error; errno=%d",errno);
//    return;
//  }
//  
//  struct dirent *pDirEnt;
//  while ( (pDirEnt = readdir(pDir)) )
//  {
//    if (0 == Str::iCompare(pDirEnt->d_name,"."))
//      continue;
//    
//    if (0 == Str::iCompare(pDirEnt->d_name,".."))
//      continue;
//    
//    Str::vSprint(wTestPath,"%s/%s",sDirPath,pDirEnt->d_name);
//    
//    //if (DT_DIR == pDirEnt->d_type)
//    if ( bIsDir(wTestPath) ) {
//      char wSubDirPath[File::C_PATH_MAX];
//      Str::vSprint(wSubDirPath
//                    ,"%s/%s"
//                    ,sDirPath
//                    ,pDirEnt->d_name
//                    );
//      
//      Canon canon;
//      vRemoveRecursive(wSubDirPath,canon);
//      
//      if ( ! canon.bIsSuccess() )
//      {
//        inw rCode = closedir(pDir);
//        if (-1 == rCode) {
//          vReportWarning("closedir() error; errno=%d",errno);
//          return;
//        }
//        return;
//      }
//      continue;
//    }
//    
//    char wFilePath[File::C_PATH_MAX];
//    Str::vSprint(wFilePath
//                  ,"%s/%s"
//                  ,sDirPath
//                  ,pDirEnt->d_name
//                  );
//    
//    Canon canon;
//    File::vDeleteFile(wFilePath,canon);
//    
//    if (canon.bIsError()) {
//      
//      inw rCode = closedir(pDir);
//      
//      if (-1 == rCode) {
//        vReportWarning("closedir() error; errno=%d",errno);
//        return;
//      }
//      return;
//    }
//  }
//  
//  inw rCode = closedir(pDir);
//  if (-1 == rCode) {
//    vReportWarning("closedir() error; errno=%d",errno);
//    return;
//  }
//  
//  Dir::vRemoveDir(sDirPath,canon);
//  
//  if (canon.bIsError()) {
//    vReportWarning("Error while trying remove dir: %s",sDirPath);
//    return;
//  }
//  
//  if (canon.bIsFailure()) {
//    vReportInfo("Unable to remove dir: %s",sDirPath);
//    return;
//  }
//  
//  canon.vSuccess();
//}
//
///*---------------------------------------------------------------------------*/
///* vDumpDirList: dump dir list                                               */
///*---------------------------------------------------------------------------*/
//void Dir::vDumpDirList(Skip* pDirList, Canon& canon)
//{
//  DirEntry* pDirEnt;
//  
//  SkipCursor x;
//  while ( (pDirEnt = (DirEntry*)pDirList->pEach(&x)) ) {
//    vReportInfo("DumpDirList: %s",pDirEnt->m_oDirPath);
//  }
//}
//
///*---------------------------------------------------------------------------*/
///* vDumpFileList: dump file list                                             */
///*---------------------------------------------------------------------------*/
//void Dir::vDumpFileList(Skip* pFileList, Canon& canon)
//{
//  FileEnt* pFileEnt;
//  
//  SkipCursor x;
//  while ( (pFileEnt = (FileEnt*)pFileList->pEach(&x)) ) {
//    vReportInfo("DumpFileList: %s/%s",
//                pFileEnt->m_oDirName,
//                pFileEnt->m_oFileName);
//  }
//}
//

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/