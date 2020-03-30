/*============================================================================*/
/* Steps_RegInfoSet.cpp -- Register InfoSet step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegInfoSet(void)
{
  vAddStep("InfoSet[]"_Stz,                &Step::vRegInfoSetGet);
  vAddStep("InfoSet[].addColumnDef"_Stz,   &Step::vRegInfoSetAddColumnDef);
  vAddStep("InfoSet[].addGridSanity"_Stz,  &Step::vRegInfoSetAddGridSanity);
  vAddStep("InfoSet[].addNodeInfo"_Stz,    &Step::vRegInfoSetAddNodeInfo);
  vAddStep("InfoSet[].addProgressUnits"_Stz, &Step::vRegInfoSetAddProgressUnits);
  vAddStep("InfoSet[].addSpaceInfo"_Stz,   &Step::vRegInfoSetAddSpaceInfo);
  vAddStep("InfoSet[].addTotalUnits"_Stz,    &Step::vRegInfoSetAddTotalUnits);
  vAddStep("InfoSet[].chunkFile"_Stz,      &Step::vRegInfoSetChunkFile);
  vAddStep("InfoSet[].delete"_Stz,         &Step::vRegInfoSetDelete);
  vAddStep("InfoSet[].firstRow"_Stz,       &Step::vRegInfoSetFirstRow);
  vAddStep("InfoSet[].get"_Stz,            &Step::vRegInfoSetGet);
  vAddStep("InfoSet[].infoRows"_Stz,       &Step::vRegInfoSetInfoRows);
  vAddStep("InfoSet[].name"_Stz,           &Step::vRegInfoSetName);
  vAddStep("InfoSet[].new"_Stz,            &Step::vRegInfoSetNew);
  vAddStep("InfoSet[].nextRow"_Stz,        &Step::vRegInfoSetNextRow);
  vAddStep("InfoSet[].pipeOut"_Stz,        &Step::vRegInfoSetPipeOut);
  vAddStep("InfoSet[].readChunk"_Stz,      &Step::vRegInfoSetReadChunk);
  vAddStep("InfoSet[].set"_Stz,            &Step::vRegInfoSetSet);
  vAddStep("InfoSet[].writeCsv"_Stz,       &Step::vRegInfoSetWriteCsv);
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addColumnDef -- add a row to the infoset with the column def     */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetAddColumnDef(void)
{
  ENTRY(); // ENTRY("vRegInfoSetAddColumnDef"_Stz);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  Column* pColumn = pTable->pStreamDef()->pGetColumn(iColumnNo);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pColumn->iColumnNo(),
                                  pColumn->szColNm(),
                                  aCT[pColumn->eColumnType()],
                                  pColumn->iColumnLength(),
                                  pColumn->iColumnScale(),
                                  pColumn->bIsNullable(),
                                  pColumn->bIsPtnKey(),
                                  C_VARGS_END);
  ASSERT(pRow);
  pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addGridSanity -- add a row to the infoset with the gridSanity    */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetAddGridSanity(void)
{
  ENTRY(); // ENTRY("vRegInfoSetDelete"_Stz);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  Row* pNodeInfo = _pRow(m_pDataStack->pPopPtr(__func__));
  //StreamDef* pStreamDefNI = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  //pNodeInfo->vDump(pStreamDefNI);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  Grid* pGrid = AM::pGrid();
  
  // get regionSize on this node
  bool bRegionSize = false;
  i8 iRegionSize = AM::iRegionSize();
  if (pNodeInfo->m_waColVal[1] == iRegionSize)
    bRegionSize = true;
  
  // get slots per node for this node
  bool bPartitions = false;
  i8 nSlotsPerNode = pGrid->nSlotsPerNode();
  if (pNodeInfo->m_waColVal[2] == nSlotsPerNode)
    bPartitions = true;
  
  // check if the regionSize and partitions match
  bool bHomogenous = false;
  if ( bRegionSize && bPartitions )
    bHomogenous = true;
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pNodeInfo->szStzGet(0),
                                  bHomogenous,
                                  C_VARGS_END);
  ASSERT(pRow);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addNodeInfo -- add a row to the infoset with the nodeInfo        */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetAddNodeInfo(void)
{
  ENTRY(); // ENTRY("vRegInfoSetDelete"_Stz);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);

  Grid* pGrid = AM::pGrid();
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pGrid->pThisNode()->szNodeNm(),
                                  AM::iRegionSize(),
                                  pGrid->nSlotsPerNode(),
                                  C_VARGS_END);
  ASSERT(pRow);
  pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addProgressUnits -- add a row to infoset with the progressUnits  */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetAddProgressUnits(void)
{
  ENTRY(); // ENTRY("vRegInfoSetAddProgressUnits"_Stz);
  
  i8 iProgressUnits = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  iProgressUnits,
                                  C_VARGS_END);
  ASSERT(pRow);
  pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addNSpaceInfo -- add a row to the infoset with the spaceInfo     */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetAddSpaceInfo(void)
{
  ENTRY(); // ENTRY("vRegInfoSetAddSpaceInfo"_Stz);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  Grid* pGrid = AM::pGrid();
  
  i8 iTotalSpace = AM::iRegionSize()
  - AM::iJumboSize();
  
  i8 iInUseSpace = AM::iRegionSize()
    - AM::iJumboSize()
    - gpBufferPool->iFreeSpace()
    - PoolHelper::iPoolFree();
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  i8 iFreeSpaceInPool = PoolHelper::iPoolFree();
  i8 iTotalFreeSpace = iFreeSpace + iFreeSpaceInPool;
  
//  s256 s;
//
//  MagLib::vByteMag(iTotalSpace, iScale, s);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pGrid->pThisNode()->szNodeNm(),
                                  iTotalSpace,
                                  iInUseSpace,
                                  iTotalFreeSpace,
                                  C_VARGS_END);
  ASSERT(pRow);
  pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addTotalUnits -- add a row to infoset with the totalUnits        */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetAddTotalUnits(void)
{
  ENTRY(); // ENTRY("vRegInfoSetAddTotalUnits"_Stz);
  
  i8 iTotalUnits = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  iTotalUnits,
                                  C_VARGS_END);
  ASSERT(pRow);
  pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].chunkFile -- info set chunk file                                 */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetChunkFile(void)
{
  ENTRY(); // ENTRY("vRegInfoSetChunkFile"_Stz);
  
  // get the seekOffsets
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  // get the file we are chunking
  File* pFile = _pFile(*m_pRegSet->m_xpFile0);
  
  // get the last chunk location
  i8 i_ChunkLoc = m_pRegSet->m_iCounter0;
  
  // get the Eof location
  i8 i_EofLoc = m_pRegSet->m_iCounter1;
  
  char* pReadBlock = (char*)MemLib::aMallocBlock(C_CHUNKBLOCK);
  
  i8 nChunkBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process chunks till eof or till we get C_SET_SEEKOFFSETS
  
  while (true) {
    
    // compute the location of the final block of the current chunk
    i8 i_FinalBlockLoc = i_ChunkLoc + C_LOAD_READCHUNK - C_CHUNKBLOCK;
    
    // the final block will be past the eof if the chunk is
    // less than maximum chunk size
    if (i_FinalBlockLoc > i_EofLoc) {
      
      // compute the number of bytes which belong to the current chunk
      nChunkBytes = i_EofLoc - i_ChunkLoc;
      Row* pRow = pInfoSet->aAllotRow();
      
      if (pRow) {
        pRow->m_waColVal[0].m_iWrd = i_ChunkLoc;
        pRow->m_waColVal[1].m_iWrd = nChunkBytes;
      }
      
      s32 sFileSize;
      MagLib::vByteMag(i_EofLoc, 3, sFileSize);
      vTrace("Loading %s %s in %lld chunks by %lld nodes %lld partitions\n"_Stz,
             pFile->sFilePath().szStz(),sFileSize.szStz(),
             pInfoSet->nInfoRows(),
             AM::nDefinedNodes(),
             AM::nPartitions());
      
      // set the chunk location
      m_pRegSet->m_iCounter0 = i_ChunkLoc + nChunkBytes;
      
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
    i8 nPartialBytes = Correctness::nSub((void*)aFinalEol,pReadBlock) + 1;
    
    // compute the total chunk bytes
    nChunkBytes = nPartialBytes + C_WSP_DATACHUNK - 2048;
    
    // compute end location of chunk
    i8 i_EndChunkLoc = i_ChunkLoc + nChunkBytes - 1;
    
    Row* pRow = pInfoSet->aAllotRow();
    
    if (pRow) {
      pRow->m_waColVal[0].m_iWrd = i_ChunkLoc;
      pRow->m_waColVal[1].m_iWrd = nChunkBytes;
      
      // advance to start of next chunk
      i_ChunkLoc = i_EndChunkLoc + 1;
    }
    else {
      
      // save the current chunk location
      m_pRegSet->m_iCounter0 = i_ChunkLoc;
      
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
/* InfoSet[].delete -- operand infoset                                        */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetDelete(void)
{
  ENTRY(); // ENTRY("vRegInfoSetDelete"_Stz);
  
  AM::vDeleteOperand(*m_pRegSet->m_xpInfoSet);
  
  m_pRegSet->m_xpInfoSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].firstRow -- get first row from the infoset                       */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetFirstRow(void)
{
  ENTRY(); // ENTRY("vRegInfoSetFirstRow"_Stz);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  m_pDataStack->vPushPtr(pInfoSet->aFirstRow());
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].get -- get infoset                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetGet(void)
{
  ENTRY(); // ENTRY("vRegInfoSetGet"_Stz);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);

  m_pDataStack->vPushPtr(pInfoSet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].infoRows -- get now of info rows in info set                     */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetInfoRows(void)
{
  ENTRY(); // ENTRY("vRegInfoSetInfoRows"_Stz);
  
  ASSERT ( ! m_pRegSet->m_xpInfoSet.bIsNull() );
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  m_pDataStack->vPushInt(pInfoSet->nInfoRows());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].name -- get infoset name                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetName(void)
{
  ENTRY(); // ENTRY("vRegInfoSetName"_Stz);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);

  Stz* szInfoSetNm = pInfoSet->szInfoSetNm();
  
  m_pDataStack->vPushStz(szInfoSetNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].new -- new an infoset operand                                    */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetNew(void)
{
  ENTRY(); // ENTRY("vRegInfoSetNew"_Stz);
  
  ASSERT(m_pRegSet->m_xpInfoSet.bIsNull());
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  i8 nRowSize = Row::nSizeBytes(pStreamDef);
  
  m_pRegSet->m_xpInfoSet = new InfoSet(nRowSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].nextRow -- get next row from the infoset                         */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetNextRow(void)
{
  ENTRY(); // ENTRY("vRegInfoSetNextRow"_Stz);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  Row* pRow = _pRow(*m_pRegSet->m_xpRow);
  
  ASSERT(pRow);
  
  m_pDataStack->vPushPtr(pInfoSet->aNextRow(pRow));
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].pipeOut -- pipeout infoset                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetPipeOut(void)
{
  ENTRY(); // ENTRY("vRegInfoSetPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpInfoSet);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pRegSet->m_xpInfoSet);
  
  m_pRegSet->m_xpInfoSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].readChunk -- reach a chunk from file given the infoSet           */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetReadChunk(void)
{
  ENTRY(); // ENTRY("vRegInfoSetReadChunk"_Stz);
  
  ASSERT(*m_pRegSet->m_xpInfoSet);
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  File* pFile = _pFile(*m_pRegSet->m_xpFile0);
  
  Row* pRow = _pRow(m_pDataStack->pPopPtr(__func__));
  
  i8 iOffset = pRow->m_waColVal[0];
  i8 nBytes = pRow->m_waColVal[1];
  
  vTrace("reading chunk @%lld of %lld bytes\n"_Stz, iOffset, nBytes);
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
  pFile->vSeek(iOffset);
  pCsvBuffer->vTake(nBytes);
  i8 nBytesRead = pFile->nReadBytes(pCsvBuffer->ayDataArea(),nBytes);
  ASSERT(nBytesRead == nBytes);
  
  // return the CsvBuffer
  m_pDataStack->vPushPtr(pCsvBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].set -- set InfoSet                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetSet(void)
{
  ENTRY(); // ENTRY("vRegInfoSetSet"_Stz);
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pRegSet->m_xpInfoSet.bIsNull());
  m_pRegSet->m_xpInfoSet = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].writeCsv -- write InfoSet to file as csv                         */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetWriteCsv(void)
{
  ENTRY(); // ENTRY("vRegInfoSetWriteCsv"_Stz);
  
  ASSERT(*m_pRegSet->m_xpInfoSet);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpInfoSet);
  
  StreamDef* pStreamDef = *m_pRegSet->m_xpStreamDef;

  File* pFile = _pFile(*m_pRegSet->m_xpFile0);

  MetricsHelper::vWriteCsv(pInfoSet,pStreamDef,pFile);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
