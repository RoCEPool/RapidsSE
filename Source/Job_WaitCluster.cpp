/*============================================================================*/
/* Job_WaitCluster.cpp -- Wait for cluster to start                           */
/*============================================================================*/

// Job Short: WaitCluster

#include "StorageEngine.h"
#include "Jobs.h"

extern AttachMap*      gpAttachMap;

JobClass xJobClass_WaitCluster =
{
  CN_JOB_WAITCLUSTER,     // class enum
  JK_DIST2PC,             // job kind
  RK_NONE,                // response kind
  "wait cluster"_Stz,     // comment about the job
  "waitCluster"_Stz,      // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "waitCluster_req"_Stz,  // the _req task seed
  "waitCluster_beg"_Stz,  // the _rcv task seed
  "waitCluster_exe"_Stz,  // the _exe task seed
  "waitCluster_end"_Stz,  // the _end task seed
  "waitCluster_can"_Stz,  // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  DL_NONE,                // ddl lock model
  PY_NORMAL,              // priority
  JF_NONE,                // flags

  eJob_checkCluster,      // prompt check function
  pJob_WaitCluster_pre,   // prompt pre-process function
  vJob_WaitCluster_post,  // prompt post-process function
  nullptr,                // job new function
  vJob_WaitCluster_delete,// job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_WaitCluster_show,  // job show function
  
  // help and example
  true,
  "waitCluster"_Stz,
  "waitCluster;"_Stz,
  
  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_WaitCluster_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Walk_class;
  
  return &xJobClass_WaitCluster;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_WaitCluster* Jobs::_pJob_WaitCluster(void* aAddress)
{
  Job_WaitCluster* pJob_WaitCluster = reinterpret_cast<Job_WaitCluster*>(aAddress);
  
  RASSERT_TYPE(pJob_WaitCluster,CN_JOB_WAITCLUSTER);
  
  return pJob_WaitCluster;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_WaitCluster::operator new(size_t)
{
  Job_WaitCluster* pJob_WaitCluster =
    static_cast<Job_WaitCluster*>(HeapMem::pAllocate(CN_JOB_WAITCLUSTER));
  
  return pJob_WaitCluster;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_WaitCluster::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_WaitCluster::operator delete(void* aAddress)
{
  Job_WaitCluster* pJob_Status = static_cast<Job_WaitCluster*>(aAddress);
  
  HeapMem::vFree(pJob_Status);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_WaitCluster::Job_WaitCluster(Interface::Session* pSession)
: Job(CN_JOB_WAITCLUSTER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAITCLUSTER,_FOOTER_);
    
  _checkInitialised_(CN_JOB_WAITCLUSTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAITCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_WaitCluster::~Job_WaitCluster(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAITCLUSTER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_WaitCluster::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_WaitCluster* pJob_WaitCluster = _pJob_WaitCluster(pObject);

  pJob_WaitCluster->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_WaitCluster::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAITCLUSTER,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("waitCluster"_Stz));
  
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
void Job_WaitCluster::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAITCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_WaitCluster::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAITCLUSTER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_WaitCluster_pre(Interface::Session* pSession)
{
  Job_WaitCluster* pJob_WaitCluster = new Job_WaitCluster(pSession);
  
  return pJob_WaitCluster;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_WaitCluster_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_WaitCluster_delete(Interface::Session* pSession, Job* pJob)
{
  Job_WaitCluster* pJob_WaitCluster = _pJob_WaitCluster(pJob);
  
  delete pJob_WaitCluster;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_WaitCluster_show(Job* pJob)
{
  Job_WaitCluster* pJob_WaitCluster = _pJob_WaitCluster(pJob);

  pJob_WaitCluster->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
