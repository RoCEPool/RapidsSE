/*============================================================================*/
/* Job_ListTables.cpp -- List Tables Job                                      */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_ListTables =
{
  CN_JOB_LISTTABLES,      // class number
  JK_LOCAL,               // job kind
  RS_LOCAL_ROWS,          // response kind
  "list tables"_Stz,      // comment about the job
  "lstbl"_Stz,            // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "listTables_req"_Stz,   // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  DL_GRAB_READ,           // ddl lock model
  PY_NORMAL,              // priority
  JF_NONE,                // flags

  eJob_checkCluster,      // prompt check function
  pJob_ListTables_pre,    // prompt pre-process function
  vJob_post,              // prompt post-process function
  nullptr,                // job new function
  vJob_ListTables_delete, // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_ListTables_show,   // job show function
  
  // help and example
  true,
  "lstbl"_Stz,
  "lstbl;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_ListTables_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_ListTenants_class;
  
  return &xJobClass_ListTables;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_ListTables* Jobs::_pJob_ListTables(void* aAddress)
{
  Job_ListTables* pJob_ListTables = reinterpret_cast<Job_ListTables*>(aAddress);
  
  RASSERT_TYPE(pJob_ListTables,CN_JOB_LISTTABLES);
  
  return pJob_ListTables;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_ListTables::operator new(size_t)
{
  Job_ListTables* pJob_ListTables =
    static_cast<Job_ListTables*>(HeapMem::pAllocate(CN_JOB_LISTTABLES));
  
  return pJob_ListTables;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_ListTables::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_ListTables::operator delete(void* aAddress)
{
  Job_ListTables* pJob_ListTables = static_cast<Job_ListTables*>(aAddress);
  
  HeapMem::vFree(pJob_ListTables);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_ListTables::Job_ListTables(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
  
  _checkInitialised_(CN_JOB_LISTTABLES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_ListTables::~Job_ListTables(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListTables::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_ListTables* pJob_ListTables = _pJob_ListTables(pObject);

  pJob_ListTables->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_ListTables::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("lstbl"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1 tokens
  if (iNumTokens != 1) {
    return nullptr;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_ListTables::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_ListTables::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_LISTTABLES);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_ListTables_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_ListTables* pJob_ListTables = new Job_ListTables(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_ListTables->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_ListTables->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_ListTables;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListTables_delete(Job* pJob)
{
  Job_ListTables* pJob_ListTables = _pJob_ListTables(pJob);
  
  delete pJob_ListTables;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_ListTables_show(Job* pJob)
{
  Job_ListTables* pJob_ListTables = _pJob_ListTables(pJob);

  pJob_ListTables->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
