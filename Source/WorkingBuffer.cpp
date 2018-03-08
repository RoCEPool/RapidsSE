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
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  m_bIsLast      = false;
  m_iPartition   = iPartition;
  m_nWorkingRows = 0;
  m_pTable       = pTable;
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append row                                                                 */
/*----------------------------------------------------------------------------*/
bool WorkingBuffer::bAppendRow(Row* aRow, i8 nRowSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  i8 nWorkingRowSize = WorkingRow::nBytesRequired(nRowSize);
  
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < nWorkingRowSize)
    return false;
  
  WorkingRow* aWorkingRow = pAllotRow(nWorkingRowSize);
  
  new(aWorkingRow) WorkingRow(aRow,nWorkingRowSize,nRowSize);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* append row                                                                 */
/*----------------------------------------------------------------------------*/
bool WorkingBuffer::bAppendRow(RowWrk xRowWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  i8 nWorkingRowSize = WorkingRow::nBytesRequired(xRowWrk.nRowSize());
  
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < nWorkingRowSize)
    return false;
  
  WorkingRow* aWorkingRow = pAllotRow(nWorkingRowSize);
  
  new(aWorkingRow) WorkingRow(xRowWrk.aRow(),nWorkingRowSize,xRowWrk.nRowSize());
  
  //vShow();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* WorkingBuffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  char* aStart = m_xWsp.aArea();
  
  return aStart;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
WorkingRow* WorkingBuffer::pFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  //if ( m_nWorkingRows == 0 )
  //  return nullptr;
  
  if (nUsedBytes() == 0)
    return nullptr;
  
  WorkingRow* pFirst = reinterpret_cast<WorkingRow*>(m_xWsp.aArea());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void WorkingBuffer::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool WorkingBuffer::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
WorkingRow* WorkingBuffer::pNextRow(WorkingRow* pCurrentItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  i8 nBytes = pCurrentItem->nRowSize();
  
  WorkingRow* pNext = pAdd<WorkingRow*>(pCurrentItem,nBytes);
  
  if ((char*)pNext >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* get the partition                                                          */
/*----------------------------------------------------------------------------*/
i8 WorkingBuffer::nPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void WorkingBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  vItemCountRestore();
}

/*----------------------------------------------------------------------------*/
/* get number of result rows in this buffer                                   */
/*----------------------------------------------------------------------------*/
i8 WorkingBuffer::nWorkingRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  return m_nWorkingRows;
}

/*----------------------------------------------------------------------------*/
/* show the contents of the buffer                                            */
/*----------------------------------------------------------------------------*/
void WorkingBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  ColumnVec* pColVec = m_pTable->pColumnVec();
  
  WorkingRow* pWorkingRow;
  for (pWorkingRow = pFirstRow();
       pWorkingRow;
       pWorkingRow = pNextRow(pWorkingRow)) {
    
    pWorkingRow->vShow(pColVec);
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 WorkingBuffer::nUsedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
WorkingRow* WorkingBuffer::pAllotRow(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  WorkingRow* pWorkingRow = reinterpret_cast<WorkingRow*>(m_xWsp.aAllot(nBytes));
  
  m_nWorkingRows++;
  
  return pWorkingRow;
}

/*----------------------------------------------------------------------------*/
/* allot space of nBytes in this buffer                                       */
/*----------------------------------------------------------------------------*/
WorkingRow* WorkingBuffer::pAllotSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  WorkingRow* pWorkingRow = reinterpret_cast<WorkingRow*>(m_xWsp.aAllot(nBytes));
  
  m_nWorkingRows++;
  
  return pWorkingRow;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 WorkingBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* restore item count                                                         */
/*----------------------------------------------------------------------------*/
void WorkingBuffer::vItemCountRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WORKINGBUFFER,_FOOTER_);
    
  m_nWorkingRows = 0;
  for (WorkingRow* pRow = pFirstRow(); pRow; pRow = pNextRow(pRow))
    m_nWorkingRows++;
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
