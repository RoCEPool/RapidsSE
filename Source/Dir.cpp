/*============================================================================*/
/* Dir.cpp -- directory services                                              */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

extern ResourceTracker gxResourceTracker;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Dir* System::_pDir(void* aAddr)
{
  Dir* pDir = reinterpret_cast<Dir*>(aAddr);
  
  RASSERT_TYPE(pDir,CN_DIR);
  
  return pDir;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Dir::operator new(size_t)
{
  void* pDir = HeapMem::pAllocate(CN_DIR);
  
  return pDir;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Dir::operator delete(void* pDirAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pDirAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dir::Dir(void)
: Object(CN_DIR)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_sDirPath   = "<none>"_Stz;
  
  m_aDir    = nullptr;
  m_sEntryName = "<none>"_Stz;
  m_iEntryType = 0;
  
  //m_oSysCall
  
  ASSERT(bIsClosed());
  
  _checkInitialised_(CN_DIR);
  
  _postcondition_(_MAGIC_,CN_DIR,_FOOTER_);
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dir::~Dir(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* test if directory path exists                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bExists(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_oSysCall.vClear();
  
  struct stat xStat;

  int iCode = stat(rPath.aChar(),&xStat);

  if (0 == iCode && (xStat.st_mode & S_IFDIR)) {
    m_oSysCall.vSuccess();
    return true;
  }

  if (iCode && ENOENT == errno) {
    m_oSysCall.vSuccess();
    return false;
  }

  m_oSysCall.vError(errno);
  
  vError("Dir::bExists: stat(%s) error; errno=%d"_Stz,rPath.aChar(),errno);

  return false;
}

/*----------------------------------------------------------------------------*/
/* get current working directory                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bGetCurrent(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_oSysCall.vClear();
  
  char* aDirPath = getcwd(rPath.aChar(),rPath.nAreaSize());

  m_sDirPath = (Stz*)aDirPath;
  
  if ( ! aDirPath ) {
    m_oSysCall.vError(errno);
    vWarn("Dir::bGet: getcwd() error; errno=%d"_Stz,errno);
    return false;
  }
  
  m_oSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* make specified directory                                                   */
/*----------------------------------------------------------------------------*/
bool Dir::bMake(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_oSysCall.vClear();
  
  char* aPath = rPath.aChar();

  if (mkdir(aPath,0777) != 0 && EEXIST != errno) {
    m_oSysCall.vError(errno);
    vWarn("Dir::bMake: mkdir() error; errno=%d"_Stz,errno);
    return false;
  }

  m_oSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove specified directory                                                 */
/*----------------------------------------------------------------------------*/
bool Dir::bRemove(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_oSysCall.vClear();
  
  char* aPath = rPath.aChar();
  
  int rCode = rmdir(aPath);

  if (-1 == rCode) {
    m_oSysCall.vError(errno);
    vWarn("Dir::bRemove: rmdir() error; errno=%d"_Stz,errno);
    return false;
  }

  m_oSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* change current working directory                                           */
/*----------------------------------------------------------------------------*/
bool Dir::bChangeCurrent(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_oSysCall.vClear();
  i8 iCode = chdir(rPath.aChar());
  
  if (iCode) {
    m_oSysCall.vError(errno);
    //vWarn("chdir() error; errno=%d\n"_Stz,errno);
    return false;
  }

  m_oSysCall.vSuccess();
  return true;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* close an open directory                                                    */
/*----------------------------------------------------------------------------*/
void Dir::vClose(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  
  i8 iResult = closedir(m_aDir);
  if (-1 == iResult) {
    m_oSysCall.vError(errno);
    vWarn("Dir::vClose: closedir() error; errno=%d"_Stz,errno);
    return;
  }

  gxResourceTracker.vDecDirOpen();

  m_aDir = nullptr;
  
  ASSERT(bIsClosed());
}

/*----------------------------------------------------------------------------*/
/* get entry name                                                             */
/*----------------------------------------------------------------------------*/
StrVal& Dir::sEntryName(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  return m_sEntryName;
}

/*----------------------------------------------------------------------------*/
/* get entry type                                                             */
/*----------------------------------------------------------------------------*/
i8 Dir::iEntryType(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  return m_iEntryType;
}

/*----------------------------------------------------------------------------*/
/* is this directory currently closed?                                        */
/*----------------------------------------------------------------------------*/
bool Dir::bIsClosed(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  if (! m_aDir)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this directory currently open?                                          */
/*----------------------------------------------------------------------------*/
bool Dir::bIsOpen(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  if (m_aDir)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* open the directory for reading                                             */
/*----------------------------------------------------------------------------*/
bool Dir::bOpen(Stz* aDirPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsClosed());
  
  m_sDirPath = aDirPath;
  
  char* wPath = m_sDirPath.aChar();
 
  m_aDir = opendir(wPath);
  if (! m_aDir) {
    m_oSysCall.vError(errno);
    vWarn("Dir::bOpen: opendir() error; errno=%d"_Stz,errno);
    return false;
  }

  gxResourceTracker.vIncDirOpen();

  ASSERT(bIsOpen());
  return true;
}

/*----------------------------------------------------------------------------*/
/* get directory path                                                         */
/*----------------------------------------------------------------------------*/
StrVal& Dir::sPath(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  return m_sDirPath;
}

/*----------------------------------------------------------------------------*/
/* read next directory entry                                                  */
/*----------------------------------------------------------------------------*/
bool Dir::bRead(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  m_oSysCall.vClear();
  
  struct dirent *pDirEnt;
  
  pDirEnt = readdir(m_aDir);
  
  if ( ! pDirEnt ) {
    m_sEntryName = "<none>"_Stz;
    m_iEntryType = 0;
    m_oSysCall.vSuccess();
    return false;
  }
  
  m_sEntryName = (Stz*)pDirEnt->d_name;
  
  m_iEntryType = pDirEnt->d_type;
  
  m_oSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* rewind within directory                                                    */
/*----------------------------------------------------------------------------*/
void Dir::vRewind(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  m_oSysCall.vClear();
  
  rewinddir(m_aDir);
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* seek within directory                                                      */
/*----------------------------------------------------------------------------*/
void Dir::vSeek(i8 iPosition)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  m_oSysCall.vClear();

  seekdir(m_aDir,iPosition);
  
  m_oSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* tell direcrtory position                                                  */
/*----------------------------------------------------------------------------*/
i8 Dir::iTell(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  m_oSysCall.vClear();
  
  i8 iPosition = telldir(m_aDir);
  
  m_oSysCall.vSuccess();
  return iPosition;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Dir::aFooter(void)
{
  _precondition_(_MAGIC_,CN_DIR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
