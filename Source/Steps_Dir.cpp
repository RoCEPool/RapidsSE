/*============================================================================*/
/* Steps_Dir.cpp - dir step group                                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dir(void)
{
  vAddStep("dir::new"_Stz,           &Step::vDirNew);
  
  vAddStep("dir::changeCurrent"_Stz, &Step::vDirChangeCurrent);
  vAddStep("dir::exists"_Stz,        &Step::vDirExists);
  vAddStep("dir::getCurrent"_Stz,    &Step::vDirGetCurrent);
  vAddStep("dir::make"_Stz,          &Step::vDirMake);
  vAddStep("dir::remove"_Stz,        &Step::vDirRemove);
  
  vAddStep("dir.close"_Stz,          &Step::vDirClose);
  vAddStep("dir.delete"_Stz,         &Step::vDirDelete);
  vAddStep("dir.entryName"_Stz,      &Step::vDirEntryName);
  vAddStep("dir.entryType"_Stz,      &Step::vDirEntryType);
  vAddStep("dir.isClosed"_Stz,       &Step::vDirIsClosed);
  vAddStep("dir.isOpen"_Stz,         &Step::vDirIsOpen);
  vAddStep("dir.open"_Stz,           &Step::vDirOpen);
  vAddStep("dir.path@"_Stz,          &Step::vDirPathGet);
  vAddStep("dir.read"_Stz,           &Step::vDirRead);
  vAddStep("dir.rewind"_Stz,         &Step::vDirRewind);
  vAddStep("dir.seek"_Stz,           &Step::vDirSeek);
  vAddStep("dir.tell"_Stz,           &Step::vDirTell);
}

/*----------------------------------------------------------------------------*/
/* dir.changetCurrent -- change working directory                             */
/*----------------------------------------------------------------------------*/
void Step::vDirChangeCurrent(void)
{
  ENTRY("vDirChangeCurrent"_Stz);
  
  s256 sDir;
  if ( ! m_pDataStack->bPop(sDir) )
    THROW_DSTACK();
  
  if ( ! Dir::bChangeCurrent(sDir) )
    vThrow(FK_FAILED,__func__,__FILE__,__LINE__,
           "Unable to change current directory");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::exists -- test if a directory exists                                   */
/*----------------------------------------------------------------------------*/
void Step::vDirExists(void)
{
ENTRY("vDirExists"_Stz);
  
  s256 sDirPath;
  if ( ! m_pDataStack->bPop(sDirPath) )
    THROW_DSTACK();
  
  i8 iResult = Dir::bExists(sDirPath);
  
  m_pDataStack->vPushI8(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::getCurrent -- get working directory                                    */
/*----------------------------------------------------------------------------*/
void Step::vDirGetCurrent(void)
{
  ENTRY("vDirGetCurrent"_Stz);
  
  GenVar xStrVal;
  
  s256 sDirPath;
  
  if (Dir::bGetCurrent(sDirPath)) {
    xStrVal = sDirPath;
    m_pDataStack->vPushGenVar(xStrVal);
  }
  else {
    vThrow(FK_FAILED,__func__,__FILE__,__LINE__,
           "Unable to get current directory");
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::make -- make a directory                                               */
/*----------------------------------------------------------------------------*/
void Step::vDirMake(void)
{
ENTRY("vDirMake"_Stz);
  
  s256 sPath;
  if ( ! m_pDataStack->bPop(sPath) )
    THROW_DSTACK();
  
  i8 iResult = Dir::bMake(sPath);
  
  m_pDataStack->vPushI8(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::remove -- remove a directory                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirRemove(void)
{
ENTRY("vDirRemove"_Stz);
  
  s256 sPath;
  if ( ! m_pDataStack->bPop(sPath) )
    THROW_DSTACK();
  
  i8 iResult = Dir::bRemove(sPath);
  
  m_pDataStack->vPushI8(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.close -- close directory                                               */
/*----------------------------------------------------------------------------*/
void Step::vDirClose(void)
{
ENTRY("vDirClose"_Stz);
  
  Object* pPtrDir;

  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.delete -- delete instance of dir                                       */
/*----------------------------------------------------------------------------*/
void Step::vDirDelete(void)
{
ENTRY("vDirDelete"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  delete pDir;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryName -- get dir entry name                                        */
/*----------------------------------------------------------------------------*/
void Step::vDirEntryName(void)
{
ENTRY("vDirEntryName"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sEntryName;
  
  sEntryName = pDir->sEntryName();
  
  m_pDataStack->vPushS256(sEntryName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryType -- get dir entry type                                        */
/*----------------------------------------------------------------------------*/
void Step::vDirEntryType(void)
{
ENTRY("vDirEntryType"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iEntryType;
  
  iEntryType = pDir->iEntryType();
  
  m_pDataStack->vPushI8(iEntryType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isClosed -- is dir closed?                                             */
/*----------------------------------------------------------------------------*/
void Step::vDirIsClosed(void)
{
ENTRY("vDirIsClosed"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsClosed = pDir->bIsClosed();
  
  m_pDataStack->vPushI8(bIsClosed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isOpen -- is dir open?                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDirIsOpen(void)
{
ENTRY("vDirIsOpen"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsOpen = pDir->bIsOpen();
  
  m_pDataStack->vPushI8(bIsOpen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::new -- new instance of dir                                             */
/*----------------------------------------------------------------------------*/
void Step::vDirNew(void)
{
ENTRY("vDirNew"_Stz);
  
  Dir* pDir = new Dir;
  
  Object* pPtrDir;
  pPtrDir = (Object*)pDir;
  
  m_pDataStack->vPushPointer(pPtrDir);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.open -- open dir for reading                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirOpen(void)
{
ENTRY("vDirOpen"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  if ( ! m_pDataStack->bPop(sPath) )
    THROW_DSTACK();
  
  bool bOpened = pDir->bOpen(sPath);
  
  m_pDataStack->vPushI8(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path@ -- get path                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDirPathGet(void)
{
ENTRY("vDirPathGet"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  
  sPath = pDir->sPath();
  
  m_pDataStack->vPushS256(sPath);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.read -- read next dir entry                                            */
/*----------------------------------------------------------------------------*/
void Step::vDirRead(void)
{
ENTRY("vDirRead"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bRead  = pDir->bRead();
  
  m_pDataStack->vPushI8(bRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.rewind -- rewind dir pointer                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirRewind(void)
{
ENTRY("vDirRewind"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vRewind();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.seek -- seek dir ptr to a given position                               */
/*----------------------------------------------------------------------------*/
void Step::vDirSeek(void)
{
ENTRY("vDirSeek"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition;
  if ( ! m_pDataStack->bPop(iPosition) )
    THROW_DSTACK();
  
  pDir->vSeek(iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.tell -- tel current dir position                                       */
/*----------------------------------------------------------------------------*/
void Step::vDirTell(void)
{
ENTRY("vDirTell"_Stz);
  
  Object* pPtrDir;
  
  if ( ! m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition = pDir->iTell();
  
  m_pDataStack->vPushI8(iPosition);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
