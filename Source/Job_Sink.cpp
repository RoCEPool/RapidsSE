/*============================================================================*/
/* Job_Sink.cpp -- Sink Job                                                   */
/*============================================================================*/

// Job Short: Sink

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Sink =
{
  "Job_Sink"_Stz,      // name of the job class
  CN_JOB_SINK,         // class enum
  JK_LOCAL,            // job kind
  "sink data"_Stz,     // comment about the job
  "sink"_Stz,          // the prompt command
  nullptr,             // the second word
  nullptr,             // the _run task seed
  "sink_req"_Stz,      // the _req task seed
  nullptr,             // the _rcv task seed
  nullptr,             // the _exe task seed
  nullptr,             // the _end task seed
  nullptr,             // the _can task seed
  false,               // check resources?
  false,               // detached job?
  false,               // buffer attached?
  JT_NONE,             // job trace
  pJob_Sink_pre,       // prompt pre-process function
  vJob_Sink_post,      // prompt post-process function
  pJob_Sink_new,       // job new function
  vJob_Sink_delete,    // job delete function
  nullptr,             // job pre-send function
  nullptr,             // job post-receive function
  vJob_Sink_show,      // job show function
  
  // help and example
  true,
  "sink <pipeNm>"_Stz,
  "sink \"/rapids/SensorPipe\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Sink_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SizeIndex_class;
  
  return &xJobClass_Sink;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Sink* Jobs::_pJob_Sink(void* aAddress)
{
  Job_Sink* pJob_Sink = reinterpret_cast<Job_Sink*>(aAddress);
  
  RASSERT_TYPE(pJob_Sink,CN_JOB_SINK);
  
  return pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Sink::operator new(size_t)
{
  Job_Sink* pJob_Sink =
    static_cast<Job_Sink*>(HeapMem::pAllocate(CN_JOB_SINK));
  
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
: Job(CN_JOB_SINK, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SINK,_FOOTER_);
  
  m_pIoChunk  = new IoChunk(&gpAttachMap->m_oFormatters);
  m_pByteRate = nullptr;
  
  _checkInitialised_(CN_JOB_SINK);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SINK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Sink::~Job_Sink(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SINK,_FOOTER_);
  
  delete m_pIoChunk;
  delete m_pByteRate;
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SINK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Sink::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SINK);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Sink_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Sink* pJob_Sink = new Job_Sink(pSession);

  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be at least 2 tokens
  if (nTokens < 2)
    return pJob_Sink;

  // set the file name
  pJob_Sink->m_sFileName = pTknVec->pGet(1)->sText();
  
  // is the last token the stats option?
  i8 i_Last = nTokens - 1;
  if (pTknVec->pGet(i_Last)->sText().bSame("stats"_Stz) ||
      pTknVec->pGet(i_Last)->sText().bSame("statistics"_Stz))
    pJob_Sink->m_pByteRate = new ByteRate;
  
  return pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Sink_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Sink_new(Job* pArg)
{
  Job_Sink* pJob_Sink = new Job_Sink(nullptr);
  
  return pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Sink_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Sink* pJob_Sink = _pJob_Sink(pJob);
  
  delete pJob_Sink;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Sink_show(Job* pJob)
{
  Job_Sink* pJob_Sink = _pJob_Sink(pJob);

  pJob_Sink->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin sink                                                                 */
/*----------------------------------------------------------------------------*/
bool Job_Sink::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SINK,_FOOTER_);
  
  vTrace("Sink data from %s\n"_Stz,m_sFileName.aStz());
  
  bool bOpened = m_pIoChunk->bOpenRead(m_sFileName);
  
  if (m_pByteRate) {
    m_pByteRate->vSetReporting("  sink : "_Stz,C_1GB * 10);
    m_pByteRate->vBeginTiming();
  }
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* sink data                                                                  */
/*----------------------------------------------------------------------------*/
bool Job_Sink::bChunk(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SINK,_FOOTER_);
  
  i8 nBytesRead = m_pIoChunk->nReadBytes(-1);
  if (! nBytesRead)
    return false;
  
  //vTrace("[[chunk]]\n%s\n[[end-chunk]]\n"_Stz,m_pIoChunk->aStz());
  
  if (m_pByteRate)
    m_pByteRate->vAddBytes(nBytesRead);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end sink                                                                   */
/*----------------------------------------------------------------------------*/
void Job_Sink::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SINK,_FOOTER_);
  
  if (m_pByteRate)
    m_pByteRate->vEndTiming();
  
  m_pIoChunk->vCloseFile();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
