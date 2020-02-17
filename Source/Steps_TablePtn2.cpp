/*============================================================================*/
/* Steps_TablePtn.cpp -- Table Partition Step Group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TablePartition2(void)
{
  vAddStep("tablePtn[].get"_Stz,           &Step::vTablePtnGet);
  vAddStep("tablePtn[].genCrwBuffers"_Stz, &Step::vTablePtnGenCrwBuffers);
  vAddStep("tablePtn[].purge"_Stz,         &Step::vTablePtnPurge);
  vAddStep("tablePtn[].root"_Stz ,         &Step::vTablePtnRoot);
  vAddStep("tablePtn[].set"_Stz  ,         &Step::vTablePtnSet);
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].get -- get table                                                */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnGet(void)
{
  ENTRY(); // ENTRY("vTablePtnGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].genCrwBuffers -- generate crw buffers for this tablePtn         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnGenCrwBuffers(void)
{
  ENTRY(); // ENTRY("vTablePtnGenCrwBuffers"_Stz);
  
  
  // get no of buffers to generate
  i8 nBuffers = m_pDataStack->iPopInt(__func__);
  
  // get the table ptn
  TablePtn* pTablePtn = *m_pRegSet->m_xpTablePtn;
  
  // get the table and streamdef
  Table* pTable = pTablePtn->pTable();
  StreamDef* pStreamDef = pTable->pStreamDef();

  ExtractRow* pExtractRow = new ExtractRow(*m_pRegSet->m_xpTable);
  Row* pCsvRow = new Row(pStreamDef,C_ALL);
  Row* pCrwRow = nullptr;
  
  // create a row compactor for the crw buffer
  RowCompactor* pRowCompactor = new RowCompactor;
  
  // create a new crw buffer
  CrwBuffer* pCrwBuffer = new CrwBuffer();
  pCrwBuffer->vIsPermanent(false);
  pCrwBuffer->vTable(pTable);
  pCrwBuffer->vAppendOpen(pRowCompactor, true);
  
  CrwBuffer* pRootBuffer = pCrwBuffer;
  CrwBuffer* pPrevBuffer = nullptr;
  
  while (nBuffers) {
    
    // set the prev buffer
    pPrevBuffer = pCrwBuffer;
    
    // reset the csv row
    pCsvRow->vResetValues();
    
    // generate a random row
    RowGenerator::vRandom(pCsvRow,pStreamDef);
    
    // append row to buffer
    if (! pCrwBuffer->bAppendRow(pCsvRow)) {
      
      // close current buffer
      pCrwBuffer->vAppendClose();
      vTrace("Generated CrwBuffer with %lld compact rows\n"_Stz,pCrwBuffer->nRowCount());
      
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
      bool bAppend = pCrwBuffer->bAppendRow(pCsvRow);
      ASSERT(bAppend);
      nBuffers--;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushPtr(pRootBuffer);
  
  delete pRowCompactor;
  delete pCsvRow;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].purge -- purge prior versions of this table ptn                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnPurge(void)
{
  ENTRY(); // ENTRY("vTablePtnPurge"_Stz);
  
  TablePtn* pTablePtn = *m_pRegSet->m_xpTablePtn;
  
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
  ENTRY(); // ENTRY("vTablePtnSet"_Stz);
  
  TablePtn* pTablePtn = _pTablePtn(*m_pRegSet->m_xpTablePtn);
  
  m_pDataStack->vPushPtr(pTablePtn->pRootCrwBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tablePtn[].set -- set the tablePtn                                         */
/*----------------------------------------------------------------------------*/
void Step::vTablePtnSet(void)
{
  ENTRY(); // ENTRY("vTablePtnSet"_Stz);
  
  m_pRegSet->m_xpTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
