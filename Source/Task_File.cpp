/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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
/* file.close -- close a file                                                 */
/*----------------------------------------------------------------------------*/
void Task::vFileClose(NNP_Task nnpTask)
{
ENTRY("vFileClose");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.close");
  
  File* pFile = _pFile(pPtrFile);
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file                                               */
/*----------------------------------------------------------------------------*/
void Task::vFileDelete(NNP_Task nnpTask)
{
ENTRY("vFileDelete");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.delete");
  
  File* pFile = _pFile(pPtrFile);

  delete pFile;
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.init -- initialise a file                                             */
/*----------------------------------------------------------------------------*/
void Task::vFileInit(NNP_Task nnpTask)
{
ENTRY("vFileInit");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.init");
  
  File* pFile = _pFile(pPtrFile);
  
  new (pFile) File;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.new -- new a file                                                     */
/*----------------------------------------------------------------------------*/
void Task::vFileNew(NNP_Task nnpTask)
{
ENTRY("vFileNew");
  
  File* pFile = new File;
  
  Object* pPtrFile;
  pPtrFile = (Object*)pFile;
  
  nnpTask->m_xDataStack.vPush(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file                                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileOpen(NNP_Task nnpTask)
{
ENTRY("vFileOpen");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.open");
  
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  
  if ( ! nnpTask->m_xDataStack.bPop(sMode) )
    THROW("file.open");
  
  bool bOpened = pFile->bOpen(sMode.a());
  
  nnpTask->m_xDataStack.vPush(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path                                                */
/*----------------------------------------------------------------------------*/
void Task::vFilePathGet(NNP_Task nnpTask)
{
ENTRY("vFilePathGet");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path@");
  
  File* pFile = _pFile(pPtrFile);

  s256 sPath = pFile->sPath();
  
  nnpTask->m_xDataStack.vPush(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path! -- set file path                                                */
/*----------------------------------------------------------------------------*/
void Task::vFilePathSet(NNP_Task nnpTask)
{
ENTRY("vFilePathSet");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path!");
  
  File* pFile = _pFile(pPtrFile);
  
  s256 sPath;
  
  if ( ! nnpTask->m_xDataStack.bPop(sPath) )
    THROW_INVALID_DSTACK("file.path!");
  
  pFile->vPath(sPath.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read from a file                                              */
/*----------------------------------------------------------------------------*/
void Task::vFileRead(NNP_Task nnpTask)
{
ENTRY("vFileRead");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.read");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s8K sLine;

  if (pFile->bReadLine(sLine)) {
    nnpTask->m_xDataStack.vPush(sLine);
    nnpTask->m_xDataStack.vPush((i8)1);
  }
  else {
    nnpTask->m_xDataStack.vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.readRecord -- read record from file                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileReadRecord(NNP_Task nnpTask)
{
  ENTRY("vFileRead");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.readRecord");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  if (pFile->bReadRecord()) {
    nnpTask->m_xDataStack.vPush((i8)1);
  }
  else {
    nnpTask->m_xDataStack.vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.write -- write to a file                                              */
/*----------------------------------------------------------------------------*/
void Task::vFileWrite(NNP_Task nnpTask)
{
ENTRY("vFileWrite");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.write");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  ValWsp xValue;
  
  if ( ! nnpTask->m_xDataStack.bPop(xValue) )
    THROW_INVALID_DSTACK("file.write");
  
  char* wStr = xValue.pAsStrVal()->sStr();
  
  pFile->vPrint("%s",wStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
