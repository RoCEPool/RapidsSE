/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Table(void)
{
  vAddStep("table.addIndex"_Stz,           &Step::vTableAddIndex);
  vAddStep("table.analyze"_Stz,            &Step::vTableAnalyze);
  vAddStep("table.columnCount@"_Stz,       &Step::vTableColumnCountFetch);
  vAddStep("table.column@"_Stz,            &Step::vTableColumnFetch);
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
  vAddStep("table.tableDef@"_Stz,          &Step::vTableTableDefFetch);
  vAddStep("table.tableDef!"_Stz,          &Step::vTableTableDefStore);
  vAddStep("table.truncate"_Stz,           &Step::vTableTruncate);
  vAddStep("table.verify"_Stz,             &Step::vTableVerify);
  vAddStep("table.visualize"_Stz,          &Step::vTableVisualize);
}

/*----------------------------------------------------------------------------*/
/* table.addIndex -- add index to the table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableAddIndex(void)
{
  ENTRY("vTableAddIndex"_Stz);
  
  Object* pPtrTable;
  Object* pPtrIndex;
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pTable->vAddIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.analyze -- analyze table                                             */
/*----------------------------------------------------------------------------*/
void Step::vTableAnalyze(void)
{
  ENTRY("vTableAnalyze"_Stz);
  
  Object* pPtrTable;
  i8 iDetailed;
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! m_pDataStack->bPopI8(iDetailed) )
    THROW_DSTACK();
  
  s48K sAnalysis;
  i8 nRows;
  sAnalysis = (Stz*)pTable->pAnalyzeTable(iDetailed == 1 ? true : false, nRows);
  
  m_pDataStack->vPushS48K(sAnalysis);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isValidIndexColumns -- validate index columns from job_createIndex  */
/*----------------------------------------------------------------------------*/
void Step::vTableIsValidIndexColumns(void)
{
  ENTRY("vTableIsValidIndexColumns"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrJob;
  if ( ! m_pDataStack->bPopObject(pPtrJob) )
    THROW_DSTACK();
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pPtrJob);
  ASSERT(CN_JOB_CREATEINDEX == pJob_CreateIndex->eClass());
  
  m_pDataStack->vPushI8(pTable->bIsValidIndexColumns(pJob_CreateIndex));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.lazyBuildIndexes -- lazy build of indexes (from lazy chains)         */
/*----------------------------------------------------------------------------*/
void Step::vTableLazyBuildIndexes(void)
{
  ENTRY("vTableLazyBuildIndexes"_Stz);
  i8 iSlot;
  i8 iTxId;
  s48K sString;
  
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();
  
  Table* pTable = reinterpret_cast<Table*>(sString.iToI8());
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! m_pDataStack->bPopS48K(sString) )
    THROW_DSTACK();
  
  iSlot = sString.iToI8();
  
  pTable->vLazyBuildIndexes(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isLoading! -- store lable is loading                                 */
/*----------------------------------------------------------------------------*/
void Step::vTableIsLoadingStore(void)
{
  ENTRY("vTableIsLoadingStore"_Stz);
  
  Object* pPtrTable;
  i8 iLoading;

  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! m_pDataStack->bPopI8(iLoading) )
    THROW_DSTACK();
  
  pTable->vIsLoading(iLoading == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.column@ -- get the column from the table                             */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnFetch(void)
{
  ENTRY("vTableColumnFetch"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
  THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iColumnNo;
  if ( ! m_pDataStack->bPopI8(iColumnNo) )
  THROW_DSTACK();
  
  ColumnVec* pColVec = pTable->pColumnVec();
  
  m_pDataStack->vPushPointer(pColVec->pGet(iColumnNo));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.columnCount@ -- get column count                                     */
/*----------------------------------------------------------------------------*/
void Step::vTableColumnCountFetch(void)
{
  ENTRY("vTableColumnCountFetch"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
  THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  ColumnVec* pColVec = pTable->pColumnVec();
  i8 nColumns = pColVec->nItems();
  
  m_pDataStack->vPushI8(nColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableDrop(void)
{
ENTRY("vTableDrop"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.dump                                                                 */
/*----------------------------------------------------------------------------*/
void Step::vTableDump(void)
{
ENTRY("vTableDump"_Stz);
  
  vTrace("TableDump\n"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  for (i8 iPartition = 0; iPartition < AM::pGrid()->nSlotsPerNode(); iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    vTrace("Partition %lld\n"_Stz, iPartition);
    pPartition->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.genData -- generate data for table                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableGenData(void)
{
  ENTRY("vTableGenData"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.getColumnsAsString -- get columns as a csv string                    */
/*----------------------------------------------------------------------------*/
void Step::vTableGetColumnsAsString(void)
{
  ENTRY("vTableGetColumnsAsString"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
  THROW_DSTACK();
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
  
  m_pDataStack->vPushS16KB(s);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.getPtn -- get table partition given slot no                          */
/*----------------------------------------------------------------------------*/
void Step::vTableGetPtn(void)
{
  ENTRY("vTableGetPtn"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  
  i8 i_Ptn;
  if ( ! m_pDataStack->bPopAsInt(i_Ptn) )
    THROW_DSTACK();
  
  // get the table partition
  i8 i_Slot = TablePtn::iPartitionToSlot(i_Ptn);
  TablePtn* pTablePtn = pTable->pPartition(i_Slot);

  m_pDataStack->vPushPointer(pTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.inflateScan -- scan the table for inflation                          */
/*----------------------------------------------------------------------------*/
void Step::vTableInflateScan(void)
{
  ENTRY("vTableInflateScan"_Stz);
  
  Object* pPtrTable = nullptr;
  FetchBuffer* pFetchBuffer;
  GenVar xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrJob;
  if ( ! m_pDataStack->bPopObject(pPtrJob) )
    THROW_DSTACK();
  
  Job_SubScan* pJob_SubScan = _pJob_SubScan(pPtrJob);
  ASSERT(CN_JOB_SUBSCAN == pJob_SubScan->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  i8 iSlot = pJob_SubScan->i_Slot();
  Queue* pQueue = pJob_SubScan->m_pBufferQueue;
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set buffer count in the inflate job
  //Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJob_SubScan->m_pParentJob);
  //pJob_InflateRows->m_inBufferCount += pPartition->nCrwBuffers();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first row buffer
 CrwBuffer* pCrwBuffer = pPartition->pRootCrwBuffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entryBuffer ?
  if (! pCrwBuffer) {
    
    EXIT();
    return;
  }
  
  i8 nBufferCount = 0;
  
  while (pCrwBuffer) {
    
    // put this FetchBuffer in session bufferQueue
    pQueue->vAppend(pCrwBuffer);
    nBufferCount++;
    
    // get next buffer from the current indexBuffer
    pCrwBuffer = pCrwBuffer->pNextBuffer();
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.indexCount -- get the no of indexes in the table                     */
/*----------------------------------------------------------------------------*/
void Step::vTableIndexCount(void)
{
  ENTRY("vTableIndexCount"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass())
  
  m_pDataStack->vPushI8(pTable->pIdxVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableInit(void)
{
ENTRY("vTableInit"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  new(pTable) Table;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.findColumn -- find the column from the table ?                       */
/*----------------------------------------------------------------------------*/
void Step::vTableFindColumn(void)
{
  ENTRY("vTableFindColumn"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sColumnName;
  if ( ! m_pDataStack->bPopS32(sColumnName) )
    THROW_DSTACK();
  
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
  
  m_pDataStack->vPushPointer(pColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isColumn -- is the column a valid column of the table ?              */
/*----------------------------------------------------------------------------*/
void Step::vTableIsColumn(void)
{
  ENTRY("vTableInit"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sColumnName;
  if ( ! m_pDataStack->bPopS32(sColumnName) )
    THROW_DSTACK();
  
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
  
  m_pDataStack->vPushI8(bFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.name -- get table name                                               */
/*----------------------------------------------------------------------------*/
void Step::vTableName(void)
{
  ENTRY("vTableName"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sName;
  
  sName = (Stz*)pTable->szTableNm();
  
  m_pDataStack->vPushS32(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.publish -- purge rows/buffers in table on data stack                 */
/*----------------------------------------------------------------------------*/
void Step::vTablePublish(void)
{
  ENTRY("vTablePublish"_Stz);
  
  Object* pPtrTable;
  i8 iSlot;
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! m_pDataStack->bPopAsInt(iSlot) )
    THROW_DSTACK();
  
  pTable->vPublish(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.purge -- purge rows/buffers in table on data stack                   */
/*----------------------------------------------------------------------------*/
void Step::vTablePurge(void)
{
  ENTRY("vTablePurge"_Stz);
  
  Object* pPtrTable;
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  
  i8 iSlot;
  if ( ! m_pDataStack->bPopAsInt(iSlot) )
    THROW_DSTACK();
  
  i8 iOlderThanSeconds;
  if ( ! m_pDataStack->bPopAsInt(iOlderThanSeconds) )
  THROW_DSTACK();
  
  bool bPurged = pTable->bPurge(iSlot,iOlderThanSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.removeIndex -- remove an index from the table                        */
/*----------------------------------------------------------------------------*/
void Step::vTableRemoveIndex(void)
{
  ENTRY("vTableRef"_Stz);
  
  Object* pPtrTable;
  Object* pPtrIndex;
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);

  pTable->vRemoveIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.scan -- scan the table                                               */
/*----------------------------------------------------------------------------*/
void Step::vTableScan(void)
{
  ENTRY("vTableScan"_Stz);
  
  Object* pPtrTable = nullptr;
  FetchBuffer* pFetchBuffer;
  GenVar xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrJob;
  if ( ! m_pDataStack->bPopObject(pPtrJob) )
    THROW_DSTACK();
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
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
      Crw* aCrw = pCrwBuffer->pCrw(iRow);
      
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
/* table.showColumnVec -- show column vector                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableShowColumnVec(void)
{
  ENTRY("vTableShowColumnVec"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vShowColumnVec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.subDic -- get table sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Step::vTableSubDic(void)
{
  ENTRY("vTableSubDic"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dic* pDic = pTable->pSubDic();
  
  m_pDataStack->vPushPointer(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.subLoaders -- set table sub loaders                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableSubLoaders(void)
{
  ENTRY("vTableSubDic"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iSubLoaders;
  if ( ! m_pDataStack->bPopI8(iSubLoaders) )
    THROW_DSTACK();
  
  pTable->vSubLoaders(iSubLoaders);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.slots -- push the slots of table on data stack                       */
/*----------------------------------------------------------------------------*/
void Step::vTableSlots(void)
{
  ENTRY("vTableSlots"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  m_pDataStack->vPushPointer(pTable->pSlots());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.space -- push space used by table                                    */
/*----------------------------------------------------------------------------*/
void Step::vTableSpace(void)
{
  ENTRY("vTableSpace"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iBuffers = pTable->nCrwBuffers();
  i8 iSpace = iBuffers * C_SIZE_POOL_BUFFER;
  m_pDataStack->vPushI8(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.tableDef@ -- fetch table definition                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableTableDefFetch(void)
{
  ENTRY("vTableTableDefFetch"_Stz);

  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  
  m_pDataStack->vPushPointer(pTable->pTableDef());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.tableDef! -- store table definition                                  */
/*----------------------------------------------------------------------------*/
void Step::vTableTableDefStore(void)
{
  ENTRY("vTableTableDefStore"_Stz);
  
  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);
  
  Object* pPtrTableDef;
  if ( ! m_pDataStack->bPopObject(pPtrTableDef) )
    THROW_DSTACK();
  TableDef* pTableDef = _pTableDef(pPtrTableDef);

  pTable->vTableDef(pTableDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on data stack          */
/*----------------------------------------------------------------------------*/
void Step::vTableTruncate(void)
{
  ENTRY("vTableTruncate"_Stz);
  
  Object* pPtrTable;
  i8 iSlot;
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! m_pDataStack->bPopAsInt(iSlot) )
    THROW_DSTACK();
  
  pTable->vTruncate(iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.verify -- verify the table                                           */
/*----------------------------------------------------------------------------*/
void Step::vTableVerify(void)
{
  ENTRY("vTableVerify"_Stz);
  
  Object* pPtrTable;
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  bool bVerified = pTable->bVerifyTable();
  s256 s = bVerified ? "Valid"_Stz : "Invalid"_Stz;
  
  m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.visualize -- visualize the table                                     */
/*----------------------------------------------------------------------------*/
void Step::vTableVisualize(void)
{
  ENTRY("vTableVisualize"_Stz);
  
  Object* pPtrTable;
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
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
