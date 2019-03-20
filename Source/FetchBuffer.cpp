/*============================================================================*/
/* FetchBuffer.cpp -- Fetch Buffer                                            */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FetchBuffer* Jobs::_pFetchBuffer(void* aAddr)
{
  FetchBuffer* pFetchBuffer = reinterpret_cast<FetchBuffer*>(aAddr);
  
  return pFetchBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new result buffer from buffer pool                                 */
/*----------------------------------------------------------------------------*/
void* FetchBuffer::operator new(size_t)
{
  PoolBuffer* pBuffer = (PoolBuffer*)HeapMem::pAllocate(CN_FETCHBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* FetchBuffer::operator new(size_t, void* aAddr)
{
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a result buffer to the buffer pool                                  */
/*----------------------------------------------------------------------------*/
void FetchBuffer::operator delete(void* pBufferAddress)
{
  PoolBuffer* pBuffer = static_cast<PoolBuffer*>(pBufferAddress);
  
  HeapMem::vFree_NonObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FetchBuffer::FetchBuffer(Table* pTable, i8 iPartition)
: Object(CN_FETCHBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  m_bIsLast      = false;
  m_iPartition   = iPartition;
  m_nFetchedRows = 0;
  m_pTable       = pTable;
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_FETCHBUFFER);
#endif
  
  _checkInitialised_(CN_FETCHBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FetchBuffer::~FetchBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append row                                                                 */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ROWBUFFER
bool FetchBuffer::bAppendRow(CacheLine* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  i8 iCacheLineSize = sizeof(CacheLine*);
  
  // do we have enough space for this row ?
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < iCacheLineSize)
    return false;
  
  // allocate row
  CacheLine** ppFetchRow = (CacheLine**)aaAllotRowPtr(iCacheLineSize);
  
  // copy the cacheLine pointer
  *ppFetchRow = pRow;
  
  //pRow->vShow(m_pTable->pColumnVec(), true);
  //vTrace(" %p \n"_Stz, pRow);
  
  return true;
}

#else
bool FetchBuffer::bAppendRow(RowData* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  i8 iRowIdSize = sizeof(RowData*);
  
  // do we have enough space for this row ?
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < iRowIdSize)
    return false;

  // allocate row
  RowData** ppFetchRow = aaAllotRowPtr(iRowIdSize);
  
  // copy the row id
  *ppFetchRow = pRow;
  
  //pRow->vShow(m_pTable->pColumnVec(), true);
  //vTrace(" %p \n"_Stz, pRow);
  
  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* FetchBuffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  char* aStart = m_xWsp.aArea();
  
  return aStart;
}

/*----------------------------------------------------------------------------*/
/* get number of fetched rows in this buffer                                  */
/*----------------------------------------------------------------------------*/
i8 FetchBuffer::nFetchedRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  return m_nFetchedRows;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void FetchBuffer::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool FetchBuffer::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ROWBUFFER
/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
CacheLine** FetchBuffer::aaFirstRowPtr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  if (nUsedBytes() == 0)
    return nullptr;
  
  CacheLine** aaFirstRow = reinterpret_cast<CacheLine**>(m_xWsp.aArea());
  
  return aaFirstRow;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
CacheLine** FetchBuffer::aaNextRowPtr(CacheLine** ppCurrentRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  CacheLine** aaNextRow = aAdd<CacheLine**>(ppCurrentRow,sizeof(CacheLine*));
  
  if ((char**)aaNextRow >= (char**)m_xWsp.aLatter())
    return nullptr;
  
  return aaNextRow;
}

#else

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
RowData** FetchBuffer::aaFirstRowPtr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  if (nUsedBytes() == 0)
    return nullptr;
  
  RowData** aaFirstRow = reinterpret_cast<RowData**>(m_xWsp.aArea());
  
  return aaFirstRow;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
RowData** FetchBuffer::aaNextRowPtr(RowData** ppCurrentRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  RowData** aaNextRow = aAdd<RowData**>(ppCurrentRow,sizeof(RowData*));
  
  if ((char**)aaNextRow >= (char**)m_xWsp.aLatter())
    return nullptr;
  
  return aaNextRow;
}

#endif

/*----------------------------------------------------------------------------*/
/* get the partition                                                          */
/*----------------------------------------------------------------------------*/
i8 FetchBuffer::nPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void FetchBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  vItemCountRestore();
}

/*----------------------------------------------------------------------------*/
/* show the contents of the buffer                                            */
/*----------------------------------------------------------------------------*/
void FetchBuffer::vShow(bool bCompressed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);

//  for (RowData** aaRow = aaFirstRowPtr();
//       aaRow;
//       aaRow = aaNextRowPtr(aaRow)) {
//
//    (*aaRow)->vShow(m_pTable->pColumnVec(),true);
//    vTrace("\n"_Stz);
//  }
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 FetchBuffer::nUsedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot space for a row ptr in this buffer                                   */
/*----------------------------------------------------------------------------*/
RowData** FetchBuffer::aaAllotRowPtr(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  RowData** aaRow = reinterpret_cast<RowData**>(m_xWsp.aAllot(nBytes));
  
  m_nFetchedRows++;
  
  return aaRow;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 FetchBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* restore item count                                                         */
/*----------------------------------------------------------------------------*/
void FetchBuffer::vItemCountRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);

//  m_nFetchedRows = 0;
//  for (RowData** aaRow = aaFirstRowPtr(); aaRow; aaRow =aaNextRowPtr(aaRow))
//    m_nFetchedRows++;

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* FetchBuffer::aFooter(void)
{
  _poolAddress_(this);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
