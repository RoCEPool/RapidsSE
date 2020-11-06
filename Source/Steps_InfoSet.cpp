/*============================================================================*/
/* Steps_RegInfoSet.cpp -- Register InfoSet step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_InfoSet(void)
{
  vAddStep("InfoSet[]"_Stz,                  &Step::vInfoSetGet);
  vAddStep("InfoSet[].addLockStats"_Stz,     &Step::vInfoSetAddLockStats);
  vAddStep("InfoSet[].addNodeInfo"_Stz,      &Step::vInfoSetAddNodeInfo);
  vAddStep("InfoSet[].addProgressUnits"_Stz, &Step::vInfoSetAddProgressUnits);
  vAddStep("InfoSet[].addRowCount"_Stz,      &Step::vInfoSetAddRowCount);
  vAddStep("InfoSet[].addRowVargs"_Stz,      &Step::vInfoSetAddRowVargs);
  vAddStep("InfoSet[].addTotalUnits"_Stz,    &Step::vInfoSetAddTotalUnits);
  vAddStep("InfoSet[].chunkFile"_Stz,        &Step::vInfoSetChunkFile);
  vAddStep("InfoSet[].colVal@"_Stz,          &Step::vInfoSetColValFetch);
  vAddStep("InfoSet[].delete"_Stz,           &Step::vInfoSetDelete);
  vAddStep("InfoSet[].firstRow"_Stz,         &Step::vInfoSetFirstRow);
  vAddStep("InfoSet[].get"_Stz,              &Step::vInfoSetGet);
  vAddStep("InfoSet[].infoRows"_Stz,         &Step::vInfoSetInfoRows);
  vAddStep("InfoSet[].isProgress"_Stz,       &Step::vInfoSetIsProgress);
  vAddStep("InfoSet[].isTotalUnits"_Stz,     &Step::vInfoSetIsTotalUnits);
  vAddStep("InfoSet[].name"_Stz,             &Step::vInfoSetName);
  vAddStep("InfoSet[].new"_Stz,              &Step::vInfoSetNew);
  vAddStep("InfoSet[].nextRow"_Stz,          &Step::vInfoSetNextRow);
  vAddStep("InfoSet[].opInfoset@"_Stz,       &Step::vInfoSetOpInfosetFetch);
  vAddStep("InfoSet[].opInfoset!"_Stz,       &Step::vInfoSetOpInfosetStore);
  vAddStep("InfoSet[].pipeOut"_Stz,          &Step::vInfoSetPipeOut);
  vAddStep("InfoSet[].rdCsvChunk"_Stz,       &Step::vInfoSetRdCsvChunk);
  vAddStep("InfoSet[].set"_Stz,              &Step::vInfoSetSet);
  vAddStep("InfoSet[].writeCsv"_Stz,         &Step::vInfoSetWriteCsv);
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addLockStats -- add a row to infoset with the lockStats          */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddLockStats(void)
{
  ENTRY();
  
  // get the table
  Table* pStreamTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pStreamTable->pStreamDef();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_LOCKSTATS);
  
  Row* pRow = LockMetrics::pSample(pStreamDef, pInfoSet);
  
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addNodeInfo -- add a row to the infoset with the nodeInfo        */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddNodeInfo(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_NODEINFO);
  
  Grid* pGrid = AM::pGrid();
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    AM::iRegionSize(),
                                    pGrid->nTablePtns(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addProgressUnits -- add a row to infoset with the progressUnits  */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddProgressUnits(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get no of progress units
  i8 iProgressUnits = m_pDataStack->iPopInt(__func__);
  
  // get the streamDef
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  // get the infoSet
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_PROGRESSUNITS);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  iProgressUnits,
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addRowCount -- add a row to infoset with the row counts for ptn  */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddRowCount(void)
{
  ENTRY();
  
  // get the infoset table
  Table* pInfoSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pInfoSetTable->pStreamDef();
  
  // get the slot
  Job* pJob = m_pTask->m_pJob;
  i8 i_Ptn = pJob->m_pOp->iPartition();
  
  i8 i_TablePtn = TablePtn::iPtnNoToTablePtn(i_Ptn);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_COUNTINFO);
  
  i8 nBuffers = 0;
  i8 nRows = 0;
  
  pTable->vAnalyze(i_TablePtn, nBuffers, nRows);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  nRows,
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addRowVarg -- add a row by variable arguments                    */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddRowVargs(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpInfoSetTable);
  StreamDef* pStreamDef = pTable->pStreamDef();
  i8 nCols = pStreamDef->nColumns(C_ALL);
  i8 nArgs = m_pDataStack->iPopInt(__func__);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  
  Row* aRow = pInfoSet->aAllotRow();
  Stz* szTail;
  i8   nSize;

  i8 iInt;
  s256 sStr;
  for (i8 i_Arg = 0; i_Arg < nArgs && i_Arg < nCols; i_Arg++) {
    
    Column* pColumn = pStreamDef->pGetColumn(i_Arg);

    eDM eDatum = pColumn->eDatum();
    
    switch (eDatum) {
        
      case DM_INT:
        iInt = m_pDataStack->iPopInt(__func__);
        aRow->m_waColVal[i_Arg].m_iWrd = iInt;
        break;
        
      case DM_VCH:
        m_pDataStack->vPopStr(sStr,__func__);
        nSize = sStr.nSize();
        szTail = (Stz*)pInfoSet->ayAllotTail(nSize);
        StrLib::vCopy(szTail->azChar(),sStr.azChar());
        aRow->m_waColVal[i_Arg].m_iWrd = pInfoSet->iToOffset(szTail);
        break;
        
      default:
        RASSERT(0);
    }
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addTotalUnits -- add a row to infoset with the totalUnits        */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddTotalUnits(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the total units
  i8 iTotalUnits = m_pDataStack->iPopInt(__func__);
  
  // get the file name
  s256 sFileName;
  m_pDataStack->vPopStr(sFileName, __func__);
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_TOTALUNITS);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  sFileName.azChar(),
                                  iTotalUnits,
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].chunkFile -- info set chunk file                                 */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetChunkFile(void)
{
  ENTRY();

  m_pLogicUnit->m_iCounter2++;

  i8 nRows = 0;
  Row* pRow;
  
  // get the seekOffsets
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_SEEKOFFSETS);
  
  // get the file we are chunking
  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);
  
  // get the last chunk location
  i8 i_ChunkLoc = m_pLogicUnit->m_iCounter0;
  
  // get the Eof location
  i8 i_EofLoc = m_pLogicUnit->m_iCounter1;
  
  char* pReadBlock = (char*)MemLib::aMallocBlock(C_CHUNKBLOCK);
  
  i8 nChunkBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process chunks till eof or till we get C_MAX_SEEKOFFSETS
  
  while (true) {
    
    // compute the location of the final block of the current chunk
    i8 i_FinalBlockLoc = i_ChunkLoc + C_LOAD_READCHUNK - C_CHUNKBLOCK;
    
    // the final block will be past the eof if the chunk is
    // less than maximum chunk size
    if (i_FinalBlockLoc > i_EofLoc) {
      
      // compute the number of bytes which belong to the current chunk
      nChunkBytes = i_EofLoc - i_ChunkLoc;
      pRow = nullptr;
      if (nRows < C_MAX_SEEKOFFSETS)
        pRow = pInfoSet->aAllotRow();
      
      if (pRow) {
        nRows++;
        
        pRow->m_waColVal[0].m_iWrd = i_ChunkLoc;
        pRow->m_waColVal[1].m_iWrd = nChunkBytes;
      }

      s32 sFileSize;
      MagLib::vByteMag(i_EofLoc, 3, sFileSize);
      vTrace("Loading %s %s "
             "in %lld chunks (%lld offsets per chunk) "
             "by %lld nodes %lld partitions\n"_Stz,
             
             pFile->sFilePath().szStz(),
             sFileSize.szStz(),
             
             m_pLogicUnit->m_iCounter2,
             C_MAX_SEEKOFFSETS,
             
             AM::nDefinedNodes(),
             AM::nPartitionsInGrid());
      
      // set the chunk location
      m_pLogicUnit->m_iCounter0 = i_ChunkLoc + nChunkBytes;
      
      //vTrace("*****SeekOfset has %lld rows\n"_Stz, nRows);
      
      break;
    }
    
    // read next block of the chunk
    pFile->vSeek(i_FinalBlockLoc);
    
    i8 nBytesRead = pFile->nReadBytes(pReadBlock,C_CHUNKBLOCK);
    
    // compute last actual byte of final block just read in
    char *aLastByte = &pReadBlock[nBytesRead - 1];
    
    // find the final end of line for the block (and therefore the chunk)
    char *aFinalEol = aLastByte;
    bool bFoundEol = StrLib::bBackward(&aFinalEol,pReadBlock,'\n');
    if (!bFoundEol)
      bFoundEol = StrLib::bBackward(&aFinalEol,pReadBlock,'\r');
    RASSERT(bFoundEol);
    
    // compute the number of bytes which belong to the current chunk
    i8 nPartialBytes = nDifference((void*)aFinalEol,pReadBlock) + 1;
    
    // compute the total chunk bytes
    nChunkBytes = nPartialBytes + C_WSP_DATACHUNK - 2048;
    
    // compute end location of chunk
    i8 i_EndChunkLoc = i_ChunkLoc + nChunkBytes - 1;
    
    pRow = nullptr;
    if (nRows < C_MAX_SEEKOFFSETS)
      pRow = pInfoSet->aAllotRow();
    
    if (pRow) {
      nRows++;
      
      pRow->m_waColVal[0].m_iWrd = i_ChunkLoc;
      pRow->m_waColVal[1].m_iWrd = nChunkBytes;
      
      // advance to start of next chunk
      i_ChunkLoc = i_EndChunkLoc + 1;
    }
    else {
      
      // save the current chunk location
      m_pLogicUnit->m_iCounter0 = i_ChunkLoc;
      
      //vTrace("*****SeekOfset has %lld rows\n"_Stz, nRows);
      // dump
      //pInfoSet->vPrintRows();
      
      break;
    }
  }
  
  // free the read block
  MemLib::vFreeBlock(pReadBlock, C_2K);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].colVal@ -- column value fetch                                    */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetColValFetch(void)
{
  ENTRY();
    
  Table* pTable = _pTable(*m_pLogicUnit->m_xpInfoSetTable);
  StreamDef* pStreamDef = pTable->pStreamDef();
  i8 nCols = pStreamDef->nColumns(C_ALL);

  i8 i_Col = m_pDataStack->iPopInt(__func__);

  Column* pColumn = pStreamDef->pGetColumn(i_Col);
  eDM eDatum = pColumn->eDatum();

  Row* aRow = *m_pLogicUnit->m_xpRow;
  
  i8 iInt;
  s256 sStr;
  Dtm wDtm;
  
  switch (eDatum) {
      
    case DM_INT:
      iInt = aRow->m_waColVal[i_Col].m_iWrd;
      m_pDataStack->vPushInt(iInt);
      break;
      
    case DM_VCH:
      wDtm = aRow->m_waColVal[i_Col];
      sStr = aRow->szStzGet(i_Col);
      m_pDataStack->vPushStz(sStr);
      break;
      
    default:
      RASSERT(0);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].delete -- operand infoset                                        */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpInfoSet);
  
  m_pLogicUnit->m_xpInfoSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].firstRow -- get first row from the infoset                       */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetFirstRow(void)
{
  ENTRY();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  
  m_pDataStack->vPushPtr(pInfoSet->aFirstRow());
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].get -- get infoset                                               */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetGet(void)
{
  ENTRY();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);

  m_pDataStack->vPushPtr(pInfoSet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].infoRows -- get now of info rows in info set                     */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetInfoRows(void)
{
  ENTRY();
  
  ASSERT ( ! m_pLogicUnit->m_xpInfoSet.bIsNull() );
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  
  m_pDataStack->vPushInt(pInfoSet->nInfoRows());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].isProgress --is infor set a progress info set ?                  */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetIsProgress(void)
{
  ENTRY();
  
  ASSERT ( ! m_pLogicUnit->m_xpInfoSet.bIsNull() );
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  
  bool bIsProgress = pInfoSet->eOpInfoSet() == OI_PROGRESSUNITS ||
                     pInfoSet->eOpInfoSet() == OI_TOTALUNITS;
  m_pDataStack->vPushInt(bIsProgress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].isTotalUnits --is infor set a totalUnits info set ?              */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetIsTotalUnits(void)
{
  ENTRY();
  
  ASSERT ( ! m_pLogicUnit->m_xpInfoSet.bIsNull() );
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  
  m_pDataStack->vPushInt(pInfoSet->eOpInfoSet() == OI_TOTALUNITS ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].name -- get infoset name                                         */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetName(void)
{
  ENTRY();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);

  Stz* szInfoSetNm = pInfoSet->szInfoSetNm();
  
  m_pDataStack->vPushStz(szInfoSetNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].new -- new an infoset operand                                    */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetNew(void)
{
  ENTRY();
  
  ASSERT(m_pLogicUnit->m_xpInfoSet.bIsNull());
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pLogicUnit->m_xpInfoSet = new InfoSet(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].nextRow -- get next row from the infoset                         */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetNextRow(void)
{
  ENTRY();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  Row* pRow = _pRow(*m_pLogicUnit->m_xpRow);
  
  ASSERT(pRow);
  
  m_pDataStack->vPushPtr(pInfoSet->aNextRow(pRow));
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].opInfoset@ -- fetch m_eOpInfoset                                 */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetOpInfosetFetch(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpInfoSet);
  
  eOI eOpInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet)->eOpInfoSet();
  
  m_pDataStack->vPushInt(eOpInfoSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].opInfoset! -- store m_eOpInfoset                                 */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetOpInfosetStore(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpInfoSet);
  
  eOI eOpInfoSet = (eOI)m_pDataStack->iPopInt(__func__);
  
  _pInfoSet(*m_pLogicUnit->m_xpInfoSet)->vOpInfoSet(eOpInfoSet);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].pipeOut -- pipeout infoset                                       */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpInfoSet);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpInfoSet,m_pTask);
  
  m_pLogicUnit->m_xpInfoSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].rdCsvChunk -- reach a chunk from file given the infoSet          */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetRdCsvChunk(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpInfoSet);

  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);
  
  Row* pRow = _pRow(m_pDataStack->pPopPtr(__func__));
  
  i8 iOffset = pRow->m_waColVal[0];
  i8 nBytes = pRow->m_waColVal[1];
  
  //vTrace("reading chunk @%lld of %lld bytes\n"_Stz, iOffset, nBytes);
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
  char* azInto = (char*)pCsvBuffer->ayDataArea();

  pFile->vSeek(iOffset);
  pCsvBuffer->vTake(nBytes);
  i8 nBytesRead = pFile->nReadBytes(azInto,nBytes);
  ASSERT(nBytesRead == nBytes);
  azInto[nBytes] = '\0';
  
  //vTrace("trailing: %s\n"_Stz,azInto + nBytes - 64);
  
  // return the CsvBuffer
  m_pDataStack->vPushPtr(pCsvBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].set -- set InfoSet                                               */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pLogicUnit->m_xpInfoSet.bIsNull());
  m_pLogicUnit->m_xpInfoSet = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].writeCsv -- write InfoSet to file as csv                         */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetWriteCsv(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpInfoSet);
  
  Table* pInfoSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  
  StreamDef* pStreamDef = pInfoSetTable->pStreamDef();

  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);

  MetricsHelper::vWriteCsv(pInfoSet,pStreamDef,pFile);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
