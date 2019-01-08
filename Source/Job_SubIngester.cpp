/*============================================================================*/
/* Job_SubIngester.cpp -- Table SubIngester Job                               */
/*============================================================================*/

// Job Short: SubIngester

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubIngester =
{
  "Job_SubIngester"_Stz,    // name of the job class
  CN_JOB_SUBINGESTER,       // class enum
  JK_CHILD,                 // job kind
  "table sub-ingester"_Stz, // comment about the job
  nullptr,                  // the prompt command
  nullptr,                  // the second word
  "subIngester_run"_Stz,    // the _run task seed
  nullptr,                  // the _req task seed
  nullptr,                  // the _rcv task seed
  nullptr,                  // the _exe task seed
  nullptr,                  // the _end task seed
  nullptr,                  // the _can task seed
  false,                    // check resources?
  false,                    // detached job?
  false,                    // buffer attached?
  JT_NONE,                  // job trace
  vJob_SubIngester_pre,     // prompt pre-process function
  nullptr,                  // prompt post-process function
  pJob_SubIngester_new,     // job new function
  vJob_SubIngester_delete,  // job delete function
  nullptr,                  // job pre-send function
  nullptr,                  // job post-receive function
  vJob_SubIngester_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_SubIngester_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubLoader_class;
  
  return &xJobClass_SubIngester;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubIngester* JobModel::_pJob_SubIngester(void* aAddress)
{
  Job_SubIngester* pJob_SubIngester = reinterpret_cast<Job_SubIngester*>(aAddress);
  
  RASSERT_TYPE(pJob_SubIngester,CN_JOB_SUBINGESTER);
  
  return pJob_SubIngester;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubIngester::operator new(size_t)
{
  Job_SubIngester* pJob_SubIngester =
    static_cast<Job_SubIngester*>(HeapMem::pAllocate(CN_JOB_SUBINGESTER));
  
  return pJob_SubIngester;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubIngester::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubIngester::operator delete(void* aAddress)
{
  Job_SubIngester* pJob_SubIngester = static_cast<Job_SubIngester*>(aAddress);
  
  HeapMem::vFree(pJob_SubIngester);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubIngester::Job_SubIngester(Interface::Session* pSession)
: Job(CN_JOB_SUBINGESTER, pSession) , JobWriteBehind(DS_INGEST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
  
  m_pJob_Ingest = nullptr;
  
  _checkInitialised_(CN_JOB_SUBINGESTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubIngester::~Job_SubIngester(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubIngester::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pObject);

  pJob_SubIngester->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get job_ingest                                                             */
/*----------------------------------------------------------------------------*/
Job_Ingest* Job_SubIngester::pJob_Ingest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
  
  return m_pJob_Ingest;
}

/*----------------------------------------------------------------------------*/
/* set job_loadtable                                                          */
/*----------------------------------------------------------------------------*/
void Job_SubIngester::vJob_Ingest(Job_Ingest* pJob_Ingest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
  
  m_pJob_Ingest = pJob_Ingest;
}


/*----------------------------------------------------------------------------*/
/* begin sub-ingesting                                                        */
/*----------------------------------------------------------------------------*/
bool Job_SubIngester::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
  
  // set the table and parentJob in the writeBehind Job
  JobWriteBehind::m_pTable = m_pJob_Ingest->m_pTable;
  JobWriteBehind::m_pParentJob = m_pJob_Ingest;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* chunk records                                                               */
/*----------------------------------------------------------------------------*/
bool Job_SubIngester::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
  
  IngestChunk* pIngestChunk = m_pJob_Ingest->pPopIngestChunk();
  if (! pIngestChunk)
    return false;
  
  //vTrace("Job_SubIngester: got IngestChunk %lld\n"_Stz,pIngestChunk->m_iChunkNo);
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process the records in the ingest chunk
  
  Stz* aRecord;
  i8   nRecordBytes;

  while (pIngestChunk->bGetRecord(&aRecord,nRecordBytes)) {
    
    // process that record into write behind
    eEC eError = eProcessRecord(&gpAttachMap->m_xFormatters,
                                aRecord, nRecordBytes, DS_INGEST);
    
    if (EC_NONE == eError)
      continue;
    
    if (EC_BLANKLINE == eError)
      continue;
    
    if (EC_MAX_ROW_LENGTH == eError)
      continue;
    
    delete pIngestChunk;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // finished processing the ingest chunk
  
  delete pIngestChunk;
  return true;
}
/*----------------------------------------------------------------------------*/
//bool Job_SubIngester::bChunk(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
//
//  IngestChunk* pIngestChunk = m_pJob_Ingest->pPopIngestChunk();
//
//  if (! pIngestChunk) {
//    return false;
//  }
//
//  //vTrace("Job_SubIngester: got IngestChunk %lld\n"_Stz,pIngestChunk->m_iChunkNo);
//
//  m_pRecordIngestor->vCopy(pIngestChunk);
//
//  m_pRecordIngestor->m_aNextRecord = (Stz*)m_pRecordIngestor->aDataArea();
//
//  delete pIngestChunk;
//
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // process the records in the chunk
//
//
//  Stz* aRecord;
//  i8   nRecordBytes;
//
//  while (m_pRecordIngestor->bGetRecord(&aRecord,nRecordBytes)) {
//
//    // process that record into write behind
//    eEC eError = eProcessRecord(m_pJob_Ingest->m_pSession,
//                                m_pJob_Ingest->m_iJobId,
//                                m_pJob_Ingest->m_pTable,
//                                &gpAttachMap->m_xFormatters,
//                                aRecord, nRecordBytes, DS_INGEST);
//
//    if (EC_NONE == eError)
//      continue;
//
//    if (EC_BLANKLINE == eError)
//      continue;
//
//    if (EC_MAX_ROW_LENGTH == eError)
//      continue;
//
//    return false;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  return true;
//}

/*----------------------------------------------------------------------------*/
/* end sub-ingesting                                                          */
/*----------------------------------------------------------------------------*/
void Job_SubIngester::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
  
  vHandleDanglingBuffers();
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubIngester::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubIngester::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINGESTER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::vJob_SubIngester_pre(Interface::Session* pSession)
{
  Job_SubIngester* pJob_SubIngester = new Job_SubIngester(pSession);
  
  return pJob_SubIngester;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_SubIngester_new(Job* pArg)
{
  Job_SubIngester* pJob_SubIngester = new Job_SubIngester(nullptr);
  
  return pJob_SubIngester;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubIngester_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pJob);
  
  delete pJob_SubIngester;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_SubIngester_show(Job* pJob)
{
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pJob);

  pJob_SubIngester->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
