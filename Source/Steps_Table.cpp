/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  vAddPrimitive("table.analyze",      &vTableAnalyze,      false);
  vAddPrimitive("table.append",       &vTableAppend,       false);
  vAddPrimitive("table.appendBuffer", &vTableAppendBuffer, false);
  vAddPrimitive("table.drop",         &vTableDrop,         false);
  vAddPrimitive("table.init",         &vTableInit,         false);
  vAddPrimitive("table.insert",       &vTableInsert,       false);
  vAddPrimitive("table.name",         &vTableName,         false);
  vAddPrimitive("table.ref",          &vTableRef,          false);
  vAddPrimitive("table.space",        &vTableSpace,        false);
  vAddPrimitive("table.subDic",       &vTableSubDic,       false);
  vAddPrimitive("table.truncate",     &vTableTruncate,     false);
}

/*----------------------------------------------------------------------------*/
/* table.analyze -- analyze table                                             */
/*----------------------------------------------------------------------------*/
void Task::vTableAnalyze(NNP_Task nnpTask)
{
  ENTRY("vTableAnalyze");
  
  Object* pPtrTable;
  i8 iDetailed;
  
  if ( ! nnpTask->m_pDataStack->bPop(iDetailed) )
    THROW_INVALID_DSTACK("table.analyze");
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.analyze");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s48K sAnalysis;
  sAnalysis = pTable->pAnalyze(iDetailed == 1 ? true : false);
  nnpTask->m_pDataStack->vPush(sAnalysis);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.append -- append buffer into table (from seitf)                      */
/*----------------------------------------------------------------------------*/
void Task::vTableAppend(NNP_Task nnpTask)
{
  ENTRY("vTableAppend");
  i8 iPartition;
  s48K sString;
  
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append");
  
  Table* pTable = reinterpret_cast<Table*>(String::iToI8(sString.a()));
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append");
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(String::iToI8(sString.a()));
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append");
  
  iPartition = String::iToI8(sString.a());
  pTable->vAppendBuffer(iPartition,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.appendBuffer -- append row buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vTableAppendBuffer(NNP_Task nnpTask)
{
  ENTRY("vTableAppendBuffer");

  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition = 0;
  if ( ! nnpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
  
  pTable->vAppendBuffer(iSlot,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vTableDrop(NNP_Task nnpTask)
{
ENTRY("vTableDrop");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.drop");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table dump                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDump(NNP_Task nnpTask)
{
ENTRY("vTableDump");
  
  vTrace("TableDump\n");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.dump");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on data stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vTableInit(NNP_Task nnpTask)
{
ENTRY("vTableInit");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  new(pTable) Table;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.insert -- insert row into table on data stack                        */
/*----------------------------------------------------------------------------*/
void Task::vTableInsert(NNP_Task nnpTask)
{
ENTRY("vTableInsert");
  
  Object* pPtrTable;
  Object* pPtrTxRef;
  ValWsp xRow;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.insert");
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("table.insert");
  if ( ! nnpTask->m_pDataStack->bPop(xRow) )
    THROW_INVALID_DSTACK("table.insert");

  i8 iPartition;
  if ( ! nnpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("table.insert");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());

  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pTable->vInsertRow(iPartition,
                     pTxRef,
                     xRow.pAsStrVal()->sStr(),
                     xRow.pAsStrVal()->iChars());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.name -- get table name                                               */
/*----------------------------------------------------------------------------*/
void Task::vTableName(NNP_Task nnpTask)
{
  ENTRY("vTableName");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sName;
  
  sName = pTable->pTableName();
  
  nnpTask->m_pDataStack->vPush(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.ref -- has table got references?                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableRef(NNP_Task nnpTask)
{
  ENTRY("vTableRef");
  
  Object* pPtrTable;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.ref");
  
  Table* pTable = _pTable(pPtrTable);
  
  bool bRef = pTable->bRefOut();
  
  nnpTask->m_pDataStack->vPush(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.subDic -- get table sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Task::vTableSubDic(NNP_Task nnpTask)
{
  ENTRY("vTableSubDic");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.subDic");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dic* pDic = pTable->pSubDic();
  
  nnpTask->m_pDataStack->vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.space -- push the buffer space used bytable on data stack            */
/*----------------------------------------------------------------------------*/
void Task::vTableSpace(NNP_Task nnpTask)
{
ENTRY("vTableSpace");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.space");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 nBuffers = pTable->nRowBuffers();
  i8 iSpace   = nBuffers * C_SIZE_POOL_BUFFER;
  
  nnpTask->m_pDataStack->vPush(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on data stack          */
/*----------------------------------------------------------------------------*/
void Task::vTableTruncate(NNP_Task nnpTask)
{
  ENTRY("vTableTruncate");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.truncate");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vTruncate();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
