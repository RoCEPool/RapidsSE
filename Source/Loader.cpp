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
: WriteHash(CN_LOADER, pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  m_pRecordData = new RecordData(&m_oFormatter);
  
  _checkInitialised_(CN_LOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loader::~Loader(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);
  
  delete m_pRecordData;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* read records                                                               */
/*----------------------------------------------------------------------------*/
bool Loader::bReadRecords(Job* pPipelineJob, Stz* aDataArea, i8 nDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

  Stz* aRecord;
  i8 nRecordBytes;
  
  m_pRecordData->vInit(aDataArea, nDataBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process the records in the chunk
  for (;;) {

    if (bTerminate())
      return false;

    // fetch next record from data area
    bool bHaveRecord = m_pRecordData->bNextRecord(&aRecord,nRecordBytes);
    if (! bHaveRecord)
      return true;

    // process that record into write behind
    eEC eError = eProcessRecord(aRecord, nRecordBytes);

    if (EC_NONE == eError) {
      //vTrace(aRecord);
      AM::vIncRowsProcessed(pPipelineJob);
      continue;
    }

    if (EC_BLANKLINE == eError)
      continue;

    if (EC_INVALID_VALUE == eError) {
      AM::vIncRowsInvalid(pPipelineJob);
      //++m_pJob_LoadTable->m_ixErrors;
      continue;
    }

    if (EC_MAX_ROW_LENGTH == eError) {
      AM::vIncRowsInvalid(pPipelineJob);
      //++m_pJob_LoadTable->m_ixErrors;
      continue;
    }

    return false;
  }

  NEVER_REACH
}

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
bool Loader::bTerminate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOADER,_FOOTER_);

//  if (m_oFormatters.m_nMaxErrors &&
//      *m_pJob_LoadTable->m_ixErrors >= m_oFormatters.m_nMaxErrors) {
//    vTrace("Loader -- terminating with max errors %lld %lld\n"_Stz,
//           *m_pJob_LoadTable->m_ixErrors,m_oFormatters.m_nMaxErrors);
//    return true;
//  }
//
//  if (gpBufferPool->bLowMemory()) {
//    vTrace("Job_SubLoader #%lld -- terminating with low memory\n"_Stz,
//           m_iFocalId);
//    return true;
//  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Loader)


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
