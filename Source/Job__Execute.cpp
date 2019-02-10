/*============================================================================*/
/* Job__Execute.cpp -- Job Execute                                            */
/*============================================================================*/

// Job Short: _Execute

#include "Jobs.h"

JobClass xJobClass__Execute =
{
  "Job__Execute"_Stz,      // name of the job class
  CN_JOB__EXECUTE,         // class enum
  JK_GENERIC,              // job kind
  "job to execute"_Stz,    // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the _run task seed
  nullptr,                 // the second word
  nullptr,                 // the _req task seed
  "Job__Execute_rcv"_Stz,  // the _rcv task seed
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
  vJob__Execute_delete,    // job delete function
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
JobClass* Jobs::pJob__Execute_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob__Executed_class;
  
  return &xJobClass__Execute;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__Execute* Jobs::_pJob__Execute(void* aAddress)
{
  Job__Execute* pJob__Execute = reinterpret_cast<Job__Execute*>(aAddress);
  
  RASSERT_TYPE(pJob__Execute,CN_JOB__EXECUTE);
  
  return pJob__Execute;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Execute::operator new(size_t)
{
  Job__Execute* pJob__Execute =
    static_cast<Job__Execute*>(HeapMem::pAllocate(CN_JOB__EXECUTE));
  
  return pJob__Execute;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Execute::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Execute::operator delete(void* aAddress)
{
  Job__Execute* pJob__Execute = static_cast<Job__Execute*>(aAddress);
  
  HeapMem::vFree(pJob__Execute);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Execute::Job__Execute(void)
: Job(CN_JOB__EXECUTE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTE,_FOOTER_);
  
  _checkInitialised_(CN_JOB__EXECUTE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Execute::~Job__Execute(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTE,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Execute::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job__Execute* pJob__Execute = _pJob__Execute(pObject);

  pJob__Execute->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__Execute::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__Execute::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__EXECUTE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob__Execute_delete(Interface::Session* pSession, Job* pJob)
{
  Job__Execute* pJob__Execute = _pJob__Execute(pJob);
  
  delete pJob__Execute;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
