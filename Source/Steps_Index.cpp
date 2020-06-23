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
  vAddStep("index.name@"_Stz,           &Step::vIndexNameFetch);
  vAddStep("index.populate"_Stz,        &Step::vIndexPopulate);
  vAddStep("index[].position"_Stz,      &Step::vIndexPositionN);
  vAddStep("index[].size"_Stz,          &Step::vIndexSize);
  vAddStep("index.table@"_Stz,          &Step::vIndexTableFetch);
  vAddStep("index.table!"_Stz,          &Step::vIndexTableStore);
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
  
  ASSERT(iColumnNo < pIndex->nColumns());
  Column* pColumn = pIndex->pGetColumn(iColumnNo);
  
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
  
  m_pDataStack->vPushInt(pIndex->nColumns());
  
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
  
  ASSERT(iIndex < pIndex->nColumns());
  Column* pColumn = pIndex->pGetColumn(iIndex);
  
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
  
  ASSERT(iIndex < pIndex->nColumns());
  Column* pColumn = pIndex->pGetColumn(iIndex);
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

  i8 nIdxCol = pIndexDef->nColumns();
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCol; i_IdxCol++)
    vTrace("  [%lld] %s\n"_Stz,i_IdxCol,pIndexDef->pGetColumn(i_IdxCol)->szColNm());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  EntWrk oEntWrk;
  
  EntBuffer* pEntBuffer = new EntBuffer();
  
  pEntBuffer->vIsPermanent(false);
  
  i8 iBufferNo = pEntBuffer->iBufferNo();
  
  Ent* aPrevEnt = nullptr;

  i8 i_EntNo;
  for (i_EntNo = 0; ; i_EntNo++) {
    
    oEntWrk.vReset();
    
    oEntWrk.vRandomEnt(pIndexDef);
    oEntWrk.vBuffer(iBufferNo);
    oEntWrk.vItem(i_EntNo);
    oEntWrk.vOffsets(1);

    //oEntWrk.vShow(pIndexDef);

    i8 nBytes = oEntWrk.nSize_EntKeyNeo();

    Ent* aEnt = (Ent*)pEntBuffer->ayAllotBytes(nBytes);
    if (! aEnt)
      break;
    
    MemLib::vCopy(aEnt,oEntWrk.aEnt(),nBytes,CN_ENTBUFFER);
    aEnt->vNextEntry(0,C_ENT_END_TICKET);
    
    tET tEntryTicket = pEntBuffer->tToTicket(aEnt);
    if (aPrevEnt)
      aPrevEnt->vNextEntry(0,tEntryTicket);
    
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
  
  i8 nIdxCol = pIndex->nColumns();
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCol; i_IdxCol++)
    vTrace("  [%lld] %s\n"_Stz,i_IdxCol,pIndexDef->pGetColumn(i_IdxCol)->szColNm());
  
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
      
      oEntWrk.vRandomEnt(pIndexDef);
      oEntWrk.vBuffer(iBufferNo);
      oEntWrk.vItem(i_EntNo);
      oEntWrk.vOffsets(1);
      
      //oEntWrk.vShow(pIndexDef);
      
      i8 nBytes = oEntWrk.nSize_EntKeyNeo();
      
      Ent* aEnt = (Ent*)pEntBuffer->ayAllotBytes(nBytes);
      if (! aEnt)
        break;
      
      MemLib::vCopy(aEnt,oEntWrk.aEnt(),nBytes,CN_ENTBUFFER);
      aEnt->vNextEntry(0,C_ENT_END_TICKET);
      
      tET tEntryTicket = pEntBuffer->tToTicket(aEnt);
      if (aPrevEnt)
        aPrevEnt->vNextEntry(0,tEntryTicket);

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
  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxColumns = pIndexDef->nColumns();
  bool bValid = true;
  
  //! ALKA: shouldn't this be checking for the leading edge of the index to include
  //!  all partition keys ???
  // a unique index must include all columns in the partitioning keys
  i8 nPtnColumns = pPtnStreamDef->nColumns(C_ALL);
  bool bFound = false;
  for (i8 i = 0; i < nPtnColumns; i++)
  {
    Column* pColumn = pPtnStreamDef->pGetColumn(i);
    
    for (i8 i_Item = 0; i_Item < nIdxColumns; i_Item++) {
      if (pIndexDef->pGetColumn(i_Item)->iColumnNo() == pColumn->iColumnNo()) {
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
  IndexDef* pIndexDef = pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get column0 of the index
  Column* pColumn0 = pIndexDef->pGetColumn(0);
  
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
      aLowerEntry = aLowerEntBuffer->aPositionPriorEntry(xEntWrkLower.aKey());
      
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
    
    aUpperEntry = aUpperEntBuffer->aPositionPriorEntry(xEntWrkUpper.aKey());
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
