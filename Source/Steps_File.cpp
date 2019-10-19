/*============================================================================*/
/* Steps_File.cpp - file step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_File(void)
{
  vAddStep("file::new"_Stz,       &Step::vFileNew);
  
  vAddStep("file.close"_Stz,      &Step::vFileClose);
  vAddStep("file.delete"_Stz,     &Step::vFileDelete);
  vAddStep("file.open"_Stz,       &Step::vFileOpen);
  vAddStep("file.path@"_Stz,      &Step::vFilePathGet);
  vAddStep("file.read"_Stz,       &Step::vFileRead);
  vAddStep("file.write"_Stz,      &Step::vFileWrite);
}

/*----------------------------------------------------------------------------*/
/* file.close -- close a file ( file -- )                                     */
/*----------------------------------------------------------------------------*/
void Step::vFileClose(Forth* pForth)
{
ENTRY("vFileClose"_Stz);
  
  Object* pPtrFile;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrFile) )
    THROW_DSTACK();
  
  File* pFile = _pFile(pPtrFile);
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file ( file -- )                                   */
/*----------------------------------------------------------------------------*/
void Step::vFileDelete(Forth* pForth)
{
ENTRY("vFileDelete"_Stz);
  
  Object* pPtrFile;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrFile) )
    THROW_DSTACK();
  
  File* pFile = _pFile(pPtrFile);

  delete pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file::new -- new a file ( -- file )                                         */
/*----------------------------------------------------------------------------*/
void Step::vFileNew(Forth* pForth)
{
ENTRY("vFileNew"_Stz);
  
  File* pFile = new File;
  
  Object* pPtrFile;
  pPtrFile = (Object*)pFile;
  
  pForth->m_pDataStack->vPushPointer(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file ( mode file -- bool )                             */
/*----------------------------------------------------------------------------*/
void Step::vFileOpen(Forth* pForth)
{
ENTRY("vFileOpen"_Stz);
  
  Object* pPtrFile;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrFile) )
    THROW_DSTACK();
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  if ( ! pForth->m_pDataStack->bPop(sMode) )
    THROW_DSTACK();

  s256 sPath;
  if ( ! pForth->m_pDataStack->bPop(sPath) )
    THROW_DSTACK();
  
  bool bOpened = pFile->bOpenMode(sPath,sMode);
  
  pForth->m_pDataStack->vPushI8(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path ( file -- "<path>" )                           */
/*----------------------------------------------------------------------------*/
void Step::vFilePathGet(Forth* pForth)
{
ENTRY("vFilePathGet"_Stz);
  
  Object* pPtrFile;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrFile) )
    THROW_DSTACK();
  
  File* pFile = _pFile(pPtrFile);

  s256 sPath = pFile->sFilePath();
  
  pForth->m_pDataStack->vPushS256(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read line from a file ( file -- "<line>" 1 | 0 )              */
/*----------------------------------------------------------------------------*/
void Step::vFileRead(Forth* pForth)
{
  ENTRY("vFileRead"_Stz);
  
  Object* pPtrFile;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrFile) )
    THROW_DSTACK();
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;
  
  bool bResult = pFile->bReadLine(sLine);
  
  if (bResult) {
    pForth->m_pDataStack->vPushS48K(sLine);
    pForth->m_pDataStack->vPushBool(true);
  }
  else {
    pForth->m_pDataStack->vPushBool(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.write -- write to a file ( "<line>" file -- )                         */
/*----------------------------------------------------------------------------*/
void Step::vFileWrite(Forth* pForth)
{
ENTRY("vFileWrite"_Stz);
  
  Object* pPtrFile;
  
  if ( ! pForth->m_pDataStack->bPop(pPtrFile) )
    THROW_DSTACK();
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;
  
  if ( ! pForth->m_pDataStack->bPop(sLine) )
    THROW_DSTACK();
  
  Stz* wStr = sLine;
  
  pFile->vPrint("%s"_Stz,wStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
