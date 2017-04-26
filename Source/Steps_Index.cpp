/*============================================================================*/
/* Steps_Index.cpp - index step group                                         */
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

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Index(void)
{
  vAddPrimitive("index.analyze"_Str,         &vIndexAnalyze,         false);
  vAddPrimitive("index.columnCount"_Str,     &vIndexColumnCount,     false);
  vAddPrimitive("index.columnName"_Str,      &vIndexColumnName,      false);
  vAddPrimitive("index.columnType"_Str,      &vIndexColumnType,      false);
  vAddPrimitive("index.createColumnVec"_Str, &vIndexCreateColumnVec, false);
  vAddPrimitive("index.dump"_Str,            &vIndexDump,            false);
  vAddPrimitive("index.init"_Str,            &vIndexInit,            false);
  vAddPrimitive("index.populate"_Str,        &vIndexPopulate,        false);
  vAddPrimitive("index.showColumnVec"_Str,   &vIndexShowColumnVec,   false);
  vAddPrimitive("index.space"_Str,           &vIndexSpace,           false);
  vAddPrimitive("index.subDic"_Str,          &vIndexSubDic,          false);
  vAddPrimitive("index.table@"_Str,          &vIndexTableFetch,      false);
  vAddPrimitive("index.table!"_Str,          &vIndexTableStore,      false);
  vAddPrimitive("index.truncate"_Str,        &vIndexTruncate,        false);

  // these should go !!!
  vAddPrimitive("index::columnTest"_Str, &vIndexColumnTest, false);
  vAddPrimitive("index::entryTest"_Str,  &vIndexEntryTest,  false);
  vAddPrimitive("index::ibNew"_Str,      &vIndexIbNew,      false);
  vAddPrimitive("index::ibDump"_Str,     &vIndexIbDump,     false);
}

/*----------------------------------------------------------------------------*/
/* index.analyze -- analyze index                                             */
/*----------------------------------------------------------------------------*/
void Task::vIndexAnalyze(NNP_Task nnpTask)
{
  ENTRY("vIndexAnalyze"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.analyze"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  s48K sAnalysis;
  sAnalysis = (Str*)pIndex->pTable()->pAnalyzeIndex(pIndex);
  nnpTask->m_pDataStack->vPush(sAnalysis);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnCount -- get column count                                      */
/*----------------------------------------------------------------------------*/
void Task::vIndexColumnCount(NNP_Task nnpTask)
{
  ENTRY("vIndexColumnCount"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnCount"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  nnpTask->m_pDataStack->vPushInt(pIndex->pIdxColVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnName -- get column name                                        */
/*----------------------------------------------------------------------------*/
void Task::vIndexColumnName(NNP_Task nnpTask)
{
  ENTRY("vIndexColumnName"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnName"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex;
  if ( ! nnpTask->m_pDataStack->bPop(iIndex) )
    THROW_INVALID_DSTACK("index.columnName"_Str);
  
  ASSERT(iIndex < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iIndex);
  
  nnpTask->m_pDataStack->vPush(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnType -- get column type                                        */
/*----------------------------------------------------------------------------*/
void Task::vIndexColumnType(NNP_Task nnpTask)
{
  ENTRY("vIndexColumnType"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnType"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex;
  if ( ! nnpTask->m_pDataStack->bPop(iIndex) )
    THROW_INVALID_DSTACK("index.columnType"_Str);
  
  ASSERT(iIndex < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iIndex);
  s32 sColumnType = DBobjects::aCT[pColumn->eColumnType()];
  
  nnpTask->m_pDataStack->vPush(sColumnType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.createColumnVec -- create column vector from sub dictionary          */
/*----------------------------------------------------------------------------*/
void Task::vIndexCreateColumnVec(NNP_Task nnpTask)
{
  ENTRY("vIndexCreateColumnVec"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  s32 sColumnNm;
  if ( ! nnpTask->m_pDataStack->bPop(sColumnNm) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Str);
  
  s48K sColumnDef;
  if ( ! nnpTask->m_pDataStack->bPop(sColumnDef) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Str);
  
  pIndex->vCreateColumnVec(sColumnNm,sColumnDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.dump -- dump the index                                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexDump(NNP_Task nnpTask)
{
  ENTRY("vIndexDump"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.dump"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    Partition* pPartition = pTable->pPartition(iPartition);
    IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
    vTrace("Partition %lld\n"_Str, iPartition);
    pIndexData->vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::init -- initialize the index                                        */
/*----------------------------------------------------------------------------*/
void Task::vIndexInit(NNP_Task nnpTask)
{
  ENTRY("vIndexInit"_Str);
  Object* pPtrIndex = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index::vIndexInit"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  new(pIndex) Index;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.populate -- populate the index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexPopulate(NNP_Task nnpTask)
{
  ENTRY("vIndexPopulate"_Str);
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.populate!"_Str);

  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  pTable->vPopulateIndex(iIndexNo);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.showColumnVec -- show column vector                                  */
/*----------------------------------------------------------------------------*/
void Task::vIndexShowColumnVec(NNP_Task nnpTask)
{
  ENTRY("vIndexShowColumnVec"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.showColumnVec"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndex->vShowColumnVec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.space -- push space used by index                                    */
/*----------------------------------------------------------------------------*/
void Task::vIndexSpace(NNP_Task nnpTask)
{
  ENTRY("vIndexSpace"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.space"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iBuffers = pIndex->pTable()->nIndexBuffers(pIndex->iIndexNo());
  i8 iSpace = iBuffers * C_SIZE_POOL_BUFFER;
  nnpTask->m_pDataStack->vPushInt(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.subDic -- get table sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Task::vIndexSubDic(NNP_Task nnpTask)
{
  ENTRY("vIndexSubDic"_Str);
  
  Object* pPtrIndex;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.subDic"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Dic* pDic = pIndex->pSubDic();
  
  nnpTask->m_pDataStack->vPushObject(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table@ -- fetch table in index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexTableFetch(NNP_Task nnpTask)
{
  ENTRY("vIndexTableFetch"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.table@"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Table* pTable = pIndex->pTable();
  ASSERT(CN_TABLE == pTable->eClass());
  
  nnpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table! -- store table in index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexTableStore(NNP_Task nnpTask)
{
  ENTRY("vIndexTableStore"_Str);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.table!"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("index.table!"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pIndex->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.truncate -- truncate the index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexTruncate(NNP_Task nnpTask)
{
  ENTRY("vIndexTruncate"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.truncate"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  pTable->vTruncateIndex(iIndexNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::entryTest -- initial test of index entry                            */
/*----------------------------------------------------------------------------*/
/*
void Task::vIndexEntryTest(NNP_Task nnpTask)
{
  ENTRY("vIndexEntryTest"_Str);
  
  Object* pPtrIb = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIb) )
    THROW_INVALID_DSTACK("index::entryTest"_Str);
  
  IndexBufferSeq* pIndexBufferSeq = _pIndexBufferSeq(pPtrIb);
  ASSERT(CN_INDEXBUFFER == pIndexBufferSeq->eClass());
  
  s48K sRow;
  if ( ! nnpTask->m_pDataStack->bPop(sRow) )
    THROW_INVALID_DSTACK("index::entryTest"_Str);
  
  vTrace("index::entryTest -- %s\n"_Str,sRow.s());
  
  EntWspSeq xEntWspSeq;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set rowId
  RowId xRowId;
  i8 iRowNo = sRow.iToI8();
  
  xRowId.vBuffer(999);
  xRowId.vRow(iRowNo);
  xEntWspSeq.vRowId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add element to key
  KeyWsp xKeyWsp;
  ValWsp xValWsp;
  
  
  //i8 iKey0 = sRow.iToI8();
  i8 iKey0 = Random::iRandom(0,1000);
  
  xValWsp = iKey0;
  
  xKeyWsp.vAddElement(xValWsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add key to entry
  xEntWspSeq.vAddKey(xKeyWsp);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // show
  //xEntWsp.vShow();
  
  // insert entry into index buffer
  //pIndexBufferSeq->vInsertEntry(xEntWsp);
  
  // verify index buffer
  //pIndexBufferSeq->vVerify();
  
  
  bool bResult = false;
  IndexBufferSeq* pFirstBuffer = pIndexBufferSeq;
  
  while (!bResult) {
    pIndexBufferSeq = pFirstBuffer->pFindBuffer(xEntWspSeq);
    
    // insert entry into index buffer
    bResult = pIndexBufferSeq->bInsertEntry(xEntWspSeq);
  }
  
  pIndexBufferSeq = pFirstBuffer;
  while (pIndexBufferSeq) {
    pIndexBufferSeq->vVerify();
    
    pIndexBufferSeq = pIndexBufferSeq->pNextBuffer();
  }
  
  
  EXIT();
}
*/
/*----------------------------------------------------------------------------*/
/* index::columnTest -- initial test of index columns                         */
/*----------------------------------------------------------------------------*/
void Task::vIndexColumnTest(NNP_Task nnpTask)
{
  ENTRY("vIndexColumnTest"_Str);
  
  s48K sRow = "0|1|2|3|4|5|6|7|8|9"_Str;
  
  KeyWsp xKeyWsp;
  
  bool b;
  
  b = xKeyWsp.bExtractElement(sRow,0,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,1,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,2,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,3,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_INT,DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,4,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,5,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,6,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,7,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,8,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_NONE);
  
  b = xKeyWsp.bExtractElement(sRow,9,DM_INT);
  xKeyWsp.vShow(DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_INT,DM_NONE);
  
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* index::entryTest -- initial test of index entry                            */
/*----------------------------------------------------------------------------*/
void Task::vIndexEntryTest(NNP_Task nnpTask)
{
  ENTRY("vIndexEntryTest"_Str);
  
  Object* pPtrIb = nullptr;
  //static i8 i = 1;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIb) )
    THROW_INVALID_DSTACK("index::entryTest"_Str);
  
  IndexBuffer* pIndexBuffer = _pIndexBuffer(pPtrIb);
  ASSERT(CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  s48K sRow;
  if ( ! nnpTask->m_pDataStack->bPop(sRow) )
    THROW_INVALID_DSTACK("index::entryTest"_Str);
  
  vTrace("index::entryTest -- %s\n"_Str,sRow.s());
  
  EntWsp xEntWsp;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set rowId
  RowId xRowId;
  i8 iRowNo = sRow.iToI8();
  
  xRowId.vBuffer(999);
  xRowId.vRow(iRowNo);
  xEntWsp.vRowId(xRowId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add element to key
  KeyWsp xKeyWsp;
  ValWsp xValWsp;
  
  i8 iKey0 = Random::iRandom(0,1000);
  
  xValWsp = iKey0;
  
  xKeyWsp.vAddElement(xValWsp);

  Key xKey(xKeyWsp);
  
  // insert entry into index buffer
  pIndexBuffer->bInsert(&xKey, xRowId);
  
  // verify index buffer
  //pIndexBuffer->vVerify();
  
  /*
  IndexBuffer* pFirstBuffer = pIndexBuffer;


  bool bResult = false;
  
  while (!bResult) {
    pIndexBuffer = pFirstBuffer->pFindBuffer(xEntWsp);
    
    // insert entry into index buffer
    bResult = pIndexBuffer->bInsert(xEntWsp);
  }
  */
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::ibDump -- dump the index buffer                                     */
/*----------------------------------------------------------------------------*/
void Task::vIndexIbDump(NNP_Task nnpTask)
{
  ENTRY("vIndexIbDump"_Str);
  
  Object* pPtrIB;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIB) )
    THROW_INVALID_DSTACK("index::ibDump"_Str);
  
  IndexBuffer* pIndexBuffer = _pIndexBuffer(pPtrIB);
  ASSERT(CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  vTrace("\nIndex Dump \n"_Str);
  
  while (pIndexBuffer) {
    pIndexBuffer->vShow();
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::ibNew -- new index buffer (temporary?)                              */
/*----------------------------------------------------------------------------*/
void Task::vIndexIbNew(NNP_Task nnpTask)
{
  ENTRY("vIndexIbNew"_Str);
  
  IndexBuffer* pIndexBuffer = new IndexBuffer;
  ASSERT(CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  nnpTask->m_pDataStack->vPushObject(pIndexBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
