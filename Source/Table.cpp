/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include "DBobjects.h"
#include "Word.hpp"
#include "Arguments.hpp"

extern BufferPool*  gpBufferPool;
extern Arguments    gxArguments;
extern Grid* gpGrid;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Table* DBobjects::_pTable(void* pAddr)
{
  Table* pTable = reinterpret_cast<Table*>(pAddr);
  
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
  //m_xRing
  //m_xSlots
  
  m_zRefCount.vCheckInit();
  
  _checkInitialised_(CN_TABLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(const char* pTableName)
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
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*analyze table                                                               */
/*----------------------------------------------------------------------------*/
char* Table::pAnalyze(bool bDetailed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 nRowsTotal = 0;
  i8 nBuffersTotal = 0;
  i8 nStrandsTotal = 0;
  s48K sAnalysis;
  
  String::vSprintCat(sAnalysis.a(),"N%lld: Analysis of Table %s\n",gpGrid->iThisNode(),m_sTableName.a());

  for (i8 iSlot = 0; iSlot < gxArguments.m_nSlots; iSlot++) {
    
    i8 iPartition = Partition::iSlotToPartition(iSlot);
    
    //i8 iNode = Partition::iPartitionToNode(iPartition);
    //if (iNode != gxArguments.m_iNode)
    //  continue;
    
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
          String::vSprintCat(sAnalysis.a(),
              "   strand:%lld rowBuffer%lld: %lld rows\n", nStrands, nRowBuffer, nRowsInBuffer);
        }
      }
      
      nBuffers += pStrand->nRowBuffers();
      nRows += pStrand->nRows();
      pStrand = pStrand->pNextStrand();
    }
    
    String::vSprintCat(sAnalysis.a(),"  partition %lld: %3lld strands; %5lld buffers; %lld rows\n",
           iPartition,nStrands,nBuffers,nRows);
    nRowsTotal += nRows;
    nBuffersTotal += nBuffers;
    nStrandsTotal += nStrands;
  }
  String::vSprintCat(sAnalysis.a(),"        total: %3lld strands; %5lld buffers; %lld rows\n",
         nStrandsTotal,nBuffersTotal,nRowsTotal);
  
  return sAnalysis.a();
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
/* hash the first field of row return partition                               */
/*----------------------------------------------------------------------------*/
i8 Table::iRowPartition(char* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  // calculate length of first field
  i8 iLength = 0;
  char* pChar = pRow;
  while ('|' != *pChar++)
    iLength++;
  
  // hash the field using xxhash
  i8 iHash = XXH32(pRow,iLength,0);
  
  // modulo to get partition number
  i8 iPartition = iHash % (gpGrid->m_nSlots * gpGrid->iNodes());
  
  //vTrace("Partition: %lld\n",m_iPartition);
  return iPartition;
}

/*----------------------------------------------------------------------------*/
/* get number of row buffers in this table                                    */
/*----------------------------------------------------------------------------*/
i8 Table::nRowBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;

  for (i8 iPartition = 0; iPartition < gxArguments.m_nSlots; iPartition++) {
    RowStrand *pStrand = pPartition(iPartition)->pData()->pHeadStrand();
    while (pStrand) {
      iBuffers += pStrand->nRowBuffers();
      pStrand = pStrand->pNextStrand();
    }
  }
  
  return iBuffers;
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
/* vDump                                                                      */
/*----------------------------------------------------------------------------*/
void Table::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vDump(CN_TABLE, this, pSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump table                                                                 */
/*----------------------------------------------------------------------------*/
void Table::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_TABLE);
  ASSERT(pObject->eClass() == CN_TABLE);
  
  Table* pTable = reinterpret_cast<Table*>(pObject);
  
  vTrace("name %s type %s\n",
         pTable->m_sTableName.a(),
         pTT[pTable->m_eTableType]);
}

/*----------------------------------------------------------------------------*/
/* insert a row in the table                                                  */
/*----------------------------------------------------------------------------*/
void Table::vInsertRow(i8 iPartition, TxRef* pTxRef, char* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  Partition* pPartn = pPartition(iPartition);
  
  bool bInserted = pPartn->pData()->bInsertRow(pTxRef,pRecord,iLength);
  
  ASSERT(bInserted);
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
/* get the table name                                                         */
/*----------------------------------------------------------------------------*/
char* Table::pTableName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return m_sTableName.a();
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
