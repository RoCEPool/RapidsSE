/*============================================================================*/
/* Job_Start.cpp -- System Start Job                                          */
/*============================================================================*/

// Job Short: Start

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Start =
{
  "Job_Start"_Stz,    // name of the job class
  CN_JOB_START,       // class enum
  JK_SYSTEM,          // job kind
  "system start"_Stz, // comment about the job
  nullptr,            // the prompt command
  nullptr,            // the second word
  "start_run"_Stz,    // the _run task seed
  nullptr,            // the _req task seed
  nullptr,            // the _rcv task seed
  nullptr,            // the _exe task seed
  nullptr,            // the _end task seed
  nullptr,            // the _can task seed
  false,              // check resources?
  false,              // detached job?
  false,              // buffer attached?
  JT_NONE,            // job trace
  
  eJob_checkCluster,  // prompt check function
  nullptr,            // prompt pre-process function
  nullptr,            // prompt post-process function
  pJob_Start_new,     // job new function
  vJob_Start_delete,  // job delete function
  nullptr,            // job pre-send function
  nullptr,            // job post-receive function
  vJob_Start_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Start_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_StartCluster_class;
  
  return &xJobClass_Start;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Start* Jobs::_pJob_Start(void* aAddress)
{
  Job_Start* pJob_Start = reinterpret_cast<Job_Start*>(aAddress);
  
  RASSERT_TYPE(pJob_Start,CN_JOB_START);
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Start::operator new(size_t)
{
  Job_Start* pJob_Start =
    static_cast<Job_Start*>(HeapMem::pAllocate(CN_JOB_START));
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Start::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Start::operator delete(void* aAddress)
{
  Job_Start* pJob_Start = static_cast<Job_Start*>(aAddress);
  
  HeapMem::vFree(pJob_Start);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Start::Job_Start(Interface::Session* pSession)
: Job(CN_JOB_START, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
  
  _checkInitialised_(CN_JOB_START);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_START,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Start::~Job_Start(void) noexcept(false)
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
Job* Jobs::pJob_Start_new(Job* pArg)
{
  Job_Start* pJob_Start = new Job_Start(nullptr);
  
  return pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Start_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Start* pJob_Start = _pJob_Start(pJob);
  
  delete pJob_Start;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Start_show(Job* pJob)
{
  Job_Start* pJob_Start = _pJob_Start(pJob);

  pJob_Start->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
