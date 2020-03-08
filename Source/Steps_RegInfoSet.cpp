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
  vAddStep("InfoSet[].newInfoSet"_Stz,     &Step::vRegInfoSetNewInfoSet);
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
      
      if ( pInfoSet->bIncRows(2) ) {
        pInfoSet->vAddValue(i_ChunkLoc);
        pInfoSet->vAddValue(nChunkBytes);
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
    
    if ( pInfoSet->bIncRows(2) ) {
      pInfoSet->vAddValue(i_ChunkLoc);
      pInfoSet->vAddValue(nChunkBytes);
      
      // advance to start of next chunk
      i_ChunkLoc = i_EndChunkLoc + 1;
    }
    else {
      
      // save the current chunk location
      m_pRegSet->m_iCounter0 = i_ChunkLoc;
      
      // dump
      //pSeekOffsets->vDump();
      
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
/* InfoSet[].newInfoSet -- new an infoset operand                             */
/*----------------------------------------------------------------------------*/
void Step::vRegInfoSetNewInfoSet(void)
{
  ENTRY(); // ENTRY("vRegInfoSetNewInfoSet"_Stz);
  
  ASSERT (m_pRegSet->m_xpInfoSet.bIsNull());
  
  m_pRegSet->m_xpInfoSet = new InfoSet();
  
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
