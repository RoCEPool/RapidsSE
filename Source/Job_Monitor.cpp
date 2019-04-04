/*============================================================================*/
/* Job_Monitor.cpp -- Monitor Job                                             */
/*============================================================================*/

// Job Short: Cat

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Monitor =
{
  CN_JOB_MONITOR,       // class enum
  JK_LOCAL,             // job kind
  RK_NONE,              // response kind
  "monitor system"_Stz, // comment about the job
  "monitor"_Stz,        // the prompt command
  nullptr,              // the second word
  nullptr,              // the _run task seed
  "monitor_req"_Stz,    // the _req task seed
  nullptr,              // the _rcv task seed
  nullptr,              // the _exe task seed
  nullptr,              // the _end task seed
  nullptr,              // the _can task seed
  false,                // check resources?
  false,                // detached job?
  false,                // buffer attached?
  JT_NONE,              // job trace
  DL_NONE,              // ddl lock model
  PY_NORMAL,            // priority
  JF_NONE,              // flags

  eJob_checkCluster,    // prompt check function
  pJob_Monitor_pre,     // prompt pre-process function
  vJob_Monitor_post,    // prompt post-process function
  pJob_Monitor_new,     // job new function
  vJob_Monitor_delete,  // job delete function
  nullptr,              // job pre-send function
  nullptr,              // job post-receive function
  vJob_Monitor_show,    // job show function
  
  // help and example
  true,
  "monitor [<pipeNm>]"_Stz,
  "monitor \"/tmp/MonitorPipe\" ;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Monitor_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_NetHog_class;
  
  return &xJobClass_Monitor;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Monitor* Jobs::_pJob_Monitor(void* aAddress)
{
  Job_Monitor* pJob_Monitor = reinterpret_cast<Job_Monitor*>(aAddress);
  
  RASSERT_TYPE(pJob_Monitor,CN_JOB_MONITOR);
  
  return pJob_Monitor;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Monitor::operator new(size_t)
{
  Job_Monitor* pJob_Monitor =
    static_cast<Job_Monitor*>(HeapMem::pAllocate(CN_JOB_MONITOR));
  
  return pJob_Monitor;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Monitor::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Monitor::operator delete(void* aAddress)
{
  Job_Monitor* pJob_Monitor = static_cast<Job_Monitor*>(aAddress);
  
  HeapMem::vFree(pJob_Monitor);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Monitor::Job_Monitor(Interface::Session* pSession)
: Job(CN_JOB_MONITOR, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_MONITOR,_FOOTER_);

  m_iInterval  = 10;
  m_bNamedFile = false;
  m_pOutFile   = nullptr;
  
  _checkInitialised_(CN_JOB_MONITOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_MONITOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Monitor::~Job_Monitor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_MONITOR,_FOOTER_);
  
  delete m_pOutFile;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Monitor::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pObject);

  pJob_Monitor->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Monitor::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_MONITOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Monitor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_MONITOR);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Monitor_pre(Interface::Session* pSession)
{
  i8 nTokens;

  Job_Monitor* pJob_Monitor = new Job_Monitor(pSession);

  TokenVec* pTknVec = pSession->m_pCommand->pTknVec();

  // get no of tokens in token vector
  nTokens = pTknVec->nTokens();

  if (2 <= nTokens)
    pJob_Monitor->m_iInterval = pTknVec->pGet(1)->iToI8();
  
  // set the output file name
  if (3 <= nTokens) {
    pJob_Monitor->m_bNamedFile = true;
    pJob_Monitor->m_sFilePath = pTknVec->pGet(2)->sText();
  }
  
  return pJob_Monitor;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Monitor_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Monitor_new(Job* pArg)
{
  Job_Monitor* pJob_Monitor = new Job_Monitor(nullptr);
  
  return pJob_Monitor;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Monitor_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pJob);
  
  delete pJob_Monitor;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Monitor_show(Job* pJob)
{
  Job_Monitor* pJob_Monitor = _pJob_Monitor(pJob);

  pJob_Monitor->vShow();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin monitor                                                              */
/*----------------------------------------------------------------------------*/
bool Job_Monitor::bBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_MONITOR,_FOOTER_);
  
  m_pOutFile = new File;
  
  if (m_bNamedFile) {
    if (! m_pOutFile->bOpenWrite(m_sFilePath))
      return false;
  }
  else {
    m_pOutFile->vUseStdout();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* write a portrait of the queues to output                                   */
/*----------------------------------------------------------------------------*/
bool Job_Monitor::bTake(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_MONITOR,_FOOTER_);
  
  s48K sRapidsPortrait;
  Engine::vMonitorPortrait(sRapidsPortrait);
  
  m_pOutFile->vPrint("%s\n"_Stz,sRapidsPortrait.aStz());

  ActivityMap::vMarkString(-1,sRapidsPortrait);

  return true;
}

/*----------------------------------------------------------------------------*/
/* end monitor                                                                */
/*----------------------------------------------------------------------------*/
void Job_Monitor::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_MONITOR,_FOOTER_);
  
  m_pOutFile->vClose();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
