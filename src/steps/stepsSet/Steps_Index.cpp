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
  vAddStep("index[].compact"_Stz,       &Step::vIndexCompact);
  vAddStep("index[].compactPtn"_Stz,    &Step::vIndexCompactPtn);
  vAddStep("index[].deleteNext"_Stz,    &Step::vIndexDeleteNext);
  vAddStep("index[].dump"_Stz,          &Step::vIndexDump);
  vAddStep("index.indexDef@"_Stz,       &Step::vIndexIndexDefFetch);
  vAddStep("index.indexDef!"_Stz,       &Step::vIndexIndexDefStore);
  vAddStep("index[].indexTree"_Stz,     &Step::vIndexIndexTree);
  vAddStep("index.isPrimaryKey@"_Stz,   &Step::vIndexIsPrimaryKeyFetch);
  vAddStep("index.isPrimaryKey!"_Stz,   &Step::vIndexIsPrimaryKeyStore);
  vAddStep("index.isUnique@"_Stz,       &Step::vIndexIsUniqueFetch);
  vAddStep("index.isValidUniqueIndex"_Stz,&Step::vIndexIsValidUniqueIndex);
  vAddStep("index[].name@"_Stz,         &Step::vIndexNameFetch);
  vAddStep("index.populate"_Stz,        &Step::vIndexPopulate);
  vAddStep("index[].size"_Stz,          &Step::vIndexSize);
  vAddStep("index.table@"_Stz,          &Step::vIndexTableFetch);
  vAddStep("index.table!"_Stz,          &Step::vIndexTableStore);
  vAddStep("index[].verify"_Stz,        &Step::vIndexVerify);
  vAddStep("index[].verifyPtn"_Stz,     &Step::vIndexVerifyPtn);
  vAddStep("index[].visualize"_Stz,     &Step::vIndexVisualize);
}

/*----------------------------------------------------------------------------*/
/* index[].column@ -- get the column from the index                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexColumnFetch(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
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
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eClass());
  
  m_pDataStack->vPushInt(pIndex->nColumns());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].compact -- index compaction                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexCompact(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eClass());
  
  pIndex->pTable()->vCompactIndex(pIndex);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].compactPtn -- compact index partition                              */
/*----------------------------------------------------------------------------*/
void Step::vIndexCompactPtn(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIdxNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Job* pJob = m_pTask->m_pJob;
  i8 i_Ptn  = pJob->m_pOp->tPtnNo();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(i_Ptn);

  
  IndexTree *pIndexTree = pTable->pTablePtn(i_TablePtn)->pIndexTree(iIdxNo);
  
  pIndexTree->vCompact();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index::delete -- delete an instance of index                               */
/*----------------------------------------------------------------------------*/
void Step::vIndexDelete(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

  delete pIndex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].deleteNext -- delete rows within the range                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexDeleteNext(void)
{
  ENTRY();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the index and the table
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table
  Table* pTable = pIndex->pTable();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the table partition
  TablePtn* pTablePtn = _pTablePtn(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the lower and upper entries
  Ent* aLowerEntry = m_pLogicUnit->m_aLowerEntry;
  Ent* aUpperEntry = m_pLogicUnit->m_aUpperEntry;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the entry buffer to start from
  EntBuffer* pEntBuffer = m_pLogicUnit->m_pEntBufferOuter;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get IndexTree
  i8 iIndexNo = pIndex->iIndexNo();
  IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  HV_RowSingle hvRowSingle(pTable->pStreamDef());
  
  bool bBefore = true;
  Ent* aEntryToDelete;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get a lower entry ?
  if ( ! aLowerEntry) {
    goto L_Exit;
  }
  
  vTrace("------------------------------------------\n"_Stz);
  vTrace("Deleting rows:\n"_Stz);
  
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
      Rrf xRef = aLowerEntry->rxRef();
        
      Crw* aCrw = CrwBuffer::pRefdCrw(xRef);
      
      RowInflator::vInflate_AllColumns_into_RowSingle(hvRowSingle,aCrw);
      hvRowSingle->vPrintRow();
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // delete the refernced crw
      bool bDeleted = CrwBuffer::bDeleteRefdCrw(xRef);
      ASSERT(bDeleted);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // save the entry
      aEntryToDelete = aLowerEntry;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      aLowerEntry = pEntBuffer->aNextEntry(aLowerEntry, 0);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // now purge the entry
      pIndexTree->vDelete(pEntBuffer, aEntryToDelete);
      
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
  vTrace("------------------------------------------\n"_Stz);
  m_pLogicUnit->m_pEntBufferOuter = pEntBuffer;
  m_pLogicUnit->m_aLowerEntry = aLowerEntry;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].dump -- dump the index                                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexDump(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::nPtnsPerTable(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
    vTrace("Partition %lld\n"_Stz, i_TablePtn);
    
    pIndexTree->vDump(false);
  }
  
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
/* index[].IndexTree -- get index tree of index[] for current partition       */
/*----------------------------------------------------------------------------*/
void Step::vIndexIndexTree(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);

  // -- -- -- -- -- -- -- -- -- -- -- --
  
  Job* pJob = m_pTask->m_pJob;
  
  Table* pTable = pIndex->pTable();
 
  i8 i_Ptn = pJob->m_pOp->tPtnNo();

  i8 i_TablePtn = pTable->iPtnNoToTablePtn(i_Ptn);

  TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
  
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
/* index.name@ -- fetch index name                                            */
/*----------------------------------------------------------------------------*/
void Step::vIndexNameFetch(void)
{
  ENTRY();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the index
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
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
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
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
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  bool bVerified = pIndex->pTable()->bVerifyIndex(pIndex);
  m_pDataStack->vPushInt(bVerified);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].verifyPtn -- verify partition of index                             */
/*----------------------------------------------------------------------------*/
void Step::vIndexVerifyPtn(void)
{
  ENTRY();
  
  Index* pIndex = *m_pLogicUnit->m_xpIndex;
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIdxNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  Job* pJob = m_pTask->m_pJob;
  i8 i_Ptn  = pJob->m_pOp->tPtnNo();
  i8 i_TablePtn = pTable->iPtnNoToTablePtn(i_Ptn);

  IndexTree *pIndexTree = pTable->pTablePtn(i_TablePtn)->pIndexTree(iIdxNo);
  
  EntBuffer *pIndexRootBuffer = pIndexTree->pRootBuffer();

  bool bVerified;
  if (pIndexRootBuffer)
    bVerified = pIndexRootBuffer->bVerify();
  else
    bVerified = true;
  
  m_pDataStack->vPushInt(bVerified);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* index[].visualize -- visualize the index                                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexVisualize(void)
{
  ENTRY();
  
  Index* pIndex = _pIndex(*m_pLogicUnit->m_xpIndex);
  ASSERT(CN_INDEX == pIndex->eClass());
  
  i8 iIndexNo = pIndex->iIndexNo();
  Table* pTable = pIndex->pTable();
  
  for (i8 i_TablePtn = 0; i_TablePtn < AM::nPtnsPerTable(); i_TablePtn++) {
    TablePtn* pTablePtn = pTable->pTablePtn(i_TablePtn);
    IndexTree* pIndexTree = pTablePtn->pIndexTree(iIndexNo);
    vTrace("Table Partition %lld\n"_Stz, i_TablePtn);
    pIndexTree->vVisualize();
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
