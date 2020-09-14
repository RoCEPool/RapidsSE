/*============================================================================*/
/* Steps_File.cpp - file step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_File(void)
{
  vAddStep("file0[]"_Stz,                &Step::vFile0Get);
  vAddStep("file0[].get"_Stz,            &Step::vFile0Get);
  vAddStep("file0[].getEofLocation"_Stz, &Step::vFile0GetEofLocation);
  vAddStep("file0[].isNull"_Stz,         &Step::vFile0IsNull);
  vAddStep("file0[].size"_Stz,           &Step::vFile0Size);
  vAddStep("file0[].set"_Stz,            &Step::vFile0Set);
  
  vAddStep("file::modTime"_Stz,          &Step::vFileModTime);
  vAddStep("file::new"_Stz,              &Step::vFileNew);
  vAddStep("file::print"_Stz,            &Step::vFilePrint);
  vAddStep("file::sizeNm"_Stz,           &Step::vFileSizeNm);

  vAddStep("file.close"_Stz,             &Step::vFileClose);
  vAddStep("file.delete"_Stz,            &Step::vFileDelete);
  vAddStep("file.open"_Stz,              &Step::vFileOpen);
  vAddStep("file.path@"_Stz,             &Step::vFilePathGet);
  vAddStep("file.readChunk"_Stz,         &Step::vFileReadChunk);
  vAddStep("file.readCrwBlock"_Stz,      &Step::vFileReadCrwBlock);
  vAddStep("file.readLine"_Stz,          &Step::vFileReadLine);
  vAddStep("file.readText"_Stz,          &Step::vFileReadText);
  vAddStep("file.seek"_Stz,              &Step::vFileSeek);
  vAddStep("file.seekRndCsv"_Stz,        &Step::vFileSeekRndCsv);
  vAddStep("file.size"_Stz,              &Step::vFileSize);
  vAddStep("file.tell"_Stz,              &Step::vFileTell);
  vAddStep("file.writeLine"_Stz,         &Step::vFileWriteLine);
}

/*----------------------------------------------------------------------------*/
/* file0[].get -- get reg file0                                               */
/*----------------------------------------------------------------------------*/
void Step::vFile0Get(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpFile0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].getEofLocation -- get eof location of file0                        */
/*----------------------------------------------------------------------------*/
void Step::vFile0GetEofLocation(void)
{
  ENTRY();
  
  File* pFile = *m_pLogicUnit->m_xpFile0;
  
  // get the end-of-file location
  pFile->vSeekEnd();
  i8 iEofLoc = pFile->iTell();
  
  m_pDataStack->vPushInt(iEofLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].isNull -- is file null ?                                           */
/*----------------------------------------------------------------------------*/
void Step::vFile0IsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpFile0.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].set -- set reg file0                                               */
/*----------------------------------------------------------------------------*/
void Step::vFile0Set(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  m_pLogicUnit->m_xpFile0 = pFile;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].size -- gett size of file0                                         */
/*----------------------------------------------------------------------------*/
void Step::vFile0Size(void)
{
  ENTRY();
  
  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);
  
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
/* file0[].modTime -- get the modification time of file by name               */
/*----------------------------------------------------------------------------*/
void Step::vFileModTime(void)
{
  ENTRY();
  
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);

  time_t tTime;
  
  bool bResult = File::bModTime(sPath,tTime);
  
  if (bResult)
    m_pDataStack->vPushInt(tTime);
  else
    m_pDataStack->vPushInt(0);

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
/* file.readChunk-- read read csv chunk returning bytes read                  */
/*----------------------------------------------------------------------------*/
void Step::vFileReadChunk(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  pCsvBuffer->vReset();
  
  i8 nFreeBytes = pCsvBuffer->nFreeBytes();
  
  i8 nReadBytes = pFile->nReadBytes(pCsvBuffer->ayDataArea(),nFreeBytes-1);
  
  pCsvBuffer->vTake(nReadBytes);
  
  i8 nTrimmed = pCsvBuffer->nTrim();
  
  if (nTrimmed) {
    pFile->vSeekBackwards(nTrimmed);
    nReadBytes -= nTrimmed;
  }
  pCsvBuffer->ayDataArea()[nReadBytes] = 0;
  
  m_pDataStack->vPushInt(nReadBytes);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.readCrwBlock -- read a crw block returning bytes read                 */
/*----------------------------------------------------------------------------*/
void Step::vFileReadCrwBlock(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  byte* aCrwProper = aAdd<byte*>(pCrwBuffer,sizeof(DataBuffer));
  i8 nSizeCrwProper = sizeof(CrwBuffer) - sizeof(DataBuffer);
  
  i8 nReadBytes = pFile->nReadBytes(aCrwProper,nSizeCrwProper);

  RASSERT(CN_CRWBUFFER == pCrwBuffer->eClass());
  
  m_pDataStack->vPushInt(nReadBytes);
  
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
/* file.readText -- read multiple lines from a file                           */
/*----------------------------------------------------------------------------*/
void Step::vFileReadText(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  ASSERT(pFile);
  
  Text* pText = _pText(m_pDataStack->pPopPtr(__func__));
  
  Stz* szDst = pText->szGet();
  
  i8 nReadBytes = pFile->nReadBytes(szDst,C_SIZE_LARGE_TEXT);

  szDst->azChar()[nReadBytes] = '\0';
  
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
/* file.seekRndCsv -- seek to random position in csv file                     */
/*----------------------------------------------------------------------------*/
void Step::vFileSeekRndCsv(void)
{
  ENTRY();
  
  s48K sBuffer;
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  ASSERT(pFile);

  i8 nFileSize = pFile->iSize();
  
  if (nFileSize > C_MAX_RECORD * 2) {
    i8 iRndLoc = RndLib::iInteger(nFileSize - C_MAX_RECORD * 2);
    pFile->vSeek(iRndLoc);
  
    pFile->nReadBytes(sBuffer.ayByte(),C_MAX_RECORD * 2);
    
    char* acStart = (char*)sBuffer.ayByte();
    char* acNewLine = acStart;
    if (StrLib::bFind(&acNewLine,'\n')) {
      i8 iAdjust = nDifference(acNewLine,acStart) + 1;
      pFile->vSeek(iRndLoc + iAdjust);
    }
    else {
      NEVER_REACH;
    }
  }
  else {
    pFile->vSeek(0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].size -- gett size of file0                                         */
/*----------------------------------------------------------------------------*/
void Step::vFileSize(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  ASSERT(pFile);

  i8 nSize = pFile->iSize();
  
  m_pDataStack->vPushInt(nSize);
                         
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file0[].sizeNm -- gett size of file by name                                */
/*----------------------------------------------------------------------------*/
void Step::vFileSizeNm(void)
{
  ENTRY();
  
  s256 sPath;
  m_pDataStack->vPopStr(sPath,__func__);

  i8 nSize = File::nSize(sPath);
  
  m_pDataStack->vPushInt(nSize);
                         
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.tell -- tell file                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFileTell(void)
{
  ENTRY();
  
  File* pFile = _pFile(m_pDataStack->pPopPtr(__func__));
  ASSERT(pFile);

  i8 iOffset = pFile->iTell();
  
  m_pDataStack->vPushInt(iOffset);

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
