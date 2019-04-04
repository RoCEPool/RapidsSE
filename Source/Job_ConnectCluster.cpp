/*============================================================================*/
/* Job_ConnectCluster.cpp -- Cluster Connect Job                              */
/*============================================================================*/

// Job Short: StartCluster

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_ConnectCluster =
{
  CN_JOB_CONNECTCLUSTER,     // class enum
  JK_DIST2PC,                // job kind
  RK_NONE,                  // response kind
  "connect cluster"_Stz,     // comment about the job
  "connectCluster"_Stz,      // the prompt command
  nullptr,                   // the second word
  nullptr,                   // the _run task seed
  "connectCluster_req"_Stz,  // the _req task seed
  "connectCluster_beg"_Stz,  // the _rcv task seed
  "connectCluster_exe"_Stz,  // the _exe task seed
  "connectCluster_end"_Stz,  // the _end task seed
  "connectCluster_can"_Stz,  // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_NONE,                   // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  nullptr,                   // prompt check function
  pJob_ConnectCluster_pre,   // prompt pre-process function
  vJob_ConnectCluster_post,  // prompt post-process function
  nullptr,                   // job new function
  vJob_ConnectCluster_delete,// job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_ConnectCluster_show,  // job status function
  
  // help and example
  true,
  "connectCluster"_Stz,
  "connectCluster;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ConnectCluster_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Connector_class;
  
  return &xJobClass_ConnectCluster;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ConnectCluster* Jobs::_pJob_ConnectCluster(void* aAddress)
{
  Job_ConnectCluster* pJob_ConnectCluster = reinterpret_cast<Job_ConnectCluster*>(aAddress);
  
  RASSERT_TYPE(pJob_ConnectCluster,CN_JOB_CONNECTCLUSTER);
  
  return pJob_ConnectCluster;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ConnectCluster::operator new(size_t)
{
  Job_ConnectCluster* pJob_ConnectCluster =
  static_cast<Job_ConnectCluster*>(HeapMem::pAllocate(CN_JOB_CONNECTCLUSTER));
  
  return pJob_ConnectCluster;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ConnectCluster::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ConnectCluster::operator delete(void* aAddress)
{
  Job_ConnectCluster* pJob_ConnectCluster = static_cast<Job_ConnectCluster*>(aAddress);
  
  HeapMem::vFree(pJob_ConnectCluster);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ConnectCluster::Job_ConnectCluster(Interface::Session* pSession)
: Job(CN_JOB_CONNECTCLUSTER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTCLUSTER,_FOOTER_);
  
  _checkInitialised_(CN_JOB_CONNECTCLUSTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ConnectCluster::~Job_ConnectCluster(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTCLUSTER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ConnectCluster::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ConnectCluster* pJob_ConnectCluster = _pJob_ConnectCluster(pObject);
  
  pJob_ConnectCluster->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ConnectCluster::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTCLUSTER,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("connectCluster"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 token
  if (iNumTokens != 1)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ConnectCluster::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ConnectCluster::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CONNECTCLUSTER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ConnectCluster_pre(Interface::Session* pSession)
{
  Job_ConnectCluster* pJob_ConnectCluster = new Job_ConnectCluster(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommand->pTknVec();
  
  bool bResult = pJob_ConnectCluster->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_ConnectCluster->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_ConnectCluster;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ConnectCluster_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ConnectCluster_delete(Interface::Session* pSession, Job* pJob)
{
  Job_ConnectCluster* pJob_ConnectCluster = _pJob_ConnectCluster(pJob);
  
  delete pJob_ConnectCluster;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ConnectCluster_show(Job* pJob)
{
  Job_ConnectCluster* pJob_ConnectCluster = _pJob_ConnectCluster(pJob);
  
  pJob_ConnectCluster->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
