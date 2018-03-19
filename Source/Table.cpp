/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include "DBobjects.h"

//#include "Word.hpp"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Table* DBobjects::_pTable(void* aAddr)
{
  Table* pTable = reinterpret_cast<Table*>(aAddr);
  
  RASSERT( !pTable || CN_TABLE == pTable->eClass() );
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Table::operator new(size_t iSize)
{
  RASSERT(iSize);
  Table* pTable = nullptr;
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Table::operator new(size_t iSize, Table* pTable)
{
  RASSERT(iSize);
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Table::operator delete(void* pTableAddress)
{
  RASSERT(pTableAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(void)
: Object(CN_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Word* pWord = Word::pToWord(this);
  
  m_sTableName = pWord->sName();
  m_eTableType = TT_NORMAL;
  
  //m_xSubDic

  m_xSlots.vTable(this);
  
  m_zIndexNo.vCheckInit();
  m_zRefCount.vCheckInit();
  m_pHiddenIndex = nullptr;
  
  m_pIdxVec = new IdxVec;
  
  m_pColumnVec = new ColumnVec;
  
  // create the hidden column for the hidden index here
  Column* pColumn = new Column("hidden_column"_Stz);
  pColumn->vColumnNo(-1);
  pColumn->vColumnType(CT_INTEGER);
  m_pColumnVec->vAdd(pColumn);
  
  for (i8 i_Slot = 0; i_Slot < C_SLOTS_PER_NODE; i_Slot++)
    m_ziSubLoaders[i_Slot] = 0;
  
  _checkInitialised_(CN_TABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(Stz* pTableName)
: Object(CN_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_sTableName = pTableName;
  
  _class_(CN_TABLE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Table::~Table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/* dump table                                                                 */
/*----------------------------------------------------------------------------*/
void Table::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_TABLE);
  ASSERT(pObject->eClass() == CN_TABLE);
  
  Table* pTable = reinterpret_cast<Table*>(pObject);
  
  vTrace("name %s type %s\n"_Stz,
         pTable->m_sTableName.aChar(),
         aTT[pTable->m_eTableType]->aChar());
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void Table::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_TABLE);
  ASSERT(pObject->eClass() == CN_TABLE);
  
  Table* pTable = reinterpret_cast<Table*>(pObject);
  ASSERT(pTable->bWordBody());
  
  pTable->vTidyUp();
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
  pIndex->vIndexNo(++m_zIndexNo);
  
  // add index to table
  m_pIdxVec->vAdd(pIndex);
  
  // create an IndexTree for the index in each partition
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlots; iPartition++) {
    Partition* pPartition = this->pPartition(iPartition);
    pPartition->vAddIndex(new IndexTree(pIndex, iPartition));
  }
}

/*----------------------------------------------------------------------------*/
/* analyze index                                                              */
/*----------------------------------------------------------------------------*/
Stz* Table::pAnalyzeIndex(Index* pIndex, bool bDetailed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
 
  i8 nEntriesTotal = 0;
  i8 nBuffersTotal = 0;
  s48K sAnalysis;
  
  if ( ! gpAttachMap->m_bMetadata  || bDetailed ) {
    if (pIndex->bIsHidden())
      sAnalysis.vCatSprint("N%lld: Analysis of table %s \n"_Stz,
                         gpAttachMap->m_pGrid->iThisNode(),
                         m_sTableName.aChar());
    else
      sAnalysis.vCatSprint("N%lld: Analysis of Index %s of table %s \n"_Stz,
                           gpAttachMap->m_pGrid->iThisNode(),
                           pIndex->aIndexName()->aChar(),
                           m_sTableName.aChar());
  }
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlots; iSlot++) {
    
    i8 iPartition = Partition::iSlotToPartition(iSlot);
    
    EntryBuffer *pIndexBuffer = pPartition(iSlot)->pIndexTree(pIndex->iIndexNo())->pRoot();
    
    i8 nBuffers = 0;
    i8 nEntries = 0;

    if (pIndexBuffer)
      pIndexBuffer->vAnalyze(sAnalysis, bDetailed, nBuffers, nEntries);
    
    nEntriesTotal += nEntries;
    nBuffersTotal += nBuffers;
    
    if ( !gpAttachMap->m_bMetadata  || bDetailed ) {
      sAnalysis.vCatSprint("partition %lld:  %3lld buffers; %5lld entries\n\n"_Stz,
                           iPartition,
                           nBuffers,
                           nEntries);
    } else {
      sAnalysis.vCatSprint("row: ('%s',%lld,%lld,%lld)\n"_Stz,
                           gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                           iPartition,
                           nBuffers,
                           nEntries);

    }
    
  }
  
  if ( ! gpAttachMap->m_bMetadata   || bDetailed)
    sAnalysis.vCatSprint(
                         "        total: %3lld buffers; %5lld entries\n"_Stz,
                         nBuffersTotal,
                         nEntriesTotal);
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* append row buffer; return true if all indexes were also loaded             */
/*----------------------------------------------------------------------------*/
/*
bool Table::bAppendBuffer(i8 iPartition, EntryBuffer* pEntryBuffer)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  ASSERT(iPartition < gpAttachMap->m_pGrid->m_nSlots);
  
  // mark buffer as permanent
  pEntryBuffer->vDurability(BD_PERMANENT);
  
  // first append buffer to the hidden index
  Partition* pPartition = this->pPartition(iPartition);
  IndexTree* pHiddenIndexTree = pPartition->pHiddenIndexTree();
  
  // is this the last buffer
  bool bLastBuffer = pEntryBuffer->bIsLast();
  
  // do not insert empty buffers
  if (pEntryBuffer->nItems() == 0) {
    delete pEntryBuffer;
  }
  else {
    // append the buffer to the hidden index
    pHiddenIndexTree->vAppendBuffer(pEntryBuffer);
  }
  
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
  // the new index load technique
  
  // we build the other indexes when we detect the last buffer being inserted
  if ( ! bLastBuffer )
    return false;
  
  // decrement no of subLoaders that have appended last buffer
  //--m_ziSubLoaders[iPartition];
  
  // check if any sub loaders still have more buffers
  if ( --m_ziSubLoaders[iPartition] != 0 )
    return false;
  
  // compute the number of entries in hidden index
  i8 nHiddenEntries = nHiddenEntryCount(pHiddenIndexTree->pRoot());
  if (gxArguments.m_bVerbose) {
    vTrace("Hidden index has %lld entries\n"_Stz,
           nHiddenEntries);
  }
  
  // get no of indexes of the table
  IdxDataVec* pIdxDataVec = pPartition->pIdxDataVec();
  i8 nIndexes = pIdxDataVec->nItems();

  // iterate all indexes to be built
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pNewIndexTree = pIdxDataVec->pGet(i_Index);
    
    // skip the hidden index
    Index* pIndex = pNewIndexTree->pIndex();
    if (pIndex->bIsHidden())
      continue;
  
    // load from hidden index into index being built
    vNewIndexLoad(iPartition,pNewIndexTree,pHiddenIndexTree,nHiddenEntries);
  }
  
  //vTrace("MARKING %lld %lld\n"_Stz, gpAttachMap->m_pGrid->m_iThisNode, *m_ziIndexesLoaded);
  
  // mark indexes being loaded
  --m_ziIndexesLoaded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // temporary                                                           //! fix
  
  if (1 == gpAttachMap->m_pGrid->m_nPartitions) {
    bool bValid = bVerifyIndexes();
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return true;
 
  // ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
}
*/

/*----------------------------------------------------------------------------*/
/* insert entries from a row buffer into the table (hidden and other indexes) */
/*----------------------------------------------------------------------------*/
void Table::vInsertEntries(i8 iTxId, i8 iSlot, WorkingBuffer* pWorkingBuffer)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  //----------------------------------------------------------------------------
  // This function is invoked when we insert rows into a table via a
  // begin/insert/end and via the API's insert-select
  
  ASSERT(iSlot < gpAttachMap->m_pGrid->m_nSlots);
  
  // if buffer has 0 entries (only head) then should be last buffer
  if (pWorkingBuffer->nWorkingRows() == 0) {
    ASSERT(pWorkingBuffer->bIsLast());
    return;
  }
  
  //----------------------------------------------------------------------------
  // (1) append the buffer entries into the hidden index
  Partition* pPartition = this->pPartition(iSlot);
  IndexTree* pHiddenIndexTree = pPartition->pHiddenIndexTree();
  
  EntryBuffer* pFirstBuffer;
  Entry* pFirstEntry;
  pHiddenIndexTree->vAppendEntriesFromBuffer(iTxId, pWorkingBuffer, &pFirstBuffer, &pFirstEntry);
  
  //----------------------------------------------------------------------------
  // (2) insert entries from hidden index pFirstBuffer starting at pFirstEntry
  // into all other indexes of the table
  
  // get no of indexes of the table
  IdxDataVec* pIdxDataVec = pPartition->pIdxDataVec();
  i8 nIndexes = pIdxDataVec->nItems();
  
  // iterate all indexes of the table
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    
    // get index
    IndexTree* pNewIndexTree = pIdxDataVec->pGet(i_Index);
    
    // skip the hidden index
    Index* pIndex = pNewIndexTree->pIndex();
    if (pIndex->bIsHidden())
      continue;
    
    EntryBuffer* pHiddenEntryBuffer = pFirstBuffer;
    Entry* pEntry = pFirstEntry;
    
    //pHiddenEntryBuffer->vDump();
    
    // iterate through all the buffers of the hidden index starting from pFirstEntry
    // in pFirstBuffer and insert corresponding entries in other indexes
    while (pHiddenEntryBuffer) {
      
      pNewIndexTree->vInsertEntriesFromBuffer(pHiddenEntryBuffer, pEntry);
      pHiddenEntryBuffer = pHiddenEntryBuffer->pNextBuffer();
      
      // get the next buffer and first entry in it
      if (pHiddenEntryBuffer)
        pEntry = pHiddenEntryBuffer->pFirstEntry();
    }
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
  
  vDump(CN_TABLE, this, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* create column vector                                                       */
/*----------------------------------------------------------------------------*/
void Table::vCreateColumnVec(s32& sColumnNm, s48K& sColumnDefn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
//  vTrace("vCreateColumnVec: table %s column %s %s\n"_Stz,
//         aTableName(),
//         sColumnNm.aStr(),
//         sColumnDefn.aStr());
  
  Column* pColumn = new Column(sColumnNm.aStr(),sColumnDefn.aStr());
  
  m_pColumnVec->vInsert(pColumn);
  
  // renumber
  for (i8 iColumn = 0; iColumn < m_pColumnVec->nItems(); iColumn++) {
    Column* pColumn = m_pColumnVec->pGet(iColumn);
    if (pColumn->iColumnNo() != -1)
      pColumn->vColumnNo(iColumn);
  }
}

/*----------------------------------------------------------------------------*/
/* get the hidden index from the table                                        */
/*----------------------------------------------------------------------------*/
Index* Table::pHiddenIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_pHiddenIndex;
}

/*----------------------------------------------------------------------------*/
/* set the hidden index in the table                                          */
/*----------------------------------------------------------------------------*/
void Table::vHiddenIndex(Index* pHiddenIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_pHiddenIndex = pHiddenIndex;
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
/* get a partition                                                            */
/*----------------------------------------------------------------------------*/
Partition* Table::pPartition(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Partition* pPart = m_xSlots.pPartition(iSlot);
  
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
/* decrement references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Table::vRefDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  --m_zRefCount;
}

/*----------------------------------------------------------------------------*/
/* increment references to this dictionary                                    */
/*----------------------------------------------------------------------------*/
void Table::vRefInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  ++m_zRefCount;
}

/*----------------------------------------------------------------------------*/
/* test for outstanding references to this dictionary                         */
/*----------------------------------------------------------------------------*/
bool Table::bRefOut(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  u8 iRef = *m_zRefCount;
  
  return iRef ? true : false;
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
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nSlots; iPartition++) {
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
/* hash the first field of row return partition                               */
/*----------------------------------------------------------------------------*/
i8 Table::iRowPartition(Stz* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nCols = m_xSlots.pPtnColVec()->nItems();
  ValWrk xValWrk;
  s256 s;
  i8 iHash = 0;
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    // get the column from the index column vector
    Column* pColumn = m_xSlots.pPtnColVec()->pGet(iColumn);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    // get the column value
    Row* aRow = (Row*)pRow;
    char* sCursor = aRow->aColumnDatum(iColumnNo);
    
    // hash the column value
    DtmLib::vHash(pColumn->eDatumType(),iHash,(Stz*)sCursor,pColumn);
  }
  
  // modulo to get partition number
  i8 iPartition = iHash % (gpAttachMap->m_pGrid->m_nSlots * gpAttachMap->m_pGrid->iNodes());
  //vTrace("Hashed to Partition: %lld\n"_Stz,iPartition);
  
  return iPartition;
}
/*
i8 Table::iRowPartition(Stz* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nCols = m_xSlots.pPtnColVec()->nItems();
  ValWrk xValWrk;
  s256 s;
  i8 iHash = 0;
  
  for (i8 iColumn = 0; iColumn < nCols; iColumn++) {
    
    // get the column from the index column vector
    Column* pColumn = m_xSlots.pPtnColVec()->pGet(iColumn);
    
    // get the column number of the column
    i8 iColumnNo = pColumn->iColumnNo();
    
    char* pDst = s.aChar();
    char* aCursor = reinterpret_cast<char*>(pRow->aChar());
    
    // get the column value from the row for the iColumnNo
    i8 iCol = -1;
    do {
      bool bFindPipe = StrLib::bExtractItem(&aCursor, pDst, ',');
      ASSERT(bFindPipe);
      iCol++;
      aCursor++;
    } while (iColumnNo != iCol);
    
    DtmLib::vHash(pColumn->eDatumType(),iHash,(Stz*)pDst,pColumn);
  }
  
  // modulo to get partition number
  i8 iPartition = iHash % (gpAttachMap->m_pGrid;->m_nSlots * gpAttachMap->m_pGrid;->iNodes());
  
  //vTrace("Partition: %lld\n"_Stz,m_iPartition);
  return iPartition;
}
*/

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
    m_ziSubLoaders[i_Slot] = iSubLoaders;
  
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
void Table::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_xSlots.vTruncate();
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void Table::vTruncateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_xSlots.vTruncateIndex(iIndexNo);
}

/*----------------------------------------------------------------------------*/
/* verify all indexes                                                         */
/*----------------------------------------------------------------------------*/
bool Table::bVerifyIndexes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Partition* pPartition = this->pPartition(0);             //# discuss with Alka
  IdxDataVec* pIdxDataVec = pPartition->pIdxDataVec();
  i8 nIndexes = pIdxDataVec->nItems();

  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    IndexTree* pIndexTree = pIdxDataVec->pGet(i_Index);
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
  
  for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->m_nSlots; iSlot++) {
    
    bool bVerify = true;

    i8 iPartition = Partition::iSlotToPartition(iSlot);
    
    EntryBuffer *pIndexBuffer = pPartition(iSlot)->pIndexTree(pIndex->iIndexNo())->pRoot();
    
    if (pIndexBuffer)
      bVerify = pIndexBuffer->bVerify();
    if ( ! bVerify )
      break;
  }
  
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



/*----------------------------------------------------------------------------*/
/*-- New Index Loader --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* load the given new index from the hidden index using the sort technique    */
/*----------------------------------------------------------------------------*/
/*
void Table::vNewIndexLoad(i8 iPartition, IndexTree* pNewIndexTree,
                          IndexTree* pHiddenIndexTree, i8 nHiddenEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  int iFD;
  s256 sRegionName;
  s32 sRegionMagnitude;
  bool bResult;
  SysCall xSysCall;
  i8 iDebug;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Index* pNewIndex = pNewIndexTree->pIndex();
  if (gxArguments.m_bVerbose) {
    vTrace("Loading index %s on partition %lld\n"_Stz,
         pNewIndex->aIndexName(), iPartition);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute the size of the sort element
  
  i8 iSortElmSize = pNewIndex->iSortKeyDatum() + sizeof(Id);
  if (gxArguments.m_bVerbose) {
    vTrace("Sort element size %lldB\n"_Stz,
           iSortElmSize);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute the size of the sort region
  
  // the size is no. entries * element size + some fudge
  i8 nSortRegionSize = nHiddenEntries * iSortElmSize + C_16KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compute the address of the sort region
  
  char* aSortRegionAddr = pAdd<char*>((void*)C_SORT_REGION_BASE,
                                      iPartition * C_SORT_REGION_DELTA);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the name of the sort region for given partition

  //# add table name
  sRegionName.vSprint("%s_%s_PTN%lld"_Stz,
                      gxArguments.m_sRegionName.aChar(),
                      pNewIndexTree->pIndex()->sIndexName().aChar(),
                      iPartition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the sort region for given partition
  
  if (gxArguments.m_bVerbose) {
    Magnitudes::vMagnitude(nSortRegionSize, sRegionMagnitude);
    vTrace("Creating sort region %s; size %s (%lldB)...\n"_Stz,
           sRegionName.aChar(),
           sRegionMagnitude.aChar(),
           nSortRegionSize);
  }
  
  // open a new shm region
  bResult = Shm::bCreate(sRegionName,
                         nSortRegionSize,
                         iFD,
                         true,
                         xSysCall);
  
  if (gxArguments.m_bVerbose) {
    if (bResult) {
      vTrace("Created sort region %s\n"_Stz,
             sRegionName.aChar());
    }
    else {
     vFatal("Table::vNewIndexLoad: failed to create sort region %s\n"_Stz,
            sRegionName.aChar());
     NEVER_REACH;
    }
  }
  
  // allocate memory for the shm region
  void* aSortRegion = Shm::pMap(iFD,
                                aSortRegionAddr,
                                nSortRegionSize,
                                true,
                                false,
                                xSysCall);
  
  if (gxArguments.m_bVerbose) {
    if (aSortRegion)  {
      vTrace("Mapped sort region %s address %p size %s\n"_Stz,
             sRegionName.aChar(),
             aSortRegionAddr,
             sRegionMagnitude.aChar());
    }
    else {
      Shm::bClose(iFD,xSysCall);
      vFatal("Table::vNewIndexLoad: failed to map sort region\n"_Stz);
      NEVER_REACH;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the sort array
  //pHiddenIndexTree->vDump(true);
  vPopulateSortArray(aSortRegionAddr,nHiddenEntries,iSortElmSize,
                     pNewIndexTree,pHiddenIndexTree->pRoot());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the unsorted array
  
  //vDumpSortArray(aSortRegionAddr,nHiddenEntries,iSortElmSize,pNewIndexTree);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sort the array
  
  vSortTheArray(aSortRegionAddr,nHiddenEntries,iSortElmSize,pNewIndexTree);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the sorted array
  
  //vDumpSortArray(aSortRegionAddr,nHiddenEntries,iSortElmSize,pNewIndexTree);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // build the new index
  
  vBuildNewIndex(aSortRegionAddr,nHiddenEntries,iSortElmSize,pNewIndexTree);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // destroy the sort region
  
  bResult = Shm::bClose(iFD,xSysCall);
  bResult = Shm::bUnmap(aSortRegionAddr,nSortRegionSize,xSysCall);
  bResult = Shm::bUnlink(sRegionName,xSysCall);
 
  iDebug = 2;
  //vTrace("vNewIndexLoad: DONE\n"_Stz);
-------------------------------------------------------------------------------
  
  KeyWrk xKeyWrk;
  for (EntryBuffer* pEB = pHiddenIndexTree->pRoot(); pEB; pEB = pEB->pNextBuffer()) {
    
    for ( Entry* pRow = pEB->pFirstEntry() ; pRow; pRow = pEB->pNextEntry(pRow, 0)) {
      pNewIndexTree->bInsertEntry(pRow);
    }
  }

}
*/
/*----------------------------------------------------------------------------*/
/* compute the number of entries in the hidden index                          */
/*----------------------------------------------------------------------------*/
i8 Table::nHiddenEntryCount(EntryBuffer* pFirstL0Buffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  ASSERT(pFirstL0Buffer == nullptr || 0 == pFirstL0Buffer->iBufferLevel());
  
  i8 nEntries = 0;
  for (EntryBuffer* pEB = pFirstL0Buffer; pEB; pEB = pEB->pNextBuffer())
    nEntries += pEB->nItems();
  
  return nEntries;
}

//#define _COPY_DIRECT
/*----------------------------------------------------------------------------*/
/* populate the sort array for the given index from the hidden index          */
/*----------------------------------------------------------------------------*/
/*
void Table::vPopulateSortArray(char* aSortArray, i8 nElements, i8 iSortElmSize,
                               IndexTree* pNewIndexTree, EntryBuffer* pFirstL0Buffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Timer xTimer;
  xTimer.vStart();
  
  ASSERT(pFirstL0Buffer == nullptr || 0 == pFirstL0Buffer->iBufferLevel());
  ASSERT(sizeof(i8) == sizeof(Id));
  
  Index* pIndex = pNewIndexTree->pIndex();
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  //if (gxArguments.m_bVerbose)
  //  vTrace("vPopulateSortArray for index %s\n"_Stz,
  //         pIndex->aIndexName());
  
  char* aSortElm = aSortArray;
  
  KeyWrk xKeyWrk;
  for (EntryBuffer* pEB = pFirstL0Buffer; pEB; pEB = pEB->pNextBuffer()) {
    
    //Entry* pRow = pEB->pHeadEntry();
    //pRow = pEB->pNextEntry(pRow, 0);
    
    //if (gxArguments.m_bVerbose)
    //  vTrace("vPopulateSortArray for buffer %p\n"_Stz, pEB);
    
#ifdef _COPY_DIRECT
    for ( ; pRow; pRow = pEB->pNextEntry(pRow)) {
      
      // get the key datum for this index
      aSortElm = pRow->pExtractSortKey(aSortElm,pIdxColVec);
      
      // set the id into the sort element
      *((Id*)aSortElm) = pRow->xId();
      aSortElm = pAdd<char*>(aSortElm,sizeof(Id));
      
    }
  }

#else
  for ( Entry* pRow = pEB->pFirstEntry() ; pRow; pRow = pEB->pNextEntry(pRow, 0)) {

      // get the key datum for this index
      xKeyWrk.vReset();
      pRow->vExtractSortKey(xKeyWrk,pIdxColVec);
      
      // copy the key datum into the sort element
      MemLib::vCopy(aSortElm,xKeyWrk.aEmbedded(),xKeyWrk.nKeySize(),CN_TABLE);
      aSortElm = pAdd<char*>(aSortElm,xKeyWrk.nKeySize());
      
      // set the id into the sort element
      *((Id*)aSortElm) = pRow->xId();
      aSortElm = pAdd<char*>(aSortElm,sizeof(Id));
      
    }
  }
#endif
  
  xTimer.vUpdate();
  
  s32 sTimeMagnitude;
  Magnitudes::vTimeMag(xTimer.iNanoseconds(),sTimeMagnitude);
  
  if (gxArguments.m_bVerbose)
    vTrace("vPopulateSortArray for index %s took %s\n"_Stz,
           pIndex->aIndexName(),
           sTimeMagnitude.aChar());
}
*/

/*----------------------------------------------------------------------------*/
/* the qsort compare function for a complex key datum                         */
/*----------------------------------------------------------------------------*/
/*
#ifdef __APPLE__
int DBobjects::iQsort_complex(void* aIndexTree, const void* aArg1, const void* aArg2)
#else
int DBobjects::iQsort_complex(const void* aArg1, const void* aArg2, void* aIndexTree)
#endif
{
  IndexTree* pIndexTree = _pIndexTree(aIndexTree);
  Index* pIndex = pIndexTree->pIndex();
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  
  int iCmp = 0;
  i8 iCompare = 0;
  
  char* aCursorThis = (char*)aArg1;
  char* aCursorThat = (char*)aArg2;

  char* aDatumThis;
  char* aDatumThat;

  i8 nBytesThis;
  i8 nBytesThat;
  
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    eDM eDatum = pColumn->eDatumType();
    
    if (DM_VCH == eDatum) {
      aDatumThis = *(char**)aCursorThis;
      aCursorThis = pAdd<char*>(aCursorThis,8);
      nBytesThis = Vch::iSize(aDatumThis);
      
      aDatumThat = *(char**)aCursorThat;
      aCursorThat = pAdd<char*>(aCursorThat,8);
      nBytesThat = Vch::iSize(aDatumThat);
    }
    else {
      aDatumThis = Key::aEachDatum(pColumn,nBytesThis,&aCursorThis);
      aDatumThat = Key::aEachDatum(pColumn,nBytesThat,&aCursorThat);
    }
    
    i8 iCompare = pColumn->iCmp(aDatumThis,nBytesThis,
                              aDatumThat,nBytesThat);
    
    if (iCompare < 0)
      iCmp = -1;
    else if (iCompare > 0)
      iCmp = 1;
    else
      iCmp = 0;
    
    //s256 s1,s2;
    //Dte::vSprint(s1, (Stz*)aDatumThis);
    //Dte::vSprint(s2, (Stz*)aDatumThat);
    //vTrace("%lld %s %lld %s %lld %d\n"_Stz, aDatumThis, s1.aChar(), aDatumThat, s2.aChar(),iCompare, iCmp);
    
    if (iCmp)
      break;
  }
  
  return iCmp;
}
*/

/*----------------------------------------------------------------------------*/
/* the qsort compare function for an {i8} key datum                           */
/*----------------------------------------------------------------------------*/
/*
int DBobjects::iQsort_i8(const void* aArg1, const void* aArg2)
{
  
  //vTrace("%lld, %lld\n"_Stz, *((i8*)aArg1), *((i8*)aArg2));
  
  // check for nulls first
  if (((Int*)aArg1)->bIsNull())
    return -1;
  
  if (((Int*)aArg2)->bIsNull())
    return 1;
  
  // check for min
  if (*((i8*)aArg1) == C_I8_MIN) {
    if (*((i8*)aArg2) == C_I8_MIN)
      return 0;
    else
      return -1;
  }
  
  if (*((i8*)aArg2) == C_I8_MIN) {
    return 1;
  }
  
  i8 iCmp = *((i8*)aArg1) - *((i8*)aArg2);
  
  if (iCmp > 0)
    return 1;
  
  if (iCmp < 0)
    return -1;
  
  return 0;
}
*/

/*----------------------------------------------------------------------------*/
/* the qsort compare function for an {i8,i8} key datum                        */
/*----------------------------------------------------------------------------*/
/*
int DBobjects::iQsort_i8i8(const void* aArg1, const void* aArg2)
{
  i8* pArg1a = (i8*)aArg1;
  i8* pArg2a = (i8*)aArg2;
  
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Int*)pArg1a)->bIsNull())
  return -1;
  
  if (((Int*)pArg2a)->bIsNull())
  return 1;
  
  //----------------------------------------------------------------------------
  // check for min
  if (*pArg1a == C_I8_MIN) {
    if (*pArg2a == C_I8_MIN)
      return 0;
    else
      return -1;
  }
  
  if (*pArg2a == C_I8_MIN) {
    return 1;
  }

  //----------------------------------------------------------------------------
  // now compare the integers
  i8 iCmpA = *pArg1a - *pArg2a;
  
  if (iCmpA > 0)
    return 1;
  
  if (iCmpA < 0)
    return -1;
  
  //----------------------------------------------------------------------------
  // move to next integer
  
  i8* pArg1b = pArg1a + 1;
  i8* pArg2b = pArg2a + 1;

  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Int*)pArg1b)->bIsNull())
  return -1;
  
  if (((Int*)pArg2b)->bIsNull())
  return 1;
  
  //----------------------------------------------------------------------------
  // check for min
  if (*pArg1b == C_I8_MIN) {
    if (*pArg2b == C_I8_MIN)
      return 0;
    else
      return -1;
  }
  
  if (*pArg2b == C_I8_MIN) {
    return 1;
  }
  
  //----------------------------------------------------------------------------
  i8 iCmpB = *pArg1b - *pArg2b;
  
  if (iCmpB > 0)
    return 1;
  
  if (iCmpB < 0)
    return -1;
  
  return 0;
}
*/

/*----------------------------------------------------------------------------*/
/* the qsort compare function for an {i8,i8,i8} key datum                     */
/*----------------------------------------------------------------------------*/
/*
int DBobjects::iQsort_i8i8i8(const void* aArg1, const void* aArg2)
{
  i8* pArg1a = (i8*)aArg1;
  i8* pArg2a = (i8*)aArg2;
  
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Int*)pArg1a)->bIsNull())
    return -1;
  
  if (((Int*)pArg2a)->bIsNull())
    return 1;
  
  //----------------------------------------------------------------------------
  // check for min
  if (*pArg1a == C_I8_MIN) {
    if (*pArg2a == C_I8_MIN)
      return 0;
    else
      return -1;
  }
  
  if (*pArg2a == C_I8_MIN) {
    return 1;
  }
  
  //----------------------------------------------------------------------------
  // now compare the integers
  i8 iCmpA = *pArg1a - *pArg2a;
  
  if (iCmpA > 0)
    return 1;
  
  if (iCmpA < 0)
    return -1;
  
  //----------------------------------------------------------------------------
  // move to next integer
  
  i8* pArg1b = pArg1a + 1;
  i8* pArg2b = pArg2a + 1;
  
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Int*)pArg1b)->bIsNull())
    return -1;
  
  if (((Int*)pArg2b)->bIsNull())
    return 1;
  
  //----------------------------------------------------------------------------
  // check for min
  if (*pArg1b == C_I8_MIN) {
    if (*pArg2b == C_I8_MIN)
      return 0;
    else
      return -1;
  }
  
  if (*pArg2b == C_I8_MIN) {
    return 1;
  }
  
  //----------------------------------------------------------------------------
  i8 iCmpB = *pArg1b - *pArg2b;
  
  if (iCmpB > 0)
    return 1;
  
  if (iCmpB < 0)
    return -1;
  
  //----------------------------------------------------------------------------
  // move to next integer
  
  i8* pArg1c = pArg1b + 1;
  i8* pArg2c = pArg2b + 1;
  
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Int*)pArg1c)->bIsNull())
    return -1;
  
  if (((Int*)pArg2c)->bIsNull())
    return 1;
  
  //----------------------------------------------------------------------------
  // check for min
  if (*pArg1c == C_I8_MIN) {
    if (*pArg2c == C_I8_MIN)
      return 0;
    else
      return -1;
  }
  
  if (*pArg2c == C_I8_MIN) {
    return 1;
  }
  
  //----------------------------------------------------------------------------
  i8 iCmpC = *pArg1c - *pArg2c;
  
  if (iCmpC > 0)
    return 1;
  
  if (iCmpC < 0)
    return -1;

  return 0;
}
*/

/*----------------------------------------------------------------------------*/
/* qsort the sort array for the given index                                   */
/*----------------------------------------------------------------------------*/
/*
void Table::vSortTheArray(char* aSortArray, i8 nElements, i8 nElmSize,
                          IndexTree* pNewIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Timer xTimer;
  xTimer.vStart();

  Index* pIndex = pNewIndexTree->pIndex();
  eIF eIndexForm = pIndex->eIndexForm();
  
  if (IF_I8 == eIndexForm)
    qsort(aSortArray,nElements,nElmSize,iQsort_i8);
  else
  if (IF_I8_I8 == eIndexForm)
    qsort(aSortArray,nElements,nElmSize,iQsort_i8i8);
  else
  if (IF_I8_I8_I8 == eIndexForm)
    qsort(aSortArray,nElements,nElmSize,iQsort_i8i8i8);
  
#ifdef __APPLE__
    qsort_r(aSortArray,nElements,nElmSize,(void*)pNewIndexTree,iQsort_complex);
#else
    qsort_r(aSortArray,nElements,nElmSize,iQsort_complex,(void*)pNewIndexTree);
#endif
  xTimer.vUpdate();
  
  s32 sTimeMagnitude;
  Magnitudes::vTimeMag(xTimer.iNanoseconds(),sTimeMagnitude);
  
  if (gxArguments.m_bVerbose) {
    Index* pIndex = pNewIndexTree->pIndex();
    vTrace("qsort for index %s took %s\n"_Stz,
           pIndex->aIndexName(),
           sTimeMagnitude.aChar());
  }
}
*/

/*----------------------------------------------------------------------------*/
/* dump the sort array                                                        */
/*----------------------------------------------------------------------------*/
//void Table::vDumpSortArray(char* aSortArray, i8 nElements, i8 nElmSize,
//                           IndexTree* pNewIndexTree)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
//  
//  Index* pIndex = pNewIndexTree->pIndex();
//  eIF eIndexForm = pIndex->eIndexForm();
//  char* aSortElm = aSortArray;
//  
//  i8 iKeyDatum0;
//  i8 iKeyDatum1;
//  
//  vTrace("-- Sort Array -------------------\n"_Stz);
//         
//  for (i8 i_Elm = 0; i_Elm < nElements; i_Elm++) {
//    
//    if (IF_I8 == eIndexForm) {
//      iKeyDatum0 = *((i8*)aSortElm);
//      aSortElm = pAdd<char*>(aSortElm,sizeof(i8));
//    
//      Id xId = *((Id*)aSortElm);
//      aSortElm = pAdd<char*>(aSortElm,sizeof(Id));
//    
//      vTrace("[%lld] %lld.%lld : %lld\n"_Stz,
//             i_Elm,
//             xId.iBuffer(),
//             xId.iRow(),
//             iKeyDatum0);
//    }
//    else
//    if (IF_I8_I8 == eIndexForm) {
//      iKeyDatum0 = *((i8*)aSortElm);
//      aSortElm = pAdd<char*>(aSortElm,sizeof(i8));
//      iKeyDatum1 = *((i8*)aSortElm);
//      aSortElm = pAdd<char*>(aSortElm,sizeof(i8));
//      
//      Id xId = *((Id*)aSortElm);
//      aSortElm = pAdd<char*>(aSortElm,sizeof(Id));
//      
//      vTrace("[%lld] %lld.%lld : %lld,%lld\n"_Stz,
//             i_Elm,
//             xId.iBuffer(),
//             xId.iRow(),
//             iKeyDatum0,
//             iKeyDatum1);
//    }
//  }
//  
//  vTrace("---------------------------------\n"_Stz);
//}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
void Table::vDumpSortArray(char* aSortArray, i8 nElements, i8 nElmSize,
                           IndexTree* pNewIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Index* pIndex = pNewIndexTree->pIndex();
  IdxColVec* pIdxColVec = pIndex->pIdxColVec();
  eIF eIndexForm = pIndex->eIndexForm();
  char* aSortElm = aSortArray;
  
  Id xId;
  s48K sSortKey;
  
  i8 nDatums = (nElmSize - sizeof(Id)) / sizeof(w8);
  ASSERT(pIdxColVec->nItems() == nDatums);
  
  vTrace("-- Sort Array -------------------\n"_Stz);
  
  for (i8 i_Elm = 0; i_Elm < nElements; i_Elm++) {
    
    Key* aSortKey = (Key*)aSortElm;
    aSortElm = pAdd<char*>(aSortElm,nElmSize - sizeof(Id));
    
    Id xId = *((Id*)aSortElm);
    aSortElm = pAdd<char*>(aSortElm,sizeof(Id));
    
    aSortKey->vSprintSortKey(sSortKey,pIdxColVec);
    
    vTrace("[%lld] %lld.%lld : %s\n"_Stz,
           i_Elm,
           xId.iBuffer(),
           xId.iRow(),
           sSortKey.aChar());
  }
  
  vTrace("---------------------------------\n"_Stz);
}
*/
/*----------------------------------------------------------------------------*/
/* build the new index                                                        */
/*----------------------------------------------------------------------------*/
/*
void Table::vBuildNewIndex(char* aSortArray, i8 nElements, i8 nElmSize,
                           IndexTree* pNewIndexTree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Timer xTimer;
  xTimer.vStart();
  
  if (gxArguments.m_bVerbose)
    vTrace("-- Build New Index --------------\n"_Stz);
  
  KeyWrk xKeyWrk;
  EntryVec xPriorVec;
  IdxColVec* pIdxColVec = pNewIndexTree->pIndex()->pIdxColVec();
  bool bUniqueIndex = pNewIndexTree->pIndex()->bIsUnique();
  
  i8 nDatums = (nElmSize - sizeof(Id)) / sizeof(w8);
  ASSERT(pIdxColVec->nItems() == nDatums);
  
  // step thru each element in the sort array
  char* aSortElm = aSortArray;
  Key* aPrevKey = nullptr;
  
  for (i8 i_Elm = 0; i_Elm < nElements; i_Elm++) {
    
    // get sort key
    Key* aSortKey = (Key*)aSortElm;
    aSortElm = pAdd<char*>(aSortElm,nElmSize - sizeof(i8));
    
    // get id
    Id xId = *((Id*)aSortElm);
    aSortElm = pAdd<char*>(aSortElm,sizeof(Id));
    
//    if (i_Elm % 10000 == 0) {
//      vTrace("[%lld] %p\n"_Stz,
//           i_Elm,
//           pRow);
//    }
    
    // put key datum into a keywrk                    //# do we need to do this?
    //KeyWrk xKeyWrk((Key*)aSortKey,pIdxColVec);
    vBuildKeyWrk(aSortKey,pIdxColVec,xKeyWrk);
    
    if (bUniqueIndex) {
      if (aPrevKey) {
        
        if (aPrevKey->bEqualTo(aSortKey, pIdxColVec)) {
          vTrace("Found duplicate\n"_Stz);
          
        }
      }
      aPrevKey = aSortKey;
    }
    
    xKeyWrk.vId(xId);
    
    // append an entry into index structure
    pNewIndexTree->vAppendEntry(xKeyWrk,
                                //xId,
                                //xKeyWrk.xId(),
                                xPriorVec);
  }

  if (gxArguments.m_bVerbose)
    vTrace("---------------------------------\n"_Stz);
  
  xTimer.vUpdate();
  
  s32 sTimeMagnitude;
  Magnitudes::vTimeMag(xTimer.iNanoseconds(),sTimeMagnitude);
  
  if (gxArguments.m_bVerbose) {
    Index* pIndex = pNewIndexTree->pIndex();
    vTrace("vBuildNewIndex for index %s took %s\n"_Stz,
           pIndex->aIndexName(),
           sTimeMagnitude.aChar());
  }
}
*/

/*----------------------------------------------------------------------------*/
/* build a new key work from given sort key                                   */
/*----------------------------------------------------------------------------*/
/*
void Table::vBuildKeyWrk(Key* aSortKey, IdxColVec* pIdxColVec, KeyWrk& rKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  rKeyWrk.vReset();
  
  char* aSortElm = (char*)aSortKey;
  for (i8 i_Column = 0; i_Column < pIdxColVec->nItems(); i_Column++) {
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    eDM eDatum = pColumn->eDatumType();
    
    if (DM_VCH == eDatum) {
      char* aDatum = *(char**)aSortElm;
      if (((Vch*)aDatum)->bIsNull())
        rKeyWrk.vAddWord(C_NULL_VCH);
      else
        rKeyWrk.vAddDatum(aDatum);
    }
    else {
      w8 wDatum = *(w8*)aSortElm;
      rKeyWrk.vAddWord(wDatum);
    }
    
    aSortElm = pAdd<char*>(aSortElm,8);
  }
}
*/
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
