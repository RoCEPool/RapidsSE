/*============================================================================*/
/* Job__Ended.cpp -- Job Ended                                                */
/*============================================================================*/

// Job Short: _Ended

#include "Jobs.h"

JobClass xJobClass__Ended =
{
  "Job__Ended"_Stz,        // name of the job class
  CN_JOB__ENDED,           // class enum
  JK_GENERIC,              // job kind
  "job was ended"_Stz,     // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _run task seed
  nullptr,                 // the _req task seed
  "Job__Ended_rcv"_Stz,    // the _rcv task seed
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
  vJob__Ended_delete,      // job delete function
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
JobClass* Jobs::pJob__Ended_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob__Execute_class;
  
  return &xJobClass__Ended;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__Ended* Jobs::_pJob__Ended(void* aAddress)
{
  Job__Ended* pJob__Ended = reinterpret_cast<Job__Ended*>(aAddress);
  
  RASSERT_TYPE(pJob__Ended,CN_JOB__ENDED);
  
  return pJob__Ended;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Ended::operator new(size_t)
{
  Job__Ended* pJob__Ended =
    static_cast<Job__Ended*>(HeapMem::pAllocate(CN_JOB__ENDED));
  
  return pJob__Ended;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Ended::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Ended::operator delete(void* aAddress)
{
  Job__Ended* pJob__Ended = static_cast<Job__Ended*>(aAddress);
  
  HeapMem::vFree(pJob__Ended);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Ended::Job__Ended(void)
: Job(CN_JOB__ENDED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__ENDED,_FOOTER_);
  
  _checkInitialised_(CN_JOB__ENDED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__ENDED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Ended::~Job__Ended(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__ENDED,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Ended::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job__Ended* pJob__Ended = _pJob__Ended(pObject);

  pJob__Ended->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Ended::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__ENDED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__Ended::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__ENDED);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob__Ended_delete(Interface::Session* pSession, Job* pJob)
{
  Job__Ended* pJob__Ended = _pJob__Ended(pJob);
  
  delete pJob__Ended;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
