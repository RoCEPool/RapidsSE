/*============================================================================*/
/* Job_ShowCluster.cpp -- Show Cluster Job                                    */
/*============================================================================*/

// Job Short: ShowCluster

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ShowCluster =
{
  "Job_ShowCluster"_Stz, // name of the job class
  CN_JOB_SHOWCLUSTER,    // class enum
  JK_LOCAL,              // job kind
  "show cluster"_Stz,    // comment about the job
  "showCluster"_Stz,     // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "showCluster_req"_Stz, // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_NONE,               // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_ShowCluster_pre,  // prompt pre-process function
  vJob_ShowCluster_post, // prompt post-process function
  nullptr,               // job new function
  vJob_ShowCluster_delete,  // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_ShowCluster_show, // job show function
  
  // help and example
  true,
  "showCluster"_Stz,
  "showCluster;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ShowCluster_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Sink_class;
  
  return &xJobClass_ShowCluster;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ShowCluster* Jobs::_pJob_ShowCluster(void* aAddress)
{
  Job_ShowCluster* pJob_ShowCluster = reinterpret_cast<Job_ShowCluster*>(aAddress);
  
  RASSERT_TYPE(pJob_ShowCluster,CN_JOB_SHOWCLUSTER);
  
  return pJob_ShowCluster;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ShowCluster::operator new(size_t)
{
  Job_ShowCluster* pJob_ShowCluster =
    static_cast<Job_ShowCluster*>(HeapMem::pAllocate(CN_JOB_SHOWCLUSTER));
  
  return pJob_ShowCluster;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ShowCluster::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ShowCluster::operator delete(void* aAddress)
{
  Job_ShowCluster* pJob_ShowCluster = static_cast<Job_ShowCluster*>(aAddress);
  
  HeapMem::vFree(pJob_ShowCluster);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ShowCluster::Job_ShowCluster(Interface::Session* pSession)
: Job(CN_JOB_SHOWCLUSTER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SHOWCLUSTER,_FOOTER_);
  
  _checkInitialised_(CN_JOB_SHOWCLUSTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SHOWCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ShowCluster::~Job_ShowCluster(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SHOWCLUSTER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ShowCluster::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ShowCluster* pJob_ShowCluster = _pJob_ShowCluster(pObject);

  pJob_ShowCluster->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ShowCluster::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SHOWCLUSTER,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("pwf"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be only 1 tokens
  if (iNumTokens != 1) {
    return nullptr;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ShowCluster::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SHOWCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ShowCluster::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SHOWCLUSTER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ShowCluster_pre(Interface::Session* pSession)
{
  Job_ShowCluster* pJob_ShowCluster = new Job_ShowCluster(pSession);
  
  return pJob_ShowCluster;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ShowCluster_post(Interface::Session* pSession, Job* pJob)
{
  //Job_ShowCluster* pJob_ShowCluster = _pJob_ShowCluster(pJob);
  
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ShowCluster_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ShowCluster* pJob_ShowCluster = _pJob_ShowCluster(pJob);
  
  delete pJob_ShowCluster;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ShowCluster_show(Job* pJob)
{
  Job_ShowCluster* pJob_ShowCluster = _pJob_ShowCluster(pJob);

  pJob_ShowCluster->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
