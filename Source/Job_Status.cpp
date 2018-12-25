/*============================================================================*/
/* Job_Status.cpp -- Status Job                                               */
/*============================================================================*/

// Job Short: Status

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Status =
{
  "Job_Status"_Stz,  // name of the job class
  CN_JOB_STATUS,     // class enum
  JK_DIST2PC,        // job kind
  "status"_Stz,      // comment about the job
  "status"_Stz,      // the prompt command
  nullptr,           // the second word
  nullptr,           // the _run task seed
  "status_req"_Stz,  // the _req task seed
  "status_beg"_Stz,  // the _rcv task seed
  "status_exe"_Stz,  // the _exe task seed
  "status_end"_Stz,  // the _end task seed
  "status_can"_Stz,  // the _can task seed
  false,             // check resources?
  false,             // detached job?
  false,             // buffer attached?
  JT_NONE,           // job trace
  pJob_Status_pre,   // prompt pre-process function
  vJob_Status_post,  // prompt post-process function
  nullptr,           // job new function
  vJob_Status_delete,// job delete function
  vJob_send,         // job pre-send function
  vJob_recv,         // job post-receive function
  vJob_Status_show,  // job status function
  
  // help and example
  true,
  "status"_Stz,
  "status;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_Status_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubFetch_class;
  
  return &xJobClass_Status;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Status* JobModel::_pJob_Status(void* aAddress)
{
  Job_Status* pJob_Status = reinterpret_cast<Job_Status*>(aAddress);
  
  RASSERT_TYPE(pJob_Status,CN_JOB_STATUS);
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Status::operator new(size_t)
{
  Job_Status* pJob_Status =
    static_cast<Job_Status*>(HeapMem::pAllocate(CN_JOB_STATUS));
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Status::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Status::operator delete(void* aAddress)
{
  Job_Status* pJob_Status = static_cast<Job_Status*>(aAddress);
  
  HeapMem::vFree(pJob_Status);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Status::Job_Status(Interface::Session* pSession)
: Job(CN_JOB_STATUS, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
    
   _checkInitialised_(CN_JOB_STATUS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Status::~Job_Status(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Status::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Status* pJob_Status = _pJob_Status(pObject);

  pJob_Status->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Status::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("status"_Stz));
  
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
void Job_Status::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Status::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_STATUS);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_Status_pre(Interface::Session* pSession)
{
  Job_Status* pJob_Status = new Job_Status(pSession);
  
  TokenVec* pTokenVec = pSession->m_pCommandProcessor->pTknVec();
  
  bool bResult = pJob_Status->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    i8 iNode = gpAttachMap->m_pGrid->m_iThisNode;
    pJob_Status->m_bHasError = true;
    pJob_Status->m_xNode[iNode].m_eErrorCode = EC_INVALID_COMMAND;
  }
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Status_post(Interface::Session* pSession, Job* pJob)
{
  Job_Status* pJob_Status = _pJob_Status(pJob);
  
  if ( pJob_Status->m_bHasError ) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("error: (node%lld) %s\n"_Stz, i_Node, aEC[pJob_Status->m_xNode[i_Node].m_eErrorCode]);
    }
  }
  else {
    if (gpAttachMap->m_bMetadata) {
      vTrace("metadata: %s\n"_Stz, pJob_Status->m_sMetadata.aStr());
    }
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      vTrace("%s\n"_Stz, pJob_Status->m_xNode[i_Node].m_sResponse.aChar());
    }
    vTrace("success: %lld rows affected\n"_Stz, gpAttachMap->m_pGrid->m_nNodes);
  }
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Status_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Status* pJob_Status = _pJob_Status(pJob);
  
  // now delete the job
  delete pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Status_show(Job* pJob)
{
  Job_Status* pJob_Status = _pJob_Status(pJob);

  pJob_Status->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
