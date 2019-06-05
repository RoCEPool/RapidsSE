/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Table(void)
{
  vAddStep("table.addIndex"_Stz,           &Step::vTableAddIndex);
  vAddStep("table.analyze"_Stz,            &Step::vTableAnalyze);
  vAddStep("table.columnCount@"_Stz,       &Step::vTableColumnCountFetch);
  vAddStep("table.column@"_Stz,            &Step::vTableColumnFetch);
  vAddStep("table.createColumnVec"_Stz,    &Step::vTableCreateColumnVec);
  vAddStep("table.drop"_Stz,               &Step::vTableDrop);
  vAddStep("table.dump"_Stz,               &Step::vTableDump);
  vAddStep("table.findColumn"_Stz,         &Step::vTableFindColumn);
  vAddStep("table.genData"_Stz,            &Step::vTableGenData);
  vAddStep("table.getColumnsAsString"_Stz, &Step::vTableGetColumnsAsString);
  vAddStep("table.getPtn"_Stz,             &Step::vTableGetPtn);
  vAddStep("table.indexCount"_Stz,         &Step::vTableIndexCount);
  vAddStep("table.inflateScan"_Stz,        &Step::vTableInflateScan);
  vAddStep("table.init"_Stz,               &Step::vTableInit);
  vAddStep("table.isColumn"_Stz,           &Step::vTableIsColumn);
  vAddStep("table.isLoading!"_Stz,         &Step::vTableIsLoadingStore);
  vAddStep("table.isValidIndexColumns"_Stz,&Step::vTableIsValidIndexColumns);
  vAddStep("table.lazyBuildIndexes"_Stz,   &Step::vTableLazyBuildIndexes);
  vAddStep("table.name"_Stz,               &Step::vTableName);
  vAddStep("table.publish"_Stz,            &Step::vTablePublish);
  vAddStep("table.purge"_Stz,              &Step::vTablePurge);
  vAddStep("table.removeIndex"_Stz,        &Step::vTableRemoveIndex);
  vAddStep("table.scan"_Stz,               &Step::vTableScan);
  vAddStep("table.showColumnVec"_Stz,      &Step::vTableShowColumnVec);
  vAddStep("table.slots"_Stz,              &Step::vTableSlots);
  vAddStep("table.space"_Stz,              &Step::vTableSpace);
  vAddStep("table.subDic"_Stz,             &Step::vTableSubDic);
  vAddStep("table.subLoaders"_Stz,         &Step::vTableSubLoaders);
  vAddStep("table.truncate"_Stz,           &Step::vTableTruncate);
  vAddStep("table.verify"_Stz,             &Step::vTableVerify);
  vAddStep("table.visualize"_Stz,          &Step::vTableVisualize);
}

/*----------------------------------------------------------------------------*/
/* table.addIndex -- add index to the table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableAddIndex(MP_Task mpTask)
{
  ENTRY("vTableAddIndex"_Stz);
  
  Object* pPtrTable;
  Object* pPtrIndex;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.addIndex"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("table.addIndex"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pTable->vAddIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.analyze -- analyze table                                             */
/*----------------------------------------------------------------------------*/
void Step::vTableAnalyze(MP_Task mpTask)
{
  ENTRY("vTableAnalyze"_Stz);
  
  Object* pPtrTable;
  i8 iDetailed;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.analyze"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iDetailed) )
    THROW_INVALID_DSTACK("table.analyze"_Stz);
  
  s48K sAnalysis;
  i8 nRows;
  sAnalysis = (Stz*)pTable->pAnalyzeTable(iDetailed == 1 ? true : false, nRows);
  
  mpTask->m_pDataStack->vPushS48K(sAnalysis);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isValidIndexColumns -- validate index columns from job_createIndex  */
/*----------------------------------------------------------------------------*/
void Step::vTableIsValidIndexColumns(MP_Task mpTask)
{
  ENTRY("vTableIsValidIndexColumns"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK(" table.isValidateIndexColumns"_Stz);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK(" table.isValidateIndexColumns"_Stz);
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pPtrJob);
  ASSERT(CN_JOB_CREATEINDEX == pJob_CreateIndex->eClass());
  
  mpTask->m_pDataStack->vPushI8(pTable->bIsValidIndexColumns(pJob_CreateIndex));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.lazyBuildIndexes -- lazy build of indexes (from lazy chains)         */
/*----------------------------------------------------------------------------*/
void Step::vTableLazyBuildIndexes(MP_Task mpTask)
{
  ENTRY("vTableLazyBuildIndexes"_Stz);
  i8 iSlot;
  i8 iTxId;
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.lazyBuildIndexes"_Stz);
  
  Table* pTable = reinterpret_cast<Table*>(sString.iToI8());
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.lazyBuildIndexes"_Stz);
  
  iSlot = sString.iToI8();
  
  pTable->vLazyBuildIndexes(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isLoading! -- store lable is loading                                 */
/*----------------------------------------------------------------------------*/
void Step::vTableIsLoadingStore(MP_Task mpTask)
{
  ENTRY("vTableIsLoadingStore"_Stz);
  
  Object* pPtrTable;
  i8 iLoading;

  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.analyze"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iLoading) )
    THROW_INVALID_DSTACK("table.isLoading"_Stz);
  
  pTable->vIsLoading(iLoading == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.column@ -- get the column from the table                             */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnFetch(MP_Task mpTask)
{
  ENTRY("vTableColumnFetch"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
  THROW_INVALID_DSTACK("table.coumn@"_Stz);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iColumnNo;
  if ( ! mpTask->m_pDataStack->bPop(iColumnNo) )
  THROW_INVALID_DSTACK("table.columnCount@"_Stz);
  
  ColumnVec* pColVec = pTable->pColumnVec();
  
  mpTask->m_pDataStack->vPushPointer(pColVec->pGet(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.columnCount@ -- get column count                                     */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnCountFetch(MP_Task mpTask)
{
  ENTRY("vTableColumnCountFetch"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
  THROW_INVALID_DSTACK("table.columnCount@"_Stz);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  ColumnVec* pColVec = pTable->pColumnVec();
  i8 nColumns = pColVec->nItems();
  
  mpTask->m_pDataStack->vPushI8(nColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.createColumnVec -- create column vector from job_createTable     */
/*----------------------------------------------------------------------------*/
void Step::vTableCreateColumnVec(MP_Task mpTask)
{
  ENTRY("vTableCreateColumnVec"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Stz);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Stz);
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pPtrJob);
  ASSERT(CN_JOB_CREATETABLE == pJob_CreateTable->eClass());
  
  pTable->vCreateColumnVec(pJob_CreateTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableDrop(MP_Task mpTask)
{
ENTRY("vTableDrop"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.drop"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.dump                                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTableDump(MP_Task mpTask)
{
ENTRY("vTableDump"_Stz);
  
  vTrace("TableDump\n"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.dump"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlotsPerNode; iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    vTrace("Partition %lld\n"_Stz, iPartition);
    pPartition->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.genData -- generate data for table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableGenData(MP_Task mpTask)
{
  ENTRY("vTableGenData"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.getColumnsAsString -- get columns as a csv string                    */
/*----------------------------------------------------------------------------*/
void Step::vTableGetColumnsAsString(MP_Task mpTask)
{
  ENTRY("vTableGetColumnsAsString"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
  THROW_INVALID_DSTACK("table.getColumnsAsString"_Stz);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  ColumnVec* pColVec = pTable->pColumnVec();
  i8 nColumns = pColVec->nItems();
  
  s16KB s;
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    if (i_Column != 0)
    s.vConcat(","_Stz);
    
    if (i_Column == 0)
    s.vCopy(pColVec->pGet(i_Column)->sColumnName());
    else
    s.vConcat(pColVec->pGet(i_Column)->sColumnName());
    
    s.vConcat(" "_Stz);
    s.vConcat(aCT[pColVec->pGet(i_Column)->eColumnType()]);
  }
  
  mpTask->m_pDataStack->vPushS16KB(s);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.getPtn -- get table partition given slot no                          */
/*----------------------------------------------------------------------------*/
void Step::vTableGetPtn(MP_Task mpTask)
{
  ENTRY("vTableGetPtn"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.getPtn"_Stz);
  Table* pTable = _pTable(pPtrTable);
  
  i8 i_Ptn;
  if ( ! mpTask->m_pDataStack->bPopAsInt(i_Ptn) )
    THROW_INVALID_DSTACK("table.getPtn"_Stz);
  
  // get the table partition
  i8 i_Slot = TablePtn::iPartitionToSlot(i_Ptn);
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);

  mpTask->m_pDataStack->vPushPointer(pTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.inflateScan -- scan the table for inflation                          */
/*----------------------------------------------------------------------------*/
void Step::vTableInflateScan(MP_Task mpTask)
{
  ENTRY("vTableInflateScan"_Stz);
  
  Session* pSession = mpTask->m_pSession;
  Object* pPtrTable = nullptr;
  FetchBuffer* pFetchBuffer;
  GenVar xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.inflateScan"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("table.inflateScan"_Stz);
  
  Job_SubScan* pJob_SubScan = _pJob_SubScan(pPtrJob);
  ASSERT(CN_JOB_SUBINFLATE == pJob_SubScan->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  i8 iSlot = pJob_SubScan->m_i_Slot;
  Queue* pQueue = pJob_SubScan->m_pBufferQueue;
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set buffer count in the inflate job
  //Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJob_SubScan->m_pParentJob);
  //pJob_InflateRows->m_inBufferCount += pPartition->nRowBuffers();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first row buffer
  RowBuffer* pRowBuffer = pPartition->pFirstRowBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entryBuffer ?
  if (! pRowBuffer) {
    
    EXIT();
    return;
  }
  
  i8 nBufferCount = 0;
  
  while (pRowBuffer) {
    
    // put this FetchBuffer in session bufferQueue
    pQueue->vAppend(pRowBuffer);
    nBufferCount++;
    
    // get next buffer from the current indexBuffer
    pRowBuffer = pRowBuffer->pNextBuffer();
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.indexCount -- get the no of indexes in the table                     */
/*----------------------------------------------------------------------------*/
void Step::vTableIndexCount(MP_Task mpTask)
{
  ENTRY("vTableIndexCount"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.indexCount@"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass())
  
  mpTask->m_pDataStack->vPushI8(pTable->pIdxVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableInit(MP_Task mpTask)
{
ENTRY("vTableInit"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  new(pTable) Table;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.findColumn -- find the column from the table ?                       */
/*----------------------------------------------------------------------------*/
void Step::vTableFindColumn(MP_Task mpTask)
{
  ENTRY("vTableFindColumn"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.findColumn"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sColumnName;
  if ( ! mpTask->m_pDataStack->bPop(sColumnName) )
    THROW_INVALID_DSTACK("table.findColumn"_Stz);
  
  ColumnVec* pColVec = pTable->pColumnVec();
  Column* pColumn = nullptr;
  bool bFound = false;
  
  // look for the column in the columnVec
  i8 nTableColumns = pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nTableColumns; i_Col++) {
    
    pColumn = pColVec->pGet(i_Col);
    
    if (pColumn->sColumnName().bSame(sColumnName)) {
      bFound = true;
      break;
    }
  }
  RASSERT(bFound);
  
  mpTask->m_pDataStack->vPushPointer(pColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isColumn -- is the column a valid column of the table ?              */
/*----------------------------------------------------------------------------*/
void Step::vTableIsColumn(MP_Task mpTask)
{
  ENTRY("vTableInit"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.isColumn"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sColumnName;
  if ( ! mpTask->m_pDataStack->bPop(sColumnName) )
    THROW_INVALID_DSTACK("table.isColumn"_Stz);
  
  ColumnVec* pColVec = pTable->pColumnVec();
  Column* pColumn;
  bool bFound = false;
  
  // look for the column in the columnVec
  i8 nTableColumns = pColVec->nItems();
  for (i8 i_Col = 0; i_Col < nTableColumns; i_Col++) {
    
    pColumn = pColVec->pGet(i_Col);
    
    if (pColumn->sColumnName().bSame(sColumnName)) {
      bFound = true;
      break;
    }
  }
  
  mpTask->m_pDataStack->vPushI8(bFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.name -- get table name                                               */
/*----------------------------------------------------------------------------*/
void Step::vTableName(MP_Task mpTask)
{
  ENTRY("vTableName"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.name"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sName;
  
  sName = (Stz*)pTable->aTableName();
  
  mpTask->m_pDataStack->vPushS32(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.publish -- purge rows/buffers in table on data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePublish(MP_Task mpTask)
{
  ENTRY("vTablePublish"_Stz);
  
  Object* pPtrTable;
  i8 iSlot;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.publish"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("table.publish"_Stz);
  
  pTable->vPublish(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.purge -- purge rows/buffers in table on data stack                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePurge(MP_Task mpTask)
{
  ENTRY("vTablePurge"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.purge"_Stz);
  Table* pTable = _pTable(pPtrTable);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("table.purge"_Stz);
  
  i8 iOlderThanSeconds;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iOlderThanSeconds) )
  THROW_INVALID_DSTACK("table.purge"_Stz);
  
  bool bPurged = pTable->bPurge(iSlot,iOlderThanSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.removeIndex -- remove an index from the table                        */
/*----------------------------------------------------------------------------*/
void Step::vTableRemoveIndex(MP_Task mpTask)
{
  ENTRY("vTableRef"_Stz);
  
  Object* pPtrTable;
  Object* pPtrIndex;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.removeIndex"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("table.removeIndex"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);

  pTable->vRemoveIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.scan -- scan the table                                               */
/*----------------------------------------------------------------------------*/
void Step::vTableScan(MP_Task mpTask)
{
  ENTRY("vTableScan"_Stz);
  
  Session* pSession = mpTask->m_pSession;
  Object* pPtrTable = nullptr;
  FetchBuffer* pFetchBuffer;
  GenVar xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.scan"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("table.scan"_Stz);
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  i8 iSlot = pJob_SubFetch->m_i_Slot;
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position in the first row buffer
  RowBuffer* pRowBuffer = pPartition->pFirstRowBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entryBuffer ?
  if (! pRowBuffer) {
    // mark this buffer as the last buffer
    pFetchBuffer->vIsLast(true);
    
    // put the buffer in the session bufferQueue
    pQueue->vAppend(pFetchBuffer);
    
    EXIT();
    return;
  }
  
  
  while (pRowBuffer) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // iterate thru the buffer till no more entries
    for (i8 iRow = 0; iRow < pRowBuffer->nRowCount(); iRow++) {
      
      // get the row from the buffer
      HeadLine* aRowData = pRowBuffer->pRow(iRow);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // insert row from entry in FetchBuffer
      if ( ! pFetchBuffer->bAppendRow(aRowData)) {
        
        // Buffer is Full
        // put this FetchBuffer in session bufferQueue
        pQueue->vAppend(pFetchBuffer);
        
        // create another FetchBuffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in new fetchBuffer
        bool bResult = pFetchBuffer->bAppendRow(aRowData);
        RASSERT(bResult);
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // check if session exiting
      if (pSession->m_bSessionExiting)
        pRowBuffer = nullptr;
    }
    
    // get next buffer from the current indexBuffer
    pRowBuffer = pRowBuffer->pNextBuffer();
    
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
/* table.showColumnVec -- show column vector                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableShowColumnVec(MP_Task mpTask)
{
  ENTRY("vTableShowColumnVec"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Stz);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vShowColumnVec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.subDic -- get table sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSubDic(MP_Task mpTask)
{
  ENTRY("vTableSubDic"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.subDic"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dic* pDic = pTable->pSubDic();
  
  mpTask->m_pDataStack->vPushPointer(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.subLoaders -- set table sub loaders                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableSubLoaders(MP_Task mpTask)
{
  ENTRY("vTableSubDic"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.subLoaders"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iSubLoaders;
  if ( ! mpTask->m_pDataStack->bPop(iSubLoaders) )
    THROW_INVALID_DSTACK("table.subLoaders"_Stz);
  
  pTable->vSubLoaders(iSubLoaders);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.slots -- push the slots of table on data stack                       */
/*----------------------------------------------------------------------------*/
void Step::vTableSlots(MP_Task mpTask)
{
  ENTRY("vTableSlots"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.slots"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  mpTask->m_pDataStack->vPushPointer(pTable->pSlots());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.space -- push space used by table                                    */
/*----------------------------------------------------------------------------*/
void Step::vTableSpace(MP_Task mpTask)
{
  ENTRY("vTableSpace"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.space"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iBuffers = pTable->nRowBuffers();
  i8 iSpace = iBuffers * C_SIZE_POOL_BUFFER;
  mpTask->m_pDataStack->vPushI8(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on data stack          */
/*----------------------------------------------------------------------------*/
void Step::vTableTruncate(MP_Task mpTask)
{
  ENTRY("vTableTruncate"_Stz);
  
  Object* pPtrTable;
  i8 iSlot;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.truncate"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("table.truncate"_Stz);
  
  pTable->vTruncate(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.verify -- verify the table                                           */
/*----------------------------------------------------------------------------*/
void Step::vTableVerify(MP_Task mpTask)
{
  ENTRY("vTableVerify"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.verify"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  bool bVerified = pTable->bVerifyTable();
  s256 s = bVerified ? "Valid"_Stz : "Invalid"_Stz;
  
  mpTask->m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.visualize -- visualize the table                                     */
/*----------------------------------------------------------------------------*/
void Step::vTableVisualize(MP_Task mpTask)
{
  ENTRY("vTableVisualize"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.visualize"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlotsPerNode; iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    vTrace("Table Partition %lld\n"_Stz, iPartition);
    pPartition->vVisualizeTable();
  }
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
