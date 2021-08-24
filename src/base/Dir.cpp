/*============================================================================*/
/* Dir.cpp -- directory services                                              */
/*============================================================================*/

#include "BASE.h"

#include "Dir.hpp" //?
#include "Resources.hpp" //?

#include <sys/stat.h>

#include "MY.hpp"

Atomic gixDir;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Dir,CN_DIR)
__aNewFunction(Dir,CN_DIR)
__vDeleteFunction(Dir,CN_DIR)
__aFooterFunction(Dir)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dir::Dir(Object* pIsFor)
: Resource(CN_DIR,pIsFor)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  m_aDir       = nullptr;
  m_sEntryNm   = C_NO_NAME;
  m_iEntryType = 0;
  
  //m_oSysCall
  
  ASSERT(bIsClosed());
  
  _checkInitialised_(CN_DIR);
  
  ++ gixDir;
  
  _postcondition_(_MAGIC_,CN_DIR,_FOOTER_);
};

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dir::~Dir(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
  if (m_aDir) {
    vWarn("Dir::~Dir dir not closed: %s\n"_Stz,m_sPath.azChar());

    vClose();
    
    if (m_oSysCall.bHadError())
      vWarn("Dir::~Dir: dir could not be closed: %s"_Stz,m_sPath.azChar());
  }

  
  -- gixDir;
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
bool Dir::bChangeCurrent(Stz* szDirPath)
{
  i8 iResult = chdir(szDirPath->azChar());
  
  if (iResult) {
    vWarn("chdir(%s) error; errno=%d\n"_Stz,szDirPath->azChar(),errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get current working directory                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bGetCurrent(Stz* szDirPath)
{
  char* aCurrPath = getcwd(szDirPath->azChar(),C_MAX_PATH);
  
  if ( ! aCurrPath ) {
    vWarn("Dir::bGet: getcwd() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if directory path exists                                              */
/*----------------------------------------------------------------------------*/
bool Dir::bExists(Stz* szDirPath)
{
  struct stat xStat;
  
  int iResult = stat(szDirPath->azChar(),&xStat);
  
  if (0 == iResult && (xStat.st_mode & S_IFDIR))
    return true;
  
  if (iResult && ENOENT == errno)
    return false;
    
  vWarn("Dir::bExists: stat(%s) error; errno=%d"_Stz,szDirPath->azChar(),errno);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* make specified directory                                                   */
/*----------------------------------------------------------------------------*/
bool Dir::bMake(Stz* szDirPath)
{
  char* aPath = szDirPath->azChar();
  
  if (mkdir(aPath,0777) != 0 && EEXIST != errno) {
    vWarn("Dir::bMake: mkdir() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove specified directory                                                 */
/*----------------------------------------------------------------------------*/
bool Dir::bRemove(Stz* szDirPath)
{
  char* aPath = szDirPath->azChar();
  
  int iResult = rmdir(aPath);
  
  if (-1 == iResult) {
    vWarn("Dir::bRemove: rmdir() error; errno=%d"_Stz,errno);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* repair the dir                                                             */
/*----------------------------------------------------------------------------*/
void Dir::vRepair(Object* pObject, eRF eRepair)
{
  _static_

  Dir* pDir = _pDir(pObject);

  // close the file if open
  if (pDir->bIsOpen())
    pDir->vClose();

  // delete the dir object
  delete pDir;
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
  
  if (AM::bAttached()) AM::pResources()->vDecDirOpen();
  
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
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Dir::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DIR,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
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
bool Dir::bOpen(Stz* szDirPath)
{
  _precondition_(_MAGIC_,CN_DIR,_FOOTER_);
  
//  if (! MY::pTask()) {
//    printf("Dir::bOpen %s (with no task)\n",szDirPath->azChar());
//  }

  ASSERT(bIsClosed());
  
  m_sPath = szDirPath;
  
  char* wPath = m_sPath.azChar();
 
  m_aDir = opendir(wPath);
  if (! m_aDir) {
    m_oSysCall.vError(errno);
    vWarn("Dir::bOpen: opendir() error; errno=%d"_Stz,errno);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (AM::bAttached()) AM::pResources()->vIncDirOpen();
  
  ASSERT(bIsOpen());
  return true;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
