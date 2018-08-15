/*============================================================================*/
/* Job_Start.cpp -- System Start Job                                          */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Start (
  "Job_Start"_Stz,    // name of the job class
  CN_JOB_START,       // class enum
  JK_CHILD,           // job kind
  "System Start"_Stz, // comment about the job
  nullptr,            // the prompt command
  nullptr,            // the second word
  "Start_seed"_Stz,   // the task seed for this job
  false,              // check resources?
  false,              // detached job?
  JT_NONE,            // job trace
  nullptr,            // prompt pre-process function
  nullptr,            // prompt post-process function
  pJob_Start_new,     // job new function
  vJob_Start_delete,  // job delete function
  nullptr,            // job send function
  nullptr,            // job receive function
  vJob_Start_show,    // job show function
  false,              // job is not prompt enabled
  "<help>"_Stz,       // help
  "<example>"_Stz     // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Start* JobsModel::_pJob_Start(void* aAddress)
{
  Job_Start* pJob_Start = reinterpret_cast<Job_Start*>(aAddress);
  
  RASSERT( !pJob_Start || CN_JOB_START == pJob_Start->eClass());
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Start::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Start* pJob_Start =
    static_cast<Job_Start*>(Heap::pAllocate(CN_JOB_START));
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Start::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Start::operator delete(void* aAddress)
{
  Job_Start* pJob_Start = static_cast<Job_Start*>(aAddress);
  
  Heap::vFree(pJob_Start);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Start::Job_Start(Interface::SEitf* pSEitf)
: Job(CN_JOB_START, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
  
  m_sSeed = mJobNote_Start.pJobClass()->m_aSeed;
  
  _checkInitialised_(CN_JOB_START);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Start::~Job_Start(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Start::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_START == eClass);
  RASSERT(pObject->eClass() == CN_JOB_START);
  
  Job_Start* pJob_Start = _pJob_Start(pObject);

  pJob_Start->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Start::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Start::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_START);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Start_new(Job* pArg)
{
  Job_Start* pJob_Start = new Job_Start(nullptr);
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Start_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Start* pJob_Start = _pJob_Start(pJob);
  
  delete pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Start_show(Job* pJob)
{
  Job_Start* pJob_Start = _pJob_Start(pJob);

  pJob_Start->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
