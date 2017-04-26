/*============================================================================*/
/* Steps_File.cpp - file step group                                           */
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

// file: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_File(void)
{
  vAddPrimitive("file.close"_Str,      &vFileClose,        false);
  vAddPrimitive("file.delete"_Str,     &vFileDelete,       false);
  vAddPrimitive("file.init"_Str,       &vFileInit,         false);
  vAddPrimitive("file.new"_Str,        &vFileNew,          false);
  vAddPrimitive("file.open"_Str,       &vFileOpen,         false);
  vAddPrimitive("file.path@"_Str,      &vFilePathGet,      false);
  vAddPrimitive("file.path!"_Str,      &vFilePathSet,      false);
  vAddPrimitive("file.read"_Str,       &vFileRead,         false);
  vAddPrimitive("file.readRecord"_Str, &vFileReadRecord,   false);
  vAddPrimitive("file.write"_Str,      &vFileWrite,        false);
}

/*----------------------------------------------------------------------------*/
/* file.close -- close a file ( file -- )                                     */
/*----------------------------------------------------------------------------*/
void Task::vFileClose(NNP_Task nnpTask)
{
ENTRY("vFileClose"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.close"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file ( file -- )                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileDelete(NNP_Task nnpTask)
{
ENTRY("vFileDelete"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.delete"_Str);
  
  File* pFile = _pFile(pPtrFile);

  delete pFile;
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.init -- initialise a file ( file -- )                                 */
/*----------------------------------------------------------------------------*/
void Task::vFileInit(NNP_Task nnpTask)
{
ENTRY("vFileInit"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.init"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  new (pFile) File;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.new -- new a file ( -- file )                                         */
/*----------------------------------------------------------------------------*/
void Task::vFileNew(NNP_Task nnpTask)
{
ENTRY("vFileNew"_Str);
  
  File* pFile = new File;
  
  Object* pPtrFile;
  pPtrFile = (Object*)pFile;
  
  nnpTask->m_pDataStack->vPushObject(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file ( mode file -- bool )                             */
/*----------------------------------------------------------------------------*/
void Task::vFileOpen(NNP_Task nnpTask)
{
ENTRY("vFileOpen"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.open"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  
  if ( ! nnpTask->m_pDataStack->bPop(sMode) )
    THROW("file.open"_Str);
  
  bool bOpened = pFile->bOpen(sMode);
  
  nnpTask->m_pDataStack->vPush(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path ( file -- "<path>" )                           */
/*----------------------------------------------------------------------------*/
void Task::vFilePathGet(NNP_Task nnpTask)
{
ENTRY("vFilePathGet"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path@"_Str);
  
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
ENTRY("vFilePathSet"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path!"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  s256 sPath;
  
  if ( ! nnpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("file.path!"_Str);
  
  pFile->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read line from a file ( file -- "<line>" 1 | 0 )              */
/*----------------------------------------------------------------------------*/
void Task::vFileRead(NNP_Task nnpTask)
{
ENTRY("vFileRead"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.read"_Str);
  
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
  ENTRY("vFileRead"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.readRecord"_Str);
  
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
ENTRY("vFileWrite"_Str);
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.write"_Str);
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;
  
  if ( ! nnpTask->m_pDataStack->bPop(sLine) )
    THROW_INVALID_DSTACK("file.write"_Str);
  
  Str* wStr = sLine;
  
  pFile->vPrint("%s"_Str,wStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
