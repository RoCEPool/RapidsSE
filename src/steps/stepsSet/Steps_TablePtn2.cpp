/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePartition2(void)
{
  vAddStep("tablePtn[]"_Stz,                  &Step::vTablePtnGet);
  
  vAddStep("tablePtn[].appendBuffer"_Stz,     &Step::vTablePtnAppendBuffer);
  vAddStep("tablePtn[].genCrwBuffers"_Stz,    &Step::vTablePtnGenCrwBuffers);
  vAddStep("tablePtn[].inflateCrwBuffer"_Stz, &Step::vTablePtnInflateCrwBuffer);
  vAddStep("tablePtn[].populateIndex"_Stz,    &Step::vTablePtnPopulateIndex);
  vAddStep("tablePtn[].purge"_Stz,            &Step::vTablePtnPurge);
  vAddStep("tablePtn[].root"_Stz ,            &Step::vTablePtnRoot);
  vAddStep("tablePtn[].set"_Stz  ,            &Step::vTablePtnSet);
  vAddStep("tablePtn[].truncate"_Stz,         &Step::vTablePtnTruncate);
  vAddStep("tablePtn[].truncateIndex"_Stz,    &Step::vTablePtnTruncateIndex);
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].appendBuffer -- append a buffer into the tablePtn               */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnAppendBuffer(void)
{
  ENTRY();
  
  // get the CrwBuffer
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));
  
  // get the tablePtn from the register
  TablePtn* pTablePtn = _pTablePtn(*m_pLogicUnit->m_xpTablePtn);
  
  // append the buffer to the tablePtn
  pTablePtn->vAppendBuffer(pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[] -- get table                                                    */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].genCrwBuffers -- generate crw buffers for this tablePtn         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnGenCrwBuffers(void)
{
  ENTRY();
  
  // get no of buffers to generate
  i8 nBuffers = m_pDataStack->iPopInt(__func__);
  
  // get the table ptn
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  
  // get the table and streamdef
  Table* pTable = pTablePtn->pTable();
  StreamDef* pStreamDef = pTable->pStreamDef();

  HV_Row hCsvRow(pStreamDef,C_ALL);
  
  // create a row compactor for the crw buffer
  RowCompactor* pRowCompactor = new RowCompactor;
  
  // create a new crw buffer
  CrwBuffer* pCrwBuffer = new CrwBuffer();
  pCrwBuffer->vIsPermanent(false);
  pCrwBuffer->vTable(pTable);
  pCrwBuffer->vAppendOpen(pRowCompactor, true);
  nBuffers--;
  
  CrwBuffer* pRootBuffer = pCrwBuffer;
  CrwBuffer* pPrevBuffer = nullptr;
  
  while (nBuffers) {
    
    // set the prev buffer
    pPrevBuffer = pCrwBuffer;
    
    // reset the csv row
    hCsvRow->vResetValues();
    
    // generate a random row
    RowGenerator::vRandom(hCsvRow,pStreamDef);
    
    // append row to buffer
    if (! pCrwBuffer->bAppendSrw(hCsvRow)) {
      
      // close current buffer
      pCrwBuffer->vAppendClose();
      //vTrace("Generated CrwBuffer with %lld compact rows\n"_Stz,pCrwBuffer->nRowCount());
      
      // create a new buffer
      pCrwBuffer = new CrwBuffer();
      
      // link the new buffer with the prev
      if (pPrevBuffer)
        pPrevBuffer->vNextBuffer(pCrwBuffer);
      pCrwBuffer->vPrevBuffer(pPrevBuffer);
      
      // set the buffer
      pCrwBuffer->vIsPermanent(false);
      pCrwBuffer->vTable(pTable);
      pCrwBuffer->vAppendOpen(pRowCompactor, true);
      
      // now append the row
      bool bAppend = pCrwBuffer->bAppendSrw(hCsvRow);
      ASSERT(bAppend);
      nBuffers--;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // close current buffer
  pCrwBuffer->vAppendClose();
  
  m_pDataStack->vPushPtr(pRootBuffer);
  
  delete pRowCompactor;
  //delete pCsvRow;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].inflateCrwBuffer -- inflate crw buffers for this tablePtn       */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnInflateCrwBuffer(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(*m_pLogicUnit->m_xpOperandA);
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  StreamDef* pStreamDef = pTablePtn->pTable()->pStreamDef();
  
  // create a row
  HV_Row hRow(pStreamDef,C_ALL);
  
//#ifdef WITH_DEBUG_VERSION
//  HV_Rec hvRec(pStreamDef,C_ALL);
//#endif
  
  i8 nRows = pCrwBuffer->nRowCount();
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    // get compact row
    Crw* aCompactRow = (Crw*)pCrwBuffer->aGetHead(i_Row);
    
    // inflate compact row into row
    RowInflator::vInflate_AllColumns_into_RowSingle(hRow,aCompactRow);

//#ifdef WITH_DEBUG_VERSION
//    // append the row to the record
//    hvRec->vAppend(hRow);
//    vTrace("row: (%s)\n"_Stz,hvRec->szRecStr());
//#endif
  }
  
  m_pDataStack->vPushInt(nRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].populateIndex -- populate index of this table ptn               */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnPopulateIndex(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  
  bool bPopulated = pTablePtn->bPopulateIndex(iIndexNo);
  
  m_pDataStack->vPushInt(bPopulated ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].purge -- purge prior versions of this table ptn                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnPurge(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  
  // get the oldest perspective
  t8 tOldestPsp = AM::pTxSet()->tOldestPsp();
  pTablePtn->pReadApi()->vPurgePriorVersions(tOldestPsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].root -- get the tablePtn root                                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnRoot(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = _pTablePtn(*m_pLogicUnit->m_xpTablePtn);
  
  m_pDataStack->vPushPtr(pTablePtn->pRootCrwBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].set -- set the tablePtn                                         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].truncate -- truncate the tablePtn for the table                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnTruncate(void)
{
  ENTRY();
  
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  pTablePtn->vTruncate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].truncateIndex -- truncate the tablePtn for the index            */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnTruncateIndex(void)
{
  ENTRY();
  
  // get the index to be truncated
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  TablePtn* pTablePtn = *m_pLogicUnit->m_xpTablePtn;
  pTablePtn->vTruncateIndex(pIndex);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
