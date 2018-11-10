/*============================================================================*/
/* Job_NetHog.cpp -- Network Hog Job                                          */
/*============================================================================*/

// Job Short: NetHog

#include "JobsModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_NetHog =
{
  "Job_NetHog"_Stz,      // name of the job class
  CN_JOB_NETHOG,         // class enum
  JK_LOCAL,              // job kind
  "load table"_Stz,      // comment about the job
  "ld"_Stz,              // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "netHog_req"_Stz,      // the _req task seed
  "netHog_rcv"_Stz,      // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  JT_NONE,               // job trace
  pJob_NetHog_pre,       // prompt pre-process function
  nullptr,               // prompt post-process function
  pJob_NetHog_new,       // job new function
  vJob_NetHog_delete,    // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_NetHog_show,      // job show function
  
  // help and example
  true,
  "nethog <nHogs>"_Stz,
  "nethog 73;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

i8 giHogNo = 0;

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobsModel::pJob_NetHog_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_NodeInfo_class;
  
  return &xJobClass_NetHog;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_NetHog* JobsModel::_pJob_NetHog(void* aAddress)
{
  Job_NetHog* pJob_NetHog = reinterpret_cast<Job_NetHog*>(aAddress);
  
  RASSERT_TYPE(pJob_NetHog,CN_JOB_NETHOG);
  
  return pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_NetHog::operator new(size_t)
{
  Job_NetHog* pJob_NetHog =
    static_cast<Job_NetHog*>(HeapMem::pAllocate(CN_JOB_NETHOG));
  
  return pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_NetHog::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_NetHog::operator delete(void* aAddress)
{
  Job_NetHog* pJob_NetHog = static_cast<Job_NetHog*>(aAddress);
  
  HeapMem::vFree(pJob_NetHog);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_NetHog::Job_NetHog(Interface::Session* pSession)
: Job(CN_JOB_NETHOG, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
  m_iHogNo = -1;
  
  _checkInitialised_(CN_JOB_NETHOG);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_NetHog::~Job_NetHog(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_NetHog::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pObject);

  pJob_NetHog->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_NetHog::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_NetHog::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_NETHOG);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_NetHog_pre(Interface::Session* pSession)
{
  Job_NetHog* pJob_NetHog = new Job_NetHog(pSession);
  
  return pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_NetHog_post(Interface::Session* pSession, Job* pJob)
{
}

/*----------------------------------------------------------------------------*/
/* job new function                                                           */
/*----------------------------------------------------------------------------*/
Job* JobsModel::pJob_NetHog_new(Job* pArg)
{
  Job_NetHog* pJob_NetHog = new Job_NetHog(nullptr);
  
  return pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_NetHog_delete(Interface::Session* pSession, Job* pJob)
{
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pJob);
  
  delete pJob_NetHog;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_NetHog_show(Job* pJob)
{
  Job_NetHog* pJob_NetHog = _pJob_NetHog(pJob);

  pJob_NetHog->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
