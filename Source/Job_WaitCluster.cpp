/*============================================================================*/
/* Job_WaitCluster.cpp -- Wait for cluster to start                           */
/*============================================================================*/
#include "StorageEngine.h"
#include "JobModel.h"

extern AttachMap*      gpAttachMap;

JobNote mJobNote_WaitCluster (
  "Job_WaitCluster"_Stz,  // name of the job class
  CN_JOB_WAITCLUSTER,     // class enum
  JK_DIST2PC,             // job kind
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
  pJob_WaitCluster_pre,   // prompt pre-process function
  vJob_WaitCluster_post,  // prompt post-process function
  nullptr,                // job new function
  vJob_WaitCluster_delete,// job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_WaitCluster_show,  // job show function
  true,                   // job is enabled
  "<help>"_Stz,           // help
  "<example>"_Stz         // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_WaitCluster* JobsModel::_pJob_WaitCluster(void* aAddress)
{
  Job_WaitCluster* pJob_WaitCluster = reinterpret_cast<Job_WaitCluster*>(aAddress);
  
  RASSERT( !pJob_WaitCluster || CN_JOB_WAITCLUSTER == pJob_WaitCluster->eClass());
  
  return pJob_WaitCluster;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_WaitCluster::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_WaitCluster* pJob_WaitCluster =
    static_cast<Job_WaitCluster*>(Heap::pAllocate(CN_JOB_WAITCLUSTER));
  
  return pJob_WaitCluster;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_WaitCluster::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_WaitCluster::operator delete(void* aAddress)
{
  Job_WaitCluster* pJob_Status = static_cast<Job_WaitCluster*>(aAddress);
  
  Heap::vFree(pJob_Status);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_WaitCluster::Job_WaitCluster(Interface::SEitf* pSEitf)
: Job(CN_JOB_WAITCLUSTER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAITCLUSTER,_FOOTER_);
    
  m_sSeed    = mJobNote_WaitCluster.pJobClass()->m_aSeedReq;
  
  _checkInitialised_(CN_JOB_WAITCLUSTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_WAITCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_WaitCluster::~Job_WaitCluster(void)
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
  RASSERT(CN_JOB_WAITCLUSTER == eClass);
  RASSERT(pObject->eClass() == CN_JOB_WAITCLUSTER);
  
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
Job* JobsModel::pJob_WaitCluster_pre(Interface::SEitf* pSEitf)
{
  Job_WaitCluster* pJob_WaitCluster = new Job_WaitCluster(pSEitf);
  
  return pJob_WaitCluster;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_WaitCluster_post(Interface::SEitf* pSEitf, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_WaitCluster_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_WaitCluster* pJob_WaitCluster = _pJob_WaitCluster(pJob);
  
  delete pJob_WaitCluster;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_WaitCluster_show(Job* pJob)
{
  Job_WaitCluster* pJob_WaitCluster = _pJob_WaitCluster(pJob);

  pJob_WaitCluster->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
