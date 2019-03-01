/*============================================================================*/
/* Job_Boot.cpp -- System Boot Job                                            */
/*============================================================================*/

// Job Short: Boot

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Boot =
{
  "Job_Boot"_Stz,    // name of the job class
  CN_JOB_BOOT,       // class enum
  JK_THREAD,         // job kind
  "system boot"_Stz, // comment about the job
  nullptr,           // the prompt command
  nullptr,           // the second word
  "boot_run"_Stz,    // the _run task seed
  nullptr,           // the _req task seed
  nullptr,           // the _exe task seed
  nullptr,           // the _rcv task seed
  nullptr,           // the _end task seed
  nullptr,           // the _can task seed
  false,             // check resources?
  false,             // detached job?
  false,             // buffer attached?
  JT_NONE,           // job trace
  DL_NONE,           // ddl lock model
  PY_NORMAL,         // priority
  JF_NONE,           // flags

  nullptr,           // prompt check function
  nullptr,           // prompt pre-process function
  nullptr,           // prompt post-process function
  pJob_Boot_new,     // job new function
  vJob_Boot_delete,  // job delete function
  nullptr,           // job pre-send function
  nullptr,           // job post-receive function
  vJob_Boot_show,    // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Boot_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Cat_class;
  
  return &xJobClass_Boot;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Boot* Jobs::_pJob_Boot(void* aAddress)
{
  Job_Boot* pJob_Boot = reinterpret_cast<Job_Boot*>(aAddress);
  
  RASSERT_TYPE(pJob_Boot,CN_JOB_BOOT);
  
  return pJob_Boot;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Boot::operator new(size_t)
{
  Job_Boot* pJob_Boot =
    static_cast<Job_Boot*>(HeapMem::pAllocate(CN_JOB_BOOT));
  
  return pJob_Boot;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Boot::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Boot::operator delete(void* aAddress)
{
  Job_Boot* pJob_Boot = static_cast<Job_Boot*>(aAddress);
  
  HeapMem::vFree(pJob_Boot);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Boot::Job_Boot(Interface::Session* pSession)
: Job(CN_JOB_BOOT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_BOOT,_FOOTER_);
    
  _checkInitialised_(CN_JOB_BOOT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_BOOT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Boot::~Job_Boot(void) noexcept(false)
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
Job* Jobs::pJob_Boot_new(Job* pArg)
{
  Job_Boot* pJob_Boot = new Job_Boot(nullptr);
  
  return pJob_Boot;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Boot_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Boot* pJob_Boot = _pJob_Boot(pJob);
  
  delete pJob_Boot;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Boot_show(Job* pJob)
{
  Job_Boot* pJob_Boot = _pJob_Boot(pJob);

  pJob_Boot->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
