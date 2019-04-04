/*============================================================================*/
/* Job_Supervisor.cpp -- Supervisor Job                                           */
/*============================================================================*/

// Job Short: Supervisor

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Supervisor =
{
  CN_JOB_SUPERVISOR,     // class enum
  JK_SYSTEM,             // job kind
  RK_NONE,               // response kind
  "supervisor"_Stz,      // comment about the job
  nullptr,               // the prompt command
  nullptr,               // the second word
  "supervisor_run"_Stz,  // the _run task seed
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
  pJob_Supervisor_new,   // job new function
  vJob_Supervisor_delete,// job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Supervisor_show,  // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Supervisor_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_TableSizes_class;
  
  return &xJobClass_Supervisor;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Supervisor* Jobs::_pJob_Supervisor(void* aAddress)
{
  Job_Supervisor* pJob_Supervisor = reinterpret_cast<Job_Supervisor*>(aAddress);
  
  RASSERT_TYPE(pJob_Supervisor,CN_JOB_SUPERVISOR);
  
  return pJob_Supervisor;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Supervisor::operator new(size_t)
{
  Job_Supervisor* pJob_Supervisor =
    static_cast<Job_Supervisor*>(HeapMem::pAllocate(CN_JOB_SUPERVISOR));
  
  return pJob_Supervisor;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Supervisor::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Supervisor::operator delete(void* aAddress)
{
  Job_Supervisor* pJob_Supervisor = static_cast<Job_Supervisor*>(aAddress);
  
  HeapMem::vFree(pJob_Supervisor);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Supervisor::Job_Supervisor(Interface::Session* pSession)
: Job(CN_JOB_SUPERVISOR, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUPERVISOR,_FOOTER_);
  
   _checkInitialised_(CN_JOB_SUPERVISOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUPERVISOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Supervisor::~Job_Supervisor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUPERVISOR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Supervisor::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Supervisor* pJob_Supervisor = _pJob_Supervisor(pObject);

  pJob_Supervisor->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Supervisor::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUPERVISOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Supervisor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUPERVISOR);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Supervisor_new(Job* pArg)
{
  Job_Supervisor* pJob_Supervisor = new Job_Supervisor(nullptr);
  
  return pJob_Supervisor;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Supervisor_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Supervisor* pJob_Supervisor = _pJob_Supervisor(pJob);
  
  delete pJob_Supervisor;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Supervisor_show(Job* pJob)
{
  Job_Supervisor* pJob_Supervisor = _pJob_Supervisor(pJob);

  pJob_Supervisor->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
