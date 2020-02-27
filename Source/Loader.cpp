/*============================================================================*/
/* Loader.cpp -- Loader                                                       */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Loader* Load::_pLoader(void* aAddress)
{
  Loader* pLoader = reinterpret_cast<Loader*>(aAddress);
  
  RASSERT_TYPE(pLoader,CN_LOADER);
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t)
{
  Loader* pLoader =
    static_cast<Loader*>(HeapMem::pAllocate(CN_LOADER));
  
  return pLoader;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Loader::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Loader::operator delete(void* aAddress)
{
  Loader* pLoader = static_cast<Loader*>(aAddress);
  
  HeapMem::vFree(pLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loader::Loader(Table* pTable)
: WriteBehind(pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  _checkInitialised_(CN_JOB_SUBLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin loading                                                              */
/*----------------------------------------------------------------------------*/
//bool Job_SubLoader::bBegin(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,LOADER,_FOOTER_);
//
//  s256 sPath;
//  sPath = m_pJob_LoadTable->szFilePath();
//
//  // open the chunked file
//  bool bOpened = m_pLoadChunk->bOpenRead(sPath);
//
//  // set the table and parentJob in the writeBehind Job
//  JobWriteBehind::m_pTable = m_pJob_LoadTable->m_pTable;
//  JobWriteBehind::m_pParentJob = (Job*)m_pJob_LoadTable;
//  JobWriteBehind::m_oFormatters.vSet(&m_pJob_LoadTable->m_oFormatters);

//  return bOpened;
//}

/*----------------------------------------------------------------------------*/
/* chunk records                                                               */
/*----------------------------------------------------------------------------*/
//bool Loader::bChunk(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
//  
//  Session* pSession = m_pJob_LoadTable->pSession();
//  Table* pTable = m_pJob_LoadTable->m_pTable;
//  i8 iParentJobId = m_pJob_LoadTable->m_iFocalId;
//  Format* pFmt = &m_pJob_LoadTable->m_oFormatters;
//
//  i8 i_Seek;
//  i8 nChunkBytes;
//
//  Stz* aRecord;
//  i8 nRecordBytes;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // read next mapped chunk
//  if (! m_pJob_LoadTable->m_pLoadMap->bNextChunk(i_Seek,nChunkBytes))
//    return false;
//
//  m_pLoadChunk->vReadSpecificChunk(i_Seek,nChunkBytes);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // process the records in the chunk
//  for (;;) {
//
//    if (bTerminate())
//      return false;
//
//    // fetch next record from current chunk
//    bool bHaveRecord = m_pLoadChunk->bNextRecord(&aRecord,nRecordBytes);
//    if (! bHaveRecord)
//      return true;
//
//    // process that record into write behind
//    eEC eError = eProcessRecord(aRecord, nRecordBytes);
//
//    if (EC_NONE == eError) {
//      vIncRowsProcessed();
//      continue;
//    }
//
//    if (EC_BLANKLINE == eError)
//      continue;
//
//    if (EC_INVALID_VALUE == eError) {
//      vIncRowsInvalid();
//      ++m_pJob_LoadTable->m_ixErrors;
//      continue;
//    }
//
//    if (EC_MAX_ROW_LENGTH == eError) {
//      vIncRowsInvalid();
//      ++m_pJob_LoadTable->m_ixErrors;
//      continue;
//    }
//
//    return false;
//  }
//
//  NEVER_REACH
//}

/*----------------------------------------------------------------------------*/
/* end loading                                                                */
/*----------------------------------------------------------------------------*/
//void Job_SubLoader::vEnd(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
//
//  if (gpBufferPool->bLowMemory())
//    vTrace("Job_SubLoader #%lld -- ending with low memory\n"_Stz,
//           m_iFocalId);
//
//  // handle any dangling buffers
//  vHandleDanglingBuffers();
//
//  // add invalid rows into parent load table job
//  i8 iRowsInvalid = this->iRowsInvalid();
//  m_pJob_LoadTable->vAddRowsInvalid(iRowsInvalid);
//
//  // add processed rows into parent load table job
//  i8 iRowsProcessed = this->iRowsProcessed();
//  //! REMOVE THIS TRACE
//  //vTrace("iRowsProcessed %lld [RJH REMOVE]\n"_Stz,iRowsProcessed);
//  m_pJob_LoadTable->vAddRowsProcessed(iRowsProcessed);
//
//  // close the file being loaded
//  m_pLoadChunk->vCloseFile();
//}

/*----------------------------------------------------------------------------*/
/* terminate loading early?                                                   */
/*----------------------------------------------------------------------------*/
//bool Job_SubLoader::bTerminate(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
//
//  if (m_oFormatters.m_nMaxErrors &&
//      *m_pJob_LoadTable->m_ixErrors >= m_oFormatters.m_nMaxErrors) {
//    vTrace("Job_SubLoader #%lld -- terminating with max errors %lld %lld\n"_Stz,
//           m_iFocalId,*m_pJob_LoadTable->m_ixErrors,m_oFormatters.m_nMaxErrors);
//    return true;
//  }
//
//  if (gpBufferPool->bLowMemory()) {
//    vTrace("Job_SubLoader #%lld -- terminating with low memory\n"_Stz,
//           m_iFocalId);
//    return true;
//  }
//
//  return false;
//}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Loader)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
