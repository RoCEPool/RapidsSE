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
  
  i8 iBuffers = 0;

  RowStrand *pStrand = pPartition(0)->pHeadStrand();
  while (pStrand) {
    iBuffers += pStrand->iRowBuffers();
    pStrand = pStrand->pNextStrand();
  }
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* copy given table into this one                                             */
/*----------------------------------------------------------------------------*/
void Table::vCopy(Table *pFromTable)
{
  _poolAddress_(this);
  
  Row* pRow;
  
  RowCursor* pRowCursor = new RowCursor;
  
  pRowCursor->vTable(pFromTable);
  
  pRowCursor->vBegin(0);
  pRowCursor->vFirst();
  while ( (pRow = pRowCursor->pRow()) ) {
    //vTrace("Copying %s\n",((StrVal*)pRow)->sStr());
    vInsertRow(0,nullptr,(StrVal*)pRow->pData());
    pRowCursor->vNext();
  }
  
  delete pRowCursor;
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
/* export table into file                                                     */
/*----------------------------------------------------------------------------*/
void Table::vExport(File* pFile)
{
  _poolAddress_(this);
  
  Row* pRow;
  
  RowCursor* pRowCursor = new RowCursor;
  
  pRowCursor->vTable(this);
  
  pRowCursor->vBegin(0);
  pRowCursor->vFirst();
  while ( (pRow = pRowCursor->pRow()) ) {
    //vTrace("Exporting %s\n",((StrVal*)pRow)->sStr());
    pFile->vPrint("%s\n",((StrVal*)pRow->pData())->sStr());
    pRowCursor->vNext();
  }
  
  delete pRowCursor;
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
/* load file into tabl                                                        */
/*----------------------------------------------------------------------------*/
void Table::vLoad2(File* pFile)
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
    
    vInsertRow(0,nullptr,xBuffer.pAsStrVal());
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void Table::vLoad(File* pFile)
{
  _poolAddress_(this);

  //
  // IN PROGRESS
  //
  
  static const i8 C_CHUNK = C_256KB;
  char  xWorkspace[C_CHUNK];
  
  char* pReadInto;
  char* pScan;
  char* pNext;
  char* pEnd;
  char* z;
  
  i8 iBytesRead;
  i8 iBytesToRead;
  i8 iBytesLeftover;
  i8 iRowBytes;
  
  ValWsp xRow;
  xRow.vClass(CN_STRVAL);
  
  // process file in chunks
  iBytesLeftover = 0;
  pScan = xWorkspace;
  
  for (;;) {
  
    // how much to read
    iBytesToRead = C_CHUNK - iBytesLeftover;
    
    // where to read into to
    pReadInto = pcAdd(xWorkspace,iBytesLeftover);
   
    // read next (partial) chunk
    iBytesRead = pFile->iReadChunk(pReadInto,iBytesToRead);
    if ( ! iBytesRead )
      break;
    
    iBytesLeftover += iBytesRead;
    pEnd = &xWorkspace[iBytesLeftover];

    z = xRow.pAsStrVal()->sStr();
    pNext = xWorkspace;
    pScan = pNext;
    iRowBytes = 0;
    while (pScan < pEnd) {

      //      vTrace("%c\n",*pScan);
      
      // test for eol
      if ('\n' == *pScan) {
        // got a line/row
        *z = '\0';
        xRow.pAsStrVal()->vSize();
        //vTrace("Row: %s\n",xRow.sStr());
        vInsertRow(0,nullptr,xRow.pAsStrVal());
        
        // continue with next line/row
        pNext = ++pScan;
        iBytesLeftover -= (iRowBytes + 1);
        iRowBytes = 0;
        z = xRow.pAsStrVal()->sStr();
        continue;
      }
      
      *z++ = *pScan++;
      iRowBytes++;
    }
    
    if (iBytesLeftover) {
      Memory::vCopy(xWorkspace,pNext,iBytesLeftover);
    }
  }
  
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