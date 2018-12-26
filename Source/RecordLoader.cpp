/*============================================================================*/
/* RecordLoader.cpp -- Record Loader                                          */
/*============================================================================*/

#include "Chunks.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RecordLoader* Chunks::_pRecordLoader(void* aAddr)
{
  RecordLoader* pRecordLoader = reinterpret_cast<RecordLoader*>(aAddr);
  
  RASSERT_TYPE(pRecordLoader,CN_RECORDLOADER);
  
  return pRecordLoader;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RecordLoader::operator new(size_t)
{
  RecordLoader* pRecordLoader = (RecordLoader*)HeapMem::pAllocate(CN_RECORDLOADER);
  
  return pPoolCheck<void*>(pRecordLoader);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RecordLoader::operator delete(void* pRecordLoaderAddress)
{
  RecordLoader* pRecordLoader = static_cast<RecordLoader*>(pRecordLoaderAddress);
  
  HeapMem::vFree(pRecordLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RecordLoader::RecordLoader(Formatters* pFormatters)
: Object(CN_RECORDLOADER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);

  m_pFormatters = pFormatters;
  
  m_xWsp.vFill(0xEF,CN_RECORDLOADER);
    
  vReset();
  
  ASSERT(! m_aEndChunk);
  ASSERT(! m_aNextRecord);
 
  _checkInitialised_(CN_RECORDLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RecordLoader::~RecordLoader(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump SubLoader                                                             */
/*----------------------------------------------------------------------------*/
void RecordLoader::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  //RecordLoader* pRecordLoader = _pRecordLoader(pObject);
  
  //vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close file                                                                 */
/*----------------------------------------------------------------------------*/
void RecordLoader::vCloseFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
  
  m_oFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* get address of next record and its byte length                             */
/*----------------------------------------------------------------------------*/
bool RecordLoader::bGetRecord(Stz** aaRecord, i8& nRecordBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
  
  ASSERT(m_aEndChunk);
  ASSERT(m_aNextRecord);
  
  //
  // At this point, m_aNextRecord is the address of the next record
  // in the chunk.
  //
  
  // have we exhausted the chunk?
  if (m_aNextRecord > m_aEndChunk) {
    *aaRecord = nullptr;
    nRecordBytes = 0;
    vReset();
    return false;
  }
  
  // get start address of next record
  Stz* aRecord = m_aNextRecord;
    
  bool bFound = bFindRecordEnd((char**)&m_aNextRecord);
  ASSERT(bFound);

  // compute the number of bytes
  m_nRecordBytes = m_aNextRecord - aRecord;
  nRecordBytes = m_nRecordBytes;
  
  // advance to beginning of next record for next time
  m_aNextRecord++;
  
  *aaRecord = aRecord;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open file                                                                  */
/*----------------------------------------------------------------------------*/
void RecordLoader::vOpenFile(s256& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
  
  // open the file
  bool b = m_oFile.bOpen(sPath,C_OPEN_READ);
  RASSERT(b);
  
  // set buffer
  //m_oFile.vBuffer(C_1MB);
  
  // set no buffering (WHY??)
  m_oFile.vSetBuffering(C_NO_BUFFERING);
}

/*----------------------------------------------------------------------------*/
/* read given chunk                                                           */
/*----------------------------------------------------------------------------*/
void RecordLoader::vReadChunk(i8 iSeek, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
  
  //vTrace("vReadChunk: seek %lld bytes %lld\n"_Stz,iSeek,nBytes);

  m_oFile.vSeek(iSeek);
  
  //MemLib::vHexDump(m_xWsp.aStr(),nBytes+16);
  i8 iBytesRead = m_oFile.iReadBytes(m_xWsp.aArea(),nBytes);

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
bool RecordLoader::bFindRecordEnd(char** aaSrc)
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
      return true;
      
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
      return true;
      
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
void RecordLoader::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
  
  m_aEndChunk    = nullptr;
  m_aNextRecord  = nullptr;
  m_nRecordBytes = 0;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RecordLoader::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
