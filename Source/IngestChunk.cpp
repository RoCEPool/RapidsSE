/*============================================================================*/
/* IngestChunk.cpp -- Ingest Chunk                                            */
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
  
  return pIngestChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IngestChunk::operator new(size_t)
{
  IngestChunk* pIngestChunk = (IngestChunk*)HeapMem::pAllocate(CN_IOCHUNK);
  
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
: DataChunk(CN_INGESTCHUNK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);

  m_pFormatters  = pFormatters;
  m_iChunkNo     = 0;
  m_aNextRecord  = nullptr;
  m_nRecordBytes = 0;
  
  //m_oFile
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
bool IngestChunk::bGetRecord(Stz** aaRecord, i8& nRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);

  // get start address of next record
  Stz* aStartRecord = m_aNextRecord;
  Stz* aEndRecord   = m_aNextRecord;
  
  bool bFound = bFindRecordEnd((char**)&aEndRecord);
  
  if (! bFound)
    return false;
  
  // compute the number of bytes
  m_nRecordBytes = aEndRecord - aStartRecord + 1;
  nRecordBytes = m_nRecordBytes;
  
  // advance to beginning of next record for next time
  m_aNextRecord = aEndRecord + 1;
  m_nUnprocessedBytes -= m_nRecordBytes;
  
  *aaRecord = aStartRecord;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get wsp as string pointer                                                  */
/*----------------------------------------------------------------------------*/
Stz* IngestChunk::aStz(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);

  return m_xWsp.aStz();
}

/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find the last character of next record within chunk (a '\r' or '\n')       */
/*----------------------------------------------------------------------------*/
bool IngestChunk::bFindRecordEnd(char** aaRecordEnd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INGESTCHUNK,_FOOTER_);
  
  // test for explicit record ends
  
  char* aRecordStart;
  
  // which record end are we looking for?
  switch (m_pFormatters->m_eRecordEnd) {
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_NL: // advance to next [<terminator>]\n
      
      // debug
      aRecordStart = *aaRecordEnd;
      
      for (;;) {
        
        // find next \n within chunk
        while (*aaRecordEnd <= (char*)m_aLastDataByte
               && C_NL != **aaRecordEnd)
          (*aaRecordEnd)++;
        
        // have we gone past end of chunk?
        if (*aaRecordEnd > (char*)m_aLastDataByte)
          return false;
        
        // if no record terminator we have found record end (ie. \n)
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        
        // we have found ending \n if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaRecordEnd-1))
          break;
        
        // did not see [terminator>\n... move on
        (*aaRecordEnd)++;
      }
      
      ASSERT(C_NL == **aaRecordEnd);
      return true;
      
      break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_CR: // advance to next [<terminator>]\r
      
      // debug
      aRecordStart = *aaRecordEnd;
      
      for (;;) {
        
        // find next \r within chunk
        while (*aaRecordEnd <= (char*)m_aLastDataByte
               && C_CR != **aaRecordEnd)
          (*aaRecordEnd)++;
        
        // have we gone past end of chunk?
        if (*aaRecordEnd > (char*)m_aLastDataByte)
          return false;
        
        // if no record terminator we have found record end (ie. \r)
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        
        // we have found ending \r if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaRecordEnd-1))
          break;
        
        // did not see [terminator>\r... move on
        (*aaRecordEnd)++;
      }
      
      ASSERT(C_CR == **aaRecordEnd);
      return true;
      
      break;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_CRNL: // advance to next [terminator>]\r\n
      
      // debug
      aRecordStart = *aaRecordEnd;
      
      for (;;) {
        
        // find next \r\n within chunk
        while (*aaRecordEnd + 1 <= (char*)m_aLastDataByte
               && C_CR != **aaRecordEnd && C_NL != *(*aaRecordEnd+1))
          (*aaRecordEnd)++;
        
        // have we gone past end of chunk?
        if (*aaRecordEnd + 1 > (char*)m_aLastDataByte)
          return false;
        
        // if no record terminator we have found record end (ie. \r\n)
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        
        // we have found ending \r\n if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aaRecordEnd-1))
          break;
        
        // did not see [terminator>\r\n... move on
        (*aaRecordEnd)++;
      }
      
      // skip over \r to be on the \n
      ASSERT(C_CR == **aaRecordEnd);
      (*aaRecordEnd)++;
      ASSERT(C_NL == **aaRecordEnd);
      
      return true;
      
      break;
  }
  
  NEVER_REACH;
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
