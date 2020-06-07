/*============================================================================*/
/* Steps_Index.cpp - index step group                                         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Index(void)
{
  vAddStep("index::delete"_Stz,         &Step::vIndexDelete);
  vAddStep("index::new"_Stz,            &Step::vIndexNew);
  
  vAddStep("index[].columnCount@"_Stz,  &Step::vIndexColumnCountFetch);
  vAddStep("index[].column@"_Stz,       &Step::vIndexColumnFetch);
  vAddStep("index.columnName"_Stz,      &Step::vIndexColumnName);
  vAddStep("index.columnType"_Stz,      &Step::vIndexColumnType);
  vAddStep("index[].compact"_Stz,       &Step::vIndexCompact);
  vAddStep("index[].dump"_Stz,          &Step::vIndexDump);
  vAddStep("index[].fetchNext"_Stz,     &Step::vIndexFetchNextN);
  vAddStep("index[].genEntBuffer"_Stz,  &Step::vIndexGenEntBuffer);
  vAddStep("index[].genEntBuffers"_Stz, &Step::vIndexGenEntBuffers);
  vAddStep("index.indexDef@"_Stz,       &Step::vIndexIndexDefFetch);
  vAddStep("index.indexDef!"_Stz,       &Step::vIndexIndexDefStore);
  vAddStep("index.IndexTree"_Stz,       &Step::vIndexIndexTree);
  vAddStep("index.isPrimaryKey@"_Stz,   &Step::vIndexIsPrimaryKeyFetch);
  vAddStep("index.isPrimaryKey!"_Stz,   &Step::vIndexIsPrimaryKeyStore);
  vAddStep("index.isUnique@"_Stz,       &Step::vIndexIsUniqueFetch);
  vAddStep("index.isValidUniqueIndex"_Stz,&Step::vIndexIsValidUniqueIndex);
  vAddStep("index[].lowerBound"_Stz,      &Step::vIndexLowerBoundN);
  vAddStep("index.lowerBound"_Stz,      &Step::vIndexLowerBound);
  vAddStep("index.name@"_Stz,           &Step::vIndexNameFetch);
  vAddStep("index.populate"_Stz,        &Step::vIndexPopulate);
  vAddStep("index[].position"_Stz,      &Step::vIndexPositionN);
  vAddStep("index[].positionLower"_Stz, &Step::vIndexPositionLowerN);
  vAddStep("index[].positionUpper"_Stz, &Step::vIndexPositionUpperN);
  vAddStep("index.range"_Stz,           &Step::vIndexRange);
  vAddStep("index[].size"_Stz,          &Step::vIndexSize);
  vAddStep("index.table@"_Stz,          &Step::vIndexTableFetch);
  vAddStep("index.table!"_Stz,          &Step::vIndexTableStore);
  vAddStep("index.upperBound"_Stz,      &Step::vIndexUpperBound);
  vAddStep("index[].verify"_Stz,        &Step::vIndexVerify);
  vAddStep("index[].visualize"_Stz,     &Step::vIndexVisualize);
}

/*----------------------------------------------------------------------------*/
/* index.column@ -- get the column from the index                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iColumnNo = m_pDataStack->iPopInt(__func__);
  
  ASSERT(iColumnNo < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iColumnNo);
  
  m_pDataStack->vPushPtr(pColumn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnCount@ -- fetch index column count                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnCountFetch(void)
{
  ENTRY();
  
  Index* pIndex = *m_pRegSet->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eClass());
  
  m_pDataStack->vPushInt(pIndex->pIdxColVec()->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnName -- get column name                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnName(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex = m_pDataStack->iPopInt(__func__);
  
  ASSERT(iIndex < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iIndex);
  
  s32 sColNm(pColumn->szColNm());
  m_pDataStack->vPushStr(sColNm);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.columnType -- get column type                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnType(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndex = m_pDataStack->iPopInt(__func__);
  
  ASSERT(iIndex < pIndex->pIdxColVec()->nItems());
  Column* pColumn = pIndex->pIdxColVec()->pGet(iIndex);
  s32 sColumnType = aCT[pColumn->eColumnType()];
  
  m_pDataStack->vPushStr(sColumnType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.compact -- index compaction                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexCompact(void)
{
  ENTRY();
  
  Index* pIndex = *m_pRegSet->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndex->pTable()->vCompactIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::new -- delete an instance of index                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexDelete(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

  delete pIndex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].dump -- dump the index                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDump(void)
{
  ENTRY();
  
  Index* pIndex = *m_pRegSet->m_xpIndex;
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
/* index[].genEntBuffer -- generate an ent buffer for this index              */
/*----------------------------------------------------------------------------*/
void Step::vIndexGenEntBuffer(void)
{
  ENTRY();
  
  Index* pIndex = *m_pRegSet->m_xpIndex;
  IndexDef* pIndexDef = pIndex->pIndexDef();
  IdxColVec* pIdxColVec = pIndexDef->pIdxColVec();

  i8 nIdxCol = pIdxColVec->nItems();
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCol; i_IdxCol++)
    vTrace("  [%lld] %s\n"_Stz,i_IdxCol,pIdxColVec->pGet(i_IdxCol)->szColNm());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EntWrk oEntWrk;
  
  EntBuffer* pEntBuffer = new EntBuffer();
  
  pEntBuffer->vIsPermanent(false);
  
  i8 iBufferNo = pEntBuffer->iBufferNo();
  
  Ent* aPrevEnt = nullptr;

  i8 i_EntNo;
  for (i_EntNo = 0; ; i_EntNo++) {
    
    oEntWrk.vReset();
    
    oEntWrk.vRandomEnt(pIdxColVec);
    oEntWrk.vBuffer(iBufferNo);
    oEntWrk.vItem(i_EntNo);
    oEntWrk.vOffsets(1);

    //oEntWrk.vShow(pIdxColVec);

    i8 nBytes = oEntWrk.nSize_EntKeyNeo();

    Ent* aEnt = (Ent*)pEntBuffer->ayAllotBytes(nBytes);
    if (! aEnt)
      break;
    
    MemLib::vCopy(aEnt,oEntWrk.aEnt(),nBytes,CN_ENTBUFFER);
    aEnt->vNEO(0,-1);
    
    i8 iOffset = pEntBuffer->iToOffset(aEnt);
    if (aPrevEnt)
      aPrevEnt->vNEO(0,(i4)iOffset);
    
    pEntBuffer->vIncItems();
    
    aPrevEnt = aEnt;
  }
  
  vTrace("Generated EntBuffer with %lld entries\n"_Stz,i_EntNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushPtr(pEntBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].genEntBuffers -- generate ent buffers for this index               */
/*----------------------------------------------------------------------------*/
void Step::vIndexGenEntBuffers(void)
{
  ENTRY();
  
  i8 nBuffers = m_pDataStack->iPopInt(__func__);
  
  Index* pIndex = *m_pRegSet->m_xpIndex;
  IndexDef* pIndexDef = pIndex->pIndexDef();
  IdxColVec* pIdxColVec = pIndexDef->pIdxColVec();
  
  i8 nIdxCol = pIdxColVec->nItems();
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCol; i_IdxCol++)
    vTrace("  [%lld] %s\n"_Stz,i_IdxCol,pIdxColVec->pGet(i_IdxCol)->szColNm());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EntWrk oEntWrk;
  
  EntBuffer* pEntBuffer = nullptr;
  EntBuffer* pRootBuffer = nullptr;
  EntBuffer* pPrevBuffer = nullptr;
  
  while (nBuffers) {
    
    pPrevBuffer = pEntBuffer;
    pEntBuffer = new EntBuffer();
    
    // set root buffer
    if ( ! pRootBuffer )
      pRootBuffer = pEntBuffer;
    
    pEntBuffer->vIsPermanent(false);
    i8 iBufferNo = pEntBuffer->iBufferNo();
    
    // link the new buffer with the prev
    if (pPrevBuffer)
      pPrevBuffer->vNextBuffer(pEntBuffer);
    pEntBuffer->vPrevBuffer(pPrevBuffer);
    
    Ent* aPrevEnt = nullptr;
    
    i8 i_EntNo;
    for (i_EntNo = 0; ; i_EntNo++) {
      
      oEntWrk.vReset();
      
      oEntWrk.vRandomEnt(pIdxColVec);
      oEntWrk.vBuffer(iBufferNo);
      oEntWrk.vItem(i_EntNo);
      oEntWrk.vOffsets(1);
      
      //oEntWrk.vShow(pIdxColVec);
      
      i8 nBytes = oEntWrk.nSize_EntKeyNeo();
      
      Ent* aEnt = (Ent*)pEntBuffer->ayAllotBytes(nBytes);
      if (! aEnt)
        break;
      
      MemLib::vCopy(aEnt,oEntWrk.aEnt(),nBytes,CN_ENTBUFFER);
      aEnt->vNEO(0,-1);
      
      i8 iOffset = pEntBuffer->iToOffset(aEnt);
      if (aPrevEnt)
        aPrevEnt->vNEO(0,(i4)iOffset);
      
      pEntBuffer->vIncItems();
      
      aPrevEnt = aEnt;
    }
    
    vTrace("Generated EntBuffer with %lld entries\n"_Stz,i_EntNo);
    nBuffers--;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushPtr(pRootBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.indexDef@ -- fetch index definition                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexIndexDefFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pIndex->pIndexDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.indexDef! -- store index definition                                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexIndexDefStore(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  IndexDef* pIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));
  
  pIndex->vIndexDef(pIndexDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.IndexTree -- get  index tree of given index for current partition    */
/*----------------------------------------------------------------------------*/
void Step::vIndexIndexTree(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  // -- -- -- -- -- -- -- -- -- -- -- --
  
  Job* pJob = m_pTask->m_pJob;
  
  i8 iPtn = pJob->m_pOp->nPartition();

  i8 iSlot = TablePtn::iPartitionToSlot(iPtn);

  Table* pTable = pIndex->pTable();
  
  TablePtn* pTablePtn = pTable->pPartition(iSlot);
  
  i8 iIndexNo = pIndex->iIndexNo();

  IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  m_pDataStack->vPushPtr(pIndexTree);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey@ -- fetch isPrimaryKey from index                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsPrimaryKeyFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bIsPrimaryKey = pIndex->bIsPrimaryKey();
  
  m_pDataStack->vPushInt(bIsPrimaryKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isPrimaryKey! -- store isPrimaryKey into index                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsPrimaryKeyStore(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIsPrimaryKey = m_pDataStack->iPopInt(__func__);
  
  pIndex->vIsPrimaryKey(iIsPrimaryKey ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isUnique@ -- fetch isUnique from index                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsUniqueFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bIsUnique = pIndex->bIsUnique();
  
  m_pDataStack->vPushInt(bIsUnique);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.isValidUniqueIndex -- is it a valid unqiue index                     */
/*----------------------------------------------------------------------------*/
void Step::vIndexIsValidUniqueIndex(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // get the table partition stream def
  StreamDef* pPtnStreamDef = pIndex->pTable()->pPtnStreamDef();
  
  // get the index column vector
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  bool bValid = true;
  
  //! ALKA: shouldn't this be checking for the leading edge of the index to include
  //!  all partition keys ???
  // a unique index must include all columns in the partitioning keys
  i8 nPtnColumns = pPtnStreamDef->nColumns(C_ALL);
  bool bFound = false;
  for (i8 i = 0; i < nPtnColumns; i++)
  {
    Column* pColumn = pPtnStreamDef->pGetColumn(i);
    
    for (i8 i_Item = 0; i_Item < pIdxColVec->nItems(); i_Item++) {
      if (pIdxColVec->pGet(i_Item)->iColumnNo() == pColumn->iColumnNo()) {
        bFound = true;
        break;
      }
    }
    
    if ( ! bFound ) {
      bValid = false;
      break;
    }
  }
  
  m_pDataStack->vPushInt(bValid ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].fetchNext -- return rows within range                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexFetchNextN(void)
{
  ENTRY();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the index and the table
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  TablePtn* pPartition = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the rowOnce skipList
  RowOnce* pRowOnce = _pRowOnce(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table
  Table* pTable = pIndex->pTable();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the lower and upper entries
  Ent* aLowerEntry = m_pRegSet->m_aLowerEntry;
  Ent* aUpperEntry = m_pRegSet->m_aUpperEntry;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the entry buffer to start from
  EntBuffer* pEntBuffer = m_pRegSet->m_pEntBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  CrwBuffer* pCrwBuffer = nullptr;
  RowCompactor* pRowCompactor = nullptr;
  HV_Row hRow(pTable->pStreamDef(),C_ALL);
  
  bool bBefore = true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get a lower entry ?
  if ( ! aLowerEntry) {
    goto L_Exit;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pCrwBuffer = new CrwBuffer;
  pCrwBuffer->vTable(pPartition->pTable());
  pCrwBuffer->vPartitionNo(pPartition->iPartitionId()) ;
  
  // create a row compactor for the crw buffer
  pRowCompactor = new RowCompactor;
  pCrwBuffer->vAppendOpen(pRowCompactor, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffers starting from aLowerEntry
  while (aLowerEntry) {
    
    // is lowerEntry before upperEntry ?
    if (aUpperEntry)
      bBefore = (aLowerEntry == aUpperEntry) ? false : true;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // lowerEntry is before upperEntry
    if (bBefore) {
      
      // get the crw entry from the lowerEntry
      Ref xRef = aLowerEntry->xRef();
      
      // has this row already been seen ?
      if ( ! pRowOnce || ! pRowOnce->bRowSeen(xRef) ) {
          
        Crw* aCrw = CrwBuffer::pRefdCrw(xRef);
       
        RowInflator::vInflateRow(hRow,aCrw);
        
        // insert row from entry in crwBuffer
        if ( ! pCrwBuffer->bAppendSrw(hRow)) {
          
          // Buffer is full ?
          goto L_Exit;
        }
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      aLowerEntry = pEntBuffer->aNextEntry(aLowerEntry, 0);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // no more entries ?
      if ( ! aLowerEntry ) {
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // move to next buffer
        pEntBuffer = pEntBuffer->pNextBuffer();
        
        // next buffer ?
        if ( pEntBuffer )
          aLowerEntry = pEntBuffer->aLeadEntry();
      }
    }
    else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // we are done
      aLowerEntry = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    //if (pSession->m_bSessionExiting)
    //  aItem = nullptr;
  }
  
L_Exit:
  m_pRegSet->m_pEntBuffer = pEntBuffer;
  m_pRegSet->m_aLowerEntry = aLowerEntry;
  
  // cleanup and return the buffer
  if (pCrwBuffer) {
    pCrwBuffer->vAppendClose();
    delete pRowCompactor;
  }
  m_pDataStack->vPushPtr(pCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].position -- position for lower bound and upper bound key           */
/*----------------------------------------------------------------------------*/
void Step::vIndexPositionN(void)
{
  ENTRY();
  
  EntBuffer* aUpperEntBuffer;
  Ent* aUpperEntry = nullptr;
  
  eRO eUpperRelnOp;
  eRO eLowerRelnOp;
  
  EntWrk xEntWrkLower;
  EntWrk xEntWrkUpper;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the index
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  TablePtn* pPartition = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the planner
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the upper bound key
  Datum xUpperVariable;
  m_pDataStack->vPopVal(xUpperVariable,__func__);
  Stz* szUpperKey = xUpperVariable.szStz();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the lower bound key
  Datum xLowerVariable;
  m_pDataStack->vPopVal(xLowerVariable,__func__);
  Stz* szLowerKey = xLowerVariable.szStz();
  
  EntBuffer* aLowerEntBuffer = nullptr;
  Ent* aLowerEntry = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the phrase
  i8 iPhrase = m_pDataStack->iPopInt(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get IndexTree
  i8 iIndexNo = pIndex->iIndexNo();
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of columns in the index
  i8 nIdxCols = pIndex->pIdxColVec()->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get column0 of the index
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  
  bool bExactQuery = false;
  bool bHaveLowerKey = false;
  bool bHaveUpperKey = false;
  
  Format* pFmt = AM::pFmt();
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Lower Key
  
  if ( ! StrLib::bSame(szLowerKey->azChar(), "") ) {
    bHaveLowerKey = true;
    
    // form the lower key
    KeyBuilder::vAddElement(&xEntWrkLower,pColumn0,szLowerKey,pFmt);
    for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
      xEntWrkLower.vAppendMinusInfinity();
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Upper Key
  
  if ( ! StrLib::bSame(szUpperKey->azChar(), "") ) {
    bHaveUpperKey = true;
    
    // form the upper key
    KeyBuilder::vAddElement(&xEntWrkUpper,pColumn0,szUpperKey,pFmt);
    for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
      xEntWrkUpper.vAppendPlusInfinity();
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if we have both upper and lower keys
  if ( bHaveLowerKey && bHaveUpperKey) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if upperKey > lowerKey
    eKC eCompare = Key::eCompare(xEntWrkUpper.aKey(),
                                 xEntWrkLower.aKey());
    
    if (eCompare == KC_BEFORE)
      goto L_Exit;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Position for Lower Key
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bHaveLowerKey ) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the lower relational OP
    eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // exact key ?
    if (eLowerRelnOp == RO_EQUAL_TO) {
      bExactQuery = true;
      eLowerRelnOp = RO_GREATER_EQUAL;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // first position at a level 0 buffer with lowerKey
    aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkLower.aKey());
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // did we get an entry buffer ?
    if (aLowerEntBuffer) {
      
      // position in the index buffer at an entry prior to the lowerKey
      aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xEntWrkLower.aKey());
      
      if (eLowerRelnOp == RO_GREATER_THAN) {
        aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry,
                                                         xEntWrkLower.aKey(),
                                                         &aLowerEntBuffer);
      }
      else {
        RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
        aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xEntWrkLower.aKey(), &aLowerEntBuffer);
      }
    }
  }
  else {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // no lower key
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // position at the first level 0 buffer
    aLowerEntBuffer = pIndexTree->pFirstH0Buffer();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // did we get an entryBuffer ?
    if ( ! aLowerEntBuffer) {
      goto L_Exit;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the first entry in the buffer
    aLowerEntry = aLowerEntBuffer->aLeadEntry();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Position for Upper Key
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // there is no upper key ?
  if ( ! bHaveUpperKey )
    goto L_Exit;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get upper relational OP
  if (bExactQuery) {
    eUpperRelnOp = RO_LESS_EQUAL;
  }
  else {
    eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer for the upper key
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkUpper.aKey());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // do we have an upper Entry buffer ?
  if (aUpperEntBuffer) {
    
    aUpperEntry = aUpperEntBuffer->pPositionPriorEntry(xEntWrkUpper.aKey());
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry,
                                                       xEntWrkUpper.aKey(),
                                                       &aUpperEntBuffer);
    }
  }
  
L_Exit:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set registers
  m_pRegSet->m_aLowerEntry = aLowerEntry;
  m_pRegSet->m_aUpperEntry = aUpperEntry;
  m_pRegSet->m_pEntBuffer  = aLowerEntBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].positionLower --position for lower bound key                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexPositionLowerN(void)
{
  ENTRY();
  
  Datum xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the index
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  TablePtn* pPartition = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the key
  m_pDataStack->vPopVal(xVariable,__func__);
  
  i8 iPhrase = 0;
  
  EntBuffer* aLowerEntBuffer;
  Ent* aLowerEntry = nullptr;
  
  eRO eLowerRelnOp;
  i8 iIndexNo = pIndex->iIndexNo();
  
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of columns in the index
  i8 nIdxCols = pIndex->pIdxColVec()->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  EntWrk xEntWrkLower;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Format* pFmt = AM::pFmt();
  Stz* szStz = xVariable.szStz();
  
  KeyBuilder::vAddElement(&xEntWrkLower,pColumn0,szStz,pFmt);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkLower.vAppendMinusInfinity();
  }
  //xEntWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkLower.aKey());
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aLowerEntBuffer) {
    // position in the index buffer at an entry prior to the lower key
    //aLowerEntry = aLowerEntBuffer->aPositionQuery(xEntWrkLower, eLowerRelnOp);
    
    aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xEntWrkLower.aKey());
    
    if (eLowerRelnOp == RO_GREATER_THAN) {
      aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry,
                                                       xEntWrkLower.aKey(),
                                                       &aLowerEntBuffer);
    }
    else {
      RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
      aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xEntWrkLower.aKey(), &aLowerEntBuffer);
    }
  }
  
  m_pRegSet->m_aLowerEntry = aLowerEntry;
  m_pRegSet->m_pEntBuffer = aLowerEntBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].positionUpperN -- position with upper bound                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexPositionUpperN(void)
{
  ENTRY();
  
  Datum xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  TablePtn* pPartition = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the key
  m_pDataStack->vPopVal(xVariable,__func__);
  
  EntBuffer* pEntBuffer;
  EntBuffer* aUpperEntBuffer;
  
  Ent* aUpperEntry = nullptr;
  Ent* aItem      = nullptr;
  
  eRO eUpperRelnOp;
  
  i8 iIndexNo = pIndex->iIndexNo();
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of columns in the index
  i8 nIdxCols = pIndex->pIdxColVec()->nItems();
  i8 iPhrase = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the upper key
  EntWrk xEntWrkUpper;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Format* pFmt = AM::pFmt();
  Stz* szStz = xVariable.szStz();
  
  KeyBuilder::vAddElement(&xEntWrkUpper,pColumn0,szStz,pFmt);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkUpper.vAppendPlusInfinity();
  }
  //xEntWrkUpper.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // position at the first level 0 buffer
  pEntBuffer = pIndexTree->pFirstH0Buffer();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entryBuffer ?
  if ( ! pEntBuffer) {
    goto L_Exit;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first entry in the buffer
  aItem = pEntBuffer->aLeadEntry();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer for the upper key
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkUpper.aKey());
  
  eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  
  if (aUpperEntBuffer) {
    
    aUpperEntry = aUpperEntBuffer->pPositionPriorEntry(xEntWrkUpper.aKey());
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry,
                                                       xEntWrkUpper.aKey(),
                                                       &aUpperEntBuffer);
    }
  }
  
  //aUpperEntry->vShow(0,pIndex,true);
  m_pRegSet->m_aLowerEntry = aItem;
  m_pRegSet->m_aUpperEntry = aUpperEntry;
  m_pRegSet->m_pEntBuffer  = pEntBuffer;
  
L_Exit:
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].lowerBound -- return rows with lower bound                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexLowerBoundN(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer;
  
  Datum xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the index
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  TablePtn* pPartition = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the key
  m_pDataStack->vPopVal(xVariable,__func__);
  
  i8 iPhrase = 0;
  
  EntBuffer* aLowerEntBuffer;
  Ent* aLowerEntry = nullptr;
  eRO eLowerRelnOp;
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  IndexTree* pIndexTree = pPartition->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a root result buffer for the results
  CrwBuffer* pRootCrwBuffer = new CrwBuffer;
  pRootCrwBuffer->vTable(pPartition->pTable());
  pRootCrwBuffer->vPartitionNo(pPartition->iPartitionId()) ;
  
  pCrwBuffer = pRootCrwBuffer;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of columns in the index
  i8 nIdxCols = pIndex->pIdxColVec()->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  EntWrk xEntWrkLower;
  Column* pColumn0 = pIndex->pIdxColVec()->pGet(0);
  Format* pFmt = AM::pFmt();
  Stz* szStz = xVariable.szStz();
  
  KeyBuilder::vAddElement(&xEntWrkLower,pColumn0,szStz,pFmt);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkLower.vAppendMinusInfinity();
  }
  //xEntWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkLower.aKey());
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aLowerEntBuffer) {
    // position in the index buffer at an entry prior to the lower key
    //aLowerEntry = aLowerEntBuffer->aPositionQuery(xEntWrkLower, eLowerRelnOp);
    
    aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xEntWrkLower.aKey());
    
    if (eLowerRelnOp == RO_GREATER_THAN) {
      aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry,
                                                       xEntWrkLower.aKey(),
                                                       &aLowerEntBuffer);
    }
    else {
      RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
      aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xEntWrkLower.aKey(), &aLowerEntBuffer);
    }
  }
  
  HV_Row hRow(pTable->pStreamDef(),C_ALL);
  
  // create a row compactor for the crw buffer
  RowCompactor* pRowCompactor = new RowCompactor;
  pCrwBuffer->vAppendOpen(pRowCompactor, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get a lower entry ?
  if ( ! aLowerEntry) {
    goto L_Exit;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffer till no more entries
  while (aLowerEntry) {
    
    Crw* aCrw = CrwBuffer::pRefdCrw(aLowerEntry->xRef());
    
    RowInflator::vInflateRow(hRow,aCrw);
    
    // insert row from entry in crwBuffer
    if ( ! pCrwBuffer->bAppendSrw(hRow)) {
      
      // Buffer is Full;
      pCrwBuffer->vAppendClose();
      
      // create another crwbuffer for the results
      CrwBuffer* pNewCrwBuffer = new CrwBuffer;
      pNewCrwBuffer->vTable(pPartition->pTable());
      pNewCrwBuffer->vPartitionNo(pPartition->iPartitionId());
      pRowCompactor = new RowCompactor;
      pNewCrwBuffer->vAppendOpen(pRowCompactor, false);
      
      // link the old buffer with this buffer
      pCrwBuffer->vNextBuffer(pNewCrwBuffer);
      
      // now append to new buffer
      pCrwBuffer = pNewCrwBuffer;
      
      // insert row from entry in fetchBuffer
      bool bResult = pCrwBuffer->bAppendCrw(aCrw);
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
    //if (pSession->m_bSessionExiting)
    //  aLowerEntry = nullptr;
  }
  
L_Exit:
  //pRootCrwBuffer->vDump();
  m_pDataStack->vPushPtr(pRootCrwBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.lowerBound -- return rows with lower bound                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexLowerBound(void)
{
  ENTRY();
 /*
  FetchBuffer* pFetchBuffer;
  
  Datum xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get members from subFetch
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the key
  m_pDataStack->vPopVal(xVariable,__func__);
  
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
  Format* pFmt = AM::pFmt();
  Stz* szStz = xVariable.szStz();
  
  KeyBuilder::vAddElement(&xEntWrkLower,pColumn0,szStz,pFmt);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkLower.vAppendMinusInfinity();
  }
  //xEntWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkLower.aKey());
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aLowerEntBuffer) {
    // position in the index buffer at an entry prior to the lower key
    //aLowerEntry = aLowerEntBuffer->aPositionQuery(xEntWrkLower, eLowerRelnOp);
    
    aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xEntWrkLower.aKey());
    
    if (eLowerRelnOp == RO_GREATER_THAN) {
      aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry,
                                                       xEntWrkLower.aKey(),
                                                       &aLowerEntBuffer);
    }
    else {
      RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
      aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xEntWrkLower.aKey(), &aLowerEntBuffer);
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
    //if (pSession->m_bSessionExiting)
    //  aLowerEntry = nullptr;
  }
  
LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  pQueue->vAppend(pFetchBuffer);
 */
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.name@ -- fetch index name                                            */
/*----------------------------------------------------------------------------*/
void Step::vIndexNameFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  s32 sIndexName = pIndex->szIndexNm();
  
  m_pDataStack->vPushStr(sIndexName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::new -- create a new instance of index                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexNew(void)
{
  ENTRY();
  
  s32 sIndexNm;
  m_pDataStack->vPopStr(sIndexNm,__func__);

  Index* pIndex = new Index(sIndexNm);
  
  m_pDataStack->vPushPtr(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.populate -- populate the index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexPopulate(void)
{
  ENTRY();
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  bool bPopulated = pTable->bPopulateIndex(iIndexNo);
  
  m_pDataStack->vPushInt(bPopulated ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.range -- return rows within given range                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexRange(void)
{
  ENTRY();
 /*
  Datum xValWrkLwr;
  Datum xValWrkUpr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the lower key
  m_pDataStack->vPopVal(xValWrkLwr,__func__);
  
  // get the upper key
  m_pDataStack->vPopVal(xValWrkUpr,__func__);
  
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
  Format* pFmt = AM::pFmt();
  Stz* szStz = xValWrkLwr.szStz();
  
  KeyBuilder::vAddElement(&xEntWrkLower,pColumn0,szStz,pFmt);

  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkLower.vAppendMinusInfinity();
  }
  //xEntWrkLower.vShow(pIndex->pIdxColVec());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // make the upper key
  EntWrk xEntWrkUpper;
  pColumn0 = pIndex->pIdxColVec()->pGet(0);
  szStz = xValWrkUpr.szStz();
  
  KeyBuilder::vAddElement(&xEntWrkUpper,pColumn0,szStz,pFmt);
  for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
    xEntWrkUpper.vAppendPlusInfinity();
  }
  //xEntWrkUpper.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if upperKey > lowerKey
  eKC eCompare = Key::eCompare(xEntWrkUpper.aKey(),
                               xEntWrkLower.aKey());
  
  if (eCompare == KC_BEFORE)
    goto LastBuffer;
  
  eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
  if (eLowerRelnOp == RO_EQUAL_TO) {
    eLowerRelnOp = RO_GREATER_EQUAL;
    eUpperRelnOp = RO_LESS_EQUAL;
  }
  else {
    eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first position at a level 0 buffer with keyLower
  aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkLower.aKey());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aLowerEntBuffer) {
    // position in the index buffer at the lower key
    aLowerEntry = aLowerEntBuffer->pPositionPriorEntry(xEntWrkLower.aKey());
    
    if (eLowerRelnOp == RO_GREATER_THAN) {
      aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry,
                                                       xEntWrkLower.aKey(),
                                                       &aLowerEntBuffer);
    }
    else {
      RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
      aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xEntWrkLower.aKey(), &aLowerEntBuffer);
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we find a lower entry ?
  if (! aLowerEntry )
    goto LastBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer with keyUpper
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkUpper.aKey());
  
  //aUpperEntBuffer->vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get an entry buffer ?
  if (aUpperEntBuffer) {
    //aUpperEntry = aUpperEntBuffer->aPositionQuery(xEntWrkUpper, eUpperRelnOp);
    aUpperEntry = aUpperEntBuffer->pPositionPriorEntry(xEntWrkUpper.aKey());
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry,
                                                       xEntWrkUpper.aKey(),
                                                       &aUpperEntBuffer);
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
        //if (pSession->m_bSessionExiting)
        //  aLowerEntry = nullptr;
      }
    }
    else {
      aLowerEntry = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    //if (pSession->m_bSessionExiting)
    //  aLowerEntry = nullptr;
  }
  
LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  pQueue->vAppend(pFetchBuffer);
*/
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].size -- push space used by index                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexSize(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iBuffers = pIndex->pTable()->nIndexBuffers(pIndex->iIndexNo());
  i8 iSpace = iBuffers * C_SIZE_POOL_BUFFER;
  m_pDataStack->vPushInt(iSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table@ -- fetch table in index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTableFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Table* pTable = pIndex->pTable();
  ASSERT(CN_TABLE == pTable->eClass());
  
  m_pDataStack->vPushPtr(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.table! -- store table in index                                       */
/*----------------------------------------------------------------------------*/
void Step::vIndexTableStore(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_TABLE == pTable->eClass());
  
  pIndex->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.upperBound -- return rows with upper bound                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexUpperBound(void)
{
  ENTRY();
  /*

  Datum xVariable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get parameters for the primitive
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_INDEX == pIndex->eClass());
  
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_JOB_SUBFETCH == pJob_SubFetch->eClass());
  
  // get the key
  m_pDataStack->vPopVal(xVariable,__func__);
  
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
  Format* pFmt = AM::pFmt();
  Stz* szStz = xVariable.szStz();
  
  KeyBuilder::vAddElement(&xEntWrkUpper,pColumn0,szStz,pFmt);
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
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkUpper.aKey());
  
  eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
  
  if (aUpperEntBuffer) {
    
    aUpperEntry = aUpperEntBuffer->pPositionPriorEntry(xEntWrkUpper.aKey());
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry,
                                                       xEntWrkUpper.aKey(),
                                                       &aUpperEntBuffer);
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
    //if (pSession->m_bSessionExiting)
    //  aItem = nullptr;
  }
  
LastBuffer:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark this buffer as the last buffer
  pFetchBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  pQueue->vAppend(pFetchBuffer);
  */
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].verify -- verify index                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexVerify(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bVerified = pIndex->pTable()->bVerifyIndex(pIndex);
  m_pDataStack->vPushInt(bVerified);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index.visualize -- visualize the index                                     */
/*----------------------------------------------------------------------------*/
void Step::vIndexVisualize(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pRegSet->m_xpIndex);
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
