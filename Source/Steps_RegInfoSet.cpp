/*============================================================================*/
/* Steps_RegInfoSet.cpp -- Register InfoSet step group                        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegInfoSet(void)
{
  vAddStep("InfoSet[].chunkFile"_Stz,      &Step::vRegInfoSetChunkFile);
  vAddStep("InfoSet[].delete"_Stz,         &Step::vRegInfoSetDelete);
  vAddStep("InfoSet[].firstRow"_Stz,       &Step::vRegInfoSetFirstRow);
  vAddStep("InfoSet[].infoRows"_Stz,       &Step::vRegInfoSetInfoRows);
  vAddStep("InfoSet[].newInfoSet"_Stz,     &Step::vRegInfoSetNewInfoSet);
  vAddStep("InfoSet[].nextRow"_Stz,        &Step::vRegInfoSetNextRow);
  vAddStep("InfoSet[].pipeOut"_Stz,        &Step::vRegInfoSetPipeOut);
  vAddStep("InfoSet[].readChunk"_Stz,      &Step::vRegInfoSetReadChunk);
  vAddStep("InfoSet[].set"_Stz,            &Step::vRegInfoSetSet);
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
  
  // dump
  //pSeekOffsets->vDump();
  
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
/* InfoSet[].newInfoSet -- new an infoset operand                             */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetNewInfoSet(void)
{
  ENTRY(); // ENTRY("vRegInfoSetNewInfoSet"_Stz);
  
  i8 nRowSize = m_pDataStack->iPopInt(__func__);
  
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
  Row* pRow = _pRow2(*m_pRegSet->m_xpRow);
  
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
  
  pOp->vPipeOut(*m_pRegSet->m_xpInfoSet);
  
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
  
  Row* pRow = _pRow2(m_pDataStack->pPopPtr(__func__));
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
