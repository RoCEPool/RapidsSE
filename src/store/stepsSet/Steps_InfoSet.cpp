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
  //vAddStep("InfoSet[].addLockStats"_Stz,     &Step::vInfoSetAddLockStats);
  //vAddStep("InfoSet[].addNodeInfo"_Stz,      &Step::vInfoSetAddNodeInfo);
  //vAddStep("InfoSet[].addProgressUnits"_Stz, &Step::vInfoSetAddProgressUnits);
  vAddStep("InfoSet[].addRowCount"_Stz,      &Step::vInfoSetAddRowCount);
  vAddStep("InfoSet[].addRowVargs"_Stz,      &Step::vInfoSetAddRowVargs);
  //vAddStep("InfoSet[].addTotalUnits"_Stz,    &Step::vInfoSetAddTotalUnits);
  vAddStep("InfoSet[].addWarning"_Stz,       &Step::vInfoSetAddWarning);
  //vAddStep("InfoSet[].chunkFile"_Stz,        &Step::vInfoSetChunkFile);
  vAddStep("InfoSet[].colVal@"_Stz,          &Step::vInfoSetColValFetch);
  vAddStep("InfoSet[].delete"_Stz,           &Step::vInfoSetDelete);
  vAddStep("InfoSet[].detectLineEnding"_Stz, &Step::vInfoSetDetectLineEnding);
  vAddStep("InfoSet[].firstRow"_Stz,         &Step::vInfoSetFirstRow);
  vAddStep("InfoSet[].get"_Stz,              &Step::vInfoSetGet);
  vAddStep("InfoSet[].infoRows"_Stz,         &Step::vInfoSetInfoRows);
  vAddStep("InfoSet[].isNotNull"_Stz,        &Step::vInfoSetIsNotNull);
  vAddStep("InfoSet[].isNull"_Stz,           &Step::vInfoSetIsNull);
  //vAddStep("InfoSet[].isProgress"_Stz,       &Step::vInfoSetIsProgress);
  //vAddStep("InfoSet[].isTotalUnits"_Stz,     &Step::vInfoSetIsTotalUnits);
  vAddStep("InfoSet[].isWarning"_Stz,        &Step::vInfoSetIsWarning);
  vAddStep("InfoSet[].name"_Stz,             &Step::vInfoSetName);
  vAddStep("InfoSet[].new"_Stz,              &Step::vInfoSetNew);
  vAddStep("InfoSet[].nextRow"_Stz,          &Step::vInfoSetNextRow);
  vAddStep("InfoSet[].opInfoset@"_Stz,       &Step::vInfoSetOpInfosetFetch);
  vAddStep("InfoSet[].opInfoset!"_Stz,       &Step::vInfoSetOpInfosetStore);
  //vAddStep("InfoSet[].rdCsvChunk"_Stz,       &Step::vInfoSetRdCsvChunk);
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
