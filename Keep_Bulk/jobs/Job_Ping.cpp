/*============================================================================*/
/* Job_Ping.cpp -- Ping Job                                                   */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Ping =
{
  CN_JOB_PING,       // class number
  JK_LOCAL,          // job kind
  RS_LOCAL_ROWS,     // response kind
  "ping"_Stz,        // comment about the job
  "ping"_Stz,        // the prompt command
  nullptr,           // the second word
  nullptr,           // the _run task seed
  "ping_req"_Stz,    // the _req task seed
  nullptr,           // the _exe task seed
  nullptr,           // the _rcv task seed
  nullptr,           // the _end task seed
  nullptr,           // the _can task seed
  false,             // check resources?
  false,             // detached job?
  false,             // buffer attached?
  DL_NONE,           // ddl lock model
  PY_NORMAL,         // priority
  JF_NONE,           // flags
  
  nullptr,           // prompt check function
  pJob_Ping_pre,     // prompt pre-process function
  vJob_post,    // prompt post-process function
  nullptr,           // job new function
  vJob_Ping_delete,  // job delete function
  nullptr,           // job pre-send function
  nullptr,           // job post-receive function
  vJob_Ping_show,    // job show function
  
  // help and example
  true,
  "ping"_Stz,
  "ping;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Ping_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Pipeline_class;
  
  return &xJobClass_Ping;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Ping* Jobs::_pJob_Ping(void* aAddress)
{
  Job_Ping* pJob_Ping = reinterpret_cast<Job_Ping*>(aAddress);
  
  RASSERT_TYPE(pJob_Ping,CN_JOB_PING);
  
  return pJob_Ping;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Ping::operator new(size_t)
{
  Job_Ping* pJob_Ping =
    static_cast<Job_Ping*>(HeapMem::pAllocate(CN_JOB_PING));
  
  return pJob_Ping;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Ping::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Ping::operator delete(void* aAddress)
{
  Job_Ping* pJob_Lf = static_cast<Job_Ping*>(aAddress);
  
  HeapMem::vFree(pJob_Lf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Ping::Job_Ping(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
  
  _checkInitialised_(CN_JOB_PING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Ping::~Job_Ping(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Ping::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Ping* pJob_Ping = _pJob_Ping(pObject);

  pJob_Ping->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Ping::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("ping"_Stz));
  
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
void Job_Ping::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Ping::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PING);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Ping_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Ping* pJob_Ping = new Job_Ping(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_Ping->bPopulateJob(pTokenVec);
  
  if ( !bResult) {
    pJob_Ping->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Ping;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Ping_delete(Job* pJob)
{
  Job_Ping* pJob_Ping = _pJob_Ping(pJob);
  
  delete pJob_Ping;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Ping_show(Job* pJob)
{
  Job_Ping* pJob_Ping= _pJob_Ping(pJob);

  pJob_Ping->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
