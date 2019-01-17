/*============================================================================*/
/* Job_Report.cpp -- Report Job                                               */
/*============================================================================*/

// Job Short: Cat

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Report =
{
  "Job_Report"_Stz,   // name of the job class
  CN_JOB_REPORT,         // class enum
  JK_LOCAL,           // job kind
  "report"_Stz,       // comment about the job
  "report"_Stz,       // the prompt command
  nullptr,            // the second word
  nullptr,            // the _run task seed
  "report_req"_Stz,   // the _req task seed
  nullptr,            // the _rcv task seed
  nullptr,            // the _exe task seed
  nullptr,            // the _end task seed
  nullptr,            // the _can task seed
  false,              // check resources?
  false,              // detached job?
  false,              // buffer attached?
  JT_NONE,            // job trace
  pJob_Report_pre,    // prompt pre-process function
  vJob_Report_post,   // prompt post-process function
  pJob_Report_new,    // job new function
  vJob_Report_delete, // job delete function
  nullptr,            // job pre-send function
  nullptr,            // job post-receive function
  vJob_Report_show,   // job show function
  
  // help and example
  true,
  "[report <tableNm> <inPipeNm> [<outPipeNm>]"_Stz,
  "report sensorEvents\"/rapids/SensorPipe\" \"/rapids/Stage1Pipe\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_Report_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Sanity_class;
  
  return &xJobClass_Report;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Report* JobModel::_pJob_Report(void* aAddress)
{
  Job_Report* pJob_Report = reinterpret_cast<Job_Report*>(aAddress);
  
  RASSERT_TYPE(pJob_Report,CN_JOB_REPORT);
  
  return pJob_Report;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Report::operator new(size_t)
{
  Job_Report* pJob_Report =
    static_cast<Job_Report*>(HeapMem::pAllocate(CN_JOB_REPORT));
  
  return pJob_Report;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Report::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Report::operator delete(void* aAddress)
{
  Job_Report* pJob_Report = static_cast<Job_Report*>(aAddress);
  
  HeapMem::vFree(pJob_Report);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Report::Job_Report(Interface::Session* pSession)
: Job(CN_JOB_REPORT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  m_pInFile = nullptr;
  
  m_bNamedOut = false;
  m_sOutputPath = "stdout"_Stz;
  m_pOutFile = nullptr;
  
  _checkInitialised_(CN_JOB_REPORT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Report::~Job_Report(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  delete m_pInFile;
  delete m_pOutFile;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Report::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Report* pJob_Report = _pJob_Report(pObject);

  pJob_Report->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Report::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Report::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_Report_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Report* pJob_Report = new Job_Report(pSession);

  TokenVec* pTknVec = pSession->m_pCommandProcessor->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be at least 3 tokens
  if (nTokens < 3)
    return pJob_Report;

  // set the table name
  pJob_Report->m_sObjectName = pTknVec->pGet(1)->aText();

  // set the input file name
  pJob_Report->m_sFileName = pTknVec->pGet(2)->sText();
  
  // set the output file name
  if (3 <= nTokens) {
    pJob_Report->m_bNamedOut = true;
    pJob_Report->m_sOutputPath = pTknVec->pGet(3)->sText();
  }
  
  return pJob_Report;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Report_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_Report_new(Job* pArg)
{
  Job_Report* pJob_Report = new Job_Report(nullptr);
  
  return pJob_Report;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Report_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Report* pJob_Report = _pJob_Report(pJob);
  
  delete pJob_Report;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Report_show(Job* pJob)
{
  Job_Report* pJob_Report = _pJob_Report(pJob);

  pJob_Report->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin cat                                                                  */
/*----------------------------------------------------------------------------*/
bool Job_Report::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  if (! m_pInFile->bOpenRead(m_sFileName))
    return false;
  
  if (m_bNamedOut) {
     if (! m_pOutFile->bOpenWrite(m_sOutputPath))
       return false;
  }
  else {
    m_pOutFile->vUseStdout();
  }
  
  vTrace("Reporting from %s to %s\n"_Stz,m_sFileName.aStz(),m_sOutputPath.aStz());

  return true;
}

/*----------------------------------------------------------------------------*/
/* copy from input to output                                                  */
/*----------------------------------------------------------------------------*/
bool Job_Report::bTake(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  vTrace("Report...\n"_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end cat                                                                    */
/*----------------------------------------------------------------------------*/
void Job_Report::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  m_pInFile->vClose();
  m_pOutFile->vClose();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
