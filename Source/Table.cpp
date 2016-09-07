/*============================================================================*/
/* Table.cpp -- Table                                                         */
/*============================================================================*/

#include "DBobjects.h"
#include "Word.hpp"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Table::operator new(size_t iSize)
{
  Table* pTable = nullptr;
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Table::operator new(size_t iSize, Table* pTable)
{
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Table::operator delete(void* pTableAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(void)
: Object(CN_TABLE)
{
  _poolAddress_(this);
  _magic_();
  
  Word* pWord = Word::pToWord(this);
  
  m_sTableName = pWord->sName();
  m_eTableType = TT_NORMAL;
  
  //m_xDictionary
  //m_xRing
  //m_xPartitions
  
  Memory::vCheckInitialised(this,sizeof(Table));
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(const char* pTableName)
: Object(CN_TABLE)
{
  _poolAddress_(this);
  _magic_();
  
  m_sTableName = pTableName;
  
  _class_(CN_TABLE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Table::~Table(void)
{
  _poolAddress_(this);
}

/*----------------------------------------------------------------------------*/
/* get number of row buffers in this table                                    */
/*----------------------------------------------------------------------------*/
i8 Table::iRowBuffers(void)
{
  _poolAddress_(this);
  _class_(CN_TABLE);
  
  i8 iBuffers = 0;

  RowStrand *pStrand = pPartition(0)->pHeadStrand();
  while (pStrand) {
    iBuffers += pStrand->iRowBuffers();
    pStrand = pStrand->pNextStrand();
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* get dictionary                                                             */
/*----------------------------------------------------------------------------*/
Dictionary* Table::pDic(void)
{
  _poolAddress_(this);
  
  return &m_xDictionary;
}

/*----------------------------------------------------------------------------*/
/* vDump                                                                      */
/*----------------------------------------------------------------------------*/
void Table::vDump(void)
{
  _poolAddress_(this);
  
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
}

/*----------------------------------------------------------------------------*/
/* insert a row in the table                                                  */
/*----------------------------------------------------------------------------*/
void Table::vInsertRow(i8 iPartition, TxRef* pTxRef, StrVal* pRowStr)
{
  _poolAddress_(this);

  Partition* pPartn = pPartition(iPartition);
  
  bool bInserted = pPartn->bInsertRow(pTxRef,pRowStr);
  
  ASSERT(bInserted);
}

/*----------------------------------------------------------------------------*/
/* release all rows/buffers                                                   */
/*----------------------------------------------------------------------------*/
void Table::vTruncate(void)
{
  _poolAddress_(this);

  m_xPartitions.vTruncate();
}

/*----------------------------------------------------------------------------*/
/* get the table name                                                         */
/*----------------------------------------------------------------------------*/
char* Table::pTableName(void)
{
  _poolAddress_(this);
  
  return m_sTableName.a();
}

/*----------------------------------------------------------------------------*/
/* get a partition                                                            */
/*----------------------------------------------------------------------------*/
Partition* Table::pPartition(i8 iPartition)
{
  _poolAddress_(this);

  Partition* pPart = m_xPartitions.pPartition(iPartition);
  
  return pPart;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/