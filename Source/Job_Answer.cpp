/*============================================================================*/
/* Job_Answer.cpp -- Socket Answer Job                                        */
/*============================================================================*/

// Job Short: Answer

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Answer =
{
  "Job_Answer"_Stz,    // name of the job class
  CN_JOB_ANSWER,       // class enum
  JK_SYSTEM,           // job kind
  "socket answer"_Stz, // comment about the job
  nullptr,             // the prompt command
  nullptr,             // the second word
  "answer_run"_Stz,    // the _run task seed
  nullptr,             // the _req task seed
  nullptr,             // the _rcv task seed
  nullptr,             // the _exe task seed
  nullptr,             // the _end task seed
  nullptr,             // the _can task seed
  false,               // check resources?
  false,               // detached job?
  false,               // buffer attached?
  JT_NONE,             // job trace
  
  eJob_checkCluster,   // prompt check function
  nullptr,             // prompt pre-process function
  nullptr,             // prompt post-process function
  pJob_Answer_new,     // job new function
  vJob_Answer_delete,  // job delete function
  nullptr,             // job pre-send function
  nullptr,             // job post-receive function
  vJob_Answer_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Answer_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Boot_class;
  
  return &xJobClass_Answer;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Answer* Jobs::_pJob_Answer(void* aAddress)
{
  Job_Answer* pJob_Answer = reinterpret_cast<Job_Answer*>(aAddress);
  
  RASSERT_TYPE(pJob_Answer,CN_JOB_ANSWER);
  
  return pJob_Answer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Answer::operator new(size_t)
{
  Job_Answer* pJob_Answer =
    static_cast<Job_Answer*>(HeapMem::pAllocate(CN_JOB_ANSWER));
  
  return pJob_Answer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Answer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Answer::operator delete(void* aAddress)
{
  Job_Answer* pJob_Answer = static_cast<Job_Answer*>(aAddress);
  
  HeapMem::vFree(pJob_Answer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Answer::Job_Answer(Interface::Session* pSession)
: Job(CN_JOB_ANSWER, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANSWER,_FOOTER_);
    
  _checkInitialised_(CN_JOB_ANSWER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_ANSWER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Answer::~Job_Answer(void) noexcept(false)
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
Job* Jobs::pJob_Answer_new(Job* pArg)
{
  Job_Answer* pJob_Answer = new Job_Answer(nullptr);
  
  return pJob_Answer;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Answer_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Answer* pJob_Answer = _pJob_Answer(pJob);
  
  delete pJob_Answer;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Answer_show(Job* pJob)
{
  Job_Answer* pJob_Answer = _pJob_Answer(pJob);

  pJob_Answer->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
