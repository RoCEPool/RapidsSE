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
  vAddStep("table.genData"_Stz,            &Step::vTableGenData);
  vAddStep("table.getColumnsAsString"_Stz, &Step::vTableGetColumnsAsString);
  vAddStep("table.getPtn"_Stz,             &Step::vTableGetPtn);
  vAddStep("table.hiddenIndex@"_Stz,       &Step::vTableHiddenIndexFetch);
  vAddStep("table.hiddenIndex!"_Stz,       &Step::vTableHiddenIndexStore);
  vAddStep("table.indexCount"_Stz,         &Step::vTableIndexCount);
  vAddStep("table.init"_Stz,               &Step::vTableInit);
  vAddStep("table.insert"_Stz,             &Step::vTableInsert);
  vAddStep("table.isLoading!"_Stz,         &Step::vTableIsLoadingStore);
  vAddStep("table.isValidIndexColumns"_Stz,&Step::vTableIsValidIndexColumns);
  vAddStep("table.lazyBuildIndexes"_Stz,   &Step::vTableLazyBuildIndexes);
  vAddStep("table.name"_Stz,               &Step::vTableName);
  vAddStep("table.publish"_Stz,            &Step::vTablePublish);
  vAddStep("table.purge"_Stz,              &Step::vTablePurge);
  vAddStep("table.removeIndex"_Stz,        &Step::vTableRemoveIndex);
  vAddStep("table.showColumnVec"_Stz,      &Step::vTableShowColumnVec);
  vAddStep("table.slots"_Stz,              &Step::vTableSlots);
  vAddStep("table.subDic"_Stz,             &Step::vTableSubDic);
  vAddStep("table.subLoaders"_Stz,         &Step::vTableSubLoaders);
  vAddStep("table.truncate"_Stz,           &Step::vTableTruncate);
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
  sAnalysis = (Stz*)pTable->pAnalyzeIndex(pTable->pHiddenIndex(),
                                          iDetailed == 1 ? true : false, nRows);

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
  
  mpTask->m_pDataStack->vPushInt(pTable->bIsValidIndexColumns(pJob_CreateIndex));
  
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
/* table.coumn@ -- get the column from the table                              */
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
  
  mpTask->m_pDataStack->vPushObject(pColVec->pGet(iColumnNo));
  
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
  
  mpTask->m_pDataStack->vPushInt(nColumns);
  
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
/* table dump                                                                 */
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
  
  pTable->vContentDump();
  
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
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    if (i_Column != 0)
    s.vConcat(","_Stz);
    
    if (i_Column == 0)
    s.vCopy(pColVec->pGet(i_Column)->sColumnName());
    else
    s.vConcat(pColVec->pGet(i_Column)->sColumnName());
    
    s.vConcat(" "_Stz);
    s.vConcat(DBobjects::aCT[pColVec->pGet(i_Column)->eColumnType()]);
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

  mpTask->m_pDataStack->vPushObject(pTablePtn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.hiddenIndex@ -- fetch the hidden index from the table                */
/*----------------------------------------------------------------------------*/
void Step::vTableHiddenIndexFetch(MP_Task mpTask)
{
  ENTRY("vTableHiddenIndexFetch"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.hiddenIndex@"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Index* pHiddenIndex = pTable->pHiddenIndex();
  
  mpTask->m_pDataStack->vPushObject(pHiddenIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.hiddenIndex! -- store the hidden index in the table                  */
/*----------------------------------------------------------------------------*/
void Step::vTableHiddenIndexStore(MP_Task mpTask)
{
  ENTRY("vTableHiddenIndexStore"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.hiddenIndex!"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("table.hiddenIndex!"_Stz);
  
  Index* pHiddenIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pHiddenIndex->eClass());
  
  pTable->vHiddenIndex(pHiddenIndex);
  
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
  
  mpTask->m_pDataStack->vPushInt(pTable->pIdxVec()->nItems());
  
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
/* table.insert -- insert row into table on data stack                        */
/*----------------------------------------------------------------------------*/
void Step::vTableInsert(MP_Task mpTask)
{
ENTRY("vTableInsert"_Stz);
 
  Object* pPtrTable;
  Object* pPtrTx;
  ValWrk xRow;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.insert"_Stz);
  if ( ! mpTask->m_pDataStack->bPop(pPtrTx) )
    THROW_INVALID_DSTACK("table.insert"_Stz);
  if ( ! mpTask->m_pDataStack->bPop(xRow) )
    THROW_INVALID_DSTACK("table.insert"_Stz);

  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("table.insert"_Stz);
  
  Table* pTable = _pTable(pPtrTable);

  Tx* pTx = _pTx(pPtrTx);
  
  // invoked by copy...
  NEVER_REACH;
  
  /*
  pTable->vInsertRow(iPartition,
                     pTx,
                     _pStrVal(&xRow)->aStz(),
                     _pStrVal(&xRow)->nAreaSize());
   */
  
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
    THROW_INVALID_DSTACK("table.init"_Stz);
  
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
  
  mpTask->m_pDataStack->vPushObject(pDic);
  
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
  
  mpTask->m_pDataStack->vPushObject(pTable->pSlots());
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
