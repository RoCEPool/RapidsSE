/*============================================================================*/
/* Job__Executed.cpp -- Job Executed                                          */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass__Executed =
{
  CN_JOB__EXECUTED,        // class number
  JK_GENERIC,              // job kind
  RS_NONE,                 // response kind
  "job was executed"_Stz,  // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  nullptr,                 // the _req task seed
  "Job__Executed_rcv"_Stz, // the _rcv task seed
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
  vJob__Executed_delete,   // job delete function
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
JobClass* Jobs::pJob__Executed_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Boot_class;
  
  return &xJobClass__Executed;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__Executed* Jobs::_pJob__Executed(void* aAddress)
{
  Job__Executed* pJob__Executed = reinterpret_cast<Job__Executed*>(aAddress);
  
  RASSERT_TYPE(pJob__Executed,CN_JOB__EXECUTED);
  
  return pJob__Executed;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Executed::operator new(size_t)
{
  Job__Executed* pJob__Executed =
    static_cast<Job__Executed*>(HeapMem::pAllocate(CN_JOB__EXECUTED));
  
  return pJob__Executed;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Executed::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Executed::operator delete(void* aAddress)
{
  Job__Executed* pJob__Executed = static_cast<Job__Executed*>(aAddress);
  
  HeapMem::vFree(pJob__Executed);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Executed::Job__Executed(Job* pDistJob)
: Job(JobClass::pFind(CN_JOB__EXECUTED),&pDistJob->m_xReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED,_FOOTER_);
  
  m_iParentId = pDistJob->m_iFocalId;

//  m_sResponse      = ""_Stz;
//  m_iRowsProcessed = 0;
//  m_iRowsInvalid   = 0;
//  m_iRowsSkipped   = 0;
//  m_eErrorCode     = EC_NONE;
  
  _checkInitialised_(CN_JOB__EXECUTED);
  
  //ActLog::vAdd(this,AN_CONSTRUCT,CN_JOB__EXECUTED,_EG_CN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Executed::~Job__Executed(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Executed::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job__Executed* pJob__Executed = _pJob__Executed(pObject);

  pJob__Executed->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Executed::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__Executed::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTED);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob__Executed_delete(Job* pJob)
{
  Job__Executed* pJob__Executed = _pJob__Executed(pJob);
  
  delete pJob__Executed;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
