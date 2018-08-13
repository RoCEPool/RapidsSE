/*============================================================================*/
/* Job_Boot.cpp -- System Boot Job                                            */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Boot (
  "Job_Boot"_Stz,    // name of the job class
  CN_JOB_BOOT,       // class enum
  JK_CHILD,          // job kind
  "System Boot"_Stz, // comment about the job
  nullptr,           // the prompt command
  nullptr,           // the second word
  "Boot_seed"_Stz,   // the task seed for this job
  false,             // check resources?
  false,             // detached job?
  JT_NONE,           // job trace
  nullptr,           // prompt pre-process function
  nullptr,           // prompt post-process function
  nullptr,           // job new function
  vJob_Boot_delete,  // job delete function
  nullptr,           // job send function
  nullptr,           // job receive function
  vJob_Boot_show,    // job show function
  false,             // job is not prompt enabled
  "<help>"_Stz,      // help
  "<example>"_Stz    // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Boot* JobsModel::_pJob_Boot(void* aAddress)
{
  Job_Boot* pJob_Boot = reinterpret_cast<Job_Boot*>(aAddress);
  
  RASSERT( !pJob_Boot || CN_JOB_BOOT == pJob_Boot->eClass());
  
  return pJob_Boot;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Boot::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Boot* pJob_Boot =
    static_cast<Job_Boot*>(Heap::pAllocate(CN_JOB_BOOT));
  
  return pJob_Boot;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Boot::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Boot::operator delete(void* aAddress)
{
  Job_Boot* pJob_Boot = static_cast<Job_Boot*>(aAddress);
  
  Heap::vFree(pJob_Boot);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Boot::Job_Boot(Interface::SEitf* pSEitf)
: Job(CN_JOB_BOOT, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_BOOT,_FOOTER_);
  
  m_sSeed = mJobNote_Boot.pJobClass()->m_aSeed;
  
  _checkInitialised_(CN_JOB_BOOT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_BOOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Boot::~Job_Boot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_BOOT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Boot::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_BOOT == eClass);
  RASSERT(pObject->eClass() == CN_JOB_BOOT);
  
  Job_Boot* pJob_Boot = _pJob_Boot(pObject);

  pJob_Boot->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Boot::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Boot::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_BOOT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Boot_new(Job* pArg)
{
  Job_Boot* pJob_Boot = new Job_Boot(nullptr);
  
  return pJob_Boot;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Boot_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Boot* pJob_Boot = _pJob_Boot(pJob);
  
  delete pJob_Boot;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Boot_show(Job* pJob)
{
  Job_Boot* pJob_Boot = _pJob_Boot(pJob);

  pJob_Boot->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
