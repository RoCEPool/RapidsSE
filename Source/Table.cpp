/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include "DBobjects.h"

//#include "Word.hpp"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
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
  
  //m_xDictionary

  //m_xSlots
  
  m_zRefCount.vCheckInit();
  
  m_pColumnVec = new ColumnVec;
  
  _checkInitialised_(CN_TABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(Str* pTableName)
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
  
  delete m_pColumnVec;
}

/*----------------------------------------------------------------------------*/
/* dump table                                                                 */
/*----------------------------------------------------------------------------*/
void Table::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_TABLE);
  ASSERT(pObject->eClass() == CN_TABLE);
  
  Table* pTable = reinterpret_cast<Table*>(pObject);
  
  vTrace("name %s type %s\n"_Str,
         pTable->m_sTableName.s(),
         aTT[pTable->m_eTableType]->s());
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
Str* Table::pAnalyze(bool bDetailed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nRowsTotal = 0;
  i8 nBuffersTotal = 0;
  i8 nStrandsTotal = 0;
  s48K sAnalysis;
  
  sAnalysis.vCatSprint("N%lld: Analysis of Table %s\n"_Str,
                       gpGrid->iThisNode(),
                       m_sTableName.s());

  for (i8 iSlot = 0; iSlot < gpGrid->m_nSlots; iSlot++) {
    
    i8 iPartition = Partition::iSlotToPartition(iSlot);
    
    RowStrand *pStrand = pPartition(iSlot)->pData()->pHeadStrand();
    i8 nRows = 0;
    i8 nBuffers = 0;
    i8 nStrands = 0;
    while (pStrand) {
      nStrands++;
      
      if (bDetailed) {
        RowBuffer* pRowBuffer = pStrand->pFirstBuffer();
        i8 nRowsInBuffer = 0;
        i8 nRowBuffer = 0;
        while (pRowBuffer) {
          nRowBuffer++;
          nRowsInBuffer = pRowBuffer->nRows();
          pRowBuffer = pRowBuffer->pNextBuffer();
          sAnalysis.vCatSprint("   strand:%lld rowBuffer%lld: %lld rows\n"_Str,
                               nStrands,
                               nRowBuffer,
                               nRowsInBuffer);
        }
      }
      
      nBuffers += pStrand->nRowBuffers();
      nRows += pStrand->nRows();
      pStrand = pStrand->pNextStrand();
    }
    
    sAnalysis.vCatSprint(
      "  partition %lld: %3lld strands; %5lld buffers; %lld rows\n"_Str,
      iPartition,
      nStrands,
      nBuffers,
      nRows);
    
    nRowsTotal += nRows;
    nBuffersTotal += nBuffers;
    nStrandsTotal += nStrands;
  }
  sAnalysis.vCatSprint(
    "        total: %3lld strands; %5lld buffers; %lld rows\n"_Str,
    nStrandsTotal,
    nBuffersTotal,
    nRowsTotal);
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* append row buffer                                                          */
/*----------------------------------------------------------------------------*/
void Table::vAppendBuffer(i8 iPartition, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  ASSERT(iPartition < gpGrid->m_nSlots);
  
  // mark buffer as permanent
  pRowBuffer->vDurability(RD_PERMANENT);
  
  Partition* pPartition = this->pPartition(iPartition);
 
  pPartition->pData()->vAppendBuffer(pRowBuffer);
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
void Table::vCreateColumnVec(Dic* pDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vTrace("vCreateColumnVec: table %s\n"_Str,aTableName());
  
  for (Word* pWord = pDic->pLastWord(); pWord; pWord = pWord->pPrevWord()) {
    vTrace("vCreateColumnVec: word %s %s\n"_Str,
           pWord->sName().aStr(),
           aWT[pWord->eWordType()]);
  }
}

/*----------------------------------------------------------------------------*/
/* insert a row in the table                                                  */
/*----------------------------------------------------------------------------*/
void Table::vInsertRow(i8 iPartition, TxRef* pTxRef, Str* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  Partition* pPartn = pPartition(iPartition);
  
  bool bInserted = pPartn->pData()->bInsertRow(pTxRef,pRecord,iLength);
  
  ASSERT(bInserted);
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
/* get number of row buffers in this table                                    */
/*----------------------------------------------------------------------------*/
i8 Table::nRowBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;
  
  for (i8 iPartition = 0; iPartition < gpGrid->m_nSlots; iPartition++) {
    RowStrand *pStrand = pPartition(iPartition)->pData()->pHeadStrand();
    while (pStrand) {
      iBuffers += pStrand->nRowBuffers();
      pStrand = pStrand->pNextStrand();
    }
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* hash the first field of row return partition                               */
/*----------------------------------------------------------------------------*/
i8 Table::iRowPartition(Str* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // calculate length of first field
  i8 iLength = 0;
  char* pChar = (char*)pRow;
  while ('|' != *pChar++)
    iLength++;
  
  // hash the field using xxhash
  i8 iHash = XXH32(pRow,iLength,0);
  
  // modulo to get partition number
  i8 iPartition = iHash % (gpGrid->m_nSlots * gpGrid->iNodes());
  
  //vTrace("Partition: %lld\n"_Str,m_iPartition);
  return iPartition;
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
Str* Table::aTableName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_sTableName;
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Table::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
