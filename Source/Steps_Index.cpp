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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// index: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Index(void)
{
  vAddPrimitive("index.analyze"_Stz,         &vIndexAnalyze,         false);
  vAddPrimitive("index.buffer"_Stz,          &vIndexBuffer,          false);
  vAddPrimitive("index.columnCount"_Stz,     &vIndexColumnCount,     false);
  vAddPrimitive("index.columnName"_Stz,      &vIndexColumnName,      false);
  vAddPrimitive("index.columnType"_Stz,      &vIndexColumnType,      false);
  vAddPrimitive("index.createColumnVec"_Stz, &vIndexCreateColumnVec, false);
  vAddPrimitive("index.dump"_Stz,            &vIndexDump,            false);
  vAddPrimitive("index.init"_Stz,            &vIndexInit,            false);
  vAddPrimitive("index.isHidden@"_Stz,       &vIndexIsHiddenFetch,   false);
  vAddPrimitive("index.isHidden!"_Stz,       &vIndexIsHiddenStore,   false);
  vAddPrimitive("index.isPrimaryKey@"_Stz,   &vIndexIsPrimaryKeyFetch, false);
  vAddPrimitive("index.isPrimaryKey!"_Stz,   &vIndexIsPrimaryKeyStore, false);
  vAddPrimitive("index.isUnique@"_Stz,       &vIndexIsUniqueFetch,   false);
  vAddPrimitive("index.isUnique!"_Stz,       &vIndexIsUniqueStore,   false);
  vAddPrimitive("index.isValidUniqueIndex"_Stz,&vIndexIsValidUniqueIndex,false);
  vAddPrimitive("index.name@"_Stz,           &vIndexNameFetch,       false);
  vAddPrimitive("index.populate"_Stz,        &vIndexPopulate,        false);
  vAddPrimitive("index.position"_Stz,        &vIndexPosition,        false);
  vAddPrimitive("index.positionFirst"_Stz,   &vIndexPositionFirst,   false);
  vAddPrimitive("index.showColumnVec"_Stz,   &vIndexShowColumnVec,   false);
  vAddPrimitive("index.space"_Stz,           &vIndexSpace,           false);
  vAddPrimitive("index.subDic"_Stz,          &vIndexSubDic,          false);
  vAddPrimitive("index.table@"_Stz,          &vIndexTableFetch,      false);
  vAddPrimitive("index.table!"_Stz,          &vIndexTableStore,      false);
  vAddPrimitive("index.truncate"_Stz,        &vIndexTruncate,        false);
  vAddPrimitive("index.upper"_Stz,           &vIndexUpper,           false);
  vAddPrimitive("index.verify"_Stz,          &vIndexVerify,         false);
  vAddPrimitive("index.visualize"_Stz,       &vIndexVisualize,       false);
}

/*----------------------------------------------------------------------------*/
/* index.analyze -- analyze index                                             */
/*----------------------------------------------------------------------------*/
void Task::vIndexAnalyze(MP_Task mpTask)
{
  ENTRY("vIndexAnalyze"_Stz);
  
  Object* pPtrIndex;
  i8 iDetailed;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.analyze"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iDetailed) )
    THROW_INVALID_DSTACK("index.analyze"_Stz);
  
  s48K sAnalysis;
  sAnalysis = (Stz*)pIndex->pTable()->pAnalyzeIndex(pIndex, iDetailed == 1 ? true : false);
  mpTask->m_pDataStack->vPushS48K(sAnalysis);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.buffer -- position at an index buffer                                */
/*----------------------------------------------------------------------------*/
void Task::vIndexBuffer(MP_Task mpTask)
{
  ENTRY("vIndexBuffer"_Stz);
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  i8 iPartition;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.buffer"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("index.buffer"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.buffer"_Stz);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(iPartition);
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
  
  KeyWrk xKeyWrk;
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  //vTrace("Datum: %s\n"_Stz,pStrVal->aChar());
  
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  xKeyWrk.vAddDatum(pStrVal,pColumn0);
  //xKeyWrk.vShow();
  
  EntryBuffer* pIndexBuffer = pIndexData->pPositionLevel(xKeyWrk, JC_AFTER, 0);
  
  mpTask->m_pDataStack->vPushObject(pIndexBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnCount -- get column count                                      */
/*----------------------------------------------------------------------------*/
void Task::vIndexColumnCount(MP_Task mpTask)
{
  ENTRY("vIndexColumnCount"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnCount"_Stz);
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
  ENTRY("vIndexColumnName"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnName"_Stz);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex;
  if ( ! mpTask->m_pDataStack->bPop(iIndex) )
    THROW_INVALID_DSTACK("index.columnName"_Stz);
  
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
  ENTRY("vIndexColumnType"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.columnType"_Stz);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex;
  if ( ! mpTask->m_pDataStack->bPop(iIndex) )
    THROW_INVALID_DSTACK("index.columnType"_Stz);
  
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
  ENTRY("vIndexCreateColumnVec"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Stz);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  s32 sColumnNm;
  if ( ! mpTask->m_pDataStack->bPop(sColumnNm) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Stz);
  
  s48K sColumnDef;
  if ( ! mpTask->m_pDataStack->bPop(sColumnDef) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Stz);
  
  pIndex->vCreateColumnVec(sColumnNm,sColumnDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.dump -- dump the index                                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexDump(MP_Task mpTask)
{
  ENTRY("vIndexDump"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.dump"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    Partition* pPartition = pTable->pPartition(iPartition);
    IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
    vTrace("Partition %lld\n"_Stz, iPartition);
    
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
  ENTRY("vIndexPosition"_Stz);
  Object* pPtrIndex = nullptr;
  Object* pPtrIndexBuffer = nullptr;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.position"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("index.position"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  ASSERT(CN_ENTRYBUFFER == pIndexBuffer->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.position"_Stz);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  KeyWrk xKeyWrk;
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  Column* pColumn = pIndex->pIdxColVec()->pGet(0);
  
  xKeyWrk.vAddDatum(pStrVal, pColumn);
  //xKeyWrk.aKey()->vShow(pIndex->pIdxColVec());

  Entry* pIndexEntry = pIndexBuffer->pPosition(xKeyWrk, JC_AFTER);
  //pIndexEntry->aKey()->vShow(pIndex->pIdxColVec());
  
  if (pIndexEntry) {
    bool bIsEqual = false;
    bool bAfter = pIndexEntry->aKey()->bJuxtaCompare(JC_AFTER, xKeyWrk.aKey(),
                                                     pIndex->pIdxColVec(), bIsEqual);
    if ( ! bAfter) {
      pIndexEntry = pIndexBuffer->pNextEntry(pIndexEntry);
    }
  }
  
  mpTask->m_pDataStack->vPushObject(pIndexEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.positionFirst -- position at first index buffer                      */
/*----------------------------------------------------------------------------*/
void Task::vIndexPositionFirst(MP_Task mpTask)
{
  ENTRY("vIndexFirst"_Stz);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  ValWrk xValWrk;
  i8 iPartition;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.positionFirst"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("index.positionFirst"_Stz);
  
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
  ENTRY("vIndexInit"_Stz);
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.init"_Stz);
  
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
  ENTRY("vIndexIsHiddenFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isHidden@"_Stz);
  
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
  ENTRY("vIndexIsHiddenStore"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isHidden!"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsHidden;
  if ( ! mpTask->m_pDataStack->bPop(iIsHidden) )
    THROW_INVALID_DSTACK("index.isHidden!"_Stz);
  
  pIndex->vIsHidden(iIsHidden ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey@ -- fetch isPrimaryKey from index                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsPrimaryKeyFetch(MP_Task mpTask)
{
  ENTRY("vIndexIsPrimaryKeyFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isPrimaryKey@"_Stz);
  
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
  ENTRY("vIndexIsPrimaryKeyStore"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isPrimaryKey!"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsPrimaryKey;
  if ( ! mpTask->m_pDataStack->bPop(iIsPrimaryKey) )
    THROW_INVALID_DSTACK("index.isPrimaryKey!"_Stz);
  
  pIndex->vIsPrimaryKey(iIsPrimaryKey ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isUnique@ -- fetch isUnique from index                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsUniqueFetch(MP_Task mpTask)
{
  ENTRY("vIndexIsUniqueFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isUnique@"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bIsUnique = pIndex->bIsUnique();
  
  mpTask->m_pDataStack->vPushInt(bIsUnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isUnique! -- store isUnique into index                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsUniqueStore(MP_Task mpTask)
{
  ENTRY("vIndexIsUniqueStore"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isUnique!"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsUnique;
  if ( ! mpTask->m_pDataStack->bPop(iIsUnique) )
    THROW_INVALID_DSTACK("index.isUnique!"_Stz);
  
  pIndex->vIsUnique(iIsUnique ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isValidUniqueIndex -- is it a valid unqiue index                     */
/*----------------------------------------------------------------------------*/
void Task::vIndexIsValidUniqueIndex(MP_Task mpTask)
{
  ENTRY("vIndexIsValidUniqueIndex"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.isValidUniqueIndex"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // get the table partition column vector
  PtnColVec* pPtnColVec = pIndex->pTable()->pPtnColVec();
  
  // get the index column vector
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  bool bValid = true;
  
  // a unique index must include all columns in the partitioning keys
  i8 nPtnColumns = pPtnColVec->nItems();
  for (i8 i = 0; i < nPtnColumns; i++)
  {
    Column* pColumn = pPtnColVec->pGet(i);
    if ( ! pIdxColVec->bContains(pColumn)) {
      bValid = false;
      break;
    }
  }
  
  mpTask->m_pDataStack->vPushInt(bValid ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.name@ -- fetch index name                                            */
/*----------------------------------------------------------------------------*/
void Task::vIndexNameFetch(MP_Task mpTask)
{
  ENTRY("vIndexNameFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.name@"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  s256 sIndexName = pIndex->sIndexName();
  
  mpTask->m_pDataStack->vPushS256(sIndexName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.populate -- populate the index                                       */
/*----------------------------------------------------------------------------*/
void Task::vIndexPopulate(MP_Task mpTask)
{
  ENTRY("vIndexPopulate"_Stz);
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.populate"_Stz);

  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  // not to poulate hidden index
  ASSERT( ! pIndex->bIsHidden())
  
  bool bPopulated = pTable->bPopulateIndex(iIndexNo);
 
  mpTask->m_pDataStack->vPushInt(bPopulated ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.showColumnVec -- show column vector                                  */
/*----------------------------------------------------------------------------*/
void Task::vIndexShowColumnVec(MP_Task mpTask)
{
  ENTRY("vIndexShowColumnVec"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.showColumnVec"_Stz);
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
  ENTRY("vIndexSpace"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.space"_Stz);
  
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
  ENTRY("vIndexSubDic"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.subDic"_Stz);
  
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
  ENTRY("vIndexTableFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.table@"_Stz);
  
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
  ENTRY("vIndexTableStore"_Stz);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.table!"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("index.table!"_Stz);
  
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
  ENTRY("vIndexTruncate"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.truncate"_Stz);
  
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
  ENTRY("vIndexUpper"_Stz);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrEntry = nullptr;
  Object* pPtrTable = nullptr;

  ValWrk xValWrk;
  i8 iPartition;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.upper"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("index.upper"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrEntry) )
    THROW_INVALID_DSTACK("index.upper"_Stz);
  
  Entry* pEntry = _pEntry(pPtrEntry);
  ASSERT(CN_ENTRY == pEntry->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.upper"_Stz);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Partition* pPartition = pTable->pPartition(iPartition);
  IndexData* pIndexData = pPartition->pIndexData(iIndexNo);

  KeyWrk xKeyWrk;
  
  // assuming non-complex key
  //ASSERT(1 == pIndex->pIdxColVec()->nItems());
  
  StrVal* pStrVal = _pStrVal(xValWrk.pNestedValueSelf());
  
  xKeyWrk.vAddDatum(pStrVal, pIndex->pIdxColVec()->pGet(0));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the relation operator for the first predicate
  
  SEitf* pSEitf = mpTask->m_pSEitf;
  Planner* pPlanner = pSEitf->m_pPlanner;
  AccessPath* pChosenPath = pPlanner->pChosenPath(0);
  PredicateVec* pUpperPredicates = pChosenPath->pUpperPredicates();
  Predicate* pPredicate = pUpperPredicates->pGet(0);
  Op* pOp = pPredicate->pOp();
  eRO eRelnOp = pOp->eOpKind();
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool bIsEqual = false;
  bool bBefore = pEntry->aKey()->bJuxtaCompare(JC_BEFORE, xKeyWrk.aKey(),
                                               pIndex->pIdxColVec(), bIsEqual,
                                               eRelnOp);
  
  mpTask->m_pDataStack->vPushInt(bBefore ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.verify -- verify index                                               */
/*----------------------------------------------------------------------------*/
void Task::vIndexVerify(MP_Task mpTask)
{
  ENTRY("vIndexVerify"_Stz);
  
  Object* pPtrIndex;
  i8 iDetailed;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.verify"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bVerified = pIndex->pTable()->bVerifyIndex(pIndex);
  s256 s = bVerified ? "Valid"_Stz : "Invalid"_Stz;
  mpTask->m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.visualize -- visualize the index                                     */
/*----------------------------------------------------------------------------*/
void Task::vIndexVisualize(MP_Task mpTask)
{
  ENTRY("vIndexVisualize"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.visualize"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    Partition* pPartition = pTable->pPartition(iPartition);
    IndexData* pIndexData = pPartition->pIndexData(iIndexNo);
    vTrace("Partition %lld\n"_Stz, iPartition);
    pIndexData->vVisualize();
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
