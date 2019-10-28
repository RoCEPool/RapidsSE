/*============================================================================*/
/* Steps_Index.cpp - index step group                                         */
/*============================================================================*/

#include "Steps.h"

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
void Step::vIndexAnalyze(void)
{
  ENTRY("vIndexAnalyze"_Stz);
  
  Object* pPtrIndex;
  i8 iDetailed;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! m_pDataStack->bPopI8(iDetailed) )
    THROW_DSTACK();
  
  s48K sAnalysis;
  i8 nRows;
  sAnalysis = (Stz*)pIndex->pTable()->pAnalyzeIndex(pIndex, iDetailed == 1 ? true : false, nRows);
  m_pDataStack->vPushS48K(sAnalysis);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.column@ -- get the column from the index                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnFetch(void)
{
  ENTRY("vIndexColumnFetch"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iColumnNo;
  if ( ! m_pDataStack->bPopI8(iColumnNo) )
    THROW_DSTACK();
  
  ASSERT(iColumnNo < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iColumnNo);
  
  m_pDataStack->vPushPointer(pColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnCount@ -- fetch index column count                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnCountFetch(void)
{
  ENTRY("vIndexColumnCountFetch"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  m_pDataStack->vPushI8(pIndex->pIdxColVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnName -- get column name                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnName(void)
{
  ENTRY("vIndexColumnName"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex;
  if ( ! m_pDataStack->bPopI8(iIndex) )
    THROW_DSTACK();
  
  ASSERT(iIndex < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iIndex);
  
  m_pDataStack->vPushS32(pColumn->sColumnName());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnType -- get column type                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnType(void)
{
  ENTRY("vIndexColumnType"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex;
  if ( ! m_pDataStack->bPopI8(iIndex) )
    THROW_DSTACK();
  
  ASSERT(iIndex < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iIndex);
  s32 sColumnType = aCT[pColumn->eColumnType()];
  
  m_pDataStack->vPushS32(sColumnType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.compact -- index compaction                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexCompact(void)
{
  ENTRY("vIndexCompact"_Stz);
  
  Object* pPtrIndex;
  i8 iDetailed;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndex->pTable()->vCompactIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.createColumnVecR -- create column vector from job_createIndex        */
/*----------------------------------------------------------------------------*/
void Step::vIndexCreateColumnVec(void)
{
  ENTRY("vIndexCreateColumnVec"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! m_pDataStack->bPopObject(pPtrJob) )
    THROW_DSTACK();
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pPtrJob);
  ASSERT(CN_JOB_CREATEINDEX == pJob_CreateIndex->eClass());
  
  pIndex->vCreateColumnVec(pJob_CreateIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.dump -- dump the index                                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexDump(void)
{
  ENTRY("vIndexDump"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < AM::pGrid()->nSlotsPerNode(); iPartition++) {
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
void Step::vIndexInit(void)
{
  ENTRY("vIndexInit"_Stz);
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  new(pIndex) Index;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey@ -- fetch isPrimaryKey from index                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsPrimaryKeyFetch(void)
{
  ENTRY("vIndexIsPrimaryKeyFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bIsPrimaryKey = pIndex->bIsPrimaryKey();
  
  m_pDataStack->vPushI8(bIsPrimaryKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey! -- store isPrimaryKey into index                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsPrimaryKeyStore(void)
{
  ENTRY("vIndexIsPrimaryKeyStore"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsPrimaryKey;
  if ( ! m_pDataStack->bPopI8(iIsPrimaryKey) )
    THROW_DSTACK();
  
  pIndex->vIsPrimaryKey(iIsPrimaryKey ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isUnique@ -- fetch isUnique from index                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsUniqueFetch(void)
{
  ENTRY("vIndexIsUniqueFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bIsUnique = pIndex->bIsUnique();
  
  m_pDataStack->vPushI8(bIsUnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isUnique! -- store isUnique into index                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsUniqueStore(void)
{
  ENTRY("vIndexIsUniqueStore"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsUnique;
  if ( ! m_pDataStack->bPopI8(iIsUnique) )
    THROW_DSTACK();
  
  pIndex->vIsUnique(iIsUnique ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isValidUniqueIndex -- is it a valid unqiue index                     */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsValidUniqueIndex(void)
{
  ENTRY("vIndexIsValidUniqueIndex"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
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
  
  m_pDataStack->vPushI8(bValid ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.lowerBound -- return rows with lower bound                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexLowerBound(void)
{
  ENTRY("vIndexLowerBound"_Stz);
  
  Session* pSession = m_pTask->m_pSession;
  Object* pPtrIndex = nullptr;
  FetchBuffer* pFetchBuffer;
  
  GenVar xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! m_pDataStack->bPopObject(pPtrJob) )
    THROW_DSTACK();
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the key
  if ( ! m_pDataStack->bPopGenVar(xVariable) )
    THROW_DSTACK();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJob_SubFetch->m_pParentJob);
  i8 iSlot = pJob_SubFetch->i_Slot();
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
  EntWrk xEntWrkLower;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = AM::pFormatters();
  Stz* szStz = xVariable.szStz();
  
  RowExtractor::vAddElement(&xEntWrkLower,szStz,pColumn0,pFormatters);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkLower.vAppendMinusInfinity();
  }
  //xEntWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkLower.aEnt());
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aLowerEntBuffer) {
    // position in the index buffer at an entry prior to the lower key
    //aLowerEntry = aLowerEntBuffer->aPositionQuery(xEntWrkLower, eLowerRelnOp);
    
    aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xEntWrkLower.aEnt());
    
    if (eLowerRelnOp == RO_GREATER_THAN) {
      aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry, xEntWrkLower.aEnt(), &aLowerEntBuffer);
    }
    else {
      RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
      aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xEntWrkLower.aEnt(), &aLowerEntBuffer);
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
    if ( ! Job_FetchRows::bAppendRow(pFetchBuffer, aLowerEntry, pJob_FetchRows)) {
      
      // Buffer is Full;
      // put this tempEntBuffer in session bufferQueue
      pQueue->vAppend(pFetchBuffer);
      
      // create another fetchBuffer for the results
      pFetchBuffer = new FetchBuffer(pTable, iSlot);
      
      // insert row from entry in fetchBuffer
      bool bResult = Job_FetchRows::bAppendRow(pFetchBuffer, aLowerEntry, pJob_FetchRows);
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
void Step::vIndexNameFetch(void)
{
  ENTRY("vIndexNameFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  s32 sIndexName = pIndex->sIndexName();
  
  m_pDataStack->vPushS32(sIndexName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.populate -- populate the index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexPopulate(void)
{
  ENTRY("vIndexPopulate"_Stz);
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  bool bPopulated = pTable->bPopulateIndex(iIndexNo);
  
  m_pDataStack->vPushI8(bPopulated ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.range -- return rows within given range                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexRange(void)
{
  ENTRY("vIndexRange"_Stz);
  
  Object* pPtrIndex = nullptr;
  Session* pSession = m_pTask->m_pSession;
  
  GenVar xValWrkLwr;
  GenVar xValWrkUpr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! m_pDataStack->bPopObject(pPtrJob) )
    THROW_DSTACK();
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the lower key
  if ( ! m_pDataStack->bPopGenVar(xValWrkLwr) )
    THROW_DSTACK();
  
  // get the upper key
  if ( ! m_pDataStack->bPopGenVar(xValWrkUpr) )
    THROW_DSTACK();
  
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
  i8 iSlot = pJob_SubFetch->i_Slot();
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
  EntWrk xEntWrkLower;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = AM::pFormatters();
  Stz* szStz = xValWrkLwr.szStz();
  
  RowExtractor::vAddElement(&xEntWrkLower,szStz,pColumn0,pFormatters);

  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkLower.vAppendMinusInfinity();
  }
  //xEntWrkLower.vShow(pIndex->pIdxColVec());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the upper key
  EntWrk xEntWrkUpper;
  pColumn0 = pIndex->pIdxColVec()->pGet(0);
  szStz = xValWrkUpr.szStz();
  
  RowExtractor::vAddElement(&xEntWrkUpper,szStz,pColumn0,pFormatters);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkUpper.vAppendPlusInfinity();
  }
  //xEntWrkUpper.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if upperKey > lowerKey
  eKC eCompare = EntWrk::eCompare(xEntWrkUpper.aEnt(), xEntWrkLower.aEnt());
  
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
  aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkLower.aEnt());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aLowerEntBuffer) {
    // position in the index buffer at the lower key
    aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xEntWrkLower.aEnt());
    
    if (eLowerRelnOp == RO_GREATER_THAN) {
      aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry, xEntWrkLower.aEnt(), &aLowerEntBuffer);
    }
    else {
      RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
      aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xEntWrkLower.aEnt(), &aLowerEntBuffer);
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we find a lower entry ?
  if (! aLowerEntry )
    goto LastBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer with keyUpper
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkUpper.aEnt());
  
  //aUpperEntBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aUpperEntBuffer) {
    //aUpperEntry = aUpperEntBuffer->aPositionQuery(xEntWrkUpper, eUpperRelnOp);
    aUpperEntry = aUpperEntBuffer->pPositionPriorEntry(xEntWrkUpper.aEnt());
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, xEntWrkUpper.aEnt(), &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry, xEntWrkUpper.aEnt(), &aUpperEntBuffer);
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffers till no more entries starting from
  while (aLowerEntry) {
    
    // check against the upper bound
    bIsEqual = false;
    
    //bool bBefore = aItem->aKey()->bJuxtaCompare(JC_BEFORE, xEntWrkUpper.aKey(),
    //                                             pIndex->pIdxColVec(), 1,
    //                                             bIsEqual, eRelnOp);
    
    bBefore = (aLowerEntry == aUpperEntry) ? false : true;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // if aLowerEntry is before aUpperEntry then append to result buffer
    if (bBefore) {
      
      // insert row from entry in tempEntBuffer
      if ( ! Job_FetchRows::bAppendRow(pFetchBuffer, aLowerEntry, pJob_FetchRows)) {
        
        // Buffer is Full
        // put this fetchBuffer in session bufferQueue
        pQueue->vAppend(pFetchBuffer);
        
        // create another fetchBbuffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in temporary buffer
        bool bResult = Job_FetchRows::bAppendRow(pFetchBuffer, aLowerEntry, pJob_FetchRows);
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
void Step::vIndexShowColumnVec(void)
{
  ENTRY("vIndexShowColumnVec"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndex->vShowColumnVec();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.space -- push space used by index                                    */
/*----------------------------------------------------------------------------*/
void Step::vIndexSpace(void)
{
  ENTRY("vIndexSpace"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iBuffers = pIndex->pTable()->nIndexBuffers(pIndex->iIndexNo());
  i8 iSpace = iBuffers * C_SIZE_POOL_BUFFER;
  m_pDataStack->vPushI8(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.subDic -- get table sub dictionary                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexSubDic(void)
{
  ENTRY("vIndexSubDic"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Dic* pDic = pIndex->pSubDic();
  
  m_pDataStack->vPushPointer(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table@ -- fetch table in index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTableFetch(void)
{
  ENTRY("vIndexTableFetch"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Table* pTable = pIndex->pTable();
  ASSERT(CN_TABLE == pTable->eClass());
  
  m_pDataStack->vPushPointer(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table! -- store table in index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTableStore(void)
{
  ENTRY("vIndexTableStore"_Stz);
  
  Object* pPtrIndex = nullptr;
  Object* pPtrTable = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
    THROW_DSTACK();
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pIndex->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.truncate -- truncate the index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTruncate(void)
{
  ENTRY("vIndexTruncate"_Stz);
  
  Object* pPtrIndex = nullptr;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iSlot = 0; iSlot < AM::pGrid()->nSlotsPerNode(); iSlot++)
    pTable->vTruncateIndex(iSlot, iIndexNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.upperBound -- return rows with upper bound                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexUpperBound(void)
{
  ENTRY("vIndexUpperBound"_Stz);
  
  Object* pPtrIndex = nullptr;
  Session* pSession = m_pTask->m_pSession;
  GenVar xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Object* pPtrJob;
  if ( ! m_pDataStack->bPopObject(pPtrJob) )
    THROW_DSTACK();
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pPtrJob);
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the key
  if ( ! m_pDataStack->bPopGenVar(xVariable) )
    THROW_DSTACK();
  
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
  i8 iSlot = pJob_SubFetch->i_Slot();
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
  EntWrk xEntWrkUpper;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Formatters* pFormatters = AM::pFormatters();
  Stz* szStz = xVariable.szStz();
  
  RowExtractor::vAddElement(&xEntWrkUpper,szStz,pColumn0,pFormatters);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkUpper.vAppendPlusInfinity();
  }
  //xEntWrkUpper.vShow();
  
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
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkUpper.aEnt());
  
  eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  
  if (aUpperEntBuffer) {
    
    aUpperEntry = aUpperEntBuffer->pPositionPriorEntry(xEntWrkUpper.aEnt());
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, xEntWrkUpper.aEnt(), &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry, xEntWrkUpper.aEnt(), &aUpperEntBuffer);
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
      if ( ! Job_FetchRows::bAppendRow(pFetchBuffer,aItem, pJob_FetchRows)) {
        
        // Buffer is full
        // put this tempEntBuffer in session bufferQueue
        pQueue->vAppend(pFetchBuffer);
        
        // create another result buffer for the results
        pFetchBuffer = new FetchBuffer(pTable, iSlot);
        
        // insert row from entry in temporary buffer
        bool bResult = Job_FetchRows::bAppendRow(pFetchBuffer, aItem, pJob_FetchRows);
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
void Step::vIndexVerify(void)
{
  ENTRY("vIndexVerify"_Stz);
  
  Object* pPtrIndex;
  
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bVerified = pIndex->pTable()->bVerifyIndex(pIndex);
  s256 s = bVerified ? "Valid"_Stz : "Invalid"_Stz;
  m_pDataStack->vPushS256(s);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.visualize -- visualize the index                                     */
/*----------------------------------------------------------------------------*/
void Step::vIndexVisualize(void)
{
  ENTRY("vIndexVisualize"_Stz);
  
  Object* pPtrIndex;
  if ( ! m_pDataStack->bPopObject(pPtrIndex) )
    THROW_DSTACK();
  
  Index* pIndex = _pIndex(pPtrIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 iPartition = 0; iPartition < AM::pGrid()->nSlotsPerNode(); iPartition++) {
    TablePtn* pPartition = pTable->pPartition(iPartition);
    IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
    vTrace("Table Partition %lld\n"_Stz, iPartition);
    pIndexTree->vVisualize();
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
