/*============================================================================*/
/* Steps_Dir.cpp - dir step group                                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Dir(void)
{
  vAddStep("dir::new"_Stz,         &Step::vDirNew);
  
  vAddStep("dir.changeCurrent"_Stz, &Step::vDirChangeCurrent);
  vAddStep("dir.exists"_Stz,        &Step::vDirExists);
  vAddStep("dir.getCurrent"_Stz,    &Step::vDirGetCurrent);
  vAddStep("dir.make"_Stz,          &Step::vDirMake);
  vAddStep("dir.remove"_Stz,        &Step::vDirRemove);
  
  vAddStep("dir.close"_Stz,       &Step::vDirClose);
  vAddStep("dir.delete"_Stz,      &Step::vDirDelete);
  vAddStep("dir.entryName"_Stz,   &Step::vDirEntryName);
  vAddStep("dir.entryType"_Stz,   &Step::vDirEntryType);
  vAddStep("dir.isClosed"_Stz,    &Step::vDirIsClosed);
  vAddStep("dir.isOpen"_Stz,      &Step::vDirIsOpen);
  vAddStep("dir.open"_Stz,        &Step::vDirOpen);
  vAddStep("dir.path@"_Stz,       &Step::vDirPathGet);
  vAddStep("dir.read"_Stz,        &Step::vDirRead);
  vAddStep("dir.rewind"_Stz,      &Step::vDirRewind);
  vAddStep("dir.seek"_Stz,        &Step::vDirSeek);
  vAddStep("dir.tell"_Stz,        &Step::vDirTell);
}

/*----------------------------------------------------------------------------*/
/* dir.changetCurrent -- change working directory                             */
/*----------------------------------------------------------------------------*/
void Step::vDirChangeCurrent(MP_Task mpTask)
{
  ENTRY("vDirChangeCurrent"_Stz);
  
  s256 sDir;
  if ( ! mpTask->m_pDataStack->bPop(sDir) )
    THROW_INVALID_DSTACK("dir.changeCurrent"_Stz);
  
  if ( ! Dir::bChangeCurrent(sDir) )
    THROW("Unable to change current directory"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.exists -- test if a directory exists                                   */
/*----------------------------------------------------------------------------*/
void Step::vDirExists(MP_Task mpTask)
{
ENTRY("vDirExists"_Stz);
  
  s256 sDirPath;
  if ( ! mpTask->m_pDataStack->bPop(sDirPath) )
    THROW_INVALID_DSTACK("dir.exists"_Stz);
  
  i8 iResult = Dir::bExists(sDirPath);
  
  mpTask->m_pDataStack->vPushInt(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.getCurrent -- get working directory                                    */
/*----------------------------------------------------------------------------*/
void Step::vDirGetCurrent(MP_Task mpTask)
{
  ENTRY("vDirGetCurrent"_Stz);
  
  ValWrk xStrVal;
  
  Dir xDir;
  s256 xDirPath;
  
  if (xDir.bGetCurrent(xDirPath)) {
    xStrVal = xDirPath;
    mpTask->m_pDataStack->vPushValWrk(xStrVal);
  }
  else {
    THROW("Unable to get current directory"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.make -- make a directory                                               */
/*----------------------------------------------------------------------------*/
void Step::vDirMake(MP_Task mpTask)
{
ENTRY("vDirMake"_Stz);
  
  s256 sPath;
  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.make"_Stz);
  
  i8 iResult = Dir::bMake(sPath);
  
  mpTask->m_pDataStack->vPushInt(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.remove -- remove a directory                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirRemove(MP_Task mpTask)
{
ENTRY("vDirRemove"_Stz);
  
  s256 sPath;
  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.remove"_Stz);
  
  i8 iResult = Dir::bRemove(sPath);
  
  mpTask->m_pDataStack->vPushInt(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.close -- close directory                                               */
/*----------------------------------------------------------------------------*/
void Step::vDirClose(MP_Task mpTask)
{
ENTRY("vDirClose"_Stz);
  
  Object* pPtrDir;

  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.delete -- delete instance of dir                                       */
/*----------------------------------------------------------------------------*/
void Step::vDirDelete(MP_Task mpTask)
{
ENTRY("vDirDelete"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("file.delete"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  delete pDir;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryName -- get dir entry name                                        */
/*----------------------------------------------------------------------------*/
void Step::vDirEntryName(MP_Task mpTask)
{
ENTRY("vDirEntryName"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sEntryName;
  
  sEntryName = pDir->sEntryName();
  
  mpTask->m_pDataStack->vPushS256(sEntryName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryType -- get dir entry type                                        */
/*----------------------------------------------------------------------------*/
void Step::vDirEntryType(MP_Task mpTask)
{
ENTRY("vDirEntryType"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iEntryType;
  
  iEntryType = pDir->iEntryType();
  
  mpTask->m_pDataStack->vPushInt(iEntryType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isClosed -- is dir closed?                                             */
/*----------------------------------------------------------------------------*/
void Step::vDirIsClosed(MP_Task mpTask)
{
ENTRY("vDirIsClosed"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsClosed = pDir->bIsClosed();
  
  mpTask->m_pDataStack->vPushInt(bIsClosed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isOpen -- is dir open?                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDirIsOpen(MP_Task mpTask)
{
ENTRY("vDirIsOpen"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsOpen = pDir->bIsOpen();
  
  mpTask->m_pDataStack->vPushInt(bIsOpen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::new -- new instance of dir                                             */
/*----------------------------------------------------------------------------*/
void Step::vDirNew(MP_Task mpTask)
{
ENTRY("vDirNew"_Stz);
  
  Dir* pDir = new Dir;
  
  Object* pPtrDir;
  pPtrDir = (Object*)pDir;
  
  mpTask->m_pDataStack->vPushObject(pPtrDir);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.open -- open dir for reading                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirOpen(MP_Task mpTask)
{
ENTRY("vDirOpen"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.open"_Stz);
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.open"_Stz);
  
  bool bOpened = pDir->bOpen(sPath);
  
  mpTask->m_pDataStack->vPushInt(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path@ -- get path                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDirPathGet(MP_Task mpTask)
{
ENTRY("vDirPathGet"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.path@"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  
  sPath = pDir->sPath();
  
  mpTask->m_pDataStack->vPushS256(sPath);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.read -- read next dir entry                                            */
/*----------------------------------------------------------------------------*/
void Step::vDirRead(MP_Task mpTask)
{
ENTRY("vDirRead"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.read"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bRead  = pDir->bRead();
  
  mpTask->m_pDataStack->vPushInt(bRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.rewind -- rewind dir pointer                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirRewind(MP_Task mpTask)
{
ENTRY("vDirRewind"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.rewind"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vRewind();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.seek -- seek dir ptr to a given position                               */
/*----------------------------------------------------------------------------*/
void Step::vDirSeek(MP_Task mpTask)
{
ENTRY("vDirSeek"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.seek"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition;
  if ( ! mpTask->m_pDataStack->bPop(iPosition) )
    THROW_INVALID_DSTACK("dir.seek"_Stz);
  
  pDir->vSeek(iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.tell -- tel current dir position                                       */
/*----------------------------------------------------------------------------*/
void Step::vDirTell(MP_Task mpTask)
{
ENTRY("vDirTell"_Stz);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.tell"_Stz);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition = pDir->iTell();
  
  mpTask->m_pDataStack->vPushInt(iPosition);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
