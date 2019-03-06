/*============================================================================*/
/* Job_Report.cpp -- Report Job                                               */
/*============================================================================*/

// Job Short: Cat

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Report =
{
  CN_JOB_REPORT,      // class enum
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
  DL_NONE,            // ddl lock model
  PY_NORMAL,          // priority
  JF_NONE,            // flags

  eJob_checkCluster,  // prompt check function
  pJob_Report_pre,    // prompt pre-process function
  vJob_Report_post,   // prompt post-process function
  pJob_Report_new,    // job new function
  vJob_Report_delete, // job delete function
  nullptr,            // job pre-send function
  nullptr,            // job post-receive function
  vJob_Report_show,   // job show function
  
  // help and example
  true,
  "report <tableNm> <inPipeNm> [<outPipeNm>]"_Stz,
  "report sensorEvents\"/rapids/SensorPipe\" \"/rapids/Stage1Pipe\";"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Report_class(pFUN** pNextFun)
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
Job_Report* Jobs::_pJob_Report(void* aAddress)
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
  
  m_bNamedOut = false;
  m_sOutputPath = "stdout"_Stz;
  
  m_pRowAgg     = nullptr;
  m_pRowAgg_10s = nullptr;
  m_pRowAgg_1m  = nullptr;
  m_pRowAgg_3m  = nullptr;
  m_pRowAgg_15m = nullptr;
  
  m_iLast_10s   = -1;
  m_iLast_1m    = -1;
  m_iLast_3m    = -1;
  m_iLast_15m   = -1;

  _checkInitialised_(CN_JOB_REPORT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Report::~Job_Report(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  delete m_pRowAgg;
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
Job* Jobs::pJob_Report_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Report* pJob_Report = new Job_Report(pSession);

  TokenVec* pTknVec = pSession->m_pCommand->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  // there should be at least 3 tokens
  if (nTokens < 3)
    return pJob_Report;

  // set the table name
  pJob_Report->m_sObjectName = pTknVec->pGet(1)->aText();

  // set the input file name
  pJob_Report->m_sFilePath = pTknVec->pGet(2)->sText();
  
  // set the output file name
  if (4 <= nTokens) {
    pJob_Report->m_bNamedOut = true;
    pJob_Report->m_sOutputPath = pTknVec->pGet(3)->sText();
  }
  
  return pJob_Report;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Report_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Report_new(Job* pArg)
{
  Job_Report* pJob_Report = new Job_Report(nullptr);
  
  return pJob_Report;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Report_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Report* pJob_Report = _pJob_Report(pJob);
  
  delete pJob_Report;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Report_show(Job* pJob)
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
  
  if (! m_oInFile.bOpenRead(m_sFilePath))
    return false;
  
  if (m_bNamedOut) {
     if (! m_oOutFile.bOpenWrite(m_sOutputPath))
       return false;
  }
  else {
    m_oOutFile.vUseStdout();
  }
  
  m_pRowAgg     = new RowAgg;
  m_pRowAgg_10s = new RowAgg;
  m_pRowAgg_1m  = new RowAgg;
  m_pRowAgg_3m  = new RowAgg;
  m_pRowAgg_15m = new RowAgg;
  
  m_oBegan.vNow();

  vTrace("Reporting from %s to %s\n"_Stz,m_sFilePath.aStz(),m_sOutputPath.aStz());

  return true;
}

/*----------------------------------------------------------------------------*/
/* copy from input to output                                                  */
/*----------------------------------------------------------------------------*/
bool Job_Report::bTake(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  //vTrace("Report...\n"_Stz);
  m_pRowAgg->vReadIn(m_oInFile);
  //m_pRowAgg->vPrintOut(m_oOutFile);

  i8 iDeltaSeconds = m_oBegan.iDeltaFromThenSeconds();
  
  // initial second
  if (! iDeltaSeconds) {
    m_pRowAgg_10s->vAdd(m_pRowAgg);
    return true;
  }
  
  if (iDeltaSeconds != m_iLast_15m && 0 == iDeltaSeconds % (60*15)) {
    // every 15 minutes
    vPrint("Last 15 min:"_Stz,m_pRowAgg_15m);
    m_pRowAgg_15m->vReset();
    m_pRowAgg_3m->vReset();
    m_pRowAgg_1m->vReset();
    m_pRowAgg_10s->vReset();
    m_iLast_15m = iDeltaSeconds;
  }
  else
  if (iDeltaSeconds != m_iLast_3m && 0 == iDeltaSeconds % (60*3)) {
    // every 3 minutes
    vPrint("Last 3 min:"_Stz,m_pRowAgg_3m);
    m_pRowAgg_3m->vReset();
    m_pRowAgg_1m->vReset();
    m_pRowAgg_10s->vReset();
    m_iLast_3m = iDeltaSeconds;
  }
  else
  if (iDeltaSeconds != m_iLast_1m && 0 == iDeltaSeconds % 60) {
    // every minute
    vPrint("Last 1 min:"_Stz,m_pRowAgg_1m);
    m_pRowAgg_1m->vReset();
    m_pRowAgg_10s->vReset();
    m_iLast_1m = iDeltaSeconds;
  }
  else
  if (iDeltaSeconds != m_iLast_10s && 0 == iDeltaSeconds % 10) {
    // every 10 seconds
    vPrint("Last 10 sec:"_Stz,m_pRowAgg_10s);
    m_pRowAgg_10s->vReset();
    m_iLast_10s = iDeltaSeconds;
  }

  // every second
  m_pRowAgg_10s->vAdd(m_pRowAgg);
  m_pRowAgg_1m->vAdd(m_pRowAgg);
  m_pRowAgg_3m->vAdd(m_pRowAgg);
  m_pRowAgg_15m->vAdd(m_pRowAgg);

  return true;
}

/*----------------------------------------------------------------------------*/
/* end cat                                                                    */
/*----------------------------------------------------------------------------*/
void Job_Report::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  m_oInFile.vClose();
  m_oOutFile.vClose();
  
  delete m_pRowAgg;
  delete m_pRowAgg_10s;
  delete m_pRowAgg_1m;
  delete m_pRowAgg_3m;
  delete m_pRowAgg_15m;
}

/*----------------------------------------------------------------------------*/
/* print report                                                               */
/*----------------------------------------------------------------------------*/
void Job_Report::vPrint(Stz* aLabel, RowAgg* pRowAgg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_REPORT,_FOOTER_);
  
  m_oOutFile.vPrint("%s\n"_Stz,aLabel);
  pRowAgg->vPrintOut(m_oOutFile);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
