/*============================================================================*/
/* Job_DropTenant.cpp -- Drop Tenant Job                                      */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_DropTenant =
{
  CN_JOB_DROPTENANT,      // class number
  JK_DIST2PC,             // job kind
  RS_NODE_ZERO,           // response kind
  "drop tenant"_Stz,      // comment about the job
  "drop"_Stz,             // the prompt command
  "tenant"_Stz,           // the second word
  nullptr,                // the _run task seed
  "dropTenant_req"_Stz,   // the _req task seed
  "dropTenant_beg"_Stz,   // the _rcv task seed
  "dropTenant_exe"_Stz,   // the _exe task seed
  "dropTenant_end"_Stz,   // the _end task seed
  "dropTenant_can"_Stz,   // the _can task seed
  true,                   // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  DL_GRAB_WRITE,          // ddl lock model
  PY_NORMAL,              // priority
  JF_NONE,                // flags

  eJob_checkCluster,      // prompt check function
  pJob_DropTenant_pre,    // prompt pre-process function
  vJob_post,              // prompt post-process function
  nullptr,                // job new function
  vJob_DropTenant_delete, // job delete function
  vJob_send,              // job pre-send function
  vJob_recv,              // job post-receive function
  vJob_DropTenant_show,   // job show function
  
  // help and example
  true,
  "drop tenant <tenantNm>"_Stz,
  "drop tenant boray;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_DropTenant_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Export_class;
  
  return &xJobClass_DropTenant;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_DropTenant* Jobs::_pJob_DropTenant(void* aAddress)
{
  Job_DropTenant* pJob_DropTenant = reinterpret_cast<Job_DropTenant*>(aAddress);
  
  RASSERT_TYPE(pJob_DropTenant,CN_JOB_DROPTENANT);
  
  return pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_DropTenant::operator new(size_t)
{
  Job_DropTenant* pJob_DropTenant =
    static_cast<Job_DropTenant*>(HeapMem::pAllocate(CN_JOB_DROPTENANT));
  
  return pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_DropTenant::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::operator delete(void* aAddress)
{
  Job_DropTenant* pJob_DropTenant = static_cast<Job_DropTenant*>(aAddress);
  
  HeapMem::vFree(pJob_DropTenant);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_DropTenant::Job_DropTenant(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
    
  _checkInitialised_(CN_JOB_DROPTENANT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_DropTenant::~Job_DropTenant(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_DropTenant* pJob_DropTenant = _pJob_DropTenant(pObject);

  pJob_DropTenant->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_DropTenant::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("drop"_Stz));
  ASSERT(pTokenVec->pGet(1)->bSame("tenant"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get number of items in token
  iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3)
  return false;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // set the tenant name
  //vTenantName(pTokenVec->pGet(2)->szText());
  m_oEnameSet.vAdd("<tenantNm>"_Stz, pTokenVec->pGet(2)->szText());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_DropTenant::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_DropTenant::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DROPTENANT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_DropTenant_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_DropTenant* pJob_DropTenant = new Job_DropTenant(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_DropTenant->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_DropTenant->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropTenant_delete(Job* pJob)
{
  Job_DropTenant* pJob_DropTenant = _pJob_DropTenant(pJob);
  
  delete pJob_DropTenant;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_DropTenant_show(Job* pJob)
{
  Job_DropTenant* pJob_DropTenant = _pJob_DropTenant(pJob);

  pJob_DropTenant->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
