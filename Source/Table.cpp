/*============================================================================*/
/*  Table.cpp - Table                                                         */
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
: Segment(CN_TABLE)
{
  _poolAddress_(this);
  _magic_();
  
  Word* pWord = Word::pToWord(this);
  //.vTrace("Table name: %s\n",pWord->sName().a());
  
  m_iTxId        = -1;
  m_sTableName   = pWord->sName();
  m_eTableType   = TT_NONE;
  m_pCurrentRow  = nullptr;
  m_nParitionMgr = 0;
  m_nColumnMgr   = 0;
  m_nIndexMgr    = 0;
  
  _class_(CN_TABLE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Table::Table(i8 iTxId, const char* pTableName)
: Segment(CN_TABLE)
{
  _poolAddress_(this);
  _magic_();
  
  m_iTxId = iTxId;
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
/* copy given table into this one                                             */
/*----------------------------------------------------------------------------*/
void Table::vCopy(Table *pTable)
{
  _poolAddress_(this);

  SegmentBuffer* pFromBuffer = pTable->pFirstBuffer();
  
  while (pFromBuffer) {
    
    vTrace("FromBuffer %p\n",pFromBuffer);
    
    vExtendSpace();

    SegmentBuffer* pToBuffer = pLastBuffer();
    
    pToBuffer->vCopy(pFromBuffer);
    
    pFromBuffer = pFromBuffer->pNextBuffer();
  }
}

/*----------------------------------------------------------------------------*/
/* export table into file                                                     */
/*----------------------------------------------------------------------------*/
void Table::vExport(File* pFile)
{
  _poolAddress_(this);
  
  Object* pCurrent;
  
  for (pCurrent = pFirstObject(); pCurrent; ) {
    StrVal* pStrVal = reinterpret_cast<StrVal*>(pCurrent);
    //vTrace("Row %p: %s\n",pStrVal,pStrVal->sStr());
    pFile->vPrint("%s\n",pStrVal->sStr());
    pCurrent = pNextObject(pCurrent,pStrVal->iSize());
  }
}

/*----------------------------------------------------------------------------*/
/* get first segment buffer                                                   */
/*----------------------------------------------------------------------------*/
SegmentBuffer* Table::pFirstBuffer(void)
{
  _poolAddress_(this);
  
  return Segment::pFirstBuffer();
}

/*----------------------------------------------------------------------------*/
/* insert a row in the table                                                  */
/*----------------------------------------------------------------------------*/
void Table::vInsertRow(StrVal* pRowStr)
{
  _poolAddress_(this);

  i8 nBytes = pRowStr->iSize();
  
  Object* pRow = pAllocObject(nBytes);

  ASSERT(pRow);
  
  Memory::vCopy(pRow,pRowStr,nBytes);
}

/*----------------------------------------------------------------------------*/
/* load file into tabl                                                        */
/*----------------------------------------------------------------------------*/
void Table::vLoad(File* pFile)
{
  _poolAddress_(this);
  
   ValWsp xBuffer;
  
  char* y = xBuffer.pAsStrVal()->sStr();
  
  if (! gpBufferPool->bHasDataSpace()) {
    vTrace("Buffer pool low\n");
    return;
  }
    
  while (pFile->bReadLine(y)) {
    xBuffer.pAsStrVal()->vSize();

    if (! gpBufferPool->bHasDataSpace()) {
      vTrace("Buffer pool low\n");
      return;
    }
    
    vInsertRow(xBuffer.pAsStrVal());
  }
}

/*----------------------------------------------------------------------------*/
/* list all rows                                                              */
/*----------------------------------------------------------------------------*/
void Table::vList(void)
{
  _poolAddress_(this);

  Object* pCurrent;
  
  for (pCurrent = pFirstObject(); pCurrent; ) {
    StrVal* pStrVal = reinterpret_cast<StrVal*>(pCurrent);
    vTrace("Row %p: %s\n",pStrVal,pStrVal->sStr());
    pCurrent = pNextObject(pCurrent,pStrVal->iSize());
  }
}

/*----------------------------------------------------------------------------*/
/* release all rows/buffers                                                   */
/*----------------------------------------------------------------------------*/
void Table::vTruncate(void)
{
  _poolAddress_(this);

  vReleaseAll();
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
/* get the txid                                                               */
/*----------------------------------------------------------------------------*/
i8 Table::iTxId(void) const
{
  _poolAddress_(this);
  
  return m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* vDump                                                                      */
/*----------------------------------------------------------------------------*/
void Table::vDump(void) 
{
  _poolAddress_(this);
  
  vTrace("Table %p: TxId %lld TableName %s\n",
         this,
         m_iTxId,
         m_sTableName.a()
         );
  //m_pRowAllocator->vDump();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/