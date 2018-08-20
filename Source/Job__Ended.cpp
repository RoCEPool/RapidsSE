/*============================================================================*/
/* Job__Ended.cpp -- Job Ended                                                */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote__Ended (
  "Job__Ended"_Stz,        // name of the job class
  CN_JOB__ENDED,           // class enum
  JK_ONESHOT,              // job kind
  "job ended"_Stz,         // comment about the job
  nullptr,                 // the prompt command
  nullptr,                 // the second word
  nullptr,                 // the _req task seed 
  "Job__Ended_rcv"_Stz,    // the _rcv task seed 
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
  vJob__Ended_delete,      // job delete function
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
Job__Ended* JobsModel::_pJob__Ended(void* aAddress)
{
  Job__Ended* pJob__Ended = reinterpret_cast<Job__Ended*>(aAddress);
  
  RASSERT( !pJob__Ended || CN_JOB__ENDED == pJob__Ended->eClass());
  
  return pJob__Ended;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job__Ended::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job__Ended* pJob__Ended =
    static_cast<Job__Ended*>(Heap::pAllocate(CN_JOB__ENDED));
  
  return pJob__Ended;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job__Ended::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job__Ended::operator delete(void* aAddress)
{
  Job__Ended* pJob__Ended = static_cast<Job__Ended*>(aAddress);
  
  Heap::vFree(pJob__Ended);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job__Ended::Job__Ended(void)
: Job(CN_JOB__ENDED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB__ENDED,_FOOTER_);
  
  m_sSeed = "Job__Ended_rcv"_Stz;
  
  _checkInitialised_(CN_JOB__ENDED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB__ENDED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job__Ended::~Job__Ended(void)
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
  RASSERT(CN_JOB__ENDED == eClass);
  RASSERT(pObject->eClass() == CN_JOB__ENDED);
  
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
void JobsModel::vJob__Ended_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job__Ended* pJob__Ended = _pJob__Ended(pJob);
  
  delete pJob__Ended;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
