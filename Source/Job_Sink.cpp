/*============================================================================*/
/* Job_Sink.cpp -- Sink Job                                                   */
/*============================================================================*/

// Job Short: Sink

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Sink =
{
  "Job_Sink"_Stz,      // name of the job class
  CN_JOB_INGEST,         // class enum
  JK_LOCAL,              // job kind
  "sink data"_Stz,     // comment about the job
  "sink"_Stz,          // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "sink_req"_Stz,      // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_Sink_pre,       // prompt pre-process function
  vJob_Sink_post,      // prompt post-process function
  pJob_Sink_new,       // job new function
  vJob_Sink_delete,    // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Sink_show,      // job show function
  
  // help and example
  true,
  "sink <tableNm> <pipeNm>"_Stz,
  "sink SensorEvents \"/rapids/SensorPipe\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_Sink_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertRow_class;
  
  return &xJobClass_Sink;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Sink* JobsModel::_pJob_Sink(void* aAddress)
{
  Job_Sink* pJob_Sink = reinterpret_cast<Job_Sink*>(aAddress);
  
  RASSERT_TYPE(pJob_Sink,CN_JOB_INGEST);
  
  return pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Sink::operator new(size_t)
{
  Job_Sink* pJob_Sink =
    static_cast<Job_Sink*>(HeapMem::pAllocate(CN_JOB_INGEST));
  
  return pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Sink::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Sink::operator delete(void* aAddress)
{
  Job_Sink* pJob_Sink = static_cast<Job_Sink*>(aAddress);
  
  HeapMem::vFree(pJob_Sink);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Sink::Job_Sink(Interface::Session* pSession)
: Job(CN_JOB_INGEST, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  m_pTable = nullptr;
  
  m_pIngestChunk  = new IngestChunk(&gpAttachMap->m_xFormatters);
  m_aRawData      = nullptr;
  m_nRawDataBytes = 0;
  
  _checkInitialised_(CN_JOB_INGEST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Sink::~Job_Sink(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  delete m_pIngestChunk;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Sink::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Sink* pJob_Sink = _pJob_Sink(pObject);

  pJob_Sink->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Sink::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Sink::aFooter(void)
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
Job* JobsModel::pJob_Sink_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Sink* pJob_Sink = new Job_Sink(pSession);

  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be 3 tokens
  if (nTokens < 3)
    return pJob_Sink;

  // set the table name
  pJob_Sink->m_sObjectName = pTknVec->pGet(1)->aText();

  // set the file name
  if (pTknVec->pGet(2)->sText().bSame("pipe"_Stz))
    pJob_Sink->m_sFileName = pTknVec->pGet(3)->aText();
  else
    pJob_Sink->m_sFileName = pTknVec->pGet(2)->aText();

  return pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Sink_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Sink_new(Job* pArg)
{
  Job_Sink* pJob_Sink = new Job_Sink(nullptr);
  
  return pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Sink_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Sink* pJob_Sink = _pJob_Sink(pJob);
  
  delete pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Sink_show(Job* pJob)
{
  Job_Sink* pJob_Sink = _pJob_Sink(pJob);

  pJob_Sink->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert record into row                                                    */
/*----------------------------------------------------------------------------*/
void Job_Sink::vConvertRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* sink data for table                                                      */
/*----------------------------------------------------------------------------*/
void Job_Sink::vIngest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
 vTrace("Ingesting data into %s from %s\n"_Stz,
         m_sObjectName.aStr(), m_sFileName.aStr());
  
  bool bOpened = m_pIngestChunk->bOpenFile(m_sFileName);
  if (! bOpened) {
    vTrace("Unable to open pipe: %s\n"_Stz,m_sFileName.aStr());
    return;
  }
  
  bool bResult = bIngestPipe();
  RASSERT(bResult);
  
  m_pIngestChunk->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* sink pipe of records                                                     */
/*----------------------------------------------------------------------------*/
bool Job_Sink::bIngestPipe(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  i8 iParentJobId = m_iJobId;
  bool bBlankLine = false;
  
  while (bReadRecord()) {
    //vConvertRecord();
    //vInsertRow();
    
    bool bFormat = bFormatRow(m_pTable, &gpAttachMap->m_xFormatters, m_aRawData, m_nRawDataBytes, bBlankLine);
    RASSERT( ! bBlankLine );
    
    // attempt to add record into a buffer
    eEC eErrorCode = eInsertRow(m_pSession, m_pTable, DS_INGEST, iParentJobId);
    
    // was there an error ?
    if (eErrorCode != EC_NONE) {
      vTrace("Inserting row into buffer got error %s\n"_Stz, aEC[eErrorCode]);
      return false;
    }
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert row into table                                                      */
/*----------------------------------------------------------------------------*/
void Job_Sink::vInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  s48K sRecord;
  
  MemLib::vCopy(sRecord.aChar(),m_aRawData,m_nRawDataBytes,CN_JOB_INGEST);
  
  sRecord.aChar()[m_nRawDataBytes] = '\0';
  
  vTrace(" : %s\n"_Stz,sRecord.aChar());
}

/*----------------------------------------------------------------------------*/
/* read record from pipe                                                      */
/*----------------------------------------------------------------------------*/
bool Job_Sink::bReadRecord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  m_aRawData = m_pIngestChunk->aGetRecord(m_nRawDataBytes);

  if (m_aRawData)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* Job_Sink::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Sink::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_INGEST,_FOOTER_);
  
  m_pTable = pTable;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
