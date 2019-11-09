/*============================================================================*/
/* Job_CreateTenant.cpp -- Create Tennant Job                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_CreateTenant =
{
  CN_JOB_CREATETENANT,       // class number
  JK_DIST2PC,                // job kind
  RS_NODE_ZERO,              // response kind
  "create tenant"_Stz,       // comment about the job
  "create"_Stz,              // the prompt command
  "tenant"_Stz,              // the second word
  nullptr,                   // the _run task seed
  "createTenant_req"_Stz,    // the _req task seed
  "createTenant_beg"_Stz,    // the _rcv task seed
  "createTenant_exe"_Stz,    // the _exe task seed
  "createTenant_end"_Stz,    // the _end task seed
  "createTenant_can"_Stz,    // the _can task seed
  true,                      // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  DL_GRAB_WRITE,             // ddl lock model
  PY_NORMAL,                 // priority
  JF_LOWMEM,                 // flags

  eJob_checkCluster,         // prompt check function
  pJob_CreateTenant_pre,     // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_CreateTenant_delete,  // job delete function
  vJob_send,                 // job pre-send function
  vJob_recv,                 // job post-receive function
  vJob_CreateTenant_show,    // job show function
  
  // help and example
  true,
  "create tenant <tenantNm>"_Stz,
  "create tenant boray;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_CreateTenant_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Export_class;
  
  return &xJobClass_CreateTenant;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_CreateTenant* Jobs::_pJob_CreateTenant(void* aAddress)
{
  Job_CreateTenant* pJob_CreateTenant = reinterpret_cast<Job_CreateTenant*>(aAddress);
  
  RASSERT_TYPE(pJob_CreateTenant,CN_JOB_CREATETENANT);
  
  return pJob_CreateTenant;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_CreateTenant::operator new(size_t)
{
  Job_CreateTenant* pJob_CreateTenant =
    static_cast<Job_CreateTenant*>(HeapMem::pAllocate(CN_JOB_CREATETENANT));
  
  return pJob_CreateTenant;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_CreateTenant::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_CreateTenant::operator delete(void* aAddress)
{
  Job_CreateTenant* pJob_CreateTenant = static_cast<Job_CreateTenant*>(aAddress);
  
  HeapMem::vFree(pJob_CreateTenant);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_CreateTenant::Job_CreateTenant(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
    
  _checkInitialised_(CN_JOB_CREATETENANT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_CreateTenant::~Job_CreateTenant(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTenant::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_CreateTenant* pJob_CreateTenant = _pJob_CreateTenant(pObject);

  pJob_CreateTenant->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_CreateTenant::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("create"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("tenant"_Stz));
  
  // get number of items in token vector
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
  return false;
  
  // set the tenant name
  m_oEnameSet.vAdd("<tenantNm>"_Stz, pTokenVec->pGet(2)->szText());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_CreateTenant::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_CreateTenant::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_CREATETENANT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_CreateTenant_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_CreateTenant* pJob_CreateTenant = new Job_CreateTenant(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_CreateTenant->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_CreateTenant->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_CreateTenant;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateTenant_delete(Job* pJob)
{
  Job_CreateTenant* pJob_CreateTenant = _pJob_CreateTenant(pJob);
  
  delete pJob_CreateTenant;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_CreateTenant_show(Job* pJob)
{
  Job_CreateTenant* pJob_CreateTenant = _pJob_CreateTenant(pJob);

  pJob_CreateTenant->vShow();
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
