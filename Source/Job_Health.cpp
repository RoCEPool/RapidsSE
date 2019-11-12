/*============================================================================*/
/* Job_Health.cpp -- Health Job                                               */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Health =
{
  CN_JOB_HEALTH,         // class number
  JK_OTHER,              // job kind
  RS_NONE,               // response kind
  "network hog"_Stz,     // comment about the job
  "health"_Stz,          // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "netHog_req"_Stz,      // the _req task seed
  "netHog_rcv"_Stz,      // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_NONE,               // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_Health_pre,       // prompt pre-process function
  vJob_Health_post,      // prompt post-process function
  pJob_Health_new,       // job new function
  vJob_Health_delete,    // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_Health_show,      // job show function
  
  // help and example
  true,
  "nethog <nHogs>"_Stz,
  "nethog 73;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};


/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Health_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_InsertRow_class;
  
  return &xJobClass_Health;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Health* Jobs::_pJob_Health(void* aAddress)
{
  Job_Health* pJob_Health = reinterpret_cast<Job_Health*>(aAddress);
  
  RASSERT_TYPE(pJob_Health,CN_JOB_HEALTH);
  
  return pJob_Health;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Health::operator new(size_t)
{
  Job_Health* pJob_Health =
    static_cast<Job_Health*>(HeapMem::pAllocate(CN_JOB_HEALTH));
  
  return pJob_Health;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Health::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Health::operator delete(void* aAddress)
{
  Job_Health* pJob_Health = static_cast<Job_Health*>(aAddress);
  
  HeapMem::vFree(pJob_Health);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Health::Job_Health(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_HEALTH,_FOOTER_);
  
  m_iHogNo = -1;
  
  _checkInitialised_(CN_JOB_HEALTH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_HEALTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Health::~Job_Health(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_HEALTH,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Health::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Health* pJob_Health = _pJob_Health(pObject);

  pJob_Health->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Health::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_HEALTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Health::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_HEALTH);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Health_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession)
{
  Job_Health* pJob_Health = new Job_Health(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  TokenVec* pTknVec = pSession->m_pInput->pTknVec();
  
  Token* pHogCount = pTknVec->pGet(1);
  i8 nHogs = pHogCount->iToI8();
  
  vTrace("Starting %lld hogs\n"_Stz,nHogs);
  
  for (i8 i_Hog = 0; i_Hog < nHogs; i_Hog++) {
    
    JobClass* pJobClass_Health = JobClass::pFind(CN_JOB_HEALTH);
    Job_Health* pJob_Health = _pJob_Health(pJobClass_Health->pNewJob(nullptr));

    pJob_Health->m_iHogNo  = i_Hog;
    pJob_Health->m_bNoWait = true;
    
    pJob_Health->vSeed(SS_SEED_RCV);
    
    bool bResult = AM::bSpawnJob(pJob_Health);
    RASSERT(bResult);
  }
  
  return pJob_Health;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Health_post(Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Health_new(JobClass* aJobClass, ReqTuple* aReqTuple)
{
  Job_Health* pJob_Health = new Job_Health(aJobClass,nullptr);
  
  return pJob_Health;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Health_delete(Job* pJob)
{
  Job_Health* pJob_Health = _pJob_Health(pJob);
  
  delete pJob_Health;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Health_show(Job* pJob)
{
  Job_Health* pJob_Health = _pJob_Health(pJob);

  pJob_Health->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
