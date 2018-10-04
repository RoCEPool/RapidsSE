/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Table(void)
{
  vAddStep("table.addIndex"_Stz,           &Task::vTableAddIndex);
  vAddStep("table.analyze"_Stz,            &Task::vTableAnalyze);
  vAddStep("table.appendBuffer"_Stz,       &Task::vTableAppendBuffer);
  vAddStep("table.columnCount@"_Stz,       &Task::vTableColumnCountFetch);
  vAddStep("table.column@"_Stz,            &Task::vTableColumnFetch);
  vAddStep("table.createColumnVec"_Stz,    &Task::vTableCreateColumnVec);
  vAddStep("table.drop"_Stz,               &Task::vTableDrop);
  vAddStep("table.dump"_Stz,               &Task::vTableDump);
  vAddStep("table.getColumnsAsString"_Stz, &Task::vTableGetColumnsAsString);
  vAddStep("table.getPtn"_Stz,             &Task::vTableGetPtn);
  vAddStep("table.getRef"_Stz,             &Task::vTableGetRef);
  vAddStep("table.hiddenIndex@"_Stz,       &Task::vTableHiddenIndexFetch);
  vAddStep("table.hiddenIndex!"_Stz,       &Task::vTableHiddenIndexStore);
  vAddStep("table.indexCount"_Stz,         &Task::vTableIndexCount);
  vAddStep("table.init"_Stz,               &Task::vTableInit);
  vAddStep("table.insert"_Stz,             &Task::vTableInsert);
  vAddStep("table.insertEntries"_Stz,      &Task::vTableInsertEntries);
  vAddStep("table.isValidIndexColumns"_Stz,&Task::vTableIsValidIndexColumns);
  vAddStep("table.lazyBuildIndexes"_Stz,   &Task::vTableLazyBuildIndexes);
  vAddStep("table.isLoading!"_Stz,         &Task::vTableIsLoadingStore);
  vAddStep("table.name"_Stz,               &Task::vTableName);
  vAddStep("table.ref"_Stz,                &Task::vTableRef);
  vAddStep("table.removeIndex"_Stz,        &Task::vTableRemoveIndex);
  vAddStep("table.retRef"_Stz,             &Task::vTableRetRef);
  vAddStep("table.showColumnVec"_Stz,      &Task::vTableShowColumnVec);
  vAddStep("table.slots"_Stz,              &Task::vTableSlots);
  vAddStep("table.subDic"_Stz,             &Task::vTableSubDic);
  vAddStep("table.subLoaders"_Stz,         &Task::vTableSubLoaders);
  vAddStep("table.truncate"_Stz,           &Task::vTableTruncate);
}

/*----------------------------------------------------------------------------*/
/* table.addIndex -- add index to the table                                   */
/*----------------------------------------------------------------------------*/
void Task::vTableAddIndex(MP_Task mpTask)
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
void Task::vTableAnalyze(MP_Task mpTask)
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
/* table.insertEntries -- insert entries from buffer into table (from session)*/
/*----------------------------------------------------------------------------*/
void Task::vTableInsertEntries(MP_Task mpTask)
{
  ENTRY("vTableInsertEntries"_Stz);
  i8 iSlot;
  i8 iTxId;
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.insertEntries"_Stz);
  
  Table* pTable = reinterpret_cast<Table*>(sString.iToI8());
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.insertEntries"_Stz);
  
  WorkingBuffer* pWorkingBuffer = reinterpret_cast<WorkingBuffer*>(sString.iToI8());
  ASSERT(CN_WORKINGBUFFER == pWorkingBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.insertEntries"_Stz);
  
  iSlot = sString.iToI8();
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.insertEntries"_Stz);
  
  iTxId = sString.iToI8();
  
  pTable->vInsertEntries(iTxId, iSlot, pWorkingBuffer);
  
  // now delete the temporary buffer
  delete pWorkingBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.isValidIndexColumns -- validate index columns from job_createIndex  */
/*----------------------------------------------------------------------------*/
void Task::vTableIsValidIndexColumns(MP_Task mpTask)
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
void Task::vTableLazyBuildIndexes(MP_Task mpTask)
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
void Task::vTableIsLoadingStore(MP_Task mpTask)
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
/* table.appendBuffer -- append row buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vTableAppendBuffer(MP_Task mpTask)
{
  ENTRY("vTableAppendBuffer"_Stz);

  // no longer invoked
  RASSERT(0);
  /*
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition = 0;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Stz);
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Stz);
  
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  
  
  
  bool bIndexesLoaded = pTable->bAppendBuffer(iSlot,pRowBuffer);
  
  mpTask->m_pDataStack->vPushInt(bIndexesLoaded ? 1 : 0);
  */
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.coumn@ -- get the column from the table                              */
/*----------------------------------------------------------------------------*/
void Task::vTableColumnFetch(MP_Task mpTask)
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
void Task::vTableColumnCountFetch(MP_Task mpTask)
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
void Task::vTableCreateColumnVec(MP_Task mpTask)
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
void Task::vTableDrop(MP_Task mpTask)
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
void Task::vTableDump(MP_Task mpTask)
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
/* table.getColumnsAsString -- get columns as a csv string                    */
/*----------------------------------------------------------------------------*/
void Task::vTableGetColumnsAsString(MP_Task mpTask)
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
void Task::vTableGetPtn(MP_Task mpTask)
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
/* table.getRef -- get table reference                                        */
/*----------------------------------------------------------------------------*/
void Task::vTableGetRef(MP_Task mpTask)
{
  ENTRY("vTableGetRef"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.getRef"_Stz);
  Table* pTable = _pTable(pPtrTable);
  
  pTable->vRefInc();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.hiddenIndex@ -- fetch the hidden index from the table                */
/*----------------------------------------------------------------------------*/
void Task::vTableHiddenIndexFetch(MP_Task mpTask)
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
void Task::vTableHiddenIndexStore(MP_Task mpTask)
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
void Task::vTableIndexCount(MP_Task mpTask)
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
void Task::vTableInit(MP_Task mpTask)
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
void Task::vTableInsert(MP_Task mpTask)
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
                     _pStrVal(&xRow)->aStr(),
                     _pStrVal(&xRow)->nAreaSize());
   */
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.name -- get table name                                               */
/*----------------------------------------------------------------------------*/
void Task::vTableName(MP_Task mpTask)
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
/* table.ref -- has table got references?                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableRef(MP_Task mpTask)
{
  ENTRY("vTableRef"_Stz);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.ref"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  
  bool bRef = pTable->bRefOut();
  
  mpTask->m_pDataStack->vPushInt(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.removeIndex -- remove an index from the table                        */
/*----------------------------------------------------------------------------*/
void Task::vTableRemoveIndex(MP_Task mpTask)
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
/* table.retRef -- return table reference                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableRetRef(MP_Task mpTask)
{
  ENTRY("vTableRetRef"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.retRef"_Stz);
  Table* pTable = _pTable(pPtrTable);
  
  pTable->vRefDec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.showColumnVec -- show column vector                                  */
/*----------------------------------------------------------------------------*/
void Task::vTableShowColumnVec(MP_Task mpTask)
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
void Task::vTableSubDic(MP_Task mpTask)
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
void Task::vTableSubLoaders(MP_Task mpTask)
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
void Task::vTableSlots(MP_Task mpTask)
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
void Task::vTableTruncate(MP_Task mpTask)
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
