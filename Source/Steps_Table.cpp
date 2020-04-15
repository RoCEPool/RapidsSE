/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Table(void)
{
  vAddStep("table[]"_Stz,                  &Step::vTableGet);
  vAddStep("table[].set"_Stz,              &Step::vTableSet);
  
  vAddStep("table.addIndex"_Stz,           &Step::vTableAddIndex);
  vAddStep("table[].columnCount@"_Stz,     &Step::vTableColumnCountFetch);
  vAddStep("table[].column@"_Stz,          &Step::vTableColumnFetch);
  vAddStep("table[].dump"_Stz,             &Step::vTableDump);
  vAddStep("table.findColumn"_Stz,         &Step::vTableFindColumn);  
  vAddStep("table[].genCsvBuffer"_Stz,     &Step::vTableGenCsvBuffer);
  vAddStep("table.indexCount"_Stz,         &Step::vTableIndexCount);
  vAddStep("table.init"_Stz,               &Step::vTableInit);
  vAddStep("table.isColumn"_Stz,           &Step::vTableIsColumn);
  vAddStep("table[].isDefinitionOnly"_Stz, &Step::vTableIsDefinitionOnly);
  vAddStep("table.lazyBuildIndexes"_Stz,   &Step::vTableLazyBuildIndexes);
  vAddStep("table.publish"_Stz,            &Step::vTablePublish);
  vAddStep("table.purge"_Stz,              &Step::vTablePurge);
  vAddStep("table.removeIndex"_Stz,        &Step::vTableRemoveIndex);
  vAddStep("table.scan"_Stz,               &Step::vTableScan);
  vAddStep("table[].slots@"_Stz,           &Step::vTableSlotsFetch);
  vAddStep("table[].size"_Stz,             &Step::vTableSize);
  vAddStep("table.subLoaders"_Stz,         &Step::vTableSubLoaders);
  vAddStep("table.tableDef@"_Stz,          &Step::vTableTableDefFetch);
  vAddStep("table.tableDef!"_Stz,          &Step::vTableTableDefStore);
  vAddStep("table[].tablePtn@"_Stz,        &Step::vTableTablePtnFetch);
  vAddStep("table.truncate"_Stz,           &Step::vTableTruncate);
  vAddStep("table[].verify"_Stz,           &Step::vTableVerify);
  vAddStep("table[].visualize"_Stz,        &Step::vTableVisualize);
  
  // to go
  vAddStep("table.getColumnsAsString"_Stz, &Step::vTableGetColumnsAsString);
  vAddStep("table.getPtn"_Stz,             &Step::vTableGetPtn);
  vAddStep("table.slots"_Stz,              &Step::vTableSlots);
}

/*----------------------------------------------------------------------------*/
/* table[] -- get table                                                       */
/*----------------------------------------------------------------------------*/
void Step::vTableGet(void)
{
  ENTRY(); // ENTRY("vTableRegGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].set -- set table                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSet(void)
{
  ENTRY(); // ENTRY("vTableRegSet"_Stz);
  
  m_pRegSet->m_xpTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.addIndex -- add index to the table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableAddIndex(void)
{
  ENTRY(); // ENTRY("vTableAddIndex"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  pTable->vAddIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.lazyBuildIndexes -- lazy build of indexes (from lazy chains)         */
/*----------------------------------------------------------------------------*/
void Step::vTableLazyBuildIndexes(void)
{
  ENTRY(); // ENTRY("vTableLazyBuildIndexes"_Stz);
  i8 iSlot;
  s48K sString;
  
  m_pDataStack->vPopStr(sString,__func__);
  
  Table* pTable = reinterpret_cast<Table*>(sString.iToI8());
  ASSERT(CN_TABLE == pTable->eClass());
  
  m_pDataStack->vPopStr(sString,__func__);
  
  iSlot = sString.iToI8();
  
  pTable->vLazyBuildIndexes(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].column -- get the column from the table                            */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnFetch(void)
{
  ENTRY(); // ENTRY("vTableColumnFetch"_Stz);
  
  Table* pTable = _pTable(*m_pRegSet->m_xpTable);
  
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pDataStack->vPushPtr(pStreamDef->pGetColumn(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].columnCount -- get column count                                    */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnCountFetch(void)
{
  ENTRY(); // ENTRY("vTableColumnCountFetch"_Stz);
  
  StreamDef* pStreamDef = m_pRegSet->m_xpTable->pStreamDef();
  i8 nColumns = pStreamDef->nColumns(C_ALL);
  
  m_pDataStack->vPushInt(nColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].dump                                                               */
/*----------------------------------------------------------------------------*/
void Step::vTableDump(void)
{
  ENTRY(); // ENTRY("vTableDump"_Stz);
  
  vTrace("TableDump\n"_Stz);
  
  Table* pTable = _pTable(*m_pRegSet->m_xpTable);
  
  for (i8 iPartition = 0; iPartition < AM::pGrid()->nSlotsPerNode(); iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    vTrace("Partition %lld\n"_Stz, iPartition);
    pPartition->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].genCsvBuffer -- generate a random row for table                    */
/*----------------------------------------------------------------------------*/
void Step::vTableGenCsvBuffer(void)
{
  ENTRY(); // ENTRY("vTableGenCsvBuffer"_Stz);

  Table* pTable = _pTable(*m_pRegSet->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
  HV_Row hRow(pStreamDef,C_ALL);

  i8 nRecords = 0;
  for ( ; ; nRecords++) {
    
    hRow->vResetValues();
    RowGenerator::vRandom(hRow,pStreamDef);
    
    SP_Record spRecord(hRow);
    StrLib::vConcat(spRecord.sz()->azChar(),'\n');               //! sub-optimal
    //vTrace("Random Rec: %s"_Stz,spRecord.sz());

    if (! pCsvBuffer->szAppendRecord(spRecord.sz()))
      break;
  }
  
  //pCsvBuffer->vPrintRecords();
  
  //delete pCsvBuffer;
  //delete pRow;
  
  m_pDataStack->vPushPtr(pCsvBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.getColumnsAsString -- get columns as a csv string                    */
/*----------------------------------------------------------------------------*/
void Step::vTableGetColumnsAsString(void)
{
  ENTRY(); // ENTRY("vTableGetColumnsAsString"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  i8 nColumns = pStreamDef->nColumns(C_ALL);
  
  s16KB s;
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    if (i_Column != 0)
    s.vConcat(","_Stz);
    
    if (i_Column == 0)
      s.vCopy(pStreamDef->pGetColumn(i_Column)->szColNm());
    else
      s.vConcat(pStreamDef->pGetColumn(i_Column)->szColNm());
    
    s.vConcat(" "_Stz);
    s.vConcat(aCT[pStreamDef->pGetColumn(i_Column)->eColumnType()]);
  }
  
  m_pDataStack->vPushStr(s);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.getPtn -- get table partition given slot no                          */
/*----------------------------------------------------------------------------*/
void Step::vTableGetPtn(void)
{
  ENTRY(); // ENTRY("vTableGetPtn"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Ptn = m_pDataStack->iPopAsInt(__func__);
  
  // get the table partition
  i8 i_Slot = TablePtn::iPartitionToSlot(i_Ptn);
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);

  m_pDataStack->vPushPtr(pTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].tablePtn@ -- feth the table partition                              */
/*----------------------------------------------------------------------------*/
void Step::vTableTablePtnFetch(void)
{
  ENTRY(); // ENTRY("vTableTablePtnFetch"_Stz);
  
  Job* pJob = m_pTask->m_pJob;
  
  i8 iPtn = pJob->m_pOp->nPartition();
  
  i8 iSlot = TablePtn::iPartitionToSlot(iPtn);
  TablePtn* pTablePtn = m_pRegSet->m_xpTable->pSlots()->pPartition(iSlot);
  
  m_pDataStack->vPushPtr(pTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.indexCount -- get the no of indexes in the table                     */
/*----------------------------------------------------------------------------*/
void Step::vTableIndexCount(void)
{
  ENTRY(); // ENTRY("vTableIndexCount"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pTable->pIdxVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableInit(void)
{
  ENTRY(); // ENTRY("vTableInit"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  new(pTable) Table;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.findColumn -- find the column from the table ?                       */
/*----------------------------------------------------------------------------*/
void Step::vTableFindColumn(void)
{
  ENTRY(); // ENTRY("vTableFindColumn"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  s32 sColumnName;
  m_pDataStack->vPopStr(sColumnName,__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pColumn = nullptr;
  bool bFound = false;
  
  // look for the column in the streamDef
  i8 nTableColumns = pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nTableColumns; i_Col++) {
    
    pColumn = pStreamDef->pGetColumn(i_Col);
    
    if (pColumn->szColNm()->bSame(sColumnName)) {
      bFound = true;
      break;
    }
  }
  RASSERT(bFound);
  
  m_pDataStack->vPushPtr(pColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isColumn -- is the column a valid column of the table ?              */
/*----------------------------------------------------------------------------*/
void Step::vTableIsColumn(void)
{
  ENTRY(); // ENTRY("vTableInit"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  s32 sColumnName;
  m_pDataStack->vPopStr(sColumnName,__func__);
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  Column* pColumn;
  bool bFound = false;
  
  // look for the column in the streamDef
  i8 nTableColumns = pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nTableColumns; i_Col++) {
    
    pColumn = pStreamDef->pGetColumn(i_Col);
    
    if (pColumn->szColNm()->bSame(sColumnName)) {
      bFound = true;
      break;
    }
  }
  
  m_pDataStack->vPushInt(bFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isDefinitionOnly -- is the table a definition only                   */
/*----------------------------------------------------------------------------*/
void Step::vTableIsDefinitionOnly(void)
{
  ENTRY(); // ENTRY("vTableIsDefinitionOnly"_Stz);
  
  Table* pTable = _pTable(*m_pRegSet->m_xpTable);

  m_pDataStack->vPushInt(pTable->bIsDefinitionOnly());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.publish -- purge rows/buffers in table on data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePublish(void)
{
  ENTRY(); // ENTRY("vTablePublish"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  i8 iSlot = m_pDataStack->iPopAsInt(__func__);
  
  pTable->vPublish(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.purge -- purge rows/buffers in table on data stack                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePurge(void)
{
  ENTRY(); // ENTRY("vTablePurge"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  i8 iSlot = m_pDataStack->iPopAsInt(__func__);
  
  i8 iOlderThanSeconds = m_pDataStack->iPopAsInt(__func__);
  
  bool bPurged = pTable->bPurge(iSlot,iOlderThanSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.removeIndex -- remove an index from the table                        */
/*----------------------------------------------------------------------------*/
void Step::vTableRemoveIndex(void)
{
  ENTRY(); // ENTRY("vTableRef"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

  pTable->vRemoveIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.scan -- scan the table                                               */
/*----------------------------------------------------------------------------*/
void Step::vTableScan(void)
{
  ENTRY(); // ENTRY("vTableScan"_Stz);
  
  FetchBuffer* pFetchBuffer;
  Datum xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  i8 iSlot = pJob_SubFetch->i_Slot();
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position in the first row buffer
  CrwBuffer* pCrwBuffer = pPartition->pRootCrwBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entryBuffer ?
  if (! pCrwBuffer) {
    // mark this buffer as the last buffer
    pFetchBuffer->vIsLast(true);
    
    // put the buffer in the session bufferQueue
    pQueue->vAppend(pFetchBuffer);
    
    EXIT();
    return;
  }
  
  
  while (pCrwBuffer) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // iterate thru the buffer till no more entries
    for (i8 iRow = 0; iRow < pCrwBuffer->nRowCount(); iRow++) {
      
      // get the row from the buffer
      Crw* aCrw = pCrwBuffer->aCrw(iRow);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // insert row from entry in FetchBuffer
      if ( ! pFetchBuffer->bAppendCrw(aCrw)) {
        
        // Buffer is Full
        // put this FetchBuffer in session bufferQueue
        pQueue->vAppend(pFetchBuffer);
        
        // create another FetchBuffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in new fetchBuffer
        bool bResult = pFetchBuffer->bAppendCrw(aCrw);
        RASSERT(bResult);
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // check if session exiting
      //if (pSession->m_bSessionExiting)
      //  pCrwBuffer = nullptr;
    }
    
    // get next buffer from the current indexBuffer
    pCrwBuffer = pCrwBuffer->pNextBuffer();
    
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  pQueue->vAppend(pFetchBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.subLoaders -- set table sub loaders                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableSubLoaders(void)
{
  ENTRY(); // ENTRY("vTableSubDic"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  i8 iSubLoaders = m_pDataStack->iPopInt(__func__);
  
  pTable->vSubLoaders(iSubLoaders);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.slots -- push the slots of table on data stack                       */
/*----------------------------------------------------------------------------*/
void Step::vTableSlots(void)
{
  ENTRY(); // ENTRY("vTableSlots"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pTable->pSlots());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].slots@ -- fetch the slots of table on data stack                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSlotsFetch(void)
{
  ENTRY(); // ENTRY("vTableSlots"_Stz);
  
  Table* pTable = _pTable(*m_pRegSet->m_xpTable);
  
  m_pDataStack->vPushPtr(pTable->pSlots());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].size -- push space used by table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSize(void)
{
  ENTRY(); // ENTRY("vTableSpace"_Stz);
  
  Table* pTable = _pTable(*m_pRegSet->m_xpTable);
  
  i8 iBuffers = pTable->nCrwBuffers();
  i8 iSize = iBuffers * C_SIZE_POOL_BUFFER;
  m_pDataStack->vPushInt(iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.tableDef@ -- fetch table definition                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableTableDefFetch(void)
{
  ENTRY(); // ENTRY("vTableTableDefFetch"_Stz);

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pTable->pTableDef());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.tableDef! -- store table definition                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableTableDefStore(void)
{
  ENTRY(); // ENTRY("vTableTableDefStore"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  TableDef* pTableDef = _pTableDef(m_pDataStack->pPopPtr(__func__));

  pTable->vTableDef(pTableDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on data stack          */
/*----------------------------------------------------------------------------*/
void Step::vTableTruncate(void)
{
  ENTRY(); // ENTRY("vTableTruncate"_Stz);
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  i8 iSlot = m_pDataStack->iPopAsInt(__func__);
  
  pTable->vTruncate(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].verify -- verify the table                                         */
/*----------------------------------------------------------------------------*/
void Step::vTableVerify(void)
{
  ENTRY(); // ENTRY("vTableVerify"_Stz);
  
  Table* pTable = _pTable(*m_pRegSet->m_xpTable);
  
  bool bVerified = pTable->bVerifyTable();
  
  m_pDataStack->vPushInt(bVerified);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table[].visualize -- visualize the table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableVisualize(void)
{
  ENTRY(); // ENTRY("vTableVisualize"_Stz);
  
  Table* pTable = _pTable(*m_pRegSet->m_xpTable);
  
  for (i8 iPartition = 0; iPartition < AM::pGrid()->nSlotsPerNode(); iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    vTrace("Table Partition %lld\n"_Stz, iPartition);
    pPartition->vVisualizeTable();
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
