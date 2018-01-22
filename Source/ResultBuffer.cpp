/*============================================================================*/
/* ResultBuffer.cpp -- Result Buffer                                          */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ResultBuffer* DBobjects::_pResultBuffer(void* aAddr)
{
  ResultBuffer* pResultBuffer = reinterpret_cast<ResultBuffer*>(aAddr);
  
  return pResultBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new result buffer from buffer pool                                 */
/*----------------------------------------------------------------------------*/
void* ResultBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_RESULTBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* ResultBuffer::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a result buffer to the buffer pool                                  */
/*----------------------------------------------------------------------------*/
void ResultBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  Heap::vFree_NonObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ResultBuffer::ResultBuffer(Table* pTable, i8 iPartition)
{
  _poolAddress_(this);
  
  m_bIsLast     = false;
  m_iPartition  = iPartition;
  m_nResultRows = 0;
  m_pTable      = pTable;
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vZero(CN_ITEMBUFFER);
#endif
  
  _checkInitialised_(CN_RESULTBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ResultBuffer::~ResultBuffer(void)
{
  _poolAddress_(this);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append result row                                                          */
/*----------------------------------------------------------------------------*/
bool ResultBuffer::bAppendResult(Row* aRow, i8 nRowSize)
{
  _poolAddress_(this);
  
  i8 nResultRowSize = ResultRow::nBytesRequired(nRowSize);
  
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < nResultRowSize)
    return false;
  
  ResultRow* aResultRow = pAllotResult(nResultRowSize);
  
  new(aResultRow) ResultRow(aRow,nResultRowSize,nRowSize);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pFirstResult(void)
{
  _poolAddress_(this);
  
  if ( m_nResultRows == 0 )
    return nullptr;
  
  ResultRow* pFirst = reinterpret_cast<ResultRow*>(m_xWsp.aArea());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void ResultBuffer::vIsLast(bool bIsLast)
{
  _poolAddress_(this);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool ResultBuffer::bIsLast(void)
{
  _poolAddress_(this);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pNextResult(ResultRow* pCurrentItem)
{
  _poolAddress_(this);
  
  i8 nBytes = pCurrentItem->nResultRowSize();
  
  ResultRow* pNext = pAdd<ResultRow*>(pCurrentItem,nBytes);
  
  if ((char*)pNext >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* get the partition                                                          */
/*----------------------------------------------------------------------------*/
i8 ResultBuffer::nPartitionNo(void)
{
  _poolAddress_(this);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* get number of result rows in this buffer                                   */
/*----------------------------------------------------------------------------*/
i8 ResultBuffer::nResultRows(void)
{
  _poolAddress_(this);
  
  return m_nResultRows;
}

/*----------------------------------------------------------------------------*/
/* show the contents of the buffer                                            */
/*----------------------------------------------------------------------------*/
void ResultBuffer::vShow(void)
{
  _poolAddress_(this);
  
  ColumnVec* pColVec = m_pTable->pColumnVec();
  
  ResultRow* pResultRow;
  for (pResultRow = pFirstResult();
       pResultRow;
       pResultRow = pNextResult(pResultRow)) {
    
    pResultRow->vShow(pColVec);
    vTrace("\n"_Stz);
  }
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pAllotResult(i8 nBytes)
{
  _poolAddress_(this);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  ResultRow* pResultRow = reinterpret_cast<ResultRow*>(m_xWsp.aAllot(nBytes));
  
  m_nResultRows++;
  
  return pResultRow;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 ResultBuffer::nFreeBytes(void)
{
  _poolAddress_(this);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ResultBuffer::aFooter(void)
{
  _poolAddress_(this);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
