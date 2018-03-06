/*============================================================================*/
/* WorkingBuffer.cpp -- Working Buffer                                        */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
WorkingBuffer* DBobjects::_pWorkingBuffer(void* aAddr)
{
  WorkingBuffer* pWorkingBuffer = reinterpret_cast<WorkingBuffer*>(aAddr);
  
  return pWorkingBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new result buffer from buffer pool                                 */
/*----------------------------------------------------------------------------*/
void* WorkingBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_WORKINGBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* WorkingBuffer::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a result buffer to the buffer pool                                  */
/*----------------------------------------------------------------------------*/
void WorkingBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  Heap::vFree_NonObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
WorkingBuffer::WorkingBuffer(Table* pTable, i8 iPartition)
: Object(CN_WORKINGBUFFER)
{
  _poolAddress_(this); _footer_();
  
  m_bIsLast     = false;
  m_iPartition  = iPartition;
  m_nResultRows = 0;
  m_pTable      = pTable;
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vZero(CN_WORKINGBUFFER);
#endif
  
  _checkInitialised_(CN_WORKINGBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WorkingBuffer::~WorkingBuffer(void)
{
  _poolAddress_(this); _footer_();
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append result row                                                          */
/*----------------------------------------------------------------------------*/
bool WorkingBuffer::bAppendResult(Row* aRow, i8 nRowSize)
{
  _poolAddress_(this); _footer_();
  
  i8 nResultRowSize = ResultRow::nBytesRequired(nRowSize);
  
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < nResultRowSize)
    return false;
  
  ResultRow* aResultRow = pAllotResult(nResultRowSize);
  
  new(aResultRow) ResultRow(aRow,nResultRowSize,nRowSize);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* append row                                                                 */
/*----------------------------------------------------------------------------*/
bool WorkingBuffer::bAppendRow(RowWrk xRowWrk)
{
  _poolAddress_(this); _footer_();
  
  i8 nResultRowSize = ResultRow::nBytesRequired(xRowWrk.nRowSize());
  
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < nResultRowSize)
    return false;
  
  ResultRow* aResultRow = pAllotResult(nResultRowSize);
  
  new(aResultRow) ResultRow(xRowWrk.aRow(),nResultRowSize,xRowWrk.nRowSize());
  
  vShow();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
ResultRow* WorkingBuffer::pFirstResult(void)
{
  _poolAddress_(this); _footer_();
  
  if ( m_nResultRows == 0 )
    return nullptr;
  
  ResultRow* pFirst = reinterpret_cast<ResultRow*>(m_xWsp.aArea());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void WorkingBuffer::vIsLast(bool bIsLast)
{
  _poolAddress_(this); _footer_();
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool WorkingBuffer::bIsLast(void)
{
  _poolAddress_(this); _footer_();
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
ResultRow* WorkingBuffer::pNextResult(ResultRow* pCurrentItem)
{
  _poolAddress_(this); _footer_();
  
  i8 nBytes = pCurrentItem->nResultRowSize();
  
  ResultRow* pNext = pAdd<ResultRow*>(pCurrentItem,nBytes);
  
  if ((char*)pNext >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* get the partition                                                          */
/*----------------------------------------------------------------------------*/
i8 WorkingBuffer::nPartitionNo(void)
{
  _poolAddress_(this); _footer_();
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* get number of result rows in this buffer                                   */
/*----------------------------------------------------------------------------*/
i8 WorkingBuffer::nResultRows(void)
{
  _poolAddress_(this); _footer_();
  
  return m_nResultRows;
}

/*----------------------------------------------------------------------------*/
/* show the contents of the buffer                                            */
/*----------------------------------------------------------------------------*/
void WorkingBuffer::vShow(void)
{
  _poolAddress_(this); _footer_();
  
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
ResultRow* WorkingBuffer::pAllotResult(i8 nBytes)
{
  _poolAddress_(this); _footer_();
  
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
i8 WorkingBuffer::nFreeBytes(void)
{
  _poolAddress_(this); _footer_();
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* WorkingBuffer::aFooter(void)
{
  _poolAddress_(this);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
