/*============================================================================*/
/* RecordExtractor.cpp -- Record Extractor                                    */
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
: FileChunk(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pFormatters = pFormatters;
  
  vReset();
  
  m_nRemainingBytes = 0;
  
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
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void RecordExtractor::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  FileChunk::vCloseFile();
}

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
  if (! bFound) {
    if (! bReadNextChunk())
      return false;
    aStartRecord = m_aNextRecord;
    aEndRecord   = m_aNextRecord;
    bFound = bFindRecordEnd((char**)&aEndRecord);
  }
  
  if (! bFound)
    return false;
  
  // compute the number of bytes
  m_nRecordBytes = aEndRecord - aStartRecord + 1;
  nRecordBytes = m_nRecordBytes;
  
  // advance to beginning of next record for next time
  m_aNextRecord = aEndRecord + 1;
  m_nRemainingBytes -= m_nRecordBytes;
  
  *aaRecord = aStartRecord;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
bool RecordExtractor::bOpenFile(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  FileChunk::vOpenRead(sPath);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* read next chunk from file                                                  */
/*----------------------------------------------------------------------------*/
bool RecordExtractor::bReadNextChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vCopyDown();
  
  Stz* aStartArea = (Stz*)aDataArea();
  Stz* aIntoAddr  = pAdd<Stz*>(aStartArea,m_nRemainingBytes);
  i8   nAsMuchAs  = C_DATA_CHUNK - m_nRemainingBytes;

  i8 nBytesRead = m_oFile.iReadBytes(aIntoAddr,nAsMuchAs);
  
  if (! nBytesRead)
    return false;
  
  m_nRemainingBytes += nBytesRead;
  
  m_aEndChunk   = &(aStartArea)[m_nRemainingBytes - 1];
  m_aNextRecord = aStartArea;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* read given chunk from file                                                 */
/*----------------------------------------------------------------------------*/
void RecordExtractor::vReadSpecificChunk(i8 iSeek, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_oFile.vSeek(iSeek);
  
  Stz* aStartArea = (Stz*)aDataArea();
  Stz* aIntoAddr  = aStartArea;
  i8   nAsMuchAs  = nBytes;

  i8 nBytesRead = m_oFile.iReadBytes(aIntoAddr,nAsMuchAs);

  RASSERT(nBytesRead == nAsMuchAs);
  
  m_nRemainingBytes = nBytesRead;

  m_aEndChunk   =  &(aStartArea)[m_nRemainingBytes - 1];
  m_aNextRecord = aStartArea;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy any remainder to beginning of workspace                               */
/*----------------------------------------------------------------------------*/
void RecordExtractor::vCopyDown(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (! m_nRemainingBytes)
    return;
  
  Stz* aStartArea = (Stz*)aDataArea();
  
  MemLib::vCopy(aStartArea,m_aNextRecord,m_nRemainingBytes,eClass());
}

/*----------------------------------------------------------------------------*/
/* advance to last character of record ( \n or \r )                           */
/*----------------------------------------------------------------------------*/
bool RecordExtractor::bFindRecordEnd(char** aaRecordEnd)
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
void RecordExtractor::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_aEndChunk    = nullptr;
  m_aNextRecord  = nullptr;
  m_nRecordBytes = 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
