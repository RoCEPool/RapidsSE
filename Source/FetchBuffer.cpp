/*============================================================================*/
/* FetchBuffer.cpp -- Fetch Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FetchBuffer* DBobjects::_pFetchBuffer(void* aAddr)
{
  FetchBuffer* pFetchBuffer = reinterpret_cast<FetchBuffer*>(aAddr);
  
  return pFetchBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new result buffer from buffer pool                                 */
/*----------------------------------------------------------------------------*/
void* FetchBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_FETCHBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* FetchBuffer::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a result buffer to the buffer pool                                  */
/*----------------------------------------------------------------------------*/
void FetchBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  Heap::vFree_NonObject(pBuffer);
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
  
#ifdef __DEBUG_CHECKS
  m_xWsp.vZero(CN_FETCHBUFFER);
#endif
  
  _checkInitialised_(CN_FETCHBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FetchBuffer::~FetchBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append row                                                                 */
/*----------------------------------------------------------------------------*/
bool FetchBuffer::bAppendRow(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  i8 iRowIdSize = sizeof(Id);
  
  // do we have enough space for this row ?
  i8 nFreeSpace = nFreeBytes();
  if (nFreeSpace < iRowIdSize)
    return false;

  // allocate row
  Id* pRowId = pAllotRowId(iRowIdSize);
  
  // copy the row id
  *pRowId = xRowId;
  
  return true;
}

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
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
Id* FetchBuffer::pFirstRowId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  if (nUsedBytes() == 0)
    return nullptr;
  
  Id* pFirstRowId = reinterpret_cast<Id*>(m_xWsp.aArea());
  
  // get the referenced buffer
  //EntryBuffer* pEntryBuffer = xId.pReferencedEntryBuffer();
  
  // get the row entry
  //Entry* pRowEntry = pEntryBuffer->pGetEntryRowNo(xId.iRow());
  
  return pFirstRowId;
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
Id* FetchBuffer::pNextRowId(Id* pCurrentId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  Id* pNextRowId= pAdd<Id*>(pCurrentId,sizeof(Id));
  
  if ((char*)pNextRowId >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNextRowId;
}

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

  Id* pRowId;
  for (pRowId = pFirstRowId();
       pRowId;
       pRowId = pNextRowId(pRowId)) {
    
    Id xRowId = *pRowId;
    
    // get the referenced buffer
    EntryBuffer* pEntryBuffer = xRowId.pReferencedEntryBuffer();
    
    // get the row entry
    Entry* pEntry = pEntryBuffer->pGetEntryRowNo(xRowId.iRow());
    
    // dump the row
    pEntry->vShow(0,m_pTable->pHiddenIndex(),false);
    vTrace("\n"_Stz);
  }
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
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
Id* FetchBuffer::pAllotRowId(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHBUFFER,_FOOTER_);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytes)
    return nullptr;
  
  Id* pRowId = reinterpret_cast<Id*>(m_xWsp.aAllot(nBytes));
  
  m_nFetchedRows++;
  
  return pRowId;
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

  m_nFetchedRows = 0;
  for (Id* pRowId = pFirstRowId(); pRowId; pRowId = pNextRowId(pRowId))
    m_nFetchedRows++;

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
