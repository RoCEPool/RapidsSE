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
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  m_nItems = 0;
  //m_xWsp
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vZero(CN_ITEMBUFFER);
#endif
  
  m_bIsLast    = false;
  m_pTable     = pTable;
  m_iPartition = iPartition;
  
  //_postcondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_RESULTBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ResultBuffer::~ResultBuffer(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pAllotResult(i8 nBytes)
{
  //_precondition_(_ADDRESS_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  ResultRow* pResultRow = reinterpret_cast<ResultRow*>(m_xWsp.aAllot(nBytes));
  
  m_nItems++;
  
  return pResultRow;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* ResultBuffer::aDataArea(void)
{
  //_precondition_(_ADDRESS_);
  
  char* aStart = m_xWsp.aArea();
  
  return aStart;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pFirstResult(void)
{
  //_precondition_(_ADDRESS_);
  
  if ( m_nItems == 0 )
    return nullptr;
  
  ResultRow* pFirst = reinterpret_cast<ResultRow*>(m_xWsp.aArea());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 ResultBuffer::nFreeBytes(void)
{
  //_precondition_(_ADDRESS_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of items in the buffer                                          */
/*----------------------------------------------------------------------------*/
i8 ResultBuffer::nItems(void)
{
  //_precondition_(_ADDRESS_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pNextResult(ResultRow* pCurrentItem)
{
  //_precondition_(_ADDRESS_);
  
  i8 nBytes = pCurrentItem->nResultRowSize();
  
  ResultRow* pNext = pAdd<ResultRow*>(pCurrentItem,nBytes);
  
  if ((char*)pNext >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 ResultBuffer::nUsedBytes(void)
{
  //_precondition_(_ADDRESS_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/* append row                                                                 */
/*----------------------------------------------------------------------------*/
bool ResultBuffer::bAppendRow(Row* aRow, i8 nRowSize)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  i8 nResultRowSize = ResultRow::nBytesRequired(nRowSize);
  
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < nResultRowSize)
    return false;
  
  ResultRow* aResultRow = pAllotResult(nResultRowSize);

  new(aResultRow) ResultRow(aRow,nResultRowSize,nRowSize);  
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* first result row                                                           */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pFirstRow(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_);
  
  ResultRow* pFirst = pFirstResult();
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void ResultBuffer::vIsLast(bool bIsLast)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool ResultBuffer::bIsLast(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
ResultRow* ResultBuffer::pNextRow(ResultRow* pCurrentRow)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  ResultRow* pNext = pNextResult(pCurrentRow);
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* get the partition                                                          */
/*----------------------------------------------------------------------------*/
i8 ResultBuffer::nPartitionNo(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* show the contents of the buffer                                            */
/*----------------------------------------------------------------------------*/
void ResultBuffer::vShow(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER,_FOOTER_);

  ColumnVec* pColVec = m_pTable->pColumnVec();
  
  ResultRow* pResultRow;
  for (pResultRow = pFirstRow();
       pResultRow;
       pResultRow = pNextRow(pResultRow)) {

    pResultRow->vShow(pColVec);
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ResultBuffer::aFooter(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_RESULTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
