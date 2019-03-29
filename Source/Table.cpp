/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include "MetaData.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Table* MetaData::_pTable(void* aAddr)
{
  Table* pTable = reinterpret_cast<Table*>(aAddr);
  
  if (pTable && CN_TABLE != pTable->eClass())
    vTrace("_pTable got class %s\n"_Stz,aCN[pTable->eClass()]);
  
  RASSERT_TYPE(pTable,CN_TABLE);
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Table::operator new(size_t)
{
  Table* pTable = nullptr;
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Table::operator new(size_t, Table* pTable)
{
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Table::operator delete(void*)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(void)
: Object(CN_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  gpAttachMap->m_oStatistics.Table.m_nCreates++;
  
  Word* pWord = Word::pToWord(this);
  
  m_sTableName = pWord->sName();
  m_eTableType = TT_NORMAL;
  
  //m_xSubDic

  m_xSlots.vTable(this);
  
  m_ixIndexNo.vCheckInit();
  
  m_pIdxVec = new IdxVec;
  
  m_pColumnVec = new ColumnVec;
  
  for (i8 i_Slot = 0; i_Slot < C_SLOTS_PER_NODE; i_Slot++)
    m_ixSubLoaders[i_Slot] = 0;
  
  m_bIsLoading = false;
  
  _checkInitialised_(CN_TABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Table::~Table(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  gpAttachMap->m_oStatistics.Table.m_nDrops++;
  
  vTidyUp();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump table                                                                 */
/*----------------------------------------------------------------------------*/
void Table::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Table* pTable = _pTable(pObject);
  
  vTrace("name %s type %s\n"_Stz,
         pTable->m_sTableName.aChar(),
         aTT[pTable->m_eTableType]->aChar());
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void Table::vFinalize(eCN eClass, Object* pObject)
{
  Table* pTable = _pTable(pObject);
  ASSERT(pTable->bWordBody());
  
  pTable->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/* find table                                                                 */
/*----------------------------------------------------------------------------*/
Table* Table::pFindTable(DicStack* pDicStack, Stz* aTableNm)
{
  Dic* pDic = pDicStack->pTop();
  
  Word* pTableDicWord = pDic->pFindWord(aTableNm);
  
  Dic* pTableDic = _pDic(pTableDicWord->aWspArea());

  Word* pTableWord = pTableDic->pFindWord(aTableNm);
  
  Table* pTable = _pTable(pTableWord->aWspArea());
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void Table::vMonitorPortrait(s48K& sPortrait)
{
  Statistics* pStats = &gpAttachMap->m_oStatistics;
  
  sPortrait.vSprint("Table: creates %lld drops %lld loads %lld\n"_Stz,
                    pStats->Table.m_nCreates,
                    pStats->Table.m_nDrops,
                    pStats->Table.m_nLoads
                    );
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add index to table                                                         */
/*----------------------------------------------------------------------------*/
void Table::vAddIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // set the index no in the index
  pIndex->vIndexNo(++m_ixIndexNo);
  
  // add index to table
  m_pIdxVec->vAdd(pIndex);
  
  // create an IndexTree for the index in each partition
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlotsPerNode; iPartition++) {
    TablePtn* pPartition = this->pPartition(iPartition);
    pPartition->vAddIndex(new IndexTree(pIndex, iPartition));
  }
}

/*----------------------------------------------------------------------------*/
/* analyze index                                                              */
/*----------------------------------------------------------------------------*/
Stz* Table::pAnalyzeIndex(Index* pIndex, bool bDetailed, i8 &nRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
 
  i8 nEntriesTotal = 0;
  i8 nBuffersTotal = 0;
  s48K sAnalysis;
  
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlotsPerNode; iSlot++) {
    
    i8 iPartition = TablePtn::iSlotToPartition(iSlot);
    
    EntryBuffer *pIndexBuffer = pPartition(iSlot)->pIndexTree(pIndex->iIndexNo())->pRoot();
    
    i8 nBuffers = 0;
    i8 nEntries = 0;

    if (pIndexBuffer)
      pIndexBuffer->vAnalyze(sAnalysis, bDetailed, nBuffers, nEntries);
    
    nEntriesTotal += nEntries;
    nBuffersTotal += nBuffers;
    
    if ( ! bDetailed ) {
      sAnalysis.vCatSprint("row: ('%s',%lld,%lld,%lld)\n"_Stz,
                           gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                           iPartition,
                           nBuffers,
                           nEntries);

    }
  }
  
//  if ( ! gpAttachMap->m_bMetadata   || bDetailed)
//    sAnalysis.vCatSprint(
//                         "        total: %3lld buffers; %5lld entries\n"_Stz,
//                         nBuffersTotal,
//                         nEntriesTotal);
  
  if (bDetailed)
    nRows = nBuffersTotal;
  else
    nRows = 1;
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* analyze index                                                              */
/*----------------------------------------------------------------------------*/
Stz* Table::pAnalyzeTable(bool bDetailed, i8 &nRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nRowsTotal    = 0;
  i8 nBuffersTotal = 0;
  s48K sAnalysis;
  
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlotsPerNode; iSlot++) {
    
    i8 iPartition = TablePtn::iSlotToPartition(iSlot);
    
    TablePtn *pPartition = this->pPartition(iSlot);
    
    RowBuffer* pRowBuffer = pPartition->pRootRowBuffer();
    
    i8 nBuffers = 0;
    i8 nRows    = 0;
    
    if (pRowBuffer)
      pRowBuffer->vAnalyze(sAnalysis, bDetailed, nBuffers, nRows);
    
    nRowsTotal += nRows;
    nBuffersTotal += nBuffers;
    
    if ( ! bDetailed ) {
      sAnalysis.vCatSprint("row: ('%s',%lld,%lld,%lld)\n"_Stz,
                           gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                           iPartition,
                           nBuffers,
                           nRows);
      
    }
  }
  
  //  if ( ! gpAttachMap->m_bMetadata   || bDetailed)
  //    sAnalysis.vCatSprint(
  //                         "        total: %3lld buffers; %5lld entries\n"_Stz,
  //                         nBuffersTotal,
  //                         nEntriesTotal);
  
  if (bDetailed)
    nRows = nBuffersTotal;
  else
    nRows = 1;
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* compact index                                                              */
/*----------------------------------------------------------------------------*/
void Table::vCompactIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlotsPerNode; iSlot++) {
    
    i8 iPartition = TablePtn::iSlotToPartition(iSlot);
    
    IndexTree *pIndexTree = pPartition(iSlot)->pIndexTree(pIndex->iIndexNo());
    
    pIndexTree->vCompact();
    
  }
}

/*----------------------------------------------------------------------------*/
/* lazy build of index                                                        */
/*----------------------------------------------------------------------------*/
void Table::vLazyBuildIndexes(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pPartition = this->pPartition(iSlot);
  IndexTreeVec* pInedexTreeVec = pPartition->pIndexTreeVec();
  i8 nIndexes = pInedexTreeVec->nItems();
  
  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pNewIndexTree = pInedexTreeVec->pGet(i_Index);
    
    Index* pIndex = pNewIndexTree->pIndex();
    
    // get an L0 buffer
    EntryBuffer *pIndexBuffer = pPartition->pIndexTree(pIndex->iIndexNo())->pFirstH0Buffer();
    
    // build skip list for all L0 buffers
    if (pIndexBuffer)
      pIndexBuffer->vLazyBuild();
  }
}

/*----------------------------------------------------------------------------*/
/* insert entries from a row buffer into the table (hidden and other indexes) */
/*----------------------------------------------------------------------------*/
void Table::vInsertEntries(i8 iTxId, i8 iSlot, RowBuffer* pRowBuffer)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  //----------------------------------------------------------------------------
  // This function is invoked when we insert rows into a table via a
  // begin/insert/end and via the API's insert-select

  ASSERT(iSlot < gpAttachMap->m_pGrid->m_nSlotsPerNode);

  // if buffer has 0 entries (only head) then should be last buffer
  if (pRowBuffer->nRowCount() == 0) {
    delete pRowBuffer;
    return;
  }

  //----------------------------------------------------------------------------
  // (1) append the buffer entries into the hidden index
  TablePtn* pPartition = this->pPartition(iSlot);
  
  //----------------------------------------------------------------------------
  // append the buffer to the data row buffers
  pPartition->vAppendBuffer(pRowBuffer);
  
  //----------------------------------------------------------------------------
  // (2) insert entries from hidden index pFirstBuffer starting at pFirstEntry
  // into all other indexes of the table

  // get no of indexes of the table
  IndexTreeVec* pIndexTreeVec = pPartition->pIndexTreeVec();
  i8 nIndexes = pIndexTreeVec->nItems();

  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {

    // get index
    IndexTree* pNewIndexTree = pIndexTreeVec->pGet(i_Index);

    // insert entries from EntryBuffer into the index
    pNewIndexTree->vInsertEntriesFromBuffer(pRowBuffer);
  }
}

/*----------------------------------------------------------------------------*/
/* insert a row into the table (hidden and other indexes)                     */
/*----------------------------------------------------------------------------*/
void Table::vInsertRow(i8 iTxId, i8 iSlot, Stz* aRow_Data, i8 iRowLength)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  //----------------------------------------------------------------------------
  // This function is invoked when we insert a row into a table via a
  // insert and via the API's insert-select
  
  ASSERT(iSlot < gpAttachMap->m_pGrid->m_nSlotsPerNode);
  
  //----------------------------------------------------------------------------
  // (1) append the row into the hidden index
  TablePtn* pPartition = this->pPartition(iSlot);
  IndexTree* pHiddenIndexTree = pPartition->pHiddenIndexTree();
  
  Item* aItem = pHiddenIndexTree->pAppendRow(iTxId, aRow_Data, iRowLength);
  
  //----------------------------------------------------------------------------
  // (2) insert entry into all other indexes of the table
  
  // get no of indexes of the table
  IndexTreeVec* pIndexTreeVec = pPartition->pIndexTreeVec();
  i8 nIndexes = pIndexTreeVec->nItems();
  
  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pNewIndexTree = pIndexTreeVec->pGet(i_Index);
    
    // get the index
    Index* pIndex = pNewIndexTree->pIndex();
    
    // insert entry into other indexes
    bool bResult = pNewIndexTree->bInsertEntry(aItem);
    RASSERT(bResult);
  }
}

/*----------------------------------------------------------------------------*/
/* get ColumnVec                                                              */
/*----------------------------------------------------------------------------*/
ColumnVec* Table::pColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pColumnVec;
}

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void Table::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vDump(CN_TABLE, this, Stz::aTab(1));
}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Table::vCreateColumnVec(s32& sColumnNm, s48K& sColumnDefn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
//  vTrace("vCreateColumnVec: table %s column %s %s\n"_Stz,
//         aTableName(),
//         sColumnNm.aStz(),
//         sColumnDefn.aStz());
  
  Column* pColumn = new Column(sColumnNm.aStz(),sColumnDefn.aStz());
  
  m_pColumnVec->vInsert(pColumn);
  
  // renumber
  for (i8 iColumn = 0; iColumn < m_pColumnVec->nItems(); iColumn++) {
    Column* pColumn = m_pColumnVec->pGet(iColumn);
    if (pColumn->iColumnNo() != -1)
      pColumn->vColumnNo(iColumn);
  }
}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Table::vCreateColumnVec(Jobs::Job_CreateTable* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // NOTE: we will move creation of hidden column here after
  // old tasking model is removed
  // For now we will remove the hidden column and add it back in
  //Column* pHiddenColumn = m_pColumnVec->pGet(0);
  //m_pColumnVec->vClear();
  
  for (i8 i= 0; i < pJob->m_iColumns; i++) {
  
    Column* pColumn = new Column(pJob, i);
    m_pColumnVec->vAdd(pColumn);
  }
  
  //m_pColumnVec->vAdd(pHiddenColumn);
}

/*----------------------------------------------------------------------------*/
/* get the index vector from the table                                        */
/*----------------------------------------------------------------------------*/
IdxVec* Table::pIdxVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pIdxVec;
}

/*----------------------------------------------------------------------------*/
/* get if table is loading                                                    */
/*----------------------------------------------------------------------------*/
bool Table::bIsLoading(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_bIsLoading;
}

/*----------------------------------------------------------------------------*/
/* validate index columns                                                     */
/*----------------------------------------------------------------------------*/
bool Table::bIsValidIndexColumns(Jobs::Job_CreateIndex* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Column* pColumn = nullptr;
  
  // find the column from the table columnVector
  i8 iColumns = m_pColumnVec->nItems();
  i8 iColumnsIndex = pJob->m_iColumns;
  s32 sColumnNm;
  
  for (i8 i_ColumnIndex = 0; i_ColumnIndex < iColumnsIndex; i_ColumnIndex++) {
    
    sColumnNm = pJob->m_sColumnName[i_ColumnIndex];
    
    bool bFound = false;
    for (i8 i_Col = 0; i_Col < iColumns; i_Col++) {
      pColumn = m_pColumnVec->pGet(i_Col);
      if (pColumn->sColumnName().bSame(sColumnNm)) {
        bFound = true;
        break;
      }
    }
    if ( ! bFound )
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set table is loading                                                       */
/*----------------------------------------------------------------------------*/
void Table::vIsLoading(bool bIsLoading)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_bIsLoading = bIsLoading;
}

/*----------------------------------------------------------------------------*/
/* mark job to activity map                                                   */
/*----------------------------------------------------------------------------*/
void Table::vMarkTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
 
  s48K sTableMark;
  
  sTableMark.vSprintCat("Table %s\n"_Stz,
                        m_sTableName.aStz());
  
  i8 nCols = m_pColumnVec->nItems();
  
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pColumn = m_pColumnVec->pGet(i_Col);
    sTableMark.vSprintCat("  Col %02lld %s %s(%lld,%lld) %s\n"_Stz,
                          i_Col,
                          pColumn->sColumnName().aStz(),
                          
                          aCT[pColumn->eColumnType()],
                          pColumn->iColumnLength(),
                          pColumn->iColumnScale(),
                          
                          pColumn->bIsNullable() ? "nullable" : "not-null",
                          pColumn->bIsPartitionKey() ? "part-key" : ""
                          );

  }
  
  ActivityMap::vMarkString(-1,sTableMark);
}

/*----------------------------------------------------------------------------*/
/* get a partition                                                            */
/*----------------------------------------------------------------------------*/
TablePtn* Table::pPartition(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pPart = m_xSlots.pPartition(iSlot);
  
  return pPart;
}

/*----------------------------------------------------------------------------*/
/* populate the index                                                         */
/*----------------------------------------------------------------------------*/
bool Table::bPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_xSlots.bPopulateIndex(iIndexNo);
}

/*----------------------------------------------------------------------------*/
/* remove index from table                                                    */
/*----------------------------------------------------------------------------*/
void Table::vRemoveIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // remove index from the table
  m_pIdxVec->vRemove(pIndex);
}

/*----------------------------------------------------------------------------*/
/* get number of index buffers in this table                                  */
/*----------------------------------------------------------------------------*/
i8 Table::nIndexBuffers(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlotsPerNode; iPartition++) {
    IndexTree* pIndexTree = pPartition(iPartition)->pIndexTree(iIndexNo);

    iBuffers += pIndexTree->nEntryBuffers();
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* get partition column vector                                                */
/*----------------------------------------------------------------------------*/
PtnColVec*  Table::pPtnColVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_xSlots.pPtnColVec();
}

/*----------------------------------------------------------------------------*/
/* publish rows/buffers                                                       */
/*----------------------------------------------------------------------------*/
void Table::vPublish(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vTrace("Publishing table %s\n"_Stz, m_sTableName.aChar());
  
  // purge all other indices
  for (i8 iIndexNo = 0; iIndexNo < m_pIdxVec->nItems(); iIndexNo++) {
    Index* pIndex = m_pIdxVec->pGet(iIndexNo);
    m_xSlots.vPublishIndex(iPartition, pIndex->iIndexNo());
  }
}

/*----------------------------------------------------------------------------*/
/* attempt to purge oldest buffer                                             */
/*----------------------------------------------------------------------------*/
bool Table::bPurge(i8 iPartition, i8 iOlderThanSeconds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  //vTrace("Purging table %s\n"_Stz, m_sTableName.aChar());
  
  // purge all other indexes
  for (i8 iIndexNo = 0; iIndexNo < m_pIdxVec->nItems(); iIndexNo++) {
    
    Index* pIndex = m_pIdxVec->pGet(iIndexNo);
    
    bool bPurged = m_xSlots.bPurgeIndex(iPartition, pIndex->iIndexNo(), iOlderThanSeconds);
    
    if (! bPurged)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get root buffer of given index                                             */
/*----------------------------------------------------------------------------*/
EntryBuffer* Table::pRootBuffer(i8 i_Slot, i8 i_Index)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pTblPtn = pPartition(i_Slot);
  
  IndexTree* pIdxTree = pTblPtn->pIndexTree(i_Index); // assume 0 is hidden index
  
  EntryBuffer* pRootBuffer = pIdxTree->pRoot();
  
  return pRootBuffer;
}

/*----------------------------------------------------------------------------*/
/* hash the first field of row return partition                               */
/*----------------------------------------------------------------------------*/
i8 Table::iRowPartition(i8 iHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // modulo to get partition number
  i8 iPartition =
  iHash
  % (gpAttachMap->m_pGrid->m_nSlotsPerNode * gpAttachMap->m_pGrid->nNodes());
  
  return iPartition;  
}

/*
i8 Table::iRowPartition(Stz* aRow_Data)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nCols = m_xSlots.pPtnColVec()->nItems();
  
  i8 iHash = 0;
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    // get the column from the index column vector
    Column* pColumn = m_xSlots.pPtnColVec()->pGet(iColumn);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get the column value
    RowData* aRowData = (RowData*)aRow_Data;
    char* sCursor = aRowData->aColumnDatum(iColumnNo);
    
    // hash the column value
    DtmLib::vHash(pColumn->eDatumType(),iHash,(Stz*)sCursor);
  }
  
  // modulo to get partition number
  i8 iPartition =
       iHash
         % (gpAttachMap->m_pGrid->m_nSlotsPerNode * gpAttachMap->m_pGrid->nNodes());
  //vTrace("Hashed to Partition: %lld\n"_Stz,iPartition);
  
  return iPartition;
}
*/

/*----------------------------------------------------------------------------*/
/* get number of row buffers in this table                                    */
/*----------------------------------------------------------------------------*/
i8 Table::nRowBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlotsPerNode; iPartition++) {
    iBuffers += pPartition(iPartition)->nRowBuffers();
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* show column vector                                                         */
/*----------------------------------------------------------------------------*/
void Table::vShowColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  for (i8 iColumn = 0; iColumn < m_pColumnVec->nItems(); iColumn++) {
    Column* pColumn = m_pColumnVec->pGet(iColumn);
    pColumn->vShow();
  }
}

/*----------------------------------------------------------------------------*/
/* get slots                                                                  */
/*----------------------------------------------------------------------------*/
Slots* Table::pSlots(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return &m_xSlots;
}

/*----------------------------------------------------------------------------*/
/* get table sub dictionary                                                   */
/*----------------------------------------------------------------------------*/
Dic* Table::pSubDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return &m_xSubDic;
}

/*----------------------------------------------------------------------------*/
/* set subloaders                                                             */
/*----------------------------------------------------------------------------*/
void Table::vSubLoaders(i8 iSubLoaders)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 i_Slot = 0; i_Slot < C_SLOTS_PER_NODE; i_Slot++)
    m_ixSubLoaders[i_Slot] = iSubLoaders;
  
}

/*----------------------------------------------------------------------------*/
/* get the table name                                                         */
/*----------------------------------------------------------------------------*/
Stz* Table::aTableName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_sTableName;
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void Table::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_pIdxVec->vClear();
  
  delete m_pIdxVec;
  delete m_pColumnVec;
  
  m_xSlots.vTidyUp();
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void Table::vTidyUpIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_xSlots.vTidyUpIndex(iIndexNo);
}

/*----------------------------------------------------------------------------*/
/* release all rows/buffers                                                   */
/*----------------------------------------------------------------------------*/
void Table::vTruncate(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // truncate all indices
  for (i8 iIndexNo = 0; iIndexNo < m_pIdxVec->nItems(); iIndexNo++) {
    Index* pIndex = m_pIdxVec->pGet(iIndexNo);
    m_xSlots.vTruncateIndex(iPartition, pIndex->iIndexNo());
  }
  
  // now truncate the table data
  m_xSlots.vTruncateTable(iPartition);
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void Table::vTruncateIndex(i8 iPartitionNo, i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_xSlots.vTruncateIndex(iPartitionNo, iIndexNo);
}

/*----------------------------------------------------------------------------*/
/* verify all indexes                                                         */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  TablePtn* pPartition = this->pPartition(0);             //# discuss with Alka
  IndexTreeVec* pIdxTreeVec = pPartition->pIndexTreeVec();
  i8 nIndexes = pIdxTreeVec->nItems();

  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    IndexTree* pIndexTree = pIdxTreeVec->pGet(i_Index);
    bool bValid = pIndexTree->bVerifyIndex();
    if (! bValid)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify index                                                               */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlotsPerNode; iSlot++) {
    
    bool bVerify = true;
    
    EntryBuffer *pIndexBuffer =
      pPartition(iSlot)->pIndexTree(pIndex->iIndexNo())->pRoot();
    
    if (pIndexBuffer)
      bVerify = pIndexBuffer->bVerify();
    if ( ! bVerify )
      break;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify table                                                               */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // verify all the row buffers with the r ow data
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlotsPerNode; iSlot++) {
    
    bool bVerify = true;
    
    bVerify = pPartition(iSlot)->bVerifyTable();
    
    if ( ! bVerify )
      return bVerify;
  }
  
  // now verify the indexes
  return bVerifyIndexes();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Table::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE);
  
  return &m_xFooter;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
