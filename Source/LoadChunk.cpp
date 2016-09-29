/*============================================================================*/
/* LoadChunk.cpp -- File Load Buffer                                          */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadChunk::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LoadChunk::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadChunk::LoadChunk(void)
: Buffer(CN_LOADCHUNK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);

  Memory::vSet(m_wWorkarea,0xEF,C_LOADCHUNK_SIZE);
  
  vReset();
  
  ASSERT(! m_pEndChunk);
  ASSERT(! m_pNextRecord);
 
  _checkInitialised_(sizeof(LoadChunk));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoadChunk::~LoadChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump LoaderSub                                                             */
/*----------------------------------------------------------------------------*/
void LoadChunk::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_LOADCHUNK);
  ASSERT(pObject->eClass() == CN_LOADCHUNK);
  
  LoadChunk* pLoadChunk = reinterpret_cast<LoadChunk*>(pObject);
  
  vTrace("%LoadChunk %p:\n", pTab, pLoadChunk);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void LoadChunk::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
char* LoadChunk::pNextRecord(i8& iRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
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
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void LoadChunk::vOpenFile(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  // open the file
  m_xFile.vPath(sPath);
  bool b = m_xFile.bOpen(C_OPEN_READ);
  RASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
void LoadChunk::vReadChunk(i8 iSeek, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  m_xFile.vSeek(iSeek);
  
  i8 iBytesRead = m_xFile.iReadChunk(m_wWorkarea,nBytes);

  m_pEndChunk   = &m_wWorkarea[iBytesRead - 1];
  m_pNextRecord = m_wWorkarea;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* reset to initial position (no data)                                        */
/*----------------------------------------------------------------------------*/
void LoadChunk::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  m_pEndChunk   = nullptr;
  m_pNextRecord = nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/