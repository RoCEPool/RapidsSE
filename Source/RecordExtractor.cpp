/*============================================================================*/
/* RecordExtractor_NEW.cpp -- Record Extractor                                */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RecordExtractor_NEW* Chunks::_pRecordExtractor_NEW(void* aAddr)
{
  RecordExtractor_NEW* pRecordExtractor_NEW = reinterpret_cast<RecordExtractor_NEW*>(aAddr);
  
  return pRecordExtractor_NEW;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RecordExtractor_NEW::RecordExtractor_NEW(eCN eClassNo, Formatters* pFormatters)
: DataChunk(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pFormatters  = pFormatters;
  m_aNextRecord  = nullptr;
  m_nRecordBytes = 0;

   _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RecordExtractor_NEW::~RecordExtractor_NEW(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void RecordExtractor_NEW::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
bool RecordExtractor_NEW::bGetRecord(Stz** aaRecord, i8& nRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find the last character of next record within chunk (a '\r' or '\n')       */
/*----------------------------------------------------------------------------*/
bool RecordExtractor_NEW::bFindRecordEnd(char** aaRecordEnd)
{
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
