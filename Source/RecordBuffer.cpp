/*============================================================================*/
/* RecordBuffer.cpp -- Record Buffer                                          */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RecordBuffer* DBobjects::_pRecordBuffer(void* aAddr)
{
  RecordBuffer* pRecordBuffer = reinterpret_cast<RecordBuffer*>(aAddr);
  
  return pRecordBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new result buffer from buffer pool                                 */
/*----------------------------------------------------------------------------*/
void* RecordBuffer::operator new(size_t)
{
  Buffer* pBuffer = (Buffer*)HeapMem::pAllocate(CN_RECORDBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* RecordBuffer::operator new(size_t, void* aAddr)
{
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a result buffer to the buffer pool                                  */
/*----------------------------------------------------------------------------*/
void RecordBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  HeapMem::vFree_NonObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RecordBuffer::RecordBuffer(void)
: Object(CN_RECORDBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);

  m_nRecords   = 0;
  m_nFreeBytes = C_SIZE_LARGE_OBJECT - C_1KB;
  //m_cArea
  
  //_checkInitialised_(CN_RECORDBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RecordBuffer::~RecordBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append record                                                              */
/*----------------------------------------------------------------------------*/
bool RecordBuffer::bAppendRecord(char* pSrcRecord, i8 nRecordSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  i4 iRecordSize = (i4)nRecordSize;
  i4 nTotalRecordSize = iRecordSize + C_SIZEOF_RECORDSIZE;
  
  // do we have enough space for this row ?
  if ( m_nFreeBytes < nTotalRecordSize )
    return false;
  
  // allocate space
  char* pDstRecord = pAllotSpace(nTotalRecordSize);
  
  // copy the size of the row
  MemLib::vCopy(pDstRecord,&iRecordSize,C_SIZEOF_RECORDSIZE,CN_RECORDBUFFER);
  
  // copy the compressed row to working row in uncompressed form
  MemLib::vCopy(pDstRecord+C_SIZEOF_RECORDSIZE, pSrcRecord,
                nRecordSize, CN_RECORDBUFFER);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
char* RecordBuffer::pFirstRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  if (m_nFreeBytes == 0)
    return nullptr;
  
  char* pFirst = &m_cArea[0];
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
char* RecordBuffer::pNextRecord(char* pCurrentRecord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  i4 nBytes = *((i4*)pCurrentRecord);
  
  char* aNext = aAdd<char*>(pCurrentRecord,nBytes+C_SIZEOF_RECORDSIZE);
  
  if ((char*)aNext >= (char*)&m_cArea[C_SIZE_LARGE_OBJECT - C_1KB - m_nFreeBytes])
    return nullptr;
  
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* get number of result rows in this buffer                                   */
/*----------------------------------------------------------------------------*/
i8 RecordBuffer::nRecords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  return m_nRecords;
}

/*----------------------------------------------------------------------------*/
/* show the contents of the buffer                                            */
/*----------------------------------------------------------------------------*/
void RecordBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  vTrace("RecordBuffer %p Records %lld\n"_Stz,
         this,
         m_nRecords);
  
  char* pRecord;
  int i = 0;
  for (pRecord = pFirstRecord();
       pRecord;
       pRecord = pNextRecord(pRecord)) {
    
    u4 iSize = *((u4*)(pRecord));
    vTrace("R%lld: %lld %s\n"_Stz, i++, iSize, aAdd<char*>(pRecord, C_SIZEOF_RECORDSIZE));
  }
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 RecordBuffer::nUsedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nBytes = C_SIZE_LARGE_OBJECT - m_nFreeBytes;
  
  return nBytes;
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
char* RecordBuffer::pAllotSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  // test  sufficient freespace
  if (m_nFreeBytes < nBytes)
    return nullptr;
  
  char* pRecord = &m_cArea[C_SIZE_LARGE_OBJECT - C_1KB - m_nFreeBytes];
  
  m_nRecords++;
  m_nFreeBytes -= nBytes;
  
  return pRecord;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in this buffer?                        */
/*----------------------------------------------------------------------------*/
i8 RecordBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  return m_nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RecordBuffer::aFooter(void)
{
  _poolAddress_(this);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
