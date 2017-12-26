/*============================================================================*/
/* LoadChunk.cpp -- File Load Buffer                                          */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadChunk::operator new(size_t iSize)
{
  RASSERT(iSize);
  LoadChunk* pLoadChunk = (LoadChunk*)Heap::pAllocate(CN_LOADCHUNK);
  
  return pPoolCheck<void*>(pLoadChunk);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LoadChunk::operator delete(void* pLoadChunkAddress)
{
  LoadChunk* pLoadChunk = static_cast<LoadChunk*>(pLoadChunkAddress);
  
  Heap::vFree(pLoadChunk);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadChunk::LoadChunk(void)
: Object(CN_LOADCHUNK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);

  m_xWsp.vFill(0xEF,CN_LOADCHUNK);
  
  vReset();
  
  ASSERT(! m_aEndChunk);
  ASSERT(! m_aNextRecord);
 
  _checkInitialised_(CN_LOADCHUNK);
  
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
void LoadChunk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_LOADCHUNK);
  ASSERT(pObject->eClass() == CN_LOADCHUNK);
  
  LoadChunk* pLoadChunk = reinterpret_cast<LoadChunk*>(pObject);
  
  vTrace("\n"_Stz);
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
Stz* LoadChunk::aNextRecord(i8& iRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  ASSERT(m_aEndChunk);
  ASSERT(m_aNextRecord);
  
  //
  // At this point, m_aNextRecord is the address of the next record
  // in the chunk.
  //
  
  // have we exhausted the chunk?
  if (m_aNextRecord > m_aEndChunk) {
    iRecordBytes = 0;
    vReset();
    return nullptr;
  }
  
  // get start address of next record
  Stz* pRecord = m_aNextRecord;
  
  // find end of next record
  while ('\n' != *((char*)m_aNextRecord) &&
         '\r' != *((char*)m_aNextRecord))
    m_aNextRecord++;
  //*m_aNextRecord = '\0';
  
  // compute the number of bytes
  iRecordBytes = m_aNextRecord - pRecord;
  
  // advance to beginning of next record for next time
  m_aNextRecord++;
  
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
  
  // set buffer
  //m_xFile.vBuffer(C_1MB);
  
  // set no buffering
  m_xFile.vBuffer(0);
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
void LoadChunk::vReadChunk(i8 iSeek, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  m_xFile.vSeek(iSeek);
  
  //MemLib::vHexDump(m_xWsp.aStr(),nBytes+16);
  i8 iBytesRead = m_xFile.iReadChunk(m_xWsp.aArea(),nBytes);

  //MemLib::vHexDump(m_xWsp.aStr(), iBytesRead+16);
  ((char*)m_xWsp.aStr())[iBytesRead] = '\0';                       //! show Alka
  //MemLib::vHexDump(m_xWsp.aStr(), iBytesRead+16);
  
  m_aEndChunk   = &m_xWsp.aStr()[iBytesRead - 1];
  m_aNextRecord = m_xWsp.aStr();
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
  
  m_aEndChunk   = nullptr;
  m_aNextRecord = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* LoadChunk::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
