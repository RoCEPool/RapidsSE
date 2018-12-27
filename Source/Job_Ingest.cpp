/*============================================================================*/
/* Job_Ingest.cpp -- Ingest Job                                               */
/*============================================================================*/

// Job Short: Ingest

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Ingest =
{
  "Job_Ingest"_Stz,      // name of the job class
  CN_JOB_INGEST,         // class enum
  JK_LOCAL,              // job kind
  "ingest data"_Stz,     // comment about the job
  "ingest"_Stz,          // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "ingest_req"_Stz,      // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_Ingest_pre,       // prompt pre-process function
  vJob_Ingest_post,      // prompt post-process function
  pJob_Ingest_new,       // job new function
  vJob_Ingest_delete,    // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Ingest_show,      // job show function
  
  // help and example
  true,
  "ingest <tableNm> <pipeNm>"_Stz,
  "ingest SensorEvents \"/rapids/SensorPipe\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_Ingest_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertRow_class;
  
  return &xJobClass_Ingest;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Ingest* JobModel::_pJob_Ingest(void* aAddress)
{
  Job_Ingest* pJob_Ingest = reinterpret_cast<Job_Ingest*>(aAddress);
  
  RASSERT_TYPE(pJob_Ingest,CN_JOB_INGEST);
  
  return pJob_Ingest;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Ingest::operator new(size_t)
{
  Job_Ingest* pJob_Ingest =
    static_cast<Job_Ingest*>(HeapMem::pAllocate(CN_JOB_INGEST));
  
  return pJob_Ingest;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Ingest::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Ingest::operator delete(void* aAddress)
{
  Job_Ingest* pJob_Ingest = static_cast<Job_Ingest*>(aAddress);
  
  HeapMem::vFree(pJob_Ingest);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Ingest::Job_Ingest(Interface::Session* pSession)
: Job(CN_JOB_INGEST, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  m_pTable = nullptr;
  
  m_pRecordIngestor  = new RecordIngestor(&gpAttachMap->m_xFormatters);
  m_aRecord      = nullptr;
  m_nBytes = 0;
  
  _checkInitialised_(CN_JOB_INGEST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Ingest::~Job_Ingest(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  delete m_pRecordIngestor;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Ingest::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pObject);

  pJob_Ingest->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Ingest::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Ingest::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_Ingest_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Ingest* pJob_Ingest = new Job_Ingest(pSession);

  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 3 tokens
  if (nTokens < 3)
    return pJob_Ingest;

  // set the table name
  pJob_Ingest->m_sObjectName = pTknVec->pGet(1)->aText();

  // set the file name
  if (pTknVec->pGet(2)->sText().bSame("pipe"_Stz))
    pJob_Ingest->m_sFileName = pTknVec->pGet(3)->aText();
  else
    pJob_Ingest->m_sFileName = pTknVec->pGet(2)->aText();

  return pJob_Ingest;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Ingest_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_Ingest_new(Job* pArg)
{
  Job_Ingest* pJob_Ingest = new Job_Ingest(nullptr);
  
  return pJob_Ingest;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Ingest_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pJob);
  
  delete pJob_Ingest;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Ingest_show(Job* pJob)
{
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pJob);

  pJob_Ingest->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin ingesting                                                            */
/*----------------------------------------------------------------------------*/
void Job_Ingest::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  m_pRecordIngestor->vOpenRead(m_sFileName);
  //  bool bOpened = m_pRecordIngestor->bOpenFile(m_sFileName);
  //  if (! bOpened) {
  //    vTrace("Unable to open pipe: %s\n"_Stz,m_sFileName.aStr());
  //    return;
  //  }
  
  vTrace("Ingesting data into %s from %s\n"_Stz,
         m_sObjectName.aStr(), m_sFileName.aStr());
}

/*----------------------------------------------------------------------------*/
/* ingest data for table (return true if function is to be called again)      */
/*----------------------------------------------------------------------------*/
bool Job_Ingest::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read next sequential chunk
  if (! m_pRecordIngestor->bReadNextChunk())
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // process the records in the chunk
  for (;;) {
    
    // fetch next record from current chunk
    bool bHaveRecord = m_pRecordIngestor->bGetRecord(&m_aRecord,m_nBytes);
    if (! bHaveRecord)
      return true;
    
    // process that record into write behind
    eEC eError = eProcessRecord(m_pSession, m_iJobId, m_pTable,
                                &gpAttachMap->m_xFormatters,
                                m_aRecord, m_nBytes, DS_INGEST);
    
    if (EC_NONE == eError)
      continue;
    
    if (EC_BLANKLINE == eError)
      continue;
    
    if (EC_MAX_ROW_LENGTH == eError)
      continue;
    
    return false;
  }
  
  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* end ingesting                                                              */
/*----------------------------------------------------------------------------*/
void Job_Ingest::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  // handle any dangling buffers
  vHandleDanglingBuffers(m_pSession, m_pTable, DS_INGEST, -1);
  
  // close the file being loaded
  m_pRecordIngestor->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Job_Ingest::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Ingest::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  m_pTable = pTable;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
