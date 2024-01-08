/*============================================================================*/
/* Steps_Pds.cpp -- Pds                                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "LogicUnit.hpp"

#include "LglPtnMap.hpp"
#include "Table.hpp"
#include "Op.hpp"
#include "MetricsHelper.hpp"
#include "LockMetrics.hpp"
#include "Grid.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Pds(void)
{
  vAddStep("Pds[]"_Stz,                     &Step::vPdsGet);

  vAddStep("Pds[].addAnalyzeTable"_Stz, &Step::vPdsAddAnalyzeTable);
  vAddStep("Pds[].addBpaObjects"_Stz, &Step::vPdsAddBpaObjects);
  vAddStep("Pds[].addBpaRegions"_Stz, &Step::vPdsAddBpaRegions);
  vAddStep("Pds[].addColumnDef"_Stz, &Step::vPdsAddColumnDef);
  vAddStep("Pds[].addGridSanity"_Stz,      &Step::vPdsAddGridSanity);
  vAddStep("Pds[].addNodeInfo"_Stz, &Step::vPdsAddNodeInfo);
  vAddStep("Pds[].addListFolder"_Stz,    &Step::vPdsAddListFolder);
  vAddStep("Pds[].addListIndex"_Stz,     &Step::vPdsAddListIndex);
  vAddStep("Pds[].addListPartitions"_Stz,&Step::vPdsAddListPartitions);
  vAddStep("Pds[].addLockStats"_Stz,      &Step::vPdsAddLockStats);
  vAddStep("Pds[].addProgressUnits"_Stz, &Step::vPdsAddProgressUnits);
  vAddStep("Pds[].addRowCount"_Stz,      &Step::vPdsAddRowCount);
  vAddStep("Pds[].addRowVargs"_Stz,      &Step::vPdsAddRowVargs);
  vAddStep("Pds[].bufferId"_Stz  ,          &Step::vPdsGetBufferIdAsAddr);
  vAddStep("Pds[].bufferType@"_Stz,         &Step::vPdsBufferTypeFetch);
  vAddStep("Pds[].bufferType!"_Stz,         &Step::vPdsBufferTypeStore);
  vAddStep("Pds[].chunkFile"_Stz,         &Step::vPdsChunkFile);
  vAddStep("Pds[].delete"_Stz  ,            &Step::vPdsDelete);
  vAddStep("Pds[].firstRow"_Stz,            &Step::vPdsFirstRow);
  vAddStep("Pds[].isNull"_Stz  ,            &Step::vPdsIsNull);
  vAddStep("Pds[].new"_Stz,                 &Step::vPdsNew);
  vAddStep("Pds[].rdCsvChunk"_Stz,                 &Step::vPdsRdCsvChunk);
  vAddStep("Pds[].set"_Stz  ,               &Step::vPdsSet);
  vAddStep("Pds[].setLglPtn"_Stz  ,               &Step::vPdsSetLglPtn);
  vAddStep("Pds[].tableDefName@"_Stz,    &Step::vPdsTableDefNameFetch);
  vAddStep("Pds[].writeCsv"_Stz,    &Step::vPdsWriteCsv);
}

/*----------------------------------------------------------------------------*/
/* Pds[].addAnalyzeTable -- add a row to the Pds table analysis               */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddAnalyzeTable(void)
{
  ENTRY();

  // get the table to be analyzed
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  // get the tablePtn
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);

  // get the stream set
  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  i8 nBuffers = 0;
  i8 nRows = 0;
  i8 njRowSize = 0;

  pTable->vAnalyze(i_TablePtn, nBuffers, nRows, njRowSize);

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    pTable->tTablePtnToLglPtn(i_TablePtn).iAsI8(),
                                    nBuffers,
                                    nRows,
                                    C_VARGS_END);

  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addBpaObjects -- add rows about bpa object analysis                  */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddBpaObjects(void)
{
  ENTRY();

  // Get the warning Pds and definition
  Pds* pPdsWarn = * m_pLogicUnit->m_xpPdsWarn;
  ASSERT(pPdsWarn);
  ASSERT(pPdsWarn->eBufferType() == BT_WARNINGS);

  Pds* pPds = _pPds(*m_pLogicUnit->m_xpPds);

  PoolHelper::vObjectSummary(pPds, pPdsWarn);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addBpaRegions -- add rows about bpa region analysis                  */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddBpaRegions(void)
{
  ENTRY();

  // Get the warning Pds and definition
  Pds* pPdsWarn = * m_pLogicUnit->m_xpPdsWarn;
  ASSERT(pPdsWarn);
  ASSERT(pPdsWarn->eBufferType() == BT_WARNINGS);

  // Get the warning TableDef
  Table* pWarnTable = _pTable(* m_pLogicUnit->m_xpTableDefWarn);
  ASSERT(pWarnTable);
  StreamDef* pWarnDef = pWarnTable->pStreamDef();
  ASSERT(pWarnDef);

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  PoolHelper::vRegionSummary(pPdsRowBuffer, pPdsWarn, pWarnDef);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addColumnDef -- add a row to the Pds with the column def             */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddColumnDef(void)
{
  ENTRY();

  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));

  Pds* pPds = _pPds(*m_pLogicUnit->m_xpPds);

  Row* pRow = pPds->pAllotRow(C_VARGS_BEGIN,
                                    pColumn->szColNm(),
                                    aCT[pColumn->eColumnType()],
                                    pColumn->iColumnLength(),
                                    pColumn->iColumnScale(),
                                    (i8)pColumn->bIsNullable(),
                                    (i8)pColumn->bIsPtnKey(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addGridSanity -- add a row to Pds with the gridSanity                */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddGridSanity(void)
{
  ENTRY();

  Row* pNodeInfo = _pRow(m_pDataStack->pPopPtr(__func__));

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  // get regionSize on this node
  bool bRegionSize = false;
  i8 iRegionSize = AM::iRegionSize();
  if (pNodeInfo->iColVal_int(1) == iRegionSize)
    bRegionSize = true;

  // get partitions per table for this node
  bool bPartitions = false;
  i8 nPtnsPerNode = AM::PTNMAP::nPtnsPerNode();
  if (pNodeInfo->iColVal_int(2) == nPtnsPerNode)
    bPartitions = true;

  // check if the regionSize and partitions match
  bool bHomogenous = false;
  if ( bRegionSize && bPartitions )
    bHomogenous = true;

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    pNodeInfo->szColVal_ofs(0),
                                    bHomogenous,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addNodeInfo -- add a row to the Pds with the nodeInfo                */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddNodeInfo(void)
{
  ENTRY();

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  Grid* pGrid = AM::pGrid();

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    AM::iRegionSize(),
                                    AM::PTNMAP::nPtnsPerNode(),
                                    (i8)AM::SYSTEM::bStarted(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addListFolder -- add a row to Pds with the list folder               */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddListFolder(void)
{
  ENTRY();

  // get the name
  s256 sName;
  m_pDataStack->vPopStr(sName, __func__);

  // get the kind
  s256 sKind;
  m_pDataStack->vPopStr(sKind, __func__);

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    sName.azChar(),
                                    sKind.azChar(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addListIndex -- add a row to Pds with the list index                 */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddListIndex(void)
{
  ENTRY();

  // get the index
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

  Pds* pPds = _pPds(*m_pLogicUnit->m_xpPds);

  Row* pRow = pPds->pAllotRow(C_VARGS_BEGIN,
                                    pIndex->szIndexNm(),
                                    (i8)pIndex->bIsUnique(),
                                    (i8)pIndex->bIsPrimaryKey(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addListPartitions -- add a row to Pds with the partitions*/
/*----------------------------------------------------------------------------*/
void Step::vPdsAddListPartitions(void)
{
  ENTRY();

  LglPtn tLglPtn;
  tLglPtn = m_pDataStack->iPopInt(__func__);

  Pds* pPds = _pPds(*m_pLogicUnit->m_xpPds);

  Grid* pGrid = AM::pGrid();

  i8 iNodeId = AM::pLglPtnMap()->iNodeId(tLglPtn);
  Node* pNode = pGrid->pFindNodeById(iNodeId);

  Row* pRow = pPds->pAllotRow(C_VARGS_BEGIN,
                                    tLglPtn.iAsI8(),
                                    pNode->szNodeNm(),
                                    C_VARGS_END);


  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addLockStats -- add a row to Pds with the lockStats                  */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddLockStats(void)
{
  ENTRY();

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  Row* pRow = LockMetrics::pSamplePds(pPdsRowBuffer);

  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addProgressUnits -- add row to Pds with the progressUnits            */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddProgressUnits(void)
{
  ENTRY();

  // get no of progress units
  i8 iProgressUnits = m_pDataStack->iPopInt(__func__);

  // get the Pds
  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    iProgressUnits,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addRowCount -- add a row to Pds with the row counts for ptn          */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddRowCount(void)
{
  ENTRY();

  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  LglPtn tLglPtn = m_pOp->tLglPtn();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tLglPtn);

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);
  pPdsRowBuffer->vBufferType(BT_COUNT);

  // RAY: UNCOMMENT next 2 lines to see relative sizes when running "count tableName;"
  //i8 iRelativeSize = pTable->iRelativeSize();
  //vTrace("Relative size of table %s is %lld\n"_Stz, pTable->szTableNm(), iRelativeSize);

  i8 nBuffers = 0;
  i8 nRows = 0;
  i8 jSumRowBytes = 0;

  pTable->vAnalyze(i_TablePtn, nBuffers, nRows, jSumRowBytes);

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    nRows,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addRowVargs -- add a row by variable arguments                       */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddRowVargs(void)
{
  ENTRY();

  i8 nArgs = m_pDataStack->iPopInt(__func__);

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);
  RowPix* pRowPix = pPdsRowBuffer->pRowPix();
  i8 nCols = pRowPix->nColumns();

  Row* pRow = pPdsRowBuffer->pAllotRow();

  if( ! pRow) {
    m_pDataStack->vPushInt(false);
    return;
  }

  i8   nSize;
  i8 iInt;
  s256 sStr;

  for (i8 i_Arg = 0; i_Arg < nArgs && i_Arg < nCols; i_Arg++) {

    eDM eDatum = (eDM) pRowPix->m_xaColPix[i_Arg].m_uDatum;

    switch (eDatum) {

      case DM_INT:
        iInt = m_pDataStack->iPopInt(__func__);
        pPdsRowBuffer->vDepositColVal(iInt);
        break;

      case DM_VCH:

        // STRING_DEPOSIT
        m_pDataStack->vPopStr(sStr,__func__);
        nSize = sStr.nSize();
        pPdsRowBuffer->vDepositColVal(sStr, nSize);
        // END STRING_DEPOSIT

        break;

      default:
        RASSERT(0);
    }

  }

  m_pDataStack->vPushInt(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[] -- get pds                                                           */
/*----------------------------------------------------------------------------*/
void Step::vPdsGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPds);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].bufferId -- fetch buffer id as address                               */
/*----------------------------------------------------------------------------*/
void Step::vPdsGetBufferIdAsAddr(void)
{
  ENTRY();

  Pds* pPds = *m_pLogicUnit->m_xpPds;

  m_pDataStack->vPushPtr((Object*)pPds->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].bufferType@ -- fetch the bufferType                                  */
/*----------------------------------------------------------------------------*/
void Step::vPdsBufferTypeFetch(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPds->eBufferType());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].bufferType! -- store buffer type                                     */
/*----------------------------------------------------------------------------*/
void Step::vPdsBufferTypeStore(void)
{
  ENTRY();

  eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_xpPds->vBufferType(eBufferType);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].chunkFile -- Pds chunk file                                          */
/*----------------------------------------------------------------------------*/
void Step::vPdsChunkFile(void)
{
  ENTRY();

  // get the seekOffsets
  Pds* pSeekOffsets = _pPds(*m_pLogicUnit->m_xpPds);

  i8 nRowsInSeekOffsets = 0;
  Row* pSeekOffsetRow = nullptr;

  // get the file we are chunking
  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);

  // allocate a work area for reading in last block of chunk
  char* aLastBlock = (char*)MemLib::aMallocBlock(C_CHUNK_BLOCK);

  i8 nSumBytes = 0;
  i8 nChunkBytes = C_UNKNOWN;
  i8 i_OfsOfLastBlockInChunk = C_UNKNOWN;

  // get the file offset of the EOF
  i8 i_EofOfs = m_pLogicUnit->m_iCounter1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process chunks till eof or till we get C_MAX_SEEKOFFSETS

  for (;;) {

    // get the file offset of next chunk
    i8 i_ChunkOfs = m_pLogicUnit->m_iCounter0;

    RASSERT (i_ChunkOfs < i_EofOfs);

    // allot row for seek offset info
    pSeekOffsetRow = pSeekOffsets->pAllotRow();

    // test for final chunk in file
    if (i_ChunkOfs + C_CHUNK_SIZE >= i_EofOfs) {
      // + final chunk in file

      // compute the total chunk bytes
      nChunkBytes = i_EofOfs - i_ChunkOfs;

      // end of file
      m_pLogicUnit->m_iCounter0 = i_EofOfs;
    }
    else {
      // -not final chunk in file

      // compute offset of last block in current chunk
      i_OfsOfLastBlockInChunk = i_ChunkOfs + C_CHUNK_SIZE - C_CHUNK_BLOCK;

      // read last block of current chunk
      pFile->vSeek(i_OfsOfLastBlockInChunk);
      i8 nBytesRead = pFile->nReadBytes(aLastBlock, C_CHUNK_BLOCK);

      // compute address of last data byte of last block of current chunk
      char *aLastDataByte = &aLastBlock[nBytesRead - 1];

      // find the last end of line in last block of current chunk
      char *aLastEol = aLastDataByte;
      bool bFoundEol = StrLib::bBackward(&aLastEol, aLastBlock, '\n');
      if (!bFoundEol)
        bFoundEol = StrLib::bBackward(&aLastEol, aLastBlock, '\r');
      RASSERT(bFoundEol);

      // compute the number of bytes which belong to the last block in chunk
      i8 nLastBlockBytes = nDifference(aLastEol, aLastBlock) + 1;

      // compute the total chunk bytes
      nChunkBytes = C_WSP_DATACHUNK - C_CHUNK_BLOCK + nLastBlockBytes;

      // compute end location of chunk
      i8 i_EndChunkLoc = i_ChunkOfs + nChunkBytes - 1;

      // advance to start of next chunk
      m_pLogicUnit->m_iCounter0 = i_EndChunkLoc + 1;
    }

    if (MY::pJob()->pJobTags()->bHasName("<OP_Chunk>"_Stz))
      vTrace("<OP_Chunk>: i_ChunkOfs %lld nChunkBytes %lld\n"_Stz,
             i_ChunkOfs,nChunkBytes);

    pSeekOffsetRow->vColVal_int(0,i_ChunkOfs);
    pSeekOffsetRow->vColVal_int(1,nChunkBytes);

    nSumBytes += nChunkBytes;

    if (++nRowsInSeekOffsets >= C_MAX_SEEKOFFSETS)
      break;
    if (m_pLogicUnit->m_iCounter0 >= i_EofOfs)
      break;
  }


  // free the read block
  MemLib::vFreeBlock(aLastBlock, C_CHUNK_BLOCK);

  if (MY::pJob()->pJobTags()->bHasName("<OP_Chunk>"_Stz))
    vTrace("<OP_Chunk>: nSumBytes %lld\n"_Stz,nSumBytes);

  m_pDataStack->vPushInt(nSumBytes);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].delete -- operand delete                                             */
/*----------------------------------------------------------------------------*/
void Step::vPdsDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpPds,nullptr);

  m_pLogicUnit->m_xpPds = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].firstRow -- get first row from the PdsRowBuffer                      */
/*----------------------------------------------------------------------------*/
void Step::vPdsFirstRow(void)
{
  ENTRY();

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  m_pDataStack->vPushPtr(pPdsRowBuffer->pFirstRow());
}

/*----------------------------------------------------------------------------*/
/* Pds[].isNull --                                                            */
/*----------------------------------------------------------------------------*/
void Step::vPdsIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPds.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].new -- new an Pds operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vPdsNew(void)
{
  ENTRY();

  ASSERT(m_pLogicUnit->m_xpPds.bIsNull());

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

  StreamDef* pStreamDef = pTable->pStreamDef();

  m_pLogicUnit->m_xpPds = new Pds(pStreamDef, false, eBufferType);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].rdCsvChunk -- reach a chunk from file given the Pds                  */
/*----------------------------------------------------------------------------*/
void Step::vPdsRdCsvChunk(void)
{
  ENTRY();

  ASSERT(*m_pLogicUnit->m_xpPds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get file and info arguments

  File* pCsvFile = _pFile(*m_pLogicUnit->m_xpFile0);
  Stz* szFilePath = pCsvFile->sFilePath();

  Row* pInfoRow = _pRow(m_pDataStack->pPopPtr(__func__));
  i8 iOffset = pInfoRow->iColVal_int(0);
  i8 nBytes = pInfoRow->iColVal_int(1);

  //vTrace("reading chunk @%lld of %lld bytes\n"_Stz, iOffset, nBytes);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create new csvBuffer and set csvMeta

#ifdef WITH_OOM_FAULTINJECTION
  PsxThd::vTlsSet(giTLS_CurrentPoint,(void*)6);
#endif

  CsvBuffer* pCsvBuffer = new CsvBuffer;

  CsvMeta* pCsvMeta = pCsvBuffer->pCsvMeta();

  pCsvMeta->vOriginNm(AM::NODE::szNodeNm());
  pCsvMeta->vFilePath(szFilePath);
  pCsvMeta->vOffset(iOffset);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read chunk into buffer

  char* azInto = (char*)pCsvBuffer->ayDataArea();

  pCsvFile->vSeek(iOffset);
  pCsvBuffer->vTake(nBytes);
  i8 nBytesRead = pCsvFile->nReadBytes(azInto,nBytes);

//  if (nBytesRead != nBytes) {
//    nBytesRead = pCsvFile->nReadBytes(azInto,nBytes);
//    ASSERT(nBytesRead == nBytes);
//  }

  ASSERT(nBytesRead == nBytes);
  UNUSED(nBytesRead);

  azInto[nBytes] = '\0';

  //vTrace("trailing: %s\n"_Stz,azInto + nBytes - 64);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // return the CsvBuffer

  m_pDataStack->vPushInt(nBytes);
  m_pDataStack->vPushInt(iOffset);

  m_pDataStack->vPushPtr(pCsvBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].set -- set the pds                                                   */
/*----------------------------------------------------------------------------*/
void Step::vPdsSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xpPds = _pPds(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].setLglPtn -- set logical partition                                   */
/*----------------------------------------------------------------------------*/
void Step::vPdsSetLglPtn(void)
{
  ENTRY();

  i8 iLglPtn = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_xpPds->vLglPtn((LglPtn)iLglPtn);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].tableDefName@ -- fetch the corresoonding tableDef name               */
/*----------------------------------------------------------------------------*/
void Step::vPdsTableDefNameFetch(void)
{
  ENTRY();

  s256 sTdefNm;

  eBT eBufferType = m_pLogicUnit->m_xpPds->eBufferType();

  switch(eBufferType) {
    case BT_AFFECTEDROWS:
      sTdefNm = C_TDEF_AFFECTEDROWS;
      break;
    case BT_COMMAND:
      sTdefNm = C_TDEF_COMMAND;
      break;
    case BT_COMPILEERRORS:
      sTdefNm = C_TDEF_COMPILEERRORS;
      break;
    case BT_COUNT:
      sTdefNm = C_TDEF_COUNT;
      break;
    case BT_DEREFMETRICS:
      sTdefNm = C_TDEF_DEREFMETRICS;
      break;
    case BT_EXECERRORS:
      sTdefNm = C_TDEF_EXECERRORS;
      break;
    case BT_EXPLAIN:
      sTdefNm = C_TDEF_EXPLAIN;
      break;
      //case BT_FORKCOMMAND:
      //	break;
    case BT_JOBMETRICS:
      sTdefNm = C_TDEF_JOBMETRICS;
      break;
    case BT_JOBNETDISSECT:
      sTdefNm = C_TDEF_JOBNETDISSECT;
      break;
    case BT_JOBNETWORK:
      sTdefNm = C_TDEF_JOBNETWORK;
      break;
    case BT_JOBOPERANDS:
      sTdefNm = C_TDEF_JOBOPERANDS;
      break;
    case BT_JOBOPERATORS:
      sTdefNm = C_TDEF_JOBOPERATORS;
      break;
      //case BT_LOADRESULT:
      //	sTdefNm = C_TDEF_AFFECTEDROWS;
      //	break;
    case BT_LOCKSTATS:
      sTdefNm = C_TDEF_LOCKSTATS;
      break;
    case BT_NODEINFO:
      sTdefNm = C_TDEF_NODEINFO;
      break;
    case BT_PAGEINREQUEST:
      sTdefNm = C_TDEF_PAGEINREQUEST;
      break;
    case BT_PATH:
      sTdefNm = C_TDEF_PATH;
      break;
    case BT_PROGRESSUNITS:
      sTdefNm = C_TDEF_PROGRESSUNITS;
      break;
    case BT_PROMPTOUT:
      sTdefNm = C_TDEF_PROMPTOUT;
      break;
    case BT_SEEKOFFSETS:
      sTdefNm = C_TDEF_SEEKOFFSETS;
      break;
    case BT_SYSLOCKS:
      sTdefNm = C_TDEF_SYSLOCKS;
      break;
    case BT_SYSMETRICS:
      sTdefNm = C_TDEF_SYSMETRICS;
      break;
    case BT_TOTALUNITS:
      sTdefNm = C_TDEF_TOTALUNITS;
      break;
    case BT_WARNINGS:
      sTdefNm = C_TDEF_WARNINGS;
      break;
    case BT_DATA:
    default:
      NEVER_REACH
  }

  m_pDataStack->vPushStr(sTdefNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].writeCsv -- write Pds to file as csv                                 */
/*----------------------------------------------------------------------------*/
void Step::vPdsWriteCsv(void)
{
  ENTRY();

  ASSERT(*m_pLogicUnit->m_xpPds);

  Table* pTableDef = _pTable(m_pDataStack->pPopPtr(__func__));

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  StreamDef* pStreamDef = pTableDef->pStreamDef();

  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);

  MetricsHelper::vWriteCsvPds(pPdsRowBuffer,pStreamDef,pFile);

  EXIT();
}

