/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include "DBobjects.h"
#include "Word.hpp"
#include "Arguments.hpp"

extern BufferPool*  gpBufferPool;
extern Arguments    gxArguments;

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
  //m_xPartitions
  
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
void Table::vAnalyze(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  vTrace("Analysis of Table %s\n",m_sTableName.a());
}

/*----------------------------------------------------------------------------*/
/* append row buffer                                                          */
/*----------------------------------------------------------------------------*/
void Table::vAppendBuffer(i8 iPartition, RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  ASSERT(iPartition < gxArguments.m_nPartitions);
  
  Partition* pPartition = this->pPartition(iPartition);
 
  pPartition->vAppendBuffer(pRowBuffer);
  
}

/*----------------------------------------------------------------------------*/
/* get number of row buffers in this table                                    */
/*----------------------------------------------------------------------------*/
i8 Table::iRowBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  i8 iBuffers = 0;

  for (i8 iPartition = 0; iPartition < gxArguments.m_nPartitions; iPartition++) {
    RowStrand *pStrand = pPartition(iPartition)->pHeadStrand();
    while (pStrand) {
      iBuffers += pStrand->iRowBuffers();
      pStrand = pStrand->pNextStrand();
    }
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* get dictionary                                                             */
/*----------------------------------------------------------------------------*/
Dic* Table::pDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);
  
  return &m_xDic;
}

/*----------------------------------------------------------------------------*/
/* vDump                                                                      */
/*----------------------------------------------------------------------------*/
void Table::vDump(void)
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
  
  vTrace("%sTable %p:\n", pTab, pTable);
  vTrace("%s  name %s \n", pTab, pTable->m_sTableName.a());
  vTrace("%s  type %s \n", pTab, pTT[pTable->m_eTableType]);
}

/*----------------------------------------------------------------------------*/
/* insert a row in the table                                                  */
/*----------------------------------------------------------------------------*/
void Table::vInsertRow(i8 iPartition, TxRef* pTxRef, char* pRecord, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  Partition* pPartn = pPartition(iPartition);
  
  bool bInserted = pPartn->bInsertRow(pTxRef,pRecord,iLength);
  
  ASSERT(bInserted);
}

/*----------------------------------------------------------------------------*/
/* release all rows/buffers                                                   */
/*----------------------------------------------------------------------------*/
void Table::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  m_xPartitions.vTruncate();
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
Partition* Table::pPartition(i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TABLE,_FOOTER_);

  Partition* pPart = m_xPartitions.pPartition(iPartition);
  
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
