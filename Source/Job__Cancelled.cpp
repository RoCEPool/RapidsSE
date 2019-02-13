/*============================================================================*/
/* Job__Cancelled.cpp -- Job Cancelled                                        */
/*============================================================================*/

// Job Short: _Cancelled

#include "Jobs.h"

JobClass xJobClass__Cancelled =
{
  "Job__Cancelled"_Stz,    // name of the job class
  CN_JOB__CANCELLED,       // class enum
  JK_GENERIC,              // job kind
  "job was cancelled"_Stz, // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  nullptr,                 // the _req task seed
  "Job__Cancelled_rcv"_Stz,// the _rcv task seed
  nullptr,                 // the _exe task seed
  nullptr,                 // the _end task seed
  nullptr,                 // the _can task seed
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  
  nullptr,                 // prompt check function
  nullptr,                 // prompt pre-process function
  nullptr,                 // prompt post-process function
  nullptr,                 // job new function
  vJob__Cancelled_delete,  // job delete function
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
JobClass* Jobs::pJob__Cancelled_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob__End_class;
  
  return &xJobClass__Cancelled;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__Cancelled* Jobs::_pJob__Cancelled(void* aAddress)
{
  Job__Cancelled* pJob__Cancelled = reinterpret_cast<Job__Cancelled*>(aAddress);
  
  RASSERT_TYPE(pJob__Cancelled,CN_JOB__CANCELLED);
  
  return pJob__Cancelled;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Cancelled::operator new(size_t)
{
  Job__Cancelled* pJob__Cancelled =
    static_cast<Job__Cancelled*>(HeapMem::pAllocate(CN_JOB__CANCELLED));
  
  return pJob__Cancelled;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Cancelled::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Cancelled::operator delete(void* aAddress)
{
  Job__Cancelled* pJob__Executed = static_cast<Job__Cancelled*>(aAddress);
  
  HeapMem::vFree(pJob__Executed);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Cancelled::Job__Cancelled(Job* pDistJob)
: Job(CN_JOB__CANCELLED,nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED,_FOOTER_);
  
  m_iParentId = pDistJob->m_iJobId;
  m_i_ActionRoot = pDistJob->m_i_ActionRoot;

  m_eErrorCode = EC_NONE;
  
  _checkInitialised_(CN_JOB__CANCELLED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Cancelled::~Job__Cancelled(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Cancelled::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job__Cancelled* pJob__Cancelled = _pJob__Cancelled(pObject);

  pJob__Cancelled->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Cancelled::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__Cancelled::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__CANCELLED);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob__Cancelled_delete(Interface::Session* pSession, Job* pJob)
{
  Job__Cancelled* pJob__Cancelled = _pJob__Cancelled(pJob);
  
  delete pJob__Cancelled;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
