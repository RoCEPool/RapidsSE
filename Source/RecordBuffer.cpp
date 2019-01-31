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
  PoolBuffer* pBuffer = (PoolBuffer*)HeapMem::pAllocate(CN_RECORDBUFFER);
  
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
  PoolBuffer* pBuffer = static_cast<PoolBuffer*>(pBufferAddress);
  
  HeapMem::vFree_NonObject(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RecordBuffer::RecordBuffer(void)
: Object(CN_RECORDBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);

  vInit();
  
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
bool RecordBuffer::bAppendRecord(char* aSrcRecord, i8 nRecordSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  i4 iRecordSize = (i4)nRecordSize;
  i4 nTotalRecordSize = iRecordSize + C_SIZEOF_RECORDSIZE;
  
  // do we have enough space for this row ?
  if ( m_nFreeBytes < nTotalRecordSize )
    return false;
  
  // allocate space
  char* aDstRecord = aAllotSpace(nTotalRecordSize);
  
  // copy the size of the row
  MemLib::vCopy(aDstRecord,&iRecordSize,C_SIZEOF_RECORDSIZE,CN_RECORDBUFFER);
  
  // copy the compressed row to working row in uncompressed form
  MemLib::vCopy(aDstRecord+C_SIZEOF_RECORDSIZE, aSrcRecord,
                nRecordSize, CN_RECORDBUFFER);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* first item                                                                 */
/*----------------------------------------------------------------------------*/
char* RecordBuffer::aFirstRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  if (m_nUsedBytes == 0)
    return nullptr;
  
  char* aFirst = &m_caArea[0];
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void RecordBuffer::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  m_nRecords   = 0;
  m_nUsedBytes = 0;
  m_nFreeBytes = C_SIZE_LARGE_OBJECT - C_1KB;
  //m_caArea
  
  //_checkInitialised_(CN_RECORDBUFFER);
}

/*----------------------------------------------------------------------------*/
/* get next item                                                              */
/*----------------------------------------------------------------------------*/
char* RecordBuffer::aNextRecord(char* aCurrentRecord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  i4 nBytes = *((i4*)aCurrentRecord);
  
  char* aNext = aAdd<char*>(aCurrentRecord,nBytes+C_SIZEOF_RECORDSIZE);
  
  RASSERT(C_SIZE_LARGE_OBJECT - C_1KB - m_nFreeBytes == m_nUsedBytes);
  
  if ((char*)aNext >= (char*)&m_caArea[m_nUsedBytes])
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
  
  char* aRecord;
  int i = 0;
  for (aRecord = aFirstRecord();
       aRecord;
       aRecord = aNextRecord(aRecord)) {
    
    u4 uSize = *((u4*)(aRecord));
    vTrace("R%lld: (%p)%lld (%p) %s\n"_Stz, i++, aRecord, uSize, aRecord + 4, aAdd<char*>(aRecord, C_SIZEOF_RECORDSIZE));
  }
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 RecordBuffer::nUsedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(C_SIZE_LARGE_OBJECT -  - C_1KB - m_nFreeBytes == m_nUsedBytes);
  
  return m_nUsedBytes;
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an item of nBytes in this buffer                                     */
/*----------------------------------------------------------------------------*/
char* RecordBuffer::aAllotSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDBUFFER,_FOOTER_);
  
  // test  sufficient freespace
  if (m_nFreeBytes < nBytes)
    return nullptr;
  
  RASSERT(C_SIZE_LARGE_OBJECT - C_1KB - m_nFreeBytes == m_nUsedBytes);
  
  char* aRecord = &m_caArea[m_nUsedBytes];
  
  m_nRecords++;
  m_nFreeBytes -= nBytes;
  m_nUsedBytes += nBytes;
  
  return aRecord;
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
