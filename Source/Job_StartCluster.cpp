/*============================================================================*/
/* Job_StartCluster.cpp -- Cluster Start Job                                  */
/*============================================================================*/

// Job Short: StartCluster

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_StartCluster =
{
  "Job_StartCluster"_Stz,  // name of the job class
  CN_JOB_STARTCLUSTER,     // class enum
  JK_DIST2PC,              // job kind
  "start cluster"_Stz,     // comment about the job
  "startCluster"_Stz,      // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  "startCluster_req"_Stz,  // the _req task seed
  "startCluster_beg"_Stz,  // the _rcv task seed
  "startCluster_exe"_Stz,  // the _exe task seed
  "startCluster_end"_Stz,  // the _end task seed
  "startCluster_can"_Stz,  // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  pJob_StartCluster_pre,   // prompt pre-process function
  vJob_StartCluster_post,  // prompt post-process function
  nullptr,                 // job new function
  vJob_StartCluster_delete,// job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  vJob_StartCluster_show,  // job status function
  true,                    // job is enabled
  "<help>"_Stz,            // help
  "<example>"_Stz,         // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_StartCluster_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Status_class;
  
  return &xJobClass_StartCluster;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_StartCluster* JobsModel::_pJob_StartCluster(void* aAddress)
{
  Job_StartCluster* pJob_StartCluster = reinterpret_cast<Job_StartCluster*>(aAddress);
  
  //RASSERT( !pJob_StartCluster || CN_JOB_STARTCLUSTER == pJob_StartCluster->eClass());
  
  return pJob_StartCluster;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_StartCluster::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_StartCluster* pJob_StartCluster =
  static_cast<Job_StartCluster*>(Heap::pAllocate(CN_JOB_STARTCLUSTER));
  
  return pJob_StartCluster;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_StartCluster::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_StartCluster::operator delete(void* aAddress)
{
  Job_StartCluster* pJob_StartCluster = static_cast<Job_StartCluster*>(aAddress);
  
  Heap::vFree(pJob_StartCluster);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_StartCluster::Job_StartCluster(Interface::SEitf* pSEitf)
: Job(CN_JOB_STARTCLUSTER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTCLUSTER,_FOOTER_);
  
  // default always spawn nodes
  m_bSpawnNodes = true;
  
  _checkInitialised_(CN_JOB_STARTCLUSTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_StartCluster::~Job_StartCluster(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTCLUSTER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_StartCluster::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_DOCOL == eClass);
  RASSERT(pObject->eClass() == CN_JOB_STARTCLUSTER);
  
  Job_StartCluster* pJob_StartCluster = _pJob_StartCluster(pObject);
  
  pJob_StartCluster->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_StartCluster::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTCLUSTER,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("startCluster"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 or 2 tokens
  if (iNumTokens != 1 && iNumTokens != 2)
    return false;
  
  // 4th token should be '.'
  if (iNumTokens == 2) {
    if (! pTokenVec->pGet(1)->bSame("true"_Stz) && ! pTokenVec->pGet(1)->bSame("false"_Stz))
      return false;
    
    if (pTokenVec->pGet(1)->bSame("false"_Stz))
      m_bSpawnNodes = false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_StartCluster::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTCLUSTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_StartCluster::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STARTCLUSTER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_StartCluster_pre(Interface::SEitf* pSEitf)
{
  Job_StartCluster* pJob_StartCluster = new Job_StartCluster(pSEitf);
  
  TokenVec* pTokenVec = pSEitf->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_StartCluster->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_StartCluster->m_bHasError = true;
    pJob_StartCluster->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_StartCluster;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_StartCluster_post(Interface::SEitf* pSEitf, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_StartCluster_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_StartCluster* pJob_StartCluster = _pJob_StartCluster(pJob);
  
  delete pJob_StartCluster;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_StartCluster_show(Job* pJob)
{
  Job_StartCluster* pJob_StartCluster = _pJob_StartCluster(pJob);
  
  pJob_StartCluster->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
