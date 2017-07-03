/*============================================================================*/
/* Steps_Dir.cpp - dir step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// dir: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Dir(void)
{
  vAddPrimitive("dir.changeCurrent"_Str, &vDirChangeCurrent, false);
  vAddPrimitive("dir.exists"_Str,        &vDirExists,        false);
  vAddPrimitive("dir.getCurrent"_Str,    &vDirGetCurrent,    false);
  vAddPrimitive("dir.make"_Str,          &vDirMake,          false);
  vAddPrimitive("dir.remove"_Str,        &vDirRemove,        false);
  
  vAddPrimitive("dir.close"_Str,       &vDirClose,         false);
  vAddPrimitive("dir.delete"_Str,      &vDirDelete,        false);
  vAddPrimitive("dir.entryName"_Str,   &vDirEntryName,     false);
  vAddPrimitive("dir.entryType"_Str,   &vDirEntryType,     false);
  vAddPrimitive("dir.init"_Str,        &vDirInit,          false);
  vAddPrimitive("dir.isClosed"_Str,    &vDirIsClosed,      false);
  vAddPrimitive("dir.isOpen"_Str,      &vDirIsOpen,        false);
  vAddPrimitive("dir::new"_Str,         &vDirNew,           false);
  vAddPrimitive("dir.open"_Str,        &vDirOpen,          false);
  vAddPrimitive("dir.path@"_Str,       &vDirPathGet,       false);
  vAddPrimitive("dir.path!"_Str,       &vDirPathSet,       false);
  vAddPrimitive("dir.read"_Str,        &vDirRead,          false);
  vAddPrimitive("dir.rewind"_Str,      &vDirRewind,        false);
  vAddPrimitive("dir.seek"_Str,        &vDirSeek,          false);
  vAddPrimitive("dir.tell"_Str,        &vDirTell,          false);
}

/*----------------------------------------------------------------------------*/
/* dir.changetCurrent -- change working directory                             */
/*----------------------------------------------------------------------------*/
void Task::vDirChangeCurrent(MP_Task mpTask)
{
  ENTRY("vDirChangeCurrent"_Str);
  
  s256 sDir;
  if ( ! mpTask->m_pDataStack->bPop(sDir) )
    THROW_INVALID_DSTACK("dir.changeCurrent"_Str);
  
  Dir xDir;
  if ( ! xDir.bChangeCurrent(sDir) )
    THROW("Unable to change current directory"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.exists -- test if a directory exists                                   */
/*----------------------------------------------------------------------------*/
void Task::vDirExists(MP_Task mpTask)
{
ENTRY("vDirExists"_Str);
  
  s256 sPath;
  
  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.exists"_Str);
  
  Dir xDir;
  
  i8 iResult = xDir.bExists(sPath);
  mpTask->m_pDataStack->vPushInt(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.getCurrent -- get working directory                                    */
/*----------------------------------------------------------------------------*/
void Task::vDirGetCurrent(MP_Task mpTask)
{
  ENTRY("vDirGetCurrent"_Str);
  
  ValWrk xStrVal;
  
  Dir xDir;
  s256 xDirPath;
  
  if (xDir.bGetCurrent(xDirPath)) {
    xStrVal = xDirPath;
    mpTask->m_pDataStack->vPushValWrk(xStrVal);
  }
  else {
    THROW("Unable to get current directory"_Str);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.make -- make a directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vDirMake(MP_Task mpTask)
{
ENTRY("vDirMake"_Str);
  
  s256 sPath;
  
  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.make"_Str);
  
  Dir xDir;
  
  i8 iResult = xDir.bMake(sPath);
  mpTask->m_pDataStack->vPushInt(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.remove -- remove a directory                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirRemove(MP_Task mpTask)
{
ENTRY("vDirRemove"_Str);
  
  s256 sPath;
  
  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.remove"_Str);
  
  Dir xDir;
  
  i8 iResult = xDir.bRemove(sPath);
  mpTask->m_pDataStack->vPushInt(iResult);
  
  EXIT();
}



/*----------------------------------------------------------------------------*/
/* dir.close -- close directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vDirClose(MP_Task mpTask)
{
ENTRY("vDirClose"_Str);
  
  Object* pPtrDir;

  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.delete -- delete instance of dir                                       */
/*----------------------------------------------------------------------------*/
void Task::vDirDelete(MP_Task mpTask)
{
ENTRY("vDirDelete"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("file.delete"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  delete pDir;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryName -- get dir entry name                                        */
/*----------------------------------------------------------------------------*/
void Task::vDirEntryName(MP_Task mpTask)
{
ENTRY("vDirEntryName"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sEntryName;
  
  sEntryName = pDir->sEntryName();
  
  mpTask->m_pDataStack->vPushS256(sEntryName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryType -- get dir entry type                                        */
/*----------------------------------------------------------------------------*/
void Task::vDirEntryType(MP_Task mpTask)
{
ENTRY("vDirEntryType"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iEntryType;
  
  iEntryType = pDir->iEntryType();
  
  mpTask->m_pDataStack->vPushInt(iEntryType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.init -- init directory                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDirInit(MP_Task mpTask)
{
ENTRY("vDirInit"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.init"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  new(pDir) Dir;
  
  pDir->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isClosed -- is dir closed?                                             */
/*----------------------------------------------------------------------------*/
void Task::vDirIsClosed(MP_Task mpTask)
{
ENTRY("vDirIsClosed"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsClosed = pDir->bIsClosed();
  
  mpTask->m_pDataStack->vPushInt(bIsClosed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isOpen -- is dir open?                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDirIsOpen(MP_Task mpTask)
{
ENTRY("vDirIsOpen"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsOpen = pDir->bIsOpen();
  
  mpTask->m_pDataStack->vPushInt(bIsOpen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir::new -- new instance of dir                                             */
/*----------------------------------------------------------------------------*/
void Task::vDirNew(MP_Task mpTask)
{
ENTRY("vDirNew"_Str);
  
  Dir* pDir = new Dir;
  
  Object* pPtrDir;
  pPtrDir = (Object*)pDir;
  
  mpTask->m_pDataStack->vPushObject(pPtrDir);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.open -- open dir for reading                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirOpen(MP_Task mpTask)
{
ENTRY("vDirOpen"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.open"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bOpened = pDir->bOpen();
  
  mpTask->m_pDataStack->vPushInt(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path@ -- get path                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDirPathGet(MP_Task mpTask)
{
ENTRY("vDirPathGet"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.path@"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  
  sPath = pDir->sPath();
  
  mpTask->m_pDataStack->vPushS256(sPath);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path! -- set path                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDirPathSet(MP_Task mpTask)
{
ENTRY("vDirPathSet"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.path!"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;

  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.path!"_Str);
  
  pDir->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.read -- read next dir entry                                            */
/*----------------------------------------------------------------------------*/
void Task::vDirRead(MP_Task mpTask)
{
ENTRY("vDirRead"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.read"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bRead  = pDir->bRead();
  
  mpTask->m_pDataStack->vPushInt(bRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.rewind -- rewind dir pointer                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirRewind(MP_Task mpTask)
{
ENTRY("vDirRewind"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.rewind"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vRewind();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.seek -- seek dir ptr to a given position                               */
/*----------------------------------------------------------------------------*/
void Task::vDirSeek(MP_Task mpTask)
{
ENTRY("vDirSeek"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.seek"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition;
  if ( ! mpTask->m_pDataStack->bPop(iPosition) )
    THROW_INVALID_DSTACK("dir.seek"_Str);
  
  pDir->vSeek(iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.tell -- tel current dir position                                       */
/*----------------------------------------------------------------------------*/
void Task::vDirTell(MP_Task mpTask)
{
ENTRY("vDirTell"_Str);
  
  Object* pPtrDir;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.tell"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition = pDir->iTell();
  
  mpTask->m_pDataStack->vPushInt(iPosition);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
