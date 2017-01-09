/*============================================================================*/
/* Task_Dir.cpp - task dir group                                              */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  vAddPrimitive("dir.changeCurrent", &vDirChangeCurrent, false);
  vAddPrimitive("dir.exists",        &vDirExists,        false);
  vAddPrimitive("dir.getCurrent",    &vDirGetCurrent,    false);
  vAddPrimitive("dir.make",          &vDirMake,          false);
  vAddPrimitive("dir.remove",        &vDirRemove,        false);
  
  vAddPrimitive("dir.close",       &vDirClose,         false);
  vAddPrimitive("dir.delete",      &vDirDelete,        false);
  vAddPrimitive("dir.entryName",   &vDirEntryName,     false);
  vAddPrimitive("dir.entryType",   &vDirEntryType,     false);
  vAddPrimitive("dir.init",        &vDirInit,          false);
  vAddPrimitive("dir.isClosed",    &vDirIsClosed,      false);
  vAddPrimitive("dir.isOpen",      &vDirIsOpen,        false);
  vAddPrimitive("dir.new",         &vDirNew,           false);
  vAddPrimitive("dir.open",        &vDirOpen,          false);
  vAddPrimitive("dir.path@",       &vDirPathGet,       false);
  vAddPrimitive("dir.path!",       &vDirPathSet,       false);
  vAddPrimitive("dir.read",        &vDirRead,          false);
  vAddPrimitive("dir.rewind",      &vDirRewind,        false);
  vAddPrimitive("dir.seek",        &vDirSeek,          false);
  vAddPrimitive("dir.tell",        &vDirTell,          false);
}

/*----------------------------------------------------------------------------*/
/* dir.changetCurrent -- change working directory                             */
/*----------------------------------------------------------------------------*/
void Task::vDirChangeCurrent(NNP_Task nnpTask)
{
  ENTRY("vDirChangeCurrent");
  
  s256 sDir;
  if ( ! nnpTask->m_pDataStack->bPop(sDir) )
    THROW_INVALID_DSTACK("dir.changeCurrent");
  
  Dir xDir;
  if ( ! xDir.bChangeCurrent(sDir) )
    THROW("Unable to change current directory");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.exists -- test if a directory exists                                   */
/*----------------------------------------------------------------------------*/
void Task::vDirExists(NNP_Task nnpTask)
{
ENTRY("vDirExists");
  
  s256 sPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.exists");
  
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
  ENTRY("vDirGetCurrent");
  
  ValWsp xStrVal;
  
  Dir xDir;
  s256 sDir;
  
  if (xDir.bGetCurrent(sDir)) {
    xStrVal = sDir.a();
    nnpTask->m_pDataStack->vPush(&xStrVal);
  }
  else {
    THROW("Unable to get current directory");
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.make -- make a directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vDirMake(NNP_Task nnpTask)
{
ENTRY("vDirMake");
  
  s256 sPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.make");
  
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
ENTRY("vDirRemove");
  
  s256 sPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("dir.remove");
  
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
ENTRY("vDirClose");
  
  Object* pPtrDir;

  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.delete -- delete instance of dir                                       */
/*----------------------------------------------------------------------------*/
void Task::vDirDelete(NNP_Task nnpTask)
{
ENTRY("vDirDelete");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("file.delete");
  
  Dir* pDir = _pDir(pPtrDir);
  
  delete pDir;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryName -- get dir entry name                                        */
/*----------------------------------------------------------------------------*/
void Task::vDirEntryName(NNP_Task nnpTask)
{
ENTRY("vDirEntryName");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close");
  
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
ENTRY("vDirEntryType");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close");
  
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
ENTRY("vDirInit");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.init");
  
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
ENTRY("vDirIsClosed");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close");
  
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
ENTRY("vDirIsOpen");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.close");
  
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
ENTRY("vDirNew");
  
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
ENTRY("vDirOpen");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.open");
  
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
ENTRY("vDirPathGet");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.path@");
  
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
ENTRY("vDirPathSet");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.path!");
  
  Dir* pDir = _pDir(pPtrDir);
  
  ValWsp xWspPath;

  if ( ! nnpTask->m_pDataStack->bPop(xWspPath) )
    THROW_INVALID_DSTACK("dir.path!");
  
  s256 sPath;
  sPath = xWspPath.sStr();
  
  pDir->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.read -- read next dir entry                                            */
/*----------------------------------------------------------------------------*/
void Task::vDirRead(NNP_Task nnpTask)
{
ENTRY("vDirRead");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.read");
  
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
ENTRY("vDirRewind");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.rewind");
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vRewind();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.seek -- seek dir ptr to a given position                               */
/*----------------------------------------------------------------------------*/
void Task::vDirSeek(NNP_Task nnpTask)
{
ENTRY("vDirSeek");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.seek");
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition;
  if ( ! nnpTask->m_pDataStack->bPop(iPosition) )
    THROW_INVALID_DSTACK("dir.seek");
  
  pDir->vSeek(iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.tell -- tel current dir position                                       */
/*----------------------------------------------------------------------------*/
void Task::vDirTell(NNP_Task nnpTask)
{
ENTRY("vDirTell");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrDir) )
    THROW_INVALID_DSTACK("dir.tell");
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition = pDir->iTell();
  
  nnpTask->m_pDataStack->vPush(iPosition);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
