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
extern StepBoard*      gpStepBoard;
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
  vAddPrimitive("dir.new"_Str,         &vDirNew,           false);
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
void Task::vDirChangeCurrent(NNP_Task nnpTask)
{
  ENTRY("vDirChangeCurrent"_Str);
  
  s256 sDir;
  if ( ! nnpTask->m_pDataStack->bPop(sDir) )
    THROW_INVALID_DSTACK("dir.changeCurrent"_Str);
  
  Dir xDir;
  if ( ! xDir.bChangeCurrent(sDir) )
    THROW("Unable to change current directory"_Str);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.exists -- test if a directory exists                                   */
/*----------------------------------------------------------------------------*/
void Task::vDirExists(NNP_Task nnpTask)
{
ENTRY("vDirExists"_Str);
  
  s256 sPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.exists"_Str);
  
  Dir xDir;
  
  i8 iResult = xDir.bExists(sPath);
  nnpTask->m_pDataStack->vPush(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.getCurrent -- get working directory                                    */
/*----------------------------------------------------------------------------*/
void Task::vDirGetCurrent(NNP_Task nnpTask)
{
  ENTRY("vDirGetCurrent"_Str);
  
  ValWsp xStrVal;
  
  Dir xDir;
  s256 xDirPath;
  
  if (xDir.bGetCurrent(xDirPath)) {
    xStrVal = xDirPath;
    nnpTask->m_pDataStack->vPush(&xStrVal);
  }
  else {
    THROW("Unable to get current directory"_Str);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.make -- make a directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vDirMake(NNP_Task nnpTask)
{
ENTRY("vDirMake"_Str);
  
  s256 sPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.make"_Str);
  
  Dir xDir;
  
  i8 iResult = xDir.bMake(sPath);
  nnpTask->m_pDataStack->vPush(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.remove -- remove a directory                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirRemove(NNP_Task nnpTask)
{
ENTRY("vDirRemove"_Str);
  
  s256 sPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.remove"_Str);
  
  Dir xDir;
  
  i8 iResult = xDir.bRemove(sPath);
  nnpTask->m_pDataStack->vPush(iResult);
  
  EXIT();
}



/*----------------------------------------------------------------------------*/
/* dir.close -- close directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vDirClose(NNP_Task nnpTask)
{
ENTRY("vDirClose"_Str);
  
  Object* pPtrDir;

  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.delete -- delete instance of dir                                       */
/*----------------------------------------------------------------------------*/
void Task::vDirDelete(NNP_Task nnpTask)
{
ENTRY("vDirDelete"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("file.delete"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  delete pDir;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryName -- get dir entry name                                        */
/*----------------------------------------------------------------------------*/
void Task::vDirEntryName(NNP_Task nnpTask)
{
ENTRY("vDirEntryName"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sEntryName;
  
  sEntryName = pDir->sEntryName();
  
  nnpTask->m_pDataStack->vPush(sEntryName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryType -- get dir entry type                                        */
/*----------------------------------------------------------------------------*/
void Task::vDirEntryType(NNP_Task nnpTask)
{
ENTRY("vDirEntryType"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iEntryType;
  
  iEntryType = pDir->iEntryType();
  
  nnpTask->m_pDataStack->vPush(iEntryType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.init -- init directory                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDirInit(NNP_Task nnpTask)
{
ENTRY("vDirInit"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.init"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  new(pDir) Dir;
  
  pDir->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isClosed -- is dir closed?                                             */
/*----------------------------------------------------------------------------*/
void Task::vDirIsClosed(NNP_Task nnpTask)
{
ENTRY("vDirIsClosed"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsClosed = pDir->bIsClosed();
  
  nnpTask->m_pDataStack->vPush(bIsClosed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isOpen -- is dir open?                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDirIsOpen(NNP_Task nnpTask)
{
ENTRY("vDirIsOpen"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsOpen = pDir->bIsOpen();
  
  nnpTask->m_pDataStack->vPush(bIsOpen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.new -- new instance of dir                                             */
/*----------------------------------------------------------------------------*/
void Task::vDirNew(NNP_Task nnpTask)
{
ENTRY("vDirNew"_Str);
  
  Dir* pDir = new Dir;
  
  Object* pPtrDir;
  pPtrDir = (Object*)pDir;
  
  nnpTask->m_pDataStack->vPush(pPtrDir);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.open -- open dir for reading                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirOpen(NNP_Task nnpTask)
{
ENTRY("vDirOpen"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.open"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bOpened = pDir->bOpen();
  
  nnpTask->m_pDataStack->vPush(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path@ -- get path                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDirPathGet(NNP_Task nnpTask)
{
ENTRY("vDirPathGet"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.path@"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  
  sPath = pDir->sPath();
  
  nnpTask->m_pDataStack->vPush(sPath);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path! -- set path                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDirPathSet(NNP_Task nnpTask)
{
ENTRY("vDirPathSet"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.path!"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  ValWsp xWspPath;

  if ( ! nnpTask->m_pDataStack->bPop(xWspPath) )
    THROW_INVALID_DSTACK("dir.path!"_Str);
  
  s256 sPath;
  sPath = _pStrVal(&xWspPath);
  
  pDir->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.read -- read next dir entry                                            */
/*----------------------------------------------------------------------------*/
void Task::vDirRead(NNP_Task nnpTask)
{
ENTRY("vDirRead"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.read"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bRead  = pDir->bRead();
  
  nnpTask->m_pDataStack->vPush(bRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.rewind -- rewind dir pointer                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirRewind(NNP_Task nnpTask)
{
ENTRY("vDirRewind"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.rewind"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vRewind();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.seek -- seek dir ptr to a given position                               */
/*----------------------------------------------------------------------------*/
void Task::vDirSeek(NNP_Task nnpTask)
{
ENTRY("vDirSeek"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.seek"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition;
  if ( ! nnpTask->m_pDataStack->bPop(iPosition) )
    THROW_INVALID_DSTACK("dir.seek"_Str);
  
  pDir->vSeek(iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.tell -- tel current dir position                                       */
/*----------------------------------------------------------------------------*/
void Task::vDirTell(NNP_Task nnpTask)
{
ENTRY("vDirTell"_Str);
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.tell"_Str);
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition = pDir->iTell();
  
  nnpTask->m_pDataStack->vPush(iPosition);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
