/*============================================================================*/
/* Job_Prompt.cpp -- Prompt Job                                               */
/*============================================================================*/

// Job Short: Prompt

#include "JobsModel.h"

JobClass xJobClass_Prompt =
{
  "Job_Prompt"_Stz,  // name of the job class
  CN_JOB_PROMPT,     // class enum
  JK_THREAD,         // job kind
  "SE prompt"_Stz,   // comment about the job
  nullptr,           // the prompt command
  nullptr,           // the second word
  "prompt_run"_Stz,  // the _run task seed
  nullptr,           // the _req task seed
  nullptr,           // the _exe task seed
  nullptr,           // the _rcv task seed
  nullptr,           // the _end task seed
  nullptr,           // the _can task seed
  false,             // check resources?
  true,              // detached job?
  false,             // buffer attached?
  JT_NONE,           // job trace
  nullptr,           // prompt pre-process function
  nullptr,           // prompt post-process function
  pJob_Prompt_new,   // job new function
  vJob_Prompt_delete,// job delete function
  nullptr,           // job pre-send function
  nullptr,           // job post-receive function
  vJob_Prompt_show,  // job show function
  false,             // job is not prompt enabled
  "<help>"_Stz,      // help
  "<example>"_Stz,   // example

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_Prompt_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Pwf_class;
  
  return &xJobClass_Prompt;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Prompt* JobsModel::_pJob_Prompt(void* aAddress)
{
  Job_Prompt* pJob_Prompt = reinterpret_cast<Job_Prompt*>(aAddress);
  
  //RASSERT( !pJob_Prompt || CN_JOB_PROMPT == Job_Space->eClass());
  
  return pJob_Prompt;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Prompt::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_Prompt* pJob_Prompt =
    static_cast<Job_Prompt*>(HeapMem::pAllocate(CN_JOB_PROMPT));
  
  return pJob_Prompt;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Prompt::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Prompt::operator delete(void* aAddress)
{
  Job_Prompt* pJob_Prompt = static_cast<Job_Prompt*>(aAddress);
  
  HeapMem::vFree(pJob_Prompt);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Prompt::Job_Prompt(Interface::Session* pSession)
: Job(CN_JOB_PROMPT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PROMPT,_FOOTER_);
    
  _checkInitialised_(CN_JOB_PROMPT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PROMPT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Prompt::~Job_Prompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PROMPT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Prompt::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_PROMPT == eClass);
  RASSERT(pObject->eClass() == CN_JOB_PROMPT);
  
  Job_Prompt* pJob_Prompt = _pJob_Prompt(pObject);

  pJob_Prompt->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Prompt::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PROMPT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Prompt::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PROMPT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_Prompt_new(Job* pArg)
{
  Job_Prompt* pJob_Prompt = new Job_Prompt(nullptr);
  
  return pJob_Prompt;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Prompt_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Prompt* pJob_Prompt = _pJob_Prompt(pJob);
  
  delete pJob_Prompt;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_Prompt_show(Job* pJob)
{
  Job_Prompt* pJob_Prompt = _pJob_Prompt(pJob);
  
  pJob_Prompt->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
