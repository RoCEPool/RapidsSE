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
  vAddStep("index.createColumnVec"_Stz, &Step::vIndexCreateColumnVec);
  vAddStep("index.dump"_Stz,            &Step::vIndexDump);
  vAddStep("index.init"_Stz,            &Step::vIndexInit);
  vAddStep("index.isPrimaryKey@"_Stz,   &Step::vIndexIsPrimaryKeyFetch);
  vAddStep("index.isPrimaryKey!"_Stz,   &Step::vIndexIsPrimaryKeyStore);
  vAddStep("index.isUnique@"_Stz,       &Step::vIndexIsUniqueFetch);
  vAddStep("index.isUnique!"_Stz,       &Step::vIndexIsUniqueStore);
  vAddStep("index.isValidUniqueIndex"_Stz,&Step::vIndexIsValidUniqueIndex);
  vAddStep("index.lowerBound"_Stz,      &Step::vIndexLowerBound);
  vAddStep("index.name@"_Stz,           &Step::vIndexNameFetch);
  vAddStep("index.populate"_Stz,        &Step::vIndexPopulate);
  vAddStep("index.range"_Stz,           &Step::vIndexRange);
  vAddStep("index.showColumnVec"_Stz,   &Step::vIndexShowColumnVec);
  vAddStep("index.space"_Stz,           &Step::vIndexSpace);
  vAddStep("index.subDic"_Stz,          &Step::vIndexSubDic);
  vAddStep("index.table@"_Stz,          &Step::vIndexTableFetch);
  vAddStep("index.table!"_Stz,          &Step::vIndexTableStore);
  vAddStep("index.truncate"_Stz,        &Step::vIndexTruncate);
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
  
  mpTask->m_pDataStack->vPushPointer(pColumn);
  
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
  
  mpTask->m_pDataStack->vPushI8(pIndex->pIdxColVec()->nItems());
  
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
  s32 sColumnType = aCT[pColumn->eColumnType()];
  
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
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlotsPerNode; iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
    vTrace("Partition %lld\n"_Stz, iPartition);
    
    pIndexTree->vDump(false);
  }
  
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
  
  mpTask->m_pDataStack->vPushI8(bIsPrimaryKey);
  
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
  
  mpTask->m_pDataStack->vPushI8(bIsUnique);
  
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
  
  mpTask->m_pDataStack->vPushI8(bValid ? 1 : 0);
  
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
  
  GenVar xVariable;
  
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
  if ( ! mpTask->m_pDataStack->bPop(xVariable) )
    THROW_INVALID_DSTACK("index.lowerBound"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob_SubFetch->m_pParentJob);
  i8 iSlot = pJob_SubFetch->m_i_Slot;
  i8 iPhrase = pJob_SubFetch->m_iPhrase;
  Planner* pPlanner = pJob_SubFetch->m_pPlanner;
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  
  EntBuffer* aLowerEntBuffer;
  Ent* aLowerEntry = nullptr;
  eRO eLowerRelnOp;
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  TablePtn* pPartition = pTable->pPartition(iSlot);
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of columns in the index
  i8 nIdxCols = pIndex->pIdxColVec()->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  KeyWrk xKeyWrkLower;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = &gpAttachMap->m_oFormatters;
  StrVal* aStrVal = _aStrVal(xVariable.aNestedStrVal());
  
  xKeyWrkLower.vAddDatum(aStrVal->aStz(),pColumn0,pFormatters);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xKeyWrkLower.vAddMinusInfinity();
  }
  //xKeyWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xKeyWrkLower);
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aLowerEntBuffer) {
    // position in the index buffer at an entry prior to the lower key
    //aLowerEntry = aLowerEntBuffer->aPositionQuery(xKeyWrkLower, eLowerRelnOp);
    
    aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xKeyWrkLower);
    
    if (eLowerRelnOp == RO_GREATER_THAN) {
      aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry, xKeyWrkLower, &aLowerEntBuffer);
    }
    else {
      RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
      aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xKeyWrkLower, &aLowerEntBuffer);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get a lower entry ?
  if ( ! aLowerEntry) {
    goto LastBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffer till no more entries
  while (aLowerEntry) {
    
    // insert row from entry in fetchBuffer
    if ( ! Task::bAppendRow(pFetchBuffer, aLowerEntry, pJob_FetchRows)) {
      
      // Buffer is Full;
      // put this tempEntBuffer in session bufferQueue
      pQueue->vAppend(pFetchBuffer);
      
      // create another fetchBuffer for the results
      pFetchBuffer = new FetchBuffer(pTable, iSlot);
      
      // insert row from entry in fetchBuffer
      bool bResult = Task::bAppendRow(pFetchBuffer, aLowerEntry, pJob_FetchRows);
      RASSERT(bResult);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move to next entry in the buffer
    aLowerEntry = aLowerEntBuffer->aNextEntry(aLowerEntry, 0);
    
    if ( ! aLowerEntry ) {
      // get next buffer from the current indexBuffer
      aLowerEntBuffer = aLowerEntBuffer->pNextBuffer();
      
      if ( aLowerEntBuffer )
        aLowerEntry = aLowerEntBuffer->aLeadEntry();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    if (pSession->m_bSessionExiting)
      aLowerEntry = nullptr;
  }
  
LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  pQueue->vAppend(pFetchBuffer);
  
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
  
  s32 sIndexName = pIndex->sIndexName();
  
  mpTask->m_pDataStack->vPushS32(sIndexName);
  
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
  
  bool bPopulated = pTable->bPopulateIndex(iIndexNo);
  
  mpTask->m_pDataStack->vPushI8(bPopulated ? 1 : 0);
  
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.range"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("index.range"_Stz);
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the lower key
  if ( ! mpTask->m_pDataStack->bPop(xValWrkLwr) )
    THROW_INVALID_DSTACK("index.range"_Stz);
  
  // get the upper key
  if ( ! mpTask->m_pDataStack->bPop(xValWrkUpr) )
    THROW_INVALID_DSTACK("index.range"_Stz);
  
  FetchBuffer* pFetchBuffer;
  EntBuffer* aLowerEntBuffer;
  EntBuffer* aUpperEntBuffer;
  
  Ent* aLowerEntry = nullptr;
  Ent* aUpperEntry = nullptr;
  
  eRO eUpperRelnOp;
  eRO eLowerRelnOp;
  
  bool bBefore;
  bool bIsEqual;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob_SubFetch->m_pParentJob);
  i8 iSlot = pJob_SubFetch->m_i_Slot;
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
  // get no of columns in the index
  i8 nIdxCols = pIndex->pIdxColVec()->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the lower key
  KeyWrk xKeyWrkLower;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = &gpAttachMap->m_oFormatters;
  StrVal* aStrVal = _aStrVal(xValWrkLwr.aNestedStrVal());
  
  xKeyWrkLower.vAddDatum(aStrVal->aStz(),pColumn0,pFormatters);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xKeyWrkLower.vAddMinusInfinity();
  }
  //xKeyWrkLower.vShow(pIndex->pIdxColVec());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the upper key
  KeyWrk xKeyWrkUpper;
  pColumn0 = pIndex->pIdxColVec()->pGet(0);
  aStrVal = _aStrVal(xValWrkUpr.aNestedStrVal());
  
  xKeyWrkUpper.vAddDatum(aStrVal->aStz(),pColumn0,pFormatters);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xKeyWrkUpper.vAddPlusInfinity();
  }
  //xKeyWrkUpper.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if upperKey > lowerKey
  eKC eCompare = Key::eCompare(xKeyWrkUpper, xKeyWrkLower);
  
  if (eCompare == KC_BEFORE) {
    goto LastBuffer;
  }
  
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  if (eLowerRelnOp == RO_EQUAL_TO) {
    eLowerRelnOp = RO_GREATER_EQUAL;
    eUpperRelnOp = RO_LESS_EQUAL;
  }
  else
    eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xKeyWrkLower);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aLowerEntBuffer) {
    // position in the index buffer at the lower key
    aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xKeyWrkLower);
    
    if (eLowerRelnOp == RO_GREATER_THAN) {
      aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry, xKeyWrkLower, &aLowerEntBuffer);
    }
    else {
      RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
      aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xKeyWrkLower, &aLowerEntBuffer);
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we find a lower entry ?
  if (! aLowerEntry )
    goto LastBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer with keyUpper
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xKeyWrkUpper);
  
  //aUpperEntBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aUpperEntBuffer) {
    //aUpperEntry = aUpperEntBuffer->aPositionQuery(xKeyWrkUpper, eUpperRelnOp);
    aUpperEntry = aUpperEntBuffer->pPositionPriorEntry(xKeyWrkUpper);
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, xKeyWrkUpper, &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry, xKeyWrkUpper, &aUpperEntBuffer);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffers till no more entries starting from
  while (aLowerEntry) {
    
    // check against the upper bound
    bIsEqual = false;
    
    //bool bBefore = aItem->aKey()->bJuxtaCompare(JC_BEFORE, xKeyWrkUpper.aKey(),
    //                                             pIndex->pIdxColVec(), 1,
    //                                             bIsEqual, eRelnOp);
    
    bBefore = (aLowerEntry == aUpperEntry) ? false : true;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // if aLowerEntry is before aUpperEntry then append to result buffer
    if (bBefore) {
      
      // insert row from entry in tempEntBuffer
      if ( ! Task::bAppendRow(pFetchBuffer, aLowerEntry, pJob_FetchRows)) {
        
        // Buffer is Full
        // put this fetchBuffer in session bufferQueue
        pQueue->vAppend(pFetchBuffer);
        
        // create another fetchBbuffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in temporary buffer
        bool bResult = Task::bAppendRow(pFetchBuffer, aLowerEntry, pJob_FetchRows);
        RASSERT(bResult);
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      aLowerEntry = aLowerEntBuffer->aNextEntry(aLowerEntry, 0);
      
      if ( ! aLowerEntry ) {
        // get next buffer from the current indexBuffer
        aLowerEntBuffer = aLowerEntBuffer->pNextBuffer();
        
        if ( aLowerEntBuffer )
          aLowerEntry = aLowerEntBuffer->aLeadEntry();
      }
      else {
        // check if session exiting
        if (pSession->m_bSessionExiting)
          aLowerEntry = nullptr;
      }
    }
    else {
      aLowerEntry = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    if (pSession->m_bSessionExiting)
      aLowerEntry = nullptr;
  }
  
LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  pQueue->vAppend(pFetchBuffer);
  
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
  mpTask->m_pDataStack->vPushI8(iSpace);
  
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
  
  mpTask->m_pDataStack->vPushPointer(pDic);
  
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
  
  mpTask->m_pDataStack->vPushPointer(pTable);
  
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
  
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlotsPerNode; iSlot++)
    pTable->vTruncateIndex(iSlot, iIndexNo);
  
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
  GenVar xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! mpTask->m_pDataStack->bPop(pPtrIndex) )
    THROW_INVALID_DSTACK("index.upperBound"_Stz);
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob) )
    THROW_INVALID_DSTACK("index.upperBound"_Stz);
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the key
  if ( ! mpTask->m_pDataStack->bPop(xVariable) )
    THROW_INVALID_DSTACK("index.upperBound"_Stz);
  
  FetchBuffer* pFetchBuffer;
  EntBuffer* pEntBuffer;
  EntBuffer* aUpperEntBuffer;
  
  Ent* aUpperEntry = nullptr;
  Ent* aItem      = nullptr;
  
  eRO eUpperRelnOp;
  bool bBefore;
  bool bIsEqual;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob_SubFetch->m_pParentJob);
  i8 iSlot = pJob_SubFetch->m_i_Slot;
  i8 iPhrase = pJob_SubFetch->m_iPhrase;
  Planner* pPlanner = pJob_SubFetch->m_pPlanner;
  Queue* pQueue = pJob_SubFetch->m_pBufferQueue;
  
  //IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  TablePtn* pPartition = pTable->pPartition(iSlot);
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pFetchBuffer = new FetchBuffer(pTable, iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of columns in the index
  i8 nIdxCols = pIndex->pIdxColVec()->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the upper key
  KeyWrk xKeyWrkUpper;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = &gpAttachMap->m_oFormatters;
  StrVal* aStrVal = _aStrVal(xVariable.aNestedStrVal());
  
  xKeyWrkUpper.vAddDatum(aStrVal->aStz(),pColumn0,pFormatters);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xKeyWrkUpper.vAddPlusInfinity();
  }
  //xKeyWrkUpper.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at the first level 0 buffer
  pEntBuffer = pIndexTree->pFirstH0Buffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entryBuffer ?
  if ( ! pEntBuffer) {
    goto LastBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first entry in the buffer
  aItem = pEntBuffer->aLeadEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer for the upper key
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xKeyWrkUpper);
  
  eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  
  if (aUpperEntBuffer) {
    
    aUpperEntry = aUpperEntBuffer->pPositionPriorEntry(xKeyWrkUpper);
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, xKeyWrkUpper, &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry, xKeyWrkUpper, &aUpperEntBuffer);
    }
  }
  
  //aUpperEntry->vShow(0,pIndex,true);
  
  //---------------------------------------------------------------------------
  // iterate thru the buffers till no more entries starting from
  while (aItem) {
    
    bIsEqual = false;
    
    bBefore = (aItem == aUpperEntry) ? false : true;
    
    if (bBefore) {
      
      // insert row from entry in tempEntBuffer
      if ( ! Task::bAppendRow(pFetchBuffer,aItem, pJob_FetchRows)) {
        
        // Buffer is full
        // put this tempEntBuffer in session bufferQueue
        pQueue->vAppend(pFetchBuffer);
        
        // create another result buffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in temporary buffer
        bool bResult = Task::bAppendRow(pFetchBuffer, aItem, pJob_FetchRows);
        RASSERT(bResult);
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      aItem = pEntBuffer->aNextEntry(aItem, 0);
      
      if ( ! aItem ) {
        // get next buffer from the current indexBuffer
        pEntBuffer = pEntBuffer->pNextBuffer();
        
        if ( pEntBuffer )
          aItem = pEntBuffer->aLeadEntry();
      }
    }
    else {
      aItem = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    if (pSession->m_bSessionExiting)
      aItem = nullptr;
  }
  
LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  pQueue->vAppend(pFetchBuffer);
  
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
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlotsPerNode; iPartition++) {
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
bool Task::bAppendRow(FetchBuffer* pFetchBuffer, Ent* aItem, Job_FetchRows* pJob_FetchRows)
{
  
  Id xId = aItem->xId();
  
  if (pJob_FetchRows && pJob_FetchRows->bRowSeen(xId)) {
    // row already inserted
    //vTrace("RowData already inserted %p \n"_Stz, aItem);
    return true;
  }
  
  // get the rowBuffer
  RowBuffer* pRowBuffer = RowBuffer::pRefdRowBuffer(xId);
  
  // get the dataRow
  HeadLine* aRowData = pRowBuffer->pRow(xId.iItem());

  if (pFetchBuffer->bAppendRow(aRowData))
    return true;
  else
    return false;
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
