/*============================================================================*/
/* LoadBuffer.cpp -- File Load Buffer                                         */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LoadBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadBuffer::LoadBuffer(void)
: Buffer(CN_SCANBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);

  Memory::vSet(m_wWorkarea,0xEF,C_LOADBUFFER_SIZE);
  
  vReset();
  
  ASSERT(! m_pEndChunk);
  ASSERT(! m_pNextRecord);
 
  _checkInitialised_(sizeof(LoadBuffer));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoadBuffer::~LoadBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the number of bytes in workarea of this load buffer                    */
/*----------------------------------------------------------------------------*/
i8 LoadBuffer::iBytesWorkarea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(! m_pEndChunk);
  ASSERT(! m_pNextRecord);
  
  return C_LOADBUFFER_SIZE;
}

/*----------------------------------------------------------------------------*/
/* get the number of bytes for set of complete records just read in           */
/*----------------------------------------------------------------------------*/
i8 LoadBuffer::iCompleteBytes(i8 iBytesRead)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(! m_pEndChunk);
  ASSERT(! m_pNextRecord);
  
  //
  // At this point, the caller has just read upto C_WORKAREA bytes
  // of data into the workarea.  It may contain a dangling record.
  // We scan backards to find the end of the last complete record.
  // The caller will use the number of complete bytes to adjust/seek
  // the next read.
  //
  
  // start scanning backward from last byte read
  char* pScanBackward = &m_wWorkarea[iBytesRead - 1];
  
  // find the last end-of-line
  while (pScanBackward >= m_wWorkarea)
    if ('\n' == *pScanBackward)
      break;
    else
      pScanBackward--;
  
  // if we didnt find an end-of-line there are no more records
  if (pScanBackward < m_wWorkarea)
    return 0;
  
  // double check we found the end-of-line
  ASSERT('\n' == *pScanBackward);
  
  // compute the bytes in the chunk of complete records
  i8 iCompleteBytes = pScanBackward - m_wWorkarea + 1;
  
  // set up for scanning of first record
  m_pEndChunk   = &m_wWorkarea[iCompleteBytes - 1];
  m_pNextRecord = m_wWorkarea;
  
  return iCompleteBytes;
}

/*----------------------------------------------------------------------------*/
/* get offset of next record                                                  */
/*----------------------------------------------------------------------------*/
i8 LoadBuffer::iNextRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(m_pEndChunk);
  ASSERT(m_pNextRecord);
  
  i8 iNextRecord = m_pNextRecord - m_wWorkarea;
  
  return iNextRecord;
}

/*----------------------------------------------------------------------------*/
/* get offset of end chunk                                                    */
/*----------------------------------------------------------------------------*/
i8 LoadBuffer::iEndChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(m_pEndChunk);
  ASSERT(m_pNextRecord);
  
  i8 iEndChunk = m_pEndChunk - m_wWorkarea;
  
  return iEndChunk;
}

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
char* LoadBuffer::pNextRecord(i8& iRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(m_pEndChunk);
  ASSERT(m_pNextRecord);
  
  //
  // At this point, m_pNextRecord is the address of the next record
  // in the chunk.
  //
  
  // have we exhausted the chunk?
  if (m_pNextRecord > m_pEndChunk) {
    iRecordBytes = 0;
    vReset();
    return nullptr;
  }
  
  // get start address of next record
  char* pRecord = m_pNextRecord;
  
  // find end of next record
  while ('\n' != *m_pNextRecord)
    m_pNextRecord++;
  
  // compute the number of bytes
  iRecordBytes = m_pNextRecord - pRecord;
  
  // advance to beginning of next record for next time
  m_pNextRecord++;
  
  return pRecord;
}

/*----------------------------------------------------------------------------*/
/* get address of the workarea                                                */
/*----------------------------------------------------------------------------*/
char* LoadBuffer::pWorkarea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(! m_pEndChunk);
  ASSERT(! m_pNextRecord);
  
  return m_wWorkarea;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reset to initial position (no data)                                        */
/*----------------------------------------------------------------------------*/
void LoadBuffer::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  m_pEndChunk   = nullptr;
  m_pNextRecord = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/