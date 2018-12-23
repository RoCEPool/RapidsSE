/*============================================================================*/
/* LoadChunk.cpp -- File Load Buffer                                          */
/*============================================================================*/

#include "Loader.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LoadChunk* Loader::_pLoadChunk(void* aAddr)
{
  LoadChunk* pLoadChunk = reinterpret_cast<LoadChunk*>(aAddr);
  
  RASSERT_TYPE(pLoadChunk,CN_LOADCHUNK);
  
  return pLoadChunk;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LoadChunk::operator new(size_t)
{
  LoadChunk* pLoadChunk = (LoadChunk*)HeapMem::pAllocate(CN_LOADCHUNK);
  
  return pPoolCheck<void*>(pLoadChunk);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LoadChunk::operator delete(void* pLoadChunkAddress)
{
  LoadChunk* pLoadChunk = static_cast<LoadChunk*>(pLoadChunkAddress);
  
  HeapMem::vFree(pLoadChunk);
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
LoadChunk::~LoadChunk(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void LoadChunk::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  LoadChunk* pLoadChunk = _pLoadChunk(pObject);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
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
Stz* LoadChunk::aNextRecord(i8& iRecordBytes, Formatters* pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
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
void LoadChunk::vOpenFile(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  // open the file
  m_xFile.vPath(sPath);
  bool b = m_xFile.bOpen(C_OPEN_READ);
  RASSERT(b);
  
  // set buffer
  //m_xFile.vBuffer(C_1MB);
  
  // set no buffering (WHY??)
  m_xFile.vSetBuffering(C_NO_BUFFERING);
}

/*----------------------------------------------------------------------------*/
/* push record back into sequence                                             */
/*----------------------------------------------------------------------------*/
void LoadChunk::vPushBack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  m_bHaveRecord = true;
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
void LoadChunk::vReadChunk(i8 iSeek, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
  //vTrace("vReadChunk: seek %lld bytes %lld\n"_Stz,iSeek,nBytes);

  m_xFile.vSeek(iSeek);
  
  //MemLib::vHexDump(m_xWsp.aStr(),nBytes+16);
  i8 iBytesRead = m_xFile.iReadBytes(m_xWsp.aArea(),nBytes);

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
void LoadChunk::vAdvanceRecordEnd(char** aaSrc, Formatters* pFormatters)
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
void LoadChunk::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK,_FOOTER_);
  
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
Footer* LoadChunk::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADCHUNK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
