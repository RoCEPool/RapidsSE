/*============================================================================*/
/* Dir.cpp -- directory services                                              */
/*============================================================================*/

#include "Resources.h"


Atomic gixDir;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Dir* Resources::_pDir(void* aAddr)
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
Dir::Dir(Focal* pIsFor)
: Resource(CN_DIR,pIsFor)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_sDirPath   = C_NO_NAME;
  
  m_aDir       = nullptr;
  m_sEntryNm   = C_NO_NAME;
  m_iEntryType = 0;
  
  //m_oSysCall
  
  ASSERT(bIsClosed());
  
  _checkInitialised_(CN_DIR);
  
  ++ gixDir;
  AM::vLogRes(SL_INF,this,RA_DIR_CONSTRUCT,
              "Dir construct : %p (%lld dirs exist)"_Stz, //!format-LogRes
              this,* gixDir);
  
  _postcondition_(_MAGIC_,CN_DIR,_FOOTER_);
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dir::~Dir(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  if (m_aDir) {
    vWarn("Dir::~Dir dir not closed: %s\n"_Stz,m_sDirPath.azChar());

    vClose();
    
    if (m_oSysCall.bHadError())
      vWarn("Dir::~Dir: dir could not be closed: %s"_Stz,m_sDirPath.azChar());
  }

  
  -- gixDir;
  AM::vLogRes(SL_INF,this,RA_DIR_DESTRUCT,
              "Dir destruct : %p (%lld dirs exist)"_Stz, //!format-LogRes
              this,* gixDir);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* test if given dir has access permissions                                  */
/*---------------------------------------------------------------------------*/
bool Dir::bAccess(Stz* szDirPath, i8 iPermission)
{
  int iResult = access(szDirPath->azChar(),(int)iPermission);

  if (0 == iResult)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* change working directory of the process                                    */
/*----------------------------------------------------------------------------*/
bool Dir::bChangeCurrent(Stz* aDirPath)
{
  i8 iResult = chdir(aDirPath->azChar());
  
  if (iResult) {
    vWarn("chdir(%s) error; errno=%d\n"_Stz,aDirPath->azChar(),errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get current working directory                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bGetCurrent(Stz* aDirPath)
{
  char* aCurrPath = getcwd(aDirPath->azChar(),C_MAX_PATH);
  
  if ( ! aCurrPath ) {
    vWarn("Dir::bGet: getcwd() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if directory path exists                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bExists(Stz* aDirPath)
{
  struct stat xStat;
  
  int iResult = stat(aDirPath->azChar(),&xStat);
  
  if (0 == iResult && (xStat.st_mode & S_IFDIR))
    return true;
  
  if (iResult && ENOENT == errno)
    return false;
    
  vWarn("Dir::bExists: stat(%s) error; errno=%d"_Stz,aDirPath->azChar(),errno);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* make specified directory                                                   */
/*----------------------------------------------------------------------------*/
bool Dir::bMake(Stz* aDirPath)
{
  char* aPath = aDirPath->azChar();
  
  if (mkdir(aPath,0777) != 0 && EEXIST != errno) {
    vWarn("Dir::bMake: mkdir() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove specified directory                                                 */
/*----------------------------------------------------------------------------*/
bool Dir::bRemove(Stz* aDirPath)
{
  char* aPath = aDirPath->azChar();
  
  int iResult = rmdir(aPath);
  
  if (-1 == iResult) {
    vWarn("Dir::bRemove: rmdir() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bAttached()) AM::pResourceTracker()->vDecDirOpen();
  
  AM::vLogRes(SL_INF,this,RA_DIR_CLOSED,
              "Dir closed : %s"_Stz, //!format-LogRes
              m_sDirPath.szStz());

  m_aDir = nullptr;
  
  ASSERT(bIsClosed());
}

/*----------------------------------------------------------------------------*/
/* get dir entry name                                                         */
/*----------------------------------------------------------------------------*/
Stz* Dir::szEntryNm(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  return m_sEntryNm;
}

/*----------------------------------------------------------------------------*/
/* get dir entry type                                                         */
/*----------------------------------------------------------------------------*/
i8 Dir::iEntryType(void)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  return m_iEntryType;
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Dir::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIR,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
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
  
  char* wPath = m_sDirPath.azChar();
 
  m_aDir = opendir(wPath);
  if (! m_aDir) {
    m_oSysCall.vError(errno);
    vWarn("Dir::bOpen: opendir() error; errno=%d"_Stz,errno);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (AM::bAttached()) AM::pResourceTracker()->vIncDirOpen();
  
  AM::vLogRes(SL_INF,this,RA_DIR_OPENED,
              "Dir opened : %s"_Stz, //!format-LogRes
              aDirPath);

  ASSERT(bIsOpen());
  return true;
}

/*----------------------------------------------------------------------------*/
/* get directory path                                                         */
/*----------------------------------------------------------------------------*/
Stz* Dir::szPath(void)
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
    m_sEntryNm   = C_NO_NAME;
    m_iEntryType = 0;
    m_oSysCall.vSuccess();
    return false;
  }
  
  m_sEntryNm = (Stz*)pDirEnt->d_name;
  
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
/* tell directory position                                                    */
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
