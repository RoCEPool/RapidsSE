/*============================================================================*/
/* Job__End.cpp -- Job End                                                    */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote__End (
  "Job__End"_Stz,          // name of the job class
  CN_JOB__END,             // class enum
  JK_ONESHOT,              // job kind
  "job to end"_Stz,        // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the task seed for this job
  "Job__End::execute"_Stz,                 // the second task seed for this job
  false,                   // check resources?
  false,                   // detached job?
  false,                   // buffer attached?
  JT_NONE,                 // job trace
  nullptr,                 // prompt pre-process function
  nullptr,                 // prompt post-process function
  nullptr,                 // job new function
  vJob__End_delete,        // job delete function
  nullptr,                 // job pre-send function
  nullptr,                 // job post-receive function
  nullptr,                 // job show function
  false,                   // job is not prompt enabled
  "<help>"_Stz,            // help
  "<example>"_Stz          // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job__End* JobsModel::_pJob__End(void* aAddress)
{
  Job__End* pJob__End = reinterpret_cast<Job__End*>(aAddress);
  
  RASSERT( !pJob__End || CN_JOB__END == pJob__End->eClass());
  
  return pJob__End;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__End::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job__End* pJob__End =
    static_cast<Job__End*>(Heap::pAllocate(CN_JOB__END));
  
  return pJob__End;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__End::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__End::operator delete(void* aAddress)
{
  Job__End* pJob__End = static_cast<Job__End*>(aAddress);
  
  Heap::vFree(pJob__End);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__End::Job__End(void)
: Job(CN_JOB__END)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__END,_FOOTER_);
  
  m_sSeed = "Job__End::execute"_Stz;
  
  _checkInitialised_(CN_JOB__END);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__END,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__End::~Job__End(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__END,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job__End::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB__END == eClass);
  RASSERT(pObject->eClass() == CN_JOB__END);
  
  Job__End* pJob__End = _pJob__End(pObject);

  pJob__End->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job__End::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__END,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job__End::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__END);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob__End_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job__End* pJob__End = _pJob__End(pJob);
  
  delete pJob__End;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
