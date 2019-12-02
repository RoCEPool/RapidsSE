/*============================================================================*/
/* Steps_File.cpp - file step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_File(void)
{
  vAddStep("file0[].get"_Stz,     &Step::vFile0Get);
  vAddStep("file0[].set"_Stz,     &Step::vFile0Set);
  
  vAddStep("file::new"_Stz,       &Step::vFileNew);
  vAddStep("file.close"_Stz,      &Step::vFileClose);
  vAddStep("file.delete"_Stz,     &Step::vFileDelete);
  vAddStep("file.open"_Stz,       &Step::vFileOpen);
  vAddStep("file.path@"_Stz,      &Step::vFilePathGet);
  vAddStep("file.read"_Stz,       &Step::vFileRead);
  vAddStep("file.write"_Stz,      &Step::vFileWrite);
}

/*----------------------------------------------------------------------------*/
/* file0[].get -- get reg file0                                               */
/*----------------------------------------------------------------------------*/
void Step::vFile0Get(void)
{
  ENTRY(); // ENTRY("vFile0Get"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpFile0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].set -- set reg file0                                               */
/*----------------------------------------------------------------------------*/
void Step::vFile0Set(void)
{
  ENTRY(); // ENTRY("vFile0Set"_Stz);
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  m_pRegSet->m_xpFile0 = pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.close -- close a file ( file -- )                                     */
/*----------------------------------------------------------------------------*/
void Step::vFileClose(void)
{
  ENTRY(); // ENTRY("vFileClose"_Stz);
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file ( file -- )                                   */
/*----------------------------------------------------------------------------*/
void Step::vFileDelete(void)
{
  ENTRY(); // ENTRY("vFileDelete"_Stz);
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  delete pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file::new -- new a file ( -- file )                                         */
/*----------------------------------------------------------------------------*/
void Step::vFileNew(void)
{
  ENTRY(); // ENTRY("vFileNew"_Stz);
  
  File* pFile = new File(m_pTask);
  
  m_pDataStack->vPushPtr(pFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file ( mode file -- bool )                             */
/*----------------------------------------------------------------------------*/
void Step::vFileOpen(void)
{
  ENTRY(); // ENTRY("vFileOpen"_Stz);
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  
  s32 sMode;
  m_pDataStack->vPopStr(sMode,__func__);

  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);
  
  bool bOpened = pFile->bOpenMode(sPath,sMode);
  
  m_pDataStack->vPushInt(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path ( file -- "<path>" )                           */
/*----------------------------------------------------------------------------*/
void Step::vFilePathGet(void)
{
  ENTRY(); // ENTRY("vFilePathGet"_Stz);
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  s256 sPath = pFile->sFilePath();
  
  m_pDataStack->vPushStr(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read line from a file ( file -- "<line>" 1 | 0 )              */
/*----------------------------------------------------------------------------*/
void Step::vFileRead(void)
{
  ENTRY(); // ENTRY("vFileRead"_Stz);
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  ASSERT(pFile);
  
  s48K sLine;
  
  bool bResult = pFile->bReadLine(sLine);
  
  if (bResult) {
    m_pDataStack->vPushStr(sLine);
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.write -- write to a file ( "<line>" file -- )                         */
/*----------------------------------------------------------------------------*/
void Step::vFileWrite(void)
{
  ENTRY(); // ENTRY("vFileWrite"_Stz);
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  ASSERT(pFile);
  
  s48K sLine;
  
  m_pDataStack->vPopStr(sLine,__func__);
  
  Stz* wStr = sLine;
  
  pFile->vPrint("%s"_Stz,wStr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
