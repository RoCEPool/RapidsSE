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
  vAddPrimitive("file::new"_Str,        &vFileNew,          false);
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
void Task::vFileClose(MP_Task mpTask)
{
ENTRY("vFileClose"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.close"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file ( file -- )                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileDelete(MP_Task mpTask)
{
ENTRY("vFileDelete"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.delete"_Str);
  
  File* pFile = _pFile(pPtrFile);

  delete pFile;
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.init -- initialise a file ( file -- )                                 */
/*----------------------------------------------------------------------------*/
void Task::vFileInit(MP_Task mpTask)
{
ENTRY("vFileInit"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.init"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  new (pFile) File;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file::new -- new a file ( -- file )                                         */
/*----------------------------------------------------------------------------*/
void Task::vFileNew(MP_Task mpTask)
{
ENTRY("vFileNew"_Str);
  
  File* pFile = new File;
  
  Object* pPtrFile;
  pPtrFile = (Object*)pFile;
  
  mpTask->m_pDataStack->vPushObject(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file ( mode file -- bool )                             */
/*----------------------------------------------------------------------------*/
void Task::vFileOpen(MP_Task mpTask)
{
ENTRY("vFileOpen"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.open"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  
  if ( ! mpTask->m_pDataStack->bPop(sMode) )
    THROW("file.open"_Str);
  
  bool bOpened = pFile->bOpen(sMode);
  
  mpTask->m_pDataStack->vPushInt(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path ( file -- "<path>" )                           */
/*----------------------------------------------------------------------------*/
void Task::vFilePathGet(MP_Task mpTask)
{
ENTRY("vFilePathGet"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path@"_Str);
  
  File* pFile = _pFile(pPtrFile);

  s256 sPath = pFile->sPath();
  
  mpTask->m_pDataStack->vPushS256(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path! -- set file path ( "<path>" file -- )                           */
/*----------------------------------------------------------------------------*/
void Task::vFilePathSet(MP_Task mpTask)
{
ENTRY("vFilePathSet"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path!"_Str);
  
  File* pFile = _pFile(pPtrFile);
  
  s256 sPath;
  
  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("file.path!"_Str);
  
  pFile->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read line from a file ( file -- "<line>" 1 | 0 )              */
/*----------------------------------------------------------------------------*/
void Task::vFileRead(MP_Task mpTask)
{
ENTRY("vFileRead"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.read"_Str);
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;

  if (pFile->bReadLine(sLine)) {
    mpTask->m_pDataStack->vPushS48K(sLine);
    mpTask->m_pDataStack->vPushInt((i8)1);
  }
  else {
    mpTask->m_pDataStack->vPushInt((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.readRecord -- read record from file  ( file -- 1 | 0 )                */
/*----------------------------------------------------------------------------*/
void Task::vFileReadRecord(MP_Task mpTask)
{
  ENTRY("vFileRead"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.readRecord"_Str);
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  if (pFile->bReadRecord()) {
    mpTask->m_pDataStack->vPushInt((i8)1);
  }
  else {
    mpTask->m_pDataStack->vPushInt((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.write -- write to a file ( "<line>" file -- )                         */
/*----------------------------------------------------------------------------*/
void Task::vFileWrite(MP_Task mpTask)
{
ENTRY("vFileWrite"_Str);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.write"_Str);
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;
  
  if ( ! mpTask->m_pDataStack->bPop(sLine) )
    THROW_INVALID_DSTACK("file.write"_Str);
  
  Str* wStr = sLine;
  
  pFile->vPrint("%s"_Str,wStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
