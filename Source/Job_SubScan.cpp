/*============================================================================*/
/* Job_SubScan.cpp -- Sub Fetch Job                                          */
/*============================================================================*/

// Job Short: SubScan

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_SubScan =
{
  CN_JOB_SUBSCAN,           // class enum
  JK_CHILD,                  // job kind
  RK_NODE_ERRORS,            // response kind
  "sub-fetch rows"_Stz,      // comment about the job
  nullptr,                   // the prompt command
  nullptr,                   // the second word
  "SubFetch_run"_Stz,        // the _run task seed
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
  pJob_SubScan_pre,         // prompt pre-process function
  vJob_post,                 // prompt post-process function
  nullptr,                   // job new function
  vJob_SubScan_delete,      // job delete function
  nullptr,                   // job pre-send function
  nullptr,                   // job post-receive function
  vJob_SubScan_show,        // job show function
  
  // help and example
  false,
  "<none>"_Stz,
  "<none>"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_SubScan_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_SubTruncate_class;
  
  return &xJobClass_SubScan;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubScan* Jobs::_pJob_SubScan(void* aAddress)
{
  Job_SubScan* pJob_SubScan = reinterpret_cast<Job_SubScan*>(aAddress);
  
  RASSERT_TYPE(pJob_SubScan,CN_JOB_SUBSCAN);
  
  return pJob_SubScan;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubScan::operator new(size_t)
{
  Job_SubScan* pJob_SubScan =
    static_cast<Job_SubScan*>(HeapMem::pAllocate(CN_JOB_SUBSCAN));
  
  return pJob_SubScan;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubScan::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubScan::operator delete(void* aAddress)
{
  Job_SubScan* pJob_SubScan = static_cast<Job_SubScan*>(aAddress);
  
  HeapMem::vFree(pJob_SubScan);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubScan::Job_SubScan(Interface::Session* pSession)
: Job(CN_JOB_SUBSCAN, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBSCAN,_FOOTER_);
  
  m_pParentJob     = nullptr;
  m_pBufferQueue   = nullptr;
  
  _checkInitialised_(CN_JOB_SUBSCAN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBSCAN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubScan::~Job_SubScan(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBSCAN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubScan::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_SubScan* pJob_SubScan = _pJob_SubScan(pObject);

  pJob_SubScan->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubScan::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBSCAN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubScan::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBSCAN);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_SubScan_pre(Interface::Session* pSession)
{
  Job_SubScan* pJob_SubScan = new Job_SubScan(pSession);
  
  return pJob_SubScan;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubScan_delete(Interface::Session* pSession, Job* pJob)
{
  Job_SubScan* pJob_SubScan = _pJob_SubScan(pJob);
  
  delete pJob_SubScan;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_SubScan_show(Job* pJob)
{
  Job_SubScan* pJob_SubScan = _pJob_SubScan(pJob);

  pJob_SubScan->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
