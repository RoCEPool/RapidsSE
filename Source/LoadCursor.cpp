/*============================================================================*/
/* LoadCursor.cpp -- Load Cursor                                              */
/*============================================================================*/

#include "UpperLoad.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadCursor::operator new(size_t)
{
  LoadCursor* pRecordData = (LoadCursor*)HeapMem::pAllocate(CN_LOADCURSOR);
  
  return pPoolCheck<void*>(pRecordData);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LoadCursor::operator delete(void* pRecordDataAddress)
{
  LoadCursor* pRecordData = static_cast<LoadCursor*>(pRecordDataAddress);
  
  HeapMem::vFree(pRecordData);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LoadCursor::LoadCursor(Format* pFmt)
: Object(CN_LOADCURSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCURSOR);

  m_pFmt  = pFmt;
  m_szNextRecord  = nullptr;
  m_aLastDataByte   = nullptr;
  m_nUnprocessedBytes = 0;
  
  m_nDataBytes = C_UNKNOWN;
  m_aDataArea  = nullptr;
  
   _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADCURSOR);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LoadCursor::~LoadCursor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCURSOR);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
void LoadCursor::vInit(Stz* aDataArea, i8 nDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCURSOR);
  
  m_nDataBytes = nDataBytes;
  m_aDataArea  = aDataArea;
  m_aLastDataByte = aDataArea + nDataBytes - 1;
  m_nUnprocessedBytes = m_nDataBytes;
  m_szNextRecord = aDataArea;
}

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
bool LoadCursor::bNextRecord(Stz** aszRecord, i8& nRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCURSOR);
  
  // get start address of next record
  Stz* szStartRecord = m_szNextRecord;
  
  // find end address of next record
  Stz* szEndRecord   = m_szNextRecord;
  bool bFound = bFindRecordEnd((char**)&szEndRecord);
  
  if (! bFound)
    return false;
  
  // compute the number of bytes
  nRecordBytes = szEndRecord - szStartRecord + 1;
  
  // advance to beginning of next record for next time
  m_szNextRecord = szEndRecord + 1;
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
bool LoadCursor::bFindRecordEnd(char** aazRecordEnd)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCURSOR);
  
  // test for explicit record ends
  
  char* aRecordStart;
  
  // which record end are we looking for?
  switch (m_pFmt->m_eRecordEnd) {
      
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
        if (! m_pFmt->m_cRecordTerminator)
          break;
        
        // we have found ending \n if prior character is the record terminator
        if (m_pFmt->m_cRecordTerminator == *(*aazRecordEnd-1))
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
        if (! m_pFmt->m_cRecordTerminator)
          break;
        
        // we have found ending \r if prior character is the record terminator
        if (m_pFmt->m_cRecordTerminator == *(*aazRecordEnd-1))
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
        if (! m_pFmt->m_cRecordTerminator)
          break;
        
        // we have found ending \r\n if prior character is the record terminator
        if (m_pFmt->m_cRecordTerminator == *(*aazRecordEnd-1))
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
