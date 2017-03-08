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
  vAddPrimitive("table.analyze"_Str,      &vTableAnalyze,      false);
  vAddPrimitive("table.append"_Str,       &vTableAppend,       false);
  vAddPrimitive("table.appendBuffer"_Str, &vTableAppendBuffer, false);
  vAddPrimitive("table.drop"_Str,         &vTableDrop,         false);
  vAddPrimitive("table.init"_Str,         &vTableInit,         false);
  vAddPrimitive("table.insert"_Str,       &vTableInsert,       false);
  vAddPrimitive("table.name"_Str,         &vTableName,         false);
  vAddPrimitive("table.ref"_Str,          &vTableRef,          false);
  vAddPrimitive("table.space"_Str,        &vTableSpace,        false);
  vAddPrimitive("table.subDic"_Str,       &vTableSubDic,       false);
  vAddPrimitive("table.truncate"_Str,     &vTableTruncate,     false);
}

/*----------------------------------------------------------------------------*/
/* table.analyze -- analyze table                                             */
/*----------------------------------------------------------------------------*/
void Task::vTableAnalyze(NNP_Task nnpTask)
{
  ENTRY("vTableAnalyze"_Str);
  
  Object* pPtrTable;
  i8 iDetailed;
  
  if ( ! nnpTask->m_pDataStack->bPop(iDetailed) )
    THROW_INVALID_DSTACK("table.analyze"_Str);
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.analyze"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s48K sAnalysis;
  sAnalysis = (Str*)pTable->pAnalyze(iDetailed == 1 ? true : false);
  nnpTask->m_pDataStack->vPush(sAnalysis);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.append -- append buffer into table (from seitf)                      */
/*----------------------------------------------------------------------------*/
void Task::vTableAppend(NNP_Task nnpTask)
{
  ENTRY("vTableAppend"_Str);
  i8 iPartition;
  s48K sString;
  
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append"_Str);
  
  Table* pTable = reinterpret_cast<Table*>(StrLib::iToI8(sString.aStr()));
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append"_Str);
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(StrLib::iToI8(sString.aStr()));
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(sString) )
    THROW_INVALID_DSTACK("table.append"_Str);
  
  iPartition = StrLib::iToI8(sString.aStr());
  pTable->vAppendBuffer(iPartition,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.appendBuffer -- append row buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vTableAppendBuffer(NNP_Task nnpTask)
{
  ENTRY("vTableAppendBuffer"_Str);

  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Str);
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition = 0;
  if ( ! nnpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Str);
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("table.appendBuffer"_Str);
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
ENTRY("vTableDrop"_Str);
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.drop"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table dump                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDump(NNP_Task nnpTask)
{
ENTRY("vTableDump"_Str);
  
  vTrace("TableDump\n"_Str);
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.dump"_Str);
  
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
ENTRY("vTableInit"_Str);
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init"_Str);
  
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
ENTRY("vTableInsert"_Str);
  
  Object* pPtrTable;
  Object* pPtrTxRef;
  ValWsp xRow;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.insert"_Str);
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("table.insert"_Str);
  if ( ! nnpTask->m_pDataStack->bPop(xRow) )
    THROW_INVALID_DSTACK("table.insert"_Str);

  i8 iPartition;
  if ( ! nnpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("table.insert"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());

  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pTable->vInsertRow(iPartition,
                     pTxRef,
                     _pStrVal(&xRow)->aStr(),
                     _pStrVal(&xRow)->nAreaSize());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.name -- get table name                                               */
/*----------------------------------------------------------------------------*/
void Task::vTableName(NNP_Task nnpTask)
{
  ENTRY("vTableName"_Str);
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sName;
  
  sName = (Str*)pTable->pTableName();
  
  nnpTask->m_pDataStack->vPush(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.ref -- has table got references?                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableRef(NNP_Task nnpTask)
{
  ENTRY("vTableRef"_Str);
  
  Object* pPtrTable;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.ref"_Str);
  
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
  ENTRY("vTableSubDic"_Str);
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.subDic"_Str);
  
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
ENTRY("vTableSpace"_Str);
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.space"_Str);
  
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
  ENTRY("vTableTruncate"_Str);
  
  Object* pPtrTable;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.truncate"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vTruncate();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
