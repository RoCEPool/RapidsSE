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
void Step::vDirChangeCurrent(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDirChangeCurrent"_Stz);
  
  s256 sDir;
  if ( ! pForth->m_pDataStack->bPop(sDir) )
    THROW_DSTACK();
  
  if ( ! Dir::bChangeCurrent(sDir) )
    vThrow(FK_FAILED,__func__,__FILE__,__LINE__,
           "Unable to change current directory");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::exists -- test if a directory exists                                   */
/*----------------------------------------------------------------------------*/
void Step::vDirExists(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirExists"_Stz);
  
  s256 sDirPath;
  if ( ! pForth->m_pDataStack->bPop(sDirPath) )
    THROW_DSTACK();
  
  i8 iResult = Dir::bExists(sDirPath);
  
  pForth->m_pDataStack->vPushI8(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::getCurrent -- get working directory                                    */
/*----------------------------------------------------------------------------*/
void Step::vDirGetCurrent(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vDirGetCurrent"_Stz);
  
  GenVar xStrVal;
  
  s256 sDirPath;
  
  if (Dir::bGetCurrent(sDirPath)) {
    xStrVal = sDirPath;
    pForth->m_pDataStack->vPushGenVar(xStrVal);
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
void Step::vDirMake(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirMake"_Stz);
  
  s256 sPath;
  if ( ! pForth->m_pDataStack->bPop(sPath) )
    THROW_DSTACK();
  
  i8 iResult = Dir::bMake(sPath);
  
  pForth->m_pDataStack->vPushI8(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::remove -- remove a directory                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirRemove(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirRemove"_Stz);
  
  s256 sPath;
  if ( ! pForth->m_pDataStack->bPop(sPath) )
    THROW_DSTACK();
  
  i8 iResult = Dir::bRemove(sPath);
  
  pForth->m_pDataStack->vPushI8(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.close -- close directory                                               */
/*----------------------------------------------------------------------------*/
void Step::vDirClose(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirClose"_Stz);
  
  Object* pPtrDir;

  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.delete -- delete instance of dir                                       */
/*----------------------------------------------------------------------------*/
void Step::vDirDelete(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirDelete"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  delete pDir;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryName -- get dir entry name                                        */
/*----------------------------------------------------------------------------*/
void Step::vDirEntryName(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirEntryName"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sEntryName;
  
  sEntryName = pDir->sEntryName();
  
  pForth->m_pDataStack->vPushS256(sEntryName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryType -- get dir entry type                                        */
/*----------------------------------------------------------------------------*/
void Step::vDirEntryType(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirEntryType"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iEntryType;
  
  iEntryType = pDir->iEntryType();
  
  pForth->m_pDataStack->vPushI8(iEntryType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isClosed -- is dir closed?                                             */
/*----------------------------------------------------------------------------*/
void Step::vDirIsClosed(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirIsClosed"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsClosed = pDir->bIsClosed();
  
  pForth->m_pDataStack->vPushI8(bIsClosed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isOpen -- is dir open?                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDirIsOpen(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirIsOpen"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsOpen = pDir->bIsOpen();
  
  pForth->m_pDataStack->vPushI8(bIsOpen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::new -- new instance of dir                                             */
/*----------------------------------------------------------------------------*/
void Step::vDirNew(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirNew"_Stz);
  
  Dir* pDir = new Dir;
  
  Object* pPtrDir;
  pPtrDir = (Object*)pDir;
  
  pForth->m_pDataStack->vPushPointer(pPtrDir);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.open -- open dir for reading                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirOpen(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirOpen"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  if ( ! pForth->m_pDataStack->bPop(sPath) )
    THROW_DSTACK();
  
  bool bOpened = pDir->bOpen(sPath);
  
  pForth->m_pDataStack->vPushI8(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path@ -- get path                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDirPathGet(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirPathGet"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  
  sPath = pDir->sPath();
  
  pForth->m_pDataStack->vPushS256(sPath);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.read -- read next dir entry                                            */
/*----------------------------------------------------------------------------*/
void Step::vDirRead(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirRead"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bRead  = pDir->bRead();
  
  pForth->m_pDataStack->vPushI8(bRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.rewind -- rewind dir pointer                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirRewind(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirRewind"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vRewind();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.seek -- seek dir ptr to a given position                               */
/*----------------------------------------------------------------------------*/
void Step::vDirSeek(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirSeek"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition;
  if ( ! pForth->m_pDataStack->bPop(iPosition) )
    THROW_DSTACK();
  
  pDir->vSeek(iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.tell -- tel current dir position                                       */
/*----------------------------------------------------------------------------*/
void Step::vDirTell(MP_Task mpTask, Forth* pForth)
{
ENTRY("vDirTell"_Stz);
  
  Object* pPtrDir;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrDir) )
    THROW_DSTACK();
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition = pDir->iTell();
  
  pForth->m_pDataStack->vPushI8(iPosition);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
