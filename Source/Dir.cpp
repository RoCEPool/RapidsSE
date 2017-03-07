/*============================================================================*/
/* Dir.cpp -- directory services                                              */
/*============================================================================*/

#include "System.h"

#include "StorageEngine.h"

Atomic gzDirOpenCount;

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Dir* System::_pDir(void* aAddr)
{
  Dir* pDir = reinterpret_cast<Dir*>(aAddr);
  
  RASSERT( !pDir || CN_DIR == pDir->eClass() );
  
  return pDir;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Dir::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pDir = Heap::pAllocate(CN_DIR);
  
  return pDir;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Dir::operator new(size_t iSize, Dir* pDir)
{
  RASSERT(iSize);
  return pDir;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Dir::operator delete(void* pDirAddress)
{
  Object* pObject = reinterpret_cast<Object*>(pDirAddress);
  
  Heap::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dir::Dir(void)
: Object(CN_DIR)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_sDirPath   = "<none>"_Str;
  
  m_pDirPtr    = nullptr;
  m_sEntryName = "<none>"_Str;
  m_iEntryType = 0;
  
  //m_xSysCall
  
  ASSERT(bIsClosed());
  
  _checkInitialised_(CN_DIR);
  
  _postcondition_(_MAGIC_,CN_DIR,_FOOTER_);
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dir::~Dir(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  vInit();
}

/*----------------------------------------------------------------------------*/
/* test if directory path exists                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bExists(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_xSysCall.vClear();
  
  struct stat xStat;

  int iCode = stat(rPath.aStr(),&xStat);

  if (0 == iCode && (xStat.st_mode & S_IFDIR)) {
    m_xSysCall.vSuccess();
    return true;
  }

  if (iCode && ENOENT == errno) {
    m_xSysCall.vSuccess();
    return false;
  }

  m_xSysCall.vError(errno);
  
  vError("Dir::bExists: stat(%s) error; errno=%d"_Str,rPath.aStr(),errno);

  return false;
}

/*----------------------------------------------------------------------------*/
/* get current working directory                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bGetCurrent(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_xSysCall.vClear();
  
  char* aDirPath = getcwd(rPath.aStr(),rPath.nAreaSize());

  m_sDirPath = aDirPath;
  
  if ( ! aDirPath ) {
    m_xSysCall.vError(errno);
    vWarn("Dir::bGet: getcwd() error; errno=%d"_Str,errno);
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
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_xSysCall.vSetMagic();
  
  m_sDirPath = ""_Str;
  m_xSysCall.vClear();
}

/*----------------------------------------------------------------------------*/
/* make specified directory                                                   */
/*----------------------------------------------------------------------------*/
bool Dir::bMake(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_xSysCall.vClear();
  
  char* aPath = rPath.aStr();

  if (mkdir(aPath,0777) != 0 && EEXIST != errno) {
    m_xSysCall.vError(errno);
    vWarn("Dir::bMake: mkdir() error; errno=%d"_Str,errno);
    return false;
  }

  m_xSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove specified directory                                                 */
/*----------------------------------------------------------------------------*/
bool Dir::bRemove(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_xSysCall.vClear();
  
  char* aPath = rPath.aStr();
  
  int rCode = rmdir(aPath);

  if (-1 == rCode) {
    m_xSysCall.vError(errno);
    vWarn("Dir::bRemove: rmdir() error; errno=%d"_Str,errno);
    return false;
  }

  m_xSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* change current working directory                                           */
/*----------------------------------------------------------------------------*/
bool Dir::bChangeCurrent(s256& rPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_xSysCall.vClear();
  i8 iCode = chdir(rPath.aStr());
  
  if (iCode) {
    m_xSysCall.vError(errno);
    //vWarn("chdir() error; errno=%d\n"_Str,errno);
    return false;
  }

  m_xSysCall.vSuccess();
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
  
  i8 iResult = closedir(m_pDirPtr);
  if (-1 == iResult) {
    m_xSysCall.vError(errno);
    vWarn("Dir::vClose: closedir() error; errno=%d"_Str,errno);
    return;
  }

  --gzDirOpenCount;

  m_pDirPtr = nullptr;
  
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
  
  if (! m_pDirPtr)
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
  
  if (m_pDirPtr)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* open the directory for reading                                             */
/*----------------------------------------------------------------------------*/
bool Dir::bOpen(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsClosed());
  
  char* wPath = m_sDirPath.aStr();
 
  m_pDirPtr = opendir(wPath);
  if (! m_pDirPtr) {
    m_xSysCall.vError(errno);
    vWarn("Dir::bOpen: opendir() error; errno=%d"_Str,errno);
    return false;
  }

  ++gzDirOpenCount;
  
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

/*---------------------------------------------------------------------------*/
/* set directory path                                                        */
/*---------------------------------------------------------------------------*/
void Dir::vPath(s256& sPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsClosed());
  
  m_sDirPath = sPath;
}

/*----------------------------------------------------------------------------*/
/* read next directory entry                                                  */
/*----------------------------------------------------------------------------*/
bool Dir::bRead(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  m_xSysCall.vClear();
  
  struct dirent *pDirEnt;
  
  pDirEnt = readdir(m_pDirPtr);
  
  if ( ! pDirEnt ) {
    m_sEntryName = "<none>"_Str;
    m_iEntryType = 0;
    m_xSysCall.vSuccess();
    return false;
  }
  
  m_sEntryName = pDirEnt->d_name;
  
  m_iEntryType = pDirEnt->d_type;
  
  m_xSysCall.vSuccess();
  return true;
}

/*----------------------------------------------------------------------------*/
/* rewind within directory                                                    */
/*----------------------------------------------------------------------------*/
void Dir::vRewind(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  m_xSysCall.vClear();
  
  rewinddir(m_pDirPtr);
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* seek within directory                                                      */
/*----------------------------------------------------------------------------*/
void Dir::vSeek(i8 iPosition)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  m_xSysCall.vClear();

  seekdir(m_pDirPtr,iPosition);
  
  m_xSysCall.vSuccess();
}

/*----------------------------------------------------------------------------*/
/* tell direcrtory position                                                  */
/*----------------------------------------------------------------------------*/
i8 Dir::iTell(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  ASSERT(bIsOpen());
  m_xSysCall.vClear();
  
  i8 iPosition = telldir(m_pDirPtr);
  
  m_xSysCall.vSuccess();
  return iPosition;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Dir::pFooter(void)
{
  _precondition_(_MAGIC_,CN_DIR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
