/*============================================================================*/
/* Job_Answer.cpp -- Socket Answer Job                                        */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_Answer (
  "Job_Answer"_Stz,    // name of the job class
  CN_JOB_ANSWER,       // class enum
  JK_CHILD,            // job kind
  "Socket Answer"_Stz, // comment about the job
  nullptr,             // the prompt command
  nullptr,             // the second word
  "Answer_seed"_Stz,   // the task seed for this job
  false,               // check resources?
  false,               // detached job?
  JT_NONE,             // job trace
  nullptr,             // prompt pre-process function
  nullptr,             // prompt post-process function
  nullptr,             // job new function
  vJob_Answer_delete,  // job delete function
  nullptr,             // job receive function
  vJob_Answer_show,    // job show function
  false,               // job is not prompt enabled
  "<help>"_Stz,        // help
  "<example>"_Stz      // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Answer* JobsModel::_pJob_Answer(void* aAddress)
{
  Job_Answer* pJob_Answer = reinterpret_cast<Job_Answer*>(aAddress);
  
  RASSERT( !pJob_Answer || CN_JOB_ANSWER == pJob_Answer->eClass());
  
  return pJob_Answer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Answer::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Answer* pJob_Answer =
    static_cast<Job_Answer*>(Heap::pAllocate(CN_JOB_ANSWER));
  
  return pJob_Answer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Answer::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Answer::operator delete(void* aAddress)
{
  Job_Answer* pJob_Answer = static_cast<Job_Answer*>(aAddress);
  
  Heap::vFree(pJob_Answer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Answer::Job_Answer(Interface::SEitf* pSEitf)
: Job(CN_JOB_ANSWER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANSWER,_FOOTER_);
  
  m_sSeed = mJobNote_Answer.pJobClass()->m_aSeed;
  
  _checkInitialised_(CN_JOB_ANSWER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANSWER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Answer::~Job_Answer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANSWER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Answer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_ANSWER == eClass);
  RASSERT(pObject->eClass() == CN_JOB_ANSWER);
  
  Job_Answer* pJob_Answer = _pJob_Answer(pObject);

  pJob_Answer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Answer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Answer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANSWER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Answer_new(Job* pArg)
{
  Job_Answer* pJob_Answer = new Job_Answer(nullptr);
  
  return pJob_Answer;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Answer_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_Answer* pJob_Answer = _pJob_Answer(pJob);
  
  delete pJob_Answer;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Answer_show(Job* pJob)
{
  Job_Answer* pJob_Answer = _pJob_Answer(pJob);

  pJob_Answer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
