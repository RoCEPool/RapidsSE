/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// table: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Table(void)
{
  vAddPrimitive("table.addIndex"_Str,           &vTableAddIndex,         false);
  vAddPrimitive("table.analyze"_Str,            &vTableAnalyze,          false);
  vAddPrimitive("table.append"_Str,             &vTableAppend,           false);
  vAddPrimitive("table.appendBuffer"_Str,       &vTableAppendBuffer,     false);
  vAddPrimitive("table.createColumnVec"_Str,    &vTableCreateColumnVec,  false);
  vAddPrimitive("table.drop"_Str,               &vTableDrop,             false);
  vAddPrimitive("table.dump"_Str,               &vTableDump,             false);
  vAddPrimitive("table.hiddenIndex@"_Str,       &vTableHiddenIndexFetch, false);
  vAddPrimitive("table.hiddenIndex!"_Str,       &vTableHiddenIndexStore, false);
  vAddPrimitive("table.init"_Str,               &vTableInit,             false);
  vAddPrimitive("table.insert"_Str,             &vTableInsert,           false);
  vAddPrimitive("table.name"_Str,               &vTableName,             false);
  vAddPrimitive("table.ref"_Str,                &vTableRef,              false);
  vAddPrimitive("table.showColumnVec"_Str,      &vTableShowColumnVec,    false);
  vAddPrimitive("table.slots"_Str,              &vTableSlots,            false);
  vAddPrimitive("table.space"_Str,              &vTableSpace,            false);
  vAddPrimitive("table.subDic"_Str,             &vTableSubDic,           false);
  vAddPrimitive("table.truncate"_Str,           &vTableTruncate,         false);
}

/*----------------------------------------------------------------------------*/
/* table.addIndex -- add index to the table                                   */
/*----------------------------------------------------------------------------*/
void Task::vTableAddIndex(MP_Task mpTask)
{
  ENTRY("vTableAddIndex"_Str);
  
  Object* pPtrTable;
  Object* pPtrIndex;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.addIndex"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("table.addIndex"_Str);
  
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
  ENTRY("vTableAnalyze"_Str);
  
  Object* pPtrTable;
  i8 iDetailed;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.analyze"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iDetailed) )
    THROW_INVALID_DSTACK("table.analyze"_Str);
  
  s48K sAnalysis;
  sAnalysis = (Str*)pTable->pAnalyze(iDetailed == 1 ? true : false);
  mpTask->m_pDataStack->vPushS48K(sAnalysis);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.append -- append buffer into table (from seitf)                      */
/*----------------------------------------------------------------------------*/
void Task::vTableAppend(MP_Task mpTask)
{
  ENTRY("vTableAppend"_Str);
  i8 iPartition;
  s48K sString;
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append"_Str);
  
  Table* pTable = reinterpret_cast<Table*>(sString.iToI8());
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append"_Str);
  
  EntryBuffer* pRowBuffer = reinterpret_cast<EntryBuffer*>(sString.iToI8());
  ASSERT(CN_ENTRYBUFFER == pRowBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append"_Str);
  
  iPartition = sString.iToI8();
  pTable->vAppendBuffer(iPartition,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.appendBuffer -- append row buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vTableAppendBuffer(MP_Task mpTask)
{
  ENTRY("vTableAppendBuffer"_Str);

  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Str);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition = 0;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Str);
  
  Object* pPtrRowBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Str);
  EntryBuffer* pRowBuffer = (EntryBuffer*)_pEntryBuffer(pPtrRowBuffer);
  ASSERT(CN_ENTRYBUFFER == pRowBuffer->eClass());
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  
  pTable->vAppendBuffer(iSlot,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.createColumnVec -- create column vector from sub dictionary          */
/*----------------------------------------------------------------------------*/
void Task::vTableCreateColumnVec(MP_Task mpTask)
{
  ENTRY("vTableCreateColumnVec"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Str);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sColumnNm;
  if ( ! mpTask->m_pDataStack->bPop(sColumnNm) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Str);

  s48K sColumnDef;
  if ( ! mpTask->m_pDataStack->bPop(sColumnDef) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Str);
  
  pTable->vCreateColumnVec(sColumnNm,sColumnDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vTableDrop(MP_Task mpTask)
{
ENTRY("vTableDrop"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.drop"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table dump                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDump(MP_Task mpTask)
{
ENTRY("vTableDump"_Str);
  
  vTrace("TableDump\n"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.dump"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.hiddenIndex@ -- fetch the hidden index from the table                */
/*----------------------------------------------------------------------------*/
void Task::vTableHiddenIndexFetch(MP_Task mpTask)
{
  ENTRY("vTableHiddenIndexFetch"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.hiddenIndex@"_Str);
  
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
  ENTRY("vTableHiddenIndexStore"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.hiddenIndex!"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("table.hiddenIndex!"_Str);
  
  Index* pHiddenIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pHiddenIndex->eClass());
  
  pTable->vHiddenIndex(pHiddenIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vTableInit(MP_Task mpTask)
{
ENTRY("vTableInit"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init"_Str);
  
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
ENTRY("vTableInsert"_Str);
 
  Object* pPtrTable;
  Object* pPtrTxRef;
  ValWrk xRow;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.insert"_Str);
  if ( ! mpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("table.insert"_Str);
  if ( ! mpTask->m_pDataStack->bPop(xRow) )
    THROW_INVALID_DSTACK("table.insert"_Str);

  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("table.insert"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());

  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  // invoked by copy...
  NEVER_REACH;
  
  /*
  pTable->vInsertRow(iPartition,
                     pTxRef,
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
  ENTRY("vTableName"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sName;
  
  sName = (Str*)pTable->aTableName();
  
  mpTask->m_pDataStack->vPushS32(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.ref -- has table got references?                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableRef(MP_Task mpTask)
{
  ENTRY("vTableRef"_Str);
  
  Object* pPtrTable;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.ref"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  
  bool bRef = pTable->bRefOut();
  
  mpTask->m_pDataStack->vPushInt(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.showColumnVec -- show column vector                                  */
/*----------------------------------------------------------------------------*/
void Task::vTableShowColumnVec(MP_Task mpTask)
{
  ENTRY("vTableShowColumnVec"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.createColumnVec"_Str);
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
  ENTRY("vTableSubDic"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.subDic"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dic* pDic = pTable->pSubDic();
  
  mpTask->m_pDataStack->vPushObject(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.slots -- push the slots of table on data stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTableSlots(MP_Task mpTask)
{
  ENTRY("vTableSlots"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.slots"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  mpTask->m_pDataStack->vPushObject(pTable->pSlots());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.space -- push the buffer space used bytable on data stack            */
/*----------------------------------------------------------------------------*/
void Task::vTableSpace(MP_Task mpTask)
{
ENTRY("vTableSpace"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.space"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 nBuffers = pTable->nRowBuffers();
  i8 iSpace   = nBuffers * C_SIZE_POOL_BUFFER;
  
  mpTask->m_pDataStack->vPushInt(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on data stack          */
/*----------------------------------------------------------------------------*/
void Task::vTableTruncate(MP_Task mpTask)
{
  ENTRY("vTableTruncate"_Str);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.truncate"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vTruncate();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
