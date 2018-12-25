/*============================================================================*/
/* IngestChunk.cpp -- Ingest Chunk Buffer                                     */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IngestChunk* Chunks::_pIngestChunk(void* aAddr)
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
IngestChunk::IngestChunk(Formatters* pFormatters)
: Object(CN_INGESTCHUNK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);

  m_pFormatters = pFormatters;
  
  m_xWsp.vFill(0xEF,CN_INGESTCHUNK);
  
  m_aStartChunk = (Stz*)m_xWsp.aArea();
    
  vReset();
  
  m_nBytesRemaining = 0;
  
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
  
  m_oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* get next record                                                            */
/*----------------------------------------------------------------------------*/
bool IngestChunk::bGetRecord(Stz** aaRecord, i8& nRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  if (! m_aNextRecord) {
    if (! bReadChunk()) {
      *aaRecord = nullptr;
      return false;
    }
  }
  
  // get start address of next record
  Stz* aStartRecord = m_aNextRecord;
  Stz* aEndRecord   = m_aNextRecord;
  
  bool bFound = bFindRecordEnd((char**)&aEndRecord);
  if (! bFound) {
    if (! bReadChunk())
      return nullptr;
    aStartRecord = m_aNextRecord;
    aEndRecord   = m_aNextRecord;
    bFound = bFindRecordEnd((char**)&aEndRecord);
  }
  
  if (! bFound)
    return nullptr;
  
  // compute the number of bytes
  m_nRecordBytes = aEndRecord - aStartRecord + 1;
  nRecordBytes = m_nRecordBytes;
  
  // advance to beginning of next record for next time
  m_aNextRecord = aEndRecord + 1;
  m_nBytesRemaining -= m_nRecordBytes;
  
  *aaRecord = aStartRecord;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
Stz* IngestChunk::aNextRecord(i8& nRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  ASSERT(m_aEndChunk);
  ASSERT(m_aNextRecord);
  
  if (m_bHaveRecord) {
    m_aNextRecord = pAdd<Stz*>(m_aNextRecord,- (m_nRecordBytes + 1));
    m_bHaveRecord = false;
  }
  
  //
  // At this point, m_aNextRecord is the address of the next record
  // in the chunk.
  //
  
  // have we exhausted the chunk?
  if (m_aNextRecord > m_aEndChunk) {
    nRecordBytes = 0;
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
  
  vAdvanceRecordEnd((char**)&m_aNextRecord);

  // compute the number of bytes
  m_nRecordBytes = m_aNextRecord - pRecord;
  nRecordBytes = m_nRecordBytes;
  
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
  bool bOpened = m_oFile.bOpen(sPath,C_OPEN_READ);

  if (! bOpened)
    return false;
  
  // set buffer
  //m_oFile.vBuffer(C_1MB);
  
  // set no buffering
  m_oFile.vSetBuffering(C_NO_BUFFERING);
  
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
/* read next chunk from file                                                  */
/*----------------------------------------------------------------------------*/
bool IngestChunk::bReadChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  vCopyDown();
  
  i8   nAsMuchAs = C_INGEST_CHUNK - m_nBytesRemaining;
  Stz* aIntoAddr = pAdd<Stz*>(m_aStartChunk,m_nBytesRemaining);
  
  i8 nBytesRead = m_oFile.iReadBytes(aIntoAddr,nAsMuchAs);
  
  if (! nBytesRead)
    return false;
  
  m_nBytesRemaining += nBytesRead;

  //((char*)m_aStartChunk)[m_nBytesRemaining] = '\0';
  
  m_aEndChunk   = &(m_aStartChunk)[m_nBytesRemaining - 1];
  m_aNextRecord = m_aStartChunk;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* read given chunk from file                                                 */
/*----------------------------------------------------------------------------*/
void IngestChunk::vReadChunk(i8 iSeek, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  //vTrace("vReadChunk: seek %lld bytes %lld\n"_Stz,iSeek,nBytes);

  m_oFile.vSeek(iSeek);
  
  //MemLib::vHexDump(m_xWsp.aStr(),nBytes+16);
  i8 iBytesRead = m_oFile.iReadBytes(m_aStartChunk,nBytes);

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
void IngestChunk::vAdvanceRecordEnd(char** aaSrc)
{
  // test for explicit record ends
  
  switch (m_pFormatters->m_eRecordEnds) {
      
    case RE_NL: // advance to next \n
      for (;;) {
        // find next \n
        while (C_NL != **aaSrc)
          (*aaSrc)++;
        // if there is no record terminator we have found ending \n
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        // we found the next \n if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaSrc-1))
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
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        // we found the next \r if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaSrc-1))
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
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        // we found the next \r\n if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaSrc-1))
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
/* copy any remainder to beginning of workspace                               */
/*----------------------------------------------------------------------------*/
void IngestChunk::vCopyDown(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  if (! m_nBytesRemaining)
    return;
  
  MemLib::vCopy(m_aStartChunk,m_aNextRecord,m_nBytesRemaining,CN_INGESTCHUNK);
}

/*----------------------------------------------------------------------------*/
/* advance to last character of record ( \n or \r )                           */
/*----------------------------------------------------------------------------*/
bool IngestChunk::bFindRecordEnd(char** aaRecordEnd)
{
  // test for explicit record ends
  
  switch (m_pFormatters->m_eRecordEnds) {
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_NL: // advance to next \n
      
      for (;;) {
        
        // find next \n
        while (*aaRecordEnd <= (char*)m_aEndChunk && C_NL != **aaRecordEnd)
          (*aaRecordEnd)++;
        
        if (*aaRecordEnd > (char*)m_aEndChunk)
          return false;
        
        // if there is no record terminator we have found ending \n
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        
        // we found the next \n if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaRecordEnd-1))
          break;
        
        (*aaRecordEnd)++;
      }
      
      ASSERT(C_NL == **aaRecordEnd);
      return true;
      
      break;
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_CR: // advance to next \r
      for (;;) {
        // find next \r
        while (C_CR != **aaRecordEnd)
          (*aaRecordEnd)++;
        // if there is no record terminator we have found ending \r
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        // we found the next \r if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaRecordEnd-1))
          break;
        (*aaRecordEnd)++;
      }
      ASSERT(C_CR == **aaRecordEnd);
      return true;
      break;
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_CRNL: // advance to next \r\n
      for (;;) {
        // find next \r\n
        while (C_CR != **aaRecordEnd && C_NL != *(*aaRecordEnd+1))
          (*aaRecordEnd)++;
        // if there is no record terminator we have found ending \r\n
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        // we found the next \r\n if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaRecordEnd-1))
          break;
        (*aaRecordEnd)++;
      }
      // skip over \r to be on the \n
      ASSERT(C_CR == **aaRecordEnd);
      (*aaRecordEnd)++;
      ASSERT(C_NL == **aaRecordEnd);
      return true;
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
  m_nRecordBytes = 0;
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
