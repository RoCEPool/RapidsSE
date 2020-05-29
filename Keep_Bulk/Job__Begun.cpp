/*============================================================================*/
/* Job__Begun.cpp -- Job Begun                                                */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass__Begun =
{
  CN_JOB__BEGUN,           // class number
  JK_GENERIC,              // job kind
  RS_NONE,                 // response kind
  "job was begun"_Stz,     // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  nullptr,                 // the _req task seed
  "Job__Begun_rcv"_Stz,    // the _rcv task seed
  nullptr,                 // the _exe task seed
  nullptr,                 // the _end task seed
  nullptr,                 // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  DL_NONE,                 // ddl lock model
  PY_NORMAL,               // priority
  JF_NONE,                 // flags

  nullptr,                 // prompt check function
  nullptr,                 // prompt pre-process function
  nullptr,                 // prompt post-process function
  nullptr,                 // job new function
  vJob__Begun_delete,      // job delete function
  vJob_send,               // job pre-send function
  vJob_recv,               // job post-receive function
  nullptr,                 // job show function
  false,                   // job is not prompt enabled
  "<help>"_Stz,            // help
  "<example>"_Stz,         // example
  
  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob__Begun_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob__Cancel_class;
  
  return &xJobClass__Begun;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__Begun* Jobs::_pJob__Begun(void* aAddress)
{
  Job__Begun* pJob__Begun = reinterpret_cast<Job__Begun*>(aAddress);
  
  RASSERT_TYPE(pJob__Begun,CN_JOB__BEGUN);
  
  return pJob__Begun;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Begun::operator new(size_t)
{
  Job__Begun* pJob__Begun =
    static_cast<Job__Begun*>(HeapMem::pAllocate(CN_JOB__BEGUN));
  
  return pJob__Begun;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Begun::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Begun::operator delete(void* aAddress)
{
  Job__Begun* pJob__Begun = static_cast<Job__Begun*>(aAddress);
  
  HeapMem::vFree(pJob__Begun);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Begun::Job__Begun(Job* pDistJob)
: Job(JobClass::pFind(CN_JOB__BEGUN),&pDistJob->m_xReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__BEGUN,_FOOTER_);
  
  m_iParentId = pDistJob->m_iFocalId;

  //m_eErrorCode = EC_NONE;
  
  _checkInitialised_(CN_JOB__BEGUN);
  
  ////ActLog::vAdd(this,AN_CONSTRUCT,CN_JOB__BEGUN,_EG_CN);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__BEGUN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Begun::~Job__Begun(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__BEGUN,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Begun::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job__Begun* pJob__Begun = _pJob__Begun(pObject);

  pJob__Begun->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Begun::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__BEGUN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__Begun::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__BEGUN);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob__Begun_delete(Job* pJob)
{
  Job__Begun* pJob__Begun = _pJob__Begun(pJob);
  
  delete pJob__Begun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
