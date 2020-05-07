/*============================================================================*/
/* Job_SubFetch.cpp -- Sub Fetch Job                                          */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_SubFetch =
{
  CN_JOB_SUBFETCH,           // class number
  JK_CHILD,                  // job kind
  RS_PARTICIPANT_RESPONSE,              // response kind
  "sub-fetch rows"_Stz,      // comment about the job
  nullptr,                   // the prompt command
  nullptr,                   // the second word
  "subFetch_run"_Stz,        // the _run task seed
  nullptr,                   // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  nullptr,                   // prompt check function
  pJob_SubFetch_pre,         // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_SubFetch_delete,      // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubFetch_show,        // job show function
  
  // help and example
  false,
  C_NO_NAME,
  C_NO_NAME,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SubFetch_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubLoader_class;
  
  return &xJobClass_SubFetch;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubFetch* Jobs::_pJob_SubFetch(void* aAddress)
{
  Job_SubFetch* pJob_SubFetch = reinterpret_cast<Job_SubFetch*>(aAddress);
  
  RASSERT_TYPE(pJob_SubFetch,CN_JOB_SUBFETCH);
  
  return pJob_SubFetch;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubFetch::operator new(size_t)
{
  Job_SubFetch* pJob_SubFetch =
    static_cast<Job_SubFetch*>(HeapMem::pAllocate(CN_JOB_SUBFETCH));
  
  return pJob_SubFetch;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubFetch::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubFetch::operator delete(void* aAddress)
{
  Job_SubFetch* pJob_SubFetch = static_cast<Job_SubFetch*>(aAddress);
  
  HeapMem::vFree(pJob_SubFetch);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubFetch::Job_SubFetch(JobClass* aJobClass, ReqTuple* aReqTuple)
: Job(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBFETCH,_FOOTER_);
  
  m_pParentJob     = nullptr;
  m_pBufferQueue   = nullptr;
  m_pPlanner       = nullptr;
  m_iPhrase        = -1;
  
  _checkInitialised_(CN_JOB_SUBFETCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBFETCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubFetch::~Job_SubFetch(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBFETCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubFetch::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pObject);

  pJob_SubFetch->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubFetch::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBFETCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubFetch::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBFETCH);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SubFetch_pre(JobClass* aJobClass, ReqTuple* aReqTuple, TokenVec* pTknVec)
{
  Job_SubFetch* pJob_SubFetch = new Job_SubFetch(aJobClass,aReqTuple);
  //pJob->vDicPath(pSession);

  return pJob_SubFetch;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubFetch_delete(Job* pJob)
{
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pJob);
  
  delete pJob_SubFetch;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubFetch_show(Job* pJob)
{
  Job_SubFetch* pJob_SubFetch = _pJob_SubFetch(pJob);

  pJob_SubFetch->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
