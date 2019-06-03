/*============================================================================*/
/* Job_SubInflate.cpp -- Sub Inflate Job                                      */
/*============================================================================*/

// Job Short: SubInflate

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubInflate =
{
  CN_JOB_SUBINFLATE,         // class enum
  JK_CHILD,                  // job kind
  RK_NODE_ERRORS,            // response kind
  "sub-inflate rows"_Stz,    // comment about the job
  nullptr,                   // the prompt command
  nullptr,                   // the second word
  "SubInflate_run"_Stz,      // the _run task seed
  nullptr,                   // the _req task seed
  nullptr,                   // the _rcv task seed
  nullptr,                   // the _exe task seed
  nullptr,                   // the _end task seed
  nullptr,                   // the _can task seed
  false,                     // check resources?
  false,                     // detached job?
  false,                     // buffer attached?
  JT_NONE,                   // job trace
  DL_GRAB_READ,              // ddl lock model
  PY_NORMAL,                 // priority
  JF_NONE,                   // flags

  nullptr,                   // prompt check function
  pJob_SubInflate_pre,       // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_SubInflate_delete,    // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubInflate_show,      // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SubInflate_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubIngester_class;
  
  return &xJobClass_SubInflate;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubInflate* Jobs::_pJob_SubInflate(void* aAddress)
{
  Job_SubInflate* pJob_SubInflate = reinterpret_cast<Job_SubInflate*>(aAddress);
  
  RASSERT_TYPE(pJob_SubInflate,CN_JOB_SUBINFLATE);
  
  return pJob_SubInflate;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubInflate::operator new(size_t)
{
  Job_SubInflate* pJob_SubInflate =
    static_cast<Job_SubInflate*>(HeapMem::pAllocate(CN_JOB_SUBINFLATE));
  
  return pJob_SubInflate;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubInflate::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubInflate::operator delete(void* aAddress)
{
  Job_SubInflate* pJob_SubInflate = static_cast<Job_SubInflate*>(aAddress);
  
  HeapMem::vFree(pJob_SubInflate);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubInflate::Job_SubInflate(Interface::Session* pSession)
: Job(CN_JOB_SUBINFLATE, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINFLATE,_FOOTER_);
    
  m_pBufferQueue   = nullptr;
  m_pPlanner       = nullptr;
  m_pJobFetchRows  = nullptr;
  m_iPhrase        = -1;
  
  _checkInitialised_(CN_JOB_SubInflate);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINFLATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubInflate::~Job_SubInflate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SubInflate,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubInflate::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubInflate* pJob_SubInflate = _pJob_SubInflate(pObject);

  pJob_SubInflate->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubInflate::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINFLATE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubInflate::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBINFLATE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SubInflate_pre(Interface::Session* pSession)
{
  Job_SubInflate* pJob_SubInflate = new Job_SubInflate(pSession);
  
  return pJob_SubInflate;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubInflate_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubInflate* pJob_SubInflate = _pJob_SubInflate(pJob);
  
  delete pJob_SubInflate;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubInflate_show(Job* pJob)
{
  Job_SubInflate* pJob_SubInflate = _pJob_SubInflate(pJob);

  pJob_SubInflate->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
