/*============================================================================*/
/* Job_G35.cpp -- Grid R35 Test Job                                           */
/*============================================================================*/

#include "JobModel.h"

JobNote mJobNote_G35 (
  "g35"_Stz,       // name of the job class
  CN_JOB_G35,      // class enum
  JK_DIST2PC,      // job kind
  "comment"_Stz,   // comment about the job
  "g35"_Stz,       // the prompt command
  nullptr,         // the second word
  "g35_req"_Stz,   // the task seed for this job
  nullptr,         // the second task seed for this job
  false,           // check resources?
  false,           // detached job?
  false,           // buffer attached?
  JT_NONE,         // job trace
  pJob_G35_pre,    // prompt pre-process function
  vJob_G35_post,   // prompt post-process function
  nullptr,         // job new function
  vJob_G35_delete, // job delete function
  nullptr,         // job pre-send function
  nullptr,         // job post-receive function
  vJob_G35_show,   // job show function
  false,           // job is not enabled
  "<help>"_Stz,    // help
  "<example>"_Stz  // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_G35* JobsModel::_pJob_G35(void* aAddress)
{
  Job_G35* pJob_G35 = reinterpret_cast<Job_G35*>(aAddress);
  
  RASSERT( !pJob_G35 || CN_JOB_G35 == pJob_G35->eClass());
  
  return pJob_G35;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_G35::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_G35* pJob_G35 =
    static_cast<Job_G35*>(Heap::pAllocate(CN_JOB_G35));
  
  return pJob_G35;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_G35::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_G35::operator delete(void* aAddress)
{
  Job_G35* pJob_G35 = static_cast<Job_G35*>(aAddress);
  
  Heap::vFree(pJob_G35);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_G35::Job_G35(Interface::SEitf* pSEitf)
: Job(CN_JOB_G35, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_G35,_FOOTER_);
    
  //m_sExecute = "Job_G35.execute"_Stz;
  
  _checkInitialised_(CN_JOB_G35);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_G35,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_G35::~Job_G35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_G35,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_G35::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_G35 == eClass);
  RASSERT(pObject->eClass() == CN_JOB_G35);
  
  Job_G35* pJob_G35 = _pJob_G35(pObject);

  pJob_G35->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_G35::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_G35,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_G35::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_G35);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_G35_pre(Interface::SEitf* pSEitf)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_G35_post(Interface::SEitf* pSEitf, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_G35_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_G35* pJob_G35 = _pJob_G35(pJob);
  
  delete pJob_G35;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_G35_show(Job* pJob)
{
  Job_G35* pJob_G35 = _pJob_G35(pJob);

  pJob_G35->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
