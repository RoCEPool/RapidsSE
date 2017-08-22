/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include "DBobjects.h"

//#include "Word.hpp"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern bool  gbMetadata;
extern Grid* gpGrid;


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
  
  m_pIdxVec = new IdxVec;
  
  m_pColumnVec = new ColumnVec;
  
  // create the hidden column for the hidden index here
  Column* pColumn = new Column("hidden_column"_Stz);
  pColumn->vColumnNo(-1);
  pColumn->vColumnType(CT_INTEGER);
  m_pColumnVec->vAdd(pColumn);
  
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
  
  // create an IndexData for the index in each partition
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    Partition* pPartition = this->pPartition(iPartition);
    pPartition->vAddIndex(new IndexData(pIndex, iPartition));
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
  
  if ( ! gbMetadata || bDetailed )
    sAnalysis.vCatSprint("N%lld: Analysis of Index %s of table %s \n"_Stz,
                       gpGrid->iThisNode(),
                       pIndex->aIndexName()->aChar(),
                       m_sTableName.aChar());
  
  for (i8 iSlot = 0; iSlot < gpGrid->m_nSlots; iSlot++) {
    
    i8 iPartition = Partition::iSlotToPartition(iSlot);
    
    EntryBuffer *pIndexBuffer = pPartition(iSlot)->pIndexData(pIndex->iIndexNo())->pRoot();
    
    i8 nBuffers = 0;
    i8 nEntries = 0;

    if (pIndexBuffer)
      pIndexBuffer->vAnalyze(sAnalysis, bDetailed, nBuffers, nEntries);
    
    nEntriesTotal += nEntries;
    nBuffersTotal += nBuffers;
    
    if ( !gbMetadata || bDetailed ) {
      sAnalysis.vCatSprint("partition %lld:  %3lld buffers; %5lld entries\n\n"_Stz,
                           iPartition,
                           nBuffers,
                           nEntries);
    } else {
      sAnalysis.vCatSprint("row: ('%s',%lld,%lld,%lld)\n"_Stz,
                           gpGrid->pThisNode()->aNodeName(),
                           iPartition,
                           nBuffers,
                           nEntries);

    }
    
  }
  
  if ( ! gbMetadata  || bDetailed)
    sAnalysis.vCatSprint(
                         "        total: %3lld buffers; %5lld entries\n"_Stz,
                         nBuffersTotal,
                         nEntriesTotal);
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* append row buffer                                                          */
/*----------------------------------------------------------------------------*/
void Table::vAppendBuffer(i8 iPartition, EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  ASSERT(iPartition < gpGrid->m_nSlots);
  
  // mark buffer as permanent
  pEntryBuffer->vDurability(BD_PERMANENT);
  
  // first append buffer to the hidden index
  Partition* pPartition = this->pPartition(iPartition);
  IndexData* pIndexData = pPartition->pHiddenIndexData();
  pIndexData->vAppendBuffer(pEntryBuffer);
  
  // now iterate thru the buffer and insert entries in all other indexes
  
  // get no of indexes of the table
  IdxDataVec* pIdxDataVec = pPartition->pIdxDataVec();
  i8 nIndexes = pIdxDataVec->nItems();
  
  Entry* pEntry = pEntryBuffer->pFirstRow();
  while( (pEntry = pEntryBuffer->pNextRow(pEntry)) ) {
    
    // insert an entry for the row in all the indexes of this table
    for (i8 i_Index=0; i_Index < nIndexes; i_Index++) {
      
      IndexData* pIndexData = pIdxDataVec->pGet(i_Index);
      Index* pIndex = pIndexData->pIndex();
      
      // skip the hidden index, as already inserted row in it
      if ( ! pIndex->bIsHidden())
        pIndexData->vInsertEntry(pEntry);
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
void Table::vPopulateIndex(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  m_xSlots.vPopulateIndex(iIndexNo);
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
  
  // delete the IndexData of the index from each partition
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    Partition* pPartition = this->pPartition(iPartition);
    pPartition->vAddIndex(new IndexData(pIndex, iPartition));
  }
  
  // remove index from the table
  m_pIdxVec->vRemove(pIndex);
}

/*----------------------------------------------------------------------------*/
/* get number of row buffers in this table                                    */
/*----------------------------------------------------------------------------*/
i8 Table::nRowBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;
  
//  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
//    EbStrand *pStrand = pPartition(iPartition)->pData()->pHeadStrand();
//    while (pStrand) {
//      iBuffers += pStrand->nEntryBuffers();
//      pStrand = pStrand->pNextStrand();
//    }
//  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* get number of index buffers in this table                                  */
/*----------------------------------------------------------------------------*/
i8 Table::nIndexBuffers(i8 iIndexNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;
  
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    IndexData* pIndexData = pPartition(iPartition)->pIndexData(iIndexNo);

    iBuffers += pIndexData->nIndexBuffers();
  }
  
  return iBuffers;
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
    char* sCursor = RowData::pColumn(pRow->aChar(), iColumnNo);
    
    // hash the column value
    DtmLib::vHash(pColumn->eDatumType(),iHash,(Stz*)sCursor,pColumn);
  }
  
  // modulo to get partition number
  i8 iPartition = iHash % (gpGrid->m_nSlots * gpGrid->iNodes());
  
  //vTrace("Partition: %lld\n"_Stz,m_iPartition);
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
      bool bFindPipe = StrLib::bExtractItem(&aCursor, pDst, '|');
      ASSERT(bFindPipe);
      iCol++;
      aCursor++;
    } while (iColumnNo != iCol);
    
    DtmLib::vHash(pColumn->eDatumType(),iHash,(Stz*)pDst,pColumn);
  }
  
  // modulo to get partition number
  i8 iPartition = iHash % (gpGrid->m_nSlots * gpGrid->iNodes());
  
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
