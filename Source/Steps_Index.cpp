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
  vAddPrimitive("index.isHidden@"_Str,       &vIndexIsHiddenFetch,   false);
  vAddPrimitive("index.isHidden!"_Str,       &vIndexIsHiddenStore,   false);
  vAddPrimitive("index.isPrimaryKey@"_Str,   &vIndexIsPrimaryKeyFetch,   false);
  vAddPrimitive("index.isPrimaryKey!"_Str,   &vIndexIsPrimaryKeyStore,   false);
  vAddPrimitive("index.isUnique@"_Str,       &vIndexIsUniqueFetch,   false);
  vAddPrimitive("index.isUnique!"_Str,       &vIndexIsUniqueStore,   false);
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
  vAddPrimitive("index.visualize"_Str,       &vIndexVisualize,       false);
}

/*----------------------------------------------------------------------------*/
/* index.analyze -- analyze index                                             */
/*----------------------------------------------------------------------------*/
void Task::vIndexAnalyze(MP_Task mpTask)
{
  ENTRY("vIndexAnalyze"_Str);
  
  Object* pPtrIndex;
  i8 iDetailed;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.analyze"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iDetailed) )
    THROW_INVALID_DSTACK("index.analyze"_Str);
  
  s48K sAnalysis;
  sAnalysis = (Str*)pIndex->pTable()->pAnalyzeIndex(pIndex, iDetailed == 1 ? true : false);
  mpTask->m_pDataStack->vPushS48K(sAnalysis);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.buffer -- position at an index buffer                                */
/*----------------------------------------------------------------------------*/
void Task::vIndexBuffer(MP_Task mpTask)
{
  ENTRY("vIndexBuffer"_Str);
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  i8 iPartition;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.buffer"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("index.buffer"_Str);
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.buffer"_Str);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(iPartition);
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
  
  KeyWrk xKeyWrk;
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  xKeyWrk.vAddDatum(pStrVal, pIndex->pIdxColVec()->pGet(0));
  EntryBuffer* pIndexBuffer = pIndexData->pPositionLevel(xKeyWrk, JC_AFTER, 0);
  
  //xKeyWrk.vShow();
  
  bn nBuffer = pIndexBuffer->nBufferNo();
  //vTrace("L0 Buffer: %lld\n"_Str, *nBuffer);
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnCount -- get column count                                      */
/*----------------------------------------------------------------------------*/
void Task::vIndexColumnCount(MP_Task mpTask)
{
  ENTRY("vIndexColumnCount"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnCount"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  mpTask->m_pDataStack->vPushInt(pIndex->pIdxColVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnName -- get column name                                        */
/*----------------------------------------------------------------------------*/
void Task::vIndexColumnName(MP_Task mpTask)
{
  ENTRY("vIndexColumnName"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnName"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex;
  if ( ! mpTask->m_pDataStack->bPop(iIndex) )
    THROW_INVALID_DSTACK("index.columnName"_Str);
  
  ASSERT(iIndex < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iIndex);
  
  mpTask->m_pDataStack->vPushS32(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnType -- get column type                                        */
/*----------------------------------------------------------------------------*/
void Task::vIndexColumnType(MP_Task mpTask)
{
  ENTRY("vIndexColumnType"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnType"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex;
  if ( ! mpTask->m_pDataStack->bPop(iIndex) )
    THROW_INVALID_DSTACK("index.columnType"_Str);
  
  ASSERT(iIndex < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iIndex);
  s32 sColumnType = DBobjects::aCT[pColumn->eColumnType()];
  
  mpTask->m_pDataStack->vPushS32(sColumnType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.createColumnVec -- create column vector from sub dictionary          */
/*----------------------------------------------------------------------------*/
void Task::vIndexCreateColumnVec(MP_Task mpTask)
{
  ENTRY("vIndexCreateColumnVec"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  s32 sColumnNm;
  if ( ! mpTask->m_pDataStack->bPop(sColumnNm) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Str);
  
  s48K sColumnDef;
  if ( ! mpTask->m_pDataStack->bPop(sColumnDef) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Str);
  
  pIndex->vCreateColumnVec(sColumnNm,sColumnDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.dump -- dump the index                                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexDump(MP_Task mpTask)
{
  ENTRY("vIndexDump"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.dump"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    Partition* pPartition = pTable->pPartition(iPartition);
    IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
    vTrace("Partition %lld\n"_Str, iPartition);
    
    if (pIndex->bIsHidden())
      pIndexData->vDump(true);
    else
      pIndexData->vDump(false);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.position -- position at key in an index buffer                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexPosition(MP_Task mpTask)
{
  ENTRY("vIndexPosition"_Str);
  Object* pPtrIndex = nullptr;
  Object* pPtrIndexBuffer = nullptr;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.position"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("index.position"_Str);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.position"_Str);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  KeyWrk xKeyWrk;
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  xKeyWrk.vAddDatum(pStrVal, pIndex->pIdxColVec()->pGet(0));

  Entry* pEntry = pIndexBuffer->pPosition(xKeyWrk, JC_AFTER);
  
  //pEntry->aKey()->vShow(pIndex->pIdxColVec());
  //xKeyWrk.aKey()->vShow(pIndex->pIdxColVec());
  
  if (pEntry) {
    bool bIsEqual = false;
    bool bAfter = pEntry->aKey()->bJuxtaCompare(JC_AFTER, xKeyWrk.aKey(), pIndex->pIdxColVec(), bIsEqual);
    if ( ! bAfter) {
      pEntry = pIndexBuffer->pNextEntry(pEntry);
    }
  }
  
  mpTask->m_pDataStack->vPushObject(pEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.positionFirst -- position at first index buffer                      */
/*----------------------------------------------------------------------------*/
void Task::vIndexPositionFirst(MP_Task mpTask)
{
  ENTRY("vIndexFirst"_Str);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  ValWrk xValWrk;
  i8 iPartition;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.positionFirst"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("index.positionFirst"_Str);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(iPartition);
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
  
  mpTask->m_pDataStack->vPushObject(pIndexData->pFirstBuffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.init -- initialize the index                                         */
/*----------------------------------------------------------------------------*/
void Task::vIndexInit(MP_Task mpTask)
{
  ENTRY("vIndexInit"_Str);
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.init"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  new(pIndex) Index;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isHidden@ -- fetch isHidden from index                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsHiddenFetch(MP_Task mpTask)
{
  ENTRY("vIndexIsHiddenFetch"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isHidden@"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bIsHidden = pIndex->bIsHidden();
  
  mpTask->m_pDataStack->vPushInt(bIsHidden);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isHidden! -- store isHidden into index                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsHiddenStore(MP_Task mpTask)
{
  ENTRY("vIndexIsHiddenStore"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isHidden!"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsHidden;
  if ( ! mpTask->m_pDataStack->bPop(iIsHidden) )
    THROW_INVALID_DSTACK("index.isHidden!"_Str);
  
  pIndex->vIsHidden(iIsHidden ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey@ -- fetch isPrimaryKey from index                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsPrimaryKeyFetch(MP_Task mpTask)
{
  ENTRY("vIndexIsPrimaryKeyFetch"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isPrimaryKey@"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bIsPrimaryKey = pIndex->bIsPrimaryKey();
  
  mpTask->m_pDataStack->vPushInt(bIsPrimaryKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey! -- store isPrimaryKey into index                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsPrimaryKeyStore(MP_Task mpTask)
{
  ENTRY("vIndexIsPrimaryKeyStore"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isPrimaryKey!"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsPrimaryKey;
  if ( ! mpTask->m_pDataStack->bPop(iIsPrimaryKey) )
    THROW_INVALID_DSTACK("index.isPrimaryKey!"_Str);
  
  pIndex->vIsPrimaryKey(iIsPrimaryKey ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isUnique@ -- fetch isUnique from index                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsUniqueFetch(MP_Task mpTask)
{
  ENTRY("vIndexIsUniqueFetch"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isUnique@"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bIsUnique = pIndex->bIsUnique();
  
  mpTask->m_pDataStack->vPushInt(bIsUnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isUnque! -- store isUnique into index                                */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsUniqueStore(MP_Task mpTask)
{
  ENTRY("vIndexIsUniqueStore"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isUnique!"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsUnique;
  if ( ! mpTask->m_pDataStack->bPop(iIsUnique) )
    THROW_INVALID_DSTACK("index.isUnique!"_Str);
  
  pIndex->vIsUnique(iIsUnique ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.populate -- populate the index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexPopulate(MP_Task mpTask)
{
  ENTRY("vIndexPopulate"_Str);
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.populate"_Str);

  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  // not to poulate hidden index
  ASSERT( ! pIndex->bIsHidden())
  pTable->vPopulateIndex(iIndexNo);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.showColumnVec -- show column vector                                  */
/*----------------------------------------------------------------------------*/
void Task::vIndexShowColumnVec(MP_Task mpTask)
{
  ENTRY("vIndexShowColumnVec"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.showColumnVec"_Str);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndex->vShowColumnVec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.space -- push space used by index                                    */
/*----------------------------------------------------------------------------*/
void Task::vIndexSpace(MP_Task mpTask)
{
  ENTRY("vIndexSpace"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.space"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iBuffers = pIndex->pTable()->nIndexBuffers(pIndex->iIndexNo());
  i8 iSpace = iBuffers * C_SIZE_POOL_BUFFER;
  mpTask->m_pDataStack->vPushInt(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.subDic -- get table sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Task::vIndexSubDic(MP_Task mpTask)
{
  ENTRY("vIndexSubDic"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.subDic"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Dic* pDic = pIndex->pSubDic();
  
  mpTask->m_pDataStack->vPushObject(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table@ -- fetch table in index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexTableFetch(MP_Task mpTask)
{
  ENTRY("vIndexTableFetch"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.table@"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Table* pTable = pIndex->pTable();
  ASSERT(CN_TABLE == pTable->eClass());
  
  mpTask->m_pDataStack->vPushObject(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table! -- store table in index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexTableStore(MP_Task mpTask)
{
  ENTRY("vIndexTableStore"_Str);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.table!"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("index.table!"_Str);
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pIndex->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.truncate -- truncate the index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexTruncate(MP_Task mpTask)
{
  ENTRY("vIndexTruncate"_Str);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
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
void Task::vIndexUpper(MP_Task mpTask)
{
  ENTRY("vIndexUpper"_Str);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrEntry = nullptr;
  Object* pPtrTable = nullptr;

  ValWrk xValWrk;
  i8 iPartition;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.upper"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("index.positionFirst"_Str);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("index.upper"_Str);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.upper"_Str);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(iPartition);
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);

  KeyWrk xKeyWrk;
  
  // assuming non-complex key
  //ASSERT(1 == pIndex->pIdxColVec()->nItems());
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  xKeyWrk.vAddDatum(pStrVal, pIndex->pIdxColVec()->pGet(0));
  
  bool bIsEqual = false;
  bool bBefore = pEntry->aKey()->bJuxtaCompare(JC_BEFORE, xKeyWrk.aKey(), pIndex->pIdxColVec(), bIsEqual);
  
  mpTask->m_pDataStack->vPushInt(bBefore ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.visualize -- visualize the index                                     */
/*----------------------------------------------------------------------------*/
void Task::vIndexVisualize(MP_Task mpTask)
{
  ENTRY("vIndexVisualize"_Str);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.visualize"_Str);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    Partition* pPartition = pTable->pPartition(iPartition);
    IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
    vTrace("Partition %lld\n"_Str, iPartition);
    pIndexData->vVisualize();
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
