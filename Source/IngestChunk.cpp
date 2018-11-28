/*============================================================================*/
/* IngestChunk.cpp -- Ingest Chunk Buffer                                     */
/*============================================================================*/

#include "Loader.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IngestChunk* Loader::_pIngestChunk(void* aAddr)
{
  IngestChunk* pIngestChunk = reinterpret_cast<IngestChunk*>(aAddr);
  
  RASSERT_TYPE(pIngestChunk,CN_INGESTCHUNK);
  
  return pIngestChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IngestChunk::operator new(size_t)
{
  IngestChunk* pIngestChunk = (IngestChunk*)HeapMem::pAllocate(CN_INGESTCHUNK);
  
  return pPoolCheck<void*>(pIngestChunk);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IngestChunk::operator delete(void* pIngestChunkAddress)
{
  IngestChunk* pIngestChunk = static_cast<IngestChunk*>(pIngestChunkAddress);
  
  HeapMem::vFree(pIngestChunk);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IngestChunk::IngestChunk(void)
: Object(CN_INGESTCHUNK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);

  m_xWsp.vFill(0xEF,CN_INGESTCHUNK);
    
  vReset();
  
  ASSERT(! m_aEndChunk);
  ASSERT(! m_aNextRecord);
 
  _checkInitialised_(CN_INGESTCHUNK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IngestChunk::~IngestChunk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void IngestChunk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  IngestChunk* pIngestChunk = _pIngestChunk(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void IngestChunk::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  m_xFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
Stz* IngestChunk::aNextRecord(i8& iRecordBytes, Formatters* pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  ASSERT(m_aEndChunk);
  ASSERT(m_aNextRecord);
  
  if (m_bHaveRecord) {
    m_aNextRecord = pAdd<Stz*>(m_aNextRecord,- (m_iRecordBytes + 1));
    m_bHaveRecord = false;
  }
  
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
  //while (C_NL != *((char*)m_aNextRecord) &&
  //       C_CR != *((char*)m_aNextRecord))
  //  m_aNextRecord++;
  //*m_aNextRecord = '\0';
  
  vAdvanceRecordEnd((char**)&m_aNextRecord, pFormatters);

  // compute the number of bytes
  m_iRecordBytes = m_aNextRecord - pRecord;
  iRecordBytes = m_iRecordBytes;
  
  // advance to beginning of next record for next time
  m_aNextRecord++;
  
  return pRecord;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool IngestChunk::bOpenFile(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  // open the file
  m_xFile.vPath(sPath);
  bool bOpened = m_xFile.bOpen(C_OPEN_READ);

  if (! bOpened)
    return false;
  
  // set buffer
  //m_xFile.vBuffer(C_1MB);
  
  // set no buffering
  m_xFile.vBuffer(0);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push record back into sequence                                             */
/*----------------------------------------------------------------------------*/
void IngestChunk::vPushBack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  m_bHaveRecord = true;
}

/*----------------------------------------------------------------------------*/
/* read next chunk                                                            */
/*----------------------------------------------------------------------------*/
void IngestChunk::vReadChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  i8 iBytesRead = m_xFile.iReadChunk(m_xWsp.aArea(),C_INGEST_CHUNK);
  
  ((char*)m_xWsp.aStr())[iBytesRead] = '\0';
  
  m_aEndChunk   = &m_xWsp.aStr()[iBytesRead - 1];
  m_aNextRecord = m_xWsp.aStr();
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
void IngestChunk::vReadChunk(i8 iSeek, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  //vTrace("vReadChunk: seek %lld bytes %lld\n"_Stz,iSeek,nBytes);

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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* advance to last character of record ( \n or \r )                           */
/*----------------------------------------------------------------------------*/
void IngestChunk::vAdvanceRecordEnd(char** aaSrc, Formatters* pFormatters)
{
  // test for explicit record ends
  
  switch (pFormatters->m_eRecordEnds) {
      
    case RE_NL: // advance to next \n
      for (;;) {
        // find next \n
        while (C_NL != **aaSrc)
          (*aaSrc)++;
        // if there is no record terminator we have found ending \n
        if (! pFormatters->m_cRecordTerminator)
          break;
        // we found the next \n if prior character is the record terminator
        if (pFormatters->m_cRecordTerminator == *(*aaSrc-1))
          break;
        (*aaSrc)++;
      }
      ASSERT(C_NL == **aaSrc);
      break;
      
    case RE_CR: // advance to next \r
      for (;;) {
        // find next \r
        while (C_CR != **aaSrc)
          (*aaSrc)++;
        // if there is no record terminator we have found ending \r
        if (! pFormatters->m_cRecordTerminator)
          break;
        // we found the next \r if prior character is the record terminator
        if (pFormatters->m_cRecordTerminator == *(*aaSrc-1))
          break;
        (*aaSrc)++;
      }
      ASSERT(C_CR == **aaSrc);
      break;
      
    case RE_CRNL: // advance to next \r\n
      for (;;) {
        // find next \r\n
        while (C_CR != **aaSrc && C_NL != *(*aaSrc+1))
          (*aaSrc)++;
        // if there is no record terminator we have found ending \r\n
        if (! pFormatters->m_cRecordTerminator)
          break;
        // we found the next \r\n if prior character is the record terminator
        if (pFormatters->m_cRecordTerminator == *(*aaSrc-1))
          break;
        (*aaSrc)++;
      }
      // skip over \r to be on the \n
      ASSERT(C_CR == **aaSrc);
      (*aaSrc)++;
      ASSERT(C_NL == **aaSrc);
      break;
      
    default:
      NEVER_REACH;
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* reset to initial position (no data)                                        */
/*----------------------------------------------------------------------------*/
void IngestChunk::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  m_aEndChunk    = nullptr;
  m_aNextRecord  = nullptr;
  m_iRecordBytes = 0;
  m_bHaveRecord  = false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IngestChunk::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
