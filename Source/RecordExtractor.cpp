/*============================================================================*/
/* RecordExtractor.cpp -- Record Extractor                                */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RecordExtractor* Chunks::_pRecordExtractor(void* aAddr)
{
  RecordExtractor* pRecordExtractor = reinterpret_cast<RecordExtractor*>(aAddr);
  
  return pRecordExtractor;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RecordExtractor::RecordExtractor(eCN eClassNo, Formatters* pFormatters)
: DataChunk(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pFormatters  = pFormatters;
  m_aNextRecord  = nullptr;

   _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RecordExtractor::~RecordExtractor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void RecordExtractor::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
bool RecordExtractor::bGetRecord(Stz** aaRecord, i8& nRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // get start address of next record
  Stz* aStartRecord = m_aNextRecord;
  Stz* aEndRecord   = m_aNextRecord;
  
  bool bFound = bFindRecordEnd((char**)&aEndRecord);
  
  if (! bFound)
    return false;
  
  // compute the number of bytes
  nRecordBytes = aEndRecord - aStartRecord + 1;
  
  // advance to beginning of next record for next time
  m_aNextRecord = aEndRecord + 1;
  m_nUnprocessedBytes -= nRecordBytes;
  
  *aaRecord = aStartRecord;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find the last character of next record within chunk (a '\r' or '\n')       */
/*----------------------------------------------------------------------------*/
bool RecordExtractor::bFindRecordEnd(char** aaRecordEnd)
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
