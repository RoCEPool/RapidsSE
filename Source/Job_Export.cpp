/*============================================================================*/
/* Job_Export.cpp -- Export Table Job                                         */
/*============================================================================*/

// Job Short: Export

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobClass xJobClass_Export =
{
  "Job_Export"_Stz,       // name of the job class
  CN_JOB_EXPORT,          // class enum
  JK_LOCAL,               // job kind
  "export table"_Stz,     // comment about the job
  "export"_Stz,           // the prompt command
  nullptr,                // the second word
  nullptr,                // the _run task seed
  "export_req"_Stz,       // the _req task seed
  nullptr,                // the _rcv task seed
  nullptr,                // the _exe task seed
  nullptr,                // the _end task seed
  nullptr,                // the _can task seed
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  pJob_Export_pre,        // prompt pre-process function
  vJob_Export_post,       // prompt post-process function
  nullptr,                // job new function
  vJob_Export_delete,     // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_Export_show,       // job show function
  
  // help and example
  true,
  "export <tableName> <fileName>"_Stz,
  "export SUPPLIER /tmp/supplier.csv;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* JobModel::pJob_Export_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_FetchRows_class;
  
  return &xJobClass_Export;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Export* JobModel::_pJob_Export(void* aAddress)
{
  Job_Export* pJob_Export = reinterpret_cast<Job_Export*>(aAddress);
  
  RASSERT_TYPE(pJob_Export,CN_JOB_EXPORT);
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_Export::operator new(size_t)
{
  Job_Export* pJob_Export =
    static_cast<Job_Export*>(HeapMem::pAllocate(CN_JOB_EXPORT));
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_Export::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_Export::operator delete(void* aAddress)
{
  Job_Export* pJob_Export = static_cast<Job_Export*>(aAddress);
  
  HeapMem::vFree(pJob_Export);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Export::Job_Export(Interface::Session* pSession)
: Job(CN_JOB_EXPORT, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
  
  _checkInitialised_(CN_JOB_EXPORT);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Export::~Job_Export(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Export::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Export* pJob_Export = _pJob_Export(pObject);

  pJob_Export->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Export::bPopulateJob(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Export::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Export::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_EXPORT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobModel::pJob_Export_pre(Interface::Session* pSession)
{
  Job_Export* pJob_Export = new Job_Export(pSession);
  
  return pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Export_post(Interface::Session* pSession, Job* pJob)
{
  //Job_Export* pJob_Export = _pJob_Export(pJob);
  
  vTrace("Warning: export not yet supported\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Export_delete(Interface::Session* pSession, Job* pJob)
{
  Job_Export* pJob_Export = _pJob_Export(pJob);
  
  // now delete the job
  delete pJob_Export;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobModel::vJob_Export_show(Job* pJob)
{
  Job_Export* pJob_Export = _pJob_Export(pJob);

  pJob_Export->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
