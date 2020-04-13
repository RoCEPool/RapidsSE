/*============================================================================*/
/* Steps_StreamSet.cpp -- Register StreamSet step group                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamSet(void)
{
  vAddStep("StreamSet[]"_Stz,                &Step::vStreamSetGet);
  vAddStep("StreamSet[].addColumnDef"_Stz,   &Step::vStreamSetAddColumnDef);
  vAddStep("StreamSet[].addGridSanity"_Stz,  &Step::vStreamSetAddGridSanity);
  vAddStep("StreamSet[].addListContents"_Stz,&Step::vStreamSetAddListContents);
  vAddStep("StreamSet[].addListFolder"_Stz,  &Step::vStreamSetAddListFolder);
  vAddStep("StreamSet[].addListIndex"_Stz,   &Step::vStreamSetAddListIndex);
  vAddStep("StreamSet[].addNodeInfo"_Stz,    &Step::vStreamSetAddNodeInfo);
  vAddStep("StreamSet[].addPingInfo"_Stz,    &Step::vStreamSetAddPingInfo);
  vAddStep("StreamSet[].addPwfInfo"_Stz,       &Step::vStreamSetAddPwfInfo);
  vAddStep("StreamSet[].addShowCluster"_Stz,   &Step::vStreamSetAddShowCluster);
  vAddStep("StreamSet[].addSpaceInfo"_Stz,     &Step::vStreamSetAddSpaceInfo);
  vAddStep("StreamSet[].delete"_Stz,         &Step::vStreamSetDelete);
  vAddStep("StreamSet[].firstRow"_Stz,       &Step::vStreamSetFirstRow);
  vAddStep("StreamSet[].get"_Stz,            &Step::vStreamSetGet);
  vAddStep("StreamSet[].infoRows"_Stz,       &Step::vStreamSetInfoRows);
  vAddStep("StreamSet[].name"_Stz,           &Step::vStreamSetName);
  vAddStep("StreamSet[].new"_Stz,            &Step::vStreamSetNew);
  vAddStep("StreamSet[].nextRow"_Stz,        &Step::vStreamSetNextRow);
  vAddStep("StreamSet[].pipeOut"_Stz,        &Step::vStreamSetPipeOut);
  vAddStep("StreamSet[].set"_Stz,            &Step::vStreamSetSet);
  vAddStep("StreamSet[].writeCsv"_Stz,       &Step::vStreamSetWriteCsv);
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addColumnDef -- add a row to the infoset with the column def     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddColumnDef(void)
{
  ENTRY(); // ENTRY("vStreamSetAddColumnDef"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();

  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pColumn->szColNm(),
                                  aCT[pColumn->eColumnType()],
                                  pColumn->iColumnLength(),
                                  pColumn->iColumnScale(),
                                  (i8)pColumn->bIsNullable(),
                                  (i8)pColumn->bIsPtnKey(),
                                  C_VARGS_END);
  ASSERT(pRow);
  pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addGridSanity -- add a row to the infoset with the gridSanity  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddGridSanity(void)
{
  ENTRY(); // ENTRY("vStreamSetDelete"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  Row* pNodeInfo = _pRow(m_pDataStack->pPopPtr(__func__));
  //StreamDef* pStreamDefNI = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  //pNodeInfo->vDump(pStreamDefNI);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pRegSet->m_xpStreamSet);
  
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
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pNodeInfo->szStzGet(0),
                                  bHomogenous,
                                  C_VARGS_END);
  ASSERT(pRow);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addListContents -- add a row to infoset with the list contents */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddListContents(void)
{
  ENTRY(); // ENTRY("vStreamSetAddLstblInfo"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  s256 sContents;
  m_pDataStack->vPopStr(sContents, __func__);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  sContents.azChar(),
                                  C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addListFolder -- add a row to infoset with the list folder     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddListFolder(void)
{
  ENTRY(); // ENTRY("vStreamSetAddListFolder"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  // get the name
  s256 sName;
  m_pDataStack->vPopStr(sName, __func__);
  
  // get the kind
  s256 sKind;
  m_pDataStack->vPopStr(sKind, __func__);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  sName.azChar(),
                                  sKind.azChar(),
                                  C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addListIndex -- add a row to infoset with the list index         */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddListIndex(void)
{
  ENTRY(); // ENTRY("vStreamSetAddListIndex"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  // get the index
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pIndex->szIndexNm(),
                                  (i8)pIndex->bIsUnique(),
                                  (i8)pIndex->bIsPrimaryKey(),
                                  C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addNodeInfo -- add a row to the infoset with the nodeInfo        */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddNodeInfo(void)
{
  ENTRY(); // ENTRY("vStreamSetDelete"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pRegSet->m_xpStreamSet);
  
  Grid* pGrid = AM::pGrid();
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pGrid->pThisNode()->szNodeNm(),
                                  AM::iRegionSize(),
                                  pGrid->nSlotsPerNode(),
                                  C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addPingInfo -- add a row to infoset with the ping response     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddPingInfo(void)
{
  ENTRY(); // ENTRY("vStreamSetAddPingInfo"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  "pong"_Stz,
                                  C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addProgressUnits -- add a row to infoset with the progressUnits  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddProgressUnits(void)
{
  ENTRY(); // ENTRY("vStreamSetAddProgressUnits"_Stz);
  
  i8 iProgressUnits = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  iProgressUnits,
                                  C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addStr -- add a row to infoset with a __pwf response           */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddPwfInfo(void)
{
  ENTRY(); // ENTRY("vStreamSetAddPwfInfo"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  s256 sString;
  m_pDataStack->vPopStr(sString, __func__);
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pRegSet->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    sString.azChar(),
                                    C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addShowCluster -- add a row to infoset with the cluster info     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddShowCluster(void)
{
  ENTRY(); // ENTRY("vStreamSetAddShowCluster"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  i8 i_Node = m_pDataStack->iPopInt(__func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pRegSet->m_xpStreamSet);
  
  Node* pNode = AM::pGrid()->m_pNodes[i_Node];
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pNode->iNodeNo(),
                                  pNode->szNodeNm(),
                                  pNode->szIPAddr(),
                                  pNode->iPortNo(),
                                  (i8)(pNode->pSendSocket() ? true : false),
                                  C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addSpaceInfo -- add a row to the infoset with the spaceInfo    */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddSpaceInfo(void)
{
  ENTRY(); // ENTRY("vStreamSetAddSpaceInfo"_Stz);
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
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
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addTotalUnits -- add a row to infoset with the totalUnits        */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddTotalUnits(void)
{
  ENTRY(); // ENTRY("vStreamSetAddTotalUnits"_Stz);
  
  i8 iTotalUnits = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  Row* pRow = pInfoSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  iTotalUnits,
                                  C_VARGS_END);
  ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushPtr(pRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].chunkFile -- info set chunk file                                 */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetChunkFile(void)
{
  ENTRY(); // ENTRY("vStreamSetChunkFile"_Stz);
  
  // get the seekOffsets
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
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
/* StreamSet[].delete -- operand infoset                                        */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetDelete(void)
{
  ENTRY(); // ENTRY("vStreamSetDelete"_Stz);
  
  AM::vDeleteOperand(*m_pRegSet->m_xpStreamSet);
  
  m_pRegSet->m_xpStreamSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].firstRow -- get first row from the infoset                       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetFirstRow(void)
{
  ENTRY(); // ENTRY("vStreamSetFirstRow"_Stz);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  m_pDataStack->vPushPtr(pInfoSet->aFirstRow());
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].get -- get infoset                                               */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetGet(void)
{
  ENTRY(); // ENTRY("vStreamSetGet"_Stz);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);

  m_pDataStack->vPushPtr(pInfoSet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].infoRows -- get now of info rows in info set                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetInfoRows(void)
{
  ENTRY(); // ENTRY("vStreamSetInfoRows"_Stz);
  
  ASSERT ( ! m_pRegSet->m_xpStreamSet.bIsNull() );
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  m_pDataStack->vPushInt(pInfoSet->nInfoRows());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].isProgress --is infor set a progress info set ?                  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetIsProgress(void)
{
  ENTRY(); // ENTRY("vStreamSetIsProgress"_Stz);
  
  ASSERT ( ! m_pRegSet->m_xpStreamSet.bIsNull() );
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  bool bIsProgress = pInfoSet->szInfoSetNm()->bSame(C_TDEF_PROGRESSUNITS) ||
                       pInfoSet->szInfoSetNm()->bSame(C_TDEF_TOTALUNITS);
  m_pDataStack->vPushInt(bIsProgress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].isTotalUnits --is infor set a totalUnits info set ?              */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetIsTotalUnits(void)
{
  ENTRY(); // ENTRY("vStreamSetIsTotalUnits"_Stz);
  
  ASSERT ( ! m_pRegSet->m_xpStreamSet.bIsNull() );
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  bool bIsTotalUnits = pInfoSet->szInfoSetNm()->bSame(C_TDEF_TOTALUNITS);
  m_pDataStack->vPushInt(bIsTotalUnits);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].name -- get infoset name                                         */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetName(void)
{
  ENTRY(); // ENTRY("vStreamSetName"_Stz);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);

  Stz* szInfoSetNm = pInfoSet->szInfoSetNm();
  
  m_pDataStack->vPushStz(szInfoSetNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].new -- new an infoset operand                                  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetNew(void)
{
  ENTRY(); // ENTRY("vStreamSetNew"_Stz);
  
  ASSERT(m_pRegSet->m_xpStreamSet.bIsNull());
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pRegSet->m_xpStreamSet = new InfoSet(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].nextRow -- get next row from the infoset                       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetNextRow(void)
{
  ENTRY(); // ENTRY("vStreamSetNextRow"_Stz);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  Row* pRow = _pRow(*m_pRegSet->m_xpRow);
  
  ASSERT(pRow);
  
  m_pDataStack->vPushPtr(pInfoSet->aNextRow(pRow));
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].pipeOut -- pipeout infoset                                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetPipeOut(void)
{
  ENTRY(); // ENTRY("vStreamSetPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpStreamSet);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pRegSet->m_xpStreamSet);
  
  m_pRegSet->m_xpStreamSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].readChunk -- reach a chunk from file given the infoSet           */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetReadChunk(void)
{
  ENTRY(); // ENTRY("vStreamSetReadChunk"_Stz);
  
  ASSERT(*m_pRegSet->m_xpStreamSet);
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  File* pFile = _pFile(*m_pRegSet->m_xpFile0);
  
  Row* pRow = _pRow(m_pDataStack->pPopPtr(__func__));
  
  i8 iOffset = pRow->m_waColVal[0];
  i8 nBytes = pRow->m_waColVal[1];
  
  //vTrace("reading chunk @%lld of %lld bytes\n"_Stz, iOffset, nBytes);
  
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
/* StreamSet[].set -- set InfoSet                                             */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetSet(void)
{
  ENTRY(); // ENTRY("vStreamSetSet"_Stz);
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pRegSet->m_xpStreamSet.bIsNull());
  m_pRegSet->m_xpStreamSet = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].writeCsv -- write InfoSet to file as csv                       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetWriteCsv(void)
{
  ENTRY(); // ENTRY("vStreamSetWriteCsv"_Stz);
  
  ASSERT(*m_pRegSet->m_xpStreamSet);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pRegSet->m_xpStreamSet);
  
  StreamDef* pStreamDef = *m_pRegSet->m_xpStreamDef;

  File* pFile = _pFile(*m_pRegSet->m_xpFile0);

  MetricsHelper::vWriteCsv(pInfoSet,pStreamDef,pFile);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
