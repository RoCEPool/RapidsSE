/*============================================================================*/
/* Job_Status.cpp -- Status Job                                               */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Status =
{
  CN_JOB_STATUS,     // class number
  JK_DIST2PC,        // job kind
  RS_NODE_ROWS,      // response kind
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
  DL_NONE,           // ddl lock model
  PY_NORMAL,         // priority
  JF_NONE,           // flags

  eJob_checkCluster, // prompt check function
  pJob_Status_pre,   // prompt pre-process function
  vJob_post,         // prompt post-process function
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
JobClass* Jobs::pJob_Status_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubExporter_class;
  
  return &xJobClass_Status;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Status* Jobs::_pJob_Status(void* aAddress)
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
Job_Status::Job_Status(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
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
Job* Jobs::pJob_Status_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Status* pJob_Status = new Job_Status(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_Status->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Status->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Status_delete(Job* pJob)
{
  Job_Status* pJob_Status = _pJob_Status(pJob);
  
  // now delete the job
  delete pJob_Status;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Status_show(Job* pJob)
{
  Job_Status* pJob_Status = _pJob_Status(pJob);

  pJob_Status->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
