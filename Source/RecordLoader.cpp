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
: RecordExtractor(CN_RECORDLOADER,pFormatters)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORDLOADER,_FOOTER_);
 
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
  Stz* aStartRecord = m_aNextRecord;
  Stz* aEndRecord   = m_aNextRecord;

  bool bFound = bFindRecordEnd((char**)&aEndRecord);
  ASSERT(bFound);

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
