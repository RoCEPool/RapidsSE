/*============================================================================*/
/* Steps_File.cpp - file step group                                           */
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

// file: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_File(void)
{
  vAddPrimitive("file.close",      &vFileClose,        false);
  vAddPrimitive("file.delete",     &vFileDelete,       false);
  vAddPrimitive("file.init",       &vFileInit,         false);
  vAddPrimitive("file.new",        &vFileNew,          false);
  vAddPrimitive("file.open",       &vFileOpen,         false);
  vAddPrimitive("file.path@",      &vFilePathGet,      false);
  vAddPrimitive("file.path!",      &vFilePathSet,      false);
  vAddPrimitive("file.read",       &vFileRead,         false);
  vAddPrimitive("file.readRecord", &vFileReadRecord,   false);
  vAddPrimitive("file.write",      &vFileWrite,        false);
}

/*----------------------------------------------------------------------------*/
/* file.close -- close a file ( file -- )                                     */
/*----------------------------------------------------------------------------*/
void Task::vFileClose(NNP_Task nnpTask)
{
ENTRY("vFileClose");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.close");
  
  File* pFile = _pFile(pPtrFile);
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file ( file -- )                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileDelete(NNP_Task nnpTask)
{
ENTRY("vFileDelete");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.delete");
  
  File* pFile = _pFile(pPtrFile);

  delete pFile;
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.init -- initialise a file ( file -- )                                 */
/*----------------------------------------------------------------------------*/
void Task::vFileInit(NNP_Task nnpTask)
{
ENTRY("vFileInit");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.init");
  
  File* pFile = _pFile(pPtrFile);
  
  new (pFile) File;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.new -- new a file ( -- file )                                         */
/*----------------------------------------------------------------------------*/
void Task::vFileNew(NNP_Task nnpTask)
{
ENTRY("vFileNew");
  
  File* pFile = new File;
  
  Object* pPtrFile;
  pPtrFile = (Object*)pFile;
  
  nnpTask->m_pDataStack->vPush(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file ( mode file -- bool )                             */
/*----------------------------------------------------------------------------*/
void Task::vFileOpen(NNP_Task nnpTask)
{
ENTRY("vFileOpen");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.open");
  
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  
  if ( ! nnpTask->m_pDataStack->bPop(sMode) )
    THROW("file.open");
  
  bool bOpened = pFile->bOpen(sMode.a());
  
  nnpTask->m_pDataStack->vPush(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path ( file -- "<path>" )                           */
/*----------------------------------------------------------------------------*/
void Task::vFilePathGet(NNP_Task nnpTask)
{
ENTRY("vFilePathGet");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path@");
  
  File* pFile = _pFile(pPtrFile);

  s256 sPath = pFile->sPath();
  
  nnpTask->m_pDataStack->vPush(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path! -- set file path ( "<path>" file -- )                           */
/*----------------------------------------------------------------------------*/
void Task::vFilePathSet(NNP_Task nnpTask)
{
ENTRY("vFilePathSet");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path!");
  
  File* pFile = _pFile(pPtrFile);
  
  s256 sPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("file.path!");
  
  pFile->vPath(sPath.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read line from a file ( file -- "<line>" 1 | 0 )              */
/*----------------------------------------------------------------------------*/
void Task::vFileRead(NNP_Task nnpTask)
{
ENTRY("vFileRead");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.read");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;

  if (pFile->bReadLine(sLine)) {
    nnpTask->m_pDataStack->vPush(sLine);
    nnpTask->m_pDataStack->vPush((i8)1);
  }
  else {
    nnpTask->m_pDataStack->vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.readRecord -- read record from file  ( file -- 1 | 0 )                */
/*----------------------------------------------------------------------------*/
void Task::vFileReadRecord(NNP_Task nnpTask)
{
  ENTRY("vFileRead");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.readRecord");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  if (pFile->bReadRecord()) {
    nnpTask->m_pDataStack->vPush((i8)1);
  }
  else {
    nnpTask->m_pDataStack->vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.write -- write to a file ( "<line>" file -- )                         */
/*----------------------------------------------------------------------------*/
void Task::vFileWrite(NNP_Task nnpTask)
{
ENTRY("vFileWrite");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.write");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;
  
  if ( ! nnpTask->m_pDataStack->bPop(sLine) )
    THROW_INVALID_DSTACK("file.write");
  
  char* wStr = sLine.a();
  
  pFile->vPrint("%s",wStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
