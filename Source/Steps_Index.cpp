/*============================================================================*/
/* Steps_Index.cpp - index step group                                         */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Index(void)
{
  vAddStep("index.analyze"_Stz,         &Step::vIndexAnalyze);
  vAddStep("index.columnCount@"_Stz,    &Step::vIndexColumnCountFetch);
  vAddStep("index.column@"_Stz,         &Step::vIndexColumnFetch);
  vAddStep("index.columnName"_Stz,      &Step::vIndexColumnName);
  vAddStep("index.columnType"_Stz,      &Step::vIndexColumnType);
  vAddStep("index.compact"_Stz,         &Step::vIndexCompact);
  vAddStep("index.createColumnVecHidden"_Stz, &Step::vIndexCreateColumnVecHidden);
  vAddStep("index.createColumnVec"_Stz, &Step::vIndexCreateColumnVec);
  vAddStep("index.dump"_Stz,            &Step::vIndexDump);
  vAddStep("index.exactKey"_Stz,        &Step::vIndexExactKey);
  vAddStep("index.init"_Stz,            &Step::vIndexInit);
  vAddStep("index.isHidden@"_Stz,       &Step::vIndexIsHiddenFetch);
  vAddStep("index.isHidden!"_Stz,       &Step::vIndexIsHiddenStore);
  vAddStep("index.isPrimaryKey@"_Stz,   &Step::vIndexIsPrimaryKeyFetch);
  vAddStep("index.isPrimaryKey!"_Stz,   &Step::vIndexIsPrimaryKeyStore);
  vAddStep("index.isUnique@"_Stz,       &Step::vIndexIsUniqueFetch);
  vAddStep("index.isUnique!"_Stz,       &Step::vIndexIsUniqueStore);
  vAddStep("index.isValidUniqueIndex"_Stz,&Step::vIndexIsValidUniqueIndex);
  vAddStep("index.lowerBound"_Stz,      &Step::vIndexLowerBound);
  vAddStep("index.name@"_Stz,           &Step::vIndexNameFetch);
  vAddStep("index.populate"_Stz,        &Step::vIndexPopulate);
  vAddStep("index.position"_Stz,        &Step::vIndexPosition);
  vAddStep("index.positionFirst"_Stz,   &Step::vIndexPositionFirst);
  vAddStep("index.range"_Stz,           &Step::vIndexRange);
  vAddStep("index.setSingleColumn"_Stz, &Step::vIndexSetSingleColumn);
  vAddStep("index.showColumnVec"_Stz,   &Step::vIndexShowColumnVec);
  vAddStep("index.scan"_Stz,            &Step::vIndexScan);
  vAddStep("index.space"_Stz,           &Step::vIndexSpace);
  vAddStep("index.subDic"_Stz,          &Step::vIndexSubDic);
  vAddStep("index.table@"_Stz,          &Step::vIndexTableFetch);
  vAddStep("index.table!"_Stz,          &Step::vIndexTableStore);
  vAddStep("index.truncate"_Stz,        &Step::vIndexTruncate);
  vAddStep("index.upper"_Stz,           &Step::vIndexUpper);
  vAddStep("index.upperBound"_Stz,      &Step::vIndexUpperBound);
  vAddStep("index.verify"_Stz,          &Step::vIndexVerify);
  vAddStep("index.visualize"_Stz,       &Step::vIndexVisualize);
}

/*----------------------------------------------------------------------------*/
/* index.analyze -- analyze index                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexAnalyze(MP_Task mpTask)
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
  i8 nRows;
  sAnalysis = (Stz*)pIndex->pTable()->pAnalyzeIndex(pIndex, iDetailed == 1 ? true : false, nRows);
  mpTask->m_pDataStack->vPushS48K(sAnalysis);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.column@ -- get the column from the index                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnFetch(MP_Task mpTask)
{
  ENTRY("vIndexColumnFetch"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.column@"_Stz);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iColumnNo;
  if ( ! mpTask->m_pDataStack->bPop(iColumnNo) )
    THROW_INVALID_DSTACK("index.column@"_Stz);
  
  ASSERT(iColumnNo < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iColumnNo);
  
  mpTask->m_pDataStack->vPushObject(pColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnCount@ -- fetch index column count                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnCountFetch(MP_Task mpTask)
{
  ENTRY("vIndexColumnCountFetch"_Stz);
  
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
void Step::vIndexColumnName(MP_Task mpTask)
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
void Step::vIndexColumnType(MP_Task mpTask)
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
/* index.compact -- index compaction                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexCompact(MP_Task mpTask)
{
  ENTRY("vIndexCompact"_Stz);
  
  Object* pPtrIndex;
  i8 iDetailed;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.compact"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndex->pTable()->vCompactIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.createColumnVecHidden -- create column vector for hidden index       */
/*----------------------------------------------------------------------------*/
void Step::vIndexCreateColumnVecHidden(MP_Task mpTask)
{
  ENTRY("vIndexCreateColumnVecHidden"_Stz);
  
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
/* index.createColumnVecR -- create column vector from job_createIndex        */
/*----------------------------------------------------------------------------*/
void Step::vIndexCreateColumnVec(MP_Task mpTask)
{
  ENTRY("vIndexCreateColumnVec"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Stz);
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("index.createColumnVec"_Stz);
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pPtrJob);
  ASSERT(CN_JOB_CREATEINDEX == pJob_CreateIndex->eClass());
  
  pIndex->vCreateColumnVec(pJob_CreateIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.dump -- dump the index                                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexDump(MP_Task mpTask)
{
  ENTRY("vIndexDump"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.dump"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlots; iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
    vTrace("Partition %lld\n"_Stz, iPartition);
    
    if (pIndex->bIsHidden())
      pIndexTree->vDump(true);
    else
      pIndexTree->vDump(false);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.exactKey -- return exact key matches from index                      */
/*----------------------------------------------------------------------------*/
void Step::vIndexExactKey(MP_Task mpTask)
{
  ENTRY("vIndexExactKey"_Stz);
  
  Object* pPtrIndex = nullptr;
  Entry* pEntry     = nullptr;
  
  GenVar xValWrk;
  Session* pSession = mpTask->m_pSession;
  FetchBuffer* pFetchBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.exactKey"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("index.exactKey"_Stz);
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the key
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.exactKey"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Job_FetchRows* pJob_FetchRows = pJob_SubFetch->m_pJobFetchRows;
  i8 iSlot = pJob_SubFetch->m_iSlot;
  Planner* pPlanner = pJob_SubFetch->m_pPlanner;
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  i8 iPhrase = pJob_SubFetch->m_iPhrase;

  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the key
  KeyWrk xKeyWrk;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  xKeyWrk.vAddDatum(xValWrk,pColumn0,pFormatters);
  //xKeyWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position at a level 0 buffer for given key
  EntryBuffer* pIndexBuffer = pIndexTree->pPositionLevel(xKeyWrk, 1, JC_AFTER, 0);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an index buffer ?
  if (pIndexBuffer) {
    // position in the index buffer at the key
    eRO eRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
    pEntry = pIndexBuffer->pPositionQuery(xKeyWrk.aKey(), eRelnOp);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry ?
  if ( ! pEntry) {
    goto LastBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffer till no more entries
  while (pEntry) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if entry matches key
    bool bIsEqual = false;
    bool bSame = pEntry->aKey()->bJuxtaCompare(JC_SAME, xKeyWrk.aKey(),
                                               pIndex->pIdxColVec(), 1, bIsEqual);
    
    if (bSame) {
      
      // get the dataRow from the hidden index
      Entry* pDataRowEntry = pEntry->pDataEntry();
      Row* aRow = pDataRowEntry->aRow();
    
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // insert row from entry in tempEntryBuffer
      if ( ! pFetchBuffer->bAppendRow(aRow)) {
        
        // Buffer is Full
        // put this FetchBuffer in session bufferQueue
        xValWrk = (Object*)pFetchBuffer;
        pQueue->bAppend(xValWrk.pNestedValue());
        
        // create another fetchBuffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in fetchBuffer
        bool bResult = Task::bAppendRow(pFetchBuffer, pDataRowEntry, pJob_FetchRows);
        RASSERT(bResult);
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move to next entry in the buffer
    pEntry = pIndexBuffer->pNextEntry(pEntry, 0);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    if ( ! pEntry ) {
      // get next buffer from the current indexBuffer
      pIndexBuffer = pIndexBuffer->pNextBuffer();
      
      if ( pIndexBuffer )
        pEntry = pIndexBuffer->pFirstEntry();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    if (pSession->m_bSessionExiting)
      pEntry = nullptr;
    
  }
  
LastBuffer:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  xValWrk = (Object*)pFetchBuffer;
  pQueue->bAppend(xValWrk.pNestedValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.init -- initialize the index                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexInit(MP_Task mpTask)
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
void Step::vIndexIsHiddenFetch(MP_Task mpTask)
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
void Step::vIndexIsHiddenStore(MP_Task mpTask)
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
void Step::vIndexIsPrimaryKeyFetch(MP_Task mpTask)
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
void Step::vIndexIsPrimaryKeyStore(MP_Task mpTask)
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
void Step::vIndexIsUniqueFetch(MP_Task mpTask)
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
void Step::vIndexIsUniqueStore(MP_Task mpTask)
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
void Step::vIndexIsValidUniqueIndex(MP_Task mpTask)
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
/* index.lowerBound -- return rows with lower bound                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexLowerBound(MP_Task mpTask)
{
  ENTRY("vIndexLowerBound"_Stz);
  
  Session* pSession = mpTask->m_pSession;
  Object* pPtrIndex = nullptr;
  FetchBuffer* pFetchBuffer;
  
  GenVar xValWrk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.lowerBound"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("index.lowerBound"_Stz);
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the key
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.lowerBound"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Job_FetchRows* pJob_FetchRows = pJob_SubFetch->m_pJobFetchRows;
  i8 iSlot = pJob_SubFetch->m_iSlot;
  i8 iPhrase = pJob_SubFetch->m_iPhrase;
  Planner* pPlanner = pJob_SubFetch->m_pPlanner;
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  
  EntryBuffer* pLowerEntryBuffer;
  Entry* pLowerEntry = nullptr;
  eRO eLowerRelnOp;
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  TablePtn* pPartition = pTable->pPartition(iSlot);
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  KeyWrk xKeyWrkLower;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  xKeyWrkLower.vAddDatum(xValWrk,pColumn0,pFormatters);
  //xKeyWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  pLowerEntryBuffer = pIndexTree->pPositionLevel(xKeyWrkLower, 1, JC_AFTER, 0);
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (pLowerEntryBuffer) {
    // position in the index buffer at the lower key
    pLowerEntry = pLowerEntryBuffer->pPositionQuery(xKeyWrkLower.aKey(), eLowerRelnOp);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // loop till we get an entry
  while ( ! pLowerEntry) {
    if (pLowerEntryBuffer && pLowerEntryBuffer->pNextBuffer()) {
      
      // move to the next buffer
      pLowerEntryBuffer = pLowerEntryBuffer->pNextBuffer();
      
      // position in the index buffer at the lower key
      pLowerEntry = pLowerEntryBuffer->pPositionQuery(xKeyWrkLower.aKey(), eLowerRelnOp);
    }
    else
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get a lower entry ?
  if ( ! pLowerEntry) {
    goto LastBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffer till no more entries
  while (pLowerEntry) {
    
    // get the dataRow from the hidden index
    Entry* pDataRowEntry = pLowerEntry->pDataEntry();
    
    // insert row from entry in fetchBuffer
    if ( ! Task::bAppendRow(pFetchBuffer, pDataRowEntry, pJob_FetchRows)) {
      
      // Buffer is Full;
      // put this tempEntryBuffer in session bufferQueue
      xValWrk = (Object*)pFetchBuffer;
      pQueue->bAppend(xValWrk.pNestedValue());
      
      // create another fetchBuffer for the results
      pFetchBuffer = new FetchBuffer(pTable, iSlot);
      
      // insert row from entry in fetchBuffer
      bool bResult = Task::bAppendRow(pFetchBuffer, pDataRowEntry, pJob_FetchRows);
      RASSERT(bResult);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // move to next entry in the buffer
    pLowerEntry = pLowerEntryBuffer->pNextEntry(pLowerEntry, 0);
    
    if ( ! pLowerEntry ) {
      // get next buffer from the current indexBuffer
      pLowerEntryBuffer = pLowerEntryBuffer->pNextBuffer();
      
      if ( pLowerEntryBuffer )
        pLowerEntry = pLowerEntryBuffer->pFirstEntry();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    if (pSession->m_bSessionExiting)
      pLowerEntry = nullptr;
  }
  
LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  xValWrk = (Object*)pFetchBuffer;
  pQueue->bAppend(xValWrk.pNestedValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.name@ -- fetch index name                                            */
/*----------------------------------------------------------------------------*/
void Step::vIndexNameFetch(MP_Task mpTask)
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
void Step::vIndexPopulate(MP_Task mpTask)
{
  ENTRY("vIndexPopulate"_Stz);
  Object* pPtrIndex = nullptr;
  
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
/* index.position -- position at key in an index buffer                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexPosition(MP_Task mpTask)
{
  ENTRY("vIndexPosition"_Stz);
  Object* pPtrIndex = nullptr;
  Object* pPtrPlanner = nullptr;
  Object* pPtrIndexBuffer = nullptr;
  GenVar xValWrk;
  i8 i_Phrase;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("index.position"_Stz);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  ASSERT(CN_PLANNER == pPlanner->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(i_Phrase) )
    THROW_INVALID_DSTACK("index.position"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.position"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndexBuffer) )
    THROW_INVALID_DSTACK("index.position"_Stz);
  
  EntryBuffer* pIndexBuffer = _pEntryBuffer(pPtrIndexBuffer);
  
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.position"_Stz);
  
  KeyWrk xKeyWrk;
  Column* pColumn = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  xKeyWrk.vAddDatum(xValWrk, pColumn, pFormatters);
  //xKeyWrk.aKey()->vShow(pIndex->pIdxColVec());

  // position at key in the entry buffer
  eRO eRelnOp = pPlanner->eGetFirstLowerPredicateRO(i_Phrase);
  Entry* pEntry = pIndexBuffer->pPositionQuery(xKeyWrk.aKey(), eRelnOp);
  
  mpTask->m_pDataStack->vPushEntry(pEntry);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.positionFirst -- position at first index buffer                      */
/*----------------------------------------------------------------------------*/
void Step::vIndexPositionFirst(MP_Task mpTask)
{
  ENTRY("vIndexFirst"_Stz);
  
  Object* pPtrIndex = nullptr;
  GenVar xValWrk;
  i8 iPartition;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.positionFirst"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("index.positionFirst"_Stz);
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  TablePtn* pPartition = pTable->pPartition(iPartition);
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  mpTask->m_pDataStack->vPushObject(pIndexTree->pFirstL0Buffer());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.range -- return rows within given range                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexRange(MP_Task mpTask)
{
  ENTRY("vIndexRange"_Stz);
  
  Object* pPtrIndex = nullptr;
  Session* pSession = mpTask->m_pSession;
  
  GenVar xValWrkLwr;
  GenVar xValWrkUpr;
  GenVar xValWrk;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.range"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("index.exactKey"_Stz);
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the lower key
  if ( ! mpTask->m_pDataStack->bPop(xValWrkLwr) )
    THROW_INVALID_DSTACK("index.range"_Stz);
  
  // get the upper key
  if ( ! mpTask->m_pDataStack->bPop(xValWrkUpr) )
    THROW_INVALID_DSTACK("index.range"_Stz);
  
  FetchBuffer* pFetchBuffer;
  EntryBuffer* pLowerEntryBuffer;
  EntryBuffer* pUpperEntryBuffer;
  
  Entry* pLowerEntry = nullptr;
  Entry* pUpperEntry = nullptr;
  
  eRO eUpperRelnOp;
  eRO eLowerRelnOp;
  
  bool bBefore;
  bool bIsEqual;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Job_FetchRows* pJob_FetchRows = pJob_SubFetch->m_pJobFetchRows;
  i8 iSlot = pJob_SubFetch->m_iSlot;
  i8 iPhrase = pJob_SubFetch->m_iPhrase;
  Planner* pPlanner = pJob_SubFetch->m_pPlanner;
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  TablePtn* pPartition = pTable->pPartition(iSlot);
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a fetchBuffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the lower key
  KeyWrk xKeyWrkLower;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  xKeyWrkLower.vAddDatum(xValWrkLwr,pColumn0,pFormatters);
  //xKeyWrkLower.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the upper key
  KeyWrk xKeyWrkUpper;
  pColumn0 = pIndex->pIdxColVec()->pGet(0);
  xKeyWrkUpper.vAddDatum(xValWrkUpr,pColumn0,pFormatters);
  //xKeyWrkUpper.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if upperKey > lowerKey
  bIsEqual = false;
  bBefore = xKeyWrkUpper.aKey()->bJuxtaCompare(JC_BEFORE, xKeyWrkLower.aKey(),
                                               pIndex->pIdxColVec(), 1,
                                               bIsEqual, RO_LESS_THAN);
  if (bBefore) {
    goto LastBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  pLowerEntryBuffer = pIndexTree->pPositionLevel(xKeyWrkLower, 1, JC_AFTER, 0);
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (pLowerEntryBuffer) {
    // position in the index buffer at the lower key
    pLowerEntry = pLowerEntryBuffer->pPositionQuery(xKeyWrkLower.aKey(), eLowerRelnOp);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // loop till we get an entry
  while ( ! pLowerEntry) {
    if (pLowerEntryBuffer && pLowerEntryBuffer->pNextBuffer()) {
      
      // move to the next buffer
      pLowerEntryBuffer = pLowerEntryBuffer->pNextBuffer();
      
      // position in the index buffer at the lower key
      pLowerEntry = pLowerEntryBuffer->pPositionQuery(xKeyWrkLower.aKey(), eLowerRelnOp);
    }
    else
      break;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we find a lower entry ?
  if (! pLowerEntry )
    goto LastBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer with keyUpper
  pUpperEntryBuffer = pIndexTree->pPositionLevel(xKeyWrkUpper, 1, JC_AFTER, 0);
  eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  
  //pUpperEntryBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (pUpperEntryBuffer) {
    pUpperEntry = pUpperEntryBuffer->pPositionQuery(xKeyWrkUpper.aKey(), eUpperRelnOp);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // loop till we get an upper entry
  while ( ! pUpperEntry) {
    if ( pUpperEntryBuffer && pUpperEntryBuffer->pNextBuffer() ) {
      
      // move to the next buffer
      pUpperEntryBuffer = pUpperEntryBuffer->pNextBuffer();
      
      // position in the index buffer at the upper key
      pUpperEntry = pUpperEntryBuffer->pPositionQuery(xKeyWrkUpper.aKey(), eUpperRelnOp);
    }
    else
      break;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now find the correct upper entry
  while (pUpperEntry) {
    
    //pUpperEntry->vShow(0,pIndex,true);
    
    bool bIsEqual = false;
    bool bBefore = pUpperEntry->aKey()->bJuxtaCompare(JC_BEFORE, xKeyWrkUpper.aKey(),
                                                 pIndex->pIdxColVec(), 1,
                                                 bIsEqual, eUpperRelnOp);
    
    if (! bBefore)
      break;
    
    // move to next entry in the buffer
    pUpperEntry = pUpperEntryBuffer->pNextEntry(pUpperEntry, 0);
    
    if ( ! pUpperEntry ) {
      // get next buffer from the current indexBuffer
      pUpperEntryBuffer = pUpperEntryBuffer->pNextBuffer();
      
      if ( pUpperEntryBuffer )
        pUpperEntry = pUpperEntryBuffer->pFirstEntry();
    }
  }
  
  //pUpperEntry->vShow(0,pIndex,true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffers till no more entries starting from
  while (pLowerEntry) {
    
    // check against the upper bound
    bIsEqual = false;
    
    //bool bBefore = pEntry->aKey()->bJuxtaCompare(JC_BEFORE, xKeyWrkUpper.aKey(),
    //                                             pIndex->pIdxColVec(), 1,
    //                                             bIsEqual, eRelnOp);
    
    bBefore = (pLowerEntry == pUpperEntry) ? false : true;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // if pLowerEntry is before pUpperEntry then append to result buffer
    if (bBefore) {
      // get the dataRow from the hidden index
      Entry* pDataRowEntry = pLowerEntry->pDataEntry();
      
      // insert row from entry in tempEntryBuffer
      if ( ! Task::bAppendRow(pFetchBuffer, pDataRowEntry, pJob_FetchRows)) {
        
        // Buffer is Full
        // put this fetchBuffer in session bufferQueue
        xValWrk = (Object*)pFetchBuffer;
        pQueue->bAppend(xValWrk.pNestedValue());

        // create another fetchBbuffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in temporary buffer
        bool bResult = Task::bAppendRow(pFetchBuffer, pDataRowEntry, pJob_FetchRows);
        RASSERT(bResult);
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      pLowerEntry = pLowerEntryBuffer->pNextEntry(pLowerEntry, 0);
      
      if ( ! pLowerEntry ) {
        // get next buffer from the current indexBuffer
        pLowerEntryBuffer = pLowerEntryBuffer->pNextBuffer();
        
        if ( pLowerEntryBuffer )
          pLowerEntry = pLowerEntryBuffer->pFirstEntry();
      }
      else {
        // check if session exiting
        if (pSession->m_bSessionExiting)
          pLowerEntry = nullptr;
      }
    }
    else {
      pLowerEntry = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    if (pSession->m_bSessionExiting)
      pLowerEntry = nullptr;
  }

LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  xValWrk = (Object*)pFetchBuffer;
  pQueue->bAppend(xValWrk.pNestedValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.scan -- scan the index                                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexScan(MP_Task mpTask)
{
  ENTRY("vIndexScan"_Stz);
  
  Session* pSession = mpTask->m_pSession;
  Object* pPtrIndex = nullptr;
  FetchBuffer* pFetchBuffer;
  GenVar xValWrk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.scan"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("index.scan"_Stz);
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  i8 iSlot = pJob_SubFetch->m_iSlot;
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position in the first index buffer
  EntryBuffer* pEntryBuffer = pIndexTree->pFirstL0Buffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entryBuffer ?
  if ( ! pEntryBuffer) {
    // mark this buffer as the last buffer
    pFetchBuffer->vIsLast(true);
    
    // put the buffer in the session bufferQueue
    xValWrk = (Object*)pFetchBuffer;
    pQueue->bAppend(xValWrk.pNestedValue());

    EXIT();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first entry in the buffer
  Entry* pEntry = pEntryBuffer->pFirstEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffer till no more entries
  while (pEntry) {
    
    Row* aRow = pEntry->aRow();
    //aRow->vShow(pTable->pColumnVec(), true);
    //vTrace("\n"_Stz);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // insert row from entry in FetchBuffer
    if ( ! pFetchBuffer->bAppendRow(aRow)) {
      
      // Buffer is Full
      // put this FetchBuffer in session bufferQueue
      xValWrk = (Object*)pFetchBuffer;
      pQueue->bAppend(xValWrk.pNestedValue());
      
      // create another FetchBuffer for the results
      pFetchBuffer = new FetchBuffer(pTable, iSlot);
      
      // insert row from entry in new fetchBuffer
      bool bResult = pFetchBuffer->bAppendRow(aRow);
      RASSERT(bResult);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move to next entry in the buffer
    pEntry = pEntryBuffer->pNextEntry(pEntry, 0);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // did we find an entry ?
    if ( ! pEntry ) {
      
      // get next buffer from the current indexBuffer
      pEntryBuffer = pEntryBuffer->pNextBuffer();
      
      if ( pEntryBuffer )
        pEntry = pEntryBuffer->pFirstEntry();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    if (pSession->m_bSessionExiting)
      pEntry = nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  xValWrk = (Object*)pFetchBuffer;
  pQueue->bAppend(xValWrk.pNestedValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.setSingleColumn -- set if single column index                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexSetSingleColumn(MP_Task mpTask)
{
  ENTRY("vIndexSetSingleColumn"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.showColumnVec"_Stz);
  Index* pIndex = _pIndex(pPtrIndex);
  
  pIndex->vSetSingleColumn();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.showColumnVec -- show column vector                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexShowColumnVec(MP_Task mpTask)
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
void Step::vIndexSpace(MP_Task mpTask)
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
void Step::vIndexSubDic(MP_Task mpTask)
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
void Step::vIndexTableFetch(MP_Task mpTask)
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
void Step::vIndexTableStore(MP_Task mpTask)
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
void Step::vIndexTruncate(MP_Task mpTask)
{
  ENTRY("vIndexTruncate"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.truncate"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlots; iSlot++)
    pTable->vTruncateIndex(iSlot, iIndexNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.upper -- check if entry is within upper bound for the index          */
/*----------------------------------------------------------------------------*/
void Step::vIndexUpper(MP_Task mpTask)
{
  ENTRY("vIndexUpper"_Stz);
  
  Object* pPtrPlanner = nullptr;
  Object* pPtrIndex = nullptr;
  Object* pPtrEntry = nullptr;

  GenVar xValWrk;
  i8 iPartition;
  i8 i_Phrase;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlanner) )
    THROW_INVALID_DSTACK("index.upper"_Stz);
  
  Planner* pPlanner = _pPlanner(pPtrPlanner);
  ASSERT(CN_PLANNER == pPlanner->eClass());
  
  if ( ! mpTask->m_pDataStack->bPop(i_Phrase) )
    THROW_INVALID_DSTACK("index.upper"_Stz);
  
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
  
  // assuming non-complex key
  //ASSERT(1 == pIndex->pIdxColVec()->nItems());
  KeyWrk xKeyWrk;
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  xKeyWrk.vAddDatum(xValWrk, pIndex->pIdxColVec()->pGet(0),pFormatters);
  
  eRO eRelnOp = pPlanner->eGetFirstUpperPredicateRO(i_Phrase);
  bool bIsEqual = false;
  bool bBefore = pEntry->aKey()->bJuxtaCompare(JC_BEFORE, xKeyWrk.aKey(),
                                               pIndex->pIdxColVec(), 1,
                                               bIsEqual, eRelnOp);
  mpTask->m_pDataStack->vPushInt(bBefore ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.upperBound -- return rows with upper bound                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexUpperBound(MP_Task mpTask)
{
  ENTRY("vIndexUpperBound"_Stz);
  
  Object* pPtrIndex = nullptr;
  Session* pSession = mpTask->m_pSession;
  GenVar xValWrk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.upperBound"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("index.exactKey"_Stz);
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the key
  if ( ! mpTask->m_pDataStack->bPop(xValWrk) )
    THROW_INVALID_DSTACK("index.upperBound"_Stz);
  
  FetchBuffer* pFetchBuffer;
  EntryBuffer* pEntryBuffer;
  EntryBuffer* pUpperEntryBuffer;
  
  Entry* pUpperEntry = nullptr;
  Entry* pEntry      = nullptr;
  
  eRO eUpperRelnOp;
  bool bBefore;
  bool bIsEqual;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Job_FetchRows* pJob_FetchRows = pJob_SubFetch->m_pJobFetchRows;
  i8 iSlot = pJob_SubFetch->m_iSlot;
  i8 iPhrase = pJob_SubFetch->m_iPhrase;
  Planner* pPlanner = pJob_SubFetch->m_pPlanner;
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  TablePtn* pPartition = pTable->pPartition(iSlot);
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the upper key
  KeyWrk xKeyWrkUpper;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = &gpAttachMap->m_xFormatters;
  xKeyWrkUpper.vAddDatum(xValWrk,pColumn0,pFormatters);
  //xKeyWrkUpper.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at the first level 0 buffer
  pEntryBuffer = pIndexTree->pFirstL0Buffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entryBuffer ?
  if ( ! pEntryBuffer) {
    goto LastBuffer;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first entry in the buffer
  pEntry = pEntryBuffer->pFirstEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer for the upper key
  pUpperEntryBuffer = pIndexTree->pPositionLevel(xKeyWrkUpper, 1, JC_AFTER, 0);
  eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (pUpperEntryBuffer) {
    pUpperEntry = pUpperEntryBuffer->pPositionQuery(xKeyWrkUpper.aKey(), eUpperRelnOp);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // loop till we get an upper entry
  while ( ! pUpperEntry) {
    if ( pUpperEntryBuffer && pUpperEntryBuffer->pNextBuffer() ) {
      
      // move to the next buffer
      pUpperEntryBuffer = pUpperEntryBuffer->pNextBuffer();
      
      // position in the index buffer at the upper key
      pUpperEntry = pUpperEntryBuffer->pPositionQuery(xKeyWrkUpper.aKey(), eUpperRelnOp);
    }
    else
      break;
  }
  
  //pUpperEntryBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now find the correct upper entry
  while (pUpperEntry) {
    
    //pUpperEntry->vShow(0,pIndex,true);
    
    bIsEqual = false;
    bBefore = pUpperEntry->aKey()->bJuxtaCompare(JC_BEFORE, xKeyWrkUpper.aKey(),
                                                 pIdxColVec, 1,
                                                 bIsEqual, eUpperRelnOp);
    
    if (! bBefore) {
      break;
    }
    
    // move to next entry in the buffer
    pUpperEntry = pUpperEntryBuffer->pNextEntry(pUpperEntry, 0);
    
    if ( ! pUpperEntry ) {
      // get next buffer from the current indexBuffer
      pUpperEntryBuffer = pUpperEntryBuffer->pNextBuffer();
      
      if ( pUpperEntryBuffer )
        pUpperEntry = pUpperEntryBuffer->pFirstEntry();
    }
  }
  
  //pUpperEntry->vShow(0,pIndex,true);
  
  //---------------------------------------------------------------------------
  // iterate thru the buffers till no more entries starting from
  while (pEntry) {
    
    bIsEqual = false;

    bBefore = (pEntry == pUpperEntry) ? false : true;
    
    if (bBefore) {
      // get the dataRow from the hidden index
      Entry* pDataRowEntry = pEntry->pDataEntry();
      
      // insert row from entry in tempEntryBuffer
      if ( ! Task::bAppendRow(pFetchBuffer, pDataRowEntry, pJob_FetchRows)) {
        
        // Buffer is full
        // put this tempEntryBuffer in session bufferQueue
        xValWrk = (Object*)pFetchBuffer;
        pQueue->bAppend(xValWrk.pNestedValue());
        
        // create another result buffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in temporary buffer
        bool bResult = Task::bAppendRow(pFetchBuffer, pDataRowEntry, pJob_FetchRows);
        RASSERT(bResult);
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      pEntry = pEntryBuffer->pNextEntry(pEntry, 0);
      
      if ( ! pEntry ) {
        // get next buffer from the current indexBuffer
        pEntryBuffer = pEntryBuffer->pNextBuffer();
        
        if ( pEntryBuffer )
          pEntry = pEntryBuffer->pFirstEntry();
      }
    }
    else {
      pEntry = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    if (pSession->m_bSessionExiting)
      pEntry = nullptr;
  }
  
LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  xValWrk = (Object*)pFetchBuffer;
  pQueue->bAppend(xValWrk.pNestedValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.verify -- verify index                                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexVerify(MP_Task mpTask)
{
  ENTRY("vIndexVerify"_Stz);
  
  Object* pPtrIndex;
  
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
void Step::vIndexVisualize(MP_Task mpTask)
{
  ENTRY("vIndexVisualize"_Stz);
  
  Object* pPtrIndex;
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.visualize"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlots; iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
    vTrace("Table Partition %lld\n"_Stz, iPartition);
    pIndexTree->vVisualize();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*    PRIVATE                                                                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append a row to a working buffer                                           */
/*----------------------------------------------------------------------------*/
bool Task::bAppendRow(FetchBuffer* pFetchBuffer, Entry* pEntry, Job_FetchRows* pJob_FetchRows)
{
  
  Id xId = pEntry->xId();
  
  if (pJob_FetchRows && pJob_FetchRows->bRowSeen(xId)) {
    // row already inserted
    //vTrace("Row already inserted %p \n"_Stz, pEntry);
    return true;
  }
  
  Row* aRow = pEntry->aRow();
  
  if (pFetchBuffer->bAppendRow(aRow))
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
