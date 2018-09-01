/*============================================================================*/
/* Job_ShowCluster.cpp -- Show Cluster Job                                    */
/*============================================================================*/

// Job Short: ShowCluster

#include "JobModel.h"
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
  pJob_ShowCluster_pre,  // prompt pre-process function
  vJob_ShowCluster_post, // prompt post-process function
  nullptr,               // job new function
  vJob_ShowCluster_delete,  // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_ShowCluster_show, // job show function
  true,                  // job is enabled
  "<help>"_Stz,          // help
  "<example>"_Stz,       // example

  { 0, 0, 0, 0 ,0, 0 }    // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_ShowCluster_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SizeIndex_class;
  
  return &xJobClass_ShowCluster;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ShowCluster* JobsModel::_pJob_ShowCluster(void* aAddress)
{
  Job_ShowCluster* pJob_ShowCluster = reinterpret_cast<Job_ShowCluster*>(aAddress);
  
  //RASSERT( !pJob_ShowCluster || CN_JOB_SHOWCLUSTER == pJob_ShowCluster->eClass());
  
  return pJob_ShowCluster;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ShowCluster::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_ShowCluster* pJob_ShowCluster =
    static_cast<Job_ShowCluster*>(Heap::pAllocate(CN_JOB_SHOWCLUSTER));
  
  return pJob_ShowCluster;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ShowCluster::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ShowCluster::operator delete(void* aAddress)
{
  Job_ShowCluster* pJob_ShowCluster = static_cast<Job_ShowCluster*>(aAddress);
  
  Heap::vFree(pJob_ShowCluster);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ShowCluster::Job_ShowCluster(Interface::SEitf* pSEitf)
: Job(CN_JOB_SHOWCLUSTER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SHOWCLUSTER,_FOOTER_);
  
  _checkInitialised_(CN_JOB_SHOWCLUSTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SHOWCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ShowCluster::~Job_ShowCluster(void)
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
  RASSERT(CN_JOB_DOCOL == eClass);
  RASSERT(pObject->eClass() == CN_JOB_SHOWCLUSTER);
  
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
Job* JobsModel::pJob_ShowCluster_pre(Interface::SEitf* pSEitf)
{
  Job_ShowCluster* pJob_ShowCluster = new Job_ShowCluster(pSEitf);
  
  return pJob_ShowCluster;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ShowCluster_post(Interface::SEitf* pSEitf, Job* pJob)
{
  //Job_ShowCluster* pJob_ShowCluster = _pJob_ShowCluster(pJob);
  
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ShowCluster_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_ShowCluster* pJob_ShowCluster = _pJob_ShowCluster(pJob);
  
  delete pJob_ShowCluster;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_ShowCluster_show(Job* pJob)
{
  Job_ShowCluster* pJob_ShowCluster = _pJob_ShowCluster(pJob);

  pJob_ShowCluster->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
