/*============================================================================*/
/* RecordCursor.cpp -- Record Cursor                                          */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RecordCursor::RecordCursor(eCN eClassNo, Format* pFmt)
: DataChunk(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pFormatters  = pFmt;
  m_aNextRecord  = nullptr;

   _postcondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RecordCursor::~RecordCursor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
bool RecordCursor::bNextRecord(Stz** aszRecord, i8& nRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // get start address of next record
  Stz* szStartRecord = m_aNextRecord;
  
  // find end address of next record
  Stz* szEndRecord   = m_aNextRecord;
  bool bFound = bFindRecordEnd((char**)&szEndRecord);
  
  if (! bFound)
    return false;
  
  // compute the number of bytes
  nRecordBytes = szEndRecord - szStartRecord + 1;
  
  // advance to beginning of next record for next time
  m_aNextRecord = szEndRecord + 1;
  m_nUnprocessedBytes -= nRecordBytes;
  
  *aszRecord = szStartRecord;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find the last character of next record within chunk (a '\r' or '\n')       */
/*----------------------------------------------------------------------------*/
bool RecordCursor::bFindRecordEnd(char** aazRecordEnd)
{
  // test for explicit record ends
  
  char* aRecordStart;
  
  // which record end are we looking for?
  switch (m_pFormatters->m_eRecordEnd) {
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_NL: // advance to next [<terminator>]\n
      
      // debug
      aRecordStart = *aazRecordEnd;
      
      for (;;) {
        
        // find next \n within chunk
        while (*aazRecordEnd <= (char*)m_aLastDataByte
               && C_NL != **aazRecordEnd)
          (*aazRecordEnd)++;
        
        // have we gone past end of chunk?
        if (*aazRecordEnd > (char*)m_aLastDataByte)
          return false;
        
        // if no record terminator we have found record end (ie. \n)
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        
        // we have found ending \n if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aazRecordEnd-1))
          break;
        
        // did not see [terminator>\n... move on
        (*aazRecordEnd)++;
      }
      
      ASSERT(C_NL == **aazRecordEnd);
      return true;
      
      break;
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_CR: // advance to next [<terminator>]\r
      
      // debug
      aRecordStart = *aazRecordEnd;
      
      for (;;) {
        
        // find next \r within chunk
        while (*aazRecordEnd <= (char*)m_aLastDataByte
               && C_CR != **aazRecordEnd)
          (*aazRecordEnd)++;
        
        // have we gone past end of chunk?
        if (*aazRecordEnd > (char*)m_aLastDataByte)
          return false;
        
        // if no record terminator we have found record end (ie. \r)
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        
        // we have found ending \r if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aazRecordEnd-1))
          break;
        
        // did not see [terminator>\r... move on
        (*aazRecordEnd)++;
      }
      
      ASSERT(C_CR == **aazRecordEnd);
      return true;
      
      break;
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case RE_CRNL: // advance to next [terminator>]\r\n
      
      // debug
      aRecordStart = *aazRecordEnd;
      
      for (;;) {
        
        // find next \r\n within chunk
        while (*aazRecordEnd + 1 <= (char*)m_aLastDataByte
               && C_CR != **aazRecordEnd && C_NL != *(*aazRecordEnd+1))
          (*aazRecordEnd)++;
        
        // have we gone past end of chunk?
        if (*aazRecordEnd + 1 > (char*)m_aLastDataByte)
          return false;
        
        // if no record terminator we have found record end (ie. \r\n)
        if (! m_pFormatters->m_cRecordTerminator)
          break;
        
        // we have found ending \r\n if prior character is the record terminator
        if (m_pFormatters->m_cRecordTerminator == *(*aazRecordEnd-1))
          break;
        
        // did not see [terminator>\r\n... move on
        (*aazRecordEnd)++;
      }
      
      // skip over \r to be on the \n
      ASSERT(C_CR == **aazRecordEnd);
      (*aazRecordEnd)++;
      ASSERT(C_NL == **aazRecordEnd);
      
      return true;
      
      break;
  }
  
  NEVER_REACH;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
