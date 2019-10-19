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
void Step::vFileClose(MP_Task mpTask, Forth* pForth)
{
ENTRY("vFileClose"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrFile) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  File* pFile = _pFile(pPtrFile);
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file ( file -- )                                   */
/*----------------------------------------------------------------------------*/
void Step::vFileDelete(MP_Task mpTask, Forth* pForth)
{
ENTRY("vFileDelete"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrFile) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  File* pFile = _pFile(pPtrFile);

  delete pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file::new -- new a file ( -- file )                                         */
/*----------------------------------------------------------------------------*/
void Step::vFileNew(MP_Task mpTask, Forth* pForth)
{
ENTRY("vFileNew"_Stz);
  
  File* pFile = new File;
  
  Object* pPtrFile;
  pPtrFile = (Object*)pFile;
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file ( mode file -- bool )                             */
/*----------------------------------------------------------------------------*/
void Step::vFileOpen(MP_Task mpTask, Forth* pForth)
{
ENTRY("vFileOpen"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrFile) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sMode) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);

  s256 sPath;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sPath) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  bool bOpened = pFile->bOpenMode(sPath,sMode);
  
  mpTask->m_pForth->m_pDataStack->vPushI8(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path ( file -- "<path>" )                           */
/*----------------------------------------------------------------------------*/
void Step::vFilePathGet(MP_Task mpTask, Forth* pForth)
{
ENTRY("vFilePathGet"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrFile) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  File* pFile = _pFile(pPtrFile);

  s256 sPath = pFile->sFilePath();
  
  mpTask->m_pForth->m_pDataStack->vPushS256(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read line from a file ( file -- "<line>" 1 | 0 )              */
/*----------------------------------------------------------------------------*/
void Step::vFileRead(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vFileRead"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrFile) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;
  
  bool bResult = pFile->bReadLine(sLine);
  
  if (bResult) {
    mpTask->m_pForth->m_pDataStack->vPushS48K(sLine);
    mpTask->m_pForth->m_pDataStack->vPushBool(true);
  }
  else {
    mpTask->m_pForth->m_pDataStack->vPushBool(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.write -- write to a file ( "<line>" file -- )                         */
/*----------------------------------------------------------------------------*/
void Step::vFileWrite(MP_Task mpTask, Forth* pForth)
{
ENTRY("vFileWrite"_Stz);
  
  Object* pPtrFile;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrFile) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s48K sLine;
  
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(sLine) )
    vThrow(__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Stz* wStr = sLine;
  
  pFile->vPrint("%s"_Stz,wStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
