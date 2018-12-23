/*============================================================================*/
/* Steps_File.cpp - file step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_File(void)
{
  vAddStep("file::new"_Stz,       &Step::vFileNew);
  
  vAddStep("file.close"_Stz,      &Step::vFileClose);
  vAddStep("file.delete"_Stz,     &Step::vFileDelete);
  vAddStep("file.init"_Stz,       &Step::vFileInit);
  vAddStep("file.open"_Stz,       &Step::vFileOpen);
  vAddStep("file.path@"_Stz,      &Step::vFilePathGet);
  vAddStep("file.path!"_Stz,      &Step::vFilePathSet);
  vAddStep("file.read"_Stz,       &Step::vFileRead);
  vAddStep("file.write"_Stz,      &Step::vFileWrite);
}

/*----------------------------------------------------------------------------*/
/* file.close -- close a file ( file -- )                                     */
/*----------------------------------------------------------------------------*/
void Step::vFileClose(MP_Task mpTask)
{
ENTRY("vFileClose"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.close"_Stz);
  
  File* pFile = _pFile(pPtrFile);
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file ( file -- )                                   */
/*----------------------------------------------------------------------------*/
void Step::vFileDelete(MP_Task mpTask)
{
ENTRY("vFileDelete"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.delete"_Stz);
  
  File* pFile = _pFile(pPtrFile);

  delete pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.init -- initialise a file ( file -- )                                 */
/*----------------------------------------------------------------------------*/
void Step::vFileInit(MP_Task mpTask)
{
ENTRY("vFileInit"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.init"_Stz);
  
  File* pFile = _pFile(pPtrFile);
  
  new (pFile) File;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file::new -- new a file ( -- file )                                         */
/*----------------------------------------------------------------------------*/
void Step::vFileNew(MP_Task mpTask)
{
ENTRY("vFileNew"_Stz);
  
  File* pFile = new File;
  
  Object* pPtrFile;
  pPtrFile = (Object*)pFile;
  
  mpTask->m_pDataStack->vPushObject(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file ( mode file -- bool )                             */
/*----------------------------------------------------------------------------*/
void Step::vFileOpen(MP_Task mpTask)
{
ENTRY("vFileOpen"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.open"_Stz);
  
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  
  if ( ! mpTask->m_pDataStack->bPop(sMode) )
    THROW("file.open"_Stz);
  
  bool bOpened = pFile->bOpen(sMode);
  
  mpTask->m_pDataStack->vPushInt(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path ( file -- "<path>" )                           */
/*----------------------------------------------------------------------------*/
void Step::vFilePathGet(MP_Task mpTask)
{
ENTRY("vFilePathGet"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path@"_Stz);
  
  File* pFile = _pFile(pPtrFile);

  s256 sPath = pFile->sPath();
  
  mpTask->m_pDataStack->vPushS256(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path! -- set file path ( "<path>" file -- )                           */
/*----------------------------------------------------------------------------*/
void Step::vFilePathSet(MP_Task mpTask)
{
ENTRY("vFilePathSet"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.path!"_Stz);
  
  File* pFile = _pFile(pPtrFile);
  
  s256 sPath;
  
  if ( ! mpTask->m_pDataStack->bPop(sPath) )
    THROW_INVALID_DSTACK("file.path!"_Stz);
  
  pFile->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read line from a file ( file -- "<line>" 1 | 0 )              */
/*----------------------------------------------------------------------------*/
void Step::vFileRead(MP_Task mpTask)
{
ENTRY("vFileRead"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.read"_Stz);
  
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
/* file.write -- write to a file ( "<line>" file -- )                         */
/*----------------------------------------------------------------------------*/
void Step::vFileWrite(MP_Task mpTask)
{
ENTRY("vFileWrite"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrFile) )
    THROW_INVALID_DSTACK("file.write"_Stz);
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;
  
  if ( ! mpTask->m_pDataStack->bPop(sLine) )
    THROW_INVALID_DSTACK("file.write"_Stz);
  
  Stz* wStr = sLine;
  
  pFile->vPrint("%s"_Stz,wStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
