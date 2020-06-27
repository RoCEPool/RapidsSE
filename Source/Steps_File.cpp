/*============================================================================*/
/* Steps_File.cpp - file step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_File(void)
{
  vAddStep("file0[].get"_Stz,            &Step::vFile0Get);
  vAddStep("file0[].getEofLocation"_Stz, &Step::vFile0GetEofLocation);
  vAddStep("file0[].isNull"_Stz,         &Step::vFile0IsNull);
  vAddStep("file0[].size"_Stz,           &Step::vFile0Size);
  vAddStep("file0[].set"_Stz,            &Step::vFile0Set);
  
  vAddStep("file::new"_Stz,              &Step::vFileNew);
  vAddStep("file::print"_Stz,            &Step::vFilePrint);
  vAddStep("file::size"_Stz,             &Step::vFileSize);

  vAddStep("file.close"_Stz,             &Step::vFileClose);
  vAddStep("file.delete"_Stz,            &Step::vFileDelete);
  vAddStep("file.open"_Stz,              &Step::vFileOpen);
  vAddStep("file.path@"_Stz,             &Step::vFilePathGet);
  vAddStep("file.readLine"_Stz,          &Step::vFileReadLine);
  vAddStep("file.readChunk"_Stz,         &Step::vFileReadChunk);
  vAddStep("file.seek"_Stz,              &Step::vFileSeek);
  vAddStep("file.writeLine"_Stz,         &Step::vFileWriteLine);
}

/*----------------------------------------------------------------------------*/
/* file0[].get -- get reg file0                                               */
/*----------------------------------------------------------------------------*/
void Step::vFile0Get(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpFile0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].getEofLocation -- get eof location of file0                        */
/*----------------------------------------------------------------------------*/
void Step::vFile0GetEofLocation(void)
{
  ENTRY();
  
  File* pFile = *m_pRegSet->m_xpFile0;
  
  // get the end-of-file location
  pFile->vSeekEnd();
  i8 iEofLoc = pFile->iTell();
  
  m_pDataStack->vPushInt(iEofLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].isNull -- is file null ?                                            */
/*----------------------------------------------------------------------------*/
void Step::vFile0IsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpFile0.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].set -- set reg file0                                               */
/*----------------------------------------------------------------------------*/
void Step::vFile0Set(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  m_pRegSet->m_xpFile0 = pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].size -- gett size of file0                                         */
/*----------------------------------------------------------------------------*/
void Step::vFile0Size(void)
{
  ENTRY();
  
  File* pFile = _pFile(*m_pRegSet->m_xpFile0);
  
  m_pDataStack->vPushInt(File::nSize(pFile->sFilePath()));
                         
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.close -- close a file ( file -- )                                     */
/*----------------------------------------------------------------------------*/
void Step::vFileClose(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file ( file -- )                                   */
/*----------------------------------------------------------------------------*/
void Step::vFileDelete(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  delete pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file::new -- new a file ( -- file )                                        */
/*----------------------------------------------------------------------------*/
void Step::vFileNew(void)
{
  ENTRY();
  
  File* pFile = new File(m_pTask);
  
  m_pDataStack->vPushPtr(pFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file ( mode file -- bool )                             */
/*----------------------------------------------------------------------------*/
void Step::vFileOpen(void)
{
  ENTRY();
  
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
/* file.path@ -- get file path ( pFile -- sPath )                             */
/*----------------------------------------------------------------------------*/
void Step::vFilePathGet(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  s256 sPath = pFile->sFilePath();
  
  m_pDataStack->vPushStr(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file::print -- print contents of file                                      */
/*----------------------------------------------------------------------------*/
void Step::vFilePrint(void)
{
  ENTRY();
  
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);

  File* pFile = new File(m_pTask);

  bool bOpened = pFile->bOpenMode(sPath,C_OPEN_READ);
  if (! bOpened)
    return;

  s48K sLine;
  
  while (pFile->bReadLine(sLine))
    vTrace("%s\n"_Stz,sLine.azChar());
  
  pFile->vClose();
  
  delete pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.readLine -- read line from a file ( pFile -- sLine 1 | 0 )            */
/*----------------------------------------------------------------------------*/
void Step::vFileReadLine(void)
{
  ENTRY();
  
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
/* file.readLine -- read line from a file ( pFile -- sLine 1 | 0 )                */
/*----------------------------------------------------------------------------*/
void Step::vFileReadChunk(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  pCsvBuffer->vReset();
  
  i8 nFreeBytes = pCsvBuffer->nFreeBytes();
  
  i8 nReadBytes = pFile->nReadBytes(pCsvBuffer->ayDataArea(),nFreeBytes);
  
  pCsvBuffer->vTake(nReadBytes);

  m_pDataStack->vPushInt(nReadBytes);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.seek -- seek file                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFileSeek(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  ASSERT(pFile);

  i8 iOffset = m_pDataStack->iPopInt(__func__);
  
  pFile->vSeek(iOffset);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].size -- gett size of file0                                         */
/*----------------------------------------------------------------------------*/
void Step::vFileSize(void)
{
  ENTRY();
  
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);

  i8 nSize = File::nSize(sPath);
  
  m_pDataStack->vPushInt(nSize);
                         
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.writeLine -- write to a file ( sLine pFile -- )                       */
/*----------------------------------------------------------------------------*/
void Step::vFileWriteLine(void)
{
  ENTRY();
  
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
