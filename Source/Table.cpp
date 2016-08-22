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
  
  i8 iBuffers = pPartition(0)->iBufferCount();
  
  return iBuffers;
}

/*----------------------------------------------------------------------------*/
/* copy given table into this one                                             */
/*----------------------------------------------------------------------------*/
void Table::vCopy(Table *pFromTable)
{
  _poolAddress_(this);
  
  Object* pRow;
  
  RowCursor* pRowCursor = new RowCursor;
  
  pRowCursor->vTable(pFromTable);
  
  pRowCursor->vBegin();
  pRowCursor->vFirst();
  while ( (pRow = pRowCursor->pRow()) ) {
    //vTrace("Copying %s\n",((StrVal*)pRow)->sStr());
    vInsertRow(0,(StrVal*)pRow);
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
/* export table into file                                                     */
/*----------------------------------------------------------------------------*/
void Table::vExport(File* pFile)
{
  _poolAddress_(this);
  
  Object* pRow;
  
  RowCursor* pRowCursor = new RowCursor;
  
  pRowCursor->vTable(this);
  
  pRowCursor->vBegin();
  pRowCursor->vFirst();
  while ( (pRow = pRowCursor->pRow()) ) {
    //vTrace("Exporting %s\n",((StrVal*)pRow)->sStr());
    pFile->vPrint("%s\n",((StrVal*)pRow)->sStr());
    pRowCursor->vNext();
  }
  
  delete pRowCursor;
}

/*----------------------------------------------------------------------------*/
/* get first segment buffer                                                   */
/*----------------------------------------------------------------------------*/
SegmentBuffer* Table::pFirstBuffer(void)
{
  _poolAddress_(this);
  
  return pPartition(0)->pFirstBuffer();
}

/*----------------------------------------------------------------------------*/
/* insert a row in the table                                                  */
/*----------------------------------------------------------------------------*/
void Table::vInsertRow(i8 iPartition, StrVal* pRowStr)
{
  _poolAddress_(this);

  Partition* pPartn = pPartition(iPartition);
  
  bool bInserted = pPartn->bInsertRow(pRowStr);
  
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
    
    vInsertRow(0,xBuffer.pAsStrVal());
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
        vInsertRow(0,xRow.pAsStrVal());
        
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
/* list all rows                                                              */
/*----------------------------------------------------------------------------*/
void Table::vList(void)
{
  _poolAddress_(this);

  Object* pCurrent;
  
  for (pCurrent = pPartition(0)->pFirstObject(); pCurrent; ) {
    StrVal* pStrVal = reinterpret_cast<StrVal*>(pCurrent);
    vTrace("Row %p: %s\n",pStrVal,pStrVal->sStr());
    pCurrent = pPartition(0)->pNextObject(pCurrent,pStrVal->iSize());
  }
}

/*----------------------------------------------------------------------------*/
/* release all rows/buffers                                                   */
/*----------------------------------------------------------------------------*/
void Table::vTruncate(void)
{
  _poolAddress_(this);

  pPartition(0)->vReleaseAll();
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
/* vDump                                                                      */
/*----------------------------------------------------------------------------*/
void Table::vDump(void) 
{
  _poolAddress_(this);
  
  vTrace("Table %p: %s\n",
         this,
         m_sTableName.a()
         );
  //m_pRowAllocator->vDump();
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