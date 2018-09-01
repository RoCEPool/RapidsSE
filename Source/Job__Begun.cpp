/*============================================================================*/
/* Job__Begun.cpp -- Job Begun                                                */
/*============================================================================*/

// Job Short: _Begun

#include "JobModel.h"

JobClass xJobClass__Begun =
{
  "Job__Begun"_Stz,        // name of the job class
  CN_JOB__BEGUN,           // class enum
  JK_GENERIC,              // job kind
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
  JT_NONE,                 // job trace
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
  
  { 0, 0, 0, 0 ,0, 0 }     // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob__Begun_class(pFUN** pNextFun)
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
Job__Begun* JobsModel::_pJob__Begun(void* aAddress)
{
  Job__Begun* pJob__Begun = reinterpret_cast<Job__Begun*>(aAddress);
  
  RASSERT( !pJob__Begun || CN_JOB__BEGUN == pJob__Begun->eClass());
  
  return pJob__Begun;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Begun::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job__Begun* pJob__Begun =
    static_cast<Job__Begun*>(Heap::pAllocate(CN_JOB__BEGUN));
  
  return pJob__Begun;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Begun::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Begun::operator delete(void* aAddress)
{
  Job__Begun* pJob__Begun = static_cast<Job__Begun*>(aAddress);
  
  Heap::vFree(pJob__Begun);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Begun::Job__Begun(void)
: Job(CN_JOB__BEGUN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__BEGUN,_FOOTER_);
  
  m_eErrorCode = EC_NONE;
  
  _checkInitialised_(CN_JOB__BEGUN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__BEGUN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Begun::~Job__Begun(void)
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
  RASSERT(CN_JOB__BEGUN == eClass);
  RASSERT(pObject->eClass() == CN_JOB__BEGUN);
  
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
void JobsModel::vJob__Begun_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job__Begun* pJob__Begun = _pJob__Begun(pJob);
  
  delete pJob__Begun;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
