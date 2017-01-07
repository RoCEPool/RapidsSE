/*============================================================================*/
/* Task_groups.cpp - task groups                                              */
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
  vAddPrimitive("table.append",       &vTableAppend,       false);
  vAddPrimitive("table.appendBuffer", &vTableAppendBuffer, false);
  vAddPrimitive("table.dic",          &vTableDic,          false);
  vAddPrimitive("table.drop",         &vTableDrop,         false);
  vAddPrimitive("table.init",         &vTableInit,         false);
  vAddPrimitive("table.insert",       &vTableInsert,       false);
  vAddPrimitive("table.name",         &vTableName,         false);
  vAddPrimitive("table.ref",          &vTableRef,          false);
  vAddPrimitive("table.space",        &vTableSpace,        false);
  vAddPrimitive("table.truncate",     &vTableTruncate,     false);
}

/*----------------------------------------------------------------------------*/
/* table.append -- append buffer into table (from seitf)                      */
/*----------------------------------------------------------------------------*/
void Task::vTableAppend(NNP_Task nnpTask)
{
  ENTRY("vTableAppend");
  i8 iPartition;
  s8K sString;
  
  if ( ! nnpTask->m_xDataStack.bPop(sString) )
    THROW_INVALID_DSTACK("table.append");
  
  Table* pTable = reinterpret_cast<Table*>(String::iToI8(sString.a()));
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! nnpTask->m_xDataStack.bPop(sString) )
    THROW_INVALID_DSTACK("table.append");
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(String::iToI8(sString.a()));
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  if ( ! nnpTask->m_xDataStack.bPop(sString) )
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
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition = 0;
  if ( ! nnpTask->m_xDataStack.bPop(iPartition) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_DSTACK("table.appendBuffer");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
    
  pTable->vAppendBuffer(iPartition,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.dic -- get table dictionary                                          */
/*----------------------------------------------------------------------------*/
void Task::vTableDic(NNP_Task nnpTask)
{
ENTRY("vTableDic");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.dic");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dic* pDic = pTable->pDic();
  
  nnpTask->m_xDataStack.vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDrop(NNP_Task nnpTask)
{
ENTRY("vTableDrop");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
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
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.dump");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableInit(NNP_Task nnpTask)
{
ENTRY("vTableInit");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  new(pTable) Table;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.insert -- insert row into table on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTableInsert(NNP_Task nnpTask)
{
ENTRY("vTableInsert");
  
  Object* pPtrTable;
  Object* pPtrTxRef;
  ValWsp xRow;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.insert");
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTxRef) )
    THROW_INVALID_DSTACK("table.insert");
  if ( ! nnpTask->m_xDataStack.bPop(xRow) )
    THROW_INVALID_DSTACK("table.insert");

  i8 iPartition;
  if ( ! nnpTask->m_xDataStack.bPop(iPartition) )
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
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.init");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sName;
  
  sName = pTable->pTableName();
  
  nnpTask->m_xDataStack.vPush(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.ref -- has table got references?                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableRef(NNP_Task nnpTask)
{
  ENTRY("vTableRef");
  
  Object* pPtrTable;
  
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.ref");
  
  Table* pTable = _pTable(pPtrTable);
  
  bool bRef = pTable->bRefOut();
  
  nnpTask->m_xDataStack.vPush(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTableTruncate(NNP_Task nnpTask)
{
ENTRY("vTableTruncate");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.truncate");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vTruncate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.space -- push the buffer space used bytable on param stack           */
/*----------------------------------------------------------------------------*/
void Task::vTableSpace(NNP_Task nnpTask)
{
ENTRY("vTableSpace");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xDataStack.bPop(pPtrTable) )
    THROW_INVALID_DSTACK("table.spave");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iBuffers = pTable->iRowBuffers();
  i8 iSpace   = iBuffers * C_SIZE_POOL_BUFFER;
  s256 sMag;
  
  nnpTask->m_xDataStack.vPush(iSpace);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
