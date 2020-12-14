/*============================================================================*/
/* Steps_Dir.cpp - dir step group                                             */
/*============================================================================*/

#include "Steps.h"

#include "Dir.hpp" //?

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
/* dir.changeCurrent -- change working directory                              */
/*----------------------------------------------------------------------------*/
void Step::vDirChangeCurrent(void)
{
  ENTRY();
  
  s256 sDir;
  m_pDataStack->vPopStr(sDir,__func__);
  
  if ( ! Dir::bChangeCurrent(sDir) )
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Unable to change current directory"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::exists -- test if a directory exists                                   */
/*----------------------------------------------------------------------------*/
void Step::vDirExists(void)
{
  ENTRY();
  
  s256 sDirPath;
  m_pDataStack->vPopStr(sDirPath,__func__);
  
  i8 iResult = Dir::bExists(sDirPath);
  
  m_pDataStack->vPushInt(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::getCurrent -- get working directory                                    */
/*----------------------------------------------------------------------------*/
void Step::vDirGetCurrent(void)
{
  ENTRY();
  
  Datum xDirPath;
  
  s256 sDirPath;
  
  if (Dir::bGetCurrent(sDirPath)) {
    xDirPath = sDirPath;
    m_pDataStack->vPushVal(xDirPath);
  }
  else {
    vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
           "Unable to get current directory"_Stz);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::make -- make a directory                                               */
/*----------------------------------------------------------------------------*/
void Step::vDirMake(void)
{
  ENTRY();
  
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);
  
  i8 iResult = Dir::bMake(sPath);
  
  m_pDataStack->vPushInt(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::remove -- remove a directory                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirRemove(void)
{
  ENTRY();
  
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);
  
  i8 iResult = Dir::bRemove(sPath);
  
  m_pDataStack->vPushInt(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.close -- close directory                                               */
/*----------------------------------------------------------------------------*/
void Step::vDirClose(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  pDir->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.delete -- delete instance of dir                                       */
/*----------------------------------------------------------------------------*/
void Step::vDirDelete(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  delete pDir;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryName -- get dir entry name                                        */
/*----------------------------------------------------------------------------*/
void Step::vDirEntryName(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  s256 sEntryNm = pDir->szEntryNm();
  
  m_pDataStack->vPushStr(sEntryNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryType -- get dir entry type                                        */
/*----------------------------------------------------------------------------*/
void Step::vDirEntryType(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  i8 iEntryType;
  
  iEntryType = pDir->iEntryType();
  
  m_pDataStack->vPushInt(iEntryType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isClosed -- is dir closed?                                             */
/*----------------------------------------------------------------------------*/
void Step::vDirIsClosed(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  bool bIsClosed = pDir->bIsClosed();
  
  m_pDataStack->vPushInt(bIsClosed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isOpen -- is dir open?                                                 */
/*----------------------------------------------------------------------------*/
void Step::vDirIsOpen(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  bool bIsOpen = pDir->bIsOpen();
  
  m_pDataStack->vPushInt(bIsOpen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::new -- new instance of dir                                             */
/*----------------------------------------------------------------------------*/
void Step::vDirNew(void)
{
  ENTRY();
  
  Dir* pDir = new Dir(m_pTask);
  
  Object* pPtrDir;
  pPtrDir = (Object*)pDir;
  
  m_pDataStack->vPushPtr(pPtrDir);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.open -- open dir for reading                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirOpen(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);
  
  bool bOpened = pDir->bOpen(sPath);
  
  m_pDataStack->vPushInt(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path@ -- get path                                                      */
/*----------------------------------------------------------------------------*/
void Step::vDirPathGet(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  s256 sPath = pDir->szPath();
  
  m_pDataStack->vPushStr(sPath);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.read -- read next dir entry                                            */
/*----------------------------------------------------------------------------*/
void Step::vDirRead(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  bool bRead  = pDir->bRead();
  
  m_pDataStack->vPushInt(bRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.rewind -- rewind dir pointer                                           */
/*----------------------------------------------------------------------------*/
void Step::vDirRewind(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  pDir->vRewind();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.seek -- seek dir ptr to a given position                               */
/*----------------------------------------------------------------------------*/
void Step::vDirSeek(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  i8 iPosition = m_pDataStack->iPopInt(__func__);
  
  pDir->vSeek(iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.tell -- tel current dir position                                       */
/*----------------------------------------------------------------------------*/
void Step::vDirTell(void)
{
  ENTRY();
  
  Dir* pDir = _pDir(m_pDataStack->pPopPtr(__func__));
  
  i8 iPosition = pDir->iTell();
  
  m_pDataStack->vPushInt(iPosition);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
