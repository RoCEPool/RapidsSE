/*============================================================================*/
/* Job_Periodic.cpp -- Periodic Job                                           */
/*============================================================================*/

// Job Short: Periodic

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Periodic =
{
  "Job_Periodic"_Stz,    // name of the job class
  CN_JOB_PERIODIC,            // class enum
  JK_SYSTEM,             // job kind
  "periodic"_Stz,        // comment about the job
  nullptr,               // the prompt command
  nullptr,               // the second word
  "periodic_run"_Stz,    // the _run task seed
  nullptr,               // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  DL_NONE,               // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  nullptr,               // prompt pre-process function
  nullptr,               // prompt post-process function
  pJob_Periodic_new,     // job new function
  vJob_Periodic_delete,  // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Periodic_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Periodic_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Ping_class;
  
  return &xJobClass_Periodic;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Periodic* Jobs::_pJob_Periodic(void* aAddress)
{
  Job_Periodic* pJob_Periodic = reinterpret_cast<Job_Periodic*>(aAddress);
  
  RASSERT_TYPE(pJob_Periodic,CN_JOB_PERIODIC);
  
  return pJob_Periodic;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Periodic::operator new(size_t)
{
  Job_Periodic* pJob_Periodic =
    static_cast<Job_Periodic*>(HeapMem::pAllocate(CN_JOB_PERIODIC));
  
  return pJob_Periodic;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Periodic::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Periodic::operator delete(void* aAddress)
{
  Job_Periodic* pJob_Periodic = static_cast<Job_Periodic*>(aAddress);
  
  HeapMem::vFree(pJob_Periodic);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Periodic::Job_Periodic(Interface::Session* pSession)
: Job(CN_JOB_PERIODIC, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PERIODIC,_FOOTER_);
  
   _checkInitialised_(CN_JOB_PERIODIC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PERIODIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Periodic::~Job_Periodic(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PERIODIC,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Periodic::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Periodic* pJob_Periodic = _pJob_Periodic(pObject);

  pJob_Periodic->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Periodic::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PERIODIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Periodic::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PERIODIC);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Periodic_new(Job* pArg)
{
  Job_Periodic* pJob_Periodic = new Job_Periodic(nullptr);
  
  return pJob_Periodic;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Periodic_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Periodic* pJob_Periodic = _pJob_Periodic(pJob);
  
  delete pJob_Periodic;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Periodic_show(Job* pJob)
{
  Job_Periodic* pJob_Periodic = _pJob_Periodic(pJob);

  pJob_Periodic->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
