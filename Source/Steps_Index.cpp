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
  vAddPrimitive("index.buffer"_Str,          &vIndexBuffer,          false);
  vAddPrimitive("index.columnCount"_Str,     &vIndexColumnCount,     false);
  vAddPrimitive("index.columnName"_Str,      &vIndexColumnName,      false);
  vAddPrimitive("index.columnType"_Str,      &vIndexColumnType,      false);
  vAddPrimitive("index.createColumnVec"_Str, &vIndexCreateColumnVec, false);
  vAddPrimitive("index.dump"_Str,            &vIndexDump,            false);
  vAddPrimitive("index.init"_Str,            &vIndexInit,            false);
  vAddPrimitive("index.populate"_Str,        &vIndexPopulate,        false);
  vAddPrimitive("index.position"_Str,        &vIndexPosition,        false);
  vAddPrimitive("index.positionFirst"_Str,   &vIndexPositionFirst,   false);
  vAddPrimitive("index.showColumnVec"_Str,   &vIndexShowColumnVec,   false);
  vAddPrimitive("index.space"_Str,           &vIndexSpace,           false);
  vAddPrimitive("index.subDic"_Str,          &vIndexSubDic,          false);
  vAddPrimitive("index.table@"_Str,          &vIndexTableFetch,      false);
  vAddPrimitive("index.table!"_Str,          &vIndexTableStore,      false);
  vAddPrimitive("index.truncate"_Str,        &vIndexTruncate,        false);
  vAddPrimitive("index.upper"_Str,           &vIndexUpper,           false);
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
  nnpTask->m_pDataStack->vPushS48K(sAnalysis);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.buffer -- position at an index buffer                                */
/*----------------------------------------------------------------------------*/
void Task::vIndexBuffer(NNP_Task nnpTask)
{
  ENTRY("vIndexBuffer"_Str);
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.buffer"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK("index.buffer"_Str);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(nnpTask->m_pSEitf->iPartition());
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
  
  // create a relational operator array
  RelOpAry* pOpAryGreater = new RelOpAry;
  i8 nItems = pIndex->pIdxColVec()->nItems();
  for (i8 i=0; i < nItems; i++)
    pOpAryGreater->vAdd(RO_GREATER_EQUAL);

  KeyWsp xKeyWsp;
  
  // assuming non-complex key
  //ASSERT(1 == pIndex->pIdxColVec()->nItems());
  
  StrVal* pStrVal = _pStrVal(xValWsp.pNestedValueSelf());
  
  xKeyWsp.vAddElement(pStrVal, pIndex->pIdxColVec()->pGet(0));
  IndexBuffer* pIndexBuffer = pIndexData->aPosition(xKeyWsp, pOpAryGreater, 0);
  
  //xKeyWsp.vShow();
  
  delete pOpAryGreater;
  
  bn nBuffer = pIndexBuffer->nBufferNo();
  //vTrace("L0 Buffer: %lld\n"_Str, *nBuffer);
  
  nnpTask->m_pDataStack->vPushObject(pIndexBuffer);
  
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
  
  nnpTask->m_pDataStack->vPushS32(pColumn->sColumnName());
  
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
  
  nnpTask->m_pDataStack->vPushS32(sColumnType);
  
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
/* index.position -- position at key in an index buffer                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexPosition(NNP_Task nnpTask)
{
  ENTRY("vIndexPosition"_Str);
  Object* pPtrIndex = nullptr;
  Object* pPtrIndexBuffer = nullptr;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.position"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("index.position"_Str);
  
  IndexBuffer* pIndexBuffer = _pIndexBuffer(pPtrIndexBuffer);
  ASSERT(CN_INDEXBUFFER == pIndexBuffer->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK("index.position"_Str);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(nnpTask->m_pSEitf->iPartition());
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
  
  // create a relational operator array
  RelOpAry* pOpAryGreater = new RelOpAry;
  i8 nItems = pIndex->pIdxColVec()->nItems();
  for (i8 i=0; i < nItems; i++)
    pOpAryGreater->vAdd(RO_GREATER_EQUAL);

  KeyWsp xKeyWsp;
  
  // assuming non-complex key
  //ASSERT(1 == pIndex->pIdxColVec()->nItems());
  
  StrVal* pStrVal = _pStrVal(xValWsp.pNestedValueSelf());
  
  xKeyWsp.vAddElement(pStrVal, pIndex->pIdxColVec()->pGet(0));
  
  bool bMatches = true;
  Entry* pEntry = pIndexBuffer->aPosition(xKeyWsp, pOpAryGreater);
  
  bMatches = pEntry->aKey()->bMatches(xKeyWsp.aKey(), pIndex->pIdxColVec(), pOpAryGreater);
  if ( ! bMatches) {
    pEntry = pIndexBuffer->aNextEntry(pEntry);
  }
  
  nnpTask->m_pDataStack->vPushObject(pEntry);
  
  delete pOpAryGreater;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.positionFirst -- position at first index buffer                      */
/*----------------------------------------------------------------------------*/
void Task::vIndexPositionFirst(NNP_Task nnpTask)
{
  ENTRY("vIndexFirst"_Str);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.positionFirst"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(nnpTask->m_pSEitf->iPartition());
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
  
  nnpTask->m_pDataStack->vPushObject(pIndexData->pFirstBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.init -- initialize the index                                         */
/*----------------------------------------------------------------------------*/
void Task::vIndexInit(NNP_Task nnpTask)
{
  ENTRY("vIndexInit"_Str);
  Object* pPtrIndex = nullptr;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.init"_Str);
  
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
/* index.upper -- check if entry is within upper bound for the index          */
/*----------------------------------------------------------------------------*/
void Task::vIndexUpper(NNP_Task nnpTask)
{
  ENTRY("vIndexUpper"_Str);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrEntry = nullptr;
  Object* pPtrTable = nullptr;

  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.upper"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("index.upper"_Str);
  
  Entry* pEntry = _aEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! nnpTask->m_pDataStack->bPop(xValWsp) )
    THROW_INVALID_DSTACK("index.upper"_Str);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(nnpTask->m_pSEitf->iPartition());
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
  
  // create a relational operator array
  RelOpAry* pOpAryUpper = new RelOpAry;
  i8 nItems = pIndex->pIdxColVec()->nItems();
  for (i8 i=0; i < nItems; i++)
    pOpAryUpper->vAdd(RO_LESS_EQUAL);

  KeyWsp xKeyWsp;
  
  // assuming non-complex key
  //ASSERT(1 == pIndex->pIdxColVec()->nItems());
  
  StrVal* pStrVal = _pStrVal(xValWsp.pNestedValueSelf());
  
  xKeyWsp.vAddElement(pStrVal, pIndex->pIdxColVec()->pGet(0));
  
  bool bMatches = pEntry->bMatches(xKeyWsp.aKey(), pIndex->pIdxColVec(), pOpAryUpper);
  
  delete pOpAryUpper;
  
  nnpTask->m_pDataStack->vPushInt(bMatches ? 1 : 0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
