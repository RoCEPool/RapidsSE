/*============================================================================*/
/* Steps_InfoSet.cpp -- Register InfoSet step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowBuffer.hpp"

#include "InfoSet.hpp"

#include "eYX.h"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "MetricsHelper.hpp"
#include "LockMetrics.hpp"

#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "TagConstants.h"

static const i8 C_CHUNK_BLOCK        = 2048;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_InfoSet(void)
{
  vAddStep("InfoSet[]"_Stz,                  &Step::vInfoSetGet);
  vAddStep("InfoSet[].addCommandError"_Stz,  &Step::vInfoSetAddCommandError);
  vAddStep("InfoSet[].addExecError"_Stz,     &Step::vInfoSetAddExecError);
  vAddStep("InfoSet[].addLockStats"_Stz,     &Step::vInfoSetAddLockStats);
  vAddStep("InfoSet[].addNodeInfo"_Stz,      &Step::vInfoSetAddNodeInfo);
  vAddStep("InfoSet[].addProgressUnits"_Stz, &Step::vInfoSetAddProgressUnits);
  vAddStep("InfoSet[].addRowCount"_Stz,      &Step::vInfoSetAddRowCount);
  vAddStep("InfoSet[].addRowVargs"_Stz,      &Step::vInfoSetAddRowVargs);
  vAddStep("InfoSet[].addTotalUnits"_Stz,    &Step::vInfoSetAddTotalUnits);
  vAddStep("InfoSet[].addWarning"_Stz,       &Step::vInfoSetAddWarning);
  vAddStep("InfoSet[].chunkFile"_Stz,        &Step::vInfoSetChunkFile);
  vAddStep("InfoSet[].colVal@"_Stz,          &Step::vInfoSetColValFetch);
  vAddStep("InfoSet[].delete"_Stz,           &Step::vInfoSetDelete);
  vAddStep("InfoSet[].detectLineEnding"_Stz, &Step::vInfoSetDetectLineEnding);
  vAddStep("InfoSet[].firstRow"_Stz,         &Step::vInfoSetFirstRow);
  vAddStep("InfoSet[].get"_Stz,              &Step::vInfoSetGet);
  vAddStep("InfoSet[].infoRows"_Stz,         &Step::vInfoSetInfoRows);
  vAddStep("InfoSet[].isNotNull"_Stz,        &Step::vInfoSetIsNotNull);
  vAddStep("InfoSet[].isNull"_Stz,           &Step::vInfoSetIsNull);
  vAddStep("InfoSet[].isProgress"_Stz,       &Step::vInfoSetIsProgress);
  vAddStep("InfoSet[].isTotalUnits"_Stz,     &Step::vInfoSetIsTotalUnits);
  vAddStep("InfoSet[].isWarning"_Stz,        &Step::vInfoSetIsWarning);
  vAddStep("InfoSet[].name"_Stz,             &Step::vInfoSetName);
  vAddStep("InfoSet[].new"_Stz,              &Step::vInfoSetNew);
  vAddStep("InfoSet[].nextRow"_Stz,          &Step::vInfoSetNextRow);
  vAddStep("InfoSet[].opInfoset@"_Stz,       &Step::vInfoSetOpInfosetFetch);
  vAddStep("InfoSet[].opInfoset!"_Stz,       &Step::vInfoSetOpInfosetStore);
  vAddStep("InfoSet[].rdCsvChunk"_Stz,       &Step::vInfoSetRdCsvChunk);
  vAddStep("InfoSet[].set"_Stz,              &Step::vInfoSetSet);
  vAddStep("InfoSet[].writeCsv"_Stz,         &Step::vInfoSetWriteCsv);
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addCommandError -- add a row to infoset with the command error   */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddCommandError(void)
{
  ENTRY();

  s4K sErrorText;

  // get the table
  Table* pStreamTable = _pTable(m_pDataStack->pPopPtr(__func__));

  // get the streamDef from the table
  StreamDef* pStreamDef = pStreamTable->pStreamDef();

  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_COMPILEERRORS);

  // get the error information
  eEC eErrorCode = (eEC) m_pDataStack->iPopInt(__func__);
  i8 iLineNo = m_pDataStack->iPopInt(__func__);
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  m_pDataStack->vPopStr(sErrorText, __func__);

  Grid* pGrid = AM::pGrid();

  Row* pRow = pInfoSet->pAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pGrid->pThisNode()->szNodeNm(),
                                  eErrorCode,
                                  iLineNo,
                                  iColumnNo,
                                  sErrorText.azChar(),
                                  C_VARGS_END);

  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addExecError -- add a row to infoset with the exec error         */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddExecError(void)
{
  ENTRY();

  s4K sErrorText;

  // get the table
  Table* pStreamTable = _pTable(m_pDataStack->pPopPtr(__func__));

  // get the streamDef from the table
  StreamDef* pStreamDef = pStreamTable->pStreamDef();

  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_EXECERRORS);

  // get the error information
  eEC eErrorCode = (eEC) m_pDataStack->iPopInt(__func__);
  m_pDataStack->vPopStr(sErrorText, __func__);

  Grid* pGrid = AM::pGrid();

  //pStreamDef->vListContents(1);
  
  Row* pRow = pInfoSet->pAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pGrid->pThisNode()->szNodeNm(),
                                  (i8)eErrorCode,
                                  sErrorText.azChar(),
                                  C_VARGS_END);

  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
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
  
  Row* pRow = pInfoSet->pAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
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
  
  Row* pRow = pInfoSet->pAllotRow(pStreamDef,
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
  
  LglPtn tLglPtn = m_pOp->tLglPtn();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(tLglPtn);

  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_COUNT);

  // RAY: UNCOMMENT next 2 lines to see relative sizes when running "count tableName;"
  //i8 iRelativeSize = pTable->iRelativeSize();
  //vTrace("Relative size of table %s is %lld\n"_Stz, pTable->szTableNm(), iRelativeSize);

  i8 nBuffers = 0;
  i8 nRows = 0;
  i8 jSumRowBytes = 0;

  pTable->vAnalyze(i_TablePtn, nBuffers, nRows, jSumRowBytes);
  
  Row* pRow = pInfoSet->pAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  nRows,
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].addRowVargs -- add a row by variable arguments                   */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddRowVargs(void)
{
  ENTRY();
  
  Table* pTable = _pTable(*m_pLogicUnit->m_xpInfoSetTable);
  StreamDef* pStreamDef = pTable->pStreamDef();
  i8 nCols = pStreamDef->nColumns();
  i8 nArgs = m_pDataStack->iPopInt(__func__);
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  
  Row* aRow = pInfoSet->RowBuffer::pAllotRow();
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
        aRow->vColVal_int(i_Arg,iInt);
        break;
        
      case DM_VCH:
        
        // STRING_DEPOSIT
        m_pDataStack->vPopStr(sStr,__func__);
        nSize = sStr.nSize();
        szTail = (Stz*)pInfoSet->aAllotTail(nSize);
        StrLib::vCopy(szTail->azChar(), nSize, sStr.azChar());
        //aRow->vColVal_ofs(i_Arg,iOfs);
        aRow->vColVal_ofs(i_Arg,szTail);
        // END STRING_DEPOSIT
        
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
  
  Row* pRow = pInfoSet->pAllotRow(pStreamDef,
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
/* InfoSet[].addWarning -- add a row to infoset with a warning                */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetAddWarning(void)
{
  ENTRY();

  s4K sReason;

  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  // Get the warning reason
  m_pDataStack->vPopStr(sReason, __func__);

  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();

  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pInfoSet->vOpInfoSet(OI_WARNINGS);

  Row* pRow = pInfoSet->pAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  sReason.szStz(),
                                  nullptr,
                                  nullptr,
                                  C_VARGS_END);

  m_pDataStack->vPushBol(pRow != nullptr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].chunkFile -- info set chunk file                                 */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetChunkFile(void)
{
  ENTRY();

  // get the seekOffsets
  InfoSet* pSeekOffsets = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  pSeekOffsets->vOpInfoSet(OI_SEEKOFFSETS);
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
    pSeekOffsetRow = pSeekOffsets->RowBuffer::pAllotRow();

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
/* InfoSet[].colVal@ -- column value fetch                                    */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetColValFetch(void)
{
  ENTRY();
    
  Table* pTable = _pTable(*m_pLogicUnit->m_xpInfoSetTable);
  StreamDef* pStreamDef = pTable->pStreamDef();
  i8 nCols = pStreamDef->nColumns();
  UNUSED(nCols);

  i8 i_Col = m_pDataStack->iPopInt(__func__);

  Column* pColumn = pStreamDef->pGetColumn(i_Col);
  eDM eDatum = pColumn->eDatum();

  Row* aRow = *m_pLogicUnit->m_xpInfoSetRow;
  
  i8 iInt;
  s256 sStr;
//  Dtm wDtm;

  switch (eDatum) {
      
    case DM_INT:
      iInt = aRow->iColVal_int(i_Col);
      m_pDataStack->vPushInt(iInt);
      break;
      
    case DM_VCH:
//      wDtm = aRow->wColDtm(i_Col);
      sStr = aRow->szColVal_ofs(i_Col);
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
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpInfoSet,nullptr);
  
  m_pLogicUnit->m_xpInfoSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].detectLineEnding -- push line ending of the file onto data stack */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetDetectLineEnding(void)
{
  ENTRY();

  Stz* szLineEnding = nullptr;

  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);

  i8 iBytesToRead = C_CHUNK_BLOCK;
  i8 iBytesRead;

  bool bGotCr = false;  // '\r'

  // allocate a work area for reading in last block of chunk
  char* aBlock = (char*)MemLib::aMallocBlock(C_CHUNK_BLOCK);

  pFile->vSeek(0);
  iBytesRead = pFile->nReadBytes(aBlock, iBytesToRead);
  char* aEnd = &aBlock[iBytesRead];

  while (iBytesRead > 0) {
    for (char* aCrs = aBlock ; aCrs != aEnd; aCrs++) {

      if (*aCrs == '\r') {
        if (aCrs + 1 != aEnd) {
          // There's at least one more character in the block
          szLineEnding = (*(aCrs + 1) == '\n') ? "\r\n"_Stz : "\r"_Stz;
          break;

        } else {
          // This is the last character in the block. We need to check the next block
          bGotCr = true;
        }

      } else if (*aCrs == '\n') {
        szLineEnding = bGotCr ? "\r\n"_Stz : "\n"_Stz;
        break;

      } else if (bGotCr) {
        szLineEnding = "\r"_Stz;
        break;
      }
    }

    if (szLineEnding) {
      break;
    }

    iBytesRead = pFile->nReadBytes(aBlock, iBytesToRead);
  }

  if (szLineEnding == nullptr) {
    szLineEnding = "\n"_Stz;
  }

  // free the read block
  MemLib::vFreeBlock(aBlock, C_CHUNK_BLOCK);

  m_pDataStack->vPushStz(szLineEnding);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].firstRow -- get first row from the infoset                       */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetFirstRow(void)
{
  ENTRY();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);
  
  m_pDataStack->vPushPtr(pInfoSet->pFirstRow());
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

  i8 nRows = pInfoSet->nRows();
  m_pDataStack->vPushInt(nRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].isNotNull -- is operand not null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpInfoSet.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].isNull -- is operand null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpInfoSet.bIsNull() ? 1 : 0);

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
/* InfoSet[].isWarning --is infoset a warning info set ?                      */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetIsWarning(void)
{
  ENTRY();

  ASSERT ( ! m_pLogicUnit->m_xpInfoSet.bIsNull() );

  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);

  m_pDataStack->vPushInt(pInfoSet->eOpInfoSet() == OI_WARNINGS ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* InfoSet[].name -- get infoset name                                         */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetName(void)
{
  ENTRY();
  
  InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpInfoSet);

  Stz* szBufferNm = pInfoSet->szBufferNm();
  
  m_pDataStack->vPushStz(szBufferNm);

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
  Row* pRow = _pRow(*m_pLogicUnit->m_xpInfoSetRow);
  
  ASSERT(pRow);
  
  m_pDataStack->vPushPtr(pInfoSet->pNextRow(pRow));
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
/* InfoSet[].rdCsvChunk -- reach a chunk from file given the infoSet          */
/*----------------------------------------------------------------------------*/
void Step::vInfoSetRdCsvChunk(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpInfoSet);

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
/* InfoSet[].set -- set InfoSet                                               */
/*----------------------------------------------------------------------------*/
#include <execinfo.h>
void Step::vInfoSetSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pLogicUnit->m_xpInfoSet.bIsNull());
  m_pLogicUnit->m_xpInfoSet = _pInfoSet(pOperand);
  
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
