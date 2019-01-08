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
  
  m_pQuantaChunk  = nullptr;
  m_pRemainderChunk = nullptr;

  m_pByteRate = nullptr;
  
  m_nChunksSoFar = 0;
  m_iLastChunk = -1;
  m_nWaits = 0;

  _checkInitialised_(CN_JOB_INGEST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Ingest::~Job_Ingest(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  delete m_pByteRate;
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
  pJob_Ingest->m_sFileName = pTknVec->pGet(2)->aText();
  
  // is the last token the stats option?
  i8 i_Last = nTokens - 1;
  if (pTknVec->pGet(i_Last)->sText().bSame("stats"_Stz) ||
      pTknVec->pGet(i_Last)->sText().bSame("statistics"_Stz))
    pJob_Ingest->m_pByteRate = new ByteRate;

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
bool Job_Ingest::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  vTrace("Ingesting data into %s from %s\n"_Stz,
         m_sObjectName.aStz(), m_sFileName.aStz());
  
  bool bOpened = m_oFile.bOpenRead(m_sFileName);
  
  s256 sPrefix;
  
  sPrefix.vSprint("  ingest : "_Stz);
//  sPrefix.vSprint("  ingest (%lld subIngesters) : "_Stz,
//                  gpAttachMap->m_nSubIngesters);

  if (m_pByteRate) {
    m_pByteRate->vSetReporting(sPrefix,C_1GB * 1);
    m_pByteRate->vBeginTiming();
  }

  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* ingest data for table (return true if function is to be called again)      */
/*----------------------------------------------------------------------------*/
bool Job_Ingest::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  if (! m_pQuantaChunk)
    m_pQuantaChunk = new IngestChunk(&gpAttachMap->m_xFormatters);
  
  if (! m_pRemainderChunk)
    m_pRemainderChunk = new IngestChunk(&gpAttachMap->m_xFormatters);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read next chunk, adjusting for any carry over from last chunk
  
  i8 nBytesRead = m_pQuantaChunk->nReadMore(m_oFile);
  if  (! nBytesRead) {
    m_iLastChunk = m_nChunksSoFar;
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // identify the set of whole records and move remainder into next chunk
  
  i8 nQuantumBytes   = m_pQuantaChunk->nQuantumBytes();
  i8 nRemainingBytes = m_pQuantaChunk->m_nDataBytes - nQuantumBytes;

  void* aCopyFrom = pAdd<void*>(m_pQuantaChunk->aDataArea(),nQuantumBytes);
  void* aCopyInto = m_pRemainderChunk->aDataArea();
  
  //MemLib::vHexDump((char*)aCopyFrom-C_BYTES_PER_LINE,nRemainingBytes+C_BYTES_PER_LINE);
  
  MemLib::vCopy(aCopyInto,aCopyFrom,nRemainingBytes,CN_JOB_INGEST);
  
  m_pQuantaChunk->vSetBytes(nQuantumBytes);  
  m_pRemainderChunk->vSetBytes(nRemainingBytes);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // queue the read quantum for processing by a sub-ingester
  
  if (nIngestChunks() > 32) {
    m_nWaits++;
    while (nIngestChunks() > 32)
      PosixThread::vSleepMilli(1);
  }
  
  m_pQuantaChunk->m_iChunkNo = ++m_nChunksSoFar;
  
  vAppendIngestChunk(m_pQuantaChunk);
  m_pQuantaChunk = nullptr;

  if (m_pByteRate)
    m_pByteRate->vAddBytes(nBytesRead);

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set quanta for next read
  
  m_pQuantaChunk = m_pRemainderChunk;
  m_pRemainderChunk = nullptr;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end ingesting                                                              */
/*----------------------------------------------------------------------------*/
void Job_Ingest::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  // handle any dangling buffers
  vHandleDanglingBuffers(m_pSession, m_pTable, DS_INGEST, -1);
  
  if (m_pByteRate)
    m_pByteRate->vEndTiming();
  
  // close the file being loaded
  m_oFile.vClose();
  
  vTrace("Job_Ingest : %lld waits\n"_Stz,m_nWaits);
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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append ingest chunk to queue                                               */
/*----------------------------------------------------------------------------*/
void Job_Ingest::vAppendIngestChunk(IngestChunk* pIngestChunk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  vAppend(pIngestChunk);
}

/*----------------------------------------------------------------------------*/
/* get number of ingest chunks in queue                                       */
/*----------------------------------------------------------------------------*/
i8 Job_Ingest::nIngestChunks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  i8 nChunks = iCount();
  
  return nChunks;
}

/*----------------------------------------------------------------------------*/
/* pop next ingest chunk from queue                                           */
/*----------------------------------------------------------------------------*/
IngestChunk* Job_Ingest::pPopIngestChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  IngestChunk* pIngestChunk = npPop();
  
  return pIngestChunk;
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
