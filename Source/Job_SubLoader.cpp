/*============================================================================*/
/* Job_SubLoader.cpp -- Table SubLoader Job                                   */
/*============================================================================*/

#include "JobModel.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

JobNote mJobNote_SubLoader (
  "Job_SubLoader"_Stz,    // name of the job class
  CN_JOB_SUBLOADER,       // class enum
  JK_CHILD,               // job kind
  "table subloader"_Stz,  // comment about the job
  nullptr,                // the prompt command
  nullptr,                // the second word
  "SubLoader_seed"_Stz,   // the task seed for this job
  nullptr,                // the second task seed for this job
  false,                  // check resources?
  false,                  // detached job?
  false,                  // buffer attached?
  JT_NONE,                // job trace
  vJob_SubLoader_pre,     // prompt pre-process function
  nullptr,                // prompt post-process function
  nullptr,                // job new function
  vJob_SubLoader_delete,  // job delete function
  nullptr,                // job pre-send function
  nullptr,                // job post-receive function
  vJob_SubLoader_show,    // job show function
  false,                  // job is not prompt enabled
  "<help>"_Stz,           // help
  "<example>"_Stz         // example
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_SubLoader* JobsModel::_pJob_SubLoader(void* aAddress)
{
  Job_SubLoader* pJob_SubLoader = reinterpret_cast<Job_SubLoader*>(aAddress);
  
  RASSERT( !pJob_SubLoader || CN_JOB_SUBLOADER == pJob_SubLoader->eClass());
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* Job_SubLoader::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Job_SubLoader* pJob_SubLoader =
    static_cast<Job_SubLoader*>(Heap::pAllocate(CN_JOB_SUBLOADER));
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* Job_SubLoader::operator new(size_t iSize, void* aAddress)
{
  RASSERT(iSize);
  
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::operator delete(void* aAddress)
{
  Job_SubLoader* pJob_SubLoader = static_cast<Job_SubLoader*>(aAddress);
  
  Heap::vFree(pJob_SubLoader);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_SubLoader::Job_SubLoader(Interface::SEitf* pSEitf)
: Job(CN_JOB_SUBLOADER, pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  m_sSeed = mJobNote_SubLoader.pJobClass()->m_aSeed1;
  
  m_pJob_LoadTable = nullptr;
  m_pLoaderSub = nullptr;
    
  _checkInitialised_(CN_JOB_SUBLOADER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_SubLoader::~Job_SubLoader(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(CN_JOB_SUBLOADER == eClass);
  RASSERT(pObject->eClass() == CN_JOB_SUBLOADER);
  
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pObject);

  pJob_SubLoader->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get job_loadtable                                                          */
/*----------------------------------------------------------------------------*/
Job_LoadTable* Job_SubLoader::pJob_LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  return m_pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* set job_loadtable                                                          */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vJob_LoadTable(Job_LoadTable* pJob_LoadTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  m_pJob_LoadTable = pJob_LoadTable;
}

/*----------------------------------------------------------------------------*/
/* set loadersub                                                              */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vLoaderSub(LoaderSub* pLoaderSub)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
  
  m_pLoaderSub = pLoaderSub;
  m_pAncillary = m_pLoaderSub;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_SubLoader::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_SubLoader::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_SUBLOADER);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* JobsModel::vJob_SubLoader_pre(Interface::SEitf* pSEitf)
{
  Job_SubLoader* pJob_SubLoader = new Job_SubLoader(pSEitf);
  
  return pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SubLoader_delete(Interface::SEitf* pSEitf, Job* pJob)
{
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pJob);
  
  delete pJob_SubLoader;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void JobsModel::vJob_SubLoader_show(Job* pJob)
{
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pJob);

  pJob_SubLoader->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
