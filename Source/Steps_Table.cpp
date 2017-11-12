/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// table: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Table(void)
{
  vAddPrimitive("table.addIndex"_Stz,           &vTableAddIndex,         false);
  vAddPrimitive("table.analyze"_Stz,            &vTableAnalyze,          false);
  vAddPrimitive("table.appendBuffer"_Stz,       &vTableAppendBuffer,     false);
  vAddPrimitive("table.createColumnVec"_Stz,    &vTableCreateColumnVec,  false);
  vAddPrimitive("table.drop"_Stz,               &vTableDrop,             false);
  vAddPrimitive("table.dump"_Stz,               &vTableDump,             false);
  vAddPrimitive("table.getColumns"_Stz,         &vTableGetColumns,       false);
  vAddPrimitive("table.hiddenIndex@"_Stz,       &vTableHiddenIndexFetch, false);
  vAddPrimitive("table.hiddenIndex!"_Stz,       &vTableHiddenIndexStore, false);
  vAddPrimitive("table.indexCount"_Stz,         &vTableIndexCount,       false);
  vAddPrimitive("table.indexesLoaded@"_Stz,     &vTableIndexesLoadedFetch, false);
  vAddPrimitive("table.indexesLoaded!"_Stz,     &vTableIndexesLoadedStore, false);
  vAddPrimitive("table.init"_Stz,               &vTableInit,             false);
  vAddPrimitive("table.insert"_Stz,             &vTableInsert,           false);
  vAddPrimitive("table.insertEntries"_Stz,      &vTableInsertEntries,    false);
  vAddPrimitive("table.name"_Stz,               &vTableName,             false);
  vAddPrimitive("table.ref"_Stz,                &vTableRef,              false);
  vAddPrimitive("table.removeIndex"_Stz,        &vTableRemoveIndex,      false);
  vAddPrimitive("table.showColumnVec"_Stz,      &vTableShowColumnVec,    false);
  vAddPrimitive("table.slots"_Stz,              &vTableSlots,            false);
  vAddPrimitive("table.subDic"_Stz,             &vTableSubDic,           false);
  vAddPrimitive("table.subLoaders"_Stz,         &vTableSubLoaders,       false);
  vAddPrimitive("table.truncate"_Stz,           &vTableTruncate,         false);
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
  sAnalysis = (Stz*)pTable->pAnalyzeIndex(pTable->pHiddenIndex(),
                                          iDetailed == 1 ? true : false);
  mpTask->m_pDataStack->vPushS48K(sAnalysis);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.insertEntries -- insert entries from buffer into table (from seitf)  */
/*----------------------------------------------------------------------------*/
void Task::vTableInsertEntries(MP_Task mpTask)
{
  ENTRY("vTableInsertEntries"_Stz);
  i8 iSlot;
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.insertEntries"_Stz);
  
  Table* pTable = reinterpret_cast<Table*>(sString.iToI8());
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.insertEntries"_Stz);
  
  EntryBuffer* pRowBuffer = reinterpret_cast<EntryBuffer*>(sString.iToI8());
  ASSERT(CN_ENTRYBUFFER == pRowBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.insertEntries"_Stz);
  
  iSlot = sString.iToI8();
  pTable->vInsertEntries(iSlot,pRowBuffer);
  
  // now delete the temporary buffer
  delete pRowBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.appendBuffer -- append row buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vTableAppendBuffer(MP_Task mpTask)
{
  ENTRY("vTableAppendBuffer"_Stz);

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
  ASSERT(CN_ENTRYBUFFER == pRowBuffer->eClass());
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  
  bool bIndexesLoaded = pTable->bAppendBuffer(iSlot,pRowBuffer);
  
  mpTask->m_pDataStack->vPushInt(bIndexesLoaded ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.createColumnVec -- create column vector from sub dictionary          */
/*----------------------------------------------------------------------------*/
void Task::vTableCreateColumnVec(MP_Task mpTask)
{
  ENTRY("vTableCreateColumnVec"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Stz);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sColumnNm;
  if ( ! mpTask->m_pDataStack->bPop(sColumnNm) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Stz);

  s48K sColumnDef;
  if ( ! mpTask->m_pDataStack->bPop(sColumnDef) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Stz);
  
  pTable->vCreateColumnVec(sColumnNm,sColumnDef);
  
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
/* table.getColumns -- get columns as a csv string                            */
/*----------------------------------------------------------------------------*/
void Task::vTableGetColumns(MP_Task mpTask)
{
  ENTRY("vTableGetColumns"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.getColumns"_Stz);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  ColumnVec* pColVec = pTable->pColumnVec();
  i8 nColumns = pColVec->nItems();
  
  s4K s;
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
  
  mpTask->m_pDataStack->vPushS4K(s);
  
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
/* table.indexesLoaded@ -- fetch the indexesLoaded from the table             */
/*----------------------------------------------------------------------------*/
void Task::vTableIndexesLoadedFetch(MP_Task mpTask)
{
  ENTRY("vTableIndexesLoadedFetch"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.indexesLoaded@"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass())
  
  mpTask->m_pDataStack->vPushInt(pTable->iIndexesLoaded());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.indexesLoaded! -- store the indexesLoaded in the table               */
/*----------------------------------------------------------------------------*/
void Task::vTableIndexesLoadedStore(MP_Task mpTask)
{
  ENTRY("vTableIndexesLoadedStore"_Stz);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.hiddenIndex!"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iIndexesLoaded;
  if ( ! mpTask->m_pDataStack->bPop(iIndexesLoaded) )
    THROW_INVALID_DSTACK("table.hiddenIndex!"_Stz);
  
  pTable->vIndexesLoaded(iIndexesLoaded);
  
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
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.truncate"_Stz);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vTruncate();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
